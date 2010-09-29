/****************************************************************************
** Form interface generated from reading ui file './bookmarkslayout.ui'
**
** Created: Thu Sep 9 13:22:39 2010
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef BOOKMARKSLAYOUT_H
#define BOOKMARKSLAYOUT_H

#include <qvariant.h>
#include <qpixmap.h>
#include <qdialog.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QueryView;
class QFrame;
class QPushButton;

class BookmarksLayout : public QDialog
{
    Q_OBJECT

public:
    BookmarksLayout( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~BookmarksLayout();

    QueryView* m_pView;
    QFrame* line1;
    QPushButton* m_pCloseButton;

protected:
    QVBoxLayout* BookmarksLayoutLayout;
    QHBoxLayout* layout1;
    QSpacerItem* spacer1;

protected slots:
    virtual void languageChange();

private:
    QPixmap image0;

};

#endif // BOOKMARKSLAYOUT_H
