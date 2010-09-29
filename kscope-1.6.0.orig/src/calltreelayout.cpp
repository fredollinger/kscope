#include <klocale.h>
/****************************************************************************
** Form implementation generated from reading ui file './calltreelayout.ui'
**
** Created: Thu Sep 9 13:23:05 2010
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "calltreelayout.h"

#include <qvariant.h>
#include <qpushbutton.h>
#include <qbuttongroup.h>
#include <qtoolbutton.h>
#include <qframe.h>
#include <qwidgetstack.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include "graphwidget.h"
#include "treewidget.h"

/*
 *  Constructs a CallTreeLayout as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 */
CallTreeLayout::CallTreeLayout( QWidget* parent, const char* name, WFlags fl )
    : QWidget( parent, name, fl )
{
    if ( !name )
	setName( "CallTreeLayout" );
    CallTreeLayoutLayout = new QVBoxLayout( this, 11, 6, "CallTreeLayoutLayout"); 

    layout2 = new QHBoxLayout( 0, 0, 6, "layout2"); 

    m_pViewGroup = new QButtonGroup( this, "m_pViewGroup" );
    m_pViewGroup->setFrameShape( QButtonGroup::NoFrame );
    m_pViewGroup->setLineWidth( 0 );
    m_pViewGroup->setExclusive( TRUE );
    m_pViewGroup->setColumnLayout(0, Qt::Vertical );
    m_pViewGroup->layout()->setSpacing( 6 );
    m_pViewGroup->layout()->setMargin( 0 );
    m_pViewGroupLayout = new QHBoxLayout( m_pViewGroup->layout() );
    m_pViewGroupLayout->setAlignment( Qt::AlignTop );

    m_pGraphButton = new QToolButton( m_pViewGroup, "m_pGraphButton" );
    m_pGraphButton->setToggleButton( TRUE );
    m_pGraphButton->setAutoRaise( TRUE );
    m_pViewGroupLayout->addWidget( m_pGraphButton );

    m_pCalledButton = new QToolButton( m_pViewGroup, "m_pCalledButton" );
    m_pCalledButton->setToggleButton( TRUE );
    m_pCalledButton->setAutoRaise( TRUE );
    m_pViewGroupLayout->addWidget( m_pCalledButton );

    m_pCallingButton = new QToolButton( m_pViewGroup, "m_pCallingButton" );
    m_pCallingButton->setToggleButton( TRUE );
    m_pCallingButton->setAutoRaise( TRUE );
    m_pViewGroupLayout->addWidget( m_pCallingButton );
    layout2->addWidget( m_pViewGroup );

    line1 = new QFrame( this, "line1" );
    line1->setFrameShape( QFrame::VLine );
    line1->setFrameShadow( QFrame::Sunken );
    line1->setFrameShape( QFrame::VLine );
    layout2->addWidget( line1 );

    m_pGraphGroup = new QButtonGroup( this, "m_pGraphGroup" );
    m_pGraphGroup->setFrameShape( QButtonGroup::NoFrame );
    m_pGraphGroup->setLineWidth( 0 );
    m_pGraphGroup->setFlat( TRUE );
    m_pGraphGroup->setColumnLayout(0, Qt::Vertical );
    m_pGraphGroup->layout()->setSpacing( 6 );
    m_pGraphGroup->layout()->setMargin( 0 );
    m_pGraphGroupLayout = new QHBoxLayout( m_pGraphGroup->layout() );
    m_pGraphGroupLayout->setAlignment( Qt::AlignTop );

    m_pSaveButton = new QToolButton( m_pGraphGroup, "m_pSaveButton" );
    m_pSaveButton->setAutoRaise( TRUE );
    m_pGraphGroupLayout->addWidget( m_pSaveButton );

    m_pZoomInButton = new QToolButton( m_pGraphGroup, "m_pZoomInButton" );
    m_pZoomInButton->setToggleButton( FALSE );
    m_pZoomInButton->setAutoRaise( TRUE );
    m_pGraphGroupLayout->addWidget( m_pZoomInButton );

    m_pZoomOutButton = new QToolButton( m_pGraphGroup, "m_pZoomOutButton" );
    m_pZoomOutButton->setToggleButton( FALSE );
    m_pZoomOutButton->setAutoRaise( TRUE );
    m_pGraphGroupLayout->addWidget( m_pZoomOutButton );

    m_pRotateButton = new QToolButton( m_pGraphGroup, "m_pRotateButton" );
    m_pRotateButton->setToggleButton( FALSE );
    m_pRotateButton->setAutoRaise( TRUE );
    m_pGraphGroupLayout->addWidget( m_pRotateButton );

    m_pPrefButton = new QToolButton( m_pGraphGroup, "m_pPrefButton" );
    m_pPrefButton->setToggleButton( FALSE );
    m_pPrefButton->setAutoRaise( TRUE );
    m_pGraphGroupLayout->addWidget( m_pPrefButton );
    layout2->addWidget( m_pGraphGroup );
    spacer2 = new QSpacerItem( 110, 21, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout2->addItem( spacer2 );
    CallTreeLayoutLayout->addLayout( layout2 );

    m_pStack = new QWidgetStack( this, "m_pStack" );
    m_pStack->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)7, (QSizePolicy::SizeType)7, 0, 0, m_pStack->sizePolicy().hasHeightForWidth() ) );

    WStackPage = new QWidget( m_pStack, "WStackPage" );
    WStackPageLayout = new QVBoxLayout( WStackPage, 0, 6, "WStackPageLayout"); 

    m_pGraphWidget = new GraphWidget( WStackPage, "m_pGraphWidget" );
    WStackPageLayout->addWidget( m_pGraphWidget );
    m_pStack->addWidget( WStackPage, 0 );

    WStackPage_2 = new QWidget( m_pStack, "WStackPage_2" );
    WStackPageLayout_2 = new QVBoxLayout( WStackPage_2, 0, 6, "WStackPageLayout_2"); 

    m_pCalledWidget = new TreeWidget( WStackPage_2, "m_pCalledWidget" );
    WStackPageLayout_2->addWidget( m_pCalledWidget );
    m_pStack->addWidget( WStackPage_2, 1 );

    WStackPage_3 = new QWidget( m_pStack, "WStackPage_3" );
    WStackPageLayout_3 = new QVBoxLayout( WStackPage_3, 0, 6, "WStackPageLayout_3"); 

    m_pCallingWidget = new TreeWidget( WStackPage_3, "m_pCallingWidget" );
    WStackPageLayout_3->addWidget( m_pCallingWidget );
    m_pStack->addWidget( WStackPage_3, 2 );
    CallTreeLayoutLayout->addWidget( m_pStack );

    m_pHelpLabel = new QLabel( this, "m_pHelpLabel" );
    CallTreeLayoutLayout->addWidget( m_pHelpLabel );
    languageChange();
    resize( QSize(695, 578).expandedTo(minimumSizeHint()) );
    clearWState( WState_Polished );

    // signals and slots connections
    connect( m_pPrefButton, SIGNAL( clicked() ), this, SLOT( slotPrefClicked() ) );
    connect( m_pRotateButton, SIGNAL( clicked() ), this, SLOT( slotRotateClicked() ) );
    connect( m_pZoomOutButton, SIGNAL( clicked() ), this, SLOT( slotZoomOutClicked() ) );
    connect( m_pZoomInButton, SIGNAL( clicked() ), this, SLOT( slotZoomInClicked() ) );
    connect( m_pSaveButton, SIGNAL( clicked() ), this, SLOT( slotSaveClicked() ) );
    connect( m_pViewGroup, SIGNAL( clicked(int) ), this, SLOT( slotViewChanged(int) ) );
    connect( m_pViewGroup, SIGNAL( clicked(int) ), m_pStack, SLOT( raiseWidget(int) ) );
}

/*
 *  Destroys the object and frees any allocated resources
 */
CallTreeLayout::~CallTreeLayout()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void CallTreeLayout::languageChange()
{
    setCaption( tr2i18n( "Call Graph" ) );
    m_pViewGroup->setTitle( QString::null );
    m_pGraphButton->setText( tr2i18n( "..." ) );
    QToolTip::add( m_pGraphButton, tr2i18n( "Call Graph" ) );
    m_pCalledButton->setText( tr2i18n( "..." ) );
    QToolTip::add( m_pCalledButton, tr2i18n( "Called Functions Tree" ) );
    m_pCallingButton->setText( tr2i18n( "..." ) );
    QToolTip::add( m_pCallingButton, tr2i18n( "Calling Functions Tree" ) );
    m_pGraphGroup->setTitle( QString::null );
    m_pSaveButton->setText( tr2i18n( "a" ) );
    QToolTip::add( m_pSaveButton, tr2i18n( "Save As..." ) );
    m_pZoomInButton->setText( tr2i18n( "a" ) );
    QToolTip::add( m_pZoomInButton, tr2i18n( "Zoom In" ) );
    m_pZoomOutButton->setText( tr2i18n( "a" ) );
    QToolTip::add( m_pZoomOutButton, tr2i18n( "Zoom Out" ) );
    m_pRotateButton->setText( tr2i18n( "a" ) );
    QToolTip::add( m_pRotateButton, tr2i18n( "Rotate" ) );
    m_pPrefButton->setText( tr2i18n( "a" ) );
    QToolTip::add( m_pPrefButton, tr2i18n( "Preferences" ) );
    m_pHelpLabel->setText( tr2i18n( "Help Message" ) );
}

void CallTreeLayout::slotSaveClicked()
{
    qWarning( "CallTreeLayout::slotSaveClicked(): Not implemented yet" );
}

void CallTreeLayout::slotZoomInClicked()
{
    qWarning( "CallTreeLayout::slotZoomInClicked(): Not implemented yet" );
}

void CallTreeLayout::slotZoomOutClicked()
{
    qWarning( "CallTreeLayout::slotZoomOutClicked(): Not implemented yet" );
}

void CallTreeLayout::slotRotateClicked()
{
    qWarning( "CallTreeLayout::slotRotateClicked(): Not implemented yet" );
}

void CallTreeLayout::slotViewChanged(int)
{
    qWarning( "CallTreeLayout::slotViewChanged(int): Not implemented yet" );
}

void CallTreeLayout::slotViewChanged(QWidget*)
{
    qWarning( "CallTreeLayout::slotViewChanged(QWidget*): Not implemented yet" );
}

void CallTreeLayout::slotPrefClicked()
{
    qWarning( "CallTreeLayout::slotPrefClicked(): Not implemented yet" );
}

#include "calltreelayout.moc"
