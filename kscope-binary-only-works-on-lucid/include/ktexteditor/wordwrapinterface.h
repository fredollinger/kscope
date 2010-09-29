/* This file is part of the KDE project
   Copyright (C) 2001 Christoph Cullmann (cullmann@kde.org)

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

#ifndef __ktexteditor_wordwrapinterface_h__
#define __ktexteditor_wordwrapinterface_h__

#include <kdelibs_export.h>

class QCString;

namespace KTextEditor
{

/**
*  This is an interface to control fixed-column word-wrapping of a Document.
*/
class KTEXTEDITOR_EXPORT WordWrapInterface
{
  friend class PrivateWordWrapInterface;
  
  public:
    WordWrapInterface ();
    virtual ~WordWrapInterface ();

    unsigned int wordWrapInterfaceNumber () const;
    
  protected:  
    void setWordWrapInterfaceDCOPSuffix (const QCString &suffix); 

  //
  // slots !!!
  //
  public:
    virtual void setWordWrap (bool) = 0;
    virtual bool wordWrap () = 0;

    virtual void setWordWrapAt (unsigned int ) = 0;
    virtual unsigned int wordWrapAt () = 0;

  private:
    class PrivateWordWrapInterface *d;
    static unsigned int globalWordWrapInterfaceNumber;
    unsigned int myWordWrapInterfaceNumber;
};
                          
KTEXTEDITOR_EXPORT WordWrapInterface *wordWrapInterface (class Document *doc);

}

#endif
