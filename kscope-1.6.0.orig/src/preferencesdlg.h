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

#ifndef PREFERENCESDLG_H
#define PREFERENCESDLG_H

#include <qwidget.h>
#include <kdialogbase.h>

class PrefFrontend;
class PrefColor;
class PrefFont;
class PrefOpt;

/**
 * The main configuration dialog for KScope.
 * This dialog displays a set of configuration pages, each dedicated to a
 * different subject (frontend programme paths, colours, etc.)
 * This code is based on a tutorial by Andreas Nicolai which can be found at
 * http://www.kdevelop.org/doc/tutorial_settings
 * @author Elad Lahav
 */

class PreferencesDlg : public KDialogBase
{
   Q_OBJECT

public: 
	PreferencesDlg(uint nPage = Frontend, QWidget* pParent = 0, const char*
		szName = 0);
	~PreferencesDlg();

	/** Available pages. */
	enum { Frontend = 0, Colors, Fonts, Options };

signals:
	/**
	 * Emitted when the global configuration changes as a result of using
	 * this dialogue.
	 */
	void applyPref();

protected slots:
	virtual void accept();
	virtual void slotApply();
	virtual void slotDefault();
	
private:
	/** The front-end programmes page. */
	PrefFrontend* m_pPrefFrontend;
	
	/** The colours preference page. */
	PrefColor* m_pPrefColor;
	
	/** The fonts preference page. */
	PrefFont* m_pPrefFont;
	
	/** The additional options page. */
	PrefOpt* m_pPrefOpt;
	
	void loadConfig();
	bool updateConfig();
	bool verifyPaths();

private slots:
	void slotModified();
};

#endif
