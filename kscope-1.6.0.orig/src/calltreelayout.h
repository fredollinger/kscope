/****************************************************************************
** Form interface generated from reading ui file './calltreelayout.ui'
**
** Created: Thu Sep 9 13:22:39 2010
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef CALLTREELAYOUT_H
#define CALLTREELAYOUT_H

#include <qvariant.h>
#include <qpixmap.h>
#include <qwidget.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class GraphWidget;
class TreeWidget;
class QButtonGroup;
class QToolButton;
class QFrame;
class QWidgetStack;
class QLabel;

class CallTreeLayout : public QWidget
{
    Q_OBJECT

public:
    CallTreeLayout( QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
    ~CallTreeLayout();

    QButtonGroup* m_pViewGroup;
    QToolButton* m_pGraphButton;
    QToolButton* m_pCalledButton;
    QToolButton* m_pCallingButton;
    QFrame* line1;
    QButtonGroup* m_pGraphGroup;
    QToolButton* m_pSaveButton;
    QToolButton* m_pZoomInButton;
    QToolButton* m_pZoomOutButton;
    QToolButton* m_pRotateButton;
    QToolButton* m_pPrefButton;
    QWidgetStack* m_pStack;
    QWidget* WStackPage;
    GraphWidget* m_pGraphWidget;
    QWidget* WStackPage_2;
    TreeWidget* m_pCalledWidget;
    QWidget* WStackPage_3;
    TreeWidget* m_pCallingWidget;
    QLabel* m_pHelpLabel;

protected:
    QVBoxLayout* CallTreeLayoutLayout;
    QHBoxLayout* layout2;
    QSpacerItem* spacer2;
    QHBoxLayout* m_pViewGroupLayout;
    QHBoxLayout* m_pGraphGroupLayout;
    QVBoxLayout* WStackPageLayout;
    QVBoxLayout* WStackPageLayout_2;
    QVBoxLayout* WStackPageLayout_3;

protected slots:
    virtual void languageChange();

    virtual void slotSaveClicked();
    virtual void slotZoomInClicked();
    virtual void slotZoomOutClicked();
    virtual void slotRotateClicked();
    virtual void slotViewChanged(int);
    virtual void slotViewChanged(QWidget*);
    virtual void slotPrefClicked();


private:
    QPixmap image0;

};

#endif // CALLTREELAYOUT_H
