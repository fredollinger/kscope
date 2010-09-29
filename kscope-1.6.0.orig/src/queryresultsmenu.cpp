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

#include <klocale.h>
#include "queryresultsmenu.h"

/**
 * Class constructor.
 * @param	pParent	Parent widget
 * @param	szName	Optional object name
 */
QueryResultsMenu::QueryResultsMenu(QWidget* pParent, const char* szName) :
	QPopupMenu(pParent, szName),
	m_pItem(NULL)
{
	// Create the menu
	insertItem(i18n("&View Source"), this, SLOT(slotViewSource()), 0,
		ViewSource);
	insertItem(i18n("Find &Definition"), this, SLOT(slotFindDef()), 0, 
		FindDef);
	insertSeparator();
	insertItem(i18n("&Copy"), this, SLOT(slotCopy()), 0, Copy);
	insertSeparator();
	insertItem(i18n("&Filter..."), this, SLOT(slotFilter()), 0, Filter);
	insertItem(i18n("&Show All"), this, SIGNAL(showAll()), 0, ShowAll);
	insertSeparator();
	insertItem(i18n("&Remove Item"), this, SLOT(slotRemove()), 0, Remove);
}

/**
 * Class destructor.
 */
QueryResultsMenu::~QueryResultsMenu()
{
}

/**
 * Displays the popup-menu at the requested coordinates.
 * @param	pItem	The item on which the menu was requested
 * @param	ptPos	The requested position for the menu
 * @param	nCol	The column over which the menu was requested, -1 if no
 *					column is associated with the request
 */
void QueryResultsMenu::slotShow(QListViewItem* pItem, const QPoint& ptPos, 
	int nCol)
{
	// Save the requested item and column number to use in signals
	m_pItem = pItem;
	m_nCol = nCol;
	
	if (m_pItem == NULL) {
		// No item selected, disable everything but the "Filter" and "Show All" 
		// items
		setItemEnabled(ViewSource, false);
		setItemEnabled(FindDef, false);
		setItemEnabled(Copy, false);
		setItemEnabled(Remove, false);
	}
	else {
		// Item selected, enable record-specific actions
		setItemEnabled(ViewSource, true);
		setItemEnabled(Copy, true);
		setItemEnabled(Remove, true);
			
		// The "Find Definition" item should only be enabled if the mouse
		// was clicked over a valid function name 
		setItemEnabled(FindDef, (m_nCol == 0) && 
			(m_pItem->text(0) != "<global>"));

		// Set menu contents according to the column number
		switch (m_nCol) {
		case 0:
			changeItem(Copy, "&Copy Function");
			break;
			
		case 1:
			changeItem(Copy, "&Copy File");
			break;
			
		case 2:
			changeItem(Copy, "&Copy Line Number");
			break;
			
		case 3:
			changeItem(Copy, "&Copy Text");
			break;
			
		default:
			m_nCol = 0;
		}
	}
	
	// Show the menu
	popup(ptPos);
}

/**
 * Emits the viewSource() signal.
 * This slot is activated when the "View Source" item is selected.
 */
void QueryResultsMenu::slotViewSource()
{
	if (m_pItem != NULL)
		emit viewSource(m_pItem);
}

/**
 * Emits the findDef() signal.
 * This slot is activated when the "Find Definition" item is selected.
 */
void QueryResultsMenu::slotFindDef()
{
	if (m_pItem != NULL)
		emit findDef(m_pItem->text(0));
}

/**
 * Emits the copy() signal.
 * This slot is activated when the "Copy [Column]" item is selected.
 */
void QueryResultsMenu::slotCopy()
{
	if (m_pItem != NULL)
		emit copy(m_pItem, m_nCol);
}

/**
 * Emits the filter() signal.
 * This slot is activated when the "Filter..." item is selected.
 */
void QueryResultsMenu::slotFilter()
{
	emit filter(m_nCol);
}

/**
 * Emits the remove() signal.
 * This slot is activated when the "Remove" item is selected.
 */
void QueryResultsMenu::slotRemove()
{
	if (m_pItem != NULL)
		emit remove(m_pItem);
} 

#include "queryresultsmenu.moc"
