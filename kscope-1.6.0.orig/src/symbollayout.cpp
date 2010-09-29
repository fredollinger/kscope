#include <klocale.h>
/****************************************************************************
** Form implementation generated from reading ui file './symbollayout.ui'
**
** Created: Thu Sep 9 13:23:06 2010
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "symbollayout.h"

#include <qvariant.h>
#include <qpushbutton.h>
#include <qlabel.h>
#include <qcombobox.h>
#include <kcombobox.h>
#include <qcheckbox.h>
#include <qframe.h>
#include <qheader.h>
#include <qlistview.h>
#include <qbuttongroup.h>
#include <qradiobutton.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>

/*
 *  Constructs a SymbolLayout as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
SymbolLayout::SymbolLayout( QWidget* parent, const char* name, bool modal, WFlags fl )
    : QDialog( parent, name, modal, fl )
{
    if ( !name )
	setName( "SymbolLayout" );
    SymbolLayoutLayout = new QVBoxLayout( this, 11, 6, "SymbolLayoutLayout"); 

    layout15 = new QHBoxLayout( 0, 0, 6, "layout15"); 

    layout14 = new QVBoxLayout( 0, 0, 6, "layout14"); 

    textLabel1 = new QLabel( this, "textLabel1" );
    textLabel1->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)5, (QSizePolicy::SizeType)5, 0, 0, textLabel1->sizePolicy().hasHeightForWidth() ) );
    layout14->addWidget( textLabel1 );

    textLabel2 = new QLabel( this, "textLabel2" );
    textLabel2->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)5, (QSizePolicy::SizeType)5, 0, 0, textLabel2->sizePolicy().hasHeightForWidth() ) );
    layout14->addWidget( textLabel2 );
    layout15->addLayout( layout14 );

    layout13 = new QVBoxLayout( 0, 0, 6, "layout13"); 

    m_pTypeCombo = new QComboBox( FALSE, this, "m_pTypeCombo" );
    m_pTypeCombo->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)7, (QSizePolicy::SizeType)0, 0, 0, m_pTypeCombo->sizePolicy().hasHeightForWidth() ) );
    layout13->addWidget( m_pTypeCombo );

    m_pSymbolHC = new KHistoryCombo( this, "m_pSymbolHC" );
    m_pSymbolHC->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)7, (QSizePolicy::SizeType)0, 0, 0, m_pSymbolHC->sizePolicy().hasHeightForWidth() ) );
    m_pSymbolHC->setInsertionPolicy( KHistoryCombo::AtTop );
    m_pSymbolHC->setDuplicatesEnabled( FALSE );
    layout13->addWidget( m_pSymbolHC );
    layout15->addLayout( layout13 );
    SymbolLayoutLayout->addLayout( layout15 );

    m_pSubStringCheck = new QCheckBox( this, "m_pSubStringCheck" );
    SymbolLayoutLayout->addWidget( m_pSubStringCheck );

    line2 = new QFrame( this, "line2" );
    line2->setFrameShape( QFrame::HLine );
    line2->setFrameShadow( QFrame::Sunken );
    line2->setFrameShape( QFrame::HLine );
    SymbolLayoutLayout->addWidget( line2 );

    layout3 = new QHBoxLayout( 0, 0, 6, "layout3"); 
    spacer2 = new QSpacerItem( 71, 21, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout3->addItem( spacer2 );

    m_pOKButton = new QPushButton( this, "m_pOKButton" );
    layout3->addWidget( m_pOKButton );

    m_pHintButton = new QPushButton( this, "m_pHintButton" );
    layout3->addWidget( m_pHintButton );

    m_pCancelButton = new QPushButton( this, "m_pCancelButton" );
    layout3->addWidget( m_pCancelButton );
    SymbolLayoutLayout->addLayout( layout3 );

    m_pHintList = new QListView( this, "m_pHintList" );
    SymbolLayoutLayout->addWidget( m_pHintList );

    m_pHintGroup = new QButtonGroup( this, "m_pHintGroup" );
    m_pHintGroup->setColumnLayout(0, Qt::Vertical );
    m_pHintGroup->layout()->setSpacing( 6 );
    m_pHintGroup->layout()->setMargin( 11 );
    m_pHintGroupLayout = new QVBoxLayout( m_pHintGroup->layout() );
    m_pHintGroupLayout->setAlignment( Qt::AlignTop );

    m_pBeginWithRadio = new QRadioButton( m_pHintGroup, "m_pBeginWithRadio" );
    m_pHintGroupLayout->addWidget( m_pBeginWithRadio );

    m_pContainRadio = new QRadioButton( m_pHintGroup, "m_pContainRadio" );
    m_pHintGroupLayout->addWidget( m_pContainRadio );
    SymbolLayoutLayout->addWidget( m_pHintGroup );
    languageChange();
    resize( QSize(343, 420).expandedTo(minimumSizeHint()) );
    clearWState( WState_Polished );

    // tab order
    setTabOrder( m_pSymbolHC, m_pTypeCombo );
    setTabOrder( m_pTypeCombo, m_pSubStringCheck );
    setTabOrder( m_pSubStringCheck, m_pOKButton );
    setTabOrder( m_pOKButton, m_pHintButton );
    setTabOrder( m_pHintButton, m_pCancelButton );
    setTabOrder( m_pCancelButton, m_pHintList );
    setTabOrder( m_pHintList, m_pBeginWithRadio );
    setTabOrder( m_pBeginWithRadio, m_pContainRadio );
}

/*
 *  Destroys the object and frees any allocated resources
 */
SymbolLayout::~SymbolLayout()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void SymbolLayout::languageChange()
{
    setCaption( tr2i18n( "KScope Query" ) );
    textLabel1->setText( tr2i18n( "Type" ) );
    textLabel2->setText( tr2i18n( "Symbol" ) );
    m_pTypeCombo->clear();
    m_pTypeCombo->insertItem( tr2i18n( "References to" ) );
    m_pTypeCombo->insertItem( tr2i18n( "Definition of" ) );
    m_pTypeCombo->insertItem( tr2i18n( "Functions called by" ) );
    m_pTypeCombo->insertItem( tr2i18n( "Functions calling" ) );
    m_pTypeCombo->insertItem( tr2i18n( "Find text" ) );
    m_pTypeCombo->insertItem( tr2i18n( "Find EGrep pattern" ) );
    m_pTypeCombo->insertItem( tr2i18n( "Find file" ) );
    m_pTypeCombo->insertItem( tr2i18n( "Files #including" ) );
    m_pTypeCombo->insertItem( tr2i18n( "Call graph for" ) );
    m_pSubStringCheck->setText( tr2i18n( "Search for &a Sub-String" ) );
    m_pSubStringCheck->setAccel( QKeySequence( tr2i18n( "Alt+A" ) ) );
    m_pOKButton->setText( tr2i18n( "OK" ) );
    m_pHintButton->setText( tr2i18n( "Hi&nt" ) );
    m_pCancelButton->setText( tr2i18n( "Cancel" ) );
    m_pHintGroup->setTitle( tr2i18n( "Hint Options" ) );
    m_pBeginWithRadio->setText( tr2i18n( "S&ymbols Beginning With..." ) );
    m_pContainRadio->setText( tr2i18n( "Sym&bols Containing..." ) );
}

#include "symbollayout.moc"
