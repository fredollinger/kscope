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

#include <qfile.h>
#include <klocale.h>
#include "querypage.h"
#include "queryview.h"
#include "queryviewdriver.h"

const char* QUERY_TYPES[][2] = {
	{ "References to ", "REF " },
	{ "Definition of ", "DEF " },
	{ "Functions called by ", "<-- " },
	{ "Functions calling ", "-->" },
	{ "Search for ", "TXT " },
	{ "", "" },
	{ "EGrep Search for ", "GRP " },
	{ "Files named ", "FIL " },
	{ "Files #including ", "INC " },
	{ "Query", "Query" }
};

/**
 * Class constructor.
 * @param	pParent	The parent widget
 * @param	szName	The widget's name
 */
QueryPage::QueryPage(QWidget* pParent, const char * szName) :
	QueryPageBase(pParent, szName),
	m_nType(CscopeFrontend::None)
{
	m_pView = new QueryView(this);
	m_pDriver = new QueryViewDriver(m_pView, this);
	
	connect(m_pView, SIGNAL(lineRequested(const QString&, uint)), this,
		SIGNAL(lineRequested(const QString&, uint)));
	
	// Set colours and font
	applyPrefs();
}

/**
 * Class destructor.
 */
QueryPage::~QueryPage()
{
}

/**
 * Associates the page with a query's type and text.
 * @param	nType	The type of the query
 * @param	sText	The text of the query
 */
void QueryPage::query(uint nType, const QString& sText)
{
	m_nType = nType;
	m_sText = sText;
	m_sName = getCaption();
	
	m_pDriver->query(nType, sText);
}

/**
 * Resets the query page by deleting all records.
 */
void QueryPage::clear()
{
	m_pView->clear();
	m_nType = CscopeFrontend::None;
	m_sText = QString();
	m_sName = QString();
}

/**
 * @return	true if a query is currently running in this page, false otherwise
 */
bool QueryPage::isRunning()
{
	return m_pDriver->isRunning();
}

/** 
 * Constructs a caption for this page, based on the query's type and text.
 * @param	bBrief	true to use a shortened version of the caption, false
 *					(default) for the full version
 * @return	The caption for this page
 */
QString QueryPage::getCaption(bool bBrief) const
{
	return QString(QUERY_TYPES[m_nType][bBrief ? 1 : 0] + m_sText);
}

/**
 * Creates a new query result item.
 * @param	sFile	The file name
 * @param	sFunc	The function defining the scope of the result
 * @param	sLine	The line number
 * @param	sText	The contents of the line
 */
void QueryPage::addRecord(const QString& sFile, const QString& sFunc,
	const QString& sLine, const QString& sText)
{
	new QListViewItem(m_pView, sFile, sFunc, sLine, sText);
}

/**
 * Creates a unique file name for saving the contents of the query page.
 * @param	sProjPath	The full path of the project directory
 * @return	The unique file name to use
 */
QString QueryPage::getFileName(const QString& sProjPath) const
{
	QString sFileName, sFileNameBase;
	int i = 0;
	
	// Do nothing if not initialised
	if (m_sName.isEmpty())
		return "";
	
	// Create a unique file name
	sFileNameBase = m_sName;
	sFileNameBase.replace(' ', '_');
	do {
		sFileName = sFileNameBase + QString::number(++i);
	} while (QFile(sProjPath + "/" + sFileName).exists());
	
	return sFileName;
}

/**
 * Reads query parameters from a file.
 * This mehtod is used as part of the loading process.
 * @param	str	A text stream set to the correct place in the file
 * @return	true if successful, false otherwise
 */
bool QueryPage::readHeader(QTextStream& str)
{
	QString sTemp;
	
	// Read the query name
	m_sName = str.readLine();
	if (m_sName == QString::null || m_sName.isEmpty())
		return false;
		
	// Read the query's type
	sTemp = str.readLine();
	if (sTemp == QString::null || sTemp.isEmpty())
		return false;
	
	// Convert the type string to an integer
	m_nType = sTemp.toUInt();
	if (m_nType >= CscopeFrontend::None) {
		m_nType = CscopeFrontend::None;
		return false;
	}		
				
	// Read the query's text
	m_sText = str.readLine();
	if (m_sText == QString::null || m_sText.isEmpty())
		return false;

	return true;
}

/**
 * Writes query parameters to a file.
 * This mehtod is used as part of the storing process.
 * @param	str	A text stream set to the correct place in the file
 */
void QueryPage::writeHeader(QTextStream& str)
{
	str << m_sName << "\n" << m_nType << "\n" << m_sText << "\n";
}

#include "querypage.moc"
