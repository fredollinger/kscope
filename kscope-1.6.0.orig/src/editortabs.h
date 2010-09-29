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

#ifndef EDITORTABS_H
#define EDITORTABS_H

#include <qwidget.h>
#include <qpopupmenu.h>
#include "tabwidget.h"
#include "editorpage.h"
#include "projectmanager.h"

typedef QMap<QString, EditorPage*> EditorMap;
class QueryView;

/**
 * A tab widget that holds several editor windows.
 * This class provides the main widget in the KScope window. All editors are
 * opened as pages of the tab widgets.
 * @author Elad Lahav
 */

class EditorTabs : public TabWidget
{
   Q_OBJECT
   
public:
	EditorTabs(QWidget* pParent = 0, const char* szName = 0);
	~EditorTabs();

	void setWindowMenu(QPopupMenu*);
	void addEditorPage(EditorPage*);
	EditorPage* findEditorPage(const QString&, bool bForceChange = false);
	EditorPage* getCurrentPage();
	void removeCurrentPage();
	bool removeAllPages();
	void applyPrefs();
	void getOpenFiles(FileLocationList&);
	void getBookmarks(FileLocationList&);
	void setBookmarks(FileLocationList&);
	void showBookmarks(QueryView*);
	
public slots:
	void slotRemovePage(QWidget*);
	void slotToggleTagList();
	void slotSaveAll();
	void slotGoLeft();
	void slotGoRight();
	
signals:
	/**
	 * Emitted when the current editor page changes.
	 * @param	pOld	The previous current page
	 * @param	pNew	The new current page
	 */
	void editorChanged(EditorPage* pOld, EditorPage* pNew);
	
	/**
	 * Emitted when an editor page is closed.
	 * @param	pPage	The removed page
	 */
	void editorRemoved(EditorPage* pPage);
	
	/**
	 * Indicates that files were dragged and dropped over the tab widget.
	 * @param	pEvent	The drop event information
	 */
	void filesDropped(QDropEvent* pEvent);
	
protected:
	virtual void dragMoveEvent(QDragMoveEvent*);
	virtual void dropEvent(QDropEvent*);
	
private:
	/** Links a file name with an editor page that has this file open. */
	EditorMap m_mapEdit;

	/** We need to keep track of the current page in order to implement the
		editorChanged() signal. */
	EditorPage* m_pCurPage;
	
	/** A popup menu with Cscope operations for the editor windows. */
	QPopupMenu* m_pWindowMenu;
	
	/** The number of items added to the window menu (used for removing old
		items). */
	int m_nWindowMenuItems;
	
	/** A counter for creating unique tab captions for new files. */
	int m_nNewFiles;
	
	int getModifiedFilesCount();
	bool removePage(QWidget*, bool);
		
private slots:
	void slotCurrentChanged(QWidget*);
	void slotAttachFile(EditorPage*, const QString&);
	void slotNewFile(EditorPage*);
	void slotFileModified(EditorPage*, bool);
	void slotInitiateDrag(QWidget*);
	void slotFillWindowMenu();
	void slotSetCurrentPage(int);
};

#endif
