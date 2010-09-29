/****************************************************************************

 Copyright (C) 2001-2003 Lubos Lunak        <l.lunak@kde.org>

Permission is hereby granted, free of charge, to any person obtaining a
copy of this software and associated documentation files (the "Software"),
to deal in the Software without restriction, including without limitation
the rights to use, copy, modify, merge, publish, distribute, sublicense,
and/or sell copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
DEALINGS IN THE SOFTWARE.

****************************************************************************/

#ifndef __KSTARTUPINFO_H
#define __KSTARTUPINFO_H

#include <sys/types.h>
#include <qobject.h>

#ifdef Q_WS_X11 // FIXME(E): Redo in a less X11-specific way

#include <qcstring.h>
#include <qstring.h>
#include <qvaluelist.h>
#include "kdelibs_export.h"

class KStartupInfoId;
class KStartupInfoData;

class KStartupInfoPrivate;

/**
 * Class for manipulating the application startup notification.
 *
 * This class can be used to send information about started application,
 * change the information and receive this information. For detailed
 * description, see kdelibs/kdecore/README.kstartupinfo.
 *
 * You usually don't need to use this class for sending the notification
 * information, as KDE libraries should do this when an application is
 * started (e.g. KRun class).
 *
 * For receiving the startup notification info, create an instance and connect
 * to its slots. It will automatically detect started applications and when
 * they are ready.
 *
 * @see KStartupInfoId
 * @see KStartupInfoData
 *
 * @author Lubos Lunak <l.lunak@kde.org>
 */
class KDECORE_EXPORT KStartupInfo
    : public QObject
    {
    Q_OBJECT
    public:
        /**
         * By default, the startup notification is ended for the application
         * after it shows its first toplevel window. If you app e.g. has
         * several toplevel windows after its startup, you can disable
         * the automatic handling, and call appStarted() manually after
         * all toplevel windows have been shown.
         * @since 3.2
         */
        static void disableAutoAppStartedSending( bool disable = true );

        /**
         * Manual notification that the application has started.
         * If you do not map a (toplevel) window, then startup
         * notification will not disappear for the application
         * until a timeout. You can use this as an alternative
         * method in this case.
         */
        static void appStarted();

        /**
         * Sends explicit notification that the startup notification
         * with id startup_id should end.
         * @since 3.2
         */ 
        static void appStarted( const QCString& startup_id );
        
        /**
         * Use this function if the application got a request with startup
         * notification from outside (for example, when KUniqueApplication::newInstance()
         * is called, or e.g. when khelpcenter opens new URL in its window).
         * The window can be either an already existing and visible window,
         * or a new one, before being shown. Note that this function is usually
         * needed only when a window is reused.
         * @since 3.2
         */
        static void setNewStartupId( QWidget* window, const QCString& startup_id );

        /**
         * If your application shows temporarily some window during its startup,
         * for example a dialog, and only after closing it shows the main window,
         * startup notification would normally be shown while the dialog is visible.
         * To temporarily suspend and resume the notification, use this function.
         * Note that this is cumulative, i.e. after suspending twice, you have to
         * resume twice.
         * @since 3.2
         */
        static void silenceStartup( bool silence );

        /**
         * Creates and returns new startup id. The id includes properly setup
         * user timestamp.
         * @since 3.3
         */
        static QCString createNewStartupId();
	/**
	 *
	 */
	enum {
	    CleanOnCantDetect		= 1 << 0,
	    DisableKWinModule		= 1 << 1,
	    AnnounceSilenceChanges	= 1 << 2
	    };
	
	/**
	 * Creates an instance that will receive the startup notifications.
	 * The various flags passed may be
	 * @li CleanOnCantDetect - when a new unknown window appears, all startup
	 *     notifications for applications that are not compliant with
	 *     the startup protocol are removed
	 * @li DisableKWinModule - KWinModule, which is normally used to detect
	 *     new windows, is disabled. With this flag, checkStartup() must be
	 *     called in order to check newly mapped windows.
	 * @li AnnounceSilenceChanges - normally, startup notifications are
	 *     "removed" when they're silenced, and "recreated" when they're resumed.
	 *     With this flag, the change is normally announced with gotStartupChange().
	 *
	 * @param flags OR-ed combination of flags
	 * @param parent the parent of this QObject (can be 0 for no parent)
	 * @param name the name of the QObject (can be 0 for no name)
	 *
	 */
	KStartupInfo( int flags, QObject* parent = NULL, const char* name = NULL );
	/**
	 * Creates an instance that will receive the startup notifications.
	 *
	 * @param clean_on_cantdetect if true, and a new unknown window appears,
	 *  removes all notification for applications that are not compliant
	 *  with the app startup protocol
	 * @param parent the parent of this QObject (can be 0 for no parent)
	 * @param name the name of the QObject (can be 0 for no name)
	 *
	 * @obsolete
	 */
        KStartupInfo( bool clean_on_cantdetect, QObject* parent = 0, const char* name = 0 );
        virtual ~KStartupInfo();
	/**
	 * Sends given notification data about started application
	 * with the given startup identification. If no notification for this identification
	 * exists yet, it is created, otherwise it's updated. Note that the name field
         * in data is required.
	 * 
	 * @param id the id of the application
	 * @param data the application's data
	 * @return true if successful, false otherwise
	 * @see KStartupInfoId
	 * @see KStartupInfoData
	 */
        static bool sendStartup( const KStartupInfoId& id, const KStartupInfoData& data );

	/**
	 * Like sendStartup , uses dpy instead of qt_xdisplay() for sending the info.
	 * @param dpy the display of the application. Note that the name field
         * in data is required.
	 * @param id the id of the application
	 * @param data the application's data
	 * @return true if successful, false otherwise
	 */
        static bool sendStartupX( Display* dpy, const KStartupInfoId& id,
            const KStartupInfoData& data );

	/**
	 * Sends given notification data about started application
	 * with the given startup identification. This is used for updating the notification
	 * info, if no notification for this identification exists, it's ignored.
	 * @param id the id of the application
	 * @param data the application's data
	 * @return true if successful, false otherwise
	 * @see KStartupInfoId
	 * @see KStartupInfoData
	 */
        static bool sendChange( const KStartupInfoId& id, const KStartupInfoData& data );

	/**
	 * Like sendChange , uses dpy instead of qt_xdisplay() for sending the info.
	 * @param dpy the display of the application.
	 * @param id the id of the application
	 * @param data the application's data
	 * @return true if successful, false otherwise
	 */
        static bool sendChangeX( Display* dpy, const KStartupInfoId& id,
            const KStartupInfoData& data );

	/**
	 * Ends startup notification with the given identification.
	 * @param id the id of the application
	 * @return true if successful, false otherwise
	 */
        static bool sendFinish( const KStartupInfoId& id );

	/**
	 * Like sendFinish , uses dpy instead of qt_xdisplay() for sending the info.
	 * @param dpy the display of the application.
	 * @param id the id of the application
	 * @return true if successful, false otherwise
	 */
        static bool sendFinishX( Display* dpy, const KStartupInfoId& id );

	/**
	 * Ends startup notification with the given identification and the given data ( e.g.
	 * PIDs of processes for this startup notification that exited ).
	 * @param id the id of the application
	 * @param data the application's data
	 * @return true if successful, false otherwise
	 */
        static bool sendFinish( const KStartupInfoId& id, const KStartupInfoData& data );

	/**
	 * Like sendFinish , uses dpy instead of qt_xdisplay() for sending the info.
	 * @param dpy the display of the application.
	 * @param id the id of the application
	 * @param data the application's data
	 * @return true if successful, false otherwise
	 */
        static bool sendFinishX( Display* dpy, const KStartupInfoId& id,
            const KStartupInfoData& data );

	/**
	 * Returns the current startup notification identification for the current
	 * startup notification environment variable. Note that KApplication constructor
	 * unsets the variable and you have to use KApplication::startupId .
	 * @return the current startup notification identification
	 */
        static KStartupInfoId currentStartupIdEnv();
	/**
	 * Unsets the startup notification environment variable.
	 */
        static void resetStartupEnv();
	/**
	 * @li NoMatch    - the window doesn't match any existing startup notification
	 * @li Match      - the window matches an existing startup notification
	 * @li CantDetect - unable to detect if the window matches any existing
	 *			startup notification
	 */
        enum startup_t { NoMatch, Match, CantDetect };
	/**
	 * Checks if the given windows matches any existing startup notification.
	 * @param w the window id to check
	 * @return the result of the operation
	 */
        startup_t checkStartup( WId w );
	/**
	 * Checks if the given windows matches any existing startup notification, and
	 * if yes, returns the identification in id.
	 * @param w the window id to check
	 * @param id if found, the id of the startup notification will be written here
	 * @return the result of the operation
	 */
        startup_t checkStartup( WId w, KStartupInfoId& id );
	/**
	 * Checks if the given windows matches any existing startup notification, and
	 * if yes, returns the notification data in data.
	 * @param w the window id to check
	 * @param data if found, the data of the startup notification will be written here
	 * @return the result of the operation
	 */
        startup_t checkStartup( WId w, KStartupInfoData& data );
	/**
	 * Checks if the given windows matches any existing startup notification, and
	 * if yes, returns the identification in id and notification data in data.
	 * @param w the window id to check
	 * @param id if found, the id of the startup notification will be written here
	 * @param data if found, the data of the startup notification will be written here
	 * @return the result of the operation
	 */
        startup_t checkStartup( WId w, KStartupInfoId& id, KStartupInfoData& data );
	/**
	 * Sets the timeout for notifications, after this timeout a notification is removed.
	 * @param secs the new timeout in seconds
	 */
        void setTimeout( unsigned int secs );
	/**
	 * Sets the startup notification window property on the given window.
	 * @param window the id of the window
	 * @param id the startup notification id
	 */
        static void setWindowStartupId( WId window, const QCString& id );
	/**
	 * Returns startup notification identification of the given window.
	 * @param w the id of the window
	 * @return the startup notification id. Can be null if not found.
	 */
        static QCString windowStartupId( WId w );
        /**
         * @internal
         */
        static void handleAutoAppStartedSending();
	/**
	 * @internal
	 */
        class Data;
    signals:
	/**
	 * Emitted when a new startup notification is created (i.e. a new application is
	 * being started).
	 * @param id the notification identification
	 * @param data the notification data
	 */
        void gotNewStartup( const KStartupInfoId& id, const KStartupInfoData& data );
	/**
	 * Emitted when a startup notification changes.
	 * @param id the notification identification
	 * @param data the notification data
	 */
        void gotStartupChange( const KStartupInfoId& id, const KStartupInfoData& data );
	/**
	 * Emitted when a startup notification is removed (either because it was detected
	 * that the application is ready or because of a timeout).
	 * @param id the notification identification
	 * @param data the notification data
	 */
        void gotRemoveStartup( const KStartupInfoId& id, const KStartupInfoData& data );
    protected:
	/**
	 * 
	 */ 
	virtual void customEvent( QCustomEvent* e_P );
    private slots:
        void startups_cleanup();
        void startups_cleanup_no_age();
        void got_message( const QString& msg );
        void window_added( WId w );
	void slot_window_added( WId w );
    private:
        void init( int flags );
        friend class KStartupInfoPrivate;
        void got_startup_info( const QString& msg_P, bool update_only_P );
        void got_remove_startup_info( const QString& msg_P );
        void new_startup_info_internal( const KStartupInfoId& id_P,
            Data& data_P, bool update_only_P );
        void remove_startup_info_internal( const KStartupInfoId& id_P );
        void remove_startup_pids( const KStartupInfoId& id, const KStartupInfoData& data );
        void remove_startup_pids( const KStartupInfoData& data );
        startup_t check_startup_internal( WId w, KStartupInfoId* id, KStartupInfoData* data );
        bool find_id( const QCString& id_P, KStartupInfoId* id_O,
            KStartupInfoData* data_O );
        bool find_pid( pid_t pid_P, const QCString& hostname, KStartupInfoId* id_O,
            KStartupInfoData* data_O );
        bool find_wclass( QCString res_name_P, QCString res_class_P,
            KStartupInfoId* id_O, KStartupInfoData* data_O );
        static QCString get_window_hostname( WId w_P );
        void startups_cleanup_internal( bool age_P );
        void clean_all_noncompliant();
        static QString check_required_startup_fields( const QString& msg,
            const KStartupInfoData& data, int screen );
        bool clean_on_cantdetect_; // KDE4 remove unused
        unsigned int timeout;
        KStartupInfoPrivate* d;
    };

class KStartupInfoIdPrivate;

/**
 * Class representing an identification of application startup notification.
 *
 * Every existing notification about a starting application has its own unique
 * identification, that's used to identify and manipulate the notification.
 *
 * @see KStartupInfo
 * @see KStartupInfoData
 *
 * @author Lubos Lunak <l.lunak@kde.org>
 */
class KDECORE_EXPORT KStartupInfoId
    {
    public:
	/**
	 * Overloaded operator.
	 * @return true if the notification identifications are the same
	 */
        bool operator==( const KStartupInfoId& id ) const;
	/**
	 * Overloaded operator.
	 * @return true if the notification identifications are different
	 */
        bool operator!=( const KStartupInfoId& id ) const;
	/**
	 * Checks whether the identifier is valid.
	 * @return true if this object doesn't represent a valid notification identification
	 */
        bool none() const;
	/**
	 * Initializes this object with the given identification ( which may be also "0"
	 * for no notification ), or if "" is given, tries to read it from the startup
	 * notification environment variable, and if it's not set, creates a new one.
	 * @param id the new identification, "0" for no notification or "" to read
	 *           the environment variable
	 */
        void initId( const QCString& id = "" );
	/**
	 * Returns the notification identifier as string.
	 * @return the identification string for the notification
	 */
        const QCString& id() const;
        /**
         * Return the user timestamp for the startup notification, or 0 if no timestamp
         * is set.
         * @since 3.3
         */
        unsigned long timestamp() const;
	/**
	 * Sets the startup notification environment variable to this identification.
	 * @return true if successful, false otherwise
	 */
        bool setupStartupEnv() const;
	/**
	 * Creates an empty identification
	 */
        KStartupInfoId();
	/**
	 * Copy constructor.
	 */
        KStartupInfoId( const KStartupInfoId& data );
        ~KStartupInfoId();
        KStartupInfoId& operator=( const KStartupInfoId& data );
        bool operator<( const KStartupInfoId& id ) const;
    private:
        KStartupInfoId( const QString& txt );
        QString to_text() const;
        friend class KStartupInfo;
        KStartupInfoIdPrivate* d;
    };

class KStartupInfoDataPrivate;

/**
 * Class representing data about an application startup notification.
 *
 * Such data include the icon of the starting application, the desktop on which
 * the application should start, the binary name of the application, etc.
 *
 * @see KStartupInfo
 * @see KStartupInfoId
 *
 * @author Lubos Lunak <l.lunak@kde.org>
 */
class KDECORE_EXPORT KStartupInfoData
    {
    public:
	/**
	 * Sets the binary name of the application ( e.g. 'kcontrol' ).
	 * @param bin the new binary name of the application
	 */
        void setBin( const QString& bin );
	/**
	 * Returns the binary name of the starting application
	 * @return the new binary name of the application
	 */
        const QString& bin() const;
	/**
	 * Sets the name for the notification (e.g. 'Control Center')
	 */
        void setName( const QString& name );
	/**
	 * Returns the name of the startup notification. If it's not available,
         * it tries to use other information (binary name).
	 * @return the name of the startup notification
	 */
        const QString& findName() const;
	/**
	 * Returns the name of the startup notification, or empty if not available.
	 * @return the name of the startup notification, or an empty string
	 *         if not set.
	 */
        const QString& name() const;
	/**
	 * Sets the description for the notification (e.g. 'Launching Control Center').
         * I.e. name() describes what is being started, while description() is
         * the actual action performed by the starting.
         * @since 3.2
	 */
        void setDescription( const QString& descr );
	/**
	 * Returns the description of the startup notification. If it's not available,
         * it returns name().
	 * @return the description of the startup notification
         * @since 3.2
	 */
        const QString& findDescription() const;
	/**
	 * Returns the name of the startup notification, or empty if not available.
	 * @return the name of the startup notificaiton, or an empty string
	 *         if not set.
         * @since 3.2
	 */
        const QString& description() const;
	/**
	 * Sets the icon for the startup notification ( e.g. 'kcontrol' )
	 * @param icon the name of the icon
	 */
        void setIcon( const QString& icon );
	/**
	 * Returns the icon of the startup notification, and if it's not available,
	 * tries to get it from the binary name.
	 * @return the name of the startup notification's icon, or the name of
	 *         the binary if not set
	 */
        const QString& findIcon() const;
	/**
	 * Returns the icon of the startup notification, or empty if not available.
	 * @return the name of the icon, or an empty string if not set.
	 */
        const QString& icon() const;
	/**
	 * Sets the desktop for the startup notification ( i.e. the desktop on which
	 * the starting application should appear ).
	 * @param desktop the desktop for the startup notification
	 */
        void setDesktop( int desktop );
	/**
	 * Returns the desktop for the startup notification.
	 * @return the desktop for the startup notification
	 */
        int desktop() const;
	/**
	 * Sets a WM_CLASS value for the startup notification, it may be used for increasing
	 * the chance that the windows created by the starting application will be
	 * detected correctly.
	 * @param wmclass the WM_CLASS value for the startup notification
	 */
        void setWMClass( const QCString& wmclass );
	/**
	 * Returns the WM_CLASS value for the startup notification, or binary name if not
	 * available.
	 * @return the WM_CLASS value for the startup notification, or the binary name
	 *         if not set
	 */
        const QCString findWMClass() const;
	/**
	 * Returns the WM_CLASS value for the startup notification, or empty if not available.
	 * @return the WM_CLASS value for the startup notification, or empty
	 *         if not set
	 */
        const QCString& WMClass() const;
	/**
	 * Adds a PID to the list of processes that belong to the startup notification. It
	 * may be used to increase the chance that the windows created by the starting
	 * application will be detected correctly, and also for detecting if the application
	 * has quit without creating any window.
	 * @param pid the PID to add
	 */
        void addPid( pid_t pid );
	/**
	 * Returns all PIDs for the startup notification.
	 * @return the list of all PIDs
	 */
        const QValueList< pid_t >& pids() const;
	/**
	 * Checks whether the given @p pid is in the list of PIDs for starup
	 * notification.
	 * @return true if the given @p pid is in the list of PIDs for the startup notification
	 */
        bool is_pid( pid_t pid ) const;
	/**
	 * Sets the hostname on which the application is starting. It's necessary to set
	 * it if PIDs are set.
	 * @param hostname the application's hostname. If it's a null string, the current hostname is used
	 */
        void setHostname( const QCString& hostname = QCString());
	/**
	 * Returns the hostname for the startup notification.
	 * @return the hostname
	 */
        const QCString& hostname() const;
	
	/**
	 *
	 */
	enum TriState { Yes, No, Unknown };
	
	/**
	 * Sets whether the visual feedback for this startup notification
	 * should be silenced (temporarily suspended).
	 * @since 3.1.1
	 */
	void setSilent( TriState state );
	
	/**
	 * Return the silence status for the startup notification.
	 * @return KStartupInfoData::Yes if visual feedback is silenced
	 * @since 3.1.1
	 */
	TriState silent() const;
        
        /**
         * @obsolete Timestamp is already assigned in KStartupInfoId::initId().
         * Sets timestamp for the startup notification. The timestamp is expressed
         * as XServer time, and is used to prevent activation of the matching
         * window if user interaction took place after this timestamp.
         * Value -1 means no timestamp set, value 0 means that the window should
         * not be activated.
         */
        void setTimestamp( unsigned long time );
        
        /**
         * @obsolete Use KStartupInfoId::timestamp().
         * Return the timestamp for the startup notification, or -1 if no timestamp
         * is set.
         */
        unsigned long timestamp() const;
        
        /**
         * The X11 screen on which the startup notification is happening, -1 if unknown.
         */
        int screen() const;
        
        /**
         * Sets the X11 screen on which the startup notification should happen.
         * This is usually not necessary to set, as it's set by default to qt_xscreen().
         */
        void setScreen( int screen );

	/**
	 * Updates the notification data from the given data. Some data, such as the desktop
	 * or the name, won't be rewritten if already set.
	 * @param data the data to update
	 */
        void update( const KStartupInfoData& data );

	/**
	 * Constructor. Initializes all the data to their default empty values.
	 */
        KStartupInfoData();

	/**
	 * Copy constructor.
	 */
        KStartupInfoData( const KStartupInfoData& data );
        ~KStartupInfoData();
        KStartupInfoData& operator=( const KStartupInfoData& data );
    private:
        KStartupInfoData( const QString& txt );
        QString to_text() const;
        void remove_pid( pid_t pid );
        friend class KStartupInfo;
        friend class KStartupInfo::Data;
        KStartupInfoDataPrivate* d;
    };

#endif //Q_WS_X11

#endif
