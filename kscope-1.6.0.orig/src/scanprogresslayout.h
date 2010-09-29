/****************************************************************************
** Form interface generated from reading ui file './scanprogresslayout.ui'
**
** Created: Thu Sep 9 13:22:40 2010
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef SCANPROGRESSLAYOUT_H
#define SCANPROGRESSLAYOUT_H

#include <qvariant.h>
#include <qdialog.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QLabel;
class QPushButton;

class ScanProgressLayout : public QDialog
{
    Q_OBJECT

public:
    ScanProgressLayout( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~ScanProgressLayout();

    QLabel* m_pText;
    QPushButton* m_pCancelButton;

protected:
    QVBoxLayout* ScanProgressLayoutLayout;
    QVBoxLayout* layout4;
    QSpacerItem* spacer3;
    QHBoxLayout* layout2;
    QSpacerItem* spacer2;
    QSpacerItem* spacer1;

protected slots:
    virtual void languageChange();

};

#endif // SCANPROGRESSLAYOUT_H
