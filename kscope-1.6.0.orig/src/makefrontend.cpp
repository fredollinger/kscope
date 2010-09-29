/***************************************************************************
 *
 * Copyright (C) 2006 Elad Lahav (elad_lahav@users.sourceforge.net)
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

#include <qregexp.h>
#include "makefrontend.h"

// TODO:
// This should probably be configurable on a per-project basis.
#define PATH_ELEM "[a-zA-Z0-9_\\.\\-]+"

#define RE_FILE_LINE \
	"((?:\\/)?(?:"PATH_ELEM"\\/)*"PATH_ELEM"):([0-9]+)(:[0-9]+)?: (.*)"
#define RE_ENTER_DIR \
	"Entering directory " \
	"\\`((\\/)?("PATH_ELEM"\\/)*"PATH_ELEM")"
#define RE_EXIT_DIR "Leaving directory"

/**
 * Class constructor.
 * @param	bAutoDelete	If true, the object is deleted when the process
 *						terminates (false by default)
 */
MakeFrontend::MakeFrontend(bool bAutoDelete) : Frontend(1, bAutoDelete)
{
	// Execute inside a shell
	setUseShell(true);
	
	// Each token represent a complete line
	m_delim = Newline;
}

/**
 * Class destructor.
 */
MakeFrontend::~MakeFrontend()
{
}

/**
 * Executes the make command.
 * @param	sName		The name of the process (for error messages)
 * @param	slArgs		A list containing the command-line arguments
 * @param	sWorkDir	Initial build directory
 * @param	bBlock		(Optional) true to block, false otherwise
 * @return	true if the process was executed successfully, false otherwise
 */
bool MakeFrontend::run(const QString& sName, const QStringList& slArgs, 
	const QString& sWorkDir, bool bBlock)
{
	QStringList slShellArgs;
	
	// Store the current build directory
	m_slPathStack.push_back(sWorkDir);
	
	// Join the output streams, so that they can both be parsed by
	// parseStdout()
	slShellArgs = slArgs;
	slShellArgs << "2>&1";
	
	// Execute the command
	return Frontend::run(sName, slShellArgs, sWorkDir, bBlock);
}

/**
 * Parses lines of output produced by the make command.
 * @param	sToken	A single line of output
 */
Frontend::ParseResult MakeFrontend::parseStdout(QString& sToken, ParserDelim)
{
	static QRegExp reErrWarn(RE_FILE_LINE);
	static QRegExp reEntDir(RE_ENTER_DIR);
	static QRegExp reExtDir(RE_EXIT_DIR);
	QString sRep;
	int nPos;
	QString sFile, sLine, sText;
	
	if ((nPos = reErrWarn.search(sToken)) >= 0) {
		// An error/warning message
		if (sToken.at(nPos) == '/') {
			sFile = reErrWarn.capturedTexts()[1];
		}
		else {
			sFile = m_slPathStack.last() + "/" +
				reErrWarn.capturedTexts()[1];
		}
		
		sLine = reErrWarn.capturedTexts()[2];
		sText = reErrWarn.capturedTexts()[4];
		emit error(sFile, sLine, sText);
		
		sRep = QString("<a href=\"") + sFile + "&\\2\">\\1:\\2</a>\\3: \\4";
		sToken.replace(reErrWarn, sRep);
	}
	else if ((nPos = reEntDir.search(sToken)) >= 0) {
		// Recursing into a directory
		m_slPathStack.push_back(reEntDir.capturedTexts()[1]);
		sToken = QString("<b>Entering directory</b> ") +
			m_slPathStack.last();
	}
	else if ((nPos = reExtDir.search(sToken)) >= 0) {
		// Leaving a directory
		sToken = QString("<b>Leaving directory</b> ") +
			m_slPathStack.last();
		m_slPathStack.pop_back();
	}

	return RecordReady;
}

#include "makefrontend.moc"
