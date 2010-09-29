/***************************************************************************
 *
 * Copyright (C) 2006 Elad Lahav (elad_lahav@users.sourceforge.net)
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 ***************************************************************************/

#include <ktextbrowser.h>
#include <kcombobox.h>
#include <kurlrequester.h>
#include <kmessagebox.h>
#include <klocale.h>
#include "makedlg.h"
#include "makefrontend.h"
#include "queryview.h"

/** Window flags for call-tree widgets. */
#define MAKE_DLG_W_FLAGS \
	WStyle_Customize | \
	WStyle_NormalBorder | \
	WStyle_Title

/**
 * Class constructor.
 * @param	pParent	The parent widget
 * @param	szName	The widget's name
 */
MakeDlg::MakeDlg(QWidget* pParent, const char* szName) :
	MakeLayout(pParent, szName, MAKE_DLG_W_FLAGS)
{
	// Don't show the "Function" column
	m_pErrorView->setColumnWidth(0, 0);
	
	// Create a new make front-end
	m_pMake = new MakeFrontend();
	connect(m_pMake, SIGNAL(dataReady(FrontendToken*)), this,
		SLOT(slotShowOutput(FrontendToken*)));
	connect(m_pMake, SIGNAL(finished(uint)), this, SLOT(slotFinished(uint)));
	connect(m_pMake,
		SIGNAL(error(const QString&, const QString&, const QString&)),
		this,
		SLOT(slotAddError(const QString&, const QString&, const QString&)));
	
	// The Root URL control should browse directories
	m_pRootURL->setMode(KFile::Directory);
	
	// Handle URL links in the browser
	m_pOutputBrowser->setNotifyClick(true);
	connect(m_pOutputBrowser, SIGNAL(urlClick(const QString&)), this,
		SLOT(slotBrowserClicked(const QString&)));
		
	// Handle selections in the error view
	connect(m_pErrorView, SIGNAL(lineRequested(const QString& , uint)), this,
		SIGNAL(fileRequested(const QString&, uint)));
		
	// Do not allow duplicates in the command history
	m_pCommandHistory->setDuplicatesEnabled(false);
}

/**
 * Class destructor.
 */
MakeDlg::~ MakeDlg()
{
	delete m_pMake;
}

/**
 * @return	The currently set make command
 */
QString MakeDlg::getCommand() const
{
	return m_pCommandHistory->currentText();
}

/**
 * @param	sCmd	The new make command to use
 */
void MakeDlg::setCommand(const QString& sCmd)
{
	m_pCommandHistory->setCurrentText(sCmd);
	m_pCommandHistory->addToHistory(sCmd);
}

/**
 * @return	The directory in which to run the make command
 */
QString MakeDlg::getDir() const
{
	return m_pRootURL->url();
}

/**
 * @param	sURL	The new root directory to use
 */
void MakeDlg::setDir(const QString& sURL)
{
	m_pRootURL->setURL(sURL);
}

/**
 * Overrides the default close behaviour.
 * Makes sure that a window is not closed while a make process is running,
 * unless the user explicitly requests it. In this case, the make process
 * is killed.
 * @param	pEvent	The close event descriptor
 */
void MakeDlg::closeEvent(QCloseEvent* pEvent)
{
	// Check if a process is currently running
	if (m_pMake->isRunning()) {
		// Prompt the user
		switch (KMessageBox::questionYesNoCancel(this, 
			i18n("A make process is running. Would you like to stop it first?"),
			i18n("Close Make Window"))) {
		case KMessageBox::Yes:
			// Stop the process first
			m_pMake->kill();
			break;
				
		case KMessageBox::No:
			// Do nothing
			break;
				
		case KMessageBox::Cancel:
			// Abort closing
			pEvent->ignore();
			return;
		}
	}
	
	QWidget::closeEvent(pEvent);
}

/**
 * Starts a make process using the user-supplied command.
 * This slot is connected to the clicked() signal of the "Make" button.
 */
void MakeDlg::slotMake()
{
	QString sCommand;
	
	// Clear the current contents
	m_pOutputBrowser->clear();
	m_pErrorView->clear();
	
	// Run the make command
	sCommand = m_pCommandHistory->currentText();
	if (!m_pMake->run("make", QStringList::split(" ", sCommand),
		m_pRootURL->url())) {
		KMessageBox::error(this, m_pMake->getRunError());
		return;
	}
		
	// Add the command to the command history
	m_pCommandHistory->addToHistory(sCommand);
		
	// Disbale the make button
	m_pMakeButton->setEnabled(false);
	m_pStopButton->setEnabled(true);
}

/**
 * Terminates the current make process.
 * This slot is connected to the clicked() signal of the stop button.
 */
void MakeDlg::slotStop()
{
	m_pMake->kill();
}

/**
 * Displays the parsed output, as generated by the MakeFrontend object.
 * This slot is connected to the dataReady() signal of the make front-end.
 * @param	pToken	Holds the parsed data
 */
void MakeDlg::slotShowOutput(FrontendToken* pToken)
{
	m_pOutputBrowser->append(pToken->getData());
}

/**
 * Displays the results of the make command.
 * This slot is connected to the finished() signal of the make front-end.
 */
void MakeDlg::slotFinished(uint)
{
	// Add "Success" or "Error" at the end of the output
	if (m_pMake->exitStatus() == 0) {
		m_pOutputBrowser->append("<font color=\"#008000\"><b>Success</b>"
			"</font>");
	}
	else {
		m_pOutputBrowser->append("<font color=\"#ff0000\"><b>Error</b></font>");
	}
	
	// Re-enable the "Make" button
	m_pMakeButton->setEnabled(true);
	m_pStopButton->setEnabled(false);
}

/**
 * Emits the fileRequested() signal when a browser link is clicked.
 * This slot is connected to the urlClick() signal of the browser.
 * @param	sURL	The requested URL
 */
void MakeDlg::slotBrowserClicked(const QString& sURL)
{
	QString sFile;
	QString sLine;
	
	// Exract the file name and the line number from the URL
	sFile = sURL.section('&', 0, 0);
	sLine = sURL.section('&', 1, 1);
	
	// Add root path for relative paths
	if (!sFile.startsWith("/"))
		sFile = m_pRootURL->url() + "/" + sFile;
	
	// Emit the signal
	emit fileRequested(sFile, sLine.toUInt());
}

void MakeDlg::slotAddError(const QString& sFile, const QString& sLine,
	const QString& sText)
{
	m_pErrorView->addRecord("", sFile, sLine, sText);
}
	
#include "makedlg.moc"
