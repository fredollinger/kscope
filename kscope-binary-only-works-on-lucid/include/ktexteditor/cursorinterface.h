/* This file is part of the KDE libraries
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

#ifndef __ktexteditor_cursorinterface_h__
#define __ktexteditor_cursorinterface_h__

#include <qptrlist.h>
#include <qstring.h>

#include <kdelibs_export.h>

namespace KTextEditor
{

/**
 * This class represents a text cursor.
 */
class KTEXTEDITOR_EXPORT Cursor
{
  public:
    virtual void position ( unsigned int *line, unsigned int *col ) const = 0;

    virtual bool setPosition ( unsigned int line, unsigned int col ) = 0;

    virtual bool insertText ( const QString& text ) = 0;

    virtual bool removeText ( unsigned int numberOfCharacters ) = 0;

    virtual QChar currentChar () const = 0;
};

/**
*  This is an interface for accessing the cursor of the Document class.
*/
class KTEXTEDITOR_EXPORT CursorInterface
{
  friend class PrivateCursorInterface;

  public:
    CursorInterface ();
    virtual ~CursorInterface ();

    unsigned int cursorInterfaceNumber () const;
    
  protected:  
    void setCursorInterfaceDCOPSuffix (const QCString &suffix);  

  public:
    /**
    * Create a new cursor object
    */
    virtual Cursor *createCursor ( ) = 0;

    /*
    * Accessor to the list of cursors.
    */
    virtual QPtrList<Cursor> cursors () const = 0;

    private:
      class PrivateCursorInterface *d;
      static unsigned int globalCursorInterfaceNumber;
      unsigned int myCursorInterfaceNumber;
};

KTEXTEDITOR_EXPORT CursorInterface *cursorInterface (class Document *doc);

}

#endif
