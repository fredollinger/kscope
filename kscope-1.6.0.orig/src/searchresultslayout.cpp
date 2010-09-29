#include <klocale.h>
/****************************************************************************
** Form implementation generated from reading ui file './searchresultslayout.ui'
**
** Created: Thu Sep 9 13:23:06 2010
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "searchresultslayout.h"

#include <qvariant.h>
#include <qpushbutton.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qcombobox.h>
#include <qbuttongroup.h>
#include <qradiobutton.h>
#include <qcheckbox.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>

/*
 *  Constructs a SearchResultsLayout as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
SearchResultsLayout::SearchResultsLayout( QWidget* parent, const char* name, bool modal, WFlags fl )
    : QDialog( parent, name, modal, fl )
{
    if ( !name )
	setName( "SearchResultsLayout" );
    SearchResultsLayoutLayout = new QVBoxLayout( this, 11, 6, "SearchResultsLayoutLayout"); 

    layout2 = new QHBoxLayout( 0, 0, 6, "layout2"); 

    textLabel2 = new QLabel( this, "textLabel2" );
    layout2->addWidget( textLabel2 );

    m_pSearchEdit = new QLineEdit( this, "m_pSearchEdit" );
    layout2->addWidget( m_pSearchEdit );
    SearchResultsLayoutLayout->addLayout( layout2 );

    layout4 = new QHBoxLayout( 0, 0, 6, "layout4"); 

    textLabel1 = new QLabel( this, "textLabel1" );
    layout4->addWidget( textLabel1 );
    spacer2 = new QSpacerItem( 171, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout4->addItem( spacer2 );

    m_pColumnCB = new QComboBox( FALSE, this, "m_pColumnCB" );
    layout4->addWidget( m_pColumnCB );
    SearchResultsLayoutLayout->addLayout( layout4 );

    buttonGroup1 = new QButtonGroup( this, "buttonGroup1" );
    buttonGroup1->setColumnLayout(0, Qt::Vertical );
    buttonGroup1->layout()->setSpacing( 6 );
    buttonGroup1->layout()->setMargin( 11 );
    buttonGroup1Layout = new QVBoxLayout( buttonGroup1->layout() );
    buttonGroup1Layout->setAlignment( Qt::AlignTop );

    m_pTextRadio = new QRadioButton( buttonGroup1, "m_pTextRadio" );
    buttonGroup1Layout->addWidget( m_pTextRadio );

    m_pRegExpRadio = new QRadioButton( buttonGroup1, "m_pRegExpRadio" );
    buttonGroup1Layout->addWidget( m_pRegExpRadio );

    m_pSimpRegExpRadio = new QRadioButton( buttonGroup1, "m_pSimpRegExpRadio" );
    buttonGroup1Layout->addWidget( m_pSimpRegExpRadio );
    SearchResultsLayoutLayout->addWidget( buttonGroup1 );

    m_pCaseSenCheck = new QCheckBox( this, "m_pCaseSenCheck" );
    SearchResultsLayoutLayout->addWidget( m_pCaseSenCheck );

    m_pNegateCheck = new QCheckBox( this, "m_pNegateCheck" );
    SearchResultsLayoutLayout->addWidget( m_pNegateCheck );

    layout3 = new QHBoxLayout( 0, 0, 6, "layout3"); 
    spacer1 = new QSpacerItem( 201, 21, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout3->addItem( spacer1 );

    m_pOKButton = new QPushButton( this, "m_pOKButton" );
    layout3->addWidget( m_pOKButton );

    m_pCancelButton = new QPushButton( this, "m_pCancelButton" );
    layout3->addWidget( m_pCancelButton );
    SearchResultsLayoutLayout->addLayout( layout3 );
    languageChange();
    resize( QSize(361, 307).expandedTo(minimumSizeHint()) );
    clearWState( WState_Polished );

    // tab order
    setTabOrder( m_pSearchEdit, m_pColumnCB );
    setTabOrder( m_pColumnCB, m_pTextRadio );
    setTabOrder( m_pTextRadio, m_pRegExpRadio );
    setTabOrder( m_pRegExpRadio, m_pSimpRegExpRadio );
    setTabOrder( m_pSimpRegExpRadio, m_pCaseSenCheck );
    setTabOrder( m_pCaseSenCheck, m_pOKButton );
    setTabOrder( m_pOKButton, m_pCancelButton );
}

/*
 *  Destroys the object and frees any allocated resources
 */
SearchResultsLayout::~SearchResultsLayout()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void SearchResultsLayout::languageChange()
{
    setCaption( tr2i18n( "Filter Query Results" ) );
    textLabel2->setText( tr2i18n( "Search For:" ) );
    textLabel1->setText( tr2i18n( "Search In:" ) );
    m_pColumnCB->clear();
    m_pColumnCB->insertItem( tr2i18n( "Function" ) );
    m_pColumnCB->insertItem( tr2i18n( "File" ) );
    m_pColumnCB->insertItem( tr2i18n( "Line" ) );
    m_pColumnCB->insertItem( tr2i18n( "Text" ) );
    buttonGroup1->setTitle( tr2i18n( "Search Type" ) );
    m_pTextRadio->setText( tr2i18n( "Plain Text" ) );
    m_pRegExpRadio->setText( tr2i18n( "RegE&xp" ) );
    m_pSimpRegExpRadio->setText( tr2i18n( "Simplified RegExp" ) );
    m_pCaseSenCheck->setText( tr2i18n( "Case Sensitive" ) );
    m_pNegateCheck->setText( tr2i18n( "Negate Search" ) );
    m_pOKButton->setText( tr2i18n( "OK" ) );
    m_pCancelButton->setText( tr2i18n( "Cancel" ) );
}

#include "searchresultslayout.moc"
