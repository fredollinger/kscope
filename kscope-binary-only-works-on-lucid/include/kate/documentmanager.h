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

#ifndef _KATE_DOCMANAGER_INCLUDE_
#define _KATE_DOCMANAGER_INCLUDE_

#include <qobject.h>
#include <kurl.h>

namespace Kate
{
/** This interface provides access to the Kate Document Manager.
*/
class KDE_EXPORT DocumentManager : public QObject
{
  friend class PrivateDocumentManager;

  Q_OBJECT

  public:
    DocumentManager ( void *documentManager  );
    virtual ~DocumentManager ();

  public:
    /** Returns a pointer to the document indexed by n in the managers internal list.
    */
    class Document *document (uint n = 0);
    /** Returns a pointer to the currently active document or NULL if no document is open.
    */
    class Document *activeDocument ();
    /** Returns a pointer to the document with the given ID or NULL if no such document exists.
    */
    class Document *documentWithID (uint id);

    /** Returns the ID of the document located at url if such a document is known by the manager.
     */
    int findDocument (const KURL &url);
    /** Returns true if the document located at url is open, otherwise false.
     */
    bool isOpen (const KURL &url);

    /** returns the number of documents managed by this manager.
    */
    uint documents ();

    /** open a document and return a pointer to the document, if you specify a pointer != 0 to the id parameter
     * you will get the document id returned too
     */
    class Document *openURL(const KURL&url,const QString &encoding=QString::null,uint *id =0);
    /** close a document by pointer
     */
    bool closeDocument(class Document *document);
    /** close a document identified by the index
     */
    bool closeDocument(uint n = 0);
    /** close a document identified by the ID
     */
    bool closeDocumentWithID(uint id);
    /** close all documents
     */
    bool closeAllDocuments();

  #undef signals
  #define signals public
  signals:
  #undef signals
  #define signals protected

    /**
     * emitted if the current doc changes (there need not to be a active document)
     */
    void documentChanged ();
    
    /**
     * this document has now been created
     */
    void documentCreated (Kate::Document *document);
    
    /**
     * the document with this number was deleted
     */
    void documentDeleted (uint documentNumber);

  private:
    class PrivateDocumentManager *d;
};

/**
 * Returns the document manager object
 * @return DocumentManager document manager object
 */
KDE_EXPORT DocumentManager *documentManager ();

}

#endif
