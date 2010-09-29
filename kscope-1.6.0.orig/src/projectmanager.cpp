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

#include <kmessagebox.h>
#include <klocale.h>
#include "projectmanager.h"
#include "project.h"
#include "kscopeconfig.h"

/**
 * Class constructor.
 */
ProjectManager::ProjectManager() : m_pCurProj(NULL)
{
}

/**
 * Class destructor.
 */
ProjectManager::~ProjectManager()
{
	close();
}

/**
 * Creates a project's directory, and associates this directory with the
 * current object. This directory is created under the given path, and using
 * the project's name (which, thus, has to be a legal file name). 
 * Note: this function attempts to create a new directory, so the given path
 * and name must not lead to an existing one.
 * @param	sName		The project's name
 * @param	sPath		The parent directory under which to create the 
 *						project's directory
 * @param	opt			A structure containing project options
 * @return	true if successful, false otherwise
 */
bool ProjectManager::create(const QString& sName, const QString& sPath, 
	const ProjectBase::Options& opt)
{
	// Make sure the directory doesn't exist
	QDir dir(sPath);
	if (dir.exists(sName)) {
		KMessageBox::error(0, i18n("Cannot create a project inside an "
			"existing directory"));
		return false;
	}

	// Try to create the projcet's directory
	if (!dir.mkdir(sName, false) || !dir.cd(sName, false)) {
		KMessageBox::error(0, i18n("Failed to create the project's "
			"directory"));
		return false;
	}
	
	return Project::create(sName, dir.absPath(), opt);
}

/**
 * Opens a project and makes it the current one.
 * @param	sPath	The directory containing the project's files
 * @return	true if successful, false otherwise
 */
bool ProjectManager::open(const QString& sPath)
{
	Project* pProj;
	
	// Close the current project
	close();
	
	// Try to open the new project
	pProj = new Project();
	if (!pProj->open(sPath)) {
		delete pProj;
		return false;
	}
	
	// Add to the list of recently opened projects
	Config().addRecentProject(sPath);
	
	// Project opened successfully
	m_pCurProj = pProj;
	return true;
}

/**
 * Opens a Cscope.out file as a temporary project.
 * @param	sFilePath	The full path of the Cscope.out file
 * @return	true if successful, false otherwise
 */
bool ProjectManager::openCscopeOut(const QString& sFilePath)
{
	ProjectBase* pProj;
	
	// Close the current project
	close();
	
	// Try to open the new project
	pProj = new ProjectBase();
	if (!pProj->open(sFilePath)) {
		delete pProj;
		return false;
	}
	
	// Add to the list of recently opened projects
	Config().addRecentProject(sFilePath);
	
	// Project opened successfully
	m_pCurProj = pProj;
	return true;	
}

/**
 * Performs clean-up on the project's variables, and detaches the associated
 * directory.
 */
void ProjectManager::close()
{
	if (m_pCurProj) {
		delete m_pCurProj;
		m_pCurProj = NULL;
	}
}

QString ProjectManager::getProjName() const
{
	if (!m_pCurProj)
		return i18n("No Project");
		
	return m_pCurProj->getName();
}
