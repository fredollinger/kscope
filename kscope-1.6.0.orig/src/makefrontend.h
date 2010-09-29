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

#ifndef MAKEFRONTEND_H
#define MAKEFRONTEND_H

#include <frontend.h>

/**
 * A shell-process front-end intended for running make-like tasks.
 * Records are single-line tokens delimited by newline characters. The parser
 * replaces references to source lines (e.g., filename:123) with hypertext
 * links for use in a browser.
 * @author Elad Lahav
 */
class MakeFrontend : public Frontend
{
	Q_OBJECT
	
public:
	MakeFrontend(bool bAutoDelete = false);
	~MakeFrontend();

	virtual bool run(const QString&, const QStringList&, 
		const QString&, bool bBlock = false);
	virtual ParseResult parseStdout(QString&, ParserDelim);
	
signals:
	void error(const QString& sFile, const QString& sLine,
		const QString& sText);
	
private:
	/** A stack of paths used to track the current build directory. */
	QStringList m_slPathStack;
};

#endif
