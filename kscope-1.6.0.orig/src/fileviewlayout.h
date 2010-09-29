/****************************************************************************
** Form interface generated from reading ui file './fileviewlayout.ui'
**
** Created: Thu Sep 9 13:22:39 2010
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef FILEVIEWLAYOUT_H
#define FILEVIEWLAYOUT_H

#include <qvariant.h>
#include <qpixmap.h>
#include <qwidget.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class FileList;
class KFileTreeView;
class QTabWidget;

class FileViewLayout : public QWidget
{
    Q_OBJECT

public:
    FileViewLayout( QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
    ~FileViewLayout();

    QTabWidget* m_pTabWidget;
    QWidget* tab;
    FileList* m_pFileList;
    QWidget* tab_2;
    KFileTreeView* m_pFileTree;

protected:
    QVBoxLayout* FileViewLayoutLayout;
    QVBoxLayout* tabLayout;
    QVBoxLayout* tabLayout_2;

protected slots:
    virtual void languageChange();

private:
    QPixmap image0;

};

#endif // FILEVIEWLAYOUT_H
