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
#include <klocale.h>
#include <kmessagebox.h>
#include <kurldrag.h>
#include <kate/document.h>
#include "editortabs.h"
#include "kscopepixmaps.h"
#include "queryview.h"

/**
 * Class constructor.
 * @param	pParent		The parent widget
 * @param	szName		The widget's name
 */
EditorTabs::EditorTabs(QWidget* pParent, const char* szName) :
	TabWidget(pParent, szName),
	m_pCurPage(NULL),
	m_pWindowMenu(NULL),
	m_nWindowMenuItems(0),
	m_nNewFiles(0)
{
	// Display close buttons
	setHoverCloseButton(true);

	// Accept file drops
	setAcceptDrops(true);
	
	// Close an editor page when its close button is clicked
	connect(this, SIGNAL(closeRequest(QWidget*)), this,
		SLOT(slotRemovePage(QWidget*)));
	
	// Set an editor page as the active part, when its tab is selected	
	connect(this, SIGNAL(currentChanged(QWidget*)), this,
		SLOT(slotCurrentChanged(QWidget*)));

	// Start dragging a file from a tab
	connect(this, SIGNAL(initiateDrag(QWidget*)), this,
		SLOT(slotInitiateDrag(QWidget*)));
}

/**
 * Class destructor.
 */
EditorTabs::~EditorTabs()
{
}

/**
 * @param	pWindowMenu	Pointer to the main window's "Window" menu (used to
 * 						add an activation menu item for each editor page)
 */
void EditorTabs::setWindowMenu(QPopupMenu* pWindowMenu)
{
	m_pWindowMenu = pWindowMenu;
	connect(pWindowMenu, SIGNAL(aboutToShow()), this,
		SLOT(slotFillWindowMenu()));
	connect(pWindowMenu, SIGNAL(activated(int)), this,
		SLOT(slotSetCurrentPage(int)));
}

/**
 * Adds a new editor page to the tab widget.
 * @param	pNewPage	The page to add
 */
void EditorTabs::addEditorPage(EditorPage* pNewPage)
{
	// Create a new tab and set is as the current one
	insertTab(pNewPage, "");
	showPage(pNewPage);

	// Add the file edited by this page to the map, and display its name,
	// once the file is opened
	connect(pNewPage, SIGNAL(fileOpened(EditorPage*, const QString&)), this,
		SLOT(slotAttachFile(EditorPage*, const QString&)));

	// Handle new unnamed files
	connect(pNewPage, SIGNAL(newFile(EditorPage*)), this,
		SLOT(slotNewFile(EditorPage*)));
	
	// Change tab icon when a file is modified
	connect(pNewPage, SIGNAL(modified(EditorPage*, bool)), this,
		SLOT(slotFileModified(EditorPage*, bool)));
	
	// If this is the first page, the current page will not be set by the 
	// signal handler, so we need to do it manually
	if (count() == 1)
		slotCurrentChanged(pNewPage);
}

/**
 * Finds and displays a page editing the given file.
 * NOTE: The bForceChange parameters is used as a fix for the GUI merge
 * problem arising when the found page is the current one.
 * @param	sFileName		The name of the file to search
 * @param	bForceChange	If set to true, the method will emit the signal
 *							editorChanged() even if the found page is the
 *							current one
 * @return	The editor page object, if found, NULL otherwise
 */
EditorPage* EditorTabs::findEditorPage(const QString& sFileName,
	bool bForceChange)
{
	EditorMap::iterator itr;
	EditorPage* pPage;
	bool bEmit;
	
	// Find the page according to the associated file name
	itr = m_mapEdit.find(sFileName);
	if (itr == m_mapEdit.end())
		return NULL;

	// Set the page as the current one
	pPage = *itr;
	bEmit = (bForceChange && (pPage == m_pCurPage));
	showPage(pPage);

	// Emit the editorChanged() signal, if required
	if (bEmit)
		emit editorChanged(NULL, m_pCurPage);
		
	return *itr;
}

/**
 * Returns the page associated with the selected tab.
 * @return	The current editor page
 */
EditorPage* EditorTabs::getCurrentPage()
{
	return (EditorPage*)currentPage();
}

/**
 * Deletes the currently active page.
 * Finds the current page, closes its editor window and deletes the page.
 * If other editors are open, another page becomes active.
 */
void EditorTabs::removeCurrentPage()
{
	QWidget* pPage;

	// Get the active page, if any
	pPage = currentPage();
	if (pPage == NULL)
		return;

	// Close the editor window
	removePage(pPage, false);
}

/**
 * Removes all editor pages.
 * @return	true if successful, false if the user aborts the operation
 */
bool EditorTabs::removeAllPages()
{
	QWidget* pPage;
	
	// Check if there are any modified files
	if (getModifiedFilesCount()) {
		// Prompt the user to save these files
		switch (KMessageBox::questionYesNoCancel(NULL,
			i18n("Some files contain unsaved changes.\nWould you like to "
			"save these files?"))) {
			case KMessageBox::Yes:
				// Save files
				slotSaveAll();
				break;
				
			case KMessageBox::No:
				// Close files, ignoring changes
				break;
				
			case KMessageBox::Cancel:
				// Abort
				return false;
		}
	}
	
	// Avoid warning about modification on disk
	Kate::Document::setFileChangedDialogsActivated(false);
	
	// Iterate pages until none is left
	while ((pPage = currentPage()) != NULL)
		removePage(pPage, true);
	
	// Restore kate warning if enabled
	Kate::Document::setFileChangedDialogsActivated(
		Config().getWarnModifiedOnDisk());
	
	// All pages were successfully removed
	return true;
}

/**
 * Keeps track of the currently active editor page, and notifies on a change
 * in the active page.
 * This slot is connected to the currentChanged() signal of the QTabWidget 
 * object.
 * @param	pWidget	The new active page
 */
void EditorTabs::slotCurrentChanged(QWidget* pWidget)
{
	EditorPage* pOldPage;

	// TODO:
	// For some reason, this slot is being called twice for every external
	// tab activation (e.g., through the Window menu).
	// We avoid it, but this really needs to be fixed properly.
	if (pWidget == m_pCurPage)
		return;

	// Set the new active page
	pOldPage = m_pCurPage;
	m_pCurPage = (EditorPage*)pWidget;

	if (m_pCurPage) {
		// Set the keyboard focus to the editor part of the page
		m_pCurPage->setEditorFocus();
		
		// Adjust the splitter sizes
		m_pCurPage->setLayout(Config().getShowTagList(),
			Config().getEditorSizes());
	}
	
	/* Notify the main window */
	emit editorChanged(pOldPage, m_pCurPage);
}

/**
 * Updates the tab of an editor page to reflect the newly opened file.
 * This slot is attached to the fileOpened() signal of an EditorPage object.
 * @param	pEditPage	Pointer to the calling object
 * @param	sFilePath	The full path of the file edited in this page
 */
void EditorTabs::slotAttachFile(EditorPage* pEditPage,
	const QString& sFilePath)
{
	// Set the appropriate tab icon, according to the file permissions
	if (pEditPage->isWritable())
		setTabIconSet(pEditPage, Pixmaps().getPixmap(KScopePixmaps::TabRW));
	else
		setTabIconSet(pEditPage, Pixmaps().getPixmap(KScopePixmaps::TabRO));
	
	// Do nothing if the file name has not changed
	if (m_mapEdit[sFilePath] == pEditPage)
		return;

	// Set the tab caption to the file name, and a tool-tip to the full path
	changeTab(pEditPage, pEditPage->getFileName());
	setTabToolTip(pEditPage, sFilePath);

	// Associate the EditorPage object with its file name
	m_mapEdit[sFilePath] = pEditPage;
}

/**
 * Marks a page as containing a new unnamed file.
 * This slot is attached to the newFile() signal of an EditorPage object.
 * @param	pEditPage	Pointer to the calling object
 */
void EditorTabs::slotNewFile(EditorPage* pEditPage)
{
	QString sCaption;
	
	// Set the tab caption to mark a new file
	m_nNewFiles++;
	sCaption = i18n("Untitled ") + QString::number(m_nNewFiles);
	changeTab(pEditPage, 
		Pixmaps().getPixmap(KScopePixmaps::TabRW),
		sCaption);
	setTabToolTip(pEditPage, i18n("New unsaved file"));
}

/**
 * Applies the user's colour and font preferences to all pages.
 */
void EditorTabs::applyPrefs()
{
	EditorPage* pPage;
	int i;

	// Iterate editor pages
	for (i = 0; i < count(); i++) {
		pPage = (EditorPage*)page(i);
		pPage->applyPrefs();
		setTabIconSet(pPage, Pixmaps().getPixmap(pPage->isWritable() ? 
			KScopePixmaps::TabRW : KScopePixmaps::TabRO));
	}
}

/**
 * Fills a list with the paths and cursor positions of all files currently
 * open.
 * @param	list	The list to fill
 */
void EditorTabs::getOpenFiles(FileLocationList& list)
{
	int i;
	EditorPage* pPage;
	uint nLine, nCol;
	
	// Iterate over all editor pages
	for (i = 0; i < count(); i++) {
		// Obtain file and cursor position information
		pPage = (EditorPage*)page(i);
		if (!pPage->getCursorPos(nLine, nCol)) {
			nLine = 1;
			nCol = 1;
		}
		
		// Create a new list item
		list.append(new FileLocation(pPage->getFilePath(), nLine, nCol));
	}
}

/**
 * Constructs a list bookmarks set to open files.
 * Used to store all currently set bookmarks when a session is closed.
 * @param	fll	The list to fill
 */
void EditorTabs::getBookmarks(FileLocationList& fll)
{
	int i;
	EditorPage* pPage;
	
	// Iterate over all editor pages
	for (i = 0; i < count(); i++) {
		pPage = (EditorPage*)page(i);
		pPage->getBookmarks(fll);
	}
}

/**
 * Assigns bookmarks to open files.
 * Called when a session is opened, to restore any bookmarks set to existing
 * editor pages.
 * @param	fll	A list of bookmark positions
 */
void EditorTabs::setBookmarks(FileLocationList& fll)
{
	FileLocation* pLoc;
	EditorMap::iterator itr;
	EditorPage* pPage;
	
	// Iterate over the list of bookmarks
	for (pLoc = fll.first(); pLoc; pLoc = fll.next()) {
		itr = m_mapEdit.find(pLoc->m_sPath);
		// Get the relevant page, if any
		if (itr != m_mapEdit.end()) {
			pPage = *itr;
			pPage->addBookmark(pLoc->m_nLine);
		}
	}
}

/**
 * Fills a QueryView object with the list of currently active bookmarks.
 * @param	pView	The widget to use for displaying bookmarks
 */
void EditorTabs::showBookmarks(QueryView* pView)
{
	int i;
	EditorPage* pPage;
	FileLocationList fll;
	FileLocation* pLoc;
	
	fll.setAutoDelete(true);
	
	// Iterate over all editor pages
	for (i = 0; i < count(); i++) {
		// Obtain file and cursor position information
		pPage = (EditorPage*)page(i);
		pPage->getBookmarks(fll);
		
		// Populate the view
		for (pLoc = fll.first(); pLoc; pLoc = fll.next()) {
			pView->addRecord("", pLoc->m_sPath,
				QString::number(pLoc->m_nLine + 1),
				pPage->getLineContents(pLoc->m_nLine + 1));
		}
		
		fll.clear();
	}
}

/**
 * Removes an editor page.
 * If there are unsaved changes, the user is prompted, and the file is closed 
 * according to the user's choice.
 * This slot is connected to the clicked() signal of the tab's close button.
 * @param	pPage	The EditorPage object to remove
 */
void EditorTabs::slotRemovePage(QWidget* pPage)
{
	removePage(pPage, false);
}

/**
 * Handles the "View->Show/Hide Tag List" menu item.
 * Shows/hides the tag list for the current page, and sets the default values
 * for all pages.
 */
void EditorTabs::slotToggleTagList()
{
	EditorPage* pPage;
	
	// Change the default value
	Config().setShowTagList(!Config().getShowTagList());
	
	// Apply for the current page, if any
	if ((pPage = (EditorPage*)currentPage()) != NULL) {
		pPage->setLayout(Config().getShowTagList(),
			Config().getEditorSizes());
	}
}

/**
 * Handles drag events over an empty tab widget, or over the tab bar.
 * The event is accepted if the dragged object is a list of file paths.
 * @param	pEvent	The drag move event object
 */
void EditorTabs::dragMoveEvent(QDragMoveEvent* pEvent)
{
	KURL::List list;
	bool bAccept;
	
	bAccept = KURLDrag::decode(pEvent, list);
	pEvent->accept(bAccept);
}

/**
 * Handles file drops over an empty tab widget, or over the tab bar.
 * @param	pEvent	The drop event object
 */
void EditorTabs::dropEvent(QDropEvent* pEvent)
{
	emit filesDropped(pEvent);
}

/**
 * Called when an editor tab is dragged from the tab widget.
 * Initialises the drag operation with a URL that corresponds to the path of
 * the file being edited in the corresponding page.
 * This slot is connected to the initiateDrag() signal emitted by the tab
 * widget.
 * @param	pWidget	The page whose tab is being dragged
 */
void EditorTabs::slotInitiateDrag(QWidget* pWidget)
{
	KURL url;
	KURLDrag* pDrag;

	// Create a URL list containing the appropriate file path
	url.setPath(((EditorPage*)pWidget)->getFilePath());
	pDrag = new KURLDrag(KURL::List(url), this);
	
	// Start the drag
	pDrag->dragCopy();
}

/**
 * Changes the tab icon of a modified file.
 * @param	pEditPage	The editor page whose file was modified
 * @param	bModified	true if the file has changed its status to modified,
 *						false otherwise (i.e., when undo operations restore it
 *						to its original contents.)
 */
void EditorTabs::slotFileModified(EditorPage* pEditPage, bool bModified)
{
	if (bModified)
		setTabIconSet(pEditPage, Pixmaps().getPixmap(KScopePixmaps::TabSave));
	else
		setTabIconSet(pEditPage, Pixmaps().getPixmap(KScopePixmaps::TabRW));
}

/**
 * Counts the number of pages containing modified files.
 * @return	The number of modified files
 */
int EditorTabs::getModifiedFilesCount()
{
	int i, nResult;
	
	// Iterate through pages
	for (i = 0, nResult = 0; i < count(); i++) {
		if (((EditorPage*)page(i))->isModified())
			nResult++;
	}
	
	return nResult;
}

/**
 * Saves all files open for editing.
 */
void EditorTabs::slotSaveAll()
{
	int i;
	
	// Iterate through pages
	for (i = 0; i < count(); i++)
		((EditorPage*)page(i))->save();
}

/**
 * Selects the page to the left of the current one.
 */
void EditorTabs::slotGoLeft()
{
	int nIndex;
	
	nIndex = currentPageIndex();
	if (nIndex > 0) {
		nIndex--;
		setCurrentPage(nIndex);
	}
}

/**
 * Selects the page to the right of the current one.
 */
void EditorTabs::slotGoRight()
{
	int nIndex;
	
	nIndex = currentPageIndex();
	if (nIndex < count() - 1) {
		nIndex++;
		setCurrentPage(nIndex);
	}
}

/**
 * Fills the main window's "Window" menu with the current list of file tabs.
 * This slot is attached to the aboutToShow() signal, emitted by the Window
 * popup menu.
 */
void EditorTabs::slotFillWindowMenu()
{
	QString sLabel;
	int i;
	
	// Delete old menu items
	// NOTE: We can't use aboutToHide() to do that, since it is emitted 
	// _before_ the activated() signal
	for (i = 0; i < m_nWindowMenuItems; i++)
		m_pWindowMenu->removeItem(i);
	
	// Add new items
	for (i = 0; i < count(); i++) {
		sLabel = (i < 10) ? QString("&%1 %2").arg(i).arg(label(i)) : label(i);
		m_pWindowMenu->insertItem(sLabel, i);
	}
	
	// Store the number of items added
	m_nWindowMenuItems = i;
}

/**
 * Sets the current page to the given one.
 * This slot is attached to the activated() signal, emitted by the "Window"
 * popup menu. The tab number to switch to is given by the menu item ID.
 * Note that we do not trust setCurrentPage() to filter out the IDs of other
 * menu items (which are supposed to be negative numbers).
 */
void EditorTabs::slotSetCurrentPage(int nId)
{
	if (nId >= 0 && nId < count())
		setCurrentPage(nId);
}

/**
 * Closes an edited file, and removes its page.
 * Once a file has been closed, its page is removed from the tab widget stack,
 * its menu item in the "Windows" menu is deleted and all other references to
 * it are removed.
 * Note that the operation may fail if the user chooses not to close the file
 * when prompted for unsaved changes.
 * @param	pPage	The EditorPage object to remove
 * @param	bForce	true to close the page even if there are unsaved changes,
 *					false otherwise
 * @return	true if the page was removed, false otherwise
 */
bool EditorTabs::removePage(QWidget* pPage, bool bForce)
{
	EditorPage* pEditPage;
	QString sFilePath;
	
	// Store the file path for later
	pEditPage = (EditorPage*)pPage;
	sFilePath = pEditPage->getFilePath();
	
	// Close the edited file (may fail if the user aborts the action)
	if (!pEditPage->close(bForce))
		return false;
		
	// Remove the page and all references to it
	m_mapEdit.remove(sFilePath);
	TabWidget::removePage(pPage);

	// Update the new state if no other page exists (if another page has
	// become active, it will update the new state, so there is no need for
	// special handling)
	if (currentPage() == NULL)
		slotCurrentChanged(NULL);
	
	// Notify the page has been removed
	emit editorRemoved(pEditPage);
	return true;
}

#include "editortabs.moc"
