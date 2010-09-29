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

#ifndef QUERYPAGEBASE_H
#define QUERYPAGEBASE_H

#include <qhbox.h>

class QueryView;

/**
 * Defines a page in a QueryWidget's tab widget.
 * This is a abstract base class for QueryPage and HistoryPage. It defines
 * the common behaviour for all pages, which includes appearance, display
 * of tab text, page locking, storage and retrieval of information to
 * and from files and basic navigation.
 * Each page embeds a list widget derived from QueryView. The actual type
 * of widget is defined by the different page classes.
 * @author Elad Lahav
 */
class QueryPageBase : public QHBox
{
Q_OBJECT
public:
    QueryPageBase(QWidget* pParent = 0, const char* szName = 0);
    ~QueryPageBase();

	void applyPrefs();
	bool load(const QString&, const QString&);
	bool save(const QString&, QString&);
	void selectNext();
	void selectPrev();
	
	
	/**
	 * Determines whether this page can be locked.
	 * Can be used by inheriting classes to define non-lockable pages.
	 * @return	Always true
	 */
	virtual bool canLock() { return true; }
	
	/**
	 * Locks or unlocks this page.
	 * @param	bLocked	true to lock the page, false to unlock it.
	 */
	void setLocked(bool bLocked) { m_bLocked = bLocked; }
	
	/**
	 * Determines whether this page is locked.
	 * @return	true if the page is locked, false otherwise
	 */
	bool isLocked() { return m_bLocked; }
	
	/**
	 * Determines whether this page should be saved when the project is closed.
	 * By default, pages are saved if and only if they are locked.
	 * @return	true to save the page, false otherwise
	 */
	virtual bool shouldSave() const { return m_bLocked; };
	
	/**
	 * Constructs a caption for this page.
	 * The caption appears in the page's tab button and as the page's
	 * tooltip.
	 * @param	bBrief	true to generate a brief caption, false otherwise
	 * @return	The page's title
	 */
	virtual QString getCaption(bool bBrief = false) const = 0;
	
signals:
	/**
	 * Emitted when a record is selected in the view widget.
	 * @param	sFile	The "File" field of the selected record
	 * @param	nLine	The "Line" field of the selected record
	 */
	void lineRequested(const QString& sFile, uint nLine);

protected:
	/** The embedded list. */
	QueryView* m_pView;
	
	/** Indicates whether this page is locked. A locked page is never
		overriden by new data, and is also saved to a disc file when the
		session is closed. */
	bool m_bLocked;
	
	/**
	 * Creates a new list item and adds it to the embedded view.
	 * This method is used to add records read from a stored file.
	 * @param	sFile	The "File" field of the record
	 * @param	sFunc	The "Function" field of the record
	 * @param	sLine	The "Line" field of the record
	 * @param	sText	The "Text" field of the record
	 */
	virtual void addRecord(const QString& sFile, const QString& sFunc, 
		const QString& sLine, const QString& sText) = 0;
	
	/**
	 * Creates a file path to store this page.
	 * The path is composed of the project's path and a unique file name
	 * in that directory.
	 * @param	sProjPath	The project's directory
	 * @return	The page's file path
	 */
	virtual QString getFileName(const QString& sProjPath) const = 0;
	
	/**
	 * Tries to read the file header of a stored page.
	 * The contents of the header differ among inheriting classes.
	 * @param	str	A text stream initialised to the open page file
	 * @return	true if the header was read successfully and contains the
	 *			expected information, false otherwise
	 */
	virtual bool readHeader(QTextStream& str) = 0;
	
	/**
	 * Writes a header to a page's file.
	 * The contents of the header differ among inheriting classes.
	 * @param	str	A text stream initialised to the open page file
	 */
	virtual void writeHeader(QTextStream& str) = 0;
};

#endif
