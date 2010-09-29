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

#include <qapplication.h>
#include <qclipboard.h>
#include <klocale.h>
#include "queryview.h"
#include "queryresultsmenu.h"
#include "queryviewdlg.h"
#include "cscopefrontend.h"
#include "searchresultsdlg.h"

/**
 * Class constructor.
 * @param	pParent	The parent widget
 * @param	szName	The name of the widget
 */
QueryView::QueryView(QWidget* pParent, const char* szName) :
	QListView(pParent, szName),
	m_pLastItem(NULL)
{
	// Create the popup-menu
	m_pQueryMenu = new QueryResultsMenu(this);

	// Initialise the list's columns
	setAllColumnsShowFocus(true);
	addColumn(i18n("Function"));
	addColumn(i18n("File"));
	addColumn(i18n("Line"));
	addColumn(i18n("Text"));
	setColumnAlignment(2, Qt::AlignRight);

	// A record is selected if it is either double-clicked, or the ENTER
	// key is pressed while the record is highlighted
	connect(this, SIGNAL(doubleClicked(QListViewItem*)), this, 
		SLOT(slotRecordSelected(QListViewItem*)));
	connect(this, SIGNAL(returnPressed(QListViewItem*)), this, 
		SLOT(slotRecordSelected(QListViewItem*)));
		
	// Show the popup-menu when requested
	connect(this, 
		SIGNAL(contextMenuRequested(QListViewItem*, const QPoint&, int)),
		m_pQueryMenu, SLOT(slotShow(QListViewItem*, const QPoint&, int)));
		
	// Handle popup-menu commands
	connect(m_pQueryMenu, SIGNAL(viewSource(QListViewItem*)), this,
		SLOT(slotRecordSelected(QListViewItem*)));
	connect(m_pQueryMenu, SIGNAL(findDef(const QString&)), this,
		SLOT(slotFindDef(const QString&)));
	connect(m_pQueryMenu, SIGNAL(copy(QListViewItem*, int)), this,
		SLOT(slotCopy(QListViewItem*, int)));
	connect(m_pQueryMenu, SIGNAL(filter(int)), this, SLOT(slotFilter(int)));
	connect(m_pQueryMenu, SIGNAL(showAll()), this, 
		SLOT(slotShowAll()));
	connect(m_pQueryMenu, SIGNAL(remove(QListViewItem*)), this,
		SLOT(slotRemoveItem(QListViewItem*)));
}

/**
 * Class destructor.
 */
QueryView::~QueryView()
{
}

/**
 * Creates a new list item showing a query result record.
 * @param	sFunc	The name of the function
 * @param	sFile	The file path
 * @param	sLine	The line number in the above file
 * @param	sText	The line's text
 * @param	pParent	The parent item (ignored)
 */
void QueryView::addRecord(const QString& sFunc, const QString& sFile,
	const QString& sLine, const QString& sText, QListViewItem* /* pParent */)
{
	QListViewItem* pItem;
	
	pItem = new QListViewItem(this, m_pLastItem);
	pItem->setText(0, sFunc);
	pItem->setText(1, sFile);
	pItem->setText(2, sLine);
	pItem->setText(3, sText);
	
	m_pLastItem = pItem;
}

/**
 * Selects an item.
 * When an item is selected, it is highlighted and made visible. By
 * definition, the lineRequested() signal is also emitted.
 * This method is used for selecting records programmatically (@see
 * selectNext() for example). It has nothing to do with user selection.
 * @param	pItem	The list item to select
 */
void QueryView::select(QListViewItem* pItem)
{
	ensureItemVisible(pItem);
	setSelected(pItem, true);
	slotRecordSelected(pItem);
}

/**
 * Selects the next record in the list (if one exists).
 * The function selects the next item as follows:
 * - The first item in the list, if there is no current item
 * - The current item, if it is not selected
 * - The item immediately below the current item, otherwise
 */
void QueryView::selectNext()
{
	QListViewItem* pItem;
	
	// Do nothing if the list is empty
	if (firstChild() == NULL)
		return;
		
	// Find the next record
	pItem = currentItem();
	if (pItem == NULL) {
		pItem = firstChild();
	}
	else if (pItem->isSelected()) {
		pItem = pItem->itemBelow();
		if (pItem == NULL)
			return;
	}
	
	// Select the new item
	select(pItem);
}

/**
 * Selects the previous record in the list (if one exists).
 * The function selects the previous item as follows:
 * - The first item in the list, if there is no current item
 * - The current item, if it is not selected
 * - The item immediately above the current item, otherwise
 */
void QueryView::selectPrev()
{
	QListViewItem* pItem;
	
	// Do nothing if the list is empty
	if (firstChild() == NULL)
		return;
	
	// Find the item immediately above this one
	pItem = currentItem();
	if (pItem == NULL) {
		pItem = firstChild();
	}
	else if (pItem->isSelected()) {
		pItem = pItem->itemAbove();
		if (pItem == NULL)
			return;
	}
		
	// Select the new item
	select(pItem);
}

/**
 * Informs the view that query progress information has been received.
 * The view emits the needToShow() signal telling its parent that the widget
 * should become visible (if not already so).
 */
void QueryView::queryProgress()
{
	if (!isVisible())
		emit needToShow();
}

/**
 * Called when a query using this view terminates.
 * @param	nRecords	Number of records generated by the query
 */
void QueryView::queryFinished(uint nRecords, QListViewItem*)
{
	// Auto-select a single record (no need to emit the show() signal in
	// that case)
	if (nRecords == 1) {
		emit select(firstChild());
		return;
	}
	
	// Report a query that has returned an empty record set
	if (nRecords == 0)
		addRecord(i18n("No results"), "", "", "", NULL);
	
	// Data is available, instruct the owner object to show the view
	emit needToShow();
}

/**
 * Creates an iterator and initialises it to point to the first _visible_
 * item in the list.
 * @return	A new iterator initialised to the beginning of the list
 */
QueryView::Iterator QueryView::getIterator()
{
	QListViewItem* pItem;
	
	for (pItem = firstChild(); pItem != NULL && !pItem->isVisible(); 
		pItem = pItem->nextSibling());
		
	return Iterator(pItem);
}

/**
 * Handles double-click events over the view.
 * NOTE: We override this method since the QListView implementation opens
 * expandable items. This is undesired for tree-like query views (such as the
 * call tree.
 * @param	pEvent	Event description object
 */
void QueryView::contentsMouseDoubleClickEvent(QMouseEvent* pEvent)
{
	QListViewItem* pItem;
	
	// Handle only left button double-clicks
	if (pEvent == NULL || pEvent->button() != LeftButton)
		return;
		
	// Find the clicked item
	pItem = itemAt(contentsToViewport(pEvent->pos()));
	if (pItem == NULL)
		return;
		
	// Emit the doubleClicked() signal
	emit doubleClicked(pItem);
}

/**
 * Emits the lineRequested() signal when a list item is selected.
 * This slot is connected to the doubleClicked() and returnPressed()
 * signals of the list view.
 * @param	pItem	The selected item
 */
void QueryView::slotRecordSelected(QListViewItem* pItem)
{
	QString sFileName, sLine;

	// Get the file name and line number
	sFileName = pItem->text(1);
	sLine = pItem->text(2);

	// Do not process the "No results" item
	if (!sLine.isEmpty())
		emit lineRequested(sFileName, sLine.toUInt());
}

/**
 * Looks up the definition of a given function.
 * Results are displayed in a popup window.
 * This slot is connected to the findDef() signal emitted by the results menu.
 * @param	sFunc	The function to look for
 */
void QueryView::slotFindDef(const QString& sFunc)
{
	QueryViewDlg* pDlg;
	
	// Create a query view dialogue
	pDlg = new QueryViewDlg(QueryViewDlg::DestroyOnSelect, this);
	
	// Display a line when it is selected in the dialogue
	connect(pDlg, SIGNAL(lineRequested(const QString&, uint)), this,
		SIGNAL(lineRequested(const QString&, uint)));
		
	// Start the query
	pDlg->query(CscopeFrontend::Definition, sFunc);
}

/**
 * Copies the text of the requested field (item+column) to the clipboard.
 * This slot is connected to the copy() signal of the QueryResultsMenu object.
 * @param	pItem	The item from which to copy
 * @param	nCol	The index of the item field to copy
 */
void QueryView::slotCopy(QListViewItem* pItem, int nCol)
{
	QApplication::clipboard()->setText(pItem->text(nCol),
		QClipboard::Clipboard);
}

/**
 * Hides all items in the page that do not meet the given search criteria.
 * This slot is connected to the search() signal of the QueryResultsMenu
 * object.
 * The search is incremental: only visible items are checked, so that a new
 * search goes over the results of the previous one.
 * @param	nCol	The list column to search in
 */
void QueryView::slotFilter(int nCol)
{
	SearchResultsDlg dlg(this);
	QRegExp re;
	QListViewItem* pItem;
	bool bNegate;
	
	// Prepare the dialogue
	dlg.setColumn(nCol);
	
	// Show the dialogue
	if (dlg.exec() != QDialog::Accepted)
		return;
		
	// Get the selected regular expression
	dlg.getPattern(re);
	bNegate = dlg.isNegated();
	
	// Disable visual updates while search is in progress
	setUpdatesEnabled(false);
	
	// Iterate over all items in the list
	pItem = firstChild();
	while (pItem != NULL) {
		// Filter visible items only
		if (pItem->isVisible() &&
			(re.search(pItem->text(nCol)) == -1) != bNegate) {
			pItem->setVisible(false);
		}
		
		pItem = pItem->nextSibling();
	}
	
	// Redraw the list
	setUpdatesEnabled(true);
	triggerUpdate();
}

/**
 * Makes all list items visible.
 * This slot is connected to the showAll() signal of the QueryResultsMenu
 * object.
 */
void QueryView::slotShowAll()
{
	QListViewItem* pItem;
	
	// Iterate over all items in the list
	pItem = firstChild();
	while (pItem != NULL) {
		pItem->setVisible(true);
		pItem = pItem->nextSibling();
	}
}

/**
 * Deletes the item on which a popup-menu has been invoked.
 * This slot is connected to the remove() signal of the QueryResultsMenu
 * object.
 * @param	pItem	The item to remove
 */
void QueryView::slotRemoveItem(QListViewItem* pItem)
{
	delete pItem;
}

/**
 * Moves the iterator to the next _visible_ item in the list.
 */
void QueryView::Iterator::next()
{
	if (m_pItem == NULL)
		return;
		
	do {
		m_pItem = m_pItem->nextSibling();
	} while (m_pItem != NULL && !m_pItem->isVisible());
}

/**
 * @return	The function associated with the list item the iterator points to
 */
QString QueryView::Iterator::getFunc()
{
	if (m_pItem == NULL)
		return "";
		
	return m_pItem->text(0);
}

/**
 * @return	The file associated with the list item the iterator points to
 */
QString QueryView::Iterator::getFile()
{
	if (m_pItem == NULL)
		return "";
		
	return m_pItem->text(1);
}

/**
 * @return	The line number associated with the list item the iterator points
 *			to
 */
QString QueryView::Iterator::getLine()
{
	if (m_pItem == NULL)
		return "";
		
	return m_pItem->text(2);
}

/**
 * @return	The text associated with the list item the iterator points to
 */
QString QueryView::Iterator::getText()
{
	if (m_pItem == NULL)
		return "";
		
	return m_pItem->text(3);
}

#include "queryview.moc"
