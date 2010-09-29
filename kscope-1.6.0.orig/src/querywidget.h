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

#ifndef QUERYWIDGET_H
#define QUERYWIDGET_H

#include <qlistview.h>
#include <qpopupmenu.h>
#include <kaction.h>
#include "querywidgetlayout.h"
#include "tabwidget.h"
#include "querypage.h"
#include "historypage.h"
#include "projectmanager.h"

/**
 * A tabbed-window holding Cscope query results pages.
 * @author Elad Lahav
 */

class QueryWidget : public QueryWidgetLayout
{
   Q_OBJECT

public:
	QueryWidget(QWidget* pParent = 0, const char* szName = 0);
	~QueryWidget();

	void addQueryPage();
	void initQuery(uint, const QString&);
	void applyPrefs();
	void loadPages(const QString&, const QStringList&);
	void savePages(const QString&, QStringList&);
	void addHistoryRecord(const QString&, uint, const QString&);
	void selectActiveHistory();
	void setPageMenu(QPopupMenu*, KToggleAction*);
	void getBookmarks(FileLocationList&);
	
	/**
	 * Enables/disables new history items.
	 * @param	bEnabled	true to enable new history items, false to 
	 *						disable
	 */
	void setHistoryEnabled(bool bEnabled) { m_bHistEnabled = bEnabled; }

public slots:
	void slotNewQueryPage();
	void slotLockCurrent(bool);
	void slotLockCurrent();
	void slotRefreshCurrent();
	void slotNextResult();
	void slotPrevResult();
	void slotCloseCurrent();
	void slotCloseAll();
	void slotHistoryPrev();
	void slotHistoryNext();
	
signals:
	/**
 	 * Emitted when the a lineRequested() signal is received from any of the
	 * currently open query pages. 
	 * @param	sPath	The full path of the requested source file
	 * @param	nLine	The requested line number
	 */
	void lineRequested(const QString& sPath, uint nLine);
	
	/**
	 * Emitted when new query page is requested by user
	 */
	void newQuery();
	
private:
	/** A popup menu with query page commands (new query, lock/unlock, close
		query, etc.). */
	QPopupMenu* m_pPageMenu;
	
	/** A toggle-like action for changing the locked state of a query. */
	KToggleAction* m_pLockAction;
	
	/** The active history page. */
	HistoryPage* m_pHistPage;
	
	/** Determines whether history items should be added to the active
		history page. */
	bool m_bHistEnabled;
	
	/** The number of query pages currently open. */
	int m_nQueryPages;
	
	void setPageCaption(QueryPageBase*);
	
	/**
	 * @return	The active page in the tab widget
	 */
	inline QueryPageBase* currentPage()	{ 
		return (QueryPageBase*)m_pQueryTabs->currentPage();
	}
	
	/**
	 * @param	pWidget	A query page to set as the current one
	 */
	inline void setCurrentPage(QWidget* pWidget) {
		if (pWidget)
			m_pQueryTabs->setCurrentPage(m_pQueryTabs->indexOf(pWidget));
	}
		
	/**
	 * Determines if a page is a history page.
	 * @param	pPage	The page to check
	 * @return	true if the given page is a history page
	 */
	inline bool isHistoryPage(QWidget* pPage) {
		return (dynamic_cast<HistoryPage*>(pPage) != NULL);
	}
	
	void setPageLocked(QueryPageBase*, bool);
	void findQueryPage();
	void findHistoryPage();
		
private slots:
	void slotRequestLine(const QString&, uint);
	void slotCurrentChanged(QWidget*);
	void slotClosePage(QWidget*);
	void slotContextMenu(const QPoint&);
	void slotContextMenu(QWidget*, const QPoint&);
};

#endif
