/* This file is part of the KDE libraries
   Copyright (C) 2000 Matej Koss <koss@miesto.sk>
                      David Faure <faure@kde.org>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License version 2 as published by the Free Software Foundation.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/
#ifndef __kio_observer_h__
#define __kio_observer_h__

#include <qobject.h>
#include <dcopobject.h>
#include <qintdict.h>

#include <kio/global.h>
#include <kio/authinfo.h>
#include "kio/job.h"
#include "kio/skipdlg.h"
#include "kio/renamedlg.h"

class UIServer_stub;
class KURL;

namespace KIO {
  class Job;
}

/**
 * Observer for KIO::Job progress information.
 *
 * This class, of which there is always only one instance,
 * "observes" what jobs do and forwards this information
 * to the progress-info server.
 *
 * It is a DCOP object so that the UI server can call the
 * kill method when the user presses Cancel.
 *
 * Usually jobs are automatically registered by the
 * KIO::Scheduler, so you do not have to care about that.
 *
 * @short Observer for KIO::Job progress information
 * @author David Faure <faure@kde.org>
 */
class KIO_EXPORT Observer : public QObject, public DCOPObject {

  K_DCOP
  Q_OBJECT

public:

  /**
   * Returns the unique observer object.
   * @return the observer object
   */
  static Observer * self() {
      if (!s_pObserver) s_pObserver = new Observer;
      return s_pObserver;
  }

  /**
   * Called by the job constructor, to signal its presence to the
   * UI Server.
   * @param job the new job
   * @param showProgress true to show progress, false otherwise
   * @return the progress ID assigned by the UI Server to the Job.
   */
  int newJob( KIO::Job * job, bool showProgress );

  /**
   * Called by the job destructor, to tell the UI Server that
   * the job ended.
   * @param progressId the progress ID of the job, as returned by newJob()
   */
  void jobFinished( int progressId );

  /**
   * @deprecated use KIO::AutoInfo
   */
  bool openPassDlg( const QString& prompt, QString& user, QString& pass,
                    bool readOnly );

  /**
   * Opens a password dialog.
   * @param info the authentication information
   * @return true if successful ("ok" clicked), false otherwise
   */
  bool openPassDlg( KIO::AuthInfo& info );

  /**
   * Popup a message box. See KIO::SlaveBase.
   * This doesn't use DCOP anymore, it shows the dialog in the application's process.
   * Otherwise, other apps would block when trying to communicate with UIServer.
   * @param progressId the progress ID of the job, as returned by newJob()
   * @param type the type of the message box
   * @param text the text to show
   * @param caption the window caption
   * @param buttonYes the text of the "Yes" button
   * @param buttonNo the text of the "No button
   */
  static int messageBox( int progressId, int type, const QString &text, const QString &caption,
                         const QString &buttonYes, const QString &buttonNo );

  /**
   * Popup a message box. See KIO::SlaveBase.
   * This doesn't use DCOP anymore, it shows the dialog in the application's process.
   * Otherwise, other apps would block when trying to communicate with UIServer.
   * @param progressId the progress ID of the job, as returned by newJob()
   * @param type the type of the message box
   * @param text the text to show
   * @param caption the window caption
   * @param buttonYes the text of the "Yes" button
   * @param buttonNo the text of the "No button
   * @param dontAskAgainName A checkbox is added with which further confirmation can be turned off.
   *        The string is used to lookup and store the setting in kioslaverc.
   * @since 3.3
   */
  static int messageBox( int progressId, int type, const QString &text, const QString &caption,
                         const QString &buttonYes, const QString &buttonNo, const QString &dontAskAgainName );

  /**
   * @internal
   * See renamedlg.h
   */
  KIO::RenameDlg_Result open_RenameDlg( KIO::Job * job,
                                        const QString & caption,
                                        const QString& src, const QString & dest,
                                        KIO::RenameDlg_Mode mode,
                                        QString& newDest,
                                        KIO::filesize_t sizeSrc = (KIO::filesize_t) -1,
                                        KIO::filesize_t sizeDest = (KIO::filesize_t) -1,
                                        time_t ctimeSrc = (time_t) -1,
                                        time_t ctimeDest = (time_t) -1,
                                        time_t mtimeSrc = (time_t) -1,
                                        time_t mtimeDest = (time_t) -1
                                        );

  /**
   * @internal
   * See skipdlg.h
   */
  KIO::SkipDlg_Result open_SkipDlg( KIO::Job * job,
                                    bool multi,
                                    const QString & error_text );

k_dcop:
  /**
   * Called by the UI Server (using DCOP) if the user presses cancel.
   * @param progressId the progress ID of the job, as returned by newJob()
   */
  void killJob( int progressId );

  /**
   * Called by the UI Server (using DCOP) to get all the metadata of the job
   * @param progressId the progress IDof the job, as returned by newJob()
   */
  KIO::MetaData metadata( int progressId );

protected:

  static Observer * s_pObserver;
  Observer();
  ~Observer() {}

  UIServer_stub * m_uiserver;

  QIntDict< KIO::Job > m_dctJobs;

public slots:

  void slotTotalSize( KIO::Job*, KIO::filesize_t size );
  void slotTotalFiles( KIO::Job*, unsigned long files );
  void slotTotalDirs( KIO::Job*, unsigned long dirs );

  void slotProcessedSize( KIO::Job*, KIO::filesize_t size );
  void slotProcessedFiles( KIO::Job*, unsigned long files );
  void slotProcessedDirs( KIO::Job*, unsigned long dirs );

  void slotSpeed( KIO::Job*, unsigned long speed );
  void slotPercent( KIO::Job*, unsigned long percent );
  void slotInfoMessage( KIO::Job*, const QString & msg );

  void slotCopying( KIO::Job*, const KURL& from, const KURL& to );
  void slotMoving( KIO::Job*, const KURL& from, const KURL& to );
  void slotDeleting( KIO::Job*, const KURL& url );
  /// @since 3.1
  void slotTransferring( KIO::Job*, const KURL& url );
  void slotCreatingDir( KIO::Job*, const KURL& dir );
  // currently unused
  void slotCanResume( KIO::Job*, KIO::filesize_t offset );

public:
  void stating( KIO::Job*, const KURL& url );
  void mounting( KIO::Job*, const QString & dev, const QString & point );
  void unmounting( KIO::Job*, const QString & point );
protected:
  virtual void virtual_hook( int id, void* data );
private:
  class ObserverPrivate* d;
};

// -*- mode: c++; c-basic-offset: 2 -*-
#endif
