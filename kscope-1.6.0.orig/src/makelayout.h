/****************************************************************************
** Form interface generated from reading ui file './makelayout.ui'
**
** Created: Thu Sep 9 13:22:39 2010
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef MAKELAYOUT_H
#define MAKELAYOUT_H

#include <qvariant.h>
#include <qpixmap.h>
#include <qwidget.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QueryView;
class QLabel;
class KHistoryCombo;
class KURLRequester;
class QTabWidget;
class KTextBrowser;
class QPushButton;

class MakeLayout : public QWidget
{
    Q_OBJECT

public:
    MakeLayout( QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
    ~MakeLayout();

    QLabel* textLabel1;
    KHistoryCombo* m_pCommandHistory;
    KURLRequester* m_pRootURL;
    QLabel* textLabel2;
    QTabWidget* tabWidget2;
    QWidget* tab;
    KTextBrowser* m_pOutputBrowser;
    QWidget* tab_2;
    QueryView* m_pErrorView;
    QPushButton* m_pMakeButton;
    QPushButton* m_pStopButton;
    QPushButton* m_pCloseButton;

protected:
    QVBoxLayout* MakeLayoutLayout;
    QGridLayout* layout10;
    QVBoxLayout* tabLayout;
    QVBoxLayout* tabLayout_2;
    QHBoxLayout* layout7;
    QSpacerItem* spacer1;

protected slots:
    virtual void languageChange();

    virtual void slotStop();
    virtual void slotMake();


private:
    QPixmap image0;

};

#endif // MAKELAYOUT_H
