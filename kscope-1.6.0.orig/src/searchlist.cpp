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

#include <qheader.h>
#include "searchlist.h"

/**
 * Intercepting additional key events of QLineEdit to browse the list
 * @param	pKey	The pressed key event
 */
void SearchLineEdit::keyPressEvent(QKeyEvent* pKey) 
{
	switch(pKey->key()) {
	case Qt::Key_Up:
	case Qt::Key_Down:
	case Qt::Key_PageUp:
	case Qt::Key_PageDown:
		emit keyPressed(pKey); 
		break;
	
	default:
		QLineEdit::keyPressEvent(pKey);
		break;
	}
}
	
/**
 * Class constructor.
 * @param	pParent	Owner list view widget
 */
ListToolTip::ListToolTip(SearchList* pParent) : 
	QToolTip(pParent->getList()->viewport()), 
	m_pList(pParent)
{
}

/**
 * Displays a tool-tip according to the current location of the mouse
 * pointer.
 * @param	pt	The mouse pointer coordinates
 */
void ListToolTip::maybeTip(const QPoint& pt)
{
	QString str;
	QListView* pList;
	QListViewItem* pItem;
	
	// Get the item at the given point
	pList = m_pList->getList();
	pItem = pList->itemAt(pt);
	if (pItem == NULL)
		return;

	// Get the tip string for this item
	if (!m_pList->getTip(pItem, str))
		return;

	// Get the bounding rectangle of the item
	const QRect rcItem = pList->itemRect(pItem);
	if (!rcItem.isValid())
		return;

	// Get the header coordinates
	const QRect rcHead = pList->header()->rect();
	if (!rcHead.isValid())
		return;

	// Calculate the tool-tip rectangle
	QRect rcCell(rcHead.left(), rcItem.top(), rcItem.width(), rcItem.height());

	// Display the tool-tip
	tip(rcCell, str);
}

/**
 * Class constructor.
 * @param	nSearchCol	The list column on which to perform string look-ups
 * @param	pParent		The parent widget
 * @param	szName		The widget's name
 */
SearchList::SearchList(int nSearchCol, QWidget* pParent, const char* szName) :
	QVBox(pParent, szName),
	m_nSearchCol(nSearchCol)
{
	// Create the child widgets
	m_pEdit = new SearchLineEdit(this);
	m_pList = new QListView(this);
	
	// Set up the tooltip generator
	QToolTip::remove(m_pList);
	m_pToolTip = new ListToolTip(this);
	
	connect(m_pEdit, SIGNAL(textChanged(const QString&)), this,
		SLOT(slotFindItem(const QString&)));
	connect(m_pList, SIGNAL(doubleClicked(QListViewItem*)), this,
		SLOT(slotItemSelected(QListViewItem*)));
	connect(m_pList, SIGNAL(returnPressed(QListViewItem*)), this,
		SLOT(slotItemSelected(QListViewItem*)));
	connect(m_pEdit, SIGNAL(returnPressed()), this,
		SLOT(slotItemSelected()));
	connect(m_pEdit, SIGNAL(keyPressed(QKeyEvent*)), this,
		SLOT(slotKeyPressed(QKeyEvent*)));
}

/**
 * Class destructor.
 */
SearchList::~SearchList()
{
	delete m_pToolTip;
}

/**
 * Sets the keyboad focus to the search box.
 */
void SearchList::slotSetFocus()
{
	m_pEdit->setFocus();
}

/**
 * Selects a list item whose string begins with the text entered in the edit
 * widget.
 * This slot is connected to the textChanged() signal of the line edit widget.
 * @param	sText	The new text in the edit widget
 */
void SearchList::slotFindItem(const QString& sText)
{
	QListViewItem* pItem;
	
	// Try to find an item that contains this text
	// Priority to exactly matched, 
	// then try to find line begins with the text,
	// and if not found, then try to find the line contains the text
	pItem = m_pList->findItem(sText, m_nSearchCol, 
		ExactMatch | BeginsWith | Contains);

	// Select this item
	if (pItem != 0) {
		m_pList->setSelected(pItem, true);
		m_pList->ensureItemVisible(pItem);
	}
}

/**
 * Lets inheriting classes process an item selection made through the list 
 * widget.
 * This slot is connected to the doubleClicked() and returnPressed()
 * signals of the list widget.
 */
void SearchList::slotItemSelected(QListViewItem* pItem)
{
	processItemSelected(pItem);
	m_pEdit->setText("");
}

/**
 * Lets inheriting classes process an item selection made through the edit 
 * widget.
 * This slot is connected to the returnPressed() signal of the edit widget.
 */
void SearchList::slotItemSelected()
{
	QListViewItem* pItem;

	if ((pItem = m_pList->selectedItem()) != NULL) {
		m_pEdit->setText(pItem->text(m_nSearchCol));
		processItemSelected(pItem);
	}
	
	m_pEdit->setText("");
}

#define SEARCH_MATCH(pItem) \
	pItem->text(m_nSearchCol).startsWith(m_pEdit->text())

/**
 * Sets a new current item based on key events in the edit box.
 * This slot is connected to the keyPressed() signal of the edit widget.
 * @param	pKey	The key evant passed by the edit box
 */
void SearchList::slotKeyPressed(QKeyEvent* pKey)
{
	QListViewItem* pItem, * pNewItem;
	int nPageSize, nPos;

	// Select the current item, or the first one if there is no current item
	pItem = m_pList->currentItem();
		
	// Set a new current item based on the pressed key
	switch (pKey->key()) {
	case  Qt::Key_Up:
		if (pItem) {
			for (pNewItem = pItem->itemAbove(); 
				pNewItem && !SEARCH_MATCH(pNewItem);
				pNewItem = pNewItem->itemAbove());
				
			if (pNewItem)
				pItem = pNewItem;
		}
		break;
		
	case  Qt::Key_Down:
		if (pItem) {
			for (pNewItem = pItem->itemBelow(); 
				pNewItem && !SEARCH_MATCH(pNewItem);
				pNewItem = pNewItem->itemBelow());
				
			if (pNewItem)
				pItem = pNewItem;
		}
		break;
	
	case  Qt::Key_PageUp:
		nPageSize = m_pList->visibleHeight() / pItem->height();
		for (nPos = 0; 
			pItem && pItem->itemAbove() && (nPos < nPageSize); 
			nPos++)
			pItem = pItem->itemAbove();
		break;
		
	case  Qt::Key_PageDown:
		nPageSize = m_pList->visibleHeight() / pItem->height();
		for (nPos = 0; 
			pItem && pItem->itemBelow() && (nPos < nPageSize); 
			nPos++)
			pItem = pItem->itemBelow();
		break;
	
	default:
		pKey->ignore();
		return;
	}

	// Select the first item if no other item was selected
	if (pItem == NULL)
		pItem = m_pList->firstChild();
		
	// Select the new item
	if (pItem) {
		m_pList->setSelected(pItem, true);
		m_pList->ensureItemVisible(pItem);
	}
}

#include "searchlist.moc"
