#include <klocale.h>
/****************************************************************************
** Form implementation generated from reading ui file './prefoptlayout.ui'
**
** Created: Thu Sep 9 13:23:06 2010
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "prefoptlayout.h"

#include <qvariant.h>
#include <qpushbutton.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qcheckbox.h>
#include <qcombobox.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>

/*
 *  Constructs a PrefOptLayout as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 */
PrefOptLayout::PrefOptLayout( QWidget* parent, const char* name, WFlags fl )
    : QWidget( parent, name, fl )
{
    if ( !name )
	setName( "PrefOptLayout" );
    PrefOptLayoutLayout = new QVBoxLayout( this, 11, 6, "PrefOptLayoutLayout"); 

    layout7 = new QHBoxLayout( 0, 0, 6, "layout7"); 

    m_pExtEditorLabel = new QLabel( this, "m_pExtEditorLabel" );
    m_pExtEditorLabel->setEnabled( TRUE );
    layout7->addWidget( m_pExtEditorLabel );

    m_pExtEditorEdit = new QLineEdit( this, "m_pExtEditorEdit" );
    m_pExtEditorEdit->setEnabled( TRUE );
    m_pExtEditorEdit->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)7, (QSizePolicy::SizeType)0, 1, 0, m_pExtEditorEdit->sizePolicy().hasHeightForWidth() ) );
    layout7->addWidget( m_pExtEditorEdit );
    PrefOptLayoutLayout->addLayout( layout7 );

    m_pReadOnlyCheck = new QCheckBox( this, "m_pReadOnlyCheck" );
    PrefOptLayoutLayout->addWidget( m_pReadOnlyCheck );

    m_pLastProjCheck = new QCheckBox( this, "m_pLastProjCheck" );
    PrefOptLayoutLayout->addWidget( m_pLastProjCheck );

    m_pTagHlCheck = new QCheckBox( this, "m_pTagHlCheck" );
    PrefOptLayoutLayout->addWidget( m_pTagHlCheck );

    m_pBriefQueryCaptCheck = new QCheckBox( this, "m_pBriefQueryCaptCheck" );
    PrefOptLayoutLayout->addWidget( m_pBriefQueryCaptCheck );

    m_pWarnModifiedOnDiskCheck = new QCheckBox( this, "m_pWarnModifiedOnDiskCheck" );
    PrefOptLayoutLayout->addWidget( m_pWarnModifiedOnDiskCheck );

    m_pAutoSortCheck = new QCheckBox( this, "m_pAutoSortCheck" );
    PrefOptLayoutLayout->addWidget( m_pAutoSortCheck );

    layout2 = new QHBoxLayout( 0, 0, 6, "layout2"); 

    textLabel1 = new QLabel( this, "textLabel1" );
    layout2->addWidget( textLabel1 );

    m_pSysProfileCB = new QComboBox( FALSE, this, "m_pSysProfileCB" );
    layout2->addWidget( m_pSysProfileCB );
    PrefOptLayoutLayout->addLayout( layout2 );

    layout3 = new QHBoxLayout( 0, 0, 6, "layout3"); 

    textLabel2 = new QLabel( this, "textLabel2" );
    layout3->addWidget( textLabel2 );

    m_pEditorPopupCB = new QComboBox( FALSE, this, "m_pEditorPopupCB" );
    layout3->addWidget( m_pEditorPopupCB );
    PrefOptLayoutLayout->addLayout( layout3 );
    spacer11 = new QSpacerItem( 21, 20, QSizePolicy::Minimum, QSizePolicy::Expanding );
    PrefOptLayoutLayout->addItem( spacer11 );
    languageChange();
    resize( QSize(354, 312).expandedTo(minimumSizeHint()) );
    clearWState( WState_Polished );
}

/*
 *  Destroys the object and frees any allocated resources
 */
PrefOptLayout::~PrefOptLayout()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void PrefOptLayout::languageChange()
{
    setCaption( tr2i18n( "Form4" ) );
    QWhatsThis::add( this, tr2i18n( "Determines whether KScope should automatically load the last project when started." ) );
    m_pExtEditorLabel->setText( tr2i18n( "External Editor" ) );
    m_pReadOnlyCheck->setText( tr2i18n( "Read-Onl&y Mode" ) );
    m_pReadOnlyCheck->setAccel( QKeySequence( tr2i18n( "Alt+Y" ) ) );
    QWhatsThis::add( m_pReadOnlyCheck, tr2i18n( "Forces all editor windows to work in a read-only mode, so that the user cannot modify the displayed files." ) );
    m_pLastProjCheck->setText( tr2i18n( "Open Last Project on Start-Up" ) );
    m_pTagHlCheck->setText( tr2i18n( "Automatic Tag Highlighting" ) );
    QWhatsThis::add( m_pTagHlCheck, tr2i18n( "Determines whether the tag list should highlight the relevant tag based on the cursor's position." ) );
    m_pBriefQueryCaptCheck->setText( tr2i18n( "Brief Tab Captions for &Query Pages" ) );
    QWhatsThis::add( m_pBriefQueryCaptCheck, tr2i18n( "If set, the tab captions for query pages will be shortened, by using aliases for the query types." ) );
    m_pWarnModifiedOnDiskCheck->setText( tr2i18n( "Warn When a File is Modified Outside KScope" ) );
    QWhatsThis::add( m_pWarnModifiedOnDiskCheck, tr2i18n( "If set, the user is prompted whenever the currently edited file is changed by an external programme." ) );
    m_pAutoSortCheck->setText( tr2i18n( "Automatically Sort Files in the File List" ) );
    m_pAutoSortCheck->setAccel( QKeySequence( QString::null ) );
    QWhatsThis::add( m_pAutoSortCheck, tr2i18n( "Sorts files in the project's file list when a project is loaded. This may be too slow for large projects on older machines." ) );
    textLabel1->setText( tr2i18n( "System Profile" ) );
    m_pSysProfileCB->clear();
    m_pSysProfileCB->insertItem( tr2i18n( "Fast" ) );
    m_pSysProfileCB->insertItem( tr2i18n( "Slow" ) );
    textLabel2->setText( tr2i18n( "Editor Popup Menu" ) );
    m_pEditorPopupCB->clear();
    m_pEditorPopupCB->insertItem( tr2i18n( "Embedded" ) );
    m_pEditorPopupCB->insertItem( tr2i18n( "KScope Only" ) );
}

#include "prefoptlayout.moc"
