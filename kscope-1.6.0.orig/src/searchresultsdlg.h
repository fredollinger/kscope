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

#ifndef SEARCHRESULTSDLG_H
#define SEARCHRESULTSDLG_H

#include <qregexp.h>
#include <qcheckbox.h>
#include "searchresultslayout.h"

/**
 * A dialogue for defining searches on query results.
 * The dialogue is activated from the query results menu.
 * @author Elad Lahav
 */
class SearchResultsDlg : public SearchResultsLayout
{
	Q_OBJECT

public:
	SearchResultsDlg(QWidget* pParent = 0, const char* szName = 0);
	~SearchResultsDlg();

	void setColumn(int);
	int getColumn();
	void getPattern(QRegExp&);
		
	/**
	 * @return	true if the search pattern should be negated, false otherwise
	 */
	bool isNegated() { return m_pNegateCheck->isChecked(); }
	
protected slots:
	virtual void accept();
	
private:
	/** Possible search types. */
	enum { PlainText = 0, RegExp, SimpRegExp };
	
	/** Remembers the last search type. */
	static int s_nType;
	
	/** Remembers the last value of the Case Sensitive check-box. */
	static bool s_bCaseSensitive;
	
	/** Remembers the last value of the Negate Search check-box. */
	static bool s_bNegate;
};

#endif
