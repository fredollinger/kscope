/* This file is part of the KDE project
   Copyright (C) 2002 Anders Lund <anders.lund@lund.tdcadsl.dk>

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

#ifndef _KATE_LISTBOX_VIEW_H_
#define _KATE_LISTBOX_VIEW_H_

#include "dockviewbase.h"

class KListBox;

namespace Kate
{

class ListboxView : public DockViewBase {
  Q_OBJECT
  public:
    ListboxView( QWidget *parent=0, const char *name=0 );
    ListboxView( const QString &titlePrefix, const QString &title, QWidget *parent=0, const char *name=0 );
    ~ListboxView();
    
    KListBox * listbox();
  
  private:
    class ListboxViewPrivate *d;
};

}

#endif // _KATE_LISTBOX_VIEW_H_
