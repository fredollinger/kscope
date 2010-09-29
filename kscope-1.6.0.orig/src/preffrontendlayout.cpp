#include <klocale.h>
/****************************************************************************
** Form implementation generated from reading ui file './preffrontendlayout.ui'
**
** Created: Thu Sep 9 13:23:06 2010
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "preffrontendlayout.h"

#include <qvariant.h>
#include <qpushbutton.h>
#include <qlabel.h>
#include <kurlrequester.h>
#include <qframe.h>
#include <qtextedit.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>

/*
 *  Constructs a PrefFrontendLayout as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 */
PrefFrontendLayout::PrefFrontendLayout( QWidget* parent, const char* name, WFlags fl )
    : QWidget( parent, name, fl )
{
    if ( !name )
	setName( "PrefFrontendLayout" );
    PrefFrontendLayoutLayout = new QVBoxLayout( this, 11, 6, "PrefFrontendLayoutLayout"); 

    layout20 = new QHBoxLayout( 0, 0, 6, "layout20"); 

    layout19 = new QVBoxLayout( 0, 0, 6, "layout19"); 

    textLabel1 = new QLabel( this, "textLabel1" );
    textLabel1->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)5, 0, 0, textLabel1->sizePolicy().hasHeightForWidth() ) );
    layout19->addWidget( textLabel1 );

    textLabel2 = new QLabel( this, "textLabel2" );
    textLabel2->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)5, 0, 0, textLabel2->sizePolicy().hasHeightForWidth() ) );
    layout19->addWidget( textLabel2 );

    textLabel1_2 = new QLabel( this, "textLabel1_2" );
    layout19->addWidget( textLabel1_2 );
    layout20->addLayout( layout19 );

    layout18 = new QVBoxLayout( 0, 0, 6, "layout18"); 

    m_pCscopeURL = new KURLRequester( this, "m_pCscopeURL" );
    m_pCscopeURL->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)5, (QSizePolicy::SizeType)5, 0, 0, m_pCscopeURL->sizePolicy().hasHeightForWidth() ) );
    layout18->addWidget( m_pCscopeURL );

    m_pCtagsURL = new KURLRequester( this, "m_pCtagsURL" );
    layout18->addWidget( m_pCtagsURL );

    m_pDotURL = new KURLRequester( this, "m_pDotURL" );
    layout18->addWidget( m_pDotURL );
    layout20->addLayout( layout18 );
    PrefFrontendLayoutLayout->addLayout( layout20 );

    line2 = new QFrame( this, "line2" );
    line2->setFrameShape( QFrame::HLine );
    line2->setFrameShadow( QFrame::Sunken );
    line2->setFrameShape( QFrame::HLine );
    PrefFrontendLayoutLayout->addWidget( line2 );

    layout4 = new QHBoxLayout( 0, 0, 6, "layout4"); 
    spacer8 = new QSpacerItem( 261, 21, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout4->addItem( spacer8 );

    m_pGuessButton = new QPushButton( this, "m_pGuessButton" );
    layout4->addWidget( m_pGuessButton );
    PrefFrontendLayoutLayout->addLayout( layout4 );

    m_pScriptText = new QTextEdit( this, "m_pScriptText" );
    m_pScriptText->setEnabled( TRUE );
    m_pScriptText->setReadOnly( TRUE );
    PrefFrontendLayoutLayout->addWidget( m_pScriptText );
    languageChange();
    resize( QSize(415, 368).expandedTo(minimumSizeHint()) );
    clearWState( WState_Polished );

    // tab order
}

/*
 *  Destroys the object and frees any allocated resources
 */
PrefFrontendLayout::~PrefFrontendLayout()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void PrefFrontendLayout::languageChange()
{
    setCaption( tr2i18n( "Form3" ) );
    textLabel1->setText( tr2i18n( "Cscope path:" ) );
    textLabel2->setText( tr2i18n( "Ctags path:" ) );
    textLabel1_2->setText( tr2i18n( "Dot path:" ) );
    m_pGuessButton->setText( tr2i18n( "G&uess" ) );
}

#include "preffrontendlayout.moc"
