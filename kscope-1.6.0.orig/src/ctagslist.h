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

#ifndef CTAGSLIST_H
#define CTAGSLIST_H

#include <qwidget.h>
#include <qpixmap.h>
#include <qmemarray.h>
#include "searchlist.h"
#include "frontend.h"

class CtagsListItem;
class CtagsToolTip;

/**
 * Displays a list of tags for a source file.
 * The list is embedded inside an editor page. Whenever a new document is
 * opened in that editor, or the current document is changed and saved, the
 * source file is re-scanned for tags, and the results are displayed in this
 * list.
 * @author Elad Lahav
 */

class CtagsList : public SearchList
{
   Q_OBJECT

public:
	CtagsList(QWidget* pParent = 0, const char* szName = 0);
	~CtagsList();

	void applyPrefs();
	void gotoLine(uint);
	void clear();
	
	virtual bool getTip(QListViewItem*, QString&);
	
public slots:
	void slotDataReady(FrontendToken*);
	void slotCtagsFinished(uint);
	
signals:
	/** 
	 * Emitted when the size of the list is changed (usually as the result
	 * of moving the separator between the list and the editor.)
	 */
	void resized();
	
	/**
	 * Emitted when the user selects a tag item from the list.
	 * @param	nLine	The line number associated with the selected tag
	 */
	void lineRequested(uint nLine);
	
protected:
	virtual void resizeEvent(QResizeEvent*);
	virtual void processItemSelected(QListViewItem*);
	
private:
	/** An array of pointers to the tag list items, sorted by the line
		number. */
	QMemArray<CtagsListItem*> m_arrLines;
	
	/** The number of items in the tag list. */
	uint m_nItems;
	
	/** The last item selected by gotoLine(). */
	uint m_nCurItem;
	
	/** This value is set to 'false' while the Ctags process is running. */
	bool m_bReady;
	
	/** The current line number. */
	uint m_nCurLine;
	
	/** Stores the requested line number during Ctags operation. */
	uint m_nPendLine;

private slots:
	void slotSortChanged(int);
};

#endif
