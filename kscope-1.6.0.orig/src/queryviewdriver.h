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

#ifndef QUERYVIEWDRIVER_H
#define QUERYVIEWDRIVER_H

#include <qobject.h>
#include <qlistview.h>
#include "cscopefrontend.h"

class QueryView;

/**
 * Executes a Cscope query and displays the results in a QueryView widget.
 * This class is used in conjunction with QueryView to create a query
 * display object. The driver uses the view widget to display result records
 * of an executed query. It also uses the view as a parent widget for the
 * query progress bar.
 * @author Elad Lahav
 */
class QueryViewDriver : public QObject
{
	Q_OBJECT

public:
    QueryViewDriver(QueryView*, QObject* pParent = 0, const char* szName = 0);
    ~QueryViewDriver();

	void query(uint, const QString&, QListViewItem* pItem = NULL);
	
	/**
	 * @return	true if a query is currently running, false otherwise
	 */
	bool isRunning() { return m_bRunning; }
		
private:
	/** The view to which this object adds result records. */
	QueryView* m_pView;
	
	/** QueryView item passed to addRecord(). */
	QListViewItem* m_pItem;
	
	/** Displays query progress information. */
	CscopeProgress m_progress;
	
	/** This flag is set to true when a query is executed, and back to false
		when the the CscopeFrontend object emits the finished() signal. */
	bool m_bRunning;
	
private slots:
	void slotDataReady(FrontendToken*);
	void slotFinished(uint);
	void slotProgress(int, int);
};

#endif
