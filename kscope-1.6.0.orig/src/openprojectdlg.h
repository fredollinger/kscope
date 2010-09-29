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

#ifndef OPENPROJECTDLG_H
#define OPENPROJECTDLG_H

#include <qwidget.h>
#include <openprojectlayout.h>

/**
 * A dialogue for selecting a project to open.
 * Allows projects to be searched, and displays a list of previosuly loaded
 * projects.
 * @author Elad Lahav
 */

class OpenProjectDlg : public OpenProjectLayout
{
	Q_OBJECT

public: 
	OpenProjectDlg(QWidget* pParent = 0, const char* szName = 0);
	~OpenProjectDlg();

	QString getPath() const;

protected slots:
	virtual void slotProjectSelected(const QString&);
	virtual void slotRemoveRecent();
	virtual void slotSelectRecent(QListBoxItem*);
	virtual void slotOpenRecent(QListBoxItem*);

private:
	void loadRecent();
};

#endif
