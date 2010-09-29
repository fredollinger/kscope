#include <klocale.h>
/****************************************************************************
** Form implementation generated from reading ui file './openprojectlayout.ui'
**
** Created: Thu Sep 9 13:23:06 2010
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "openprojectlayout.h"

#include <qvariant.h>
#include <qpushbutton.h>
#include <qbuttongroup.h>
#include <kurlrequester.h>
#include <qlistbox.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>

/*
 *  Constructs a OpenProjectLayout as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
OpenProjectLayout::OpenProjectLayout( QWidget* parent, const char* name, bool modal, WFlags fl )
    : QDialog( parent, name, modal, fl )
{
    if ( !name )
	setName( "OpenProjectLayout" );
    OpenProjectLayoutLayout = new QVBoxLayout( this, 11, 6, "OpenProjectLayoutLayout"); 

    buttonGroup5 = new QButtonGroup( this, "buttonGroup5" );
    buttonGroup5->setColumnLayout(0, Qt::Vertical );
    buttonGroup5->layout()->setSpacing( 6 );
    buttonGroup5->layout()->setMargin( 11 );
    buttonGroup5Layout = new QHBoxLayout( buttonGroup5->layout() );
    buttonGroup5Layout->setAlignment( Qt::AlignTop );

    m_pProjPathRequester = new KURLRequester( buttonGroup5, "m_pProjPathRequester" );
    buttonGroup5Layout->addWidget( m_pProjPathRequester );
    OpenProjectLayoutLayout->addWidget( buttonGroup5 );

    buttonGroup6 = new QButtonGroup( this, "buttonGroup6" );
    buttonGroup6->setColumnLayout(0, Qt::Vertical );
    buttonGroup6->layout()->setSpacing( 6 );
    buttonGroup6->layout()->setMargin( 11 );
    buttonGroup6Layout = new QVBoxLayout( buttonGroup6->layout() );
    buttonGroup6Layout->setAlignment( Qt::AlignTop );

    m_pRecentList = new QListBox( buttonGroup6, "m_pRecentList" );
    buttonGroup6Layout->addWidget( m_pRecentList );

    layout9 = new QHBoxLayout( 0, 0, 6, "layout9"); 
    spacer4 = new QSpacerItem( 281, 21, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout9->addItem( spacer4 );

    m_pRemoveButton = new QPushButton( buttonGroup6, "m_pRemoveButton" );
    layout9->addWidget( m_pRemoveButton );
    buttonGroup6Layout->addLayout( layout9 );
    OpenProjectLayoutLayout->addWidget( buttonGroup6 );

    layout9_2 = new QHBoxLayout( 0, 0, 6, "layout9_2"); 
    spacer6 = new QSpacerItem( 201, 31, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout9_2->addItem( spacer6 );

    m_pOpenButton = new QPushButton( this, "m_pOpenButton" );
    m_pOpenButton->setDefault( TRUE );
    layout9_2->addWidget( m_pOpenButton );

    m_pCancelButton = new QPushButton( this, "m_pCancelButton" );
    layout9_2->addWidget( m_pCancelButton );
    OpenProjectLayoutLayout->addLayout( layout9_2 );
    languageChange();
    resize( QSize(417, 384).expandedTo(minimumSizeHint()) );
    clearWState( WState_Polished );

    // signals and slots connections
    connect( m_pOpenButton, SIGNAL( clicked() ), this, SLOT( accept() ) );
    connect( m_pCancelButton, SIGNAL( clicked() ), this, SLOT( reject() ) );
    connect( m_pRemoveButton, SIGNAL( clicked() ), this, SLOT( slotRemoveRecent() ) );
    connect( m_pRecentList, SIGNAL( highlighted(QListBoxItem*) ), this, SLOT( slotSelectRecent(QListBoxItem*) ) );
    connect( m_pRecentList, SIGNAL( doubleClicked(QListBoxItem*) ), this, SLOT( slotOpenRecent(QListBoxItem*) ) );
    connect( m_pProjPathRequester, SIGNAL( urlSelected(const QString&) ), this, SLOT( slotProjectSelected(const QString&) ) );
    connect( m_pRecentList, SIGNAL( returnPressed(QListBoxItem*) ), this, SLOT( slotOpenRecent(QListBoxItem*) ) );
}

/*
 *  Destroys the object and frees any allocated resources
 */
OpenProjectLayout::~OpenProjectLayout()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void OpenProjectLayout::languageChange()
{
    setCaption( tr2i18n( "Open Project" ) );
    buttonGroup5->setTitle( tr2i18n( "Project Path" ) );
    buttonGroup6->setTitle( tr2i18n( "Recent Projects" ) );
    m_pRemoveButton->setText( tr2i18n( "Remove" ) );
    m_pOpenButton->setText( tr2i18n( "&Open" ) );
    m_pOpenButton->setAccel( QKeySequence( tr2i18n( "Alt+O" ) ) );
    m_pCancelButton->setText( tr2i18n( "C&ancel" ) );
}

void OpenProjectLayout::slotRemoveRecent()
{
    qWarning( "OpenProjectLayout::slotRemoveRecent(): Not implemented yet" );
}

void OpenProjectLayout::slotSelectRecent(QListBoxItem*)
{
    qWarning( "OpenProjectLayout::slotSelectRecent(QListBoxItem*): Not implemented yet" );
}

void OpenProjectLayout::slotOpenRecent(QListBoxItem*)
{
    qWarning( "OpenProjectLayout::slotOpenRecent(QListBoxItem*): Not implemented yet" );
}

void OpenProjectLayout::slotProjectSelected(const QString&)
{
    qWarning( "OpenProjectLayout::slotProjectSelected(const QString&): Not implemented yet" );
}

#include "openprojectlayout.moc"
