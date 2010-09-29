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

#ifndef FILEVIEW_H
#define FILEVIEW_H

#include <kfiletreeview.h>
#include "fileviewlayout.h"

/**
 * A tabbed widget that contains a file list and a file tree.
 * The list is an object of type FileList, which displays all files included
 * in the current project. The tree is a standard KFileTreeView, which can
 * browse through the entire file system. Optionally, the root of the tree
 * can be set per project.
 * @author Elad Lahav
 */

class FileView : public FileViewLayout
{
	Q_OBJECT

public:
	FileView(QWidget* pParent = 0, const char* szName = 0, WFlags fl = 0);
	~FileView();

	/**
	 * @return	The file list widget which is a child of this widget.
	 */
	FileList* getFileList() { return m_pFileList; }
	
	void setRoot(const QString&);
	void clear();
	
signals:
	/**
	 * Emitted when a file is selected, by either double-clicking a list
	 * item, or by highlighting an item and pressing the ENTER key.
	 * @param	sPath	The full path of the selected file
	 * @param	nLine	Line number, always set to 0
	 */
	void fileRequested(const QString& sPath, uint nLine);

private:
	/** The current branch in the file tree. */
	KFileTreeBranch* m_pCurBranch;

	/** The current root of the file tree. */
	QString m_sRoot;
	
private slots:
	void slotTreeItemSelected(QListViewItem*);
};

#endif

