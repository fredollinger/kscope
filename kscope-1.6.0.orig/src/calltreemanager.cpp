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

#include "calltreemanager.h"
#include "calltreedlg.h"
#include "projectmanager.h"

/**
 * Class constructor.
 * @param	pParent	The widget to use as the parent of all Call Tree 
  *					dialogues
 */
CallTreeManager::CallTreeManager(QWidget* pParent) : QObject(pParent)
{
	// Delete dialogue objects when they are removed from the list
	m_lstDialogs.setAutoDelete(true);
}

/**
 * Class destructor.
 */
CallTreeManager::~CallTreeManager()
{
}

/** 
 * Saves all call trees into the project directory.
 * @param	sProjPath	The project's directory
 * @param	slFiles		Holds a list of saved file names, upon return
 */
void CallTreeManager::saveOpenDialogs(const QString& sProjPath,
	QStringList& slFiles)
{
	CallTreeDlg *pDlg;
	
	// Iterate over the open dialogues
	for (pDlg = m_lstDialogs.first(); pDlg != NULL; 
		pDlg = m_lstDialogs.next()) {
		pDlg->store(sProjPath);
		slFiles += pDlg->getFileName();
	}
}

/** 
 * Loads all call trees according to the list of files 
 * @param	sProjPath	The project's directory
 * @param	slFiles		A list of file names to open
 */
void CallTreeManager::loadOpenDialogs(const QString& sProjPath,
	const QStringList& slFiles)
{
	QStringList::ConstIterator itr;
	CallTreeDlg *pDlg;
	
	for (itr = slFiles.begin(); itr != slFiles.end(); ++itr) {
		// Create a new dialogue for this file
		pDlg = addDialog();
		
		// Try to load the graph from the file
		if (!pDlg->load(sProjPath, *itr)) {
			m_lstDialogs.remove(pDlg);
			continue;
		}
		
		// Show the call tree
		pDlg->show();
	}
}

/** 
 * Creates a new Call Tree dialogue.
 * @return	The newly allocated dialogue object
 */
CallTreeDlg* CallTreeManager::addDialog() 
{
	CallTreeDlg* pDlg;
	
	// Create a modeless call tree dialogue
	pDlg = new CallTreeDlg();
	m_lstDialogs.append(pDlg); 
	
	// Open an editor whenever a function name is double-clicked
	connect(pDlg, SIGNAL(lineRequested(const QString&, uint)),
		this, SIGNAL(lineRequested(const QString&, uint)));

	// Track the closing of the call tree dialog
	connect(pDlg, SIGNAL(closed(const CallTreeDlg*)), this,
		SLOT(slotRemoveDialog(const CallTreeDlg*)));
	
	return pDlg;
}

/** 
 * Closes all Call Tree dialogues.
 */
void CallTreeManager::closeAll()
{
	m_lstDialogs.clear();
}

/** 
 * Removes a Call Tree dialogue from the list of open Call Trees.
 * This slot is connected to the closed() signal emitted by the dialogue.
 * @param	pDlg	The dialogue to remove from the list
 */
void CallTreeManager::slotRemoveDialog(const CallTreeDlg* pDlg) 
{
	m_lstDialogs.remove(pDlg);
}

#include "calltreemanager.moc"

