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

#include <qlabel.h>
#include <qpushbutton.h>
#include <klocale.h>
#include "scanprogressdlg.h"

/**
 * Class constructor.
 * @param	pParent	The parent widget
 * @param	szName	The widget's name
 */
ScanProgressDlg::ScanProgressDlg(QWidget* pParent, const char* szName) :
	ScanProgressLayout(pParent, szName),
	m_nFiles(0),
	m_nCurFiles(0)
{
	show();

	// Emit the cancelled() signal when the "Cancel" button is clicked
	connect(m_pCancelButton, SIGNAL(clicked()), this, SIGNAL(cancelled()));
}

/**
 * Class destructor.
 */
ScanProgressDlg::~ScanProgressDlg()
{
}

/**
 * Adds the given number of files to the total count of files already scanned.
 * A visual indication of the progress is given in intervals of more than 100
 * files (to prevent too-frequent GUI updates.)
 * @param	nFiles	The number of files scanned since the last call
 */
void ScanProgressDlg::addFiles(int nFiles)
{
	QString sText;

	// Do nothing if no files were scanned
	if (nFiles <= 0)
		return;

	// Update the total number of files scanned
	m_nFiles += nFiles;
	
	// Update progress only if more than 100 files were scanned since the last
	// update
	if ((m_nFiles - m_nCurFiles) > 100) {
		sText.sprintf(i18n("Scanned %d files..."), m_nFiles);
		m_pText->setText(sText);
		m_nCurFiles = m_nFiles;
	}
}

#include "scanprogressdlg.moc"
