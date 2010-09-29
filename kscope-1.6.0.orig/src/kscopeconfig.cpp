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

#include <kconfig.h>
#include <kapplication.h>
#include <kglobalsettings.h>
#include "kscopeconfig.h"

// NOTE:
// This configuration file entry controls whether the welcome dialogue is
// displayed. Normally it only needs to be shown once, but the entry's name
// can be changed across versions to force the display of new information.
#define SHOW_WELCOME_ENTRY "ShowWelcomeDlg"

/**
 * Stores the display name and the configuration file entry for a configurable
 * GUI element.
 * @author	Elad Lahav
 */
struct ElementInfo
{
	/** The display name of the element. */
	const char* szName;
	
	/** The configuration file entry. */
	const char* szEntry;
};

/**
 * A list of GUI elements for which the colour can be configured.
 */
const ElementInfo eiColors[] = {
	{ "File List (Foreground)", "FileListFore" },
	{ "File List (Background)", "FileListBack" },
	{ "Tag List (Foreground)", "TagListFore" },
	{ "Tag List (Background)", "TagListBack" },
	{ "Query Window (Foreground)", "QueryListFore" },
	{ "Query Window (Background)", "QueryListBack" },
	{ "Call Graph (Background)", "GraphBack" },
	{ "Call Graph (Nodes)", "GraphNode" },
	{ "Call Graph (Text)", "GraphText" },
	{ "Call Graph (Multi-Call Nodes)", "GraphMultiCall" } 
};

/**
 * A list of GUI elements for which the font can be configured.
 */
const ElementInfo eiFonts[] = {
	{ "File List", "FileList" },
	{ "Tag List", "TagList" },
	{ "Query Page", "QueryList" },
	{ "Call Graph", "Graph" }
};

#define COLOR_NAME(_i)	eiColors[_i].szName
#define COLOR_ENTRY(_i)	eiColors[_i].szEntry
#define FONT_NAME(_i)	eiFonts[_i].szName
#define FONT_ENTRY(_i)	eiFonts[_i].szEntry

KScopeConfig::ConfParams KScopeConfig::s_cpDef = {
	"/usr/bin/cscope", // Cscope path
	"/usr/bin/ctags", // Ctags path
	"/usr/bin/dot", // Dot path
	true, // Show the tag list
	SPLIT_SIZES(), // Tag list width
	{
		QColor(black), // File list foreground
		QColor(white), // File list background
		QColor(black), // Tag list foreground
		QColor(white), // Tag list background
		QColor(black), // Query page foreground
		QColor(white), // Query page background
		QColor("#c0c0c0"), // Call graph background
		QColor("#c0ff80"), // Call graph nodes
		QColor(black), // Call graph text
		QColor("#ff8000")
	},
	{
		QFont(), // Font definitions are overriden in load()
		QFont(),
		QFont(),
		QFont()
	},
	NameAsc, // Ctags sort order
	false, // Read-only mode
	true, // Load last project
	true, // Automatic tag highlighting
	false, // Brief query captions
	true, // Warn when file is modified on the disk
	true, // Sort files when a project is loaded
	"kate --line %L %F", // External editor example
	Fast, // System profile
	Embedded, // Editor context menu
	"TB",  // Call graph orientation
	10, // Maximum calls per graph node
	0 // Default graph view
};

/**
 * Class constructor.
 */
KScopeConfig::KScopeConfig() : m_bFontsChanged(false)
{
}

/**
 * Class destructor.
 */
KScopeConfig::~KScopeConfig()
{
}

/**
 * Reads KScope's parameters from the standard configuration file.
 */
void KScopeConfig::load()
{
	uint i;
	
	KConfig* pConf = kapp->config();

	// Need a working instance to get the system's default font (cannot be
	// initialised statically)
	s_cpDef.fonts[FileList] = KGlobalSettings::generalFont();
	s_cpDef.fonts[TagList] = KGlobalSettings::generalFont();
	s_cpDef.fonts[QueryWindow] = KGlobalSettings::generalFont();
	s_cpDef.fonts[Graph] = KGlobalSettings::generalFont();
	
	// Read the paths to required executables
	pConf->setGroup("Programs");
	m_cp.sCscopePath = pConf->readEntry("CScope");
	m_cp.sCtagsPath = pConf->readEntry("CTags");
	m_cp.sDotPath = pConf->readEntry("Dot");

	// Read size and position parameters
	pConf->setGroup("Geometry");
	m_cp.bShowTagList = pConf->readBoolEntry("ShowTagList", 
		s_cpDef.bShowTagList);
	m_cp.siEditor = pConf->readIntListEntry("Editor");
	if (m_cp.siEditor.empty())
		m_cp.siEditor << 200 << 800;

	// Read the recent projects list
	pConf->setGroup("Projects");
	m_slProjects = pConf->readListEntry("Recent");

	// Read colour settings
	pConf->setGroup("Colors");
	for (i = 0; i <= LAST_COLOR; i++) {
		m_cp.clrs[i] = pConf->readColorEntry(COLOR_ENTRY(i),
			&s_cpDef.clrs[i]);
	}

	// Read font settings
	pConf->setGroup("Fonts");
	for (i = 0; i <= LAST_FONT; i++) {
		m_cp.fonts[i] = pConf->readFontEntry(FONT_ENTRY(i),
			&s_cpDef.fonts[i]);
	}
	
	// Other options
	pConf->setGroup("Options");
	m_cp.ctagSortOrder = 
		(CtagSort)pConf->readUnsignedNumEntry("CtagSortOrder",
		s_cpDef.ctagSortOrder);
	m_cp.bReadOnlyMode = pConf->readBoolEntry("ReadOnlyMode", 
		s_cpDef.bReadOnlyMode);
	m_cp.bLoadLastProj = pConf->readBoolEntry("LoadLastProj", 
		s_cpDef.bLoadLastProj);
	m_cp.bAutoTagHl = pConf->readBoolEntry("AutoTagHl", 
		s_cpDef.bAutoTagHl);
	m_cp.bBriefQueryCaptions = pConf->readBoolEntry("BriefQueryCaptions", 
		s_cpDef.bBriefQueryCaptions);
	m_cp.bWarnModifiedOnDisk = pConf->readBoolEntry("WarnModifiedOnDisk", 
		s_cpDef.bWarnModifiedOnDisk);
	m_cp.bAutoSortFiles = pConf->readBoolEntry("AutoSortFiles",
		s_cpDef.bAutoSortFiles);
	m_cp.sExtEditor = pConf->readEntry("ExternalEditor", s_cpDef.sExtEditor);
	m_cp.profile = (SysProfile)pConf->readUnsignedNumEntry("SystemProfile",
		s_cpDef.profile);
	m_cp.popup = (EditorPopup)pConf->readUnsignedNumEntry("EditorPopup",
		s_cpDef.popup);
	m_cp.sGraphOrient = pConf->readEntry("GraphOrientation",
		s_cpDef.sGraphOrient);
	m_cp.nGraphMaxNodeDegree = pConf->readNumEntry("GraphMaxNodeDegree",
		s_cpDef.nGraphMaxNodeDegree);
	m_cp.nDefGraphView = pConf->readNumEntry("DefGraphView",
		s_cpDef.nDefGraphView);
}

/**
 * Sets default values to he configuration parameters (except for those where
 * a default value has no meaning, such as the recent projects list).
 */
void KScopeConfig::loadDefault()
{
	m_cp = s_cpDef;
}

/**
 * Loads the layout of the main window.
 * @param	pMainWindow	Pointer to the main docking window
 */
void KScopeConfig::loadWorkspace(KDockMainWindow* pMainWindow)
{
	pMainWindow->readDockConfig(kapp->config(), "Workspace");
}
 
/**
 * Writes KScope's parameters from the standard configuration file.
 */
void KScopeConfig::store()
{
	uint i;
	
	KConfig* pConf = kapp->config();

	// Write the paths to required executables
	pConf->setGroup("Programs");
	pConf->writeEntry("CScope", m_cp.sCscopePath);
	pConf->writeEntry("CTags", m_cp.sCtagsPath);
	pConf->writeEntry("Dot", m_cp.sDotPath);

	// Write size and position parameters
	pConf->setGroup("Geometry");
	pConf->writeEntry("ShowTagList", m_cp.bShowTagList);
	pConf->writeEntry("Editor", m_cp.siEditor);

	// Write the recent projects list
	pConf->setGroup("Projects");
	pConf->writeEntry("Recent", m_slProjects);

	// Write colour settings
	pConf->setGroup("Colors");
	for (i = 0; i <= LAST_COLOR; i++)
		pConf->writeEntry(COLOR_ENTRY(i), m_cp.clrs[i]);

	// Write font settings
	if (m_bFontsChanged) {
		pConf->setGroup("Fonts");
		for (i = 0; i <= LAST_FONT; i++)
			pConf->writeEntry(FONT_ENTRY(i), m_cp.fonts[i]);
		
		m_bFontsChanged = false;
	}
		
	// Other options
	pConf->setGroup("Options");
	pConf->writeEntry("CtagSortOrder", (uint)m_cp.ctagSortOrder);
	pConf->writeEntry("ReadOnlyMode", m_cp.bReadOnlyMode);
	pConf->writeEntry("LoadLastProj", m_cp.bLoadLastProj);
	pConf->writeEntry("AutoTagHl", m_cp.bAutoTagHl);
	pConf->writeEntry("BriefQueryCaptions", m_cp.bBriefQueryCaptions);
	pConf->writeEntry("WarnModifiedOnDisk", m_cp.bWarnModifiedOnDisk);
	pConf->writeEntry("AutoSortFiles", m_cp.bAutoSortFiles);
	pConf->writeEntry("ExternalEditor", m_cp.sExtEditor);
	pConf->writeEntry("SystemProfile", (uint)m_cp.profile);
	pConf->writeEntry("EditorPopup", (uint)m_cp.popup);
	pConf->writeEntry("GraphOrientation", m_cp.sGraphOrient);
	pConf->writeEntry("GraphMaxNodeDegree", m_cp.nGraphMaxNodeDegree);
	pConf->writeEntry("DefGraphView", m_cp.nDefGraphView);
	
	// Do not report it's the first time on the next run
	pConf->setGroup("General");
	pConf->writeEntry("FirstTime", false);
	pConf->writeEntry(SHOW_WELCOME_ENTRY, false);
}

/**
 * Stores the layout of the main window.
 * @param	pMainWindow	Pointer to the main docking window
 */
void KScopeConfig::storeWorkspace(KDockMainWindow* pMainWindow)
{
	pMainWindow->writeDockConfig(kapp->config(), "Workspace");
}

/**
 * Determines if this is the first time KScope was launched by the current
 * user.
 * @return	true if this is the first time, false otherwise
 */
bool KScopeConfig::isFirstTime()
{
	KConfig* pConf = kapp->config();

	pConf->setGroup("General");
	return pConf->readBoolEntry("FirstTime", true);
}

/**
 * Determines if the welcome dialogue should be displayed.
 * Note that while the dialogue is displayed on the first invocation of KScope,
 * it may be required on other occasions (e.g., to display important information
 * on a per-version basis) and thus it is separated from isFirstTime().
 * @return	true if the dialogue should be shown, false otherwise
 */
bool KScopeConfig::showWelcomeDlg()
{
	KConfig* pConf = kapp->config();

	pConf->setGroup("General");
	return pConf->readBoolEntry(SHOW_WELCOME_ENTRY, true);
}

/**
 * @return	The full path of the Cscope executable
 */
const QString& KScopeConfig::getCscopePath() const
{
	return m_cp.sCscopePath;
}

/**
 * @param	sPath	The full path of the Cscope executable
 */
void KScopeConfig::setCscopePath(const QString& sPath)
{
	m_cp.sCscopePath = sPath;
}

/**
 * @return	The full path of the Ctags executable
 */
const QString& KScopeConfig::getCtagsPath() const
{
	return m_cp.sCtagsPath;
}

/**
 * @param	sPath	The full path of the Ctags executable
 */
void KScopeConfig::setCtagsPath(const QString& sPath)
{
	m_cp.sCtagsPath = sPath;
}

/**
 * @return	The full path of the Dot executable
 */
const QString& KScopeConfig::getDotPath() const
{
	return m_cp.sDotPath;
}

/**
 * @param	sPath	The full path of the Dot executable
 */
void KScopeConfig::setDotPath(const QString& sPath)
{
	m_cp.sDotPath = sPath;
}

/**
 * @return	A sorted list of recently used project paths.
 */
const QStringList& KScopeConfig::getRecentProjects() const
{
	return m_slProjects;
}

/**
 * Adds the given project path to the beginning of the recently used projects
 * list.
 * @param	sProjPath	The path of the project to add
 */
void KScopeConfig::addRecentProject(const QString& sProjPath)
{
	QStringList::Iterator itr;
	
	itr = m_slProjects.find(sProjPath);
	if (itr != m_slProjects.end())
		m_slProjects.remove(itr);
			
	m_slProjects.prepend(sProjPath);
}

/**
 * Removes the given project path from recently used projects list.
 * @param	sProjPath	The path of the project to remove
 */
void KScopeConfig::removeRecentProject(const QString& sProjPath)
{
	m_slProjects.remove(sProjPath);
}

/**
 * @return	true if the tag list should be visible, false otherwise
 */
bool KScopeConfig::getShowTagList() const
{
	return m_cp.bShowTagList;
}

/**
 * @param	bShowTagList	true to make the tag list visible, false otherwise
 */
void KScopeConfig::setShowTagList(bool bShowTagList)
{
	m_cp.bShowTagList = bShowTagList;
}

/**
 * @return	A list containing the widths of the Ctags list part and the
 * editor part in an editor page.
 */
const SPLIT_SIZES& KScopeConfig::getEditorSizes() const
{
	return m_cp.siEditor;
}

/**
 * @param	siEditor	A list containing the widths of the Ctags list part
 * and the editor part in an editor page.
 */
void KScopeConfig::setEditorSizes(const SPLIT_SIZES& siEditor)
{
	m_cp.siEditor = siEditor;
}

/**
 * Finds a colour to use for a GUI element.
 * @param	ce		Identifies the GUI element
 * @return	A reference to the colour object to use
 */
const QColor& KScopeConfig::getColor(ColorElement ce) const
{
	return m_cp.clrs[ce];
}

/**
 * Returns the display name of a GUI element whose colour can be configured.
 * @param	ce	The GUI element
 * @return	A name used in the colour configuration page
 */
QString KScopeConfig::getColorName(ColorElement ce) const
{
	return COLOR_NAME(ce);
}

/**
 * Sets a new colour to a GUI element.
 * @param	ce		Identifies the GUI element
 * @param	clr		The colour to use
 */ 
void KScopeConfig::setColor(ColorElement ce, const QColor& clr)
{
	m_cp.clrs[ce] = clr;
}

/**
 * Finds a font to use for a GUI element.
 * @param	fe		Identifies the GUI element
 * @return	A reference to the font object to use
 */
const QFont& KScopeConfig::getFont(FontElement fe) const
{
	return m_cp.fonts[fe];
}

/**
 * Returns the display name of a GUI element whose font can be configured.
 * @param	ce	The GUI element
 * @return	A name used in the font configuration page
 */
QString KScopeConfig::getFontName(FontElement ce) const
{
	return FONT_NAME(ce);
}

/**
 * Sets a new font to a GUI element.
 * @param	fe		Identifies the GUI element
 * @param	font	The font to use
 */ 
void KScopeConfig::setFont(FontElement fe, const QFont& font)
{
	m_bFontsChanged = true;
	m_cp.fonts[fe] = font;
}

/**
 * @return	The column and direction by which the tags should be sorted
 */
KScopeConfig::CtagSort KScopeConfig::getCtagSortOrder()
{
	return m_cp.ctagSortOrder;
}

/**
 * @param	ctagSortOrder	The column and direction by which the tags should
 *							be sorted
 */
void KScopeConfig::setCtagSortOrder(CtagSort ctagSortOrder)
{
	m_cp.ctagSortOrder = ctagSortOrder;
}

/**
 * @return	true to work in Read-Only mode, false otherwise
 */
bool KScopeConfig::getReadOnlyMode()
{
	return m_cp.bReadOnlyMode;
}

/**
 * @param	bReadOnlyMode	true to work in Read-Only mode, false otherwise
 */
void KScopeConfig::setReadOnlyMode(bool bReadOnlyMode)
{
	m_cp.bReadOnlyMode = bReadOnlyMode;
}

/**
 * @return	true to load the last project on start-up, false otherwise
 */
bool KScopeConfig::getLoadLastProj()
{
	return m_cp.bLoadLastProj;
}

/**
 * @param	bLoadLastProj	true to load the last project on start-up, false
 *							otherwise
 */
void KScopeConfig::setLoadLastProj(bool bLoadLastProj)
{
	m_cp.bLoadLastProj = bLoadLastProj;
}

/**
 * @return	true to enable tag highlighting based on cursor position, false
 *			to disable this feature
 */
bool KScopeConfig::getAutoTagHl()
{
	return m_cp.bAutoTagHl;
}

/**
 * @param	bAutoTagHl	true to enable tag highlighting based on cursor
 *			position, false to disable this feature
 */
void KScopeConfig::setAutoTagHl(bool bAutoTagHl)
{
	m_cp.bAutoTagHl = bAutoTagHl;
}

/**
 * @return	true to use the short version of the query captions, false to use
 *			the long version
 */
bool KScopeConfig::getUseBriefQueryCaptions()
{
	return m_cp.bBriefQueryCaptions;
}

/**
 * @param	bBrief	true to use the short version of the query captions, false
 *			to use the long version
 */
void KScopeConfig::setUseBriefQueryCaptions(bool bBrief)
{
	m_cp.bBriefQueryCaptions = bBrief;
}

/**
 * @return	true to warn user when file is modified on disk, false
 *			otherwise
 */
bool KScopeConfig::getWarnModifiedOnDisk()
{
	return m_cp.bWarnModifiedOnDisk;
}

/**
 * @param	bWarn	true to warn user when file is modified on disk,
 *					false otherwise
 */
void KScopeConfig::setWarnModifiedOnDisk(bool bWarn)
{
	m_cp.bWarnModifiedOnDisk = bWarn;
}

/**
 * @return	true to sort files when a project is loaded, false otherwise
 */
bool KScopeConfig::getAutoSortFiles()
{
	return m_cp.bAutoSortFiles;
}

/**
 * @param	bSort	true to sort files when a project is loaded, false 
 *					otherwise
 */
void KScopeConfig::setAutoSortFiles(bool bSort)
{
	m_cp.bAutoSortFiles = bSort;
}

/**
 * @return	A command line for launching an external editor
 */
const QString& KScopeConfig::getExtEditor()
{
	return m_cp.sExtEditor;
}

/**
 * @param	sExtEditor	A command line for launching an external editor
 */
void KScopeConfig::setExtEditor(const QString& sExtEditor)
{
	m_cp.sExtEditor = sExtEditor;
}

/**
 * Determines if an external editor should be used.
 * An external editor is used if KScope is in Read-Only mode, and a command-
 * line for the editor was specified.
 * @return	true to use an external editor, false otherwise
 */
bool KScopeConfig::useExtEditor()
{
	return !m_cp.sExtEditor.isEmpty();
}

/**
 * @return	The chosen profile for this system (@see SysProfile)
 */
KScopeConfig::SysProfile KScopeConfig::getSysProfile() const
{
	return m_cp.profile;
}

/**
 * @param	profile	The system profile to use (@see SysProfile)
 */
void KScopeConfig::setSysProfile(KScopeConfig::SysProfile profile)
{
	m_cp.profile = profile;
}

/**
 * @return	The chosen popup menu type for the embedded editor (@see
 *			EditorPopup)
 */
KScopeConfig::EditorPopup KScopeConfig::getEditorPopup() const
{
	return m_cp.popup;
}

/**
 * @return	The name of the popup menu to use in the embedded editor
 */
QString KScopeConfig::getEditorPopupName() const
{
	switch (m_cp.popup) {
	case Embedded:
		return "ktexteditor_popup";
		
	case KScopeOnly:
		return "kscope_popup";
	}
	
	// Will not happen, but the compiler complains if no return statement is
	// given here
	return "";
}

/**
 * @param	popup	The popup menu to use for the embedded editor (@see
 *					EditorPopup)
 */
void KScopeConfig::setEditorPopup(KScopeConfig::EditorPopup popup)
{
	m_cp.popup = popup;
}

/**
 * @return	The default orientation for call graphs
 */
QString KScopeConfig::getGraphOrientation() const
{
	return m_cp.sGraphOrient;
}

/**
 * @param	sOrient	The default orientation for call graphs
 */
void KScopeConfig::setGraphOrientation(const QString& sOrient)
{
	m_cp.sGraphOrient = sOrient;
}

/**
 * @return	The maximal number of calls per graph node
 */
int KScopeConfig::getGraphMaxNodeDegree() const
{
	return m_cp.nGraphMaxNodeDegree;
}

/**
 * @param	nMaxNodeDegree	The maximal number of calls per graph node
 */
void KScopeConfig::setGraphMaxNodeDegree(int nMaxNodeDegree)
{
	m_cp.nGraphMaxNodeDegree = nMaxNodeDegree;
}

/**
 * @return	The default view in the call graph dialogue
 */
int KScopeConfig::getDefGraphView() const
{
	return m_cp.nDefGraphView;
}

/**
 * @param	nDefGraphView	The default view in the call graph dialogue
 */
void KScopeConfig::setDefGraphView(int nDefGraphView)
{
	m_cp.nDefGraphView = nDefGraphView;
}

/**
 * Returns a reference to a global configuration object.
 * The static object defined is this function should be the only KSCopeConfig
 * object in this programme. Any code that wishes to get or set global
 * configuration parameters, should call Config(), instead of defining its
 * own object.
 * @return	Reference to a statically allocated configuration object
 */
KScopeConfig& Config()
{
	static KScopeConfig conf;
	return conf;
}

#include "kscopeconfig.moc"
