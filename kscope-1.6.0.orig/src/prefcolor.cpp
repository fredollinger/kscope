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

#include <qlistview.h>
#include <qpainter.h>
#include <kcolordialog.h>
#include "prefcolor.h"
#include "kscopeconfig.h"

/**
 * A list view item that shows the name of a GUI element and the colour
 * associated with it.
 * The colour is presented in the form of a rectangle filled with that
 * colour.
 * @author	Elad Lahav
 */
class ColorListItem : public QListViewItem
{
public:
	/**
	 * Class constructor.
	 * @param	pList	The owner list view
	 * @param	ce		The GUI element shown by this item
	 */
	ColorListItem(QListView* pList, KScopeConfig::ColorElement ce) : 
		QListViewItem(pList, Config().getColorName(ce), ""),
		m_ce(ce) {
		setColor(Config().getColor(ce));
	}
	
	/**
	 * @return	The GUI element shown by this item
	 */
	KScopeConfig::ColorElement getElement() { return m_ce; }
	
	/**
	 * Changes the colour associated with this item.
	 * The function assigns a pixmap to the item which shows a rectangle
	 * filled with the requested colour.
	 * The colour set by this function is returned by getColor().
	 * @param	clr		The colour to set
	 */
	void setColor(QColor clr) {
		QPixmap pix;
		QPainter painter;
		int nWidth, nHeight;
		
		// Remember the colour
		m_clr = clr;
		
		// Set the pixmap's size to fit into the list field
		nWidth = listView()->columnWidth(1) - 1;
		nHeight = height();
		pix.resize(nWidth, nHeight);
		
		// Draw on the pixmap
		painter.begin(&pix);
		painter.setBrush(clr);
		painter.drawRect(0, 0, nWidth, nHeight);
		painter.end();
		
		// Set the pixmap to the item
		setPixmap(1, pix);
	}
	
	/**
	 * @return	The colour associated with this item
	 */
	QColor getColor() { return m_clr; }
	
private:
	/** The GUI element shown by this item. */
	KScopeConfig::ColorElement m_ce;
	
	/** The colour associated with this item. */
	QColor m_clr;
};

/**
 * Class constructor.
 * @param	pParent	The parent widget
 * @param	szName	The widget's name
 */
PrefColor::PrefColor(QWidget* pParent, const char* szName) :
	PrefColorLayout(pParent, szName)
{
	m_pList->setColumnWidthMode(1, QListView::Manual);
	
	// Set initial values
	load();
}

/**
 * Class destructor.
 */
PrefColor::~PrefColor()
{
}

/**
 * Reads the current settings from the configuration object, and applies them
 * the the page's widget.
 */
void PrefColor::load()
{
	uint i;
	ColorListItem* pItem;
	
	// Create a list item for every GUI element
	for (i = 0; i <= KScopeConfig::LAST_COLOR; i++)
		pItem = new ColorListItem(m_pList, (KScopeConfig::ColorElement)i);
}

/**
 * Commits settings changes to the configuration object.
 */
void PrefColor::apply()
{
	ColorListItem* pItem;
	
	// Create a list item for every GUI element
	for (pItem = (ColorListItem*)m_pList->firstChild(); pItem != NULL; 
		pItem = (ColorListItem*)pItem->nextSibling()) {
		Config().setColor(pItem->getElement(), pItem->getColor());
	}
}

/**
 * Displays a colour selection dialogue when an item is selected.
 * If the user chooses a new colour, it is set to the selected item.
 * This slot is connected to both the doubleClicked() and the returnPressed()
 * signals of the list view.
 * @param	pItem	The selected item
 */
void PrefColor::slotItemSelected(QListViewItem* pItem)
{
	ColorListItem* pClrItem;
	QColor clr;
	
	pClrItem = (ColorListItem*)pItem;
	if (KColorDialog::getColor(clr, pClrItem->getColor()) == 
		QDialog::Accepted) {
		pClrItem->setColor(clr);
		emit modified();
	}
}

#include "prefcolor.moc"
