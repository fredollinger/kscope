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

#include <kglobal.h>
#include <kiconloader.h>
#include "kscopepixmaps.h"

static const char* XPM_FUNC[] = {
	"12 12 2 1",
	". c #000000",
	"# c #58a8ff",
	"............",
	".##########.",
	".###....###.",
	".###.######.",
	".###.######.",
	".###.######.",
	".###....###.",
	".###.######.",
	".###.######.",
	".###.######.",
	".##########.",
	"............"
};

static const char* XPM_VAR[] = {
	"12 12 3 1",
	". c #000000",
	"a c #c00000",
	"# c #ff0000",
	"............",
	".##########.",
	".##########.",
	".##.####.##.",
	".##.####.##.",
	".##.a##a.##.",
	".##a.##.a##.",
	".###.aa.###.",
	".###a..a###.",
	".####..####.",
	".##########.",
	"............"
};

static const char* XPM_STRUCT[] = {
	"12 12 2 1",
	". c #000000",
	"# c #ffff00",
	"............",
	".##########.",
	".####...###.",
	".###.###.##.",
	".###.######.",
	".####.#####.",
	".#####.####.",
	".######.###.",
	".##.###.###.",
	".###...####.",
	".##########.",
	"............"
};

static const char* XPM_MACRO[] = {
	"12 12 2 1",
	". c #000000",
	"# c #00c000",
	"............",
	".##########.",
	".##.####.##.",
	".##..##..##.",
	".##.#..#.##.",
	".##.####.##.",
	".##.####.##.",
	".##.####.##.",
	".##.####.##.",
	".##.####.##.",
	".##########.",
	"............"
};

static const char* XPM_MEMBER[] = {
	"12 12 3 1",
	". c #000000",
	"a c #0000c0",
	"# c #c0c0ff",
	"............",
	".##########.",
	".##########.",
	".##########.",
	".##a.##.a##.",
	".##.a..a.##.",
	".##.#aa#.##.",
	".##.####.##.",
	".##.####.##.",
	".##########.",
	".##########.",
	"............"
};

static const char* XPM_ENUM[] = {
	"12 12 2 1",
	". c #000000",
	"# c #ff00ff",
	"............",
	".##########.",
	".##########.",
	".##......##.",
	".##.#######.",
	".##.#######.",
	".##.....###.",
	".##.#######.",
	".##.#######.",
	".##......##.",
	".##########.",
	"............"
};

static const char* XPM_ENUMERATOR[] = {
	"12 12 2 1",
	". c #000000",
	"# c #ffc0c0",
	"............",
	".##########.",
	".##########.",
	".###...####.",
	".##.###.###.",
	".##.###.###.",
	".##.....###.",
	".##.#######.",
	".##.###.###.",
	".###...####.",
	".##########.",
	"............"
};

static const char* XPM_TYPEDEF[] = {
	"12 12 2 1",
	". c #000000",
	"# c #c0ffc0",
	"............",
	".##########.",
	".#.......##.",
	".####.#####.",
	".####.#####.",
	".####.#####.",
	".####.#####.",
	".####.#####.",
	".####.#####.",
	".####.#####.",
	".##########.",
	"............"
};

static const char* XPM_LABEL[] = {
	"12 12 2 1",
	". c #000000",
	"# c #c0ff00",
	"............",
	".##########.",
	".#.########.",
	".#.########.",
	".#.########.",
	".#.########.",
	".#.########.",
	".#.########.",
	".#.########.",
	".#.......##.",
	".##########.",
	"............"
};

static const char* XPM_INCLUDE[] = {
	"12 12 2 1",
	". c #000000",
	"# c #c0c0c0",
	"............",
	".##########.",
	".##.....###.",
	".####.#####.",
	".####.#####.",
	".####.#####.",
	".####.#####.",
	".####.#####.",
	".####.#####.",
	".##.....###.",
	".##########.",
	"............"
};

static const char* XPM_UNKNOWN[] = {
	"12 12 2 1",
	". c #000000",
	"# c #ffffff",
	"............",
	".##########.",
	".##.....###.",
	".#.#####.##.",
	".########.#.",
	".########.#.",
	".#######.##.",
	".######.###.",
	".####.#####.",
	".##########.",
	".####.#####.",
	"............"
};

/**
 * Class constructor.
 */
KScopePixmaps::KScopePixmaps() :
	m_pPixArray(NULL),
	m_loader()
{
}

/**
 * Class destructor.
 */
KScopePixmaps::~KScopePixmaps()
{
	int i;
	
	for (i = 0; i < PIX_ARRAY_SIZE; i++)
		delete m_pPixArray[i];
		
	delete [] m_pPixArray;
}

/**
 * Creates the array of embedded pixmaps.
 * This function is separated from the constructor since QPixmap objects 
 * cannot be created at the time the static KScopePixmaps object is 
 * allocated.
 */
void KScopePixmaps::init()
{
	// Create the pixmap array
	m_pPixArray = new QPixmap * [PIX_ARRAY_SIZE];

	// Create all pixmaps
	m_pPixArray[SymFunc] = new QPixmap(XPM_FUNC);
	m_pPixArray[SymVar] = new QPixmap(XPM_VAR);
	m_pPixArray[SymStruct] = new QPixmap(XPM_STRUCT);
	m_pPixArray[SymMacro] = new QPixmap(XPM_MACRO);
	m_pPixArray[SymMember] = new QPixmap(XPM_MEMBER);
	m_pPixArray[SymEnum] = new QPixmap(XPM_ENUM);
	m_pPixArray[SymEnumerator] = new QPixmap(XPM_ENUMERATOR);
	m_pPixArray[SymTypedef] = new QPixmap(XPM_TYPEDEF);
	m_pPixArray[SymLabel] = new QPixmap(XPM_LABEL);
	m_pPixArray[SymInclude] = new QPixmap(XPM_INCLUDE);
	m_pPixArray[SymUnknown] = new QPixmap(XPM_UNKNOWN);
}

/**
 * Returns a reference to an embedded pixmap.
 * @param	name	The pixmap's identifier
 * @return	A reference to the requested pixmap
 */
const QPixmap& KScopePixmaps::getPixmap(PixName name) const
{
	return *m_pPixArray[name];
}

/**
 * Loads a pixmap with the KIconLoader mechanism.
 * @param	name	The pixmap's identifier
 * @return	The requested pixmap
 */
QPixmap KScopePixmaps::getPixmap(LoadPixName name)
{
	switch (name) {
	case TabUnlocked:
		return m_loader.loadIcon("query_unlocked", KIcon::Small, 0,
			false);

	case TabLocked:
		return m_loader.loadIcon("query_locked", KIcon::Small, 0,
			false);

	case TabBookmark:
		return m_loader.loadIcon("bookmark", KIcon::Small, 0,
			false);

	case TabRW:
		return m_loader.loadIcon("file_rw", KIcon::Small, 0,
			false);

	case TabRO:
		return m_loader.loadIcon("file_ro", KIcon::Small, 0,
			false);

	case TabSave:
		return m_loader.loadIcon("file_save", KIcon::Small, 0,
			false);
	
	case TabFileList:
		return m_loader.loadIcon("view_detailed", KIcon::Small, 0,
			false);

	case TabFileTree:
		return m_loader.loadIcon("view_tree", KIcon::Small, 0,
			false);
			
	case TabList:
		return m_loader.loadIcon("tab_list", KIcon::Small, 0,
			false);
		
	case ButtonSaveAs:
		return m_loader.loadIcon("filesaveas", KIcon::Toolbar, 
			0, false);
		
	case ButtonZoomIn:
		return m_loader.loadIcon("viewmag+", KIcon::Toolbar, 
			0, false);
		
	case ButtonZoomOut:
		return m_loader.loadIcon("viewmag-", KIcon::Toolbar, 
			0, false);
		
	case ButtonRotate:
		return m_loader.loadIcon("rotate", KIcon::Toolbar, 
			0, false);
		
	case ButtonPref:
		return m_loader.loadIcon("configure", KIcon::Toolbar, 
			0, false);
	
	case CalledTree:
		return m_loader.loadIcon("called_tree", KIcon::Toolbar, 
			0, false);
	
	case CallingTree:
		return m_loader.loadIcon("calling_tree", KIcon::Toolbar, 
			0, false);
			
	case CallGraph:
		return m_loader.loadIcon("call_graph", KIcon::Toolbar, 
			0, false);
	}

	return QPixmap();
}

/**
 * @return	A reference to a global KScopePixmaps object
 */
KScopePixmaps& Pixmaps()
{
	static KScopePixmaps pix;
	return pix;
}
