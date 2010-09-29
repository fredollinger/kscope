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

#include <qtooltip.h>
#include <klocale.h>
#include "tabwidget.h"
#include "kscopepixmaps.h"

/**
 * Class constructor.
 * @param	pParent	A pointer to the parent widget
 * @param	szName	Optional widget name
 */
TabWidget::TabWidget(QWidget* pParent, const char* szName) :
	KTabWidget(pParent, szName)
{
	// Create a popup menu
	m_pMenu = new QPopupMenu(this);
	
	// Set the current tab based on the menu selection
	connect(m_pMenu, SIGNAL(activated(int)), this, SLOT(setCurrentPage(int)));
	
	// Create a button at the top-right corner of the tab widget
	m_pButton = new QToolButton(this);
	m_pButton->setIconSet(Pixmaps().getPixmap(KScopePixmaps::TabList));
	QToolTip::add(m_pButton, i18n("Shows a list of all open tabs"));
	m_pButton->adjustSize();
	setCornerWidget(m_pButton, TopRight);
	
	// Show the popup-menu when the button is clicked
	connect(m_pButton, SIGNAL(clicked()), this, SLOT(slotShowTabList()));
}

/**
 * Class destructor.
 */
TabWidget::~TabWidget()
{
}

/**
 * Creates and displays a popup-menu containing all tab labels.
 * This slot is connected to the clicked() signal emitted by the list button.
 */
void TabWidget::slotShowTabList()
{
	int i;
	
	// Delete the previous menu
	m_pMenu->clear();

	// Create and populate the menu	
	for (i = 0; i < count(); i++)
		m_pMenu->insertItem(label(i), i);
		
	// Show the menu
	m_pMenu->popup(mapToGlobal(m_pButton->pos()));
}

#include "tabwidget.moc"
