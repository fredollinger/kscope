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

#ifndef PROJECT_H
#define PROJECT_H

#include <projectbase.h>

/**
 * @author Elad Lahav
 */
class Project : public ProjectBase
{
public:
	Project();
	virtual ~Project();
	
	struct Session {
		FileLocationList fllOpenFiles;
		QString sLastFile;
		QStringList slQueryFiles;
		QStringList slCallTreeFiles;
		FileLocationList fllBookmarks;
		QString sMakeCmd;
		QString sMakeRoot;
	};
	
	virtual bool open(const QString&);
	virtual bool loadFileList(FileListTarget*);
	virtual bool storeFileList(FileListSource*);
	virtual bool addFile(const QString&);
	virtual bool isEmpty();
	virtual void close();
	
	virtual QString getFileTypes() const;
	virtual void getOptions(Options&) const;
	virtual void setOptions(const Options&);
	virtual void loadSession(Session&);
	virtual void storeSession(const Session&);
	virtual void getSymHistory(QStringList&) const;
	virtual void setSymHistory(QStringList&);
	virtual void getMakeParams(QString&, QString&) const;

	/**
	 * Determines whether a project is based on a Cscope.out file, and is
	 * therefore considered as a temporary project.
	 * @return	true if this is a temporary project, false otherwise
	 */
	virtual bool isTemporary() { return false; }
	
	static bool create(const QString&, const QString&, const Options&);
	
private:
	/** The configuration file ("cscope.proj") */
	KConfig* m_pConf;

	/** The file that holds the paths of all source files in this project
	("cscope.files") */
	QFile m_fiFileList;
	
	QString m_sMakeCmd;
	
	QString m_sMakeRoot;
	
	static void writeOptions(KConfig*, const Options&);
};

#endif
