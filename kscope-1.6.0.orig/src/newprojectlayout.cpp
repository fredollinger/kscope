#include <klocale.h>
/****************************************************************************
** Form implementation generated from reading ui file './newprojectlayout.ui'
**
** Created: Thu Sep 9 13:23:06 2010
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "newprojectlayout.h"

#include <qvariant.h>
#include <qpushbutton.h>
#include <qtabwidget.h>
#include <qwidget.h>
#include <qlabel.h>
#include <kurlrequester.h>
#include <qlineedit.h>
#include <qgroupbox.h>
#include <qlistbox.h>
#include <qcheckbox.h>
#include <qspinbox.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>

/*
 *  Constructs a NewProjectLayout as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
NewProjectLayout::NewProjectLayout( QWidget* parent, const char* name, bool modal, WFlags fl )
    : QDialog( parent, name, modal, fl )
{
    if ( !name )
	setName( "NewProjectLayout" );
    NewProjectLayoutLayout = new QVBoxLayout( this, 11, 6, "NewProjectLayoutLayout"); 

    tabWidget2 = new QTabWidget( this, "tabWidget2" );

    tab = new QWidget( tabWidget2, "tab" );
    tabLayout = new QVBoxLayout( tab, 11, 6, "tabLayout"); 

    layout9 = new QGridLayout( 0, 1, 1, 0, 6, "layout9"); 

    textLabel2 = new QLabel( tab, "textLabel2" );

    layout9->addWidget( textLabel2, 1, 0 );

    m_pSrcRootRequester = new KURLRequester( tab, "m_pSrcRootRequester" );

    layout9->addWidget( m_pSrcRootRequester, 2, 2 );

    textLabel1_2 = new QLabel( tab, "textLabel1_2" );

    layout9->addMultiCellWidget( textLabel1_2, 2, 2, 0, 1 );

    textLabel1 = new QLabel( tab, "textLabel1" );

    layout9->addWidget( textLabel1, 0, 0 );

    m_pNameEdit = new QLineEdit( tab, "m_pNameEdit" );

    layout9->addMultiCellWidget( m_pNameEdit, 0, 0, 1, 2 );

    m_pPathRequester = new KURLRequester( tab, "m_pPathRequester" );

    layout9->addMultiCellWidget( m_pPathRequester, 1, 1, 1, 2 );
    tabLayout->addLayout( layout9 );

    textLabel1_3 = new QLabel( tab, "textLabel1_3" );
    textLabel1_3->setAlignment( int( QLabel::WordBreak | QLabel::AlignVCenter ) );
    tabLayout->addWidget( textLabel1_3 );
    spacer29 = new QSpacerItem( 31, 50, QSizePolicy::Minimum, QSizePolicy::Expanding );
    tabLayout->addItem( spacer29 );
    tabWidget2->insertTab( tab, QString::fromLatin1("") );

    tab_2 = new QWidget( tabWidget2, "tab_2" );
    tabLayout_2 = new QHBoxLayout( tab_2, 11, 6, "tabLayout_2"); 

    groupBox1 = new QGroupBox( tab_2, "groupBox1" );
    groupBox1->setColumnLayout(0, Qt::Vertical );
    groupBox1->layout()->setSpacing( 6 );
    groupBox1->layout()->setMargin( 11 );
    groupBox1Layout = new QVBoxLayout( groupBox1->layout() );
    groupBox1Layout->setAlignment( Qt::AlignTop );

    m_pTypesList = new QListBox( groupBox1, "m_pTypesList" );
    groupBox1Layout->addWidget( m_pTypesList );
    tabLayout_2->addWidget( groupBox1 );

    layout5 = new QVBoxLayout( 0, 0, 6, "layout5"); 
    spacer7 = new QSpacerItem( 21, 61, QSizePolicy::Minimum, QSizePolicy::Expanding );
    layout5->addItem( spacer7 );

    m_pAddButton = new QPushButton( tab_2, "m_pAddButton" );
    layout5->addWidget( m_pAddButton );

    m_pRemoveButton = new QPushButton( tab_2, "m_pRemoveButton" );
    layout5->addWidget( m_pRemoveButton );
    spacer2 = new QSpacerItem( 21, 50, QSizePolicy::Minimum, QSizePolicy::Expanding );
    layout5->addItem( spacer2 );
    tabLayout_2->addLayout( layout5 );

    groupBox2 = new QGroupBox( tab_2, "groupBox2" );
    groupBox2->setColumnLayout(0, Qt::Vertical );
    groupBox2->layout()->setSpacing( 6 );
    groupBox2->layout()->setMargin( 11 );
    groupBox2Layout = new QVBoxLayout( groupBox2->layout() );
    groupBox2Layout->setAlignment( Qt::AlignTop );

    m_pTypesEdit = new QLineEdit( groupBox2, "m_pTypesEdit" );
    groupBox2Layout->addWidget( m_pTypesEdit );

    m_pAvailTypesList = new QListBox( groupBox2, "m_pAvailTypesList" );
    groupBox2Layout->addWidget( m_pAvailTypesList );
    tabLayout_2->addWidget( groupBox2 );
    spacer8_2 = new QSpacerItem( 61, 21, QSizePolicy::Expanding, QSizePolicy::Minimum );
    tabLayout_2->addItem( spacer8_2 );
    tabWidget2->insertTab( tab_2, QString::fromLatin1("") );

    TabPage = new QWidget( tabWidget2, "TabPage" );
    TabPageLayout = new QVBoxLayout( TabPage, 11, 6, "TabPageLayout"); 

    m_pKernelCheck = new QCheckBox( TabPage, "m_pKernelCheck" );
    TabPageLayout->addWidget( m_pKernelCheck );

    m_pInvCheck = new QCheckBox( TabPage, "m_pInvCheck" );
    TabPageLayout->addWidget( m_pInvCheck );

    m_pNoCompCheck = new QCheckBox( TabPage, "m_pNoCompCheck" );
    TabPageLayout->addWidget( m_pNoCompCheck );

    m_pSlowPathCheck = new QCheckBox( TabPage, "m_pSlowPathCheck" );
    TabPageLayout->addWidget( m_pSlowPathCheck );

    layout31 = new QHBoxLayout( 0, 0, 6, "layout31"); 

    m_pAutoRebuildCheck = new QCheckBox( TabPage, "m_pAutoRebuildCheck" );
    layout31->addWidget( m_pAutoRebuildCheck );
    spacer32 = new QSpacerItem( 125, 21, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout31->addItem( spacer32 );

    m_pAutoRebuildLabel = new QLabel( TabPage, "m_pAutoRebuildLabel" );
    m_pAutoRebuildLabel->setEnabled( FALSE );
    layout31->addWidget( m_pAutoRebuildLabel );

    m_pAutoRebuildSpin = new QSpinBox( TabPage, "m_pAutoRebuildSpin" );
    m_pAutoRebuildSpin->setEnabled( FALSE );
    layout31->addWidget( m_pAutoRebuildSpin );
    TabPageLayout->addLayout( layout31 );

    layout30 = new QHBoxLayout( 0, 0, 6, "layout30"); 

    m_pACCheck = new QCheckBox( TabPage, "m_pACCheck" );
    layout30->addWidget( m_pACCheck );
    spacer33 = new QSpacerItem( 61, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout30->addItem( spacer33 );

    m_pACButton = new QPushButton( TabPage, "m_pACButton" );
    m_pACButton->setEnabled( FALSE );
    layout30->addWidget( m_pACButton );
    TabPageLayout->addLayout( layout30 );

    layout6 = new QHBoxLayout( 0, 0, 6, "layout6"); 

    m_pTabWidthCheck = new QCheckBox( TabPage, "m_pTabWidthCheck" );
    layout6->addWidget( m_pTabWidthCheck );
    spacer9 = new QSpacerItem( 211, 31, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout6->addItem( spacer9 );

    m_pTabWidthSpin = new QSpinBox( TabPage, "m_pTabWidthSpin" );
    m_pTabWidthSpin->setEnabled( FALSE );
    layout6->addWidget( m_pTabWidthSpin );
    TabPageLayout->addLayout( layout6 );
    spacer31 = new QSpacerItem( 21, 40, QSizePolicy::Minimum, QSizePolicy::Expanding );
    TabPageLayout->addItem( spacer31 );
    tabWidget2->insertTab( TabPage, QString::fromLatin1("") );
    NewProjectLayoutLayout->addWidget( tabWidget2 );

    layout19 = new QHBoxLayout( 0, 0, 6, "layout19"); 
    spacer8 = new QSpacerItem( 141, 21, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout19->addItem( spacer8 );

    m_pCreateButton = new QPushButton( this, "m_pCreateButton" );
    m_pCreateButton->setDefault( TRUE );
    layout19->addWidget( m_pCreateButton );

    m_pCancelButton = new QPushButton( this, "m_pCancelButton" );
    layout19->addWidget( m_pCancelButton );
    NewProjectLayoutLayout->addLayout( layout19 );
    languageChange();
    resize( QSize(539, 383).expandedTo(minimumSizeHint()) );
    clearWState( WState_Polished );

    // signals and slots connections
    connect( m_pAddButton, SIGNAL( clicked() ), this, SLOT( slotAddType() ) );
    connect( m_pRemoveButton, SIGNAL( clicked() ), this, SLOT( slotRemoveType() ) );
    connect( m_pAutoRebuildCheck, SIGNAL( toggled(bool) ), m_pAutoRebuildSpin, SLOT( setEnabled(bool) ) );
    connect( m_pACCheck, SIGNAL( toggled(bool) ), m_pACButton, SLOT( setEnabled(bool) ) );
    connect( m_pAvailTypesList, SIGNAL( highlighted(const QString&) ), this, SLOT( slotAvailTypesChanged(const QString&) ) );
    connect( m_pTabWidthCheck, SIGNAL( toggled(bool) ), m_pTabWidthSpin, SLOT( setEnabled(bool) ) );
    connect( m_pAutoRebuildCheck, SIGNAL( toggled(bool) ), m_pAutoRebuildLabel, SLOT( setEnabled(bool) ) );

    // tab order
    setTabOrder( m_pNameEdit, m_pPathRequester );
    setTabOrder( m_pPathRequester, m_pSrcRootRequester );
    setTabOrder( m_pSrcRootRequester, m_pAddButton );
    setTabOrder( m_pAddButton, m_pRemoveButton );
    setTabOrder( m_pRemoveButton, m_pKernelCheck );
    setTabOrder( m_pKernelCheck, m_pInvCheck );
    setTabOrder( m_pInvCheck, m_pNoCompCheck );
    setTabOrder( m_pNoCompCheck, m_pSlowPathCheck );
    setTabOrder( m_pSlowPathCheck, m_pAutoRebuildCheck );
    setTabOrder( m_pAutoRebuildCheck, m_pAutoRebuildSpin );
    setTabOrder( m_pAutoRebuildSpin, m_pACCheck );
    setTabOrder( m_pACCheck, m_pACButton );
    setTabOrder( m_pACButton, m_pTabWidthCheck );
    setTabOrder( m_pTabWidthCheck, m_pTabWidthSpin );
    setTabOrder( m_pTabWidthSpin, m_pCreateButton );
    setTabOrder( m_pCreateButton, m_pCancelButton );
    setTabOrder( m_pCancelButton, tabWidget2 );
    setTabOrder( tabWidget2, m_pTypesList );
    setTabOrder( m_pTypesList, m_pTypesEdit );
    setTabOrder( m_pTypesEdit, m_pAvailTypesList );
}

/*
 *  Destroys the object and frees any allocated resources
 */
NewProjectLayout::~NewProjectLayout()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void NewProjectLayout::languageChange()
{
    setCaption( tr2i18n( "Create Project" ) );
    textLabel2->setText( tr2i18n( "Path" ) );
    textLabel1_2->setText( tr2i18n( "Source Root (Optional)" ) );
    textLabel1->setText( tr2i18n( "Name" ) );
    QWhatsThis::add( m_pNameEdit, tr2i18n( "Enter a name for this project.\n"
"The name must conform to the file system's naming conventions for directories (e.g., no spaces, exclamaion marks, etc.)." ) );
    QWhatsThis::add( m_pPathRequester, tr2i18n( "The path to hold this project.\n"
"KScope will create a directory with the given name under this project, and populate it with the project configuration and database files.\n"
"This does not need to be the path in which the source files reside." ) );
    textLabel1_3->setText( tr2i18n( "<blockquote>A project consists of several files located in a directory\n"
" with the given name and path. The project's name needs to be a valid directory\n"
"name and must not contain any whitespace.</blockquote>\n"
"<br>\n"
"<blockquote>The Source Root is a convinient way to specify a common\n"
"path for all source files, but is not required.</blockquote>" ) );
    tabWidget2->changeTab( tab, tr2i18n( "Detai&ls" ) );
    groupBox1->setTitle( tr2i18n( "This Project" ) );
    QWhatsThis::add( m_pTypesList, tr2i18n( "KScope uses these filters to locate source files to include in this project." ) );
    m_pAddButton->setText( tr2i18n( "<< &Add" ) );
    m_pAddButton->setAccel( QKeySequence( tr2i18n( "Alt+A" ) ) );
    QWhatsThis::add( m_pAddButton, tr2i18n( "Adds the selected file type to the current project." ) );
    m_pRemoveButton->setText( tr2i18n( ">> &Remove" ) );
    m_pRemoveButton->setAccel( QKeySequence( tr2i18n( "Alt+R" ) ) );
    QWhatsThis::add( m_pRemoveButton, tr2i18n( "Remove the selected file type from the project." ) );
    groupBox2->setTitle( tr2i18n( "Available Types" ) );
    QWhatsThis::add( m_pTypesEdit, tr2i18n( "You can enter custom file types here." ) );
    m_pAvailTypesList->clear();
    m_pAvailTypesList->insertItem( tr2i18n( "*.c" ) );
    m_pAvailTypesList->insertItem( tr2i18n( "*.h" ) );
    m_pAvailTypesList->insertItem( tr2i18n( "*.l" ) );
    m_pAvailTypesList->insertItem( tr2i18n( "*.y" ) );
    m_pAvailTypesList->insertItem( tr2i18n( "*.S" ) );
    m_pAvailTypesList->insertItem( tr2i18n( "*.cc" ) );
    m_pAvailTypesList->insertItem( tr2i18n( "*.cpp" ) );
    m_pAvailTypesList->insertItem( tr2i18n( "*.cxx" ) );
    m_pAvailTypesList->insertItem( tr2i18n( "*.C" ) );
    m_pAvailTypesList->insertItem( tr2i18n( "*.hh" ) );
    m_pAvailTypesList->insertItem( tr2i18n( "*.hpp" ) );
    m_pAvailTypesList->insertItem( tr2i18n( "*.hxx" ) );
    m_pAvailTypesList->insertItem( tr2i18n( "*.H" ) );
    QWhatsThis::add( m_pAvailTypesList, tr2i18n( "A list of standard file types." ) );
    tabWidget2->changeTab( tab_2, tr2i18n( "File T&ypes" ) );
    m_pKernelCheck->setText( tr2i18n( "Kernel project (-k)" ) );
    m_pKernelCheck->setAccel( QKeySequence( QString::null ) );
    QWhatsThis::add( m_pKernelCheck, tr2i18n( "For kernel projects, symbols are not looked up in the standard include path." ) );
    m_pInvCheck->setText( tr2i18n( "Build inverted inde&x (-q)" ) );
    m_pInvCheck->setAccel( QKeySequence( tr2i18n( "Alt+X" ) ) );
    QWhatsThis::add( m_pInvCheck, tr2i18n( "An inverted index may greatly speed up searches in a large project. The project's building process is longer, though." ) );
    m_pNoCompCheck->setText( tr2i18n( "Do not compress the database (-c)" ) );
    m_pNoCompCheck->setAccel( QKeySequence( QString::null ) );
    m_pSlowPathCheck->setText( tr2i18n( "Slower, but more accurate, function definition detection (-D)" ) );
    m_pAutoRebuildCheck->setText( tr2i18n( "Refresh data&base automatically" ) );
    m_pAutoRebuildCheck->setAccel( QKeySequence( tr2i18n( "Alt+B" ) ) );
    QWhatsThis::add( m_pAutoRebuildCheck, tr2i18n( "Rebuild the database after changed files are saved to disk." ) );
    m_pAutoRebuildLabel->setText( tr2i18n( "(Seconds)" ) );
    QWhatsThis::add( m_pAutoRebuildSpin, tr2i18n( "Wait this number of seconds after the last save before rebuilding the database." ) );
    m_pACCheck->setText( tr2i18n( "&Use symbol auto-completion" ) );
    m_pACCheck->setAccel( QKeySequence( tr2i18n( "Alt+U" ) ) );
    QWhatsThis::add( m_pACCheck, tr2i18n( "As-you-type symbol completion." ) );
    m_pACButton->setText( tr2i18n( "Options..." ) );
    m_pTabWidthCheck->setText( tr2i18n( "Override default tab width (Kate only)" ) );
    QToolTip::add( m_pTabWidthCheck, tr2i18n( "Overrides the editor's configured tab width" ) );
    tabWidget2->changeTab( TabPage, tr2i18n( "&Options" ) );
    m_pCreateButton->setText( tr2i18n( "Cre&ate" ) );
    m_pCancelButton->setText( tr2i18n( "Ca&ncel" ) );
}

void NewProjectLayout::slotAddType()
{
    qWarning( "NewProjectLayout::slotAddType(): Not implemented yet" );
}

void NewProjectLayout::slotRemoveType()
{
    qWarning( "NewProjectLayout::slotRemoveType(): Not implemented yet" );
}

void NewProjectLayout::slotAutoRebuildChanged(bool)
{
    qWarning( "NewProjectLayout::slotAutoRebuildChanged(bool): Not implemented yet" );
}

void NewProjectLayout::slotAutoCompletionChanged(bool)
{
    qWarning( "NewProjectLayout::slotAutoCompletionChanged(bool): Not implemented yet" );
}

void NewProjectLayout::slotAvailTypesChanged(const QString&)
{
    qWarning( "NewProjectLayout::slotAvailTypesChanged(const QString&): Not implemented yet" );
}

#include "newprojectlayout.moc"
