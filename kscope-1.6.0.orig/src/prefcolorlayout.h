/****************************************************************************
** Form interface generated from reading ui file './prefcolorlayout.ui'
**
** Created: Thu Sep 9 13:22:40 2010
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef PREFCOLORLAYOUT_H
#define PREFCOLORLAYOUT_H

#include <qvariant.h>
#include <qwidget.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QListView;
class QListViewItem;

class PrefColorLayout : public QWidget
{
    Q_OBJECT

public:
    PrefColorLayout( QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
    ~PrefColorLayout();

    QListView* m_pList;

protected:
    QHBoxLayout* PrefColorLayoutLayout;

protected slots:
    virtual void languageChange();

    virtual void slotItemSelected(QListViewItem*);


};

#endif // PREFCOLORLAYOUT_H
