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

#include "historyview.h"

/**
 * Class constructor.
 * @param	pParent	The parent widget
 * @param	szName	The widget's name
 */
HistoryView::HistoryView(QWidget* pParent, const char* szName) :
	QueryView(pParent, szName)
{
	// Disable sorting
	setSortColumn(-1);
	
	setColumnWidth(0, 0);
}

/**
 * Class destructor.
 */
HistoryView::~HistoryView()
{
}

/**
 * Creates a new list item showing a history record.
 * @param	sFunc	The name of the function
 * @param	sFile	The file path
 * @param	sLine	The line number in the above file
 * @param	sText	The line's text
 */
void HistoryView::addRecord(const QString& /* sFunc */, const QString& sFile,
	const QString& sLine, const QString& sText, QListViewItem*)
{
	HistoryItem* pItem;
	
	pItem = new HistoryItem(this, sFile, sLine, sText);
	setSelected(pItem, true);
}

/**
 * Moves to the previous item in the history, selecting it for display.
 * Note that this function move to the item which chronologically precedes
 * the current one, which, in list view terms, is the next item.
 */
void HistoryView::selectPrev()
{
	QListViewItem* pItem;

	// Get the current item
	pItem = currentItem();
	
	// Select the next item in the list
	if (pItem != NULL && pItem->nextSibling() != NULL) {
		pItem = pItem->nextSibling();
		select(pItem);
	}
}

/**
 * Moves to the next item in the history, selecting it for display.
 * Note that this function move to the item which chronologically succedes
 * the current one, which, in list view terms, is the previous item.
 */
void HistoryView::selectNext()
{
	HistoryItem* pItem;

	// Get the current item
	pItem = (HistoryItem*)currentItem();
	
	// Select the previous item in the list
	if (pItem != NULL && pItem->m_pPrevSibling != NULL) {
		pItem = pItem->m_pPrevSibling;
		select(pItem);
	}
}

/**
 * Deletes the item on which a popup-menu has been invoked.
 * This slot is connected to the remove() signal of the QueryResultsMenu
 * object.
 * @param	pItem	The item to remove
 */
void HistoryView::slotRemoveItem(QListViewItem* pItem)
{
	HistoryItem* pCurItem, * pNextItem;
	
	pCurItem = (HistoryItem*)pItem;
	if ((pNextItem = (HistoryItem*)pCurItem->nextSibling()) != NULL)
		pNextItem->m_pPrevSibling = pCurItem->m_pPrevSibling;
	
	delete pCurItem;
}

#include "historyview.moc"
