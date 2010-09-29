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

#ifndef PROGRESSDLG_H
#define PROGRESSDLG_H

#include <qwidget.h>
#include <qtimer.h>
#include <kprogress.h>

/**
 * An improved progress dialog.
 * This variation of the standard KDE progress dialog displays a busy
 * indicator while waiting for the first value greater than 0.
 * @author Elad Lahav
 */

class ProgressDlg : public KProgressDialog
{
   Q_OBJECT

public: 
	ProgressDlg(const QString&, const QString&, QWidget* pParent = 0, const
		char* szName = 0);
	~ProgressDlg();

	void setValue(int);
	void setIdle();
	
private:
	/** When the value is 0, this timer initiates value changes that cause
		the progress-bar to move. */
	QTimer* m_pIdleTimer;

	/** A dummy value used to move the progress-bar while the value is 0. */
	int m_nIdleValue;

private slots:
	void slotShowBusy();
};

#endif
