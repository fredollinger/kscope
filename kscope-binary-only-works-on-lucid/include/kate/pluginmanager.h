/* This file is part of the KDE project
   Copyright (C) 2001 Christoph Cullmann <cullmann@kde.org>
   Copyright (C) 2002 Joseph Wenninger <jowenn@kde.org>

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

#ifndef _KATE_PLUGINMANAGER_INCLUDE_
#define _KATE_PLUGINMANAGER_INCLUDE_

#include <qobject.h>
#include <kurl.h>

namespace Kate
{
/** This interface provides access to the Kate Plugin Manager.
*/
class KDE_EXPORT PluginManager : public QObject
{
  friend class PrivatePluginManager;

  Q_OBJECT

  public:
    PluginManager ( void *pluginManager  );
    virtual ~PluginManager ();

  public:
    /** if the plugin with the library name "name" is loaded, a pointer to that plugin is returned */
    class Plugin *plugin(const QString &name);

    /** return true, if plugin is known to kate (either loaded or not loaded)
     *
     * This method is not used yet
     */
    bool pluginAvailable(const QString &name);

    /** tries loading the specified plugin and returns a pointer to the new plugin or 0
     *  if permanent is true (default value) the plugin will be loaded at the next kate startup
     *
     * This method is not used yet
     */
    class Plugin *loadPlugin(const QString &name,bool permanent=true);

    /** unload the specified plugin. If the value permanent is true (default value), the plugin will not be
     * loaded on kate's next startup. Even if it had been loaded with permanent=true.
     *
     * This method is not used yet
     */
    void unloadPlugin(const QString &name,bool permanent=true);

  private:
    class PrivatePluginManager *d;
};

}

#endif
