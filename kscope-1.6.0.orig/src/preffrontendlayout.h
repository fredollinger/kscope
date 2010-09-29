/****************************************************************************
** Form interface generated from reading ui file './preffrontendlayout.ui'
**
** Created: Thu Sep 9 13:22:40 2010
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef PREFFRONTENDLAYOUT_H
#define PREFFRONTENDLAYOUT_H

#include <qvariant.h>
#include <qwidget.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QLabel;
class KURLRequester;
class QFrame;
class QPushButton;
class QTextEdit;

class PrefFrontendLayout : public QWidget
{
    Q_OBJECT

public:
    PrefFrontendLayout( QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
    ~PrefFrontendLayout();

    QLabel* textLabel1;
    QLabel* textLabel2;
    QLabel* textLabel1_2;
    KURLRequester* m_pCscopeURL;
    KURLRequester* m_pCtagsURL;
    KURLRequester* m_pDotURL;
    QFrame* line2;
    QPushButton* m_pGuessButton;
    QTextEdit* m_pScriptText;

protected:
    QVBoxLayout* PrefFrontendLayoutLayout;
    QHBoxLayout* layout20;
    QVBoxLayout* layout19;
    QVBoxLayout* layout18;
    QHBoxLayout* layout4;
    QSpacerItem* spacer8;

protected slots:
    virtual void languageChange();

};

#endif // PREFFRONTENDLAYOUT_H
