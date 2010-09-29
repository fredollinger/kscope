/***************************************************************************
 *
 * Copyright (C) 2007 Elad Lahav (elad_lahav@users.sourceforge.net)
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
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AS IS'' AND ANY EXPRESS OR
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

#include "projectbase.h"
#include "kscopeconfig.h"
#include "cscopefrontend.h"

ProjectBase::ProjectBase()
{
}

ProjectBase::~ProjectBase()
{
}

bool ProjectBase::open(const QString& sPath)
{
	QFileInfo fi(sPath);
		
	// Make sure the file exists, and that is is a cross-reference file
	if (!fi.exists() || !isCscopeOut(fi.absFilePath()))
		return false;
		
	// Set the project's directory
	m_dir = fi.dirPath(true);
	
	// Set the name of the project to be the full path of the file
	m_sName = fi.absFilePath();
	
	// Initialise project options (assume source root is the folder holding the
	// cscope.out file)
	getDefOptions(m_opt);
	m_opt.sSrcRootPath = m_dir.absPath();
	
	return true;
}

/**
 * Determines if the cscope.out file for this project exists.
 * @return	true if the database exists, false otherwise
 */
bool ProjectBase::dbExists()
{
	return m_dir.exists("cscope.out");
}

void ProjectBase::getOptions(Options& opt) const
{
	getDefOptions(opt);
}

void ProjectBase::getMakeParams(QString& sCmd, QString& sDir) const
{
	sCmd = "make";
	sDir = getSourceRoot();
}

/**
 * Fills a structure with default properties for a new project.
 * Default properties are partly based on the system profile.
 * @param	opt	The structure to fill
 */
void ProjectBase::getDefOptions(Options& opt)
{
	// Set default source path to file-system root
	opt.sSrcRootPath = "/";
	
	// Initialise MIME-type list
	opt.slFileTypes.append("*.c");
	opt.slFileTypes.append("*.h");

	// Set other options
	opt.bKernel = DEF_IS_KERNEL;
	opt.bInvIndex = DEF_INV_INDEX;
	opt.bNoCompress = DEF_NO_COMPRESS;
	opt.bSlowPathDef = DEF_SLOW_PATH;
	opt.nACMinChars = DEF_AC_MIN_CHARS;
	opt.nACDelay = DEF_AC_DELAY;
	opt.nACMaxEntries = DEF_AC_MAX_ENTRIES;
	opt.nTabWidth = DEF_TAB_WIDTH;
	
	// Set profile-dependant options
	if (Config().getSysProfile() == KScopeConfig::Fast) {
		opt.nAutoRebuildTime = 10;
		opt.bACEnabled = true;
	}
	else {
		opt.nAutoRebuildTime = -1;
		opt.bACEnabled = false;
	}
}
	
void ProjectBase::initOptions()
{
	// Load the options
	getOptions(m_opt);

	// Create the argument list for invoking Cscope
	m_nArgs = 0;
	if (m_opt.bKernel)
		m_nArgs |= CscopeFrontend::Kernel;
	if (m_opt.bInvIndex)
		m_nArgs |= CscopeFrontend::InvIndex;
	if (m_opt.bNoCompress)
		m_nArgs |= CscopeFrontend::NoCompression;
	if (m_opt.bSlowPathDef)
		m_nArgs |= CscopeFrontend::SlowPathDef;
}

/**
 * Determines if the given file is a Cscope cross-reference database.
 * @param	sPath	The full path of the file to check
 * @return	true if the given file is a cscope.out file, false otherwise
 */
bool ProjectBase::isCscopeOut(const QString& sPath)
{
	QFile file(sPath);
	QString sLine;
	int nVer;
	char szDir[PATH_MAX];

	// Try to open the file
	if (!file.open(IO_ReadOnly))
		return false;
		
	// Check if the first line matches the expected format
	sLine = QTextStream(&file).readLine();
	return sscanf(sLine.latin1(), "cscope %d %s", &nVer, szDir) == 2;
}

/**
 * Fills a list object with all files in the project.
 * List items are created by reading and parsing all file name entries from
 * the project's 'cscope.files' file.
 * Note that the file may contain option lines, beginning with a dash. These
 * should be ignored.
 * @param	pList	Pointer to the object to fill
 */
bool ProjectBase::loadFileList(FileListTarget* pList)
{
	QString sFilePath;
	QFile file;
	
	// Make sure the file exists
	if (!m_dir.exists("cscope.files"))
		return false;
	
	// Open the file
	file.setName(m_dir.absPath() + "/cscope.files");
	if (!file.open(IO_ReadOnly))
		return false;

	// Read all file names from the file
	QTextStream str(&file);
	while ((sFilePath = str.readLine()) != QString::null) {
		// Skip option lines
		if (sFilePath.at(0) == '-')
			continue;

		// Set the new list item
		pList->addItem(sFilePath);
	}

	file.close();
	return true;
}
