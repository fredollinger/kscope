#include <klocale.h>
/****************************************************************************
** Form implementation generated from reading ui file './bookmarkslayout.ui'
**
** Created: Thu Sep 9 13:23:05 2010
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "bookmarkslayout.h"

#include <qvariant.h>
#include <qframe.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include "queryview.h"

/*
 *  Constructs a BookmarksLayout as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
BookmarksLayout::BookmarksLayout( QWidget* parent, const char* name, bool modal, WFlags fl )
    : QDialog( parent, name, modal, fl )
{
    if ( !name )
	setName( "BookmarksLayout" );
    BookmarksLayoutLayout = new QVBoxLayout( this, 11, 6, "BookmarksLayoutLayout"); 

    m_pView = new QueryView( this, "m_pView" );
    BookmarksLayoutLayout->addWidget( m_pView );

    line1 = new QFrame( this, "line1" );
    line1->setFrameShape( QFrame::HLine );
    line1->setFrameShadow( QFrame::Sunken );
    line1->setFrameShape( QFrame::HLine );
    BookmarksLayoutLayout->addWidget( line1 );

    layout1 = new QHBoxLayout( 0, 0, 6, "layout1"); 
    spacer1 = new QSpacerItem( 291, 21, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout1->addItem( spacer1 );

    m_pCloseButton = new QPushButton( this, "m_pCloseButton" );
    layout1->addWidget( m_pCloseButton );
    BookmarksLayoutLayout->addLayout( layout1 );
    languageChange();
    resize( QSize(600, 480).expandedTo(minimumSizeHint()) );
    clearWState( WState_Polished );

    // signals and slots connections
    connect( m_pCloseButton, SIGNAL( clicked() ), this, SLOT( reject() ) );
}

/*
 *  Destroys the object and frees any allocated resources
 */
BookmarksLayout::~BookmarksLayout()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void BookmarksLayout::languageChange()
{
    setCaption( tr2i18n( "Global Bookmarks" ) );
    m_pCloseButton->setText( tr2i18n( "Close" ) );
    m_pCloseButton->setAccel( QKeySequence( QString::null ) );
}

#include "bookmarkslayout.moc"
