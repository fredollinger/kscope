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

#ifndef CALLTREEMANAGER_H
#define CALLTREEMANAGER_H

#include <qwidget.h>
#include <qptrlist.h>

class CallTreeDlg;

/**
 * Manages all call tree dialogs within the project.
 * Responsible for saving/loading of the call tree dialogs.
 * @author Albert Yosher
 */
class CallTreeManager : public QObject
{
   Q_OBJECT
   
public: 
	CallTreeManager(QWidget*);
	~CallTreeManager();
	
	void saveOpenDialogs(const QString&, QStringList&);
	void loadOpenDialogs(const QString&, const QStringList&);
	CallTreeDlg* addDialog();
	void closeAll();
	
signals:
	/**
	 * Emitted when any call tree dialogue sends a request to view a location
	 * in the source code.
	 * @param	sPath	The full path of the file to show
	 * @param	nLine	The line number in this file
	 */
	void lineRequested(const QString& sPath, uint nLine);
	 
private:
	/** The list of open call tree dialogues. */
	QPtrList<CallTreeDlg> m_lstDialogs;
	
private slots:
	void slotRemoveDialog(const CallTreeDlg*);
};

#endif
