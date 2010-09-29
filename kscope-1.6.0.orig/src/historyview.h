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

#ifndef HISTORYVIEW_H
#define HISTORYVIEW_H

#include "queryview.h"

/**
 * A list view item for holding position history records.
 * A QListViewItem cannot reference its preceding item, which is required to
 * create a stack-like history list. Therefore a HistoryItem object
 * stores a pointer to the item just above it in the list. The pointer is
 * persistent, since the list cannot be sorted.
 * @author Elad Lahav
 */

class HistoryItem : public QListViewItem
{
public:
	/**
	 * Class constructor.
	 * @param	pList	The parent list view
	 * @param	sFile	The file path in this record
	 * @param	sLine	The line number
	 * @param	sText	The contents of the line in the given file
	 */
	HistoryItem(QListView* pList, QString sFile, QString sLine, 
		QString sText) : 
		QListViewItem(pList, "", sFile, sLine, sText),
		m_pPrevSibling(NULL) {
		HistoryItem* pNext;
		
		// Mark the new item as the predecessor of its next sibling 
		if ((pNext = (HistoryItem*)nextSibling()) != NULL)
			pNext->m_pPrevSibling = this;
	}
	
	/** The item immediately above this one in the list. */
	HistoryItem* m_pPrevSibling;
};

/**
 * A list view widget for holding position history.
 * Position history is kept in a stack-like list. Positions are always added
 * to the top of the list, immediately before the current item. If the
 * current item is not the top one, all items above it are purged first.
 * To keep the stack-like structure, the list cannot be sorted.
 * @author Elad Lahav
 */
class HistoryView : public QueryView
{
Q_OBJECT
public:
	HistoryView(QWidget* pParent = 0, const char* szName = 0);
	~HistoryView();
	
	virtual void addRecord(const QString&, const QString&, const QString&,
		const QString&, QListViewItem*);
	virtual void selectNext();
	virtual void selectPrev();

protected slots:
	virtual void slotRemoveItem(QListViewItem*);
};

#endif
