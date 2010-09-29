#include <klocale.h>
/****************************************************************************
** Form implementation generated from reading ui file './graphpreflayout.ui'
**
** Created: Thu Sep 9 13:23:05 2010
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "graphpreflayout.h"

#include <qvariant.h>
#include <qpushbutton.h>
#include <qlabel.h>
#include <qspinbox.h>
#include <qframe.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>

/*
 *  Constructs a GraphPrefLayout as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
GraphPrefLayout::GraphPrefLayout( QWidget* parent, const char* name, bool modal, WFlags fl )
    : QDialog( parent, name, modal, fl )
{
    if ( !name )
	setName( "GraphPrefLayout" );
    setSizeGripEnabled( TRUE );
    GraphPrefLayoutLayout = new QVBoxLayout( this, 11, 6, "GraphPrefLayoutLayout"); 

    layout6 = new QHBoxLayout( 0, 0, 6, "layout6"); 

    textLabel1 = new QLabel( this, "textLabel1" );
    layout6->addWidget( textLabel1 );
    spacer2 = new QSpacerItem( 81, 21, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout6->addItem( spacer2 );

    m_pMaxDegSpin = new QSpinBox( this, "m_pMaxDegSpin" );
    layout6->addWidget( m_pMaxDegSpin );
    GraphPrefLayoutLayout->addLayout( layout6 );

    layout7 = new QHBoxLayout( 0, 0, 6, "layout7"); 

    textLabel2 = new QLabel( this, "textLabel2" );
    layout7->addWidget( textLabel2 );
    spacer3 = new QSpacerItem( 131, 21, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout7->addItem( spacer3 );

    m_pColorButton = new QPushButton( this, "m_pColorButton" );
    layout7->addWidget( m_pColorButton );
    GraphPrefLayoutLayout->addLayout( layout7 );

    layout8 = new QHBoxLayout( 0, 0, 6, "layout8"); 

    textLabel3 = new QLabel( this, "textLabel3" );
    layout8->addWidget( textLabel3 );
    spacer4 = new QSpacerItem( 121, 31, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout8->addItem( spacer4 );

    m_pFontButton = new QPushButton( this, "m_pFontButton" );
    layout8->addWidget( m_pFontButton );
    GraphPrefLayoutLayout->addLayout( layout8 );

    line1 = new QFrame( this, "line1" );
    line1->setFrameShape( QFrame::HLine );
    line1->setFrameShadow( QFrame::Sunken );
    line1->setFrameShape( QFrame::HLine );
    GraphPrefLayoutLayout->addWidget( line1 );

    Layout1 = new QHBoxLayout( 0, 0, 6, "Layout1"); 
    Horizontal_Spacing2 = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    Layout1->addItem( Horizontal_Spacing2 );

    buttonOk = new QPushButton( this, "buttonOk" );
    buttonOk->setAutoDefault( TRUE );
    buttonOk->setDefault( TRUE );
    Layout1->addWidget( buttonOk );

    buttonCancel = new QPushButton( this, "buttonCancel" );
    buttonCancel->setAutoDefault( TRUE );
    Layout1->addWidget( buttonCancel );
    GraphPrefLayoutLayout->addLayout( Layout1 );
    languageChange();
    resize( QSize(328, 164).expandedTo(minimumSizeHint()) );
    clearWState( WState_Polished );

    // signals and slots connections
    connect( buttonOk, SIGNAL( clicked() ), this, SLOT( accept() ) );
    connect( buttonCancel, SIGNAL( clicked() ), this, SLOT( reject() ) );
    connect( m_pColorButton, SIGNAL( clicked() ), this, SLOT( slotColorClicked() ) );
    connect( m_pFontButton, SIGNAL( clicked() ), this, SLOT( slotFontClicked() ) );
}

/*
 *  Destroys the object and frees any allocated resources
 */
GraphPrefLayout::~GraphPrefLayout()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void GraphPrefLayout::languageChange()
{
    setCaption( tr2i18n( "Call Graph Preferences" ) );
    textLabel1->setText( tr2i18n( "Maximal In/Out Node Degree" ) );
    textLabel2->setText( tr2i18n( "Colours" ) );
    m_pColorButton->setText( tr2i18n( "..." ) );
    textLabel3->setText( tr2i18n( "Fonts" ) );
    m_pFontButton->setText( tr2i18n( "..." ) );
    buttonOk->setText( tr2i18n( "&OK" ) );
    buttonOk->setAccel( QKeySequence( QString::null ) );
    buttonCancel->setText( tr2i18n( "&Cancel" ) );
    buttonCancel->setAccel( QKeySequence( QString::null ) );
}

void GraphPrefLayout::slotColorClicked()
{
    qWarning( "GraphPrefLayout::slotColorClicked(): Not implemented yet" );
}

void GraphPrefLayout::slotFontClicked()
{
    qWarning( "GraphPrefLayout::slotFontClicked(): Not implemented yet" );
}

#include "graphpreflayout.moc"
