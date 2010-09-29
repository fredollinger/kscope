#include <klocale.h>
/****************************************************************************
** Form implementation generated from reading ui file './cscopemsglayout.ui'
**
** Created: Thu Sep 9 13:23:05 2010
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "cscopemsglayout.h"

#include <qvariant.h>
#include <qtextedit.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>

/*
 *  Constructs a CscopeMsgLayout as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
CscopeMsgLayout::CscopeMsgLayout( QWidget* parent, const char* name, bool modal, WFlags fl )
    : QDialog( parent, name, modal, fl )
{
    if ( !name )
	setName( "CscopeMsgLayout" );
    CscopeMsgLayoutLayout = new QVBoxLayout( this, 11, 6, "CscopeMsgLayoutLayout"); 

    m_pMsgText = new QTextEdit( this, "m_pMsgText" );
    m_pMsgText->setTextFormat( QTextEdit::PlainText );
    m_pMsgText->setReadOnly( TRUE );
    CscopeMsgLayoutLayout->addWidget( m_pMsgText );

    layout1 = new QHBoxLayout( 0, 0, 6, "layout1"); 
    spacer1 = new QSpacerItem( 321, 31, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout1->addItem( spacer1 );

    m_pClearButton = new QPushButton( this, "m_pClearButton" );
    layout1->addWidget( m_pClearButton );

    m_pHideButton = new QPushButton( this, "m_pHideButton" );
    layout1->addWidget( m_pHideButton );
    CscopeMsgLayoutLayout->addLayout( layout1 );
    languageChange();
    resize( QSize(600, 451).expandedTo(minimumSizeHint()) );
    clearWState( WState_Polished );
}

/*
 *  Destroys the object and frees any allocated resources
 */
CscopeMsgLayout::~CscopeMsgLayout()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void CscopeMsgLayout::languageChange()
{
    setCaption( tr2i18n( "Cscope Error Messages" ) );
    m_pClearButton->setText( tr2i18n( "Clear" ) );
    m_pHideButton->setText( tr2i18n( "Hide" ) );
}

#include "cscopemsglayout.moc"
