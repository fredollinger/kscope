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

#ifndef PROJECTFILESDLG_H
#define PROJECTFILESDLG_H

#include <qwidget.h>
#include <projectfileslayout.h>
#include <qdict.h>
#include "project.h"

class DirScanner;
class ScanProgressDlg;

/**
 * A dialog to manipulate the project's files.
 * The dialog allows the user to add source files to the current project, or
 * remove files from it. The main widget of the dialog is a list view, that
 * displays all files currently in the project. When files are added or
 * removed, this list view is updated. The project, however, is only modified
 * if the user closes the dialog using the "OK" button.
 * Since searches through a list view are very slow, the class also maintains
 * a QDict object, that connects file names with their respective list items.
 * This dictionary is used to ensure duplicated items are not added to the
 * list.
 * @author Elad Lahav
 */

class ProjectFilesDlg : public ProjectFilesLayout, public FileListTarget,
	public FileListSource
{
   Q_OBJECT

public: 
	ProjectFilesDlg(Project*, QWidget* pParent = 0,	const char* szName = 0);
	~ProjectFilesDlg();

	virtual void addItem(const QString&);
	virtual bool firstItem(QString&);
	virtual bool nextItem(QString&);

protected:
	virtual void customEvent(QCustomEvent*);

private:
	/** The project to manipulate. */
	Project* m_pProj;

	/** Holds all file paths in a quickly searchable format (for duplicate
		entries lookup). */
	QDict<QListViewItem> m_dicFiles;
	
	/** A thread object to a-synchronously scan directories for source files
		to add to the project. */
	DirScanner* m_pScanner;

	/** Displays the progress of a directory scan operation. */
	ScanProgressDlg* m_pScanDlg;

	/** A file list item that serves as an iterator. */
	QListViewItem* m_pItrItem;

	/** The last item added. */
	QListViewItem* m_pLastItem;
	
	void removeItem(QListViewItem*);
	
private slots:
	void slotAddFiles();
	void slotAddDir();
	void slotAddTree();
	void slotRemSel();
	void slotRemDir();
	void slotRemTree();
	void slotFilter();
	void slotShowAll();
	void slotCancelDirScan();
};

#endif
