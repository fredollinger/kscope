#include <klocale.h>
/****************************************************************************
** Form implementation generated from reading ui file './projectfileslayout.ui'
**
** Created: Thu Sep 9 13:23:06 2010
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "projectfileslayout.h"

#include <qvariant.h>
#include <qpushbutton.h>
#include <qlineedit.h>
#include <qheader.h>
#include <qlistview.h>
#include <qgroupbox.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>

/*
 *  Constructs a ProjectFilesLayout as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
ProjectFilesLayout::ProjectFilesLayout( QWidget* parent, const char* name, bool modal, WFlags fl )
    : QDialog( parent, name, modal, fl )
{
    if ( !name )
	setName( "ProjectFilesLayout" );
    ProjectFilesLayoutLayout = new QHBoxLayout( this, 11, 6, "ProjectFilesLayoutLayout"); 

    layout5 = new QVBoxLayout( 0, 0, 6, "layout5"); 

    layout4 = new QHBoxLayout( 0, 0, 6, "layout4"); 

    m_pFilterEdit = new QLineEdit( this, "m_pFilterEdit" );
    layout4->addWidget( m_pFilterEdit );

    m_pFilterButton = new QPushButton( this, "m_pFilterButton" );
    layout4->addWidget( m_pFilterButton );

    m_pShowAllButton = new QPushButton( this, "m_pShowAllButton" );
    layout4->addWidget( m_pShowAllButton );
    layout5->addLayout( layout4 );

    m_pFileList = new QListView( this, "m_pFileList" );
    layout5->addWidget( m_pFileList );
    ProjectFilesLayoutLayout->addLayout( layout5 );

    layout5_2 = new QVBoxLayout( 0, 0, 6, "layout5_2"); 

    groupBox1 = new QGroupBox( this, "groupBox1" );
    groupBox1->setColumnLayout(0, Qt::Vertical );
    groupBox1->layout()->setSpacing( 6 );
    groupBox1->layout()->setMargin( 11 );
    groupBox1Layout = new QVBoxLayout( groupBox1->layout() );
    groupBox1Layout->setAlignment( Qt::AlignTop );

    m_pAddFilesButton = new QPushButton( groupBox1, "m_pAddFilesButton" );
    groupBox1Layout->addWidget( m_pAddFilesButton );

    m_pAddDirButton = new QPushButton( groupBox1, "m_pAddDirButton" );
    groupBox1Layout->addWidget( m_pAddDirButton );

    m_pAddTreeButton = new QPushButton( groupBox1, "m_pAddTreeButton" );
    groupBox1Layout->addWidget( m_pAddTreeButton );
    layout5_2->addWidget( groupBox1 );

    groupBox2 = new QGroupBox( this, "groupBox2" );
    groupBox2->setColumnLayout(0, Qt::Vertical );
    groupBox2->layout()->setSpacing( 6 );
    groupBox2->layout()->setMargin( 11 );
    groupBox2Layout = new QVBoxLayout( groupBox2->layout() );
    groupBox2Layout->setAlignment( Qt::AlignTop );

    m_pRemSelButton = new QPushButton( groupBox2, "m_pRemSelButton" );
    groupBox2Layout->addWidget( m_pRemSelButton );

    m_pRemDirButton = new QPushButton( groupBox2, "m_pRemDirButton" );
    groupBox2Layout->addWidget( m_pRemDirButton );

    m_pRemTreeButton = new QPushButton( groupBox2, "m_pRemTreeButton" );
    groupBox2Layout->addWidget( m_pRemTreeButton );
    layout5_2->addWidget( groupBox2 );
    spacer3 = new QSpacerItem( 21, 118, QSizePolicy::Minimum, QSizePolicy::Expanding );
    layout5_2->addItem( spacer3 );

    groupBox3 = new QGroupBox( this, "groupBox3" );
    groupBox3->setColumnLayout(0, Qt::Vertical );
    groupBox3->layout()->setSpacing( 6 );
    groupBox3->layout()->setMargin( 11 );
    groupBox3Layout = new QVBoxLayout( groupBox3->layout() );
    groupBox3Layout->setAlignment( Qt::AlignTop );

    m_pOKButton = new QPushButton( groupBox3, "m_pOKButton" );
    groupBox3Layout->addWidget( m_pOKButton );

    m_pCancelButton = new QPushButton( groupBox3, "m_pCancelButton" );
    groupBox3Layout->addWidget( m_pCancelButton );
    layout5_2->addWidget( groupBox3 );
    ProjectFilesLayoutLayout->addLayout( layout5_2 );
    languageChange();
    resize( QSize(585, 480).expandedTo(minimumSizeHint()) );
    clearWState( WState_Polished );
}

/*
 *  Destroys the object and frees any allocated resources
 */
ProjectFilesLayout::~ProjectFilesLayout()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void ProjectFilesLayout::languageChange()
{
    setCaption( tr2i18n( "Project Files" ) );
    m_pFilterButton->setText( tr2i18n( "Filter" ) );
    m_pShowAllButton->setText( tr2i18n( "Show All" ) );
    groupBox1->setTitle( tr2i18n( "Add" ) );
    m_pAddFilesButton->setText( tr2i18n( "Files..." ) );
    m_pAddDirButton->setText( tr2i18n( "Directory..." ) );
    m_pAddTreeButton->setText( tr2i18n( "Tree..." ) );
    groupBox2->setTitle( tr2i18n( "Remove" ) );
    m_pRemSelButton->setText( tr2i18n( "Selected" ) );
    m_pRemDirButton->setText( tr2i18n( "Directory..." ) );
    m_pRemTreeButton->setText( tr2i18n( "Tree..." ) );
    groupBox3->setTitle( QString::null );
    m_pOKButton->setText( tr2i18n( "OK" ) );
    m_pCancelButton->setText( tr2i18n( "Cancel" ) );
}

#include "projectfileslayout.moc"
