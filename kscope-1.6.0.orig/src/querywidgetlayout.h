/****************************************************************************
** Form interface generated from reading ui file './querywidgetlayout.ui'
**
** Created: Thu Sep 9 13:22:40 2010
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef QUERYWIDGETLAYOUT_H
#define QUERYWIDGETLAYOUT_H

#include <qvariant.h>
#include <qpixmap.h>
#include <qwidget.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class TabWidget;

class QueryWidgetLayout : public QWidget
{
    Q_OBJECT

public:
    QueryWidgetLayout( QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
    ~QueryWidgetLayout();

    TabWidget* m_pQueryTabs;

protected:
    QHBoxLayout* QueryWidgetLayoutLayout;

protected slots:
    virtual void languageChange();

private:
    QPixmap image0;

};

#endif // QUERYWIDGETLAYOUT_H
