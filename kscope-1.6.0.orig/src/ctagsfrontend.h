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

#ifndef CTAGSFRONTEND_H
#define CTAGSFRONTEND_H

#include <frontend.h>

#define CTAGS_RECORD_SIZE 3

/**
 * Controls a Ctags process for an file in an EditorPage window.
 * A new Ctags process is run each time the file in the editor window is
 * loaded (including the initial load, and any subsequent ones which follow a
 * 'save' operation.)
 * The output of the process is parsed into a set of records, each composed of
 * the following fields:
 * - Tag type
 * - Tag name
 * - Line number
 * The records are then displayed in the CtagsList widget that is attached to
 * each EditorPage window.
 * @author Elad Lahav
 */

class CtagsFrontend : public Frontend
{
   Q_OBJECT

public:
	CtagsFrontend();
	~CtagsFrontend();

	bool run(const QString&);
	
	static bool verify(const QString&);

protected:
	virtual ParseResult parseStdout(QString&, ParserDelim);

private:
	/** State values for the parser state machine. */
	enum ParserState { Name = 0, File, Line, Type, Other };

	/** The current state of the parser state machine. */
	ParserState m_state;
};

#endif
