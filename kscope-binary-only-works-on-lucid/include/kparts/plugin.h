/* This file is part of the KDE project
   Copyright (C) 1999 Simon Hausmann <hausmann@kde.org>
             (C) 1999 David Faure <faure@kde.org>

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
#ifndef PLUGIN_H
#define PLUGIN_H

#include <qobject.h>
#include <kaction.h>
#include <kxmlguiclient.h>

class KInstance;

namespace KParts
{

/**
 * A plugin is the way to add actions to an existing KParts application,
 * or to a Part.
 *
 * The XML of those plugins looks exactly like of the shell or parts,
 * with one small difference: The document tag should have an additional
 * attribute, named "library", and contain the name of the library implementing
 * the plugin.
 *
 * If you want this plugin to be used by a part, you need to
 * install the rc file under the directory
 * "data" (KDEDIR/share/apps usually)+"/instancename/kpartplugins/"
 * where instancename is the name of the part's instance.
 *
 * You should also install a "plugin info" .desktop file with the same name.
 * \see PluginInfo
 */
class KPARTS_EXPORT Plugin : public QObject, virtual public KXMLGUIClient
{
    Q_OBJECT
public:
    /**
     * Struct holding information about a plugin
     */
    struct PluginInfo
    {
        QString m_relXMLFileName; ///< relative filename, i.e. kpartplugins/name
        QString m_absXMLFileName; ///< full path of most recent filename matching the relative filename
        QDomDocument m_document;
    };

    /**
     * Construct a new KParts plugin.
     */
    Plugin( QObject* parent = 0, const char* name = 0 );
    /**
     * Destructor.
     */
    virtual ~Plugin();

    /**
     * Reimplemented for internal reasons
     */
    virtual QString xmlFile() const;

    /**
     * Reimplemented for internal reasons
     */
    virtual QString localXMLFile() const;

    /**
     * Load the plugin libraries from the directories appropriate
     * to @p instance and make the Plugin objects children of @p parent.
     *
     * It is recommended to use the last loadPlugins method instead,
     * to support enabling and disabling of plugins.
     */
    static void loadPlugins( QObject *parent, const KInstance * instance );

    /**
     * Load the plugin libraries specified by the list @p docs and make the
     * Plugin objects children of @p parent .
     *
     * It is recommended to use the last loadPlugins method instead,
     * to support enabling and disabling of plugins.
     */
    static void loadPlugins( QObject *parent, const QValueList<PluginInfo> &pluginInfos );

    /**
     * Load the plugin libraries specified by the list @p pluginInfos, make the
     * Plugin objects children of @p parent, and use the given @p instance.
     *
     * It is recommended to use the last loadPlugins method instead,
     * to support enabling and disabling of plugins.
     */
    static void loadPlugins( QObject *parent, const QValueList<PluginInfo> &pluginInfos, const KInstance * instance );

    /**
     * Load the plugin libraries for the given @p instance, make the
     * Plugin objects children of @p parent, and insert the plugin as a child GUI client
     * of @p parentGUIClient.
     *
     * This method uses the KConfig object of the given instance, to find out which
     * plugins are enabled and which are disabled. What happens by default (i.e.
     * for new plugins that are not in that config file) is controlled by
     * @p enableNewPluginsByDefault. It can be overridden by the plugin if it
     * sets the X-KDE-PluginInfo-EnabledByDefault key in the .desktop file
     * (with the same name as the .rc file)
     *
     * If a disabled plugin is already loaded it will be removed from the GUI
     * factory and deleted.
     *
     * This method is automatically called by KParts::Plugin and by KParts::MainWindow.
     *
     * If you call this method in an already constructed GUI (like when the user
     * has changed which plugins are enabled) you need to add the new plugins to
     * the KXMLGUIFactory:
     * \code
     * if( factory() )
     * {
     *   QPtrList<KParts::Plugin> plugins = KParts::Plugin::pluginObjects( this );
     *   QPtrListIterator<KParts::Plugin> it( plugins );
     *   KParts::Plugin * plugin;
     *   while( ( plugin = it.current() ) != 0 )
     *   {
     *     ++it;
     *     factory()->addClient(  plugin );
     *   }
     * }
     * \endcode
     */
    static void loadPlugins( QObject *parent, KXMLGUIClient* parentGUIClient, KInstance* instance, bool enableNewPluginsByDefault = true );

    /**
     * Returns a list of plugin objects loaded for @p parent. This
     * functions basically calls the queryList method of
     * QObject to retrieve the list of child objects inheriting
     * KParts::Plugin .
     **/
    static QPtrList<Plugin> pluginObjects( QObject *parent );

protected:
    /**
     * Look for plugins in the @p instance's "data" directory (+"/kpartplugins")
     *
     * @return A list of QDomDocument s, containing the parsed xml documents returned by plugins.
     */
    static QValueList<Plugin::PluginInfo> pluginInfos( const KInstance * instance );

    /**
     * @internal
     * @return The plugin created from the library @p libname
     */
    static Plugin* loadPlugin( QObject * parent, const char* libname );

    virtual void setInstance( KInstance *instance );

private:
    static bool hasPlugin( QObject* parent, const QString& library );
    class PluginPrivate;
    PluginPrivate *d;
};

}

#endif
