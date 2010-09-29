/***************************************************************************
 *
 * Copyright (C) 2005 Elad Lahav (elad_lahav@users.sourceforge.net)
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

#include <qregexp.h>
#include <qpushbutton.h>
#include <qspinbox.h>
#include <qlabel.h>
#include <kurlrequester.h>
#include <klineedit.h>
#include <kmessagebox.h>
#include <klocale.h>
#include "newprojectdlg.h"

/**
 * Class constructor.
 * @param	bNewProj	true to create a new project dialog, false to display
 *						the properties of an existing project
 * @param	pParent		The parent widget
 * @param	szName		The widget's name
 */
NewProjectDlg::NewProjectDlg(bool bNewProj, QWidget* pParent, 
	const char* szName) :
	NewProjectLayout(pParent, szName),
	m_bNewProj(bNewProj)
{
	ProjectBase::Options opt;

	// Create the auto-completion sub-dialogue
	m_pAutoCompDlg = new AutoCompletionDlg(this);
	
	// Restrict the path requester to existing directories.
	m_pPathRequester->setMode(KFile::Directory | KFile::ExistingOnly | 
		KFile::LocalOnly);
	m_pSrcRootRequester->setMode(KFile::Directory | KFile::ExistingOnly | 
			KFile::LocalOnly);
	
	// Set up the Create/Cancel buttons	
	connect(m_pCreateButton, SIGNAL(clicked()), this, SLOT(accept()));
	connect(m_pCancelButton, SIGNAL(clicked()), this, SLOT(reject()));

	// Show the auto-completion properties dialogue
	connect(m_pACButton, SIGNAL(clicked()), m_pAutoCompDlg, SLOT(exec()));	
		
	// Perform actions specific to the type of dialog (new project or
	// project properties)
	if (bNewProj) {
		// Set default project properties
		ProjectBase::getDefOptions(opt);
		setProperties("", "", opt);
	}
	else {
		// Give appropriate titles to the dialog and the accept button
		setCaption(i18n("Project Properties"));
		m_pCreateButton->setText(i18n("OK"));
		
		// Disable the non-relevant widgets
		m_pNameEdit->setEnabled(false);
		m_pPathRequester->setEnabled(false);
	}
}

/**
 * Class destructor.
 */
NewProjectDlg::~NewProjectDlg()
{
}

/**
 * Configures the dialog's widget to display the properties of the current
 * project.
 * @param	sName	The project's name
 * @param	sPath	The project's path
 * @param	opt		Project parameters configurable in this dialogue
 */
void NewProjectDlg::setProperties(const QString& sName, const QString& sPath,
	const ProjectBase::Options& opt)
{
	QStringList::ConstIterator itr;
	
	// Set values for current project
	m_pNameEdit->setText(sName);
	m_pPathRequester->setURL(sPath);
	m_pSrcRootRequester->setURL(opt.sSrcRootPath);
	m_pKernelCheck->setChecked(opt.bKernel);
	m_pInvCheck->setChecked(opt.bInvIndex);
	m_pNoCompCheck->setChecked(opt.bNoCompress);
	m_pSlowPathCheck->setChecked(opt.bSlowPathDef);
	
	if (opt.nAutoRebuildTime >= 0) {
		m_pAutoRebuildCheck->setChecked(true);
		m_pAutoRebuildSpin->setValue(opt.nAutoRebuildTime);
	}

	if (opt.bACEnabled) {
		m_pACCheck->setChecked(true);
	}
	
	if (opt.nTabWidth > 0) {
		m_pTabWidthCheck->setChecked(true);
		m_pTabWidthSpin->setValue(opt.nTabWidth);
	}
	
	// Initialise the auto-completion sub-dialogue
	m_pAutoCompDlg->m_nMinChars = opt.nACMinChars;
	m_pAutoCompDlg->m_nDelay = opt.nACDelay;
	m_pAutoCompDlg->m_nMaxEntries = opt.nACMaxEntries;
	
	// Add type strings to the types list box
	for (itr = opt.slFileTypes.begin(); itr != opt.slFileTypes.end(); ++itr)
		m_pTypesList->insertItem(*itr);
}

/**
 * Retrieves the text entered by the user in the dialog's "Project Name" edit
 * box.
 * @return	The name of the new project
 */
QString NewProjectDlg::getName()
{
	return m_pNameEdit->text();
}

/**
 * Retrieves the text entered by the user in the dialog's "Project Path" edit
 * box.
 * Note that the chosen path will be the parent of the new project's
 * directory, created under it using the project's name.
 * @return	The full path of the parent directory for the new project
 */
QString NewProjectDlg::getPath()
{
	return m_pPathRequester->url();
}

/**
 * Fills a structure with all user-configured project options.
 * @param	opt	The structure to fill
 */
void NewProjectDlg::getOptions(ProjectBase::Options& opt)
{
	opt.sSrcRootPath = m_pSrcRootRequester->url();
	opt.slFileTypes = m_slTypes;
	opt.bKernel = m_pKernelCheck->isChecked();
	opt.bInvIndex = m_pInvCheck->isChecked();
	opt.bNoCompress = m_pNoCompCheck->isChecked();
	opt.bSlowPathDef = m_pSlowPathCheck->isChecked();
		
	if (m_pAutoRebuildCheck->isChecked())
		opt.nAutoRebuildTime = m_pAutoRebuildSpin->value();
	else
		opt.nAutoRebuildTime = -1;
		
	if (m_pTabWidthCheck->isChecked())
		opt.nTabWidth = m_pTabWidthSpin->value();
	else
		opt.nTabWidth = 0;
		
	opt.bACEnabled = m_pACCheck->isChecked();
	opt.nACMinChars = m_pAutoCompDlg->m_nMinChars;
	opt.nACDelay = m_pAutoCompDlg->m_nDelay;
	opt.nACMaxEntries = m_pAutoCompDlg->m_nMaxEntries;
}

/**
 * Ends the dialog after the user has clicked the "OK" button.
 */
void NewProjectDlg::accept()
{
	int i, nCount;
	
	// Validate the name of a new project
	if (m_bNewProj) {
		QRegExp re("[^ \\t\\n]+");
		if (!re.exactMatch(m_pNameEdit->text())) {
			KMessageBox::error(0, i18n("Project names must not contain "
				"whitespace."));
			return;
		}
	}
	
	// Fill the string list with all file types
	nCount = (int)m_pTypesList->count();
	for (i = 0; i < nCount; i++)
		m_slTypes.append(m_pTypesList->text(i));

	// Clean-up the source root
	QDir dir(m_pSrcRootRequester->url());
	if (dir.exists())
		m_pSrcRootRequester->setURL(dir.absPath());
	else
		m_pSrcRootRequester->setURL("/");
		
	// Close the dialog
	QDialog::accept();
}

/**
 * Adds the the file type string in the edit-box to the list of project types.
 * This slot is called when the "Add.." button is clicked.
 */
void NewProjectDlg::slotAddType()
{
	QString sType;
		
	// Try the custom type edit-box first.
	sType = m_pTypesEdit->text();
	sType.stripWhiteSpace();
	if (sType.isEmpty())
		return;

	// Validate the type string
	QRegExp reg("[ \\t\\n\\|\\\\\\/]");
	if (sType.contains(reg)) {
		KMessageBox::error(0, i18n("This is not a valid file type!"));
		return;
	}

	// Do not add an existing type.
	if (m_pTypesList->findItem(sType, Qt::CaseSensitive | Qt::ExactMatch) != 
		NULL) {
		return;
	}

	// Add the file type to the list
	m_pTypesList->insertItem(sType);
	m_pTypesEdit->clear();
}

/**
 * Removes the selected item from the list of file types.
 * This slot is called when the "Remove" button is clicked.
 */
void NewProjectDlg::slotRemoveType()
{
	int nItem;
	QString sType;
	
	// Verify an item is selected
	nItem = m_pTypesList->currentItem();
	if (nItem == -1)
		return;

	// Remove the selected item
	sType = m_pTypesList->currentText();
	m_pTypesList->removeItem(nItem);
	
	// Add to the list of available types.
	if (m_pAvailTypesList->findItem(sType, Qt::CaseSensitive | Qt::ExactMatch) 
		== NULL) {
		m_pAvailTypesList->insertItem(sType);
	}
	
}

/**
 * Changes the text in the types edit-box to reflect the current selection in
 * the list of available types.
 * This slot is called whenever a new item is highlighted in the list of
 * available types.
 * @param	sType	The newly selected type
 */
void NewProjectDlg::slotAvailTypesChanged(const QString& sType)
{
	m_pTypesEdit->setText(sType);
}

/**
 * Class constructor.
 * @param	pParent		The parent widget
 * @param	szName		The widget's name
 */
AutoCompletionDlg::AutoCompletionDlg(QWidget* pParent,
	const char* szName ) :
	AutoCompletionLayout(pParent, szName)
{
	connect(m_pOKButton, SIGNAL(clicked()), this, SLOT(accept()));
	connect(m_pCancelButton, SIGNAL(clicked()), this, SLOT(reject()));
}

/**
 * Class destructor.
 */
AutoCompletionDlg::~AutoCompletionDlg()
{
}

/**
 * Displays the dialogue, and waits for either the "OK" or "Cancel" button to
 * be clicked.
 * Before the dialogue is displayed, the stored values are set to the widgets.
 * @return	The dialogue's termination code
 */
int AutoCompletionDlg::exec()
{
	// Set current values
	m_pMinCharsSpin->setValue(m_nMinChars);
	m_pDelaySpin->setValue(m_nDelay);
	m_pMaxEntriesSpin->setValue(m_nMaxEntries);

	// Show the dialogue
	return QDialog::exec();
}

/**
 * Stores the values set by the user in the dialogue widgets, and terminates
 * the dialogue.
 * This slot is connected to the clicked() signal of the "OK" button.
 */
void AutoCompletionDlg::accept()
{
	// Store widget values
	m_nMinChars = m_pMinCharsSpin->value();
	m_nDelay = m_pDelaySpin->value();
	m_nMaxEntries = m_pMaxEntriesSpin->value();
	
	// Close the dialogue, indicating acceptance
	QDialog::accept();
}


#include "newprojectdlg.moc"
