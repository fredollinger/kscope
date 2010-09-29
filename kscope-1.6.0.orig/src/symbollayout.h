/****************************************************************************
** Form interface generated from reading ui file './symbollayout.ui'
**
** Created: Thu Sep 9 13:22:40 2010
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef SYMBOLLAYOUT_H
#define SYMBOLLAYOUT_H

#include <qvariant.h>
#include <qdialog.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QLabel;
class QComboBox;
class KHistoryCombo;
class QCheckBox;
class QFrame;
class QPushButton;
class QListView;
class QListViewItem;
class QButtonGroup;
class QRadioButton;

class SymbolLayout : public QDialog
{
    Q_OBJECT

public:
    SymbolLayout( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~SymbolLayout();

    QLabel* textLabel1;
    QLabel* textLabel2;
    QComboBox* m_pTypeCombo;
    KHistoryCombo* m_pSymbolHC;
    QCheckBox* m_pSubStringCheck;
    QFrame* line2;
    QPushButton* m_pOKButton;
    QPushButton* m_pHintButton;
    QPushButton* m_pCancelButton;
    QListView* m_pHintList;
    QButtonGroup* m_pHintGroup;
    QRadioButton* m_pBeginWithRadio;
    QRadioButton* m_pContainRadio;

protected:
    QVBoxLayout* SymbolLayoutLayout;
    QHBoxLayout* layout15;
    QVBoxLayout* layout14;
    QVBoxLayout* layout13;
    QHBoxLayout* layout3;
    QSpacerItem* spacer2;
    QVBoxLayout* m_pHintGroupLayout;

protected slots:
    virtual void languageChange();

};

#endif // SYMBOLLAYOUT_H
