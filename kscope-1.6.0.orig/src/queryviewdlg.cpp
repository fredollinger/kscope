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

#include "queryviewdlg.h"
#include "queryviewdriver.h"

/**
 * Class constructor.
 * @param	nFlags	Controls the behaviour of the diaogue
 * @param	pParent	The parent widget
 * @param	szName	The widget's name
 */
QueryViewDlg::QueryViewDlg(uint nFlags, QWidget* pParent, 
	const char* szName) :
	QueryViewLayout(pParent, szName),
	m_nFlags(nFlags)
{
	// Set the destructive flag, if required
	if (nFlags & DestroyOnClose)
		setWFlags(getWFlags() | WDestructiveClose);
		
	// Create a driver for running queries
	m_pDriver = new QueryViewDriver(m_pView, this);
		
	// Show the dialogue when instructed by the driver
	connect(m_pView, SIGNAL(needToShow()), this, SLOT(slotShow()));	
	
	// Propagate the lineRequested() signal from the QueryView object
	connect(m_pView, SIGNAL(lineRequested(const QString&, uint)), this,
		SLOT(slotLineRequested(const QString&, uint)));
		
	// Make the dialogue modal
	setModal(true);
}

/**
 * Class destructor.
 */
QueryViewDlg::~QueryViewDlg()
{
}

/**
 * Starts a query.
 * @param	nType	The type of the query
 * @param	sText	The query's text
 */
void QueryViewDlg::query(uint nType, const QString& sText)
{
	m_pDriver->query(nType, sText);
}

/**
 * Make the dialogue visible when instructed by the driver.
 * This slot is connected to the show() signal emitted by the QueryViewDriver
 * object.
 */
void QueryViewDlg::slotShow()
{
	show();
}

/**
 * Propagates the lineRequested() signal from the view object.
 * If the CloseOnSelect flag is set, the dialogue is closed.
 * This slot is connected to the lineRequested() signal emitted by the
 * QueryView widget.
 */
void QueryViewDlg::slotLineRequested(const QString& sFileName, uint nLine)
{
	emit lineRequested(sFileName, nLine);
	
	if (m_nFlags & CloseOnSelect)
		close();
}

/**
 * @return	A QueryView iterator initialised to the beginning of the result
 *			list
 */
QueryView::Iterator QueryViewDlg::getIterator()
{
	return m_pView->getIterator();
}

#include "queryviewdlg.moc"
