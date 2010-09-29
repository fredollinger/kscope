/****************************************************************************
** Form interface generated from reading ui file './welcomedlg.ui'
**
** Created: Thu Sep 9 13:22:40 2010
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef WELCOMEDLG_H
#define WELCOMEDLG_H

#include <qvariant.h>
#include <qdialog.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class KTextBrowser;
class QFrame;
class QPushButton;

class WelcomeDlg : public QDialog
{
    Q_OBJECT

public:
    WelcomeDlg( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~WelcomeDlg();

    KTextBrowser* m_pBrowser;
    QFrame* line1;
    QPushButton* m_pCloseButton;

protected:
    QVBoxLayout* WelcomeDlgLayout;
    QHBoxLayout* layout1;
    QSpacerItem* spacer1;

protected slots:
    virtual void languageChange();

};

#endif // WELCOMEDLG_H
