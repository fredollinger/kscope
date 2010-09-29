// -*- c-basic-offset: 4; indent-tabs-mode:nil -*-
// vim: set ts=4 sts=4 sw=4 et:
/* This file is part of the KDE libraries
   Copyright (C) 2000 David Faure <faure@kde.org>

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
#ifndef __kbookmark_h
#define __kbookmark_h

#include <qstring.h>
#include <qvaluelist.h>
#include <qdom.h>
#include <kurl.h>

class KBookmarkManager;
class KBookmarkGroup;

class KIO_EXPORT KBookmark
{
    friend class KBookmarkGroup;
public:
    enum MetaDataOverwriteMode {
        OverwriteMetaData, DontOverwriteMetaData
    };

    KBookmark( ) {}
    KBookmark( QDomElement elem ) : element(elem) {}

    static KBookmark standaloneBookmark( const QString & text, const KURL & url, const QString & icon = QString::null );

    /**
     * Whether the bookmark is a group or a normal bookmark
     */
    bool isGroup() const;

    /**
     * Whether the bookmark is a separator
     */
    bool isSeparator() const;

    /**
     * @return true if this is a null bookmark. This will never
     * be the case for a real bookmark (in a menu), but it's used
     * for instance as the end condition for KBookmarkGroup::next()
     */
    bool isNull() const {return element.isNull();}

    /**
     * @return true if bookmark is contained by a QDomDocument,
     * if not it is most likely that it has become separated and
     * is thus invalid and/or has been deleted from the bookmarks.
     * @since 3.2
     */
    bool hasParent() const;

    /**
     * Text shown for the bookmark
     * If bigger than 40, the text is shortened by
     * replacing middle characters with "..." (see KStringHandler::csqueeze)
     */
    QString text() const;
    /**
     * Text shown for the bookmark, not truncated.
     * You should not use this - this is mainly for keditbookmarks.
     */
    QString fullText() const;
    /**
     * URL contained by the bookmark
     */
    KURL url() const;
    /**
     * @return the pixmap file for this bookmark
     * (i.e. the name of the icon)
     */
    QString icon() const;

    /**
     * @return the group containing this bookmark
     */
    KBookmarkGroup parentGroup() const;

    /**
     * Convert this to a group - do this only if
     * isGroup() returns true.
     */
    KBookmarkGroup toGroup() const;

    /**
     * Return the "address" of this bookmark in the whole tree.
     * This is used when telling other processes about a change
     * in a given bookmark. The encoding of the address is "/4/2", for
     * instance, to design the 2nd child inside the 4th child of the root bk.
     */
    QString address() const;

    // Hard to decide. Good design would imply that each bookmark
    // knows about its manager, so that there can be several managers.
    // But if we say there is only one manager (i.e. set of bookmarks)
    // per application, then KBookmarkManager::self() is much easier.
    //KBookmarkManager * manager() const { return m_manager; }

    /**
     * @internal for KEditBookmarks
     */
    QDomElement internalElement() const { return element; }

    /**
     * Updates the bookmarks access metadata
     * Call when a user accesses the bookmark
     * @since 3.2
     */
    void updateAccessMetadata();

    // Utility functions (internal)

    /**
     * @return address of parent
     */
    static QString parentAddress( const QString & address )
    { return address.left( address.findRev('/') ); }

    /**
     * @return position in parent (e.g. /4/5/2 -> 2)
     */
    static uint positionInParent( const QString & address )
    { return address.mid( address.findRev('/') + 1 ).toInt(); }

    /**
     * @return address of previous sibling (e.g. /4/5/2 -> /4/5/1)
     * Returns QString::null for a first child
     */
    static QString previousAddress( const QString & address )
    {
        uint pp = positionInParent(address);
        return pp>0 ? parentAddress(address) + '/' + QString::number(pp-1) : QString::null;
    }

    /**
     * @return address of next sibling (e.g. /4/5/2 -> /4/5/3)
     * This doesn't check whether it actually exists
     */
    static QString nextAddress( const QString & address )
    { return parentAddress(address) + '/' + QString::number(positionInParent(address)+1); }

    /**
     * @return the common parent of both addresses which 
     * has the greatest depth
     * @since 3.5
     */
     static QString commonParent(QString A, QString B);

    /**
     * Get the value of a specific metadata item.
     * @param key Name of the metadata item
     * @return Value of the metadata item. QString::null is returned in case
     * the specified key does not exist.
     * @since 3.4
     */
    QString metaDataItem( const QString &key ) const;

    /**
     * Change the value of a specific metadata item, or create the given item
     * if it doesn't exist already.
     * @param key Name of the metadata item to change
     * @param value Value to use for the specified metadata item
     * @param mode Whether to overwrite the item's value if it exists already or not.
     * @since 3.4
     */
    void setMetaDataItem( const QString &key, const QString &value, MetaDataOverwriteMode mode = OverwriteMetaData );

protected:
    QDomElement element;
    // Note: you can't add new member variables here.
    // The KBookmarks are created on the fly, as wrappers
    // around internal QDomElements. Any additional information
    // has to be implemented as an attribute of the QDomElement.

private:
    bool hasMetaData() const;
    static QString left(const QString & str, uint len);
};

/**
 * A group of bookmarks
 */
class KIO_EXPORT KBookmarkGroup : public KBookmark
{
public:
    /**
     * Create an invalid group. This is mostly for use in QValueList,
     * and other places where we need a null group.
     * Also used as a parent for a bookmark that doesn't have one
     * (e.g. Netscape bookmarks)
     */
    KBookmarkGroup();

    /**
     * Create a bookmark group as specified by the given element
     */
    KBookmarkGroup( QDomElement elem );

    /**
     * Much like KBookmark::address, but caches the
     * address into m_address.
     */
    QString groupAddress() const;

    /**
     * @return true if the bookmark folder is opened in the bookmark editor
     */
    bool isOpen() const;

    /**
     * Return the first child bookmark of this group
     */
    KBookmark first() const;
    /**
     * Return the prevous sibling of a child bookmark of this group
     * @param current has to be one of our child bookmarks.
     */
    KBookmark previous( const KBookmark & current ) const;
    /**
     * Return the next sibling of a child bookmark of this group
     * @param current has to be one of our child bookmarks.
     */
    KBookmark next( const KBookmark & current ) const;

    /**
     * Create a new bookmark folder, as the last child of this group
     * @param mgr the manager of the bookmark
     * @param text for the folder. If empty, the user will be queried for it.
     * @param emitSignal if true emit KBookmarkNotifier signal
     */
    KBookmarkGroup createNewFolder( KBookmarkManager* mgr, const QString & text = QString::null, bool emitSignal = true );
    /**
     * Create a new bookmark separator
     * Don't forget to use KBookmarkManager::self()->emitChanged( parentBookmark );
     */
    KBookmark createNewSeparator();

    /**
     * Create a new bookmark, as the last child of this group
     * Don't forget to use KBookmarkManager::self()->emitChanged( parentBookmark );
     * @param mgr the manager of the bookmark
     * @param bm the bookmark to add
     * @param emitSignal if true emit KBookmarkNotifier signal
     * @since 3.4
     */
    KBookmark addBookmark( KBookmarkManager* mgr, const KBookmark &bm, bool emitSignal = true );

    /**
     * Create a new bookmark, as the last child of this group
     * Don't forget to use KBookmarkManager::self()->emitChanged( parentBookmark );
     * @param mgr the manager of the bookmark
     * @param text for the bookmark
     * @param url the URL that the bookmark points to
     * @param icon the name of the icon to associate with the bookmark. A suitable default
     * will be determined from the URL if not specified.
     * @param emitSignal if true emit KBookmarkNotifier signal
     */
    KBookmark addBookmark( KBookmarkManager* mgr, const QString & text, const KURL & url, const QString & icon = QString::null, bool emitSignal = true );

    /**
     * Moves @p item after @p after (which should be a child of ours).
     * If item is null, @p item is moved as the first child.
     * Don't forget to use KBookmarkManager::self()->emitChanged( parentBookmark );
     */
    bool moveItem( const KBookmark & item, const KBookmark & after );

    /**
     * Delete a bookmark - it has to be one of our children !
     * Don't forget to use KBookmarkManager::self()->emitChanged( parentBookmark );
     */
    void deleteBookmark( KBookmark bk );

    /**
     * @return true if this is the toolbar group
     */
    bool isToolbarGroup() const;
    /**
     * @internal
     */
    QDomElement findToolbar() const;

    /**
     * @return the list of urls of bookmarks at top level of the group
     * @since 3.2
     */
    QValueList<KURL> groupUrlList() const;

protected:
    QDomElement nextKnownTag( QDomElement start, bool goNext ) const;

private:
    mutable QString m_address;
    // Note: you can't add other member variables here, except for caching info.
    // The KBookmarks are created on the fly, as wrappers
    // around internal QDomElements. Any additional information
    // has to be implemented as an attribute of the QDomElement.
};

/**
 * @since 3.2
 */
class KIO_EXPORT KBookmarkGroupTraverser {
protected:
    virtual ~KBookmarkGroupTraverser() { ; }
    void traverse(const KBookmarkGroup &);
    virtual void visit(const KBookmark &) { ; }
    virtual void visitEnter(const KBookmarkGroup &) { ; }
    virtual void visitLeave(const KBookmarkGroup &) { ; }
private:
    class KBookmarkGroupTraverserPrivate *d;
};

#endif
