/****************************************************************************
** Form interface generated from reading ui file './knotifywidgetbase.ui'
**
** Created: Thu Sep 9 11:14:58 2010
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef KNOTIFYWIDGETBASE_H
#define KNOTIFYWIDGETBASE_H

#include <qvariant.h>
#include <qwidget.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class KListView;
class QListViewItem;
class QGroupBox;
class QCheckBox;
class QPushButton;
class KComboBox;
class KURLRequester;

class KNotifyWidgetBase : public QWidget
{
    Q_OBJECT

public:
    KNotifyWidgetBase( QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
    ~KNotifyWidgetBase();

    KListView* m_listview;
    QGroupBox* m_controlsBox;
    QCheckBox* m_affectAllApps;
    QPushButton* m_buttonDisable;
    QPushButton* m_buttonEnable;
    KComboBox* m_comboEnable;
    KComboBox* m_comboDisable;
    QGroupBox* m_actionsBox;
    QCheckBox* m_stderr;
    QCheckBox* m_messageBox;
    QCheckBox* m_execute;
    QCheckBox* m_playSound;
    QPushButton* m_playButton;
    QCheckBox* m_taskbar;
    QCheckBox* m_logToFile;
    KURLRequester* m_executePath;
    KURLRequester* m_logfilePath;
    KURLRequester* m_soundPath;
    QCheckBox* m_passivePopup;
    QPushButton* m_extension;
    QPushButton* m_playerButton;

protected:
    QVBoxLayout* KNotifyWidgetBaseLayout;
    QGridLayout* m_controlsBoxLayout;
    QSpacerItem* Spacer3_2;
    QSpacerItem* Spacer17;
    QGridLayout* m_actionsBoxLayout;
    QSpacerItem* Spacer3;
    QHBoxLayout* Layout25;
    QHBoxLayout* Layout8;
    QSpacerItem* Spacer7;

protected slots:
    virtual void languageChange();

};

#endif // KNOTIFYWIDGETBASE_H
