/****************************************************************************
** Form interface generated from reading ui file './projectfileslayout.ui'
**
** Created: Thu Sep 9 13:22:40 2010
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef PROJECTFILESLAYOUT_H
#define PROJECTFILESLAYOUT_H

#include <qvariant.h>
#include <qdialog.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QLineEdit;
class QPushButton;
class QListView;
class QListViewItem;
class QGroupBox;

class ProjectFilesLayout : public QDialog
{
    Q_OBJECT

public:
    ProjectFilesLayout( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~ProjectFilesLayout();

    QLineEdit* m_pFilterEdit;
    QPushButton* m_pFilterButton;
    QPushButton* m_pShowAllButton;
    QListView* m_pFileList;
    QGroupBox* groupBox1;
    QPushButton* m_pAddFilesButton;
    QPushButton* m_pAddDirButton;
    QPushButton* m_pAddTreeButton;
    QGroupBox* groupBox2;
    QPushButton* m_pRemSelButton;
    QPushButton* m_pRemDirButton;
    QPushButton* m_pRemTreeButton;
    QGroupBox* groupBox3;
    QPushButton* m_pOKButton;
    QPushButton* m_pCancelButton;

protected:
    QHBoxLayout* ProjectFilesLayoutLayout;
    QVBoxLayout* layout5;
    QHBoxLayout* layout4;
    QVBoxLayout* layout5_2;
    QSpacerItem* spacer3;
    QVBoxLayout* groupBox1Layout;
    QVBoxLayout* groupBox2Layout;
    QVBoxLayout* groupBox3Layout;

protected slots:
    virtual void languageChange();

};

#endif // PROJECTFILESLAYOUT_H
