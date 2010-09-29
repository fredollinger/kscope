/****************************************************************************
** Form interface generated from reading ui file './queryviewlayout.ui'
**
** Created: Thu Sep 9 13:22:40 2010
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef QUERYVIEWLAYOUT_H
#define QUERYVIEWLAYOUT_H

#include <qvariant.h>
#include <qpixmap.h>
#include <qdialog.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QueryView;
class QLabel;
class QFrame;
class QPushButton;

class QueryViewLayout : public QDialog
{
    Q_OBJECT

public:
    QueryViewLayout( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~QueryViewLayout();

    QueryView* m_pView;
    QLabel* textLabel1;
    QFrame* line1;
    QPushButton* buttonOk;
    QPushButton* buttonCancel;

protected:
    QVBoxLayout* QueryViewLayoutLayout;
    QHBoxLayout* Layout1;
    QSpacerItem* Horizontal_Spacing2;

protected slots:
    virtual void languageChange();

private:
    QPixmap image0;

};

#endif // QUERYVIEWLAYOUT_H
