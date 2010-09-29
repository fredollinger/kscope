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

#include <qpushbutton.h>
#include <qlistview.h>
#include <qlineedit.h>
#include <qregexp.h>
#include <klocale.h>
#include <kmessagebox.h>
#include <kfiledialog.h>
#include "projectfilesdlg.h"
#include "dirscanner.h"
#include "scanprogressdlg.h"
#include "kscopeconfig.h"

/**
 * Class constructor.
 * @param	pProjMgr	Pointer the KScope's project manager object
 * @param	pParent		The parent widget
 * @param	szName		The widget's name
 */
ProjectFilesDlg::ProjectFilesDlg(Project* pProj, QWidget* pParent,
	const char* szName) :
	ProjectFilesLayout(pParent, szName),
	m_pProj(pProj),
	m_pScanDlg(NULL),
	m_pItrItem(NULL),
	m_pLastItem(NULL)
{
	// Create the scanner object
	m_pScanner = new DirScanner(this, &m_dicFiles);

	// Initialise the list view
	m_pFileList->setSelectionMode(QListView::Extended);
	m_pFileList->addColumn("File Path");
	
	// Sort only when asked to by the user
	if (Config().getAutoSortFiles())
		m_pFileList->setSortColumn(0);
	else
		m_pFileList->setSortColumn(m_pFileList->columns() + 1);

	// Add file/directory/tree when the appropriate button is clicked
	connect(m_pAddFilesButton, SIGNAL(clicked()), this,
		SLOT(slotAddFiles()));
	connect(m_pAddDirButton, SIGNAL(clicked()), this, SLOT(slotAddDir()));
	connect(m_pAddTreeButton, SIGNAL(clicked()), this, SLOT(slotAddTree()));

	// Remove selected files/directory/tree when the appropriate button is
	// clicked
	connect(m_pRemSelButton, SIGNAL(clicked()), this, SLOT(slotRemSel()));
	connect(m_pRemDirButton, SIGNAL(clicked()), this, SLOT(slotRemDir()));
	connect(m_pRemTreeButton, SIGNAL(clicked()), this, SLOT(slotRemTree()));

	// Hide/show files according to filter
	connect(m_pFilterButton, SIGNAL(clicked()), this, SLOT(slotFilter()));
	connect(m_pShowAllButton, SIGNAL(clicked()), this, SLOT(slotShowAll()));
	
	// Close the dialog when OK/Cancel are clicked
	connect(m_pOKButton, SIGNAL(clicked()), this, SLOT(accept()));
	connect(m_pCancelButton, SIGNAL(clicked()), this, SLOT(reject()));

	// Fill the list with the project's files
	m_pFileList->setUpdatesEnabled(false);
	m_pProj->loadFileList(this);
	m_pFileList->setUpdatesEnabled(true);
	m_pFileList->triggerUpdate();
}

/**
 * Class destructor.
 */
ProjectFilesDlg::~ProjectFilesDlg()
{
	delete m_pScanner;
}

/**
 * Adds a single entry to the file list.
 * Implements the addItem() virtual method of the FileListTarget base
 * class. When a ProjectFilesDlg object is given as a parameter to
 * ProjectManager::fillList(), this method is called for each file included
 * in the project. A new list item is created, containing the file's path,
 * and is added to the list.
 * @param	sFilePath	The full path of a source file
 */
void ProjectFilesDlg::addItem(const QString& sFilePath)
{
	QListViewItem* pItem;
	
	pItem = new QListViewItem(m_pFileList, m_pLastItem);
	pItem->setText(0, sFilePath);
	m_pLastItem = pItem;
	m_dicFiles.insert(sFilePath, pItem);
}

/**
 * Retrieves the first file path in the list.
 * Imlpements the firstItem() virtual method of the FileListSource base
 * class.
 * @param	sFilePath	Contains the file path, upon successful return
 * @return	bool		true if successful, false if the list is empty
 */
bool ProjectFilesDlg::firstItem(QString& sFilePath)
{
	m_pItrItem = m_pFileList->firstChild();
	return nextItem(sFilePath);
}

/**
 * Retrieves the next file path in the list.
 * Imlpements the nextItem() virtual method of the FileListSource base
 * class. The function requires that firstItem() will be called to begin an
 * iteration through the file paths.
 * @param	sFilePath	Contains the file path, upon successful return
 * @return	bool		true if successful, false if no more items are
 *						available
 */
bool ProjectFilesDlg::nextItem(QString& sFilePath)
{
	if (m_pItrItem == NULL)
		return false;

	sFilePath = m_pItrItem->text(0);
	m_pItrItem = m_pItrItem->nextSibling();
	return true;
}

/**
 * Notifies the user on the progress of a directory scan (when adding a new
 * directory), and, if finished, allows the user to add these files to the
 * project.
 * @param	pEvent	The event object
 */
void ProjectFilesDlg::customEvent(QCustomEvent* pEvent)
{
	DirScanEvent* pDSE;
	QString sMsg;

	// Process only directory scan progress events
	if (((uint)pEvent->type()) != DirScanEvent::EventId)
		return;

	pDSE = (DirScanEvent*)pEvent;

	// Check if the scan has terminated
	if (!pDSE->m_bFinished) {
		// Create the scan progress dialog, if required
		if (m_pScanDlg == NULL) {
			m_pScanDlg = new ScanProgressDlg(this);
			connect(m_pScanDlg, SIGNAL(cancelled()), this,
				SLOT(slotCancelDirScan()));
		}

		// Set progress indication
		m_pScanDlg->addFiles(pDSE->m_nFiles);
		return;
	}

	// Destroy the scan progress dialog
	delete m_pScanDlg;
	m_pScanDlg = NULL;

	// Verify the thread has terminated
	m_pScanner->wait(500);
	if (!m_pScanner->finished())
		m_pScanner->terminate();

	// Do nothing if the operation was cancelled
	if (m_pScanner->wasCancelled())
		return;

	// Abort if no files were found
	if (pDSE->m_nFiles == 0) {
		KMessageBox::sorry(0, "No files were found");
		return;
	}

	// Prompt the user for the files to add
	sMsg.sprintf(i18n("Would you like to add %d files to your project?"),
		pDSE->m_nFiles);
	if (KMessageBox::questionYesNo(0, sMsg) == KMessageBox::No)
		return;

	// Add the files to the list
	const QStringList& slFiles = m_pScanner->getFiles();
	QStringList::const_iterator itr;

	for (itr = slFiles.begin(); itr != slFiles.end(); ++itr)
		addItem(*itr);
}

/**
 * Removes a single item from the file list.
 */
void ProjectFilesDlg::removeItem(QListViewItem* pItem)
{
	m_dicFiles.remove(pItem->text(0));
	delete pItem;
}

/**
 * Adds a list of files to the project.
 * Prompts the user for source files, and adds the selected files to the
 * current project.
 */
void ProjectFilesDlg::slotAddFiles()
{
	QStringList slFiles;
	QStringList::const_iterator itr;

	// Prompt the user
	slFiles = KFileDialog::getOpenFileNames(m_pProj->getSourceRoot(),
		m_pProj->getFileTypes());

	// Add the selected files, skipping existing entries
	for (itr = slFiles.begin(); itr != slFiles.end(); ++itr) {
		if (m_dicFiles.find(*itr) == NULL)
			addItem(*itr);
	}
}

/**
 * Adds all source files in a given directory to the project.
 * Prompts the user for a directory, and adds all files matching the
 * project's pattern to the current project.
 * Note that only source files in the selected directory are added, i.e., the
 * search does not descend to sub-directories.
 */
void ProjectFilesDlg::slotAddDir()
{
	QString sDir;
	QStringList slFiles;
	QStringList::const_iterator itr;

	// Prompt the user for a directory
	sDir = KFileDialog::getExistingDirectory(m_pProj->getSourceRoot());
	if (sDir.isEmpty())
		return;

	// Search for source files in this directory
	m_pScanner->start(sDir, m_pProj->getFileTypes(), false);
}

/**
 * Adds all source files in a given file system tree to the project.
 * Prompts the user for a directory, and adds all files matching the
 * project's pattern to the current project.
 * Note that source files are searched for in the given directory, as well as
 * in any of its sub-directories.
 */
void ProjectFilesDlg::slotAddTree()
{
	QString sDir;
	QStringList slFiles;
	QStringList::const_iterator itr;

	// Prompt the user for a directory
	sDir = KFileDialog::getExistingDirectory(m_pProj->getSourceRoot());
	if (sDir.isEmpty())
		return;

	// Search for source files in this directory
	m_pScanner->start(sDir, m_pProj->getFileTypes(), true);
}

/**
 * Removes the selected files from the project.
 */
void ProjectFilesDlg::slotRemSel()
{
	QListViewItem* pItem, * pPrevItem;
	
	// Prompt the user before removing the files
	if (KMessageBox::questionYesNo(0, i18n("Are you sure you want to remove "
		"the selected files from the project?")) == KMessageBox::No) {
		return;
	}

	// Remove the selected files
	pItem = m_pFileList->firstChild();
	while (pItem != NULL) {
		pPrevItem = pItem;
		pItem = pItem->nextSibling();
		
		if (pPrevItem->isSelected())
			removeItem(pPrevItem);
	}
}

/**
 * Removes all source files in a directory from the project.
 */
void ProjectFilesDlg::slotRemDir()
{
	QString sDir, sFilePath;
	QListViewItem* pItem, * pPrevItem;

	// Prompt the user for a directory
	sDir = KFileDialog::getExistingDirectory(m_pProj->getSourceRoot());
	if (sDir.isEmpty())
		return;
		
	// Confirm the directory removal
	if (KMessageBox::questionYesNo(0, i18n("Are you sure you want to remove "
		"the selected directory from the project?")) == KMessageBox::No) {
		return;
	}

	// Remove the files under the selected directory
	pItem = m_pFileList->firstChild();
	while (pItem != NULL) {
		pPrevItem = pItem;
		pItem = pItem->nextSibling();

		// Check if the file is under the selected directory
		sFilePath = pPrevItem->text(0);
		if (sFilePath.left(sFilePath.findRev('/') + 1) == sDir)
			removeItem(pPrevItem);
	}
}

/**
 * Removes all source files in a directory or any of its sub-directories from
 * the project.
 */
void ProjectFilesDlg::slotRemTree()
{
	QString sDir, sFilePath;
	QListViewItem* pItem, * pPrevItem;

	// Prompt the user for a directory
	sDir = KFileDialog::getExistingDirectory(m_pProj->getSourceRoot());
	if (sDir.isEmpty())
		return;

	// Confirm the directory removal
	if (KMessageBox::questionYesNo(0, i18n("Are you sure you want to remove "
		"all files in the selected tree from the project?")) ==
		KMessageBox::No) {
		return;
	}

	// Remove the files under the selected directory
	pItem = m_pFileList->firstChild();
	while (pItem != NULL) {
		pPrevItem = pItem;
		pItem = pItem->nextSibling();

		// Check if the file is under the selected directory
		sFilePath = pPrevItem->text(0);
		if (sFilePath.startsWith(sDir))
			removeItem(pPrevItem);
	}
}

/**
 * Filter files according to a pattern.
 * Hides all entries in the file list, except for those that match a given
 * pattern.
 */
void ProjectFilesDlg::slotFilter()
{
	QString sFilter;
	QListViewItem* pItem;
	
	// Get the user's filter string
	sFilter = m_pFilterEdit->text().stripWhiteSpace();
	if (sFilter.isEmpty())
		return;

	// Create the regular expression
	QRegExp reFilter(sFilter);
	reFilter.setWildcard(true);	
		
	// Iterate over the list entries, and hide all items not matching the
	// filter string
	pItem = m_pFileList->firstChild();
	while (pItem != NULL) {
		if (reFilter.search(pItem->text(0)) == -1) {
			pItem->setVisible(false);
			pItem->setSelectable(false);
		}

		pItem = pItem->nextSibling();
	}
}

/**
 * Shows all entries in the file list, after a filter has been applied.
 */
void ProjectFilesDlg::slotShowAll()
{
	QListViewItem* pItem;

	// Iterate over the list entries, and make all items visible
	pItem = m_pFileList->firstChild();
	while (pItem != NULL) {
		pItem->setVisible(true);
		pItem->setSelectable(true);
		pItem = pItem->nextSibling();
	}
}

/**
 * Stops a directory scan process.
 * This slot is called when the user clicks on the "Cancel" button in the
 * scan progress dialog.
 */
void ProjectFilesDlg::slotCancelDirScan()
{
	m_pScanner->cancel();
}

#include "projectfilesdlg.moc"
