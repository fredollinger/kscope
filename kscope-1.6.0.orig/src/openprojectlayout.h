/****************************************************************************
** Form interface generated from reading ui file './openprojectlayout.ui'
**
** Created: Thu Sep 9 13:22:40 2010
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef OPENPROJECTLAYOUT_H
#define OPENPROJECTLAYOUT_H

#include <qvariant.h>
#include <qdialog.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QButtonGroup;
class KURLRequester;
class QListBox;
class QListBoxItem;
class QPushButton;

class OpenProjectLayout : public QDialog
{
    Q_OBJECT

public:
    OpenProjectLayout( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~OpenProjectLayout();

    QButtonGroup* buttonGroup5;
    KURLRequester* m_pProjPathRequester;
    QButtonGroup* buttonGroup6;
    QListBox* m_pRecentList;
    QPushButton* m_pRemoveButton;
    QPushButton* m_pOpenButton;
    QPushButton* m_pCancelButton;

protected:
    QVBoxLayout* OpenProjectLayoutLayout;
    QHBoxLayout* buttonGroup5Layout;
    QVBoxLayout* buttonGroup6Layout;
    QHBoxLayout* layout9;
    QSpacerItem* spacer4;
    QHBoxLayout* layout9_2;
    QSpacerItem* spacer6;

protected slots:
    virtual void languageChange();

    virtual void slotRemoveRecent();
    virtual void slotSelectRecent(QListBoxItem*);
    virtual void slotOpenRecent(QListBoxItem*);
    virtual void slotProjectSelected(const QString&);


};

#endif // OPENPROJECTLAYOUT_H
