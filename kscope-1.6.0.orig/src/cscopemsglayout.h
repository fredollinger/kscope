/****************************************************************************
** Form interface generated from reading ui file './cscopemsglayout.ui'
**
** Created: Thu Sep 9 13:22:39 2010
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef CSCOPEMSGLAYOUT_H
#define CSCOPEMSGLAYOUT_H

#include <qvariant.h>
#include <qdialog.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QTextEdit;
class QPushButton;

class CscopeMsgLayout : public QDialog
{
    Q_OBJECT

public:
    CscopeMsgLayout( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~CscopeMsgLayout();

    QTextEdit* m_pMsgText;
    QPushButton* m_pClearButton;
    QPushButton* m_pHideButton;

protected:
    QVBoxLayout* CscopeMsgLayoutLayout;
    QHBoxLayout* layout1;
    QSpacerItem* spacer1;

protected slots:
    virtual void languageChange();

};

#endif // CSCOPEMSGLAYOUT_H
