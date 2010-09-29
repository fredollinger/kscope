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

#ifndef SYMBOLDLG_H
#define SYMBOLDLG_H

#include <qregexp.h>
#include "symbollayout.h"
#include "cscopefrontend.h"

/**
 * A dialogue that prompts the user for the text of a query.
 * When a query is requested, the user needs to fill in the required
 * information (usually a symbol name). This dialogue allows the user to
 * enter this information, as well as complete a symbol name, and use
 * previously entered text.
 * @author Elad Lahav
 */
 
class SymbolDlg : public SymbolLayout
{
	Q_OBJECT

public:
	SymbolDlg(QWidget* pParent = 0, const char* szName = 0);
	~SymbolDlg();

	enum { Reference = 0, Definition, Called, Calling, Text, Pattern,
		FileName, Including, CallTree };
	
	void setType(uint);
	void setSymbol(const QString&);
	void setHistory(QStringList&);
	QString getSymbol() const;
	uint getType() const;
	
	static QString promptSymbol(QWidget*, uint&, const QString&);
	static uint getQueryType(uint);
	static void resetHistory() { s_slHistory.clear(); }
	
private:
	/** A cscope process used for symbol completion. */
	CscopeFrontend* m_pCscope;
	
	/** A regular expression for extracting the symbol name out of the text
		token of a Cscope record. 
		@see note in slotHintDataReady(). */
	QRegExp m_reHint;
	
	/** Displays query progress information. */
	CscopeProgress m_progress;
	
	static QStringList s_slHistory;
	
private slots:
	void slotHintClicked();
	void slotHintDataReady(FrontendToken*);
	void slotHintItemSelected(QListViewItem*);
	void slotHintOptionChanged(bool);
	void slotHintProgress(int, int);
	void slotHintFinished(uint);
	void slotTypeChanged(int);
};

#endif

