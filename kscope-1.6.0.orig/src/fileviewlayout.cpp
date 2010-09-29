#include <klocale.h>
/****************************************************************************
** Form implementation generated from reading ui file './fileviewlayout.ui'
**
** Created: Thu Sep 9 13:23:05 2010
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "fileviewlayout.h"

#include <qvariant.h>
#include <kfiletreeview.h>
#include <qpushbutton.h>
#include <qtabwidget.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include "filelist.h"

/*
 *  Constructs a FileViewLayout as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 */
FileViewLayout::FileViewLayout( QWidget* parent, const char* name, WFlags fl )
    : QWidget( parent, name, fl )
{
    if ( !name )
	setName( "FileViewLayout" );
    FileViewLayoutLayout = new QVBoxLayout( this, 0, 0, "FileViewLayoutLayout"); 

    m_pTabWidget = new QTabWidget( this, "m_pTabWidget" );

    tab = new QWidget( m_pTabWidget, "tab" );
    tabLayout = new QVBoxLayout( tab, 0, 0, "tabLayout"); 

    m_pFileList = new FileList( tab, "m_pFileList" );
    tabLayout->addWidget( m_pFileList );
    m_pTabWidget->insertTab( tab, QString::fromLatin1("") );

    tab_2 = new QWidget( m_pTabWidget, "tab_2" );
    tabLayout_2 = new QVBoxLayout( tab_2, 0, 0, "tabLayout_2"); 

    m_pFileTree = new KFileTreeView( tab_2, "m_pFileTree" );
    tabLayout_2->addWidget( m_pFileTree );
    m_pTabWidget->insertTab( tab_2, QString::fromLatin1("") );
    FileViewLayoutLayout->addWidget( m_pTabWidget );
    languageChange();
    resize( QSize(364, 639).expandedTo(minimumSizeHint()) );
    clearWState( WState_Polished );
}

/*
 *  Destroys the object and frees any allocated resources
 */
FileViewLayout::~FileViewLayout()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void FileViewLayout::languageChange()
{
    setCaption( tr2i18n( "Form1" ) );
    QToolTip::add( m_pTabWidget, QString::null );
    QToolTip::add( m_pFileList, tr2i18n( "Project File List" ) );
    m_pTabWidget->changeTab( tab, QString::null );
    QToolTip::add( m_pFileTree, tr2i18n( "File Tree" ) );
    m_pTabWidget->changeTab( tab_2, QString::null );
}

#include "fileviewlayout.moc"
