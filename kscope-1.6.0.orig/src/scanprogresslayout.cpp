#include <klocale.h>
/****************************************************************************
** Form implementation generated from reading ui file './scanprogresslayout.ui'
**
** Created: Thu Sep 9 13:23:06 2010
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "scanprogresslayout.h"

#include <qvariant.h>
#include <qpushbutton.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>

/*
 *  Constructs a ScanProgressLayout as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
ScanProgressLayout::ScanProgressLayout( QWidget* parent, const char* name, bool modal, WFlags fl )
    : QDialog( parent, name, modal, fl )
{
    if ( !name )
	setName( "ScanProgressLayout" );
    ScanProgressLayoutLayout = new QVBoxLayout( this, 11, 6, "ScanProgressLayoutLayout"); 

    layout4 = new QVBoxLayout( 0, 0, 6, "layout4"); 

    m_pText = new QLabel( this, "m_pText" );
    layout4->addWidget( m_pText );
    spacer3 = new QSpacerItem( 20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding );
    layout4->addItem( spacer3 );

    layout2 = new QHBoxLayout( 0, 0, 6, "layout2"); 
    spacer2 = new QSpacerItem( 40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout2->addItem( spacer2 );

    m_pCancelButton = new QPushButton( this, "m_pCancelButton" );
    m_pCancelButton->setDefault( TRUE );
    layout2->addWidget( m_pCancelButton );
    spacer1 = new QSpacerItem( 40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout2->addItem( spacer1 );
    layout4->addLayout( layout2 );
    ScanProgressLayoutLayout->addLayout( layout4 );
    languageChange();
    resize( QSize(198, 103).expandedTo(minimumSizeHint()) );
    clearWState( WState_Polished );
}

/*
 *  Destroys the object and frees any allocated resources
 */
ScanProgressLayout::~ScanProgressLayout()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void ScanProgressLayout::languageChange()
{
    setCaption( tr2i18n( "Scanning Directory" ) );
    m_pText->setText( tr2i18n( "Scanned 0 files..." ) );
    m_pCancelButton->setText( tr2i18n( "Cancel" ) );
}

#include "scanprogresslayout.moc"
