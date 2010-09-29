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

#ifndef KSCOPEACTIONS_H
#define KSCOPEACTIONS_H

#include <kaction.h>

class KScope;

typedef QPtrList<KAction> ActionList;

/**
 * A helper class for managing KScope's menu commands.
 * @author	Elad Lahav
 */
class KScopeActions : public QObject
{
	Q_OBJECT
	
public:
	KScopeActions(KScope*);
	~KScopeActions();
	
	void init();
	void initPopups();
	void initLayoutActions();
	void enableExtEditor(bool);
	
	KToggleAction* getLockAction() { return m_pLockAction; }
	
public slots:
	void slotQueryDockToggled(bool);
	void slotQueryDockClosed();
	void slotFileViewDockClosed();
	void slotEnableProjectActions(bool);
	void slotEnableFileActions(bool);
	
signals:
	void toggleProject(bool bEnable);
	void toggleFile(bool bEnable);
	
private:
	KScope* m_pWindow;
	KActionCollection* m_pCollection;
	
	/** A list of actions that require an active project. */
	ActionList m_lstProjActions;
	
	/** A list of actions that require an active file. */
	ActionList m_lstFileActions;
	
	/** A toggle menu item for locking/unlocking query pages. */
	KToggleAction* m_pLockAction;

	/** The "Edit in External Editor" menu command. */
	KAction* m_pExtEditAction;
	
	/** The "Show/Hide File View" menu command. */
	KToggleAction* m_pToggleFileViewAction;
	
	/** The "Show/Hide Query Window" menu command. */
	KToggleAction* m_pToggleQueryWindowAction;
	
	/** The "Show/Hide Tag List" menu command. */
	KToggleAction* m_pToggleTagListAction;
	
	KAction* addAction(const QString&, const char*, const char*, QWidget*,
		const char*, const char*, const char*);
	KToggleAction* addToggle(const QString&, const char*, const char*,
		QWidget*, const char*, const char*, const char*);
};

#endif
