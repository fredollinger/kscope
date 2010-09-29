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

#ifndef CONFIGFRONTEND_H
#define CONFIGFRONTEND_H

#include <frontend.h>

/**
 * Frontend to the kscope_config shell script.
 * The script executes a set of tests and outputs their results.
 * @author Elad Lahav
 */
class ConfigFrontend : public Frontend
{
	Q_OBJECT

public:
	ConfigFrontend(bool bAutoDelete = false);
	~ConfigFrontend();
	
	bool run(const QString&, const QString&, const QString&, 
		bool bCscopeOptsOnly = false);
	
	/**
	 * The types of tests executed by the script.
	 */
	enum { CscopePath, CscopeVersion, CscopeVerbose, CscopeSlowPath,
		CtagsPath, CtagsExub, DotPath, DotPlain, END };
	
signals:
	/**
	 * Indicates that the script is now running a given test.
	 * @param	nType	The type of test being executed
	 */
	void test(uint nType);
	
	/**
	 * Called after a test has produced a result.
	 * @param	nType	The type of test executed
	 * @param	sResult	The obtained result
	 */
	void result(uint nType, const QString& sResult);
	
protected:
	virtual ParseResult parseStdout(QString&, ParserDelim);
	
private:
	/** The type of test whose result is expected next. */
	uint m_nNextResult;
};

#endif
