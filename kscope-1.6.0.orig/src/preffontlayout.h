/****************************************************************************
** Form interface generated from reading ui file './preffontlayout.ui'
**
** Created: Thu Sep 9 13:22:40 2010
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef PREFFONTLAYOUT_H
#define PREFFONTLAYOUT_H

#include <qvariant.h>
#include <qwidget.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QListView;
class QListViewItem;

class PrefFontLayout : public QWidget
{
    Q_OBJECT

public:
    PrefFontLayout( QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
    ~PrefFontLayout();

    QListView* m_pList;

protected:
    QHBoxLayout* PrefFontLayoutLayout;

protected slots:
    virtual void languageChange();

    virtual void slotItemSelected(QListViewItem*);


};

#endif // PREFFONTLAYOUT_H
