/****************************************************************************
** Form interface generated from reading ui file './autocompletionlayout.ui'
**
** Created: Thu Sep 9 13:22:39 2010
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef AUTOCOMPLETIONLAYOUT_H
#define AUTOCOMPLETIONLAYOUT_H

#include <qvariant.h>
#include <qdialog.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QLabel;
class QSpinBox;
class QPushButton;

class AutoCompletionLayout : public QDialog
{
    Q_OBJECT

public:
    AutoCompletionLayout( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~AutoCompletionLayout();

    QLabel* textLabel1;
    QSpinBox* m_pMinCharsSpin;
    QLabel* textLabel2;
    QSpinBox* m_pDelaySpin;
    QLabel* textLabel3;
    QSpinBox* m_pMaxEntriesSpin;
    QPushButton* m_pOKButton;
    QPushButton* m_pCancelButton;

protected:
    QVBoxLayout* AutoCompletionLayoutLayout;
    QSpacerItem* spacer19;
    QHBoxLayout* layout20;
    QSpacerItem* spacer15;
    QHBoxLayout* layout21;
    QSpacerItem* spacer16;
    QHBoxLayout* layout22;
    QSpacerItem* spacer17;
    QHBoxLayout* layout23;
    QSpacerItem* spacer18;

protected slots:
    virtual void languageChange();

};

#endif // AUTOCOMPLETIONLAYOUT_H
