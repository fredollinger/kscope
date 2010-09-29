#include <klocale.h>
/****************************************************************************
** Form implementation generated from reading ui file './welcomedlg.ui'
**
** Created: Thu Sep 9 13:23:06 2010
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "welcomedlg.h"

#include <qvariant.h>
#include <ktextbrowser.h>
#include <qframe.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>

/*
 *  Constructs a WelcomeDlg as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
WelcomeDlg::WelcomeDlg( QWidget* parent, const char* name, bool modal, WFlags fl )
    : QDialog( parent, name, modal, fl )
{
    if ( !name )
	setName( "WelcomeDlg" );
    WelcomeDlgLayout = new QVBoxLayout( this, 11, 6, "WelcomeDlgLayout"); 

    m_pBrowser = new KTextBrowser( this, "m_pBrowser" );
    m_pBrowser->setPaletteBackgroundColor( QColor( 255, 255, 255 ) );
    WelcomeDlgLayout->addWidget( m_pBrowser );

    line1 = new QFrame( this, "line1" );
    line1->setFrameShape( QFrame::HLine );
    line1->setFrameShadow( QFrame::Sunken );
    line1->setFrameShape( QFrame::HLine );
    WelcomeDlgLayout->addWidget( line1 );

    layout1 = new QHBoxLayout( 0, 0, 6, "layout1"); 
    spacer1 = new QSpacerItem( 381, 21, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout1->addItem( spacer1 );

    m_pCloseButton = new QPushButton( this, "m_pCloseButton" );
    m_pCloseButton->setDefault( TRUE );
    layout1->addWidget( m_pCloseButton );
    WelcomeDlgLayout->addLayout( layout1 );
    languageChange();
    resize( QSize(519, 386).expandedTo(minimumSizeHint()) );
    clearWState( WState_Polished );

    // signals and slots connections
    connect( m_pCloseButton, SIGNAL( clicked() ), this, SLOT( accept() ) );
}

/*
 *  Destroys the object and frees any allocated resources
 */
WelcomeDlg::~WelcomeDlg()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void WelcomeDlg::languageChange()
{
    setCaption( tr2i18n( "Welcome" ) );
    m_pBrowser->setText( tr2i18n( "<h1>Welcome to <font color=\"#c00000\">KScope</font>!</h1>\n"
"\n"
"If this is the first time you are running Kscope, please follow these steps (click on the links for detailed instructions):\n"
"<p>\n"
"1. <a href=\"help:/kscope/configuration.html#config-progs\">Configure</a> paths to the required back-end executables<br>\n"
"2. <a href=\"help:/kscope/projects.html#project-create\">Create</a> a new project<br>\n"
"3. <a href=\"help:/kscope/projects.html#project-files\">Populate</a> the project with source files<br>\n"
"4. <a href=\"help:/kscope/queries.html\">Browse</a> the project and <a href=\"help:/kscope/editing.html\">edit</a> files<br>\n"
"\n"
"</p>\n"
"\n"
"<p>\n"
"For more information, please take a look at KScope's <a href=\"help:/kscope\">manual</a>, or visit the KScope <a href=\"http://kscope.sourceforge.net\">website</a>.\n"
"</p>\n"
"\n"
"<p>\n"
"Enjoy!\n"
"</p>\n"
"\n"
"<p>\n"
"<font size=\"-1\">This message will only appear once. Use the \"<b>Help->Show Welcome Message...</b>\" menu command to show it again at any time.</font>\n"
"</p>" ) );
    m_pCloseButton->setText( tr2i18n( "Close" ) );
}

#include "welcomedlg.moc"
