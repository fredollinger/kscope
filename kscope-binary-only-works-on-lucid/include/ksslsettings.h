/* This file is part of the KDE project
 *
 * Copyright (C) 2000-2003 George Staikos <staikos@kde.org>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

#ifndef _KSSLSETTINGS_H
#define _KSSLSETTINGS_H

#include <qstring.h>
#include <qvaluelist.h>
#include <kconfig.h>

class KSSLSettingsPrivate;

/**
 * KDE SSL Settings
 *
 * This class contains some of the SSL settings for easy use.
 *
 * @author George Staikos <staikos@kde.org>
 * @see KSSL
 * @short KDE SSL Settings
 */
class KIO_EXPORT KSSLSettings {
public:
	/**
	 *  Construct a KSSL Settings object
	 *
	 *  @param readConfig read in the configuration immediately if true
	 */
	KSSLSettings(bool readConfig = true);

	/**
	 *  Destroy this KSSL Settings object
	 */
	~KSSLSettings();

	/**
	 *  Does the user allow SSLv2
	 *  @return true if the user allows SSLv2
	 */
	bool sslv2() const;

	/**
	 *  Does the user allow SSLv3
	 *  @return true if the user allows SSLv3
	 */
	bool sslv3() const;

	/**
	 *  Does the user allow TLSv1
	 *  @return true if the user allows TLSv1
	 */
	bool tlsv1() const;

	/**
	 *  Does the user want to be warned on entering SSL mode
	 *  @return true if the user wants to be warned
	 */
	bool warnOnEnter() const;

	/**
	 *  Change the user's warnOnEnter() setting
	 *  @since 3.3
	 *  @param x true if the user is to be warned
	 *  @see warnOnEnter
	 */
	void setWarnOnEnter(bool x);
	
	/**
	 *  Does the user want to be warned on sending unencrypted data
	 *  @return true if the user wants to be warned
	 *  @see setWarnOnUnencrypted
	 */
	bool warnOnUnencrypted() const;

	/**
	 *  Change the user's warnOnUnencrypted() setting
	 *  @param x true if the user is to be warned
	 *  @see warnOnUnencrypted
	 */
	void setWarnOnUnencrypted(bool x);

	/**
	 *  Does the user want to be warned on leaving SSL mode
	 *  @return true if the user wants to be warned
	 */
	bool warnOnLeave() const;

	/**
	 *  Change the user's warnOnLeave() setting
	 *  @since 3.3
	 *  @param x true if the user is to be warned
	 *  @see warnOnLeave
	 */
	void setWarnOnLeave(bool x);
	
	/**
	 *  Does the user want to be warned during mixed SSL/non-SSL mode
	 *  @return true if the user wants to be warned
	 */
	bool warnOnMixed() const;

	/**
	 *  Do not use this
	 *  @deprecated
	 */
	bool warnOnSelfSigned() const KDE_DEPRECATED;

	/**
	 *  Do not use this
	 *  @deprecated
	 */
	bool warnOnRevoked() const KDE_DEPRECATED;

	/**
	 *  Do not use this
	 *  @deprecated
	 */
	bool warnOnExpired() const KDE_DEPRECATED;

	/**
	 *  Does the user want to use the Entropy Gathering Daemon?
	 *  @return true if the user wants to use EGD
	 */
	bool useEGD() const;

	/**
	 *  Does the user want to use an entropy file?
	 *  @return true if the user wants to use an entropy file
	 */
	bool useEFile() const;
  
	/**
	 *  Change the user's TLSv1 preference
	 *  @param enabled true if TLSv1 is enabled
	 */
	void setTLSv1(bool enabled);
  
	/**
	 *  Change the user's SSLv2 preference
	 *  @param enabled true if SSLv2 is enabled
	 */
	void setSSLv2(bool enabled);
  
	/**
	 *  Change the user's SSLv3 preference
	 *  @param enabled true if SSLv3 is enabled
	 */
	void setSSLv3(bool enabled);

	/**
	 *  Does the user want X.509 client certificates to always be sent when
	 *  possible?
	 *  @return true if the user always wants a certificate sent
	 */
	bool autoSendX509() const;

	/**
	 *  Does the user want to be prompted to send X.509 client certificates
	 *  when possible?
	 *  @return true if the user wants to be prompted
	 */
	bool promptSendX509() const;

	/**
	 *  Get the OpenSSL cipher list for selecting the list of ciphers to
	 *  use in a connection.
	 *  @return the cipher list
	 */
	QString getCipherList();

	/**
	 *  Get the configured path to the entropy gathering daemon or entropy
	 *  file.
	 *  @return the path
	 */
	QString& getEGDPath();

	/**
	 *  Load the user's settings.
	 */
	void load();

	/**
	 *  Revert to default settings.
	 */
	void defaults();

	/**
	 *  Save the current settings.
	 */
	void save();

private:
	KConfig *m_cfg;
	bool m_bUseSSLv2, m_bUseSSLv3, m_bUseTLSv1;
	bool m_bWarnOnEnter, m_bWarnOnUnencrypted, m_bWarnOnLeave, m_bWarnOnMixed;
	bool m_bWarnSelfSigned, m_bWarnRevoked, m_bWarnExpired;

	QValueList<QString> v2ciphers, v2selectedciphers, v3ciphers, v3selectedciphers;
	QValueList<int> v2bits, v3bits;

	KSSLSettingsPrivate *d;
};


#endif

