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

#ifndef SCANPROGRESSDLG_H
#define SCANPROGRESSDLG_H

#include <qwidget.h>
#include <scanprogresslayout.h>

/**
 * Displays the progress of a file scan operation.
 * This dialogue is displayed while a ProjectFilesDlg dialogue scans a
 * directory tree for all files matching the patterns defined for the
 * project's source files.
 * @author Elad Lahav
 */

class ScanProgressDlg : public ScanProgressLayout
{
   Q_OBJECT

public: 
	ScanProgressDlg(QWidget* pParent = 0, const char* szName = 0);
	~ScanProgressDlg();

	void addFiles(int);

signals:
	/**
	 * Indicates that the dialogue's "Cancel" button hsa been clicked by the
	 * user.
	 */
	void cancelled();
	
private:
	/** The total number of files scanned thus far. */
	int m_nFiles;
	
	/** The number of files currently displayed in the progress report (which
		may be smaller than m_nFiles since not every call to addFiles() updates
		the progress display.)*/
	int m_nCurFiles;
};

#endif
