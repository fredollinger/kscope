/***************************************************************************
 *
 * Copyright (C) 2007 Elad Lahav (elad_lahav@users.sourceforge.net)
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
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AS IS'' AND ANY EXPRESS OR
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

#include "bookmarksdlg.h"
#include "queryview.h"

BookmarksDlg::BookmarksDlg(QWidget* pParent, const char* szName) :
	BookmarksLayout (pParent, szName, true)
{
	// Do not show the "Function" column
	m_pView->setColumnWidth(0, 0);
	
	// Handle requests for source locations
	connect(m_pView, SIGNAL(lineRequested(const QString&, uint)), this,
		SLOT(slotLineRequested(const QString&, uint)));
}

BookmarksDlg::~BookmarksDlg()
{
}

void BookmarksDlg::getBookmark(QString& sPath, uint& nLine)
{
	sPath = m_sPath;
	nLine = m_nLine;
}

void BookmarksDlg::slotLineRequested(const QString& sPath, uint nLine)
{
	m_sPath = sPath;
	m_nLine = nLine;
	accept();
}

#include "bookmarksdlg.moc"

