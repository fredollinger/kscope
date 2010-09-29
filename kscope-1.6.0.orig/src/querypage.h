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

#ifndef QUERYPAGE_H
#define QUERYPAGE_H

#include <qwidget.h>
#include <qlistview.h>
#include <qregexp.h>
#include "querypagebase.h"
#include "cscopefrontend.h"

class QueryViewDriver;

/**
 * A QueryWidget page that runs and displays Cscope queries.
 * The page uses a QueryViewDriver object to run queries, and an embedded
 * QueryView widget for displaying query results.
 * @author Elad Lahav
 */
class QueryPage : public QueryPageBase
{
   Q_OBJECT

public: 
	QueryPage(QWidget* pParent = 0, const char* szName = 0);
	~QueryPage();
	
	void query(uint, const QString&);
	void clear();
	bool isRunning();
	
	virtual QString getCaption(bool bBrief = false) const;
	
	/**
	 * @return	The type of query whose results are displayed in this page
	 */
	uint getQueryType() const { return m_nType; }
	
	/**
	 * @return	The text given as a parameter to the query whose results are
	 * displayed in this page.
	 */
	const QString& getQueryText() const { return m_sText; }

protected:
	virtual void addRecord(const QString&, const QString&, const QString&,
		const QString&);
	virtual QString getFileName(const QString&) const;
	virtual bool readHeader(QTextStream&);
	virtual void writeHeader(QTextStream&);

private:
	/** The type of query whose results are listed on this page. */
	uint m_nType;
	
	/** The text given as a parameter to the query. */
	QString m_sText;
	
	/** A formatted caption for this query, including the type of query and
		its text. */
	QString m_sName;
	
private:
	/** Runs Cscope queries whose results are displayed in this page. */
	QueryViewDriver* m_pDriver;
};

#endif
