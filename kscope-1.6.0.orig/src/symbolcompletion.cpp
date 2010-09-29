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

#include <qtimer.h> 
#include <klocale.h>
#include "symbolcompletion.h"

bool SymbolCompletion::s_bACEnabled;
uint SymbolCompletion::s_nACMinChars;
uint SymbolCompletion::s_nACDelay;
uint SymbolCompletion::s_nACMaxEntries;

/**
 * Class constructor.
 * @param	pEditor	The editor object for which symbol completion is required
 * @param	pParent	Parent object
 * @param	szName	Optional object name
 */
SymbolCompletion::SymbolCompletion(SymbolCompletion::Interface* pEditor, 
	QObject* pParent, const char* szName) :
 	QObject(pParent, szName),
	m_pEditor(pEditor),
	m_pCCObject(NULL)
{
	// Initialise member objects
	m_pCscope = new CscopeFrontend();
	m_pAutoCompTimer = new QTimer(this);
	
	// Add entries to the completion list when they are available
	connect(m_pCscope, SIGNAL(dataReady(FrontendToken*)), this, 
		SLOT(slotAddEntry(FrontendToken*)));
	
	// Show the completion list when the query finishes
	connect(m_pCscope, SIGNAL(finished(uint)), this, 
		SLOT(slotQueryFinished(uint)));
			
	// Initiate automatic symbol completion when timer expires
	connect(m_pAutoCompTimer, SIGNAL(timeout()), this,
		SLOT(slotAutoCompleteTimeout()));
}

/**
 * Class destructor.
 */
SymbolCompletion::~SymbolCompletion()
{
	delete m_pCscope;
}

/**
 * Stops a completion process.
 * This includes killing a running query, and stoping the auto-completion
 * timer.
 */
void SymbolCompletion::abort()
{
	if (m_pCscope->isRunning())
		m_pCscope->kill();
		
	m_pAutoCompTimer->stop();
}

/**
 * Configures auto-completion parameters.
 * @param	bEnabled	true to enable auto-completion, false otherwise
 * @param	nMinChars	Minimal number of characters a symbol needs to start
 *						auto-completion
 * @param	nDelay		Auto-completion time interval (in milliseconds)
 * @param	nMaxEntries	The maximal number of completion entries
 */
void SymbolCompletion::initAutoCompletion(bool bEnabled, uint nMinChars, 
	uint nDelay, uint nMaxEntries)
{
	s_bACEnabled = bEnabled;
	s_nACMinChars = nMinChars;
	s_nACDelay = nDelay;
	s_nACMaxEntries = nMaxEntries;
}

/**
 * Starts a completion process immediately for the symbol currently under the
 * cursor in the editor object.
 * Symbol completion is only available if the cursor is positioned at the end
 * of the symbol.
 */
void SymbolCompletion::slotComplete()
{
	QString sSymbol;
	uint nPosInWord;
	
	// Read the symbol currently under the cursor
	sSymbol = m_pEditor->getWordUnderCursor(&nPosInWord);
	
	// The completion was triggered by user
	m_bAutoCompletion = false;
	
	// start completion process, prefix is only on the left from the cursor
	complete(sSymbol.left(nPosInWord));
}

/**
 * Initiates an auto-completion timer.
 * When the timer times-out, is starts the symbol completion process.
 */
void SymbolCompletion::slotAutoComplete()
{
	if (s_bACEnabled)
		m_pAutoCompTimer->start(s_nACDelay, true);
}

/**
 * Creates a list of possible completions to the symbol currently being
 * edited.
 * @param	sPrefix		The symbol to complete
 * @param	nMaxEntries	The maximal number of entries to display
 */
void SymbolCompletion::complete(const QString& sPrefix, int nMaxEntries)
{	
	// Create a regular expression to extract symbol names from the query
	// results
	m_reSymbol.setPattern(sPrefix + "[a-zA-Z0-9_]*");
	
	// If the new prefix is itself a prefix of the old one, we only need to
	// filter the current entries
	if (!m_sPrefix.isEmpty() && sPrefix.startsWith(m_sPrefix)) {
		filterEntries();
		m_sPrefix = sPrefix;
		slotQueryFinished(0);
		return;
	}
	
	// Prepare member variables
	m_sPrefix = sPrefix;
	m_nMaxEntries = nMaxEntries;
	m_elEntries.clear();
		
	// Run the code-completion query 
	m_pCscope->query(CscopeFrontend::Definition, sPrefix + ".*");
}

/**
 * Removes from the current completion list all symbols that do not match
 * the current regular expression.
 * This function is used to aviod requerying the database on certain
 * situations.
 */
void SymbolCompletion::filterEntries()
{
	EntryList::Iterator itr;
	
	// Iterate over the list and check each entry against the current RE
	for (itr = m_elEntries.begin(); itr != m_elEntries.end();) {
		if (m_reSymbol.search((*itr).text) == -1)
			itr = m_elEntries.erase(itr);
		else
			++itr;
	}
}

/**
 * Conevrts the completion list into a single-entry one, containing the given
 * message.
 * @param	sMsg	The text of the message to include in the list.
 */
void SymbolCompletion::makeErrMsg(const QString& sMsg)
{
	Entry entry;
	
	// Clear the current list
	m_elEntries.clear();
	
	// Create the message item and add it to the list
	entry.text = sMsg;
	entry.userdata = "NO_INSERT"; // The message should not be insertable
	m_elEntries.append(entry);
	
	// Make sure a new completion request will start a new query
	m_sPrefix = "";
}

/**
 * Creates a new entry in the list when a query record is available.
 * This slot is connected to the dataReady() signal of the CscopeFrontend
 * object.
 * @param	pToken	Points to the head of a record's linked-list
 */
void SymbolCompletion::slotAddEntry(FrontendToken* pToken)
{
	Entry entry;
	QString sText;
	
	// Do not add entries beyond the requested limit
	if (m_elEntries.count() > m_nMaxEntries)
		return;
	
	// Get the line text
	pToken = pToken->getNext()->getNext()->getNext();
	sText = pToken->getData();

	// Find the symbol within the line
	if (m_reSymbol.search(sText) == -1)
		return;

	// Add the new entry to the completion list	
	entry.text = m_reSymbol.capturedTexts().first();
	entry.userdata = "";
	entry.comment = sText;
	
	m_elEntries.append(entry);
}

/**
 * Displays a code completion list, based on the results of the last query.
 * @param	nRecords	(ingnored)
 */
void SymbolCompletion::slotQueryFinished(uint /* nRecords */)
{
	KTextEditor::CodeCompletionInterface* pCCI;
	uint nEntryCount;
	EntryList::Iterator itr;
	QString sPrevText;
	
	// Get the number of entries
	nEntryCount = m_elEntries.count();
	
	// Do not show the box the only completion option is the prefix itself
	if (m_bAutoCompletion && (nEntryCount == 1) &&
		(m_elEntries.first().text == m_sPrefix)) {
		return;
	}
	
	// Get a pointer to the CC interface
	m_pCCObject = m_pEditor->getCCObject();
	pCCI = dynamic_cast<KTextEditor::CodeCompletionInterface*>(m_pCCObject);
	if (!pCCI)
		return;
		
	// Insert the correct part of the completed symbol, when chosen by the
	// user
	connect(m_pCCObject, 
		SIGNAL(filterInsertString(KTextEditor::CompletionEntry*, QString*)),
		this, 
		SLOT(slotFilterInsert(KTextEditor::CompletionEntry*, QString*)));

	// Check the number of entries in the list	
	if (nEntryCount == 0) {
		// No completion options, display an appropriate message
		makeErrMsg(i18n("No matching completion found..."));
	}
	else if (nEntryCount > m_nMaxEntries) {
		// The query has resulted in too many entries, display an
		// appropriate message
		makeErrMsg(i18n("Too many options..."));
	}
	else {
		// Sort the entries
		m_elEntries.sort();
		
		// Make sure entries are unique
		for (itr = m_elEntries.begin(); itr != m_elEntries.end();) {
			if ((*itr).text == sPrevText) {
				itr = m_elEntries.erase(itr);
			}
			else {
				sPrevText = (*itr).text;
				++itr;
			}
		}
	}
	
	// Display the completion list
	pCCI->showCompletionBox(m_elEntries);
}

/**
 * Determines which part of the completion entry should be added to the code
 * when that entry is selected.
 * @param	pEntry			Points to the selected entry
 * @param	pTextToInsert	Contains the string to insert, upon return
 */
void SymbolCompletion::slotFilterInsert(KTextEditor::CompletionEntry* pEntry, 
	QString* pTextToInsert)
{
	// Insert the completed entry, unless it contains an error message
	if (pEntry->userdata.isEmpty())
		*pTextToInsert = pEntry->text.mid(m_sPrefix.length());
	else
		*pTextToInsert = "";
		
	// Disconnect the CC object signals
	disconnect(m_pCCObject, 0, this, 0);
	m_pCCObject = NULL;
}

/**
 * Checks if the current symbol is eligible for auto-completion, and if so,
 * starts the completion process.
 * Auto-completion is performed for symbols that have the required minimal
 * number of entries, and the cursor is positioned at the end of the word.
 * This slot is connected to the timeout() signal of the auto-completion
 * timer.
 */
void SymbolCompletion::slotAutoCompleteTimeout()
{	
	QString sPrefix;
	uint nPosInWord, nLength;
	
	// Read the symbol currently under the cursor
	sPrefix = m_pEditor->getWordUnderCursor(&nPosInWord);
	nLength = sPrefix.length();
	
	// Check conditions, and start the completion process
	if ((nLength >= s_nACMinChars) && (nPosInWord == nLength)) {
		// The completion was triggered by auto-completion
		m_bAutoCompletion = true;
		complete(sPrefix, s_nACMaxEntries);
	}
}

#include "symbolcompletion.moc"
