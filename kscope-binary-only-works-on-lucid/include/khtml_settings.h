/* This file is part of the KDE project
   Copyright (C) 1999 David Faure <faure@kde.org>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#ifndef __konq_htmlsettings_h__
#define __konq_htmlsettings_h__

class KConfig;
#include <qcolor.h>
#include <qstring.h>
#include <qstringlist.h>
#include <qfont.h>
#include <qmap.h>

#include <kdelibs_export.h>

struct KPerDomainSettings;
class KHTMLSettingsPrivate;

/**
 * Settings for the HTML view.
 */
class KHTML_EXPORT KHTMLSettings
{
public:

    /**
     * This enum specifies whether Java/JavaScript execution is allowed.
     */
    enum KJavaScriptAdvice {
	KJavaScriptDunno=0,
	KJavaScriptAccept,
	KJavaScriptReject
    };

    enum KAnimationAdvice {
        KAnimationDisabled=0,
        KAnimationLoopOnce,
        KAnimationEnabled
    };

    /**
     * This enum specifies the policy for window.open
     */
    enum KJSWindowOpenPolicy {
    	KJSWindowOpenAllow=0,
    	KJSWindowOpenAsk,
    	KJSWindowOpenDeny,
    	KJSWindowOpenSmart
    };

    /**
     * This enum specifies the policy for window.status and .defaultStatus
     */
    enum KJSWindowStatusPolicy {
    	KJSWindowStatusAllow=0,
    	KJSWindowStatusIgnore
    };

    /**
     * This enum specifies the policy for window.moveBy and .moveTo
     */
    enum KJSWindowMovePolicy {
    	KJSWindowMoveAllow=0,
    	KJSWindowMoveIgnore
    };

    /**
     * This enum specifies the policy for window.resizeBy and .resizeTo
     */
    enum KJSWindowResizePolicy {
    	KJSWindowResizeAllow=0,
    	KJSWindowResizeIgnore
    };

    /**
     * This enum specifies the policy for window.focus
     */
    enum KJSWindowFocusPolicy {
    	KJSWindowFocusAllow=0,
    	KJSWindowFocusIgnore
    };

    /**
     * @internal Constructor
     */
    KHTMLSettings();
    KHTMLSettings(const KHTMLSettings &other);

    /**
     * Called by constructor and reparseConfiguration
     */
    void init();

    /** Read settings from @p config.
     * @param config is a pointer to KConfig object.
     * @param reset if true, settings are always set; if false,
     *  settings are only set if the config file has a corresponding key.
     */
    void init( KConfig * config, bool reset = true );

    /**
     * Destructor. Don't delete any instance by yourself.
     */
    virtual ~KHTMLSettings();

    // Behavior settings
    bool changeCursor() const;
    bool underlineLink() const;
    bool hoverLink() const;
    bool allowTabulation() const;
    bool autoSpellCheck() const;
    KAnimationAdvice showAnimations() const;

    // Font settings
    QString stdFontName() const;
    QString fixedFontName() const;
    QString serifFontName() const;
    QString sansSerifFontName() const;
    QString cursiveFontName() const;
    QString fantasyFontName() const;

    // these two can be set. Mainly for historical reasons (the method in KHTMLPart exists...)
    void setStdFontName(const QString &n);
    void setFixedFontName(const QString &n);

    int minFontSize() const;
    int mediumFontSize() const;

    bool jsErrorsEnabled() const;
    void setJSErrorsEnabled(bool enabled);

    const QString &encoding() const;

    bool followSystemColors() const;

    // Color settings
    const QColor& textColor() const;
    const QColor& baseColor() const;
    const QColor& linkColor() const;
    const QColor& vLinkColor() const;

    // Autoload images
    bool autoLoadImages() const;
    bool unfinishedImageFrame() const;

    bool isOpenMiddleClickEnabled();
    bool isBackRightClickEnabled();

    // Java and JavaScript
    // ### BIC make these const
    bool isJavaEnabled( const QString& hostname = QString::null );
    bool isJavaScriptEnabled( const QString& hostname = QString::null );
    bool isJavaScriptDebugEnabled( const QString& hostname = QString::null );
    bool isJavaScriptErrorReportingEnabled( const QString& hostname = QString::null ) const;
    bool isPluginsEnabled( const QString& hostname = QString::null );

    // AdBlocK Filtering
    bool isAdFiltered( const QString &url ) const;
    bool isAdFilterEnabled() const;
    bool isHideAdsEnabled() const;
    void addAdFilter( const QString &url );

    // Access Keys
    bool accessKeysEnabled() const;

    KJSWindowOpenPolicy windowOpenPolicy( const QString& hostname = QString::null ) const;
    KJSWindowMovePolicy windowMovePolicy( const QString& hostname = QString::null ) const;
    KJSWindowResizePolicy windowResizePolicy( const QString& hostname = QString::null ) const;
    KJSWindowStatusPolicy windowStatusPolicy( const QString& hostname = QString::null ) const;
    KJSWindowFocusPolicy windowFocusPolicy( const QString& hostname = QString::null ) const;

    // helpers for parsing domain-specific configuration, used in KControl module as well
    static KJavaScriptAdvice strToAdvice(const QString& _str);
    static void splitDomainAdvice(const QString& configStr, QString &domain,
				  KJavaScriptAdvice &javaAdvice, KJavaScriptAdvice& javaScriptAdvice);
    static const char* adviceToStr(KJavaScriptAdvice _advice);

    /** reads from @p config's current group, forcing initialization
      * if @p reset is true.
      * @param config is a pointer to KConfig object.
      * @param reset true if initialization is to be forced.
      * @param global true if the global domain is to be read.
      * @param pd_settings will be initialised with the computed (inherited)
      *		settings.
      */
    void readDomainSettings(KConfig *config, bool reset,
			bool global, KPerDomainSettings &pd_settings);

    QString settingsToCSS() const;
    static const QString &availableFamilies();

    QString userStyleSheet() const;

    // Form completion
    bool isFormCompletionEnabled() const;
    int maxFormCompletionItems() const;

    // Meta refresh/redirect (http-equiv)
    bool isAutoDelayedActionsEnabled () const;

    QValueList< QPair< QString, QChar > > fallbackAccessKeysAssignments() const;

    // Whether to show passive popup when windows are blocked
    // @since 3.5
    void setJSPopupBlockerPassivePopup(bool enabled);
    bool jsPopupBlockerPassivePopup() const;

private:
    friend class KHTMLFactory;
    QString lookupFont(int i) const;

    KHTMLSettingsPrivate *d;
    static QString *avFamilies;
};

#endif
