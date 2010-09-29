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

#ifndef FRONTEND_H
#define FRONTEND_H

#include <qobject.h>
#include <kprocess.h>


/**
 * Represents a single token in the parsed output stream.
 * @author Elad Lahav
 */

class FrontendToken
{
public:
	/**
	 * Class constructor.
	 */
	FrontendToken() : m_pNext(NULL) {}

	/**
	 * @return	The text associated with this token
	 */
	const QString& getData() const { return m_sData; }
	
	/**
	 * @return	A pointer to the next token in the strem.
	 */
	FrontendToken* getNext() const { return m_pNext; }

protected:
	/** Free text associated with the token. */
	QString m_sData;
	
	/** A pointer to the next token in the stream. */
	FrontendToken* m_pNext;

	friend class Frontend;
};
 
/**
 * Abstract base class that provides a front-end to console-based programmes.
 * Provides a parsing infrastructure which is based on a list of records, all
 * of the same structure. Each record is composed of a number of delimited
 * fields (tokens.)
 * @author Elad Lahav
 */

class Frontend : public KProcess
{
	Q_OBJECT
	
public: 
	Frontend(uint, bool bAutoDelete = false);
	~Frontend();

	virtual bool run(const QString&, const QStringList&, 
		const QString& sWorkDir = "", bool bBlock = false);
	void kill();
		
	/**
	 * @return	An string describing the error which made run() fail
	 */
	const QString& getRunError() { return m_sError; }
	
signals:
	/**
	 * Indicates tokens can be read.
	 * The Frontend object parses the back-end output and creates a list of
	 * tokens. This signal is emitted when a batch of characters has been
	 * converted into a token list.
	 * @param	pToken	The head of the token list
	 */
	void dataReady(FrontendToken* pToken);
	
	/**
	 * Emitted when the back-end process terminates.
	 * @param	nRecords	The number of complete records parsed
	 */
	void finished(uint nRecords);
	
	/**
	 * Indicates that the Cscope process was terminated.
	 */
	void aborted();
	
	/**
	 * This signal is used to report the progress of the back-end process.
	 * @param	nProgress		The current progress value
	 * @param	nTotal			The progress value that indicates the process
	 *							is finished
	 */
	void progress(int nProgress, int nTotal);
	
	/**
	 * Emitted when an error message is produced by the back-end process.
	 */
	void error(const QString& sMsg);
	
protected:
	/** A set of possible delimiters for parsing process output. */
	enum ParserDelim { Newline = 0x01, Space = 0x02, Tab = 0x04,
		WSpace = Space | Tab, All = WSpace | Newline };

	/** Defines the set of return values for parseStdout(). Determines what
		needs to be done with a new token passed to this method. */
	enum ParseResult {
		DiscardToken	/** Delete this token */,
		AcceptToken		/** Add this token to the list */,
		RecordReady		/** This token completes a record */,
		Abort			/** Kill the process */
	};

	/** Number of complete records read so far. */
	uint m_nRecords;	
	
	/** The head of the list of parsed output tokens. */
	FrontendToken* m_pHeadToken;

	/** The tail of the list of parsed output tokens. */
	FrontendToken* m_pTailToken;

	/** An iterator on the list of parsed output tokens. */
	FrontendToken* m_pCurToken;

	/** The current delimiters used for parsing the output. */
	ParserDelim m_delim;
	
	/** An error string produced if run() fails. */
	QString m_sError;
	
	/**
	 * Handles a text token received on the Standard Output stream of the
	 * controlled process.
	 * This is called by slotReadStdout whenever a new token is recognised.
	 * Inheriting classes should implement this method to parse the resutling
	 * stream of tokens.
	 * @param	sToken	A part of the text received on the Standard Output,
	 *					disected according to current delimiter settings
	 * @param	delim	The delimiter that ended this token
	 * @result	A ParseResult value, indicating what should be done with the
	 *			new token
	 */
	virtual ParseResult parseStdout(QString& sToken, ParserDelim delim) = 0;

	virtual void parseStderr(const QString&);
	
	/**
	 * Called when the process exits.
	 * Allows inheriting classes to implement process termination handlers.
	 */
	virtual void finalize() {}

protected slots:
	virtual void slotProcessExit(KProcess*);
	
private:
	/** Determines whether the object should be deleted once the process has
		exited */
	bool m_bAutoDelete;

	/** Determines whether the parser is in the middle of a token, or between
		two tokens */
	bool m_bInToken;

	/** The number of fields in each parsed record. Should be defined for 
		every sub-class. */
	uint m_nRecordSize;
	
	/** This flag is raised when kill() is called. It signifies that even
		though the process may not be dead yet, it should be considered as
		such. */
	bool m_bKilled;
	
	void addToken(FrontendToken*);
	void removeToken();
	void removeRecord();
	bool tokenize(char**, int*, QString&, ParserDelim&);
		
private slots:
	void slotReadStdout(KProcess*, char*, int);
	void slotReadStderr(KProcess*, char*, int);
};

#endif
