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

#ifndef QUERYVIEWDLG_H
#define QUERYVIEWDLG_H

#include "queryviewlayout.h"
#include "queryview.h"

class QueryViewDriver;

/**
 * A dialogue for running and displaying queries.
 * The dialogue is built around a QueryView widget, and uses a QueryViewDriver
 * object to run a query. The dialogue is used in different contexts, such
 * as executing quick definitions, displaying multiple call nodes in a graph,
 * etc.
 * The dialogue is always modal, but should not be launched using exec().
 * Instead, it is created as a modeless, hidden, dialogue. It then becomes
 * modal (and visible) only if and when information is available (@see
 * QueryViewDriver::show()).
 * @author Elad Lahav
 */
class QueryViewDlg : public QueryViewLayout
{
	Q_OBJECT

public:
	QueryViewDlg(uint nFlags = 0, QWidget* pParent = 0, 
		const char* szName = 0);
	~QueryViewDlg();

	/** These flags control the behaviour of the dialogue. */
	enum { CloseOnSelect = 0x1, DestroyOnClose = 0x2,
		DestroyOnSelect = CloseOnSelect | DestroyOnClose };
	
	void query(uint nType, const QString& sText);

	QueryView::Iterator getIterator();

signals:
	/**
	 * Propagates the lineRequested() signal of the embedded QueryView
	 * widget.
	 * @param	sFile	The "File" field of the selected record
	 * @param	nLine	The "Line" field of the selected record
	 */
	void lineRequested(const QString& sFile, uint nLine);

private:
	/** Flags the control the behaviour of the dialogue. */
	uint m_nFlags;
	
	/** Used for running Cscope queries and displaying their results in the
		view. */
	QueryViewDriver* m_pDriver;
	
private slots:
	void slotShow();
	void slotLineRequested(const QString&, uint);
};

#endif

