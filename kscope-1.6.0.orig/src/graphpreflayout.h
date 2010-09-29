/****************************************************************************
** Form interface generated from reading ui file './graphpreflayout.ui'
**
** Created: Thu Sep 9 13:22:39 2010
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef GRAPHPREFLAYOUT_H
#define GRAPHPREFLAYOUT_H

#include <qvariant.h>
#include <qdialog.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QLabel;
class QSpinBox;
class QPushButton;
class QFrame;

class GraphPrefLayout : public QDialog
{
    Q_OBJECT

public:
    GraphPrefLayout( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~GraphPrefLayout();

    QLabel* textLabel1;
    QSpinBox* m_pMaxDegSpin;
    QLabel* textLabel2;
    QPushButton* m_pColorButton;
    QLabel* textLabel3;
    QPushButton* m_pFontButton;
    QFrame* line1;
    QPushButton* buttonOk;
    QPushButton* buttonCancel;

protected:
    QVBoxLayout* GraphPrefLayoutLayout;
    QHBoxLayout* layout6;
    QSpacerItem* spacer2;
    QHBoxLayout* layout7;
    QSpacerItem* spacer3;
    QHBoxLayout* layout8;
    QSpacerItem* spacer4;
    QHBoxLayout* Layout1;
    QSpacerItem* Horizontal_Spacing2;

protected slots:
    virtual void languageChange();

    virtual void slotColorClicked();
    virtual void slotFontClicked();


};

#endif // GRAPHPREFLAYOUT_H
