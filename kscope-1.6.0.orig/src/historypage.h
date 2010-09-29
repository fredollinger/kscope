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

#ifndef HISTORYPAGE_H
#define HISTORYPAGE_H

#include "querypagebase.h"

/**
 * A QueryWidget page for holding position history.
 * @author Elad Lahav
 */
class HistoryPage : public QueryPageBase
{
	Q_OBJECT

public:
	HistoryPage(QWidget* pParent = 0, const char* szName = 0);
	~HistoryPage();
	
	void addRecord(const QString&, uint, const QString&);
	
	virtual QString getCaption(bool bBrief = false) const;

protected:
	virtual void addRecord(const QString&, const QString&, const QString&,
		const QString&);
	virtual QString getFileName(const QString&) const;
	
	/**
	 * @return	Always true, since History files do not contain a header
	 */
	virtual bool readHeader(QTextStream&) { return true; }
	
	/**
	 * This method does nothing, since History files do not contain a header.
	 */	
	virtual void writeHeader(QTextStream&) {}

private:
	/** A unique ID used to create a tab caption for this page. */
	int m_nPageID;

	/** Used to generate the unique page ID for each object. */
	static int s_nMaxPageID;
};

#endif
