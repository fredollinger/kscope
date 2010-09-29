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

#include <qspinbox.h>
#include "graphprefdlg.h"
#include "preferencesdlg.h"
#include "kscopeconfig.h"

/**
 * Class constructor.
 * @param	pParent	The parent widget
 * @param	szName	The widget's name
 */
GraphPrefDlg::GraphPrefDlg(QWidget* pParent, const char* szName) :
	GraphPrefLayout(pParent, szName, true, 0)
{
	m_pMaxDegSpin->setValue(Config().getGraphMaxNodeDegree());
}

/**
 * Class destructor.
 */
GraphPrefDlg::~GraphPrefDlg()
{
}

/**
 * @return	The maximal degree value set in the spin box
 */
int GraphPrefDlg::getMaxNodeDegree()
{
	return m_pMaxDegSpin->value();
}

/**
 * Displays the general preferences dialogue, showing the "Colours" page.
 * This slot is connected to the clicked() signal of the colours button.
 */
void GraphPrefDlg::slotFontClicked()
{
	PreferencesDlg dlg(PreferencesDlg::Fonts);
	
	dlg.exec();
}

/**
 * Displays the general preferences dialogue, showing the "Fonts" page.
 * This slot is connected to the clicked() signal of the fonts button.
 */
void GraphPrefDlg::slotColorClicked()
{
	PreferencesDlg dlg(PreferencesDlg::Colors);
	
	dlg.exec();
}

#include "graphprefdlg.moc"

