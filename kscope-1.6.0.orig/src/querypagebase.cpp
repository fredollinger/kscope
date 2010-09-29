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
#include "querypagebase.h"
#include "queryview.h"
#include "kscopeconfig.h"

#define FILE_VERSION	"VERSION=2"

/**
 * Class constructor.
 * @param	pParent	The parent widget
 * @param	szName	The widget's name
 */
QueryPageBase::QueryPageBase(QWidget* pParent, const char* szName) :
	QHBox(pParent, szName),
	m_bLocked(false)
{
}

/**
 * Class destructor.
 */
QueryPageBase::~QueryPageBase()
{
}

/**
 * Sets the list's colours and font, according the user's preferences.
 */
void QueryPageBase::applyPrefs()
{
	// Apply colour settings
	m_pView->setPaletteBackgroundColor(Config().getColor(
		KScopeConfig::QueryWindowBack));
	m_pView->setPaletteForegroundColor(Config().getColor(
		KScopeConfig::QueryWindowFore));
	m_pView->setFont(Config().getFont(KScopeConfig::QueryWindow));
}

/**
 * Restores a locked query from the given query file.
 * NOTE: The query file is deleted when loading is complete.
 * @param	sProjPath	The full path of the project directory
 * @param	sFileName	The name of the query file to load
 * @return	true if successful, false otherwise
 */
bool QueryPageBase::load(const QString& sProjPath, const QString& sFileName)
{
	QString sTemp, sFile, sFunc, sLine, sText;
	int nState;
	
	// Try to open the query file for reading
	QFile file(sProjPath + "/" + sFileName);
	if (!file.open(IO_ReadOnly))
		return false;
	
	{
		// Use a new scope for the QTextStream object, to ensure its 
		// destruction before the file is deleted
		QTextStream str(&file);
		
		// Make sure the file's version is correct
		sTemp = str.readLine();
		if (sTemp != FILE_VERSION) {
			file.remove();
			return false;
		}

		// Try to read the file header
		if (!readHeader(str))
			return false;
		
		// Read query records
		sTemp = str.readLine();
		nState = 0;
		while (sTemp != QString::null) {
			switch (nState) {
			// File path
			case 0:
				sFile = sTemp;
				break;
				
			// Function name
			case 1:
				sFunc = sTemp;
				break;
				
			// Line number
			case 2:
				sLine = sTemp;
				break;
				
			// Text string
			case 3:
				sText = sTemp;
				addRecord(sFile, sFunc, sLine, sText);
				break;
			}
			
			nState = (nState + 1) % 4;
			sTemp = str.readLine();
		}
	}
	
	// Delete the query file
	file.remove();
	
	return true;
}

/**
 * Writes the contents of the page to a file.
 * This method is called for pages that shoukld be stored before the owner 
 * project is closed (@see shouldSave()).
 * @param	sProjPath	The full path of the project directory
 * @param	sFileName	Holds the file name to which the page was saved, upon
 *						return
 * @return	true if successful, false otherwise
 */
bool QueryPageBase::save(const QString& sProjPath, QString& sFileName)
{
	QListViewItemIterator itr(m_pView);

	// Get the file name to use
	sFileName = getFileName(sProjPath);
	if (sFileName.isEmpty())
		return false;
		
	// Open the query file for writing
	QFile file(sProjPath + "/" + sFileName);
	if (!file.open(IO_WriteOnly))
		return false;
	
	QTextStream str(&file);
	
	// Write the version string
	str << FILE_VERSION << "\n";
	
	writeHeader(str);
		
	// Write all records
	for(; itr.current(); ++itr) {
		str << itr.current()->text(0) << "\n" 
			<< itr.current()->text(1) << "\n"
			<< itr.current()->text(2) << "\n"
			<< itr.current()->text(3) << "\n";
	}
	
	return true;
}

/**
 * Selects the next record in the view.
 */
void QueryPageBase::selectNext()
{
	m_pView->selectNext();
}

/**
 * Selects the previous record in the view.
 */
void QueryPageBase::selectPrev()
{
	m_pView->selectPrev();
}

#include "querypagebase.moc"
