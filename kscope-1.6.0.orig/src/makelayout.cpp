#include <klocale.h>
/****************************************************************************
** Form implementation generated from reading ui file './makelayout.ui'
**
** Created: Thu Sep 9 13:23:06 2010
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "makelayout.h"

#include <qvariant.h>
#include <qpushbutton.h>
#include <qlabel.h>
#include <kcombobox.h>
#include <kurlrequester.h>
#include <qtabwidget.h>
#include <ktextbrowser.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include "queryview.h"

/*
 *  Constructs a MakeLayout as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 */
MakeLayout::MakeLayout( QWidget* parent, const char* name, WFlags fl )
    : QWidget( parent, name, fl )
{
    if ( !name )
	setName( "MakeLayout" );
    MakeLayoutLayout = new QVBoxLayout( this, 11, 6, "MakeLayoutLayout"); 

    layout10 = new QGridLayout( 0, 1, 1, 0, 6, "layout10"); 

    textLabel1 = new QLabel( this, "textLabel1" );

    layout10->addWidget( textLabel1, 0, 0 );

    m_pCommandHistory = new KHistoryCombo( this, "m_pCommandHistory" );
    m_pCommandHistory->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)7, (QSizePolicy::SizeType)0, 0, 0, m_pCommandHistory->sizePolicy().hasHeightForWidth() ) );

    layout10->addWidget( m_pCommandHistory, 1, 1 );

    m_pRootURL = new KURLRequester( this, "m_pRootURL" );

    layout10->addWidget( m_pRootURL, 0, 1 );

    textLabel2 = new QLabel( this, "textLabel2" );

    layout10->addWidget( textLabel2, 1, 0 );
    MakeLayoutLayout->addLayout( layout10 );

    tabWidget2 = new QTabWidget( this, "tabWidget2" );

    tab = new QWidget( tabWidget2, "tab" );
    tabLayout = new QVBoxLayout( tab, 11, 6, "tabLayout"); 

    m_pOutputBrowser = new KTextBrowser( tab, "m_pOutputBrowser" );
    tabLayout->addWidget( m_pOutputBrowser );
    tabWidget2->insertTab( tab, QString::fromLatin1("") );

    tab_2 = new QWidget( tabWidget2, "tab_2" );
    tabLayout_2 = new QVBoxLayout( tab_2, 11, 6, "tabLayout_2"); 

    m_pErrorView = new QueryView( tab_2, "m_pErrorView" );
    tabLayout_2->addWidget( m_pErrorView );
    tabWidget2->insertTab( tab_2, QString::fromLatin1("") );
    MakeLayoutLayout->addWidget( tabWidget2 );

    layout7 = new QHBoxLayout( 0, 0, 6, "layout7"); 
    spacer1 = new QSpacerItem( 520, 21, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout7->addItem( spacer1 );

    m_pMakeButton = new QPushButton( this, "m_pMakeButton" );
    m_pMakeButton->setDefault( TRUE );
    layout7->addWidget( m_pMakeButton );

    m_pStopButton = new QPushButton( this, "m_pStopButton" );
    m_pStopButton->setEnabled( FALSE );
    layout7->addWidget( m_pStopButton );

    m_pCloseButton = new QPushButton( this, "m_pCloseButton" );
    layout7->addWidget( m_pCloseButton );
    MakeLayoutLayout->addLayout( layout7 );
    languageChange();
    resize( QSize(768, 642).expandedTo(minimumSizeHint()) );
    clearWState( WState_Polished );

    // signals and slots connections
    connect( m_pCloseButton, SIGNAL( clicked() ), this, SLOT( close() ) );
    connect( m_pMakeButton, SIGNAL( clicked() ), this, SLOT( slotMake() ) );
    connect( m_pStopButton, SIGNAL( clicked() ), this, SLOT( slotStop() ) );
    connect( m_pCommandHistory, SIGNAL( returnPressed() ), this, SLOT( slotMake() ) );

    // tab order
    setTabOrder( m_pRootURL, m_pCommandHistory );
    setTabOrder( m_pCommandHistory, m_pMakeButton );
    setTabOrder( m_pMakeButton, m_pStopButton );
    setTabOrder( m_pStopButton, m_pCloseButton );
}

/*
 *  Destroys the object and frees any allocated resources
 */
MakeLayout::~MakeLayout()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void MakeLayout::languageChange()
{
    setCaption( tr2i18n( "KScope - Make" ) );
    textLabel1->setText( tr2i18n( "Root Directory:" ) );
    textLabel2->setText( tr2i18n( "Command:" ) );
    tabWidget2->changeTab( tab, tr2i18n( "Output" ) );
    tabWidget2->changeTab( tab_2, tr2i18n( "Errors a&nd Warnings" ) );
    m_pMakeButton->setText( tr2i18n( "&Make" ) );
    m_pMakeButton->setAccel( QKeySequence( tr2i18n( "Alt+M" ) ) );
    m_pStopButton->setText( tr2i18n( "&Stop" ) );
    m_pStopButton->setAccel( QKeySequence( tr2i18n( "Alt+S" ) ) );
    m_pCloseButton->setText( tr2i18n( "&Close" ) );
    m_pCloseButton->setAccel( QKeySequence( tr2i18n( "Alt+C" ) ) );
}

void MakeLayout::slotStop()
{
    qWarning( "MakeLayout::slotStop(): Not implemented yet" );
}

void MakeLayout::slotMake()
{
    qWarning( "MakeLayout::slotMake(): Not implemented yet" );
}

#include "makelayout.moc"
