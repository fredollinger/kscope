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

#ifndef DIRSCANNER_H
#define DIRSCANNER_H

#include <qobject.h>
#include <qevent.h>
#include <qthread.h>
#include <qdir.h>
#include <qstringlist.h>
#include <qdict.h>
#include <qlistview.h>

class DirScanner;

/**
 * Defines a new event that can be used to pass progress information from the
 * dir scanning thread to the main application thread.
 * @author Elad Lahav
 */
class DirScanEvent : public QCustomEvent
{
public:
	/** The event's unique ID. */
	enum { EventId = 6924 };
	
	DirScanEvent(int, bool);

	/** The number of files already scanned. */
	int m_nFiles;
	
	/** True if the dir scanning thread has finished, false otherwise. */
	bool m_bFinished;
};

/**
 * A set of unique strings.
 * Qt3 does not have a set class, so this is a simple implementation based on
 * a QDict of dummy int pointers.
 * @author	Elad Lahav
 */
class StringSet : public QDict<int>
{
public:
	StringSet() : QDict<int>() {}
	
	void insert(const QString& sItem) {
		static int nDummy = 0;
		QDict<int>::insert(sItem, &nDummy);
	}
	
	bool exists(const QString& sItem) {
		return find(sItem) != NULL;
	}
};

/**
 * Scans a directory for files matching a given pattern, using a separate thread.
 * @author Elad Lahav
 */
class DirScanner : public QThread
{
public: 
	DirScanner(QObject*, QDict<QListViewItem>*);
	~DirScanner();

	void start(const QString&, const QString&, bool);
	
	/**
	 * @return	The list of files scanned by this thread.
	 */
	const QStringList& getFiles() { return m_slFiles; }
	
	/**
	 * Stops a scanning process, by setting the object's cancel flag.
	 */
	void cancel() { m_bCancel = true; }
	
	/**
	 * @return	true if the user has cancelled the process, false otherwise
	 */
	bool wasCancelled() { return m_bCancel; }
	
protected:
	virtual void run();

private:
	/** Pointer to an object that receives the scanner update events. */
	QObject* m_pEventReceiver;
	
	/** Currently scanned directory. */
	QDir m_dir;
	
	/**
	 * A set of already-scanned directories (prevents infinite loops in case
	 * of cyclic symbolic links in the scanned file system).
	 */
	StringSet m_setScanned;
	
	/** Pointer to a list of files indexed by the file path (used to identify
		files that should not appear in the scan results.) */
	QDict<QListViewItem>* m_pDicFiles;
	
	/** Regular expression for scanning source files. */
	QString m_sNameFilter;
	
	/** The list of scanned file paths. */
	QStringList m_slFiles;
	
	/** A cancellation flag. Stops the scanning process when raised. */
	bool m_bCancel;
	
	/** true to descend to child directories, false otherwise. */
	bool m_bRecursive;
	
	int scanDir(QDir&);
};

#endif
