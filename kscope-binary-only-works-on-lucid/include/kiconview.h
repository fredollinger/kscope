/* This file is part of the KDE libraries
   Copyright (C) 1999 Torben Weis <weis@kde.org>

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
#ifndef KICONVIEW_H
#define KICONVIEW_H

#include <qcursor.h>
#include <qiconview.h>

#include <kdelibs_export.h>

/**
 * @short A variant of QIconView that honors KDE's system-wide settings.
 *
 * This Widget extends the functionality of QIconView to honor the system
 * wide settings for Single Click/Double Click mode, Auto Selection and
 * Change Cursor over Link.
 *
 * There is a new signal executed(). It gets connected to either
 * QIconView::clicked() or QIconView::doubleClicked() depending on the KDE
 * wide Single Click/Double Click settings. It is strongly recommended that
 * you use this signal instead of the above mentioned. This way you don�t
 * need to care about the current settings.
 * If you want to get informed when the user selects something connect to the
 * QIconView::selectionChanged() signal.
 *
 **/
class KDEUI_EXPORT KIconView : public QIconView
{
  friend class KIconViewItem;
  Q_OBJECT
  Q_ENUMS( Mode )
  Q_PROPERTY( Mode mode READ mode WRITE setMode )

public:
  KIconView( QWidget *parent = 0, const char *name = 0, WFlags f = 0 );

  ~KIconView();

  /**
   * KIconView has two different operating modes. Execute mode is depending
   * on the configuration of single-click or double-click where the signal
   * executed() will be emitted upon click/double-click.
   * In Select mode, this signal will not be emitted.
   *
   * Default is Execute mode.
   */
  enum Mode { Execute, Select };

  /**
   * Sets the mode to Execute or Select.
   * @li In Execute mode, the signal executed()
   * will be emitted when the user clicks/double-clicks an item.
   * @li Select mode is
   * the normal QIconView mode.
   *
   * Default is Execute.
   */
  void setMode( Mode m );

  /**
   * @returns the current Mode, either Execute or Select.
   */
  Mode mode() const;

  /**
   * Reimplemented for internal purposes
   */
  virtual void setFont( const QFont & );

   /**
    * Set the maximum number of lines that will be used to display icon text.
    * Setting this value will enable word-wrap, too.
    * @since 3.3
    *
    * @param n Number of lines
    */
  void setIconTextHeight( int n );

   /**
    * @return The height of icon text in lines
    * @since 3.3
    */
  int iconTextHeight() const;

  /**
   * Reimplemented for held() signal behavior internal purposes
   */
  virtual void takeItem( QIconViewItem * item );

signals:

  /**
   * This signal is emitted whenever the user executes an iconview item.
   * That means depending on the KDE wide Single Click/Double Click
   * setting the user clicked or double clicked on that item.
   * @param item is the pointer to the executed iconview item.
   *
   * Note that you may not delete any QIconViewItem objects in slots
   * connected to this signal.
   */
  void executed( QIconViewItem *item );

  /**
   * This signal is emitted whenever the user executes an iconview item.
   * That means depending on the KDE wide Single Click/Double Click
   * setting the user clicked or double clicked on that item.
   * @param item is the pointer to the executed iconview item.
   * @param pos is the position where the user has clicked
   *
   * Note that you may not delete any QIconViewItem objects in slots
   * connected to this signal.
   */
  void executed( QIconViewItem *item, const QPoint &pos );

  /**
   * This signal is emitted whenever the user hold something on an iconview
   * during a drag'n'drop.
   * @param item is the pointer to the iconview item the hold event occur.
   *
   * Note that you may not delete any QIconViewItem objects in slots
   * connected to this signal.
   */
  void held( QIconViewItem *item );

  /**
   * This signal gets emitted whenever the user double clicks into the
   * iconview.
   * @param item is the pointer to the clicked iconview item.
   * @param pos is the position where the user has clicked, and
   *
   * Note that you may not delete any QIconViewItem objects in slots
   * connected to this signal.
   *
   * This signal is more or less here for the sake of completeness.
   * You should normally not need to use this. In most cases it's better
   * to use executed() instead.
   */
  void doubleClicked( QIconViewItem *item, const QPoint &pos );

protected slots:
  void slotOnItem( QIconViewItem *item );
  void slotOnViewport();
  void slotSettingsChanged(int);

  /**
   * Auto selection happend.
   */
  void slotAutoSelect();

protected:
  void emitExecute( QIconViewItem *item, const QPoint &pos );
  void updateDragHoldItem( QDropEvent *e );

  virtual void focusOutEvent( QFocusEvent *fe );
  virtual void leaveEvent( QEvent *e );
  virtual void contentsMousePressEvent( QMouseEvent *e );
  virtual void contentsMouseDoubleClickEvent ( QMouseEvent * e );
  virtual void contentsMouseReleaseEvent( QMouseEvent *e );
  virtual void contentsDragEnterEvent( QDragEnterEvent *e );
  virtual void contentsDragLeaveEvent( QDragLeaveEvent *e );
  virtual void contentsDragMoveEvent( QDragMoveEvent *e );
  virtual void contentsDropEvent( QDropEvent* e );
  virtual void wheelEvent( QWheelEvent *e );

  /**
   * This method allows to handle correctly cases where a subclass
   * needs the held() signal to not be triggered without calling
   * a KIconView::contentsDrag*Event() method (which have side effects
   * because they forward to QIconView).
   */
  void cancelPendingHeldSignal();
  
private slots:
  void slotMouseButtonClicked( int btn, QIconViewItem *item, const QPoint &pos );
  void slotDragHoldTimeout();

private:
  /**
   * @internal. For use by KIconViewItem.
   */
  QFontMetrics *itemFontMetrics() const;
  /**
   * @internal. For use by KIconViewItem.
   */
  QPixmap selectedIconPixmap( QPixmap *pix, const QColor &col ) const;

  bool m_bUseSingle;
  bool m_bChangeCursorOverItem;

  QIconViewItem* m_pCurrentItem;

  QTimer* m_pAutoSelect;
  int m_autoSelectDelay;

protected:
  virtual void virtual_hook( int id, void* data );
private:
  class KIconViewPrivate;
  KIconViewPrivate *d;
};

class KWordWrap;
/**
 * @short A variant of QIconViewItem that wraps words better.
 *
 * KIconViewItem exists to improve the word-wrap functionality of QIconViewItem
 * Use KIconViewItem instead of QIconViewItem for any iconview item you might have :)
 *
 * @author David Faure <david@mandrakesoft.com>
 */
class KDEUI_EXPORT KIconViewItem : public QIconViewItem
{
public:
    // Need to redefine all the constructors - I want Java !
    KIconViewItem( QIconView *parent )
        : QIconViewItem( parent ) { init(); } // We need to call it because the parent ctor won't call our reimplementation :(((
    KIconViewItem( QIconView *parent, QIconViewItem *after )
        : QIconViewItem( parent, after ) { init(); }
    KIconViewItem( QIconView *parent, const QString &text )
        : QIconViewItem( parent, text ) { init(); }
    KIconViewItem( QIconView *parent, QIconViewItem *after, const QString &text )
        : QIconViewItem( parent, after, text ) { init(); }
    KIconViewItem( QIconView *parent, const QString &text, const QPixmap &icon )
        : QIconViewItem( parent, text, icon ) { init(); }
    KIconViewItem( QIconView *parent, QIconViewItem *after, const QString &text, const QPixmap &icon )
        : QIconViewItem( parent, after, text, icon ) { init(); }
    KIconViewItem( QIconView *parent, const QString &text, const QPicture &picture )
        : QIconViewItem( parent, text, picture ) { init(); }
    KIconViewItem( QIconView *parent, QIconViewItem *after, const QString &text, const QPicture &picture )
        : QIconViewItem( parent, after, text, picture ) { init(); }
    virtual ~KIconViewItem();

   /**
    * Using this function, you can specify a custom size for the pixmap. The
    * geometry of the item will be calculated to let a pixmap of the given size
    * fit in the iconView without needing an update.
    * This may be useful if you want to change the pixmap later without breaking
    * the layout. A possible use of this function is to replace a fileItem icon
    * by a larger pixmap (preview).
    * @since 3.3
    *
    * @param size The size to use
    */
    void setPixmapSize( const QSize& size );

   /**
    * @return The size set by setPixmapSize() or QSize( 0, 0 )
    * @since 3.3
    */
    QSize pixmapSize() const;

protected:
    void init();
    virtual void calcRect( const QString& text_ = QString::null );
    virtual void paintItem( QPainter *p, const QColorGroup &c );
    KWordWrap *wordWrap();
    void paintPixmap( QPainter *p, const QColorGroup &c );
    void paintText( QPainter *p, const QColorGroup &c );

private:
    KWordWrap* m_wordWrap;
    struct KIconViewItemPrivate;
    KIconViewItemPrivate *d;
};

#endif
