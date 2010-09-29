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

#include <qcheckbox.h>
#include <qradiobutton.h>
#include <qlineedit.h>
#include <qlabel.h>
#include "prefopt.h"
#include "kscopeconfig.h"

/**
 * Class constructor.
 * @param	pParent	The parent widget
 * @param	szName	The widget's name
 */
PrefOpt::PrefOpt(QWidget* pParent, const char* szName)
	: PrefOptLayout(pParent, szName)
{
	// Set initial values
	load();
		
	// Emit the "modified" signal whenever any of the widgets changes its
	// its. This will notify the parent dialogue to enable its "Apply"
	// button
	connect(m_pReadOnlyCheck, SIGNAL(toggled(bool)), this, 
		SIGNAL(modified()));
	connect(m_pLastProjCheck, SIGNAL(toggled(bool)), this, 
		SIGNAL(modified()));
	connect(m_pTagHlCheck, SIGNAL(toggled(bool)), this, 
		SIGNAL(modified()));
	connect(m_pBriefQueryCaptCheck, SIGNAL(toggled(bool)), this, 
		SIGNAL(modified()));
	connect(m_pWarnModifiedOnDiskCheck, SIGNAL(toggled(bool)), this, 
		SIGNAL(modified()));
	connect(m_pAutoSortCheck, SIGNAL(toggled(bool)), this, 
		SIGNAL(modified()));
	connect(m_pExtEditorEdit, SIGNAL(textChanged(const QString&)), this,
		SIGNAL(modified()));
	connect(m_pSysProfileCB, SIGNAL(activated(int)), this,
		SIGNAL(modified()));
	connect(m_pEditorPopupCB, SIGNAL(activated(int)), this,
		SIGNAL(modified()));
}

/**
 * Class destructor.
 */
PrefOpt::~PrefOpt()
{
}

/**
 * Reads the current settings from the configuration object, and applies them
 * the the page's widget.
 */
void PrefOpt::load()
{
	m_pReadOnlyCheck->setChecked(Config().getReadOnlyMode());
	m_pLastProjCheck->setChecked(Config().getLoadLastProj());
	m_pTagHlCheck->setChecked(Config().getAutoTagHl());
	m_pBriefQueryCaptCheck->setChecked(Config().getUseBriefQueryCaptions());
	m_pWarnModifiedOnDiskCheck->setChecked(Config().getWarnModifiedOnDisk());
	m_pAutoSortCheck->setChecked(Config().getAutoSortFiles());
	m_pExtEditorEdit->setText(Config().getExtEditor());
	
	switch (Config().getSysProfile()) {
	case KScopeConfig::Fast:
		m_pSysProfileCB->setCurrentItem(0);
		break;
		
	case KScopeConfig::Slow:
		m_pSysProfileCB->setCurrentItem(1);
		break;
	}
	
	switch (Config().getEditorPopup()) {
	case KScopeConfig::Embedded:
		m_pEditorPopupCB->setCurrentItem(0);
		break;
		
	case KScopeConfig::KScopeOnly:
		m_pEditorPopupCB->setCurrentItem(1);
		break;
	}		
}

/**
 * Commits settings changes to the configuration object.
 */
void PrefOpt::apply()
{
	Config().setReadOnlyMode(m_pReadOnlyCheck->isChecked());
	Config().setLoadLastProj(m_pLastProjCheck->isChecked());
	Config().setAutoTagHl(m_pTagHlCheck->isChecked());
	Config().setUseBriefQueryCaptions(m_pBriefQueryCaptCheck->isChecked());
	Config().setWarnModifiedOnDisk(m_pWarnModifiedOnDiskCheck->isChecked());
	Config().setAutoSortFiles(m_pAutoSortCheck->isChecked());
	Config().setExtEditor(m_pExtEditorEdit->text());
	
	switch (m_pSysProfileCB->currentItem()) {
	case 0 :
		Config().setSysProfile(KScopeConfig::Fast);
		break;
		 
	case 1:
		Config().setSysProfile(KScopeConfig::Slow);
		break;
	}
	
	switch (m_pEditorPopupCB->currentItem()) {
	case 0:
		Config().setEditorPopup(KScopeConfig::Embedded);
		break;
		
	case 1:
		Config().setEditorPopup(KScopeConfig::KScopeOnly);
		break;
	}
}

#include "prefopt.moc"
