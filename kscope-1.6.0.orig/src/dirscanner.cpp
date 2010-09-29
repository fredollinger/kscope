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

#include <qapplication.h>
#include "dirscanner.h"

/**
 * Class constructor.
 * @param	nFiles		The number of files scanned since the previous event
 * @param	bFinished	true if all files were scanned, false otherwise
 */
DirScanEvent::DirScanEvent(int nFiles, bool bFinished)
	: QCustomEvent(EventId),
	m_nFiles(nFiles),
	m_bFinished(bFinished)
{
}

/**
 * Class constructor.
 * @param	pEventReceiver	Pointer to an object to receive DirScanEvent
 *							updates
 * @param	pDicFiles		Pointer to a map of current project files (to
 							avoid duplication)
 */
DirScanner::DirScanner(QObject* pEventReceiver,
	QDict<QListViewItem>* pDicFiles) : QThread(),
	m_pEventReceiver(pEventReceiver),
	m_pDicFiles(pDicFiles)
{
}

/**
 * Class destructor.
 */
DirScanner::~DirScanner()
{
}

/**
 * Begins a new search for source files.
 * Invokes the search thread on a given directory. The search may be either
 * recursive (i.e., the search will descend to each sub-directory) or flat
 * (will search the given directory only.)
 * @param	sDir		The name of the directory to search
 * @param	sNameFilter	Defines the search pattern
 * @param	bRecursive	true to descend into sub-dorectories, false otherwise
 */
void DirScanner::start(const QString& sDir, const QString& sNameFilter,
	bool bRecursive)
{
	// Initialise the search parameters
	m_dir = QDir(sDir);
	m_sNameFilter = sNameFilter;
	m_bCancel = false;
	m_bRecursive = bRecursive;
	m_slFiles.clear();

	// Invoke the thread
	QThread::start();
}

/**
 * Begins a scan of files on the directory associated with this object.
 * Note that this function is synchronous: it returns when the scan ends.
 */
void DirScanner::run()
{
	int nFiles;
	
	nFiles = scanDir(m_dir);
	QApplication::postEvent(m_pEventReceiver,
		new DirScanEvent(nFiles, true));
	
	m_setScanned.clear();
}

/**
 * Recursively scans a directory for a files matching the current filter.
 * @param	dir	A directory object set to the folder from which files are
 *			added
 * @return	The total number of files added
 */
int DirScanner::scanDir(QDir& dir)
{
	QString sCanon;
	QStringList slDirFiles, slDirs;
	QStringList::const_iterator itr;
	QString sFile;
	int nFiles = 0;

	if (m_bCancel)
		return -1;
		
	// Make sure this directory has not been previously visited (e.g., through a
	// symbolic link)
	sCanon = dir.canonicalPath();
	if (m_setScanned.exists(sCanon))
		return 0;
	
	m_setScanned.insert(sCanon);
	
	// Add all files in this directory
	slDirFiles = dir.entryList(m_sNameFilter, QDir::Files);
	for (itr = slDirFiles.begin(); itr != slDirFiles.end(); ++itr) {
		sFile = dir.absPath() + "/" + *itr;

		// Make sure an entry for this file does not exist
		if (m_pDicFiles->find(sFile) == NULL) {
			m_slFiles.append(sFile);
			nFiles++;
		}
	}

	QApplication::postEvent(m_pEventReceiver,
		new DirScanEvent(nFiles, false));
	
	// Recurse into sub-directories, if requested
	if (!m_bRecursive)
		return nFiles;

	slDirs = dir.entryList(QDir::Dirs);

	// Iterate the list of sub-directories
	for (itr = slDirs.begin(); itr != slDirs.end(); ++itr) {
		if (m_bCancel)
			return -1;
			
		// Skip the "." and ".." directories
		if (*itr == "." || *itr == "..")
			continue;

		// Add the files in each sub-directory
		QDir dirSub(dir);
		if (dirSub.cd(*itr))
			nFiles += scanDir(dirSub);
	}

	return nFiles;
}
