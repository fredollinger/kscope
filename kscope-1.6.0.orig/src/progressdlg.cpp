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

#include "progressdlg.h"

/**
 * Class constructor.
 * @param	sCaption	The dialogue's title
 * @param	sText		The text to display
 * @param	pParent		The parent widget
 * @param	szName		The widget's name
 */
ProgressDlg::ProgressDlg(const QString& sCaption, const QString& sText,
	QWidget* pParent, const char* szName) :
	KProgressDialog(pParent, szName, sCaption, sText, true),
	m_nIdleValue(-1)
{
	setAutoClose(false);
	setAllowCancel(false);
	
	// Create the idle-progress timer
	m_pIdleTimer = new QTimer(this);

	// Display a busy indicator by increasing the value of the idle counter
	connect (m_pIdleTimer, SIGNAL(timeout()), this, SLOT(slotShowBusy()));
}

/**
 * Class destructor.
 */
ProgressDlg::~ProgressDlg()
{
}

/**
 * Sets a new value to the progress bar.
 * If the new value is non-zero, the progress bar is advanced. Otherwise, the
 * idle timer is initiated to display a busy indicator.
 * @param	nValue	The new value to set.
 */
void ProgressDlg::setValue(int nValue)
{
	KProgress* pProgress;

	pProgress = progressBar();
	
	if (nValue != 0) {
		// Do nothing if the value hasn't changed
		if (nValue == pProgress->progress())
			return;

		// Handle first non-zero value
		if (m_nIdleValue >= 0) {
			m_pIdleTimer->stop();
			m_nIdleValue = -1;
			pProgress->setPercentageVisible(true);
		}

		// Set the new value
		pProgress->setValue(nValue);
	}
	else if (m_nIdleValue == -1) {
		// Handle first 0 value
		pProgress->setValue(0);
		pProgress->setPercentageVisible(false);
		m_nIdleValue = 0;
		m_pIdleTimer->start(200);
	}
}

void ProgressDlg::setIdle()
{
	m_nIdleValue = -1;
	setValue(0);
}

/**
 * Increaes the value of the dummy counter by 1.
 * This slot is called by the timeout() event of the idle timer.
 */
void ProgressDlg::slotShowBusy()
{
	// Increase the counter
	m_nIdleValue += 5;
	if (m_nIdleValue == 100)
		m_nIdleValue = 0;
		
	// Set the value of the progress-bar
	progressBar()->setValue(m_nIdleValue);
}

#include "progressdlg.moc"
