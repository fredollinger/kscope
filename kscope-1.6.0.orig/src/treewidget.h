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

#ifndef TREEWIDGET_H
#define TREEWIDGET_H

#include "queryview.h"
#include "encoder.h"

class QueryViewDriver;

/**
 * A tree-like widget displaying a hierarchical list of functions.
 * The widget has two modes: called functions and calling functions. Depending
 * on this mode, child items represent functions called by or calling their
 * parent item.
 * @author Elad Lahav
 */
class TreeWidget : public QueryView
{
	Q_OBJECT
	
public:
	TreeWidget(QWidget* pParent = 0, const char* szName = 0);
	~TreeWidget();
	
	/**
	 * The type of tree to display.
	 */
	enum Mode { Called, Calling };
	
	void setMode(Mode);
	void setRoot(const QString&);
	void queryRoot();
	void save(FILE*);
	
	virtual void addRecord(const QString&, const QString&, const QString&,
		const QString&, QListViewItem*);
	virtual void queryFinished(uint, QListViewItem*);
	
protected slots:
	virtual void slotSearch(QListViewItem*, const QRegExp&, int);
	virtual void slotShowAll(QListViewItem*);
	
private:
	/** The CscopeFrontend query type to use (based on the current mode). */
	uint m_nQueryType;
	
	/** Runs queries and outputs the results as tree items. */
	QueryViewDriver* m_pDriver;
	
	void saveItems(QListViewItem*, QTextStream&, Encoder&);
	
private slots:
	void slotQueryItem(QListViewItem*);
};

#endif
