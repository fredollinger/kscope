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

#include <qpushbutton.h>
#include <qlineedit.h>
#include <qcombobox.h>
#include <qradiobutton.h>
#include "searchresultsdlg.h"

int SearchResultsDlg::s_nType = PlainText;
bool SearchResultsDlg::s_bCaseSensitive = true;
bool SearchResultsDlg::s_bNegate = false;

/**
 * Class constructor.
 * @param	pParent	The parent widget
 * @param	szName	Optional widget name
 */
SearchResultsDlg::SearchResultsDlg(QWidget* pParent, const char* szName) :
	SearchResultsLayout(pParent, szName, true, 0)
{
	// Select the last selected type radio button
	switch (s_nType) {
	case PlainText:
		m_pTextRadio->setChecked(true);
		break;
		
	case RegExp:
		m_pRegExpRadio->setChecked(true);
		break;
		
	case SimpRegExp:
		m_pSimpRegExpRadio->setChecked(true);
		break;
	}

	// Set the default value of the check-boxes
	m_pCaseSenCheck->setChecked(s_bCaseSensitive);
	m_pNegateCheck->setChecked(s_bNegate);
		
	// Terminate the dialogue when either the "OK" or "Cancel" buttons are
	// clicked
	connect(m_pOKButton, SIGNAL(clicked()), this, SLOT(accept()));
	connect(m_pCancelButton, SIGNAL(clicked()), this, SLOT(reject()));
}

/**
 * Class destructor.
 */
SearchResultsDlg::~SearchResultsDlg()
{
}

/**
 * Determines the default column on which to search.
 * The column's name appears in the column combo-box.
 */
void SearchResultsDlg::setColumn(int nCol)
{
	m_pColumnCB->setCurrentItem(nCol);
}

/**
 * @return	The selected column on which to perform the search
 */
int SearchResultsDlg::getColumn()
{
	return m_pColumnCB->currentItem();
}

/**
 * Creates a regular expression based on the given pattern and type of search.
 * @param	re	A regular expression object to set
 */
void SearchResultsDlg::getPattern(QRegExp& re)
{
	QString sPattern;
	
	sPattern = m_pSearchEdit->text();
	
	// Create the regular expression
	switch (s_nType) {
	case PlainText:
		re.setPattern(QRegExp::escape(sPattern));
		re.setWildcard(false);
		break;
		
	case RegExp:
		re.setPattern(sPattern);
		re.setWildcard(false);
		break;
	
	case SimpRegExp:
		re.setPattern(sPattern);
		re.setWildcard(true);
		break;
	}
	
	// Set the case-(in)sensitive parameter
	re.setCaseSensitive(s_bCaseSensitive);
}

/**
 * Reads user values from the widgets, and closes the dialogue.
 * This slot is connected to the clicked() signal emitted by the "OK" button.
 */
void SearchResultsDlg::accept()
{
	QString sText;
		
	// Determine the selected type and store its value for the next invocation
	if (m_pTextRadio->isChecked())
		s_nType = PlainText;
	else if (m_pRegExpRadio->isChecked())
		s_nType = RegExp;
	else if (m_pSimpRegExpRadio->isChecked())
		s_nType = SimpRegExp;
	
	// Determine search parameters
	s_bCaseSensitive = m_pCaseSenCheck->isChecked();
	s_bNegate = m_pNegateCheck->isChecked();
	
	// Remove white space from the search text
	sText = m_pSearchEdit->text();
	sText.stripWhiteSpace();
	if (sText.isEmpty()) {
		QDialog::reject();
		return;
	}

	// Close the dialogue
	QDialog::accept();
}

#include "searchresultsdlg.moc"

