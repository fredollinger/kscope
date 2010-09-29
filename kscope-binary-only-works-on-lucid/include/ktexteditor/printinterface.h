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

#ifndef __ktexteditor_printinterface_h__
#define __ktexteditor_printinterface_h__

#include <kdelibs_export.h>

class QCString;

namespace KTextEditor
{

/**
*  This is an interface to printing functions of a Document.
*/
class KTEXTEDITOR_EXPORT PrintInterface
{
  friend class PrivatePrintInterface;
  
  public:
    PrintInterface ();
    virtual ~PrintInterface ();

    unsigned int printInterfaceNumber () const;
    
  protected:  
    void setPrintInterfaceDCOPSuffix (const QCString &suffix);  
    
  //
  // slots !!!
  //
  public:
    virtual bool printDialog () = 0;
    virtual bool print () = 0;
  
  private:
    class PrivatePrintInterface *d;
    static unsigned int globalPrintInterfaceNumber;
    unsigned int myPrintInterfaceNumber;
};

KTEXTEDITOR_EXPORT PrintInterface *printInterface (class Document *doc);

}

#endif
