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

#include <qfileinfo.h>
#include <kdeversion.h>
#include <ktexteditor/selectioninterface.h>
#include <ktexteditor/viewcursorinterface.h>
#include <ktexteditor/popupmenuinterface.h>
#include <ktexteditor/editinterface.h>
#include <kate/document.h>
#include <kate/view.h>
#include "editorpage.h"
#include "kscopeconfig.h"

/**
 * Class constructor.
 * @param	pDoc	The document object associated with this page
 * @param	pMenu	A Cscope queries popup menu to use with the editor
 * @param	pParent	The parent widget
 * @param	szName	The widget's name
 */
EditorPage::EditorPage(KTextEditor::Document* pDoc, QPopupMenu* pMenu,
	QTabWidget* pParent, const char* szName) : QHBox(pParent, szName),
	m_pParentTab(pParent),
	m_pDoc(pDoc),
	m_bOpen(false),
	m_bNewFile(false),
	m_sName(""),
	m_bWritable(true), /* new documents are writable by default */
	m_bModified(false),
	m_nLine(0),
	m_bSaveNewSizes(false)
{
	KTextEditor::PopupMenuInterface* pMenuIf;
	KTextEditor::ViewCursorInterface* pCursorIf;
	
	// Create code-completion objects (will be deleted by QObject destructor)
	m_pCompletion = new SymbolCompletion(this, this);
	
	// Set read-only mode, if required
	if (Config().getReadOnlyMode())
		m_pDoc->setReadWrite(false);
	
	// Create the child widgets
	m_pSplit = new QSplitter(this);
	m_pCtagsList = new CtagsList(m_pSplit);
	m_pView = m_pDoc->createView(m_pSplit);
	m_pSplit->setResizeMode(m_pCtagsList, QSplitter::KeepSize);
	
	// Perform tasks only when the document has been loaded completely
	connect(m_pDoc, SIGNAL(completed()), this, SLOT(slotFileOpened()));
	
	// Be notified when the text in the editor changes
	connect(m_pDoc, SIGNAL(textChanged()), this, SLOT(slotSetModified()));
	connect(m_pDoc, SIGNAL(undoChanged()), this, SLOT(slotUndoChanged()));
	
	// Store the sizes of the child windows when the tag list is resized
	// (since it may imply a move of the splitter divider)
	connect(m_pCtagsList, SIGNAL(resized()), this, SLOT(slotChildResized()));

	// Go to a symbol's line if it is selected in the tag list
	connect(m_pCtagsList, SIGNAL(lineRequested(uint)), this,
		SLOT(slotGotoLine(uint)));

	// Add Ctag records to the tag list
	connect(&m_ctags, SIGNAL(dataReady(FrontendToken*)), m_pCtagsList,
		SLOT(slotDataReady(FrontendToken*)));
		
	// Monitor Ctags' operation
	connect(&m_ctags, SIGNAL(finished(uint)), m_pCtagsList, 
		SLOT(slotCtagsFinished(uint)));
		
	// Set the context menu
	pMenuIf = dynamic_cast<KTextEditor::PopupMenuInterface*>(m_pView);
	if (pMenuIf)
		pMenuIf->installPopup(pMenu);

	// Emit a signal whenever the cursor's position changes
	pCursorIf = dynamic_cast<KTextEditor::ViewCursorInterface*>(m_pView);
	if (pCursorIf) {
		connect(m_pView, SIGNAL(cursorPositionChanged()), this,
			SLOT(slotCursorPosChange()));
	}
}

/**
 * Class destructor.
 */
EditorPage::~EditorPage()
{
}

/**
 * Returns a pointer to the editor document object embedded in this page.
 * @returns	the document pointer
 */
KTextEditor::Document* EditorPage::getDocument()
{
	return m_pDoc;
}

/**
 * Returns a pointer to the editor view object embedded in this page.
 * @returns	the view pointer
 */
KTextEditor::View* EditorPage::getView()
{
	return m_pView;
}

/** 
 * Returns the full path of the file being edited.
 * @return	The path of the file associated with the Document object, empty 
 *			string if no file is currently open
 */
QString EditorPage::getFilePath()
{
	return m_pDoc->url().path();
}

/** 
 * Returns the name of the file being edited.
 * @return	The name of the file associated with the Document object, empty 
 *			string if no file is currently open
 */
QString EditorPage::getFileName()
{
	return m_sName;
}

/**
 * Determines whether this file can be modified, according to the file-system
 * permissions, and KScope's global settings.
 * @return	true if this document can be changed, false otherwise
 */
bool EditorPage::isWritable()
{
	// Check global settings first
	if (Config().getReadOnlyMode())
		return false;
	
	// Return FS write permissions
	return m_bWritable;
}

/**
 * Determines if the file edited in this page was modified, and the changes
 * were not yet saved.
 * @return	true if the file was modified, false otherwise
 */
bool EditorPage::isModified()
{
	return m_pDoc->isModified();
}

/**
 * Opens a file for editing.
 * @param	sFileName	The full path name of the file to edit.
 */
void EditorPage::open(const QString& sFileName)
{
	// Open the given file
	m_bOpen = false;
	m_pDoc->openURL(sFileName);
}

/**
 * Marks the page as containing a new unnamed file.
 */
void EditorPage::setNewFile()
{
	m_bNewFile = true;
	emit newFile(this);
}

/**
 * Saves the edited file.
 */
void EditorPage::save()
{
	if (m_pDoc->isModified())
		m_pDoc->save();
}

/**
 * Closes an edited file.
 * @param	bForce	true to close the file regardless of any modifications, 
 *					false to prompt the user in case of unsaved chnages 
 * @return	true if the file has been closed, false if the user has aborted
 */
bool EditorPage::close(bool bForce)
{
	QString sPath;
	
	// To override the prompt-on-close behaviour, we need to mark the file
	// as unmodified
	if (bForce)
		m_pDoc->setModified(false);
	
	// Close the file, unless the user aborts the action
	sPath = m_pDoc->url().path();
	if (!m_pDoc->closeURL())
		return false;
		
	emit fileClosed(sPath);
	return true;
}

/**
 * Applies any changes to the user preferences concerning an editor window.
 */
void EditorPage::applyPrefs()
{
	// Determine whether the editor should work in a read-only mode
	if (m_bWritable)
		m_pDoc->setReadWrite(!Config().getReadOnlyMode());
	
	// Apply preferences to the tag list of this window
	m_pCtagsList->applyPrefs();
}

/**
 * Sets the keyboard focus to the editor part of the page.
 * This method is called whenever the page is activated. It is more reasonable
 * to set the focus to the editor than to the tag list.
 */
void EditorPage::setEditorFocus()
{
	m_pView->setFocus();
	slotCursorPosChange();
}

/**
 * Sets the keyboard focus to the tag list.
 * This method is called when the "Go To Tag" menu command is invoked.
 */
void EditorPage::setTagListFocus()
{
	m_pCtagsList->slotSetFocus();
}

/**
 * Sets a bookmark at the given line.
 * @param	nLine	The line to mark
 */
void EditorPage::addBookmark(uint nLine)
{
	KTextEditor::MarkInterface* pMarkIf;
	
	pMarkIf = dynamic_cast<KTextEditor::MarkInterface*>(m_pDoc);
	if (pMarkIf)
		pMarkIf->setMark(nLine, KTextEditor::MarkInterface::markType01);
}

/**
 * Retrieves a list of all bookmarks in this page.
 */
void EditorPage::getBookmarks(FileLocationList& fll)
{
	KTextEditor::MarkInterface* pMarkIf;
	QPtrList<KTextEditor::Mark> plMarks;
	KTextEditor::Mark* pMark;
	
	// Get the marks interface
	pMarkIf = dynamic_cast<KTextEditor::MarkInterface*>(m_pDoc);
	if (!pMarkIf)
		return;
	
	// Find all bookmarks
	plMarks = pMarkIf->marks();
	for (pMark = plMarks.first(); pMark; pMark = plMarks.next()) {
		if (pMark->type == KTextEditor::MarkInterface::markType01)
			fll.append(new FileLocation(getFilePath(), pMark->line, 0));
	}
}

/**
 * Returns the currently selected text in an open file.
 * @return	The selected text, or a null string if no text is currently
 * 			selected
 */
QString EditorPage::getSelection()
{
	KTextEditor::SelectionInterface* pSelect;

	// Get the selected text
	pSelect = dynamic_cast<KTextEditor::SelectionInterface*>(m_pDoc);
	if (!pSelect || !pSelect->hasSelection())
		return QString::null;

	// Return the selected text
	return pSelect->selection();
}
/**
 * Returns a the complete word defined by the current cursor position.
 * Attempts to extract a valid C symbol from the location of the cursor, by
 * starting at the current line and column, and looking forward and backward
 * for non-symbol characters.
 * @return	A C symbol under the cursor, if any, or QString::null otherwise
 */
QString EditorPage::getWordUnderCursor(uint* pPosInWord)
{
	KTextEditor::ViewCursorInterface* pCursor;
	KTextEditor::EditInterface* pEditIf;
	QString sLine;
	uint nLine, nCol, nFrom, nTo, nLast, nLength;
	QChar ch;

	// Get a cursor object
	pCursor = dynamic_cast<KTextEditor::ViewCursorInterface*>(m_pView);
	if (pCursor == NULL)
		return QString::null;

	// Get a pointer to the edit interface	
	pEditIf = dynamic_cast<KTextEditor::EditInterface*>(m_pDoc);
	if (!pEditIf)
		return QString::null;
	
	// Get the line on which the cursor is positioned
	pCursor->cursorPositionReal(&nLine, &nCol);
	sLine = pEditIf->textLine(nLine);
	
	// Find the beginning of the current word
	for (nFrom = nCol; nFrom > 0;) {
		ch = sLine.at(nFrom - 1);
		if (!ch.isLetter() && !ch.isDigit() && ch != '_')
			break;
		
		nFrom--;
	}
	
	// Find the end of the current word
	nLast = sLine.length();
	for (nTo = nCol; nTo < nLast;) {
		ch = sLine.at(nTo);
		if (!ch.isLetter() && !ch.isDigit() && ch != '_')
			break;
		
		nTo++;
	}
	
	// Mark empty words
	nLength = nTo - nFrom;
	if (nLength == 0)
		return QString::null;
	
	// Return the in-word position, if required
	if (pPosInWord != NULL)
		*pPosInWord = nCol - nFrom;
	
	// Extract the word under the cursor from the entire line
	return sLine.mid(nFrom, nLength);
}

/**
 * Combines getSelection() and getWordUnderCursor() to return a suggested
 * text for queries.
 * The function first looks if any text is selected. If so, the selected text
 * is returned. Otherwise, the word under the cursor location is returned, if
 * one exists.
 * @return	Either the currently selected text, or the word under the cursor,
 *			or QString::null if both options fail
 */
QString EditorPage::getSuggestedText()
{
	QString sText;
	
	sText = getSelection();
	if (sText == QString::null)
		sText = getWordUnderCursor();
	
	return sText;	
}

/**
 * Returns the contents of the requested line.
 * Truncates the leading and trailing white spaces.
 * @param	nLine	The line number
 * @return	The text of the requested line, if successful, QString::null
 *			otherwise
 */
QString EditorPage::getLineContents(uint nLine)
{
	KTextEditor::EditInterface* pEditIf;
	QString sLine;

	// Cannot accept line 0
	if (nLine == 0)
		return QString::null;
	
	// Get a pointer to the edit interface
	pEditIf = dynamic_cast<KTextEditor::EditInterface*>(m_pDoc);
	if (!pEditIf)
		return QString::null;

	// Get the line on which the cursor is positioned
	sLine = pEditIf->textLine(nLine - 1);
	return sLine.stripWhiteSpace();
}

/**
 * Moves the editing caret to the beginning of a given line.
 * @param	nLine	The line number to move to
 */
void EditorPage::slotGotoLine(uint nLine)
{
	// Ensure there is an open document
	if (!m_bOpen)
		return;
	
	// Set the cursor to the requested line
	if (!setCursorPos(nLine))
		return;

	// Update Ctags view
	m_pCtagsList->gotoLine(nLine);

	// Set the focus to the selected line
	m_pView->setFocus();
}

/**
 * Sets this editor as the current page, when the edited file's name is
 * selected in the "Window" menu.
 */
void EditorPage::slotMenuSelect()
{
	m_pParentTab->setCurrentPage(m_pParentTab->indexOf(this));
}

/**
 * Displays a list of possible completions for the symbol currently under the
 * cursor.
 */
void EditorPage::slotCompleteSymbol()
{
	m_pCompletion->slotComplete();
}

/**
 * Stores the sizes of the child widgets whenever they are changed.
 * This slot is connected to the resized() signal of the CtagsList child
 * widget.
 */
void EditorPage::slotChildResized()
{
	SPLIT_SIZES si;

	// Only store sizes when allowed to
	if (!m_bSaveNewSizes) {
		m_bSaveNewSizes = true;
		return;
	}
		
	// Get the current widths of the child widgets
	si = m_pSplit->sizes();
	if (si.count() == 2)
		Config().setEditorSizes(si);
}

/**
 * Sets the visibility status and sizes of the child widgets.
 * @param	bShowTagList	true to show the tag list, false otherwise
 * @param	si				The new sizes to use
 */
void EditorPage::setLayout(bool bShowTagList, const SPLIT_SIZES& si)
{
	// Make sure sizes are not stored during this process
	m_bSaveNewSizes = false;
	
	// Adjust the layout
	m_pCtagsList->setShown(bShowTagList);
	if (bShowTagList)
		m_pSplit->setSizes(si);
}

/**
 * Returns the current position of the cursor.
 * @param	nLine	Holds the line on which the cursor is currently located
 * @param	nCol	Holds the column on which the cursor is currently located
 * @return	true if successful, false otherwise (cursor interface was not
 *			obtained)
 */
bool EditorPage::getCursorPos(uint& nLine, uint& nCol)
{
	KTextEditor::ViewCursorInterface* pCursorIf;
	
	// Acquire the view cursor
	pCursorIf = dynamic_cast<KTextEditor::ViewCursorInterface*>(m_pView);
	if (pCursorIf == NULL)
		return false;
	
	// Get the cursor position (adjusted to 1-based counting)
	pCursorIf->cursorPosition(&nLine, &nCol);
	nLine++;
	nCol++;
	
	return true;
}

/**
 * Moves the cursor to a given position.
 * @param	nLine	The cursor's new line number
 * @param	nCol	The cursor's new column number
 * @return	true if successful, false otherwise (cursor interface was not
 *			obtained)
 */
bool EditorPage::setCursorPos(uint nLine, uint nCol)
{
	Kate::View* pKateView;
	KTextEditor::ViewCursorInterface* pCursorIf;
	
	// Cannot accept line 0
	if (nLine == 0)
		return false;
	
	// Adjust to 0-based counting
	nLine--;
	nCol--;
		
	// Acquire the view cursor
	pCursorIf = dynamic_cast<KTextEditor::ViewCursorInterface*>(m_pView);
	if (pCursorIf == NULL)
		return false;
	
	// NOTE: The following code is a fix to a bug in Kate, which wrongly
	// calculates the column number in setCursorPosition.
	pKateView = dynamic_cast<Kate::View*>(m_pView);
	if (pKateView != NULL) {
		KTextEditor::EditInterface* pEditIf;
		const char* szLine;
		uint nRealCol;
		uint nTabAdjust;
		
		// Get a pointer to the edit interface
		pEditIf = dynamic_cast<KTextEditor::EditInterface*>(m_pDoc);
		if (!pEditIf)
			return false;
		
		nRealCol = 0;
		
		// Check for out of bound line numbers
		if (nLine < pEditIf->numLines()) {
			// Get the contents of the requested line
			szLine = pEditIf->textLine(nLine).latin1();
			
			// Check for empty line
			if (szLine != NULL) {
				// The number of columns which a tab character adds
				nTabAdjust = pKateView->tabWidth() - 1;
				
				// Calculate the real column, based on the tab width
				for (; nRealCol < nCol && szLine[nRealCol] != 0; nRealCol++) {
					if (szLine[nRealCol] == '\t')
						nCol -= nTabAdjust;
				}
			}
		}
		else {
			// Marker set beyond end of file, move to the last line
			nLine = pEditIf->numLines() - 1;
		}
		// Set the cursor position
		pCursorIf->setCursorPositionReal(nLine, nRealCol);
	}
	else {
		// Non-Kate editors: set the cursor position normally
		pCursorIf->setCursorPosition(nLine, nCol);
	}
	
	return true;
}

void EditorPage::setTabWidth(uint nTabWidth)
{
	Kate::Document* pKateDoc;
	Kate::Command* pKateCmd;
	QString sCmd, sResult;
	
	pKateDoc = dynamic_cast<Kate::Document*>(m_pDoc);
	if ((pKateDoc) &&
		(pKateCmd = pKateDoc->queryCommand("set-tab-width"))) {
		sCmd.sprintf("set-tab-width %u", nTabWidth);
		pKateCmd->exec((Kate::View*)m_pView, sCmd, sResult);
	}
}

/**
 * Called when a document has completed loading.
 * Determines the file's properties and refreshes the tag list of the editor
 * window.
 * This slot is connected to the completed() signal of the document object.
 * The signal is emitted when a new file is opened, or when a modified file is
 * saved.
 */
void EditorPage::slotFileOpened()
{
	QFileInfo fi(m_pDoc->url().path());
	
	// Get file information
	m_sName = fi.fileName();
	m_bWritable = fi.isWritable();
	
	// Set read/write or read-only mode
	m_pDoc->setReadWrite(!Config().getReadOnlyMode() && m_bWritable);
	
	// Refresh the tag list
	m_pCtagsList->clear();
	m_ctags.run(m_pDoc->url().path());

	// Check if this is a modified file that has just been saved
	if (m_bModified)
		emit fileSaved(m_pDoc->url().path(), m_bNewFile);
	
	// Notify that the document has loaded
	m_bOpen = true;
	m_bModified = false;
	emit fileOpened(this, m_pDoc->url().path());

	// Set initial position of the cursor
	m_nLine = 0;
	slotCursorPosChange();
	
	// This is no longer a new file
	m_bNewFile = false;
}

/**
 * Marks a file as modified when the contents of the editor change.
 * This slot is conncted to the textChanged() signal of the Document object.
 * In addition to marking the file, this method also emits the modified()
 * signal.
 */
void EditorPage::slotSetModified()
{
	// Only perform tasks if the file is not already marked
	if (!m_bModified && m_pDoc->isModified()) {
		m_bModified = true;
		emit modified(this, m_bModified);
	
#if KDE_IS_VERSION(3,3,0)
		Kate::DocumentExt* pKateDoc;
	
		// If the editor is a Kate part, check whether it was modified on
		// disk as well, and issue a warning if so
		pKateDoc = dynamic_cast<Kate::DocumentExt*>(m_pDoc);
		if (pKateDoc)
			pKateDoc->slotModifiedOnDisk(dynamic_cast<Kate::View*>(m_pView));
#endif
	}
	
	// Start/restart the auto-completion timer
	m_pCompletion->slotAutoComplete();
}

/**
 * Marks a file as not modified if all undo levels have been applied.
 * This slot is conncted to the undoChanged() signal of the Document object.
 * In addition to marking the file, this method also emits the modified()
 * signal.
 */
void EditorPage::slotUndoChanged()
{
	// Check if the file contents have been fully restored
	if (m_bModified && !m_pDoc->isModified()) {
		m_bModified = false;
		emit modified(this, m_bModified);
	}
}

/**
 * Handles changes in the cursor position.
 * Emits a signal with the new line and column numbers.
 */
void EditorPage::slotCursorPosChange()
{
	uint nLine, nCol;
	
	// Find the new line and column number, and emit the signal
	if (!getCursorPos(nLine, nCol))
		return;
		
	emit cursorPosChanged(nLine, nCol);
	
	// Select the relevant symbol in the tag list
	if (Config().getAutoTagHl() && (m_nLine != nLine)) {
		m_pCtagsList->gotoLine(nLine);
		m_nLine = nLine;
	}
	
	// Abort code completion on cursor changes during the completion
	// process
	m_pCompletion->abort();
}

#include "editorpage.moc"
