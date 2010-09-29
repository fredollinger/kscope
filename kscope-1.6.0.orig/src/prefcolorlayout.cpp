#include <klocale.h>
/****************************************************************************
** Form implementation generated from reading ui file './prefcolorlayout.ui'
**
** Created: Thu Sep 9 13:23:06 2010
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "prefcolorlayout.h"

#include <qvariant.h>
#include <qheader.h>
#include <qlistview.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>

/*
 *  Constructs a PrefColorLayout as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 */
PrefColorLayout::PrefColorLayout( QWidget* parent, const char* name, WFlags fl )
    : QWidget( parent, name, fl )
{
    if ( !name )
	setName( "PrefColorLayout" );
    PrefColorLayoutLayout = new QHBoxLayout( this, 11, 6, "PrefColorLayoutLayout"); 

    m_pList = new QListView( this, "m_pList" );
    m_pList->addColumn( tr2i18n( "GUI Element" ) );
    m_pList->addColumn( tr2i18n( "Colour" ) );
    m_pList->header()->setResizeEnabled( FALSE, m_pList->header()->count() - 1 );
    PrefColorLayoutLayout->addWidget( m_pList );
    languageChange();
    resize( QSize(350, 210).expandedTo(minimumSizeHint()) );
    clearWState( WState_Polished );

    // signals and slots connections
    connect( m_pList, SIGNAL( doubleClicked(QListViewItem*) ), this, SLOT( slotItemSelected(QListViewItem*) ) );
    connect( m_pList, SIGNAL( returnPressed(QListViewItem*) ), this, SLOT( slotItemSelected(QListViewItem*) ) );
}

/*
 *  Destroys the object and frees any allocated resources
 */
PrefColorLayout::~PrefColorLayout()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void PrefColorLayout::languageChange()
{
    setCaption( tr2i18n( "Form4" ) );
    m_pList->header()->setLabel( 0, tr2i18n( "GUI Element" ) );
    m_pList->header()->setLabel( 1, tr2i18n( "Colour" ) );
}

void PrefColorLayout::slotItemSelected(QListViewItem*)
{
    qWarning( "PrefColorLayout::slotItemSelected(QListViewItem*): Not implemented yet" );
}

#include "prefcolorlayout.moc"
