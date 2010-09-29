/* This file is part of the KDE libraries
    Copyright (C) 2001 Carsten Pfeiffer <pfeiffer@kde.org>

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

#ifndef KSCAN_H
#define KSCAN_H

#include <kdialogbase.h>
#include <kinstance.h>
#include <klibloader.h>

class QImage;

/**
 * This is a base class for scanning dialogs. You can derive from this class
 * and implement your own dialog. An implementation is available in
 * kdegraphics/libkscan.
 *
 * Application developers that wish to add scanning support to their program
 * can use the static method @p KScanDialog::getScanDialog() to get an instance
 * of the user's preferred scanning dialog.
 *
 * Typical usage looks like this (e.g. in a slotShowScanDialog() method):
 *
 * \code
 * if ( !m_scanDialog ) {
 *     m_scanDialog = KScanDialog::getScanDialog( this, "scandialog" );
 *     if ( !m_scanDialog ) // no scanning support installed?
 *         return;
 *
 *     connect( m_scanDialog, SIGNAL( finalImage( const QImage&, int )),
 *              SLOT( slotScanned( const QImage&, int ) ));
 * }
 *
 * if ( m_scanDialog->setup() ) // only if scanner configured/available
 *     m_scanDialog->show();
 * \endcode
 *
 * This will create and show a non-modal scanning dialog. Connect to more
 * signals if you like.
 *
 * If you implement an own scan-dialog, you also have to implement a
 * KScanDialogFactory.
 *
 * @short A baseclass and accessor for Scanning Dialogs
 * @author Carsten Pfeiffer <pfeiffer@kde.org>
 */
class KIO_EXPORT KScanDialog : public KDialogBase
{
    Q_OBJECT

public:
    /**
     * Creates the user's preferred scanning dialog and returns it,
     * or 0L if no scan-support
     * is available. Pass a suitable @p parent widget, if you like. If you
     * don't you have to 'delete' the returned pointer yourself.
     * @param parent the QWidget's parent, or 0
     * @param name the name of the QObject, can be 0
     * @param modal if true the dialog is model
     * @return the KScanDialog, or 0 if the function failed
     */
    static KScanDialog * getScanDialog( QWidget *parent=0L,
					const char *name=0, bool modal=false );
    /**
     * Destructs the scan dialog.
     */
    ~KScanDialog();

    /**
     * Reimplement this if you need to set up some things, before showing the
     * dialog, e.g. to ask the user for the scanner device to use. If you
     * return false (e.g. there is no device available or the user aborted
     * device selection), the dialog will not be shown.
     *
     * @return true by default.
     */
    virtual bool setup();

protected:
    /**
     * Constructs the scan dialog. If you implement an own dialog, you can
     * customize it with the usual KDialogBase flags.
     *
     * @param dialogFace the KDialogBase::DialogType
     * @param buttonMask a ORed mask of all buttons (see
     * KDialogBase::ButtonCode)
     * @param parent the QWidget's parent, or 0
     * @param name the name of the QObject, can be 0
     * @param modal if true the dialog is model
     * @see KDialogBase
     */
    KScanDialog( int dialogFace=Tabbed, int buttonMask = Close|Help,
		 QWidget *parent=0L, const char *name=0, bool modal=false );

    /**
     * Returns the current id for an image. You can use that in your subclass
     * for the signals. The id is used in the signals to let people know
     * which preview and which text-recognition belongs to which scan.
     *
     * @return the current id for the image
     * @see nextId
     * @see finalImage
     * @see preview
     * @see textRecognized
     */
    int id() const { return m_currentId; }

    /**
     * Returns the id for the next image. You can use that in your subclass
     * for the signals.
     *
     * @return the id for the next image
     * @see id
     * @see finalImage
     * @see preview
     * @see textRecognized
     *
     */
    int nextId() { return ++m_currentId; }

signals:
    /**
     * Informs you that an image has been previewed.
     * @param img the image
     * @param id the image's id
     */
    void preview( const QImage &img, int id );

    /**
     * Informs you that an image has scanned. @p id is the same as in the
     * @p preview() signal, if this image had been previewed before.
     *
     * Note, that those id's may not be properly implemented in the current
     * libkscan.
     * @param img the image
     * @param id the image's id
     */
    void finalImage( const QImage &img, int id );

    /**
     * Informs you that the image with the id @p id has been run through
     * text-recognition. The text is in the QString parameter. In the future,
     * a compound document, using rich text will be used instead.
     *
     * @param text the text that has been recognized
     * @param id the id of the image
     */
    void textRecognized( const QString &text, int id );

private:
    int m_currentId;

protected:
    virtual void virtual_hook( int id, void* data );
private:
    class KScanDialogPrivate;
    KScanDialogPrivate *d;
};


/**
 * A factory for creating a KScanDialog. You need to reimplement
 * createDialog().
 * @short Factory for creating KScanDialogs
 */
class KIO_EXPORT KScanDialogFactory : public KLibFactory
{
public:
    virtual ~KScanDialogFactory();

    /**
     * Your library should reimplement this method to return your KScanDialog
     * derived dialog.
     * @param parent the QWidget's parent, or 0
     * @param name the name of the QObject, can be 0
     * @param modal if true the dialog is model
     */
    virtual KScanDialog * createDialog( QWidget *parent=0, const char *name=0,
					bool modal=false ) = 0;

protected:
    /**
     * Creates a new KScanDialogFactory.
     * @param parent the QWidget's parent, or 0
     * @param name the name of the QObject, can be 0
     */
    KScanDialogFactory( QObject *parent=0, const char *name=0 );

    virtual QObject* createObject( QObject* parent = 0, const char* name = 0,
                                   const char* classname = "QObject",
                                   const QStringList &args = QStringList() );


    /**
     * Creates a new instance with the given name.
     * @param instanceName the name of the instance
     */
    void setName( const QCString& instanceName ) {
	delete m_instance;
	m_instance = new KInstance( instanceName );
    }

    /**
     * Returns the instance.
     * @return the KInstance
     */
    KInstance *instance() const { return m_instance; }

private:
    KInstance *m_instance;
protected:
    virtual void virtual_hook( int id, void* data );
private:
    class KScanDialogFactoryPrivate* d;
};

/**
 * Base class for OCR Dialogs.
 */
class KIO_EXPORT KOCRDialog : public KDialogBase
{
    Q_OBJECT

public:
    /**
     * Creates the user's preferred OCR dialog and returns it,
     * or 0L if no OCR-support
     * is available. Pass a suitable @p parent widget, if you like. If you
     * don't you have to 'delete' the returned pointer yourself.
     * @param parent the QWidget's parent, or 0
     * @param name the name of the QObject, can be 0
     * @param modal if true the dialog is model
     * @return the KOCRDialog, or 0 if the function failed
     */
    static KOCRDialog * getOCRDialog( QWidget *parent=0L,
					const char *name=0, bool modal=false );
    ~KOCRDialog();

protected:
    /**
     * Constructs the OCR dialog. If you implement an own dialog, you can
     * customize it with the usual KDialogBase flags.
     *
     * @param dialogFace the KDialogBase::DialogType
     * @param buttonMask a ORed mask of all buttons (see
     * KDialogBase::ButtonCode)
     * @param parent the QWidget's parent, or 0
     * @param name the name of the QObject, can be 0
     * @param modal if true the dialog is model
     */
    KOCRDialog( int dialogFace=Tabbed, int buttonMask = Close|Help,
		 QWidget *parent=0L, const char *name=0, bool modal=false );

    /**
     * Returns the current id for an image. You can use that in your subclass
     * for the signals. The id is used in the signals to let people know
     * which text-recognition belongs to which scan.
     *
     * @return the current id for the image
     * @see nextId
     * @see textRecognized
     */
    int id() const { return m_currentId; }

    /**
     * Returns the id for the next image. You can use that in your subclass
     * for the signals.
     *
     * @return the id for the next image
     * @see id
     * @see textRecognized
     */
    int nextId() { return ++m_currentId; }

signals:
    /**
     * Informs you that the image with the id @p id has been run through
     * text-recognition. The text is in the QString parameter. In the future,
     * a compound document, using rich text will be used instead.
     *
     * @param text the text that has been recognized
     * @param id the id of the image
     */
    void textRecognized( const QString &text, int id );

private:
    int m_currentId;

protected:
    virtual void virtual_hook( int id, void* data );
private:
    class KOCRDialogPrivate;
    KOCRDialogPrivate *d;
};


/**
 * A factory for creating a KOCRDialog. You need to reimplement
 * createDialog().
 * @short Factory for creating KScanDialogs
 */
class KIO_EXPORT KOCRDialogFactory : public KLibFactory
{
public:
    virtual ~KOCRDialogFactory();

    /**
     * Your library should reimplement this method to return your KOCRDialog
     * derived dialog.
     * @param parent the QWidget's parent, or 0
     * @param name the name of the QObject, can be 0
     * @param modal if true the dialog is model
     */
    virtual KOCRDialog * createDialog( QWidget *parent=0, const char *name=0,
					bool modal=false ) = 0;

protected:
    /**
     * Creates a new KScanDialogFactory.
     * @param parent the QWidget's parent, or 0
     * @param name the name of the QObject, can be 0
     */
    KOCRDialogFactory( QObject *parent=0, const char *name=0 );

    virtual QObject* createObject( QObject* parent = 0, const char* name = 0,
                                   const char* className = "QObject",
                                   const QStringList &args = QStringList() );


    /**
     * Creates a new instance with the given name.
     * @param instanceName the name of the instance
     */
    void setName( const QCString& instanceName ) {
	delete m_instance;
	m_instance = new KInstance( instanceName );
    }

    /**
     * Returns the instance.
     * @return the KInstance
     */
    KInstance *instance() const { return m_instance; }

private:
    KInstance *m_instance;
protected:
    virtual void virtual_hook( int id, void* data );
private:
    class KOCRDialogFactory* d;
};


#endif // KSCAN_H
