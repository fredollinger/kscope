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

#include <ktexteditor/editorchooser.h>
#include <kate/document.h>
#include "editormanager.h"
#include "kscopeconfig.h"

/**
 * Class constructor.
 * @param	pParent	The parent widget
 * @param	szName	The widget's name
 */
EditorManager::EditorManager(QWidget* pParent, const char* szName) :
	KParts::PartManager(pParent, szName)
{
	applyPrefs();
}

/**
 * Class destructor.
 */
EditorManager::~EditorManager()
{
}

/**
 * Creates a new document part.
 * @return	A pointer to the new document
 */
KTextEditor::Document* EditorManager::add()
{
	KTextEditor::Document* pDoc;
	
	// Create the document
	pDoc = KTextEditor::EditorChooser::createDocument(this);
	addPart(pDoc);

	return pDoc;
}

/**
 * Deletes a document part.
 * @param	pDoc	The document to remove
 */
void EditorManager::remove(KTextEditor::Document* pDoc)
{
	removePart(pDoc);
	delete pDoc;
}

/**
 * Applies the user preferences.
 * Determines if Kate warnings are displayed in case the currently edited
 * file is modified outside KScope.
 * NOTE: This behaviour is determined by a static function, which is why this
 * code appears here, rather then for every EditorPage object.
 */
void EditorManager::applyPrefs()
{
	Kate::Document::setFileChangedDialogsActivated(
		Config().getWarnModifiedOnDisk());
}

#include "editormanager.moc"
