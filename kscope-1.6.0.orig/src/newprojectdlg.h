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

#ifndef NEWPROJECTDLG_H
#define NEWPROJECTDLG_H

#include <qlineedit.h>
#include <qcheckbox.h>
#include <newprojectlayout.h>
#include <autocompletionlayout.h>
#include "projectbase.h"
	
/**
 * A sub-dialogue of the New Project dialogue.
 * Allows the user to configure auto-completion parameters.
 * @author Elad Lahav
 */
class AutoCompletionDlg : public AutoCompletionLayout
{
	Q_OBJECT
	
public:
	AutoCompletionDlg(QWidget* pParent, const char* szName = NULL);
	~AutoCompletionDlg();

public slots:		
	int exec();
	
protected slots:
	virtual void accept();

private:
	/** The minimum number of characters in a symbol required for automatic
		completion. */
	uint m_nMinChars;
	
	/** The time, in seconds, to wait before automatic completion is
		attempted. */
	uint m_nDelay;
	
	/** The maximal number of results. */
	uint m_nMaxEntries;

	friend class NewProjectDlg;
};

/**
 * A dialog for creating new projects.
 * Prompts the user for the project's name, the directory for Cscope's files,
 * the types of files included in the project and several options.
 * Can also be used to change some of the properties of a project after it
 * has been created.
 * @author Elad Lahav
 */

class NewProjectDlg : public NewProjectLayout
{
	Q_OBJECT
	
public: 
	NewProjectDlg(bool, QWidget* pParent = NULL, const char* szName = NULL);
	~NewProjectDlg();
	
	void setProperties(const QString&, const QString&, 
		const ProjectBase::Options&);

	QString getName();
	QString getPath();
	void getOptions(ProjectBase::Options&);
	
protected slots:
	virtual void accept();
	virtual void slotAddType();
	virtual void slotRemoveType();
	virtual void slotAvailTypesChanged(const QString&);

private:
	/** The file MIME-types associated with the new project. */
	QStringList m_slTypes;
	
	/** A sub-dialogue for configuring symbol auto-completion parameters. */
	AutoCompletionDlg* m_pAutoCompDlg;
	
	/** Whether the dialogue represents a new or existing project. */
	bool m_bNewProj;
};

#endif
