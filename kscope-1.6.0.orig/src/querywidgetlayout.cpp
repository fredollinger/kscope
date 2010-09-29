#include <klocale.h>
/****************************************************************************
** Form implementation generated from reading ui file './querywidgetlayout.ui'
**
** Created: Thu Sep 9 13:23:06 2010
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "querywidgetlayout.h"

#include <qvariant.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include "tabwidget.h"

/*
 *  Constructs a QueryWidgetLayout as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 */
QueryWidgetLayout::QueryWidgetLayout( QWidget* parent, const char* name, WFlags fl )
    : QWidget( parent, name, fl )
{
    if ( !name )
	setName( "QueryWidgetLayout" );
    QueryWidgetLayoutLayout = new QHBoxLayout( this, 0, 0, "QueryWidgetLayoutLayout"); 

    m_pQueryTabs = new TabWidget( this, "m_pQueryTabs" );
    QueryWidgetLayoutLayout->addWidget( m_pQueryTabs );
    languageChange();
    resize( QSize(740, 343).expandedTo(minimumSizeHint()) );
    clearWState( WState_Polished );
}

/*
 *  Destroys the object and frees any allocated resources
 */
QueryWidgetLayout::~QueryWidgetLayout()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void QueryWidgetLayout::languageChange()
{
    setCaption( tr2i18n( "Form2" ) );
}

#include "querywidgetlayout.moc"
