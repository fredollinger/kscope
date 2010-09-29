/****************************************************************************
** Form interface generated from reading ui file './searchresultslayout.ui'
**
** Created: Thu Sep 9 13:22:40 2010
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef SEARCHRESULTSLAYOUT_H
#define SEARCHRESULTSLAYOUT_H

#include <qvariant.h>
#include <qdialog.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QLabel;
class QLineEdit;
class QComboBox;
class QButtonGroup;
class QRadioButton;
class QCheckBox;
class QPushButton;

class SearchResultsLayout : public QDialog
{
    Q_OBJECT

public:
    SearchResultsLayout( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~SearchResultsLayout();

    QLabel* textLabel2;
    QLineEdit* m_pSearchEdit;
    QLabel* textLabel1;
    QComboBox* m_pColumnCB;
    QButtonGroup* buttonGroup1;
    QRadioButton* m_pTextRadio;
    QRadioButton* m_pRegExpRadio;
    QRadioButton* m_pSimpRegExpRadio;
    QCheckBox* m_pCaseSenCheck;
    QCheckBox* m_pNegateCheck;
    QPushButton* m_pOKButton;
    QPushButton* m_pCancelButton;

protected:
    QVBoxLayout* SearchResultsLayoutLayout;
    QHBoxLayout* layout2;
    QHBoxLayout* layout4;
    QSpacerItem* spacer2;
    QVBoxLayout* buttonGroup1Layout;
    QHBoxLayout* layout3;
    QSpacerItem* spacer1;

protected slots:
    virtual void languageChange();

};

#endif // SEARCHRESULTSLAYOUT_H
