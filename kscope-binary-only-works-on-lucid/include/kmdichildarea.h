//----------------------------------------------------------------------------
//    filename             : kmdichildarea.h
//----------------------------------------------------------------------------
//    Project              : KDE MDI extension
//
//    begin                : 07/1999       by Szymon Stefanek as part of kvirc
//                                         (an IRC application)
//    changes              : 09/1999       by Falk Brettschneider to create an
//                           - 06/2000     stand-alone Qt extension set of
//                                         classes and a Qt-based library
//                           2000-2003     maintained by the KDevelop project
//
//    copyright            : (C) 1999-2003 by Falk Brettschneider
//                                         and
//                                         Szymon Stefanek (stefanek@tin.it)
//    email                :  falkbr@kdevelop.org (Falk Brettschneider)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
//
//    This program is free software; you can redistribute it and/or modify
//    it under the terms of the GNU Library General Public License as
//    published by the Free Software Foundation; either version 2 of the
//    License, or (at your option) any later version.
//
//----------------------------------------------------------------------------

#ifndef _KMDI_CHILD_AREA_H_
#define _KMDI_CHILD_AREA_H_

#include <qframe.h>
#include <qptrlist.h>

#include "kmdichildfrm.h"
#include "kmdichildview.h"

class KMdiChildAreaPrivate;

/**
  * @short Internal class.
  *
  * The main frame widget KMdiMainFrm consists of 2 child widgets. One is this class.
  * It's the widget where the child frames (emulated toplevel MDI views) live in.
  * This class is the manager for the child frame widgets because it controls the
  * Z-order widget stack of KMdiChildFrm's.
  * It provides all placing and positioning algorithms for docked (attached) MDI views.
  *
  * KMdiChildArea doesn't know anything about the actual MDI views. It only knows
  * and manages the frame widgets of attached MDI views.
  * All actions and stuff concerning only to childframes are handled here.
  */
class KMDI_EXPORT KMdiChildArea : public QFrame
{
	friend class KMdiChildFrmCaption;
	friend class KMdiChildFrm;

Q_OBJECT

	// attributes
public:
	/**
	 * Z Order stack of KMdiChildFrm childframe windows (top=last)
	 */
	QPtrList<KMdiChildFrm> *m_pZ; //Auto delete enabled

	/**
	 * the default size of an newly created childframe
	 */
	QSize m_defaultChildFrmSize;
protected:
	/**
	 * The MDI childframe window caption font
	 */
	QFont m_captionFont;

	/**
	 * The foreground color of the active MDI childframe window caption
	 */
	QColor m_captionActiveBackColor;

	/**
	 * The background color of the active MDI childframe window captions
	 */
	QColor m_captionActiveForeColor;

	/**
	 * The foreground color of inactive MDI childframe window captions
	 */
	QColor m_captionInactiveBackColor;

	/**
	 * The background color of inactive MDI childframe window captions
	 */
	QColor m_captionInactiveForeColor;

	int m_captionFontLineSpacing;

	// methods
public:

	/**
	 * Consruction. Note: This class needn't to know about KMdiMainFrm .
	 */
	KMdiChildArea( QWidget *parent );

	/**
	 * Destructor : THERE should be no child windows anymore...
	 * Howewer it simply deletes all the child widgets :)
	 */
	~KMdiChildArea();

	/**
	 * Appends a new KMdiChildFrm to this manager.
	 * The child is shown,raised and gets focus if this window has it.
	 */
	void manageChild( KMdiChildFrm *lpC, bool bShow = true, bool bCascade = true );

	/**
	 * Destroys a managed KMdiChildFrm
	 * Also deletes the client attached to this child.
	 */
	void destroyChild( KMdiChildFrm* child, bool focusTopChild = true );

	/**
	 * Destroys a managed KMdiChildFrm
	 * Clients attached to the KMdiChildFrm are not deleted.
	 */
	void destroyChildButNotItsView( KMdiChildFrm *lpC, bool bFocusTopChild = true );

	/**
	 * Brings the child to the top of the stack
	 * The child is focused if setFocus is true. If setFocus is false, the
	 * child is just raised.
	 */
	void setTopChild( KMdiChildFrm* child, bool setFocus = false );

	/**
	 * Returns the topmost child (the active one) or 0 if there are no children.
	 * Note that the topmost child may be also hidded , if ALL the windows are minimized.
	 */
	inline KMdiChildFrm * topChild() const { return m_pZ->last(); }

	/**
	 * Returns the number of visible children
	 */
	int getVisibleChildCount() const;

	/**
	 * Calculates the cascade point for the given index. If index is -1
	 * the cascade point is calculated for the window following the last window
	 * @param indexOfWindow the index of the window in relation the z-ordered window list
	 */
	QPoint getCascadePoint( int indexOfWindow = -1 );

	/**
	 * Sets the MDI childframe window caption font
	 * A relayout does not occur when using this function
	 */
	void setMdiCaptionFont( const QFont &fnt );

	/**
	 * Sets the foreground color of the active MDI childframe window caption
	 * A relayout does not occur when using this function
	 */
	void setMdiCaptionActiveForeColor( const QColor &clr );

	/**
	 * Sets the background color of the active MDI childframe window captions
	 * A relayout does not occur when using this function
	 */
	void setMdiCaptionActiveBackColor( const QColor &clr );

	/**
	 * Sets the foreground color of inactive MDI childframe window captions
	 * A relayout does not occur when using this function
	 */
	void setMdiCaptionInactiveForeColor( const QColor &clr );

	/**
	 * Sets the background color of inactive MDI childframe window captions
	 * A relayout does not occur when using this function
	 */
	void setMdiCaptionInactiveBackColor( const QColor &clr );

	/**
	 * Gets all caption colors, consistent with current WM settings
	 * (or other Desktop settings e.g. system settings for win32)
	 * This method is useful not only for KMDI child windows.
	 * Colors are returned via activeBG, activeFG, inactiveBG, inactiveFG references.
	 *
	 * @deprecated Use KGlobalSettings::activeTitleColor(), KGlobalSettings::activeTextColor(),
	 * KGlobalSettings::inactiveTitleColor() and KGlobalSettings::inactiveTextColor() instead.
	 */
	static void getCaptionColors( const QPalette &pal, QColor &activeBG, QColor &activeFG,
	                              QColor &inactiveBG, QColor &inactiveFG ) KDE_DEPRECATED;

public slots:
	/**
	 * Cascades all windows resizing them to the minimum size.
	 */
	void cascadeWindows();

	/**
	 * Cascades all windows resizing them to the maximum available size.
	 */
	void cascadeMaximized();

	/**
	 * Maximize all windows but only in vertical direction
	 */
	void expandVertical();

	/**
	 * Maximize all windows but only in horizontal direction
	 */
	void expandHorizontal();

	/**
	 * Gives focus to the topmost child if it doesn't get focus
	 * automatically or you want to wait to give it focus
	 */
	void focusTopChild();

	/**
	 * Tile Pragma
	 */
	void tilePragma();

	/**
	 * Tile Anodine
	 */
	void tileAnodine();

	/**
	 * Tile all the windows in the child area vertically
	 */
	void tileVertically();

	/**
	 * Position and layout the minimized child frames
	 */
	void layoutMinimizedChildren();

protected:

	/**
	 * Internally used for the tile algorithm
	 */
	void tileAllInternal( int maxWnds );

	/**
	 * Automatically resizes a maximized MDI view and layouts the positions of minimized MDI views.
	 */
	virtual void resizeEvent( QResizeEvent * );

	/**
	 * Shows the 'Window' popup menu on right mouse button click
	 */
	void mousePressEvent( QMouseEvent *e );

	/**
	 * Internally used. Actions that are necessary when an MDI view gets minimized
	 */
	void childMinimized( KMdiChildFrm *lpC, bool bWasMaximized );

signals:
	/**
	 * Signals that there aren't maximized child frames any more
	 */
	void noMaximizedChildFrmLeft( KMdiChildFrm* );

	/**
	 * Signals that the child frames are maximized now
	 */
	void nowMaximized( bool );

	/**
	 * Signals a KMdiMainFrm that the signal/slot connections of the system buttons in the
	 * menubar (only in Maximize mode) must be updated to another MDI view because the focused
	 * MDI view has changed
	 * @internal
	 */
	void sysButtonConnectionsMustChange( KMdiChildFrm*, KMdiChildFrm* );

	/**
	 * Signals a KMdiMainFrm that the 'Window' popup menu must be shown
	 * @internal
	 */
	void popupWindowMenu( QPoint );

	/**
	 * Signals that the last attached (docked) MDI view has been closed.
	 * Note: Detached MDI views can remain.
	 */
	void lastChildFrmClosed();

private:

	KMdiChildAreaPrivate *d;
};

#endif   // _KMDICHILDAREA_H_

// kate: indent-mode csands; tab-width 4; auto-insert-doxygen on;
