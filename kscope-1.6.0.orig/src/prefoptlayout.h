/****************************************************************************
** Form interface generated from reading ui file './prefoptlayout.ui'
**
** Created: Thu Sep 9 13:22:40 2010
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef PREFOPTLAYOUT_H
#define PREFOPTLAYOUT_H

#include <qvariant.h>
#include <qwidget.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QLabel;
class QLineEdit;
class QCheckBox;
class QComboBox;

class PrefOptLayout : public QWidget
{
    Q_OBJECT

public:
    PrefOptLayout( QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
    ~PrefOptLayout();

    QLabel* m_pExtEditorLabel;
    QLineEdit* m_pExtEditorEdit;
    QCheckBox* m_pReadOnlyCheck;
    QCheckBox* m_pLastProjCheck;
    QCheckBox* m_pTagHlCheck;
    QCheckBox* m_pBriefQueryCaptCheck;
    QCheckBox* m_pWarnModifiedOnDiskCheck;
    QCheckBox* m_pAutoSortCheck;
    QLabel* textLabel1;
    QComboBox* m_pSysProfileCB;
    QLabel* textLabel2;
    QComboBox* m_pEditorPopupCB;

protected:
    QVBoxLayout* PrefOptLayoutLayout;
    QSpacerItem* spacer11;
    QHBoxLayout* layout7;
    QHBoxLayout* layout2;
    QHBoxLayout* layout3;

protected slots:
    virtual void languageChange();

};

#endif // PREFOPTLAYOUT_H
