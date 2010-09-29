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

#include <qpushbutton.h>
#include <qfileinfo.h>
#include <qtabwidget.h>
#include <kfiledialog.h>
#include "fileview.h"
#include "filelist.h"
#include "kscopepixmaps.h"

/**
 * Class constructor.
 * @param	pParent	The parent widget
 * @param	szName	The widget's name
 * @param	fl		Widget creation flags
 */
FileView::FileView(QWidget* pParent, const char* szName, WFlags fl) :
	FileViewLayout(pParent, szName, fl),
	m_pCurBranch(0),
	m_sRoot("")
{
	QWidget* pPage;
	
	// Set the tab widget icons
	pPage = m_pTabWidget->page(0);
	m_pTabWidget->setTabIconSet(pPage, GET_PIXMAP(TabFileList));
	pPage = m_pTabWidget->page(1);
	m_pTabWidget->setTabIconSet(pPage, GET_PIXMAP(TabFileTree));
	
	// Create a single column for the file tree
	m_pFileTree->addColumn("");
	
	// Send the fileRequested() signal whenever a file is selected in either
	// the list or the tree
	connect(m_pFileList, SIGNAL(fileRequested(const QString&, uint)), this,
		SIGNAL(fileRequested(const QString&, uint)));
	connect(m_pFileTree, SIGNAL(doubleClicked(QListViewItem*)), 
		this, SLOT(slotTreeItemSelected(QListViewItem*)));
	connect(m_pFileTree, SIGNAL(returnPressed(QListViewItem*)), this, 
		SLOT(slotTreeItemSelected(QListViewItem*)));
}

/**
 * Class destructor.
 */
FileView::~FileView()
{
}

/**
 * Sets a new common root path to both the file list and the tree.
 * @param	sRoot	The full path of the new root
 */
void FileView::setRoot(const QString& sRoot)
{
	// Nothing to do if the given root is the same as the old one
	if (sRoot == m_sRoot)
		return;
	
	m_sRoot = sRoot;
	
	// Remove the current branch
	if (m_pCurBranch)
		m_pFileTree->removeBranch(m_pCurBranch);
	
	// Update the file list
	m_pFileList->setRoot(sRoot);
	
	// Nothing more to do for an empty root directory
	if (sRoot.isEmpty())
		return;
	
	// Create and open a new branch, with the newly specified root
	QFileInfo fi(sRoot);
	m_pCurBranch = m_pFileTree->addBranch(KURL(sRoot), fi.fileName());
	m_pCurBranch->setChildRecurse(false);
	m_pFileTree->setOpen(m_pCurBranch->root(), true);
}

/**
 * Clears the contents of the file view and file tree.
 */
void FileView::clear()
{
	m_pFileList->clear();
	setRoot("");
} 

/**
 * Emits the fileRequested() signal when a file name is selected in the file 
 * tree. An item is selected by either double-clicking it or by hittin 
 * "ENTER" when it is highlighted.
 * This slot is connected to the doubleClicked() and returnPressed() signals
 * of the KFileTreeView object.
 * @param	pItem	The selected tree item
 */
void FileView::slotTreeItemSelected(QListViewItem* pItem)
{
	KFileTreeViewItem* pTreeItem;
	 
	pTreeItem = (KFileTreeViewItem*)pItem;
	if (!pTreeItem->isDir())
		emit fileRequested(pTreeItem->path(), 0);
}

#include "fileview.moc"
