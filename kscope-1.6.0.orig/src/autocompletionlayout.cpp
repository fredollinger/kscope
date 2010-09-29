#include <klocale.h>
/****************************************************************************
** Form implementation generated from reading ui file './autocompletionlayout.ui'
**
** Created: Thu Sep 9 13:23:05 2010
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "autocompletionlayout.h"

#include <qvariant.h>
#include <qpushbutton.h>
#include <qlabel.h>
#include <qspinbox.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>

/*
 *  Constructs a AutoCompletionLayout as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
AutoCompletionLayout::AutoCompletionLayout( QWidget* parent, const char* name, bool modal, WFlags fl )
    : QDialog( parent, name, modal, fl )
{
    if ( !name )
	setName( "AutoCompletionLayout" );
    AutoCompletionLayoutLayout = new QVBoxLayout( this, 11, 6, "AutoCompletionLayoutLayout"); 

    layout20 = new QHBoxLayout( 0, 0, 6, "layout20"); 

    textLabel1 = new QLabel( this, "textLabel1" );
    layout20->addWidget( textLabel1 );
    spacer15 = new QSpacerItem( 71, 21, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout20->addItem( spacer15 );

    m_pMinCharsSpin = new QSpinBox( this, "m_pMinCharsSpin" );
    layout20->addWidget( m_pMinCharsSpin );
    AutoCompletionLayoutLayout->addLayout( layout20 );

    layout21 = new QHBoxLayout( 0, 0, 6, "layout21"); 

    textLabel2 = new QLabel( this, "textLabel2" );
    layout21->addWidget( textLabel2 );
    spacer16 = new QSpacerItem( 101, 21, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout21->addItem( spacer16 );

    m_pDelaySpin = new QSpinBox( this, "m_pDelaySpin" );
    m_pDelaySpin->setMaxValue( 10000 );
    m_pDelaySpin->setLineStep( 100 );
    layout21->addWidget( m_pDelaySpin );
    AutoCompletionLayoutLayout->addLayout( layout21 );

    layout22 = new QHBoxLayout( 0, 0, 6, "layout22"); 

    textLabel3 = new QLabel( this, "textLabel3" );
    layout22->addWidget( textLabel3 );
    spacer17 = new QSpacerItem( 81, 21, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout22->addItem( spacer17 );

    m_pMaxEntriesSpin = new QSpinBox( this, "m_pMaxEntriesSpin" );
    m_pMaxEntriesSpin->setMaxValue( 1000 );
    m_pMaxEntriesSpin->setMinValue( 1 );
    layout22->addWidget( m_pMaxEntriesSpin );
    AutoCompletionLayoutLayout->addLayout( layout22 );
    spacer19 = new QSpacerItem( 20, 31, QSizePolicy::Minimum, QSizePolicy::Expanding );
    AutoCompletionLayoutLayout->addItem( spacer19 );

    layout23 = new QHBoxLayout( 0, 0, 6, "layout23"); 
    spacer18 = new QSpacerItem( 111, 21, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout23->addItem( spacer18 );

    m_pOKButton = new QPushButton( this, "m_pOKButton" );
    layout23->addWidget( m_pOKButton );

    m_pCancelButton = new QPushButton( this, "m_pCancelButton" );
    layout23->addWidget( m_pCancelButton );
    AutoCompletionLayoutLayout->addLayout( layout23 );
    languageChange();
    resize( QSize(287, 183).expandedTo(minimumSizeHint()) );
    clearWState( WState_Polished );
}

/*
 *  Destroys the object and frees any allocated resources
 */
AutoCompletionLayout::~AutoCompletionLayout()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void AutoCompletionLayout::languageChange()
{
    setCaption( tr2i18n( "Auto-Completion Properties" ) );
    textLabel1->setText( tr2i18n( "Minimum Characters" ) );
    textLabel2->setText( tr2i18n( "Delay (ms)" ) );
    textLabel3->setText( tr2i18n( "Maximum Entries" ) );
    m_pOKButton->setText( tr2i18n( "OK" ) );
    m_pCancelButton->setText( tr2i18n( "Cancel" ) );
}

#include "autocompletionlayout.moc"
