/* This file is part of the KDE project
   Copyright (C) 2002 Joseph Wenninger <jowenn@kde.org>
   Copyright (C) 2002 Christoph Cullmann <cullmann@kde.org>

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

#ifndef _KATE_TOOLVIEWMANAGER_INCLUDE_
#define _KATE_TOOLVIEWMANAGER_INCLUDE_

#include <qwidget.h>
#include <kurl.h>

namespace Kate
{

/**
  Interface to the toolviewmanager
 */
class KDE_EXPORT ToolViewManager : public QObject
{
  friend class PrivateToolViewManager;

  Q_OBJECT

  public:
    /**
     * Construtor, should not interest, internal usage
     */
    ToolViewManager (void *toolViewManager);

    /**
     * Desctructor
     */
    virtual ~ToolViewManager ();

  public:
    /**
     * positions
     */
    enum Position {Left, Right, Top, Bottom};

    /**
     * add a given widget to the given sidebar if possible, name is very important
     * @param identifier unique identifier for this toolview
     * @param pos position for the toolview, if we are in session restore, this is only a preference
     * @param icon icon to use for the toolview
     * @param text text to use in addition to icon
     * @return created toolview on success or 0
     */
    QWidget *createToolView (const QString &identifier, ToolViewManager::Position pos, const QPixmap &icon, const QString &text);

    /**
     * Move the toolview
     * @param widget to show, widget given must be widget constructed by createToolView
     * @param pos position to move widget to
     * @return bool success
     */
    bool moveToolView (QWidget *widget, ToolViewManager::Position pos);

    /**
     * Show the toolview
     * @param widget to show, widget given must be widget constructed by createToolView
     * @return bool success
     */
    bool showToolView (QWidget *widget);

    /**
     * Hide the toolview
     * @param widget to hide, widget given must be widget constructed by createToolView
     * @return bool success
     */
    bool hideToolView (QWidget *widget);

  private:
    /**
     * REALLY PRIVATE ;)
     */
    class PrivateToolViewManager *d;
};

}

#endif
