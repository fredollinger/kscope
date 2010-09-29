/* This file is part of the KDE project
   Copyright (C) 2001 Christoph Cullmann <cullmann@kde.org>

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

#ifndef _KATE_VIEWMANAGER_INCLUDE_
#define _KATE_VIEWMANAGER_INCLUDE_

#include <qobject.h>
#include <kurl.h>

namespace Kate
{

class View;

/**
 * Interface to the viewmanager
 */
class KDE_EXPORT ViewManager : public QObject
{
  friend class PrivateViewManager;

  Q_OBJECT

  public:
    /**
     * Construtor, should not interest, internal usage
     */
    ViewManager (void *viewManager);
    
    /**
     * Desctructor
     */
    virtual ~ViewManager ();
      
  public slots: /*these are slots for kjs*/
    /**
     * Returns a pointer to the currently active view
     * @return View active view
     */
    Kate::View *activeView ();

    /**
     * Activates the view with the corresponding documentNumber
     * @param documentNumber the document's number
     */
    void activateView ( uint documentNumber );

    /**
     * Opens the file pointed to by URL
     * @param url url to the file
     */
    void openURL (const KURL &url);     
 
  #undef signals
  #define signals public
  signals:
  #undef signals
  #define signals protected   

    /**
     * Active view has changed
     */
    void viewChanged ();
    
  private:
    /**
     * REALLY PRIVATE ;)
     */
    class PrivateViewManager *d;
};

}

#endif
