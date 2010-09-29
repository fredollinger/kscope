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

#ifndef _KATE_MESSAGE_VIEW_H_
#define _KATE_MESSAGE_VIEW_H_

#include "dockviewbase.h"

/**
    A simple message view for Kate plugins.
    
    This is a message view for displaying output from processes.
    
    It uses a QTextEdit in _ mode to display the text. 
    
    You can use HTML links, for example to allow the user to go
    to a line in a precessed document.
    
    Connect to the linkClicked() signal to process the links.
    
    @section Usage
    
    To use it in the intended way:
    @li Create a KProcess (or derived class)
    @li As the output of the process arrives, hand it over
        using addText(). The view will add it to the end and
        make sure it is scrolled to the end.
    
    Each time you restart the process, clear() the view.
*/

class QTextBrowser;

namespace Kate
{

class MessageView : public DockViewBase {
  Q_OBJECT
  public:
    MessageView( QWidget *parent=0, const char *name=0 );
    ~MessageView();
    
  public slots:
    void addMessage( const QString &msg );
    void clear();

  signals: 
    void linkClicked( const QString & href );
        
  private:
    QTextBrowser *m_view;
};

}

#endif // _KATE_MESSAGE_VIEW_H_

