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

#ifndef QUERYRESULTSMENU_H
#define QUERYRESULTSMENU_H

#include <qpopupmenu.h>
#include <qlistview.h>
#include <qregexp.h>

/**
 * Provides a popup-menu for list views containing query results.
 * The popup menu contains commands for copying field text out of items and
 * for removing items.
 * This class assumes a certain ordering of the list columns. If an owner
 * object uses a different configuration, it needs to call setColumns() after
 * constructing the object.
 * @author Elad Lahav
 */
class QueryResultsMenu : public QPopupMenu
{
	Q_OBJECT
	
public:
    QueryResultsMenu(QWidget* pParent = 0, const char* szName = 0);
    ~QueryResultsMenu();
	
public slots:		
	void slotShow(QListViewItem*, const QPoint&, int nCol);
	
signals:
	/** 
	 * Indicates that the "View Source" menu item was selected. 
	 * @param	pItem	The item for which the menu was displayed
	 */
	void viewSource(QListViewItem* pItem);
	 
	/**
	 * Indicates that the "Find Definition" menu item was selected.
	 * @param	sFunc	The function to look for
	 */
	void findDef(const QString& sFunc);
	
	/** 
	 * Indicates that the "Copy [Column]" menu item was selected. 
	 * @param	pItem	The item for which the menu was displayed
	 * @param	nCol	The requested column
	 */
	void copy(QListViewItem* pItem, int nCol);
	
	/**
	 * Indicates that the "Filter..." menu item was selected.
	 * @param	nCol	The column in which to search
	 */
	void filter(int nCol);
	
	/**
	 * Indicates that the "Show All" menu item was selected.
	 */
	void showAll();
	
	/** 
	 * Indicates that the "Remove Item" menu item was selected. 
	 * @param	pItem	The item for which the menu was displayed
	 */
	void remove(QListViewItem* pItem);
	
private:
	/** Menu item IDs. */
	enum { ViewSource, FindDef, Copy, Filter, ShowAll, Remove };
		
	/** The item for which the popup menu is provided (cannot be NULL). */
	QListViewItem* m_pItem;
	
	/** The list column for which the query was invoked. */
	int m_nCol;
	
private slots:
	void slotViewSource();
	void slotFindDef();
	void slotCopy();
	void slotFilter();
	void slotRemove();
};

#endif
