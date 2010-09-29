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

#ifndef FILELIST_H
#define FILELIST_H

#include <qwidget.h>
#include "searchlist.h"
#include "projectmanager.h"

/**
 * Implements a searchable list of files.
 * The file list is composed of a list view, and a search box, into which the
 * user can enter a file name. The name is matched against the contents of
 * the list, and matching items are selected.
 * @author Elad Lahav
 */

class FileList : public SearchList, public FileListTarget
{
	Q_OBJECT

public: 
	FileList(QWidget* pParent = 0, const char* szName = 0);
	~FileList();
	
	virtual void addItem(const QString&);
	bool findFile(const QString&);
	void clear();
	void applyPrefs();
	void setRoot(const QString&);
	virtual bool getTip(QListViewItem*, QString&);
	
signals:
	/**
	 * Emitted when a file is selected, by either double-clicking a list
	 * item, or by highlighting an item and pressing the ENTER key.
	 * @param	sPath	The full path of the selected file
	 * @param	nLine	Line number, always set to 0
	 */
	void fileRequested(const QString& sPath, uint nLine);

protected:
	virtual void processItemSelected(QListViewItem*);
	
private:
	/** A common root path for all items in the list. */
	QString m_sRoot;
};

#endif
