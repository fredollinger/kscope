/****************************************************************************
** Form interface generated from reading ui file './newprojectlayout.ui'
**
** Created: Thu Sep 9 13:22:39 2010
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef NEWPROJECTLAYOUT_H
#define NEWPROJECTLAYOUT_H

#include <qvariant.h>
#include <qdialog.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QTabWidget;
class QWidget;
class QLabel;
class KURLRequester;
class QLineEdit;
class QGroupBox;
class QListBox;
class QListBoxItem;
class QPushButton;
class QCheckBox;
class QSpinBox;

class NewProjectLayout : public QDialog
{
    Q_OBJECT

public:
    NewProjectLayout( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~NewProjectLayout();

    QTabWidget* tabWidget2;
    QWidget* tab;
    QLabel* textLabel2;
    KURLRequester* m_pSrcRootRequester;
    QLabel* textLabel1_2;
    QLabel* textLabel1;
    QLineEdit* m_pNameEdit;
    KURLRequester* m_pPathRequester;
    QLabel* textLabel1_3;
    QWidget* tab_2;
    QGroupBox* groupBox1;
    QListBox* m_pTypesList;
    QPushButton* m_pAddButton;
    QPushButton* m_pRemoveButton;
    QGroupBox* groupBox2;
    QLineEdit* m_pTypesEdit;
    QListBox* m_pAvailTypesList;
    QWidget* TabPage;
    QCheckBox* m_pKernelCheck;
    QCheckBox* m_pInvCheck;
    QCheckBox* m_pNoCompCheck;
    QCheckBox* m_pSlowPathCheck;
    QCheckBox* m_pAutoRebuildCheck;
    QLabel* m_pAutoRebuildLabel;
    QSpinBox* m_pAutoRebuildSpin;
    QCheckBox* m_pACCheck;
    QPushButton* m_pACButton;
    QCheckBox* m_pTabWidthCheck;
    QSpinBox* m_pTabWidthSpin;
    QPushButton* m_pCreateButton;
    QPushButton* m_pCancelButton;

protected:
    QVBoxLayout* NewProjectLayoutLayout;
    QVBoxLayout* tabLayout;
    QSpacerItem* spacer29;
    QGridLayout* layout9;
    QHBoxLayout* tabLayout_2;
    QSpacerItem* spacer8_2;
    QVBoxLayout* groupBox1Layout;
    QVBoxLayout* layout5;
    QSpacerItem* spacer7;
    QSpacerItem* spacer2;
    QVBoxLayout* groupBox2Layout;
    QVBoxLayout* TabPageLayout;
    QSpacerItem* spacer31;
    QHBoxLayout* layout31;
    QSpacerItem* spacer32;
    QHBoxLayout* layout30;
    QSpacerItem* spacer33;
    QHBoxLayout* layout6;
    QSpacerItem* spacer9;
    QHBoxLayout* layout19;
    QSpacerItem* spacer8;

protected slots:
    virtual void languageChange();

    virtual void slotAddType();
    virtual void slotRemoveType();
    virtual void slotAutoRebuildChanged(bool);
    virtual void slotAutoCompletionChanged(bool);
    virtual void slotAvailTypesChanged(const QString&);


};

#endif // NEWPROJECTLAYOUT_H
