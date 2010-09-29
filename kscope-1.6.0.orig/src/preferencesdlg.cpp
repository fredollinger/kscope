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

#include <qlayout.h>
#include <klocale.h>
#include <kiconloader.h>
#include <kurlrequester.h>
#include <klineedit.h>
#include <qcheckbox.h>
#include <kcolorbutton.h>
#include <kmessagebox.h>
#include <kfontrequester.h>
#include "preferencesdlg.h"
#include "preffrontend.h"
#include "prefcolor.h"
#include "preffont.h"
#include "prefopt.h"
#include "kscopeconfig.h"
#include "cscopefrontend.h"
#include "ctagsfrontend.h"
#include "dotfrontend.h"


/**
 * Class constructor.
 * @param	nPage	The initial page to show
 * @param	pParent	The parent widget
 * @param	szName	The widget's name
 */
PreferencesDlg::PreferencesDlg(uint nPage, QWidget* pParent, 
	const char* szName) :
	KDialogBase(IconList, i18n("Preferences"), Default | Ok | Apply | Cancel,
	Ok, pParent, szName, 0)
{
	QFrame* pFrame;
	QVBoxLayout* pLayout;

	// Create and add the "Frontend" page
	pFrame = addPage(i18n("Programmes"),
		i18n("Paths to back-end programmes"),
		KGlobal::iconLoader()->loadIcon("run", KIcon::Panel, 0, false));
	pLayout = new QVBoxLayout(pFrame, 0, 0);
	m_pPrefFrontend = new PrefFrontend(pFrame);
	pLayout->addWidget(m_pPrefFrontend);
	
	// Create and add the "Colours" page
	pFrame = addPage(i18n("Colours"), i18n("Window colours"),
		KGlobal::iconLoader()->loadIcon("colors", KIcon::Panel, 0, false));
	pLayout = new QVBoxLayout(pFrame, 0, 0);
	m_pPrefColor = new PrefColor(pFrame);
	pLayout->addWidget(m_pPrefColor);
	
	// Create and add the "Fonts" page
	pFrame = addPage(i18n("Fonts"), i18n("Window fonts"),
		KGlobal::iconLoader()->loadIcon("fonts", KIcon::Panel, 0, false));
	pLayout = new QVBoxLayout(pFrame, 0, 0);
	m_pPrefFont = new PrefFont(pFrame);
	pLayout->addWidget(m_pPrefFont);
	
	// Create and add the "Options" page
	pFrame = addPage(i18n("Options"), i18n("Misc. Options"),
		KGlobal::iconLoader()->loadIcon("package_settings", 
		KIcon::Panel, 0, false));
	pLayout = new QVBoxLayout(pFrame, 0, 0);
	m_pPrefOpt = new PrefOpt(pFrame);
	pLayout->addWidget(m_pPrefOpt);

	// Make sure the "Apply" button is initially disabled
	enableButtonApply(false);
		
	// Enable the "Apply" button when a parameter changes its value
	connect(m_pPrefFrontend, SIGNAL(modified()), this, 
		SLOT(slotModified()));
	connect(m_pPrefColor, SIGNAL(modified()), this, SLOT(slotModified()));
	connect(m_pPrefFont, SIGNAL(modified()), this, SLOT(slotModified()));
	connect(m_pPrefOpt, SIGNAL(modified()), this, SLOT(slotModified()));
	
	// Set the active page
	showPage(nPage);
}

/**
 * Class destructor.
 */
PreferencesDlg::~PreferencesDlg()
{
}

/**
 * Updates the dialog's widgets with the current configuration parameters.
 */
void PreferencesDlg::loadConfig()
{
	m_pPrefFrontend->load();
	m_pPrefColor->load();
	m_pPrefFont->load();
	m_pPrefOpt->load();
}

/**
 * Sets the configured parameters to the global configuration object.
 * This method is called when either the "OK" or the "Apply" button are
 * clicked. Before the new settings are applied, their values are verified.
 * @return	true if the new parameters were applied successfully, false
 *			otherwise
 */
bool PreferencesDlg::updateConfig()
{
	// Verify configured paths lead to the executables
	if (!verifyPaths())
		return false;

	// Apply the changes
	m_pPrefFrontend->apply();
	m_pPrefColor->apply();
	m_pPrefFont->apply();
	m_pPrefOpt->apply();
	
	emit applyPref();
	return true;
}

/**
 * Tests whether the paths set for Cscope and Ctags lead to executables.
 * Cscope is verified by a different process.
 */
bool PreferencesDlg::verifyPaths()
{
	return (CtagsFrontend::verify(m_pPrefFrontend->m_pCtagsURL->url()) &&
		DotFrontend::verify(m_pPrefFrontend->m_pDotURL->url()));
}

/**
 * Updates the global configuration based on the values given in the 
 * preferences dialogue, and then closes the dialogue.
 * This function is called after the user clicks the dialogue's "OK" button.
 */
void PreferencesDlg::accept()
{
	if (updateConfig())
		KDialogBase::accept();
}

/**
 * Updates the global configuration based on the values given in the 
 * preferences dialogue, leaving the dialogue open.
 * This function is called after the user clicks the dialogue's "Apply" 
 * button.
 */
void PreferencesDlg::slotApply()
{
	if (updateConfig())
		enableButtonApply(false);
}

/**
 * Resets all configuration parameters to their default values.
 * This slot is called when the user clicks the "Default" button.
 */
void PreferencesDlg::slotDefault()
{
	// Prompt the user before applying default values
	if (KMessageBox::questionYesNo(0, i18n("This would reset all your "
		"configuration settings! Continue?")) == KMessageBox::Yes) {
		// Load the default values
		Config().loadDefault();
		loadConfig();

		// Apply the default values
		slotApply();
	}
}

/**
 * Enables the "Apply" button.
 */
void PreferencesDlg::slotModified()
{
	enableButtonApply(true);
}

#include "preferencesdlg.moc"
