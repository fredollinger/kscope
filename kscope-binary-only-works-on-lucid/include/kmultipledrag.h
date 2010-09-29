/* This file is part of the KDE project
   Copyright (C) 2001 David Faure <faure@kde.org>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License as published by the Free Software Foundation; either
   version 2 of the License.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this program; see the file COPYING.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#ifndef KMULTIPLEDRAG_H
#define KMULTIPLEDRAG_H

#ifndef QT_NO_DRAGANDDROP

#include <qdragobject.h>
#include <qvaluelist.h>
#include "kdelibs_export.h"

class KMultipleDragPrivate;
/**
 * This class makes it easy for applications to provide a drag object
 * (for drag-n-drop or for clipboard) that has several representations
 * of the same data, under different formats.
 *
 * Instead of creating a specific class for each case (as would otherwise
 * be necessary), you can simply create independent drag objects (e.g.
 * a QImageDrag object and a KURLDrag object), and bundle them together
 * using KMultipleDrag.
 *
 * Sample code for this:
 *
 * \code
 * KMultipleDrag *drag = new KMultipleDrag( parentWidget );
 * drag->addDragObject( new QImageDrag( someQImage, 0 ) );
 * drag->addDragObject( new KURLDrag( someKURL, 0 ) );
 * drag->drag();
 * \endcode
 *
 * Note that the drag objects added to the multiple drag become owned by it.
 * For that reason their parent should be 0.
 *
 * @author David Faure <faure@kde.org>
 */
class KDECORE_EXPORT KMultipleDrag : public QDragObject
{
    Q_OBJECT

public:
    /**
     * Create a new KMultipleDrag object.
     * @param dragSource the parent object which is the source of the data,
     *                   0 for a parent-less object
     * @param name the name of the object, can be 0
     */
    KMultipleDrag( QWidget *dragSource = 0, const char *name = 0 );

    /**
     * Call this to add each underlying drag object to the multiple drag object.
     * The drag object should not have a parent because the multiple drag object
     * will own it.
     *
     * @param dragObject the drag object to add. Should have no parent object.
     */
    void addDragObject( QDragObject *dragObject );

    /**
     * Returns the data of a drag object with that supports the given
     * mime type.
     * @param mime the mime type to search
     * @return the data, or a null byte array if not found
     * @reimp
     */
    virtual QByteArray encodedData( const char *mime ) const;

    /**
     * Returns the @p i'th supported format, or 0.
     * @param i the number of the format to check
     * @return the format with the number @p i, or 0 otherwise
     * @reimp
     */
    virtual const char* format( int i ) const;

protected:
// KDE4: make private
    QPtrList<QDragObject> m_dragObjects;
    QValueList<int> m_numberFormats;
protected:
    virtual void virtual_hook( int id, void* data );
private:
    KMultipleDragPrivate* d;
};

#endif // QT_NO_DRAGANDDROP

#endif // KMULTIPLEDRAG_H
