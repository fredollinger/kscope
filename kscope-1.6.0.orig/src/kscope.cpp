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
#include <kfiledialog.h>
#include <kmenubar.h>
#include <klocale.h>
#include <kmessagebox.h>
#include <klineedit.h>
#include <kinputdialog.h>
#include <kxmlguifactory.h>
#include <kstatusbar.h>
#include <kurldrag.h>
#include <kkeydialog.h>
#include "kscope.h"
#include "kscopeconfig.h"
#include "projectmanager.h"
#include "editortabs.h"
#include "fileview.h"
#include "filelist.h"
#include "querywidget.h"
#include "editormanager.h"
#include "cscopefrontend.h"
#include "ctagslist.h"
#include "newprojectdlg.h"
#include "openprojectdlg.h"
#include "preferencesdlg.h"
#include "dirscanner.h"
#include "querypage.h"
#include "calltreedlg.h"
#include "calltreemanager.h"
#include "kscopepixmaps.h"
#include "progressdlg.h"
#include "projectfilesdlg.h"
#include "cscopemsgdlg.h"
#include "symboldlg.h"
#include "symbolcompletion.h"
#include "queryviewdlg.h"
#include "graphwidget.h"
#include "makedlg.h"
#include "welcomedlg.h"
#include "bookmarksdlg.h"
#include "kscopeactions.h"

/**
 * Class constructor.
 * @param	pParent	The parent widget
 * @param	szName	The widget's name
 */
KScope::KScope(QWidget* pParent, const char* szName) :
	KParts::DockMainWindow(pParent, szName),
	m_pCscopeBuild(NULL),	
	m_sCurFilePath(""),
	m_nCurLine(0),
	m_pProgressDlg(NULL),
	m_bUpdateGUI(true),
	m_bCscopeVerified(false),
	m_bRebuildDB(false),
	m_pMakeDlg(NULL)
{
	QString sPath;

	// Load configuration
	Config().load();
	
	// Create the main child widgets
	m_pEditTabs = new EditorTabs(this, NULL);
	m_pQueryWidget = new QueryWidget(this);
	m_pFileView = new FileView(this);
	m_pFileList = m_pFileView->getFileList();
	m_pMsgDlg = new CscopeMsgDlg(this);
	m_pQueryDock = createDockWidget("Query Window", QPixmap());
	m_pFileViewDock = createDockWidget("File List Window", QPixmap());

	// Connect menu and toolbar items with the object's slots
	m_pActions = new KScopeActions(this);
	m_pActions->init();
	m_pActions->slotEnableProjectActions(false);
	
	// Show a toolbar show/hide menu
	setStandardToolBarMenuEnabled(true);
	
	// Create the initial GUI (no active part)
	setXMLFile("kscopeui.rc");
	createShellGUI();
	
	// Create all child widgets
	initMainWindow();

	// Create control objects
	m_pProjMgr = new ProjectManager();
	m_pEditMgr = new EditorManager(this);
	m_pCallTreeMgr = new CallTreeManager(this);

	// Initialise the icon manager	
	Pixmaps().init();
	
	// Open a file for editing when selected in the project's file list or the
	// file tree
	connect(m_pFileView, SIGNAL(fileRequested(const QString&, uint)), this,
		SLOT(slotShowEditor(const QString&, uint)));

	// Delete an editor page object after it is removed
	connect(m_pEditTabs, SIGNAL(editorRemoved(EditorPage*)),
		this, SLOT(slotDeleteEditor(EditorPage*)));
	
	connect(m_pEditTabs, SIGNAL(filesDropped(QDropEvent*)), this,
		SLOT(slotDropEvent(QDropEvent*)));
	
	// Set an editor as the active part whenever its owner tab is selected
	connect(m_pEditTabs, SIGNAL(editorChanged(EditorPage*, EditorPage*)),
		this, SLOT(slotChangeEditor(EditorPage*, EditorPage*)));

	// Display a file at a specific line when selected in a query list
	connect(m_pQueryWidget, SIGNAL(lineRequested(const QString&, uint)),
		this, SLOT(slotQueryShowEditor(const QString&, uint)));
	
	// Display the symbol dialogue when the user opens a new query page
	connect(m_pQueryWidget, SIGNAL(newQuery()), 
		this, SLOT(slotQueryReference()));

	// Rebuild the project database after a certain time period has elapsed
	// since the last save
	connect(&m_timerRebuild, SIGNAL(timeout()), this, SLOT(slotRebuildDB()));

	// Display a file at a specific line when selected in a call tree dialogue
	connect(m_pCallTreeMgr, SIGNAL(lineRequested(const QString&, uint)),
		this, SLOT(slotQueryShowEditor(const QString&, uint)));
		
	// Store main window settings when closed
	setAutoSaveSettings();
	
	// Initialise arrow head drawing
	GraphWidget::setArrowInfo(20, 15);
	
	// Use a maximised window the first time
	if (Config().isFirstTime())
		showMaximized();

	// Show the Welcome message
	if (Config().showWelcomeDlg()) {
		show();
		slotShowWelcome();
	}

	// If this is the first time the user has launched KScope, prompt him/her
	// to configure the global parameters
	if (Config().isFirstTime())
		slotConfigure();		
}

/**
 * Class destructor.
 */
KScope::~KScope()
{
	// Save configuration
	Config().store();
	Config().storeWorkspace(this);
	
	delete m_pCallTreeMgr;
	delete m_pEditMgr;
	delete m_pCscopeBuild;
	delete m_pProjMgr;
	
	if (m_pMakeDlg != NULL)
		delete m_pMakeDlg;
}

/**
 * Positions child widgets into their docking stations, and performs some
 * other main window initialisation.
 */
void KScope::initMainWindow()
{
	KStatusBar* pStatus;
	KDockWidget* pMainDock;
	QPopupMenu* pPopup;
	
	// Create the status bar
	pStatus = statusBar();
	pStatus->insertItem(i18n(" Line: N/A Col: N/A "), 0, 0, true);

	// Create the main dock for the editor tabs widget
	pMainDock = createDockWidget("Editors Window", QPixmap());
	pMainDock->setWidget(m_pEditTabs);
	pMainDock->setDockSite(KDockWidget::DockCorner);
	setMainDockWidget(pMainDock);
	setView(pMainDock);
	pMainDock->setEnableDocking(KDockWidget::DockNone);

	// Create the query window dock
	m_pQueryDock->setWidget(m_pQueryWidget);
	m_pQueryDock->manualDock(pMainDock, KDockWidget::DockBottom, 65);

	// Update the relevant shell action when the dock is hidden through its
	// close button
	connect(m_pQueryDock, SIGNAL(headerCloseButtonClicked()), m_pActions,
		SLOT(slotQueryDockClosed()));
		
	// Create the file view dock
	m_pFileViewDock->setWidget(m_pFileView);
	m_pFileViewDock->manualDock(pMainDock, KDockWidget::DockRight, 80);
	
	// Update the relevant shell action when the dock is hidden through its
	// close button
	connect(m_pFileViewDock, SIGNAL(headerCloseButtonClicked()), m_pActions,
		SLOT(slotFileViewDockClosed()));
	
	// Associate the "Window" menu with the editor tabs widdget
	pPopup = (QPopupMenu*)factory()->container("window", this);
	m_pEditTabs->setWindowMenu(pPopup);

	// Associate the "Query" popup menu with the query widget
	pPopup = (QPopupMenu*)factory()->container("query_popup", this);
	m_pQueryWidget->setPageMenu(pPopup, m_pActions->getLockAction());
	
	// Restore dock configuration
	Config().loadWorkspace(this);
	m_bHideQueryOnSelection = m_pQueryDock->isHidden();
	m_pActions->initLayoutActions();
}

/**
 * Handles the "File->Quit" command. Closes the main window, which terminates
 * the application.
 */
void KScope::slotClose()
{
	// Destroy the main window
	KParts::DockMainWindow::close();
}

/**
 * Called when a request has been issued to close the main window.
 * Tries to close the active project.
 * @return	true if the main window can be closed, false otherwise
 */
bool KScope::queryClose()
{
	bool bResult;

	m_bUpdateGUI = false;
	bResult = slotCloseProject();
	m_bUpdateGUI = true;

	return true;
}

/**
 * Handles the "Project->New..." command.
 * Prompts the user for the name and folder for the project, and then creates
 * the project.
 */
void KScope::slotCreateProject()
{
	NewProjectDlg dlg(true, this);
	ProjectBase::Options opt;

	// Prompt the user to close any active projects
	if (m_pProjMgr->curProject()) {
		if (KMessageBox::questionYesNo(0, 
			i18n("The current project needs to be closed before a new one is"
			" created.\nWould you like to close it now?")) != 
			KMessageBox::Yes) {
			return;
		}
		
		// Try to close the project.
		if (!slotCloseProject())
			return;
	}
	
	// Display the "New Project" dialog
	if (dlg.exec() != QDialog::Accepted)
		return;

	// Create the new project
	dlg.getOptions(opt);
	m_pProjMgr->create(dlg.getName(), dlg.getPath(), opt);

	// Open the project
	openProject(dlg.getPath() + "/" + dlg.getName());
}

/**
 * Handles the "Project->Open..." command.
 * Prompts the user for a project file ("cscope.proj"), and opens the
 * selected project.
 */
void KScope::slotOpenProject()
{
	OpenProjectDlg dlg;
	QString sPath;
	
	if (dlg.exec() == QDialog::Rejected)
		return;

	sPath = dlg.getPath();
	
	// Check if the path refers to a permanent or temporary project
	if (QFileInfo(sPath).isDir())
		openProject(sPath);
	else
		openCscopeOut(sPath);
}

/**
 * Handles the "Project->Add/Remove Files..." command.
 * Opens the project's files dialog, which allows the user to add and remove
 * source files.
 */
void KScope::slotProjectFiles()
{
	ProjectBase* pProj;
	
	// A project must be open
	pProj = m_pProjMgr->curProject();
	if (!pProj)
		return;

	// Cannot update the file list of a temporary project
	if (pProj->isTemporary()) {
		KMessageBox::error(0, i18n("The Add/Remove Files dialogue is not "
				"available for temporary projects."));
		return;
	}

	// Display the files dialog
	ProjectFilesDlg dlg((Project*)pProj, this);
	if (dlg.exec() != QDialog::Accepted)
		return;
	
	// Update the project's file list
	if (pProj->storeFileList(&dlg))
		slotProjectFilesChanged();
}

/**
 * Handles the "Project->Properties..." command.
 * Opens the project's properties dialog, which allows the user to change
 * some attributes of the current project.
 * source files.
 */
void KScope::slotProjectProps()
{
	ProjectBase* pProj;
	ProjectBase::Options opt;
	
	// A project must be open
	pProj = m_pProjMgr->curProject();
	if (!pProj)
		return;

	// No properties for a temporary project
	if (pProj->isTemporary()) {
		KMessageBox::error(0, i18n("The Project Properties dialogue is not "
			"available for temporary projects."));
		return;
	}
	
	// Create the properties dialog
	NewProjectDlg dlg(false, this);
	pProj->getOptions(opt);
	dlg.setProperties(pProj->getName(), pProj->getPath(), opt);
		
	// Display the properties dialog
	if (dlg.exec() != QDialog::Accepted)
		return;

	// Set new properties
	dlg.getOptions(opt);
	pProj->setOptions(opt);
	
	// Reset the CscopeFrontend class and the builder object
	initCscope();
	
	// Set auto-completion parameters
	SymbolCompletion::initAutoCompletion(opt.bACEnabled, opt.nACMinChars,
		opt.nACDelay, opt.nACMaxEntries);
	
	// Set the source root
	m_pFileView->setRoot(pProj->getSourceRoot());
}

/**
 * Handles the "Cscope->Open Cscope.out..." menu command.
 * Prompts the user for a Cscope.out file, and, if successful, opens a new
 * session for working with this file.
 */
void KScope::slotProjectCscopeOut()
{
	QString sFilePath;
	
	// Prompt for a Cscope.out file
	sFilePath = KFileDialog::getOpenFileName();
	if (sFilePath.isEmpty())
		return;

	// Open a temporary project
	openCscopeOut(sFilePath);	
}

/**
 * Handles the "Cscope->References..." menu command.
 * Prompts the user for a symbol name, and initiates a query to find all 
 * references to that symbol.
 */
void KScope::slotQueryReference()
{
	slotQuery(SymbolDlg::Reference, true);
}

/**
 * Handles the "Cscope->Definition..." menu command.
 * Prompts the user for a symbol name, and initiates a query to find the 
 * global definition of that symbol.
 */
void KScope::slotQueryDefinition()
{
	slotQuery(SymbolDlg::Definition, true);
}

/**
 * Handles the "Cscope->Called Functions..." menu command.
 * Prompts the user for a function name, and initiates a query to find all 
 * function calls from that function.
 */
void KScope::slotQueryCalled()
{
	slotQuery(SymbolDlg::Called, true);
}

/**
 * Handles the "Cscope->Calling Functions..." menu command.
 * Prompts the user for a function name, and initiates a query to find all 
 * functions calling that function.
 */
void KScope::slotQueryCalling()
{
	slotQuery(SymbolDlg::Calling, true);
}

/**
 * Handles the "Cscope->Find Text..." menu command.
 * Prompts the user for a string, and initiates a query to find all
occurances 
 * of that string.
 */
void KScope::slotQueryText()
{
	slotQuery(SymbolDlg::Text, true);
}

/**
 * Handles the "Cscope->Find EGrep Pattern..." menu command.
 * Prompts the user for a regular expression, and initiates a query to find 
 * all strings matching that pattern.
 */
void KScope::slotQueryPattern()
{
	slotQuery(SymbolDlg::Pattern, true);
}

/**
 * Handles the "Cscope->Find File..." menu command.
 * Prompts the user for a file name, and initiates a query to find all files
 * having that name.
 */
void KScope::slotQueryFile()
{
	slotQuery(SymbolDlg::FileName, true);
}

/**
 * Handles the "Cscope->Find Including Files..." menu command.
 * Prompts the user for a file name, and initiates a query to find all files
 * having an '#include' directive to that file.
 */
void KScope::slotQueryIncluding()
{
	slotQuery(SymbolDlg::Including, true);
}

/**
 * Handles the "Cscope->Quick Definition" menu command.
 * Initiates a query to find the global definition of the symbol currently
 * selected or under the cursor. The user is prompted only if no symbol can
 * be found.
 */
void KScope::slotQueryQuickDef()
{
	QString sSymbol;
	QueryViewDlg* pDlg;
	uint nType;
	
	// Get the requested symbol and query type
	nType = SymbolDlg::Definition;
	if (!getSymbol(nType, sSymbol, false))
		return;
		
	// Create a modeless query view dialogue
	pDlg = new QueryViewDlg(QueryViewDlg::DestroyOnSelect, this);
	
	// Display a line when it is selected in the dialogue
	connect(pDlg, SIGNAL(lineRequested(const QString&, uint)), this,
		SLOT(slotShowEditor(const QString&, uint)));
		
	// Start the query
	pDlg->query(nType, sSymbol);
}

/**
 * Handles the "Cscope->Call Tree..." menu command.
 * Displays a tree of functions calling the requested function.
 */
void KScope::slotCallTree()
{
	slotQuery(SymbolDlg::CallTree, true);
}

/**
 * Handles the "Cscope->Rebuild Database..." command.
 * Rebuilds Cscope's database for the current project.
 */
void KScope::slotRebuildDB()
{
	ProjectBase* pProj;
	
	pProj = m_pProjMgr->curProject();
	if (!pProj)
		return;
	
	if (!pProj->dbExists()) {
		m_pProgressDlg = new ProgressDlg(i18n("KScope"), i18n("Please wait "
			"while KScope builds the database"), this);
		m_pProgressDlg->setAllowCancel(false);
		m_pProgressDlg->setValue(0);
	}
	
	m_pCscopeBuild->rebuild();
}

/**
 * Handles the "Settings->Configure Shortcuts..." command.
 * Displays the prferences dialog, which allows the user 
 * to change the shortcuts for KScope.
 */
void KScope::slotShortcuts()
{
	KKeyDialog::configure(actionCollection(), this);
}

/**
 * Handles the "Settings->Configure KScope..." command.
 * Displays the prferences dialog, which allows the user to set different
 * configuration parameters for KScope.
 */
void KScope::slotConfigure()
{
	PreferencesDlg dlg;

	// Apply the preferences if either the "Apply" or the "OK" buttons are
	// clicked
	connect(&dlg, SIGNAL(applyPref()), this, SLOT(slotApplyPref()));

	// Show the dialog
	if (dlg.exec() == QDialog::Accepted) {
		// Verify Cscope's installation
		verifyCscope();
	}
}

/**
 * Refreshes the file list when files are added to or removed from a project,
 * and rebuilds the Cscope database.
 * This slot is attached to the fileListChanged() signal emitted by 
 * the ProjectManager object.
 */
void KScope::slotProjectFilesChanged()
{
	QStringList slArgs;
	
	// Refresh the file list
	m_pFileList->setUpdatesEnabled(false);
	m_pFileList->clear();
	m_pProjMgr->curProject()->loadFileList(m_pFileList);
	m_pFileList->setUpdatesEnabled(true);
	
	// Rebuild the symbol database
	if (isAutoRebuildEnabled())
		slotRebuildDB();
}

/**
 * Adds a list of files to the current project.
 * This slot is connected to the filesAdded() signal of the ProjectManager
 * object. Once files are added to the project, they are also added to the
 * file list, and the project's database is rebuilt.
 * @param	slFiles	The list of file paths added to the project
 */
void KScope::slotFilesAdded(const QStringList& slFiles)
{
	QStringList::const_iterator itr;

	// Add the file paths to the project's file list
	for (itr = slFiles.begin(); itr != slFiles.end(); ++itr)
		m_pFileList->addItem(*itr);
	
	// Rebuild the database
	if (isAutoRebuildEnabled())
		slotRebuildDB();
}

/**
 * Promts the user for a symbol, an starts a new Cscope query.
 * @param	nType	The numeric query type code
 * @param	bPrompt	true to always prompt for a symbol, false to try to
 * 					obtain the symbol automatically
 */
void KScope::slotQuery(uint nType, bool bPrompt)
{
	QString sSymbol;
	CallTreeDlg* pCallTreeDlg;
	
	// Get the requested symbol and query type
	if (!getSymbol(nType, sSymbol, bPrompt))
		return;
		
	if (nType == SymbolDlg::CallTree) {
		// Create and display a call tree dialogue
		pCallTreeDlg = m_pCallTreeMgr->addDialog();
		pCallTreeDlg->setRoot(sSymbol);
		pCallTreeDlg->show();
	}
	else {
		// Run the requested query
		nType = SymbolDlg::getQueryType(nType);
		m_pQueryWidget->initQuery(nType, sSymbol);
		
		// Ensure Query Window is visible
		toggleQueryWindow(true);	
	}
}

/**
 * Opens a project.
 * If another project is currently active, it is closed first.
 * @param	sDir	The directory of the project to open.
 */
void KScope::openProject(const QString& sDir)
{
	QString sProjDir;
	ProjectBase* pProj;
	QStringList slQueryFiles;
	QStringList slCallTreeFiles;
	QStringList slArgs;
	ProjectBase::Options opt;
	
	// Close the current project (may return false if the user clicks on the
	// "Cancel" button while prompted to save a file)
	if (!slotCloseProject())
		return;

	// Open the project in the project manager
	sProjDir = QDir::cleanDirPath(sDir);
	if (!m_pProjMgr->open(sProjDir))
		return;
	
	// Change main window title
	pProj = m_pProjMgr->curProject();
	setCaption(pProj->getName());

	// Set the root of the file tree
	m_pFileView->setRoot(pProj->getSourceRoot());
	
	// Initialise Cscope and create a builder object
	initCscope();
	
	// Set auto-completion parameters
	pProj->getOptions(opt);
	SymbolCompletion::initAutoCompletion(opt.bACEnabled, opt.nACMinChars,
		opt.nACDelay, opt.nACMaxEntries);
	
	// Create an initial query page
	m_pQueryWidget->addQueryPage();
	
	// Enable project-related actions
	m_pActions->slotEnableProjectActions(true);
	
	// If this is a new project (i.e., no source files are yet included), 
	// display the project files dialogue
	if (pProj->isEmpty()) {
		slotProjectFiles();
		return;
	}
	
	// Fill the file list with all files in the project. 
	m_pFileList->setUpdatesEnabled(false);
	pProj->loadFileList(m_pFileList);
	m_pFileList->setUpdatesEnabled(true);
	
	// Restore the last session
	restoreSession();
	
	// Rebuild the cross-reference database
	if (isAutoRebuildEnabled()) {
		// If Cscope installation was not yet verified, postpone the build
		// process
		if (m_bCscopeVerified)
		slotRebuildDB();
		else
			m_bRebuildDB = true;
	}
}

/**
 * Opens a temporary project for a Cscope.out file.
 * @param	sFilePath	The full path of the Cscope.out file
 * @return	true if successful, false otherwise
 */
bool KScope::openCscopeOut(const QString& sFilePath)
{
	ProjectBase* pProj;
	
	// Close the current project (may return false if the user clicks on the
	// "Cancel" button while prompted to save a file)
	if (!slotCloseProject())
		return false;

	// Open a temporary project for this cscope.out file
	if (!m_pProjMgr->openCscopeOut(sFilePath))
		return false;
	
	// Change main window title
	pProj = m_pProjMgr->curProject();
	setCaption(pProj->getName());
	
	// Set the root folder in the file tree
	m_pFileView->setRoot(pProj->getSourceRoot());
	
	// Initialise Cscope and create a builder object
	initCscope();
	
	// Create an initial query page
	m_pQueryWidget->addQueryPage();
	
	// Enable project-related actions
	m_pActions->slotEnableProjectActions(true);
	
	// Fill the file list with all files in the project. 
	m_pFileList->setUpdatesEnabled(false);
	pProj->loadFileList(m_pFileList);
	m_pFileList->setUpdatesEnabled(true);
	
	return true;
}

/**
 * Opens the most recently used project.
 * This method is called when KScope starts, but only if the relevant 
 * configuration flag (Reload Last Project) is set.
 */
void KScope::openLastProject()
{
	const QStringList slProjects = Config().getRecentProjects();
	QString sPath;
	
	if (slProjects.empty())
		return;
		
	// Get the project's path
	sPath = *slProjects.begin();

	// Check if the path refers to a temporary project
	if (!QFileInfo(sPath).isDir()) {
		openCscopeOut(sPath);
		return;
	}

	openProject(sPath);
}

/**
 * Reopens all files which were open when the project was last closed.
 * In order to reduce the time required by this operation, the GUI of all
 * but the last editor part is not merged with that of the main window.
 */
void KScope::restoreSession()
{
	ProjectBase* pProj;
	Project::Session sess;
	FileLocation* pLoc;
	EditorPage* pPage;
	
	// A session is available for persistent projects only
	pProj = m_pProjMgr->curProject();
	if (!pProj || pProj->isTemporary())
		return;
	
	// Make sure all FileLocation objects are deleted
	sess.fllOpenFiles.setAutoDelete(true);
	sess.fllBookmarks.setAutoDelete(true);
	
	// Load the session
	((Project*)pProj)->loadSession(sess);
	
	// Do not update the GUI when loading the editor parts of the initially
	// hidden windows
	m_bUpdateGUI = false;
	
	for (pLoc = sess.fllOpenFiles.first(); pLoc != NULL;
		pLoc = sess.fllOpenFiles.next()) {
		if (QFile::exists(pLoc->m_sPath)) {
			pPage = addEditor(pLoc->m_sPath);
			pPage->setCursorPos(pLoc->m_nLine, pLoc->m_nCol);
		}
	}
	
	// Merge the GUI of the visible editor part
	m_bUpdateGUI = true;

	// Set the active editor (or choose a default one)
	if (m_pEditTabs->findEditorPage(sess.sLastFile, true) == NULL)
		m_pEditTabs->findEditorPage(sess.fllOpenFiles.last()->m_sPath, true);
	
	// Reload bookmarks
	m_pEditTabs->setBookmarks(sess.fllBookmarks);	
	
	// Load previously stored queries and call trees
	m_pQueryWidget->loadPages(pProj->getPath(), sess.slQueryFiles);
	m_pCallTreeMgr->loadOpenDialogs(pProj->getPath(), sess.slCallTreeFiles);
}

/**
 * Shows or hides the query dock window.
 * This function is only called internally, not as a result of a user's
 * workspace action (e.g., clicking the "Show/Hide Query Window" toolbar
 * button). Therefore it does not reflect the user's preference, which is
 * kept through the m_bHideQueryOnSelection variable.
 * @param	bShow	true to show the window, false to hide it
 */
void KScope::toggleQueryWindow(bool bShow)
{
	// Remember the user's preferences
	if (bShow)
		m_bHideQueryOnSelection = m_pQueryDock->isHidden();
	else
		m_bHideQueryOnSelection = false;
	
	// Change the visibility state of the widget, if required
	if (m_pQueryDock->isShown() != bShow)
		m_pQueryDock->changeHideShowState();
		
	// Synchronise with the menu command's state
	m_pActions->slotQueryDockToggled(bShow);
}

/**
 * Parses the command line, after it was stripped of its KDE options.
 * The command line may contain one of the following options:
 * 1. A project file (named cscope.proj)
 * 2. A Cscope cross-reference database
 * 3. A list of source files
 * @param	pArgs	Command line arguments
 */
void KScope::parseCmdLine(KCmdLineArgs* pArgs)
{
	QString sArg;
	QFileInfo fi;
	int i;

	// Loop over all arguments
	for (i = 0; i < pArgs->count(); i++) {
		// Verify the argument is a file or directory name
		sArg = pArgs->arg(i);
		fi.setFile(sArg);
		if (!fi.exists())
			continue;
			
		// Handle the current argument
		if (fi.isFile()) {
			if (fi.fileName() == "cscope.proj") {
				// Open a project file
				openProject(fi.dirPath(true));
				return;
			} else if (openCscopeOut(sArg)) {
				// Opened the file as a cross-reference database
				return;
			} else {
				// Assume this is a source file
				slotShowEditor(sArg, 0);
			}
		} else if (fi.isDir()) {
			// Treat the given path as a project directory
			openProject(fi.absFilePath());
			return;
		}
	}
}

/**
 * Starts a shell script to ensure that Cscope is properly installed and to
 * extract the supported command-line arguments. 
 */
void KScope::verifyCscope()
{
	CscopeVerifier* pVer;
	
	statusBar()->message(i18n("Verifying Cscope installation..."));
	
	pVer = new CscopeVerifier();
	connect(pVer, SIGNAL(done(bool, uint)), this,
		SLOT(slotCscopeVerified(bool, uint)));
	
	pVer->verify();
}

/**
 * Initialises the CscopeFrontend class with the current project arguments,
 * and creates an object used for rebuilding the symbol database.
 */
void KScope::initCscope()
{
	ProjectBase* pProj;
	
	// Delete the current object, if one exists
	if (m_pCscopeBuild)
		delete m_pCscopeBuild;

	// Initialise CscopeFrontend
	pProj = m_pProjMgr->curProject();
	CscopeFrontend::init(pProj->getPath(), pProj->getArgs());

	// Create a persistent Cscope process
	m_pCscopeBuild = new CscopeFrontend();

	// Show build progress information in the main status bar
	connect(m_pCscopeBuild, SIGNAL(progress(int, int)), this,
		SLOT(slotBuildProgress(int, int)));
	connect(m_pCscopeBuild, SIGNAL(buildInvIndex()), this,
		SLOT(slotBuildInvIndex()));
	connect(m_pCscopeBuild, SIGNAL(finished(uint)), this,
		SLOT(slotBuildFinished(uint)));
	connect(m_pCscopeBuild, SIGNAL(aborted()), this,
		SLOT(slotBuildAborted()));

	// Show errors in a modeless dialogue
	connect(m_pCscopeBuild, SIGNAL(error(const QString&)), this,
		SLOT(slotCscopeError(const QString&)));
}

/**
 * Closes the active project.
 * Closing a project involves closing all of the editor windows (prompting
 * the user for unsaved changes); terminating the Cscope process; and further
 * clean-up of the project's data.
 */
bool KScope::slotCloseProject()
{
	ProjectBase* pProj;
	Project::Session sess;
	
	// Do nothing if no project is open
	pProj = m_pProjMgr->curProject();
	if (!pProj)
		return true;
	
	// Make sure all FileLocation objects are deleted
	sess.fllOpenFiles.setAutoDelete(true);
	sess.fllBookmarks.setAutoDelete(true);
	
	// Close all open editor pages
	if (m_pEditTabs->count() > 0) {
		// Save session information for persistent projects
		if (!pProj->isTemporary()) {
			sess.sLastFile = m_pEditTabs->getCurrentPage()->getFilePath();
			m_pEditTabs->getOpenFiles(sess.fllOpenFiles);
			m_pEditTabs->getBookmarks(sess.fllBookmarks);
		}
		
		if (!m_pEditTabs->removeAllPages())
			return false;
	}
	
	// Disable project-related actions
	m_pActions->slotEnableProjectActions(false);
	
	// Destroy the make dialogue
	if (m_pMakeDlg != NULL) {
		// Save session information for persistent projects
		if (!pProj->isTemporary()) {
			sess.sMakeCmd = m_pMakeDlg->getCommand();
			sess.sMakeRoot = m_pMakeDlg->getDir();
		}
		
		delete m_pMakeDlg;
		m_pMakeDlg = NULL;
	}
	
	// Save session information for persistent projects
	if (!pProj->isTemporary()) {
		m_pQueryWidget->savePages(pProj->getPath(), sess.slQueryFiles);
		m_pCallTreeMgr->saveOpenDialogs(pProj->getPath(), sess.slCallTreeFiles);
	}
		
	// Close all query pages and call trees
	m_pQueryWidget->slotCloseAll();
	m_pCallTreeMgr->closeAll();
	
	// Store session information for persistent projects
	if (!pProj->isTemporary())
		((Project*)pProj)->storeSession(sess);
	
	// Close the project in the project manager, and terminate the Cscope
	// process
	m_pProjMgr->close();
	delete m_pCscopeBuild;
	m_pCscopeBuild = NULL;
	setCaption(QString::null);

	// Clear the contents of the file list
	m_pFileView->clear();

	// Reset queried symbols history
	SymbolDlg::resetHistory();
	
    // Remove any remaining status bar messages
    statusBar()->message("");
    
	return true;
}

/**
 * Handles the "Edit->Edit in External Editor" menu command.
 * Invokes an external editor for the current file and line number.
 */
void KScope::slotExtEdit()
{
	QString sCmdLine;
	KProcess proc;

	// Create the command line for the external editor	
	sCmdLine = Config().getExtEditor();
	sCmdLine.replace("%F", m_sCurFilePath);
	sCmdLine.replace("%L", QString::number(m_nCurLine));
	
	// Run the external editor
	proc.setUseShell(true);
	proc << sCmdLine;
	proc.start(KProcess::DontCare);
}

/**
 * Handles the "Edit->Complete Symbol" menu command.
 * Creates a list of possible completions for the symbol currently under the
 * cursor.
 */
void KScope::slotCompleteSymbol()
{
	EditorPage* pPage;
	
	pPage = m_pEditTabs->getCurrentPage();
	if (pPage != NULL)
		pPage->slotCompleteSymbol();
}

/**
 * Handles the "Help->Show Welcome Message..." menu command.
 * Displays the "Welcome" dialogue.
 */
void KScope::slotShowWelcome()
{
	WelcomeDlg dlg;
	dlg.exec();
}

/**
 * Handles the "Edit->Go To Tag" menu command.
 * Sets the cursor to the edit box of the current tag list.
 */
void KScope::slotGotoTag()
{
	EditorPage* pPage;
	
	pPage = m_pEditTabs->getCurrentPage();
	if (pPage)
		pPage->setTagListFocus();
}

/**
 * Reports the results of the Cscope verification script.
 * This slot is connected to the done() signal emitted by the CscopeVerifier
 * object constructed in verifyCscope().
 */
void KScope::slotCscopeVerified(bool bResult, uint nArgs)
{
	statusBar()->message(i18n("Verifying Cscope installation...Done"), 3000);
	
	// Mark the flag even if Cscope was not found, to avoid nagging the user
	// (who may wish to use KScope even with Cscope disabled)
	m_bCscopeVerified = true;

	// Prompt the user in case Cscope is not properly installed
	if (!bResult) {
		KMessageBox::error(0, i18n("Cscope may not be properly installed on "
			"this system.\nPlease check the Cscope path specified in KScope's "
			"configuration dialogue."));
		slotConfigure();
		return;
	}
		
	// Set the discoverred supported command-line arguments
	CscopeFrontend::setSupArgs(nArgs);
	
	// Build the database, if required
	if (m_bRebuildDB) {
		m_bRebuildDB = false;
		slotRebuildDB();
	}
}

/**
 * Handles the "Project->Make..." menu command.
 * Displays the make dialogue.
 */
void KScope::slotProjectMake()
{
	QString sCmd, sDir;
	
	// Create the make dialogue, if it does not exist
	if (m_pMakeDlg == NULL) {
		// Create the dialogue
		m_pMakeDlg = new MakeDlg();
		
		// Set make parameters for this project
		m_pProjMgr->curProject()->getMakeParams(sCmd, sDir);
		m_pMakeDlg->setCommand(sCmd);
		m_pMakeDlg->setDir(sDir);
		
		// Show the relevant source location when an error link is clicked
		connect(m_pMakeDlg, SIGNAL(fileRequested(const QString&, uint)), this,
			SLOT(slotShowEditor(const QString&, uint)));
		
		// Show the dialogue
		m_pMakeDlg->show();
	}
	else if (m_pMakeDlg->isShown()) {
		// The dialogue exists, and is visible, just raise it
		m_pMakeDlg->raise();
		m_pMakeDlg->setActiveWindow();
	}
	else {
		// The dialogue exists but is closed, show it
		m_pMakeDlg->show();
	}
}

/**
 * Handles the "Project->Remake..." menu command.
 * Displays the make dialogue and runs the make command.
 */
void KScope::slotProjectRemake()
{
	// Make sure the make dialogue exists and is displayed
	slotProjectMake();
	
	// Run the make command
	m_pMakeDlg->slotMake();
}

/**
 * Handles the "Go->Global Bookmarks" menu command.
 * Displays a dialogue with the set of all bookmarks currently set in this
 * project.
 */
void KScope::slotShowBookmarks()
{
	BookmarksDlg dlg;
	QString sPath;
	uint nLine;
	
	// Load the bookmark list
	m_pEditTabs->showBookmarks(dlg.getView());
	
	// Show the dialogue
	if (dlg.exec() != QDialog::Accepted)
		return;
	
	// Go to the selected bookmark
	dlg.getBookmark(sPath, nLine);
	slotShowEditor(sPath, nLine);
}

/**
 * Prompts the user for a symbol to query.
 * Shows a dialog with a line edit widget, where the user can enter a symbol
 * on which to query Cscope. The meaning of the symbol depends on the type of
 * query.
 * @param	nType	The requested type of query (may be changed in the
 *					dialogue)
 * @param	sSymbol	Holds the requested symbol, upon successful return
 * @param	bPrompt	If false, the user is prompted only if a symbol cannot be
 *					determined automatically
 * @return	true if the user hs enetered a symbol, false otherwise
 */
bool KScope::getSymbol(uint& nType, QString& sSymbol, bool bPrompt)
{
	EditorPage* pPage;
	QString sSuggested;
	
	// Set the currently selected text, if any
	if ((pPage = m_pEditTabs->getCurrentPage()) != NULL)
		sSuggested = pPage->getSuggestedText();

	// Return if a symbol was found, and prompting is turned off
	if (!sSuggested.isEmpty() && !bPrompt) {
		sSymbol = sSuggested;
		return true;
	}
	
	// Show the symbol dialogue
	sSymbol = SymbolDlg::promptSymbol(this, nType, sSuggested);

	// Cannot accept empty strings
	if (sSymbol.isEmpty())
		return false;
	
	return true;
}

/**
 * Opens a file in a new editor tab.
 * If an editor page already exists for the requested file, it is selected.
 * Otherwise, a new page is created, and the requested file is loaded.
 * @param	sFilePath	The path of the file to open
 * @return	A pointer to the found or newly created editor page
 */
EditorPage* KScope::addEditor(const QString& sFilePath)
{
	EditorPage* pPage;
	QString sAbsFilePath;
	ProjectBase* pProj;
	
	// If the file name is given using a relative path, we need to convert
	// it to an absolute one
	// TODO: Project needs a translatePath() method
	pProj = m_pProjMgr->curProject();
	if (sFilePath[0] != '/' && pProj) {
		sAbsFilePath = QDir::cleanDirPath(pProj->getSourceRoot() + "/" +
			sFilePath);
	}
	else {
		sAbsFilePath = QDir::cleanDirPath(sFilePath);
	}
	
	// Do not open a new editor if one exists for this file
	pPage = m_pEditTabs->findEditorPage(sAbsFilePath);
	if (pPage != NULL)
		return pPage;

	// Create a new page
	pPage = createEditorPage();	
				
	// Open the requested file
	pPage->open(sAbsFilePath);
	
	return pPage;
}

/**
 * Creates a new editor page, and adds it to the editors tab widget.
 * @return	A pointer to the new page
 */
EditorPage* KScope::createEditorPage()
{
	KTextEditor::Document* pDoc;
	EditorPage* pPage;
	QPopupMenu* pMenu;
	ProjectBase* pProj;
	
	// Load a new document part
	pDoc = m_pEditMgr->add();
	if (pDoc == NULL)
		return NULL;

	// Create the new editor page
	pMenu = (QPopupMenu*)factory()->container(Config().getEditorPopupName(),
		this);
	pPage = new EditorPage(pDoc, pMenu, m_pEditTabs);
	m_pEditTabs->addEditorPage(pPage);

	// Show the file's path in the main title
	connect(pPage, SIGNAL(fileOpened(EditorPage*, const QString&)), this,
		SLOT(slotFileOpened(EditorPage*, const QString&)));

	// Show cursor position in the status bar
	connect(pPage, SIGNAL(cursorPosChanged(uint, uint)), this,
		SLOT(slotShowCursorPos(uint, uint)));
	
	// Rebuild the database after a file has changed
	connect(pPage, SIGNAL(fileSaved(const QString&, bool)), this,
		SLOT(slotFileSaved(const QString&, bool)));
	
	// Handle file drops
	connect(pPage->getView(), SIGNAL(dropEventPass(QDropEvent*)), this,
		SLOT(slotDropEvent(QDropEvent*)));

	// Apply per-project configuration
	pProj = m_pProjMgr->curProject();
	if (pProj && pProj->getTabWidth() > 0)
		pPage->setTabWidth(pProj->getTabWidth());
	
	return pPage;
}
	
/**
 * @return	true if database auto-rebuild is enabled for the current project,
 *			false otherwise
 */
inline bool KScope::isAutoRebuildEnabled()
{
	ProjectBase* pProj;
	
	pProj = m_pProjMgr->curProject();
	return (pProj && pProj->getAutoRebuildTime() >= 0);
}

/**
 * Deletes an editor page after it has been removed.
 * The document object associated with the page is removed from the part 
 * manager, and the view object is removed from the GUI manager.
 * This slot is connected to the editorRemoved() signal of the EditorTabs 
 * object.
 * @param	pPage	The editor page to delete
 */
void KScope::slotDeleteEditor(EditorPage* pPage)
{
	guiFactory()->removeClient(pPage->getView());
	m_pEditMgr->remove(pPage->getDocument());
	delete pPage;
}

/**
 * Sets an editor part as active when its owner tab is chosen.
 * Whenever a different editor tab is chosen, its editor part should become
 * the active part. This means that this part's GUI is merged with the
 * application's, and that it responds to actions.
 * @param	pOldPage	The editor page that has ceased to be active
 * @param	pNewPage	The newly chosen editor page
 */
void KScope::slotChangeEditor(EditorPage* pOldPage, EditorPage* pNewPage)
{
	KXMLGUIFactory* pFactory = guiFactory();
	
	// Remove the current GUI
	if (pOldPage)
		pFactory->removeClient(pOldPage->getView());

	// Set the new active part and create its GUI
	if (m_bUpdateGUI && pNewPage) {
		m_pEditMgr->setActivePart(pNewPage->getDocument());
		pFactory->addClient(pNewPage->getView());
		m_sCurFilePath = pNewPage->getFilePath();
		setCaption(m_pProjMgr->getProjName() + " - " + m_sCurFilePath);
	}
	
	// Enable/disable file-related actions, if necessary
	if (pOldPage && !pNewPage)
		m_pActions->slotEnableFileActions(false);
	else if (!pOldPage && pNewPage)
		m_pActions->slotEnableFileActions(true);
}

/**
 * Opens an editor for the given file and sets the cursor to the beginning of 
 * the requested line.
 * @param	sFilePath	The full path of the file to open for editing
 * @param	nLine		The number of the line on which to position the
 *						cursor, or 0 to maintain the cursor in its current
 *						position (which does not affect the position history)
 */
void KScope::slotShowEditor(const QString& sFilePath, uint nLine)
{
	EditorPage* pPage;

	// Save current position in the position history
	if (nLine != 0 && (pPage = m_pEditTabs->getCurrentPage())) {
		m_pQueryWidget->addHistoryRecord(m_sCurFilePath, m_nCurLine,
			pPage->getLineContents(m_nCurLine));
	}
	
	// Open the requested file (or select an already-open editor page)
	pPage = addEditor(sFilePath);
	if (pPage == NULL)
		return;
	
	// Make sure the main window is visible
	raise();
	setWindowState(windowState() & ~WindowMinimized | WindowActive);
	
	if (nLine != 0) {
		// Set the cursor to the requested line
		pPage->slotGotoLine(nLine);
	
		// Add the new position to the position history
		m_pQueryWidget->addHistoryRecord(m_sCurFilePath, m_nCurLine,
			pPage->getLineContents(m_nCurLine));
	}
}

/**
 * A wrapper around slotShowEditor, that enables auto-hiding of the query
 * widget after a query result has been chosen.
 * This slot is connected to the lineRequested() signal emitted by a QueryPage
 * object.
 * @param	sFilePath	The full path of the file to open for editing
 * @param	nLine		The number of the line on which to position the cursor
 */
void KScope::slotQueryShowEditor(const QString& sFilePath, uint nLine)
{
	// Hide the query window, if it was hidden before a query was initiated
	if (m_bHideQueryOnSelection)
		toggleQueryWindow(false);
	
	// Open an editor at the requested line
	slotShowEditor(sFilePath, nLine);
}

/**
 * Handles the "Go->Position History" menu command.
 * Ensures that the query window is visible, and selects the active history
 * page.
 */
void KScope::slotHistoryShow()
{
	toggleQueryWindow(true);
	m_pQueryWidget->selectActiveHistory();
}

/**
 * Handles the "File->New" menu command.
 * Creates an editor page for a new unnamed file.
 */
void KScope::slotNewFile()
{
	EditorPage* pPage;

	// Create the new editor page
	pPage = createEditorPage();
	
	// Mark the page as containing a new file
	pPage->setNewFile();
}

/**
 * Handles the "File->Open" menu command.
 * Prompts the user for a file name, and opens it in a new editor page.
 */
void KScope::slotOpenFile()
{
	ProjectBase* pProj;
	QStringList slFiles;
	QStringList::Iterator itr;
	
	// Prompt the user for the file(s) to open.
	pProj = m_pProjMgr->curProject();
	slFiles = KFileDialog::getOpenFileNames(pProj ? pProj->getSourceRoot() : 
		QString::null);
	
	// Open all selected files.
	for (itr = slFiles.begin(); itr != slFiles.end(); ++itr) {
		if (!(*itr).isEmpty())
			slotShowEditor(*itr, 0);
	}
}

/**
 * Handles the "File->Close" menu command.
 * Closes the currently active editor page.
 */
void KScope::slotCloseEditor()
{
	m_pEditTabs->removeCurrentPage();
}

/**
 * Handles the "Window->Close All" menu command.
 * Closes all open editor pages.
 */
void KScope::slotCloseAllWindows()
{
	m_bUpdateGUI = false;
	m_pEditTabs->removeAllPages();
	m_bUpdateGUI = true;
}

/**
 * Displays error messages from a Cscope process.
 * This slot is connected to the progress() signal emitted by the any
 * Cscope process.
 * @param	sMsg	The error message
 */
void KScope::slotCscopeError(const QString& sMsg)
{
	m_pMsgDlg->addText(sMsg);
}

/**
 * Reports progress information from the Cscope process responsible for
 * rebuilding the cross-reference database.
 * This slot is connected to the progress() signal emitted by the builder
 * process.
 * Progress information is displayed in the status bar.
 * @param	nFiles	The number of files scanned
 * @param	nTotal	The total number of files in the project
 */
void KScope::slotBuildProgress(int nFiles, int nTotal)
{
	QString sMsg;
	
	// Use the progress dialogue, if it exists (first time builds)
	if (m_pProgressDlg) {
		m_pProgressDlg->setValue((nFiles * 100) / nTotal);
		return;
	}
	
	// Show progress information
	sMsg = i18n("Rebuilding the cross reference database...") + " " +
		QString::number((nFiles * 100) / nTotal) + "%";
	statusBar()->message(sMsg);
}

/**
 * Reports to the user that Cscope has started building the inverted index.
 * This slot is connected to the buildInvIndex() signal emitted by the 
 * builder process.
 */
void KScope::slotBuildInvIndex()
{
	if (m_pProgressDlg) {
		m_pProgressDlg->setLabel(i18n("Please wait while KScope builds the "
			"inverted index"));
		m_pProgressDlg->setIdle();
		return;
	}
	
	statusBar()->message(i18n("Rebuilding inverted index..."));
}

/**
 * Informs the user the database rebuild process has finished.
 * This slot is connected to the finished() signal emitted by the builder
 * process.
 */
void KScope::slotBuildFinished(uint)
{
	// Delete the progress dialogue, if it exists (first time builds)
	if (m_pProgressDlg) {
		delete m_pProgressDlg;
		m_pProgressDlg = NULL;
		return;
	}
	
	// Show a message in the status bar
	statusBar()->message(i18n("Rebuilding the cross reference database..."
		"Done!"), 3000);
}

/**
 * Called if the build process failed to complete.
 * This slot is connected to the aborted() signal emitted by the builder
 * process.
 */
void KScope::slotBuildAborted()
{
	// Delete the progress dialogue, if it exists (first time builds)
	if (m_pProgressDlg) {
		delete m_pProgressDlg;
		m_pProgressDlg = NULL;
	
		// Display a failure message
		KMessageBox::error(0, i18n("The database could not be built.\n"
			"Cross-reference information will not be available for this "
			"project.\n"
			"Please ensure that the Cscope parameters were correctly "
			"entered in the \"Settings\" dialogue."));		
		return;
	}
	
	// Show a message in the status bar
	statusBar()->message(i18n("Rebuilding the cross reference database..."
		"Failed"), 3000);	
}

/**
 * Applies the selected user preferences once the "Apply" or "OK" buttons in
 * the preferences dialog is clicked.
 */
void KScope::slotApplyPref()
{
	m_pQueryWidget->applyPrefs();
	m_pFileList->applyPrefs();
	m_pEditTabs->applyPrefs();
	m_pEditMgr->applyPrefs();

	// Enable/disable the external editor menu item
	m_pActions->enableExtEditor(Config().useExtEditor());
}

/**
 * Displays the current cursor position, whenever it is moved by the user.
 * This slot is connected to the cursorPosChanged() signal emitted by an
 * EditorPage object.
 * @param	nLine	The new line number
 * @param	nCol	The new column number
 */
void KScope::slotShowCursorPos(uint nLine, uint nCol)
{
	KStatusBar* pStatus = statusBar();
	QString sText;
	
	/* Show the line and column numbers. */
	QTextOStream(&sText) << " Line: " << nLine << " Col: " << nCol << " ";
	pStatus->changeItem(sText, 0);
	
	/* Store the current line. */
	m_nCurLine = nLine;
}

/**
 * Stores the path of a newly opened file.
 * This slot is connected to the fileOpened() signal emitted by an
 * EditorPage object.
 * @param	sFilePath	The full path of the opened file
 */
void KScope::slotFileOpened(EditorPage*, const QString& sFilePath)
{
	m_sCurFilePath = sFilePath;
	setCaption(m_pProjMgr->getProjName() + " - " + m_sCurFilePath);
}

/**
 * Sets a timer for rebuilding the database after a file has been saved.
 * This slot is connected to the fileSaved() signal emitted by an EditorPage
 * object.
 * The time period before rebuilding is determined on a per-project basis.
 * @param	sPath	The full path of the modified file that caused this event
 * @param	bIsNew	true if this is a new file, false otherwise
 */
void KScope::slotFileSaved(const QString& sPath, bool bIsNew)
{
	ProjectBase* pProj;
	int nTime;
	
	pProj = m_pProjMgr->curProject();
	if (!pProj)
		return;
	
	// Prompt the user to add this file to the current project
	if (bIsNew && !pProj->isTemporary()) {
		if (KMessageBox::questionYesNo(0, 
			i18n("Whould you like to add this file to the active project?")) == 
				  KMessageBox::Yes) {
			
			// Add the path to the 'cscope.files' file
			if (!((Project*)pProj)->addFile(sPath)) {
				KMessageBox::error(0, i18n("Failed to write the file list."));
				return;
			}
			
			// Add the path to the file list widget
			m_pFileList->addItem(sPath);
			
			// Rebuild immediately
			slotRebuildDB();
			return;
		}
	}
	
	// Get the project's auto-rebuild time
	nTime = pProj->getAutoRebuildTime();
	
	// Do nothing if the time is set to -1
	if (nTime == -1)
		return;
		
	// Check if the file is included in the project (external files should
	// not trigger the timer)
	if (!m_pFileList->findFile(sPath))
		return;
	
	// Rebuild immediately for a time set to 0
	if (nTime == 0) {
		slotRebuildDB();
		return;
	}

	// Reset the rebuild timer
	m_timerRebuild.start(nTime * 1000, true);
}

/**
 * Handles file drops inside the editors tab widget.
 * Opens all files dropped over the widget.
 * @param	pEvent	Pointer to an object containing the list of dropped files
 */
void KScope::slotDropEvent(QDropEvent* pEvent)
{
	KURL::List list;
	KURL::List::Iterator itr;
		
	// Create a list of file URLs
	if (!KURLDrag::decode(pEvent, list))
		return;
		
	// Open all files in the list
	for (itr = list.begin(); itr != list.end(); ++itr)
		addEditor((*itr).path());
}

#include "kscope.moc"
