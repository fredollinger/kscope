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

#include <qfileinfo.h>
#include <kmessagebox.h>
#include <klocale.h>
#include "ctagsfrontend.h"
#include "kscopeconfig.h"

/**
 * Class constructor.
 */
CtagsFrontend::CtagsFrontend() : Frontend(CTAGS_RECORD_SIZE)
{
}

/**
 * Class destructor.
 */
CtagsFrontend::~CtagsFrontend()
{
}

/**
 * Executes a Ctags process on a source file.
 * @param	sFileName	The full path to the source file
 * @return	true if successful, false otherwise
 */
bool CtagsFrontend::run(const QString& sFileName)
{
	QString sPath;
	QStringList slArgs;

	// Make sure the executable exists
	sPath = Config().getCtagsPath();

	// Set the command line arguments
	slArgs.append(sPath);
	slArgs.append("--excmd=n");
	slArgs.append("-u"); // don't sort
	slArgs.append("-f");
	slArgs.append("-");
	
	// The following regular expressions add support for goto labels and file
	// inclusions.
	// NOTE: We need to add the 'include' pattern to C++, since .h files are
	// treated as C++ rather than C. This also allows KScope some limited C++
	// support.
	slArgs.append("--regex-c=/^[ \\t]*([_a-zA-Z][_a-zA-Z0-9]*):/\\1/l,label/");
	slArgs.append("--regex-c=/^[ \\t]*#[ \\t]*include[ \\t]*[\\\"<]"
		"([_a-zA-Z0-9\\.\\/]*)[\\\">]/\\1/i,include/");
	slArgs.append("--regex-c++=/^[ \\t]*#[ \\t]*include[ \\t]*[\\\"<]"
		"([_a-zA-Z0-9\\.\\/]*)[\\\">]/\\1/i,include/");
	
	slArgs.append(sFileName);

	// Run a new process
	if (!Frontend::run("ctags", slArgs))
		return false;

	// Initialize stdout parsing
	m_state = Name;
	m_delim = Tab;

	return true;
}

/**
 * Tests that the given file path leads to an executable.
 * @param	sPath	The path to check
 * @return	true if the file in the given path exists and has executable
 *			permissions, false otherwise
 */
bool CtagsFrontend::verify(const QString& sPath)
{
	QFileInfo fi(sPath);

	if (!fi.exists() || !fi.isFile() || !fi.isExecutable() ||
		fi.fileName().find("ctags", 0, false) == -1) {
		KMessageBox::error(0, i18n("Ctags cannot be found in the given "
			"path"));
		return false;
	}

	return true;
}

/**
 * Parses the output of a Ctags process.
 * @param	sToken	The current token read (the token delimiter is determined
 *					by the current state)
 * @param	delim	The delimiter that ends this token
 * @return	A value indicating the way this token should be treated: dropped,
 *			added to the token queue, or finishes a new record
 */
Frontend::ParseResult CtagsFrontend::parseStdout(QString& sToken,
	ParserDelim delim)
{
	ParseResult result = DiscardToken;

	// Handle the token according to the current state
	switch (m_state) {
	case Name:
		if (sToken.left(6) == "ctags:") {
			m_state = Other;
			m_delim = Newline;
			break;
		}

		m_state = File;
		result = AcceptToken;
		break;

	case File:
		m_state = Line;
		result = DiscardToken;
		break;

	case Line:
		sToken = sToken.left(sToken.length() - 2);
		m_state = Type;
		m_delim = All;
		result = AcceptToken;
		break;
		
	case Type:
		if (delim == Newline) {
			m_state = Name;
			m_delim = Tab;
		}
		else {
			m_state = Other;
			m_delim = Newline;
		}
		
		result = RecordReady;
		break;

	case Other:
		m_state = Name;
		m_delim = Tab;
		result = DiscardToken;
		break;

	}

	return result;
}

#include "ctagsfrontend.moc"
