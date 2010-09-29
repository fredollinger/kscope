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

#ifndef PROJECTBASE_H
#define PROJECTBASE_H

#include <qstringlist.h>
#include <qdir.h>
#include <qfile.h>
#include <kconfig.h>

#define DEF_IS_KERNEL		false
#define DEF_INV_INDEX		true
#define DEF_NO_COMPRESS		false
#define DEF_SLOW_PATH		false
#define DEF_AC_MIN_CHARS	3
#define DEF_AC_DELAY		500
#define DEF_AC_MAX_ENTRIES	100
#define DEF_TAB_WIDTH		0 /* Use editor's default */

/**
 * Abstract base class for classes that need the list of project files.
 * Objects of classes derived from this one are used as a parameter to
 * ProjectManager::fillList(), which reads all file entries in the project,
 * and calls addItem() for each.
 * Any class that wishes to retrieve the project's file list, should inherit
 * from this class, and implement addItem().
 * @author Elad Lahav
 */

class FileListTarget
{
public:
	/**
	 * Class constructor.
	 */
	FileListTarget() {}

	/**
	 * Class destructor.
	 */
	virtual ~FileListTarget() {}

	/**
	 * Appends a file to the list.
	 * @param	sFilePath	The full path of the file to add
	 */
	virtual void addItem(const QString& sFilePath) = 0;
};

/**
 * Abstract base class for classes that need the list of project files.
 * Objects of classes derived from this one are used as a parameter to
 * ProjectManager::writeList(), which calls getFirstItem() and getNextItem(),
 * and writes the returned values to the project's 'cscope.files' file.
 * Any class that wishes to retrieve the project's file list, should inherit
 * from this class, and implement firstItem() and nextItem().
 * @author Elad Lahav
 */

class FileListSource
{
public:
	/**
	 * Class constructor.
	 */
	FileListSource() {}

	/**
	 * Class destructor.
	 */
	virtual ~FileListSource() {}

	/**
	 * Returns the first file in the list, and initiates a new iteration.
	 * @param	sFilePath	Holds the path of the first file, upon return
	 * @return	true if there are more files, false otherwise
	 */
	virtual bool firstItem(QString& sFilePath) = 0;

	/**
	 * Returns the next file in the list.
	 * @param	sFilePath	Holds the path of the file, upon return
	 * @return	true if there are more files, false otherwise
	 */
	virtual bool nextItem(QString& sFilePath) = 0;
};

/**
 * Defines a cursor location inside a file.
 * This structure is used to store project session information.
 * @author Elad Lahav
 */
struct FileLocation
{
	/**
	 * Struct constructor.
	 * @param	sPath	The full path of the file
	 * @param	nLine	The line position of the cursor
	 * @param	nCol	The column position of the cursor
	 */
	FileLocation(QString sPath, uint nLine, uint nCol) : m_sPath(sPath),
		m_nLine(nLine), m_nCol(nCol) {}
		
	/** The full path of the file. */
	QString m_sPath;
	
	/** The line position of the cursor. */
	uint m_nLine;
	
	/** The column position of the cursor. */
	uint m_nCol;
};

/**
 * A list of file locations used for restoring a session.
 */
typedef QPtrList<FileLocation> FileLocationList;

class FileSemaphore;

/**
 * @author Elad Lahav
 */
class ProjectBase
{
public:
	ProjectBase();
	virtual ~ProjectBase();

	/**
	 * Configurable project options.
	 */
	struct Options {
		QString sSrcRootPath;
		
		/** A list of MIME-types that determines which files are included in
			the project. */
		QStringList slFileTypes;
		
		/** true if the -k option for CScope should be used. */
		bool bKernel;
		
		/** true if Cscope should build an inverted index. */
		bool bInvIndex;
		
		/** true if the -c option for CScope should be used. */
		bool bNoCompress;
		
		/** true if the -D option for CScope should be used. */
		bool bSlowPathDef;
		
		/** The time, in milliseconds, after which the database should be
			automatically rebuilt (-1 if this option is disabled). */
		int nAutoRebuildTime;
		
		/** true to use auto-completion. */
		bool bACEnabled;
		
		/** Minimum number of characters in a symbol for auto-completion. */
		uint nACMinChars;
		
		/** Time interval, in milliseconds, before auto-completion is
			started. */
		uint nACDelay;
		
		/** Maximal number of entries for auto-completion. */
		uint nACMaxEntries;
		
		uint nTabWidth;
	};
	
	virtual bool open(const QString&);
	virtual bool loadFileList(FileListTarget*);
	virtual bool storeFileList(FileListSource*) { return false; }
	virtual bool isEmpty() { return false; }
	bool dbExists();
	virtual void close() {}
	
	virtual QString getFileTypes() const { return QString::null; }
	virtual void getOptions(Options&) const;
	virtual void setOptions(const Options&) {}
	virtual void getSymHistory(QStringList&) const {}
	virtual void setSymHistory(QStringList&) {}
	virtual void getMakeParams(QString&, QString&) const;

	/**
	 * Determines whether a project is based on a Cscope.out file, and is
	 * therefore considered as a temporary project.
	 * @return	true if this is a temporary project, false otherwise
	 */
	virtual bool isTemporary() { return true; }

	/**
	 * @return	The name of the current project
	 */
	QString getName() const { return m_sName; }
	
	/**
	 * @return	The full path of the project's directory
	 */
	QString getPath() const { return m_dir.absPath(); }
	
	/**
	 * @return	Command-line arguments to pass to a Cscope object, based on
	 * 			project's options
	 */
	uint getArgs() const { return m_nArgs; }
	
	const QString& getSourceRoot() const { return m_opt.sSrcRootPath; }
	
	/**
	 * @return	The time, in seconds, to wait before rebuilding the
	 *			cross-refernce database.
	 */
	int getAutoRebuildTime() const { return m_opt.nAutoRebuildTime; }
	
	/**
	 * @return	The tab width to use (0 to use the editor's default)
	 */
	uint getTabWidth() const { return m_opt.nTabWidth; }
	
	static void getDefOptions(Options&);
	
protected:
	/** The name of the project, as written in the configuration file */
	QString m_sName;

	/** The directory associated with the project */
	QDir m_dir;
	
	/** A cached version of the project's options. */
	Options m_opt;
	
	/** A list of Cscope command-line arguments based on the project's
		options. */
	uint m_nArgs;
	
	/** A list of symbols previously queried. */
	QStringList m_slSymHistory;
	
	void initOptions();
	
	static bool isCscopeOut(const QString&);
};

#endif
