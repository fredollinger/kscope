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

#ifndef KSCOPEPIXMAPS_H
#define KSCOPEPIXMAPS_H

#include <qpixmap.h>
#include <kiconloader.h>

#define GET_PIXMAP(_pix) \
	Pixmaps().getPixmap(KScopePixmaps::_pix)

/**
 * Handles all pixmaps required by KScope.
 * There are two types of pixmaps: embedded, i.e., pixmaps whose pixels are
 * given by static two-dimensional arrays, and loadable, which are retrieved
 * through the KIconLoader mechanism.
 * The application uses a single global instance of this class.
 * @author Elad Lahav
 */

class KScopePixmaps
{
public: 
	KScopePixmaps();
	~KScopePixmaps();

	/** Identifiers for embedded pixmaps. */
	enum PixName { SymFunc, SymVar, SymStruct, SymMacro, SymMember, SymEnum,
		SymEnumerator, SymTypedef, SymLabel, SymInclude, SymUnknown,
		PIX_ARRAY_SIZE };
		
	/** Identifiers for loadable pixmaps. */
	enum LoadPixName { TabUnlocked, TabLocked, TabBookmark, TabRW, TabRO, 
		TabSave, TabFileList, TabFileTree, TabList, ButtonSaveAs, ButtonZoomIn,
		ButtonZoomOut, ButtonRotate, ButtonPref, CalledTree,
		CallingTree, CallGraph };
	
	void init();
	const QPixmap& getPixmap(PixName name) const;
	QPixmap getPixmap(LoadPixName name);
	
private:
	/** An array of pointers to the embedded pixmaps. */
	QPixmap** m_pPixArray;
	
	/** An icon loader used to retrieve pixmaps through the KDE mechanism. */
	KIconLoader m_loader;
};

extern KScopePixmaps& Pixmaps();

#endif
