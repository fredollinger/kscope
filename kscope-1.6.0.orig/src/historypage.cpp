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
#include "historypage.h"
#include "historyview.h"

int HistoryPage::s_nMaxPageID = 0;

/**
 * Class constructor.
 * @param	pParent	The parent widget
 * @param	szName	The widget's name
 */
HistoryPage::HistoryPage(QWidget* pParent, const char* szName) :
	QueryPageBase(pParent, szName),
	m_nPageID(++s_nMaxPageID)
{
	m_pView = new HistoryView(this);
	
	connect(m_pView, SIGNAL(lineRequested(const QString&, uint)), this,
		SIGNAL(lineRequested(const QString&, uint)));
	
	// Set colours and font
	applyPrefs();
}

/**
 * Class destructor.
 */
HistoryPage::~HistoryPage()
{
	if (s_nMaxPageID == m_nPageID)
		s_nMaxPageID--;
}

/**
 * Creates a new position history record at the top of the list.
 * @param	sFile	The file name associated with the record
 * @param	nLine	The line number
 * @param	sText	The text of the file at the given line
 */
void HistoryPage::addRecord(const QString& sFile, uint nLine, 
	const QString& sText)
{
	HistoryItem* pItem, * pNextItem;
	
	pItem = (HistoryItem*)m_pView->currentItem();
	if (pItem != NULL) {
		// Do not add duplicate items
		if ((pItem->text(1) == sFile) && (pItem->text(2).toUInt() == nLine))
			return;
			
		// Remove all items above the current one, so the new item is added to
		// the top of the list
		pItem = pItem->m_pPrevSibling;
		while (pItem != NULL) {
			pNextItem = pItem;
			pItem = pItem->m_pPrevSibling;
			delete pNextItem;
		}
	}
	
	// Create the new item at the top of the list
	m_pView->addRecord("", sFile, QString::number(nLine), sText, NULL);
}

/**
 * Creates a new history item.
 * This version is used when history records are read from a file.
 * @param	sFile	The file name
 * @param	sLine	The line number
 * @param	sText	The contents of the line
 */
void HistoryPage::addRecord(const QString&, const QString& sFile,
	const QString& sLine, const QString& sText)
{
	m_pView->addRecord("", sFile, sLine, sText, NULL);
}

/**
 * Creates a tab caption for this page, based on the unique page ID.
 * @param	bBrief	true to use brief caption names, false otherwise
 */
QString HistoryPage::getCaption(bool bBrief) const
{
	return (bBrief ? QString(i18n("HIS ")) : QString(i18n("History "))) +
		QString::number(m_nPageID);
}

/**
 * Creates a unique file name for saving the contents of the history page.
 * @return	The unique file name to use
 */
QString HistoryPage::getFileName(const QString&) const
{
	return QString("History_") + QString::number(m_nPageID);
}

#include "historypage.moc"
