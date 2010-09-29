/* This file is part of the KDE libraries
   Copyright (C) 2004 Joseph Wenninger <jowenn@kde.org>

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

#ifndef __ktexteditor_templateinterface_h__
#define __ktexteditor_templateinterface_h__

#include <qstring.h>
#include <qmap.h>
#include <qwidget.h>

#include <kdelibs_export.h>

namespace KTextEditor
{

class Document;

/**
 * This is an interface for inserting template strings with user editable
 * fields into a document.
 */
class KTEXTEDITOR_EXPORT TemplateInterface //should be named AbstractTemplateInterface, but for consistency with the other classes it is not (for the 3.x release series)
{
  friend class PrivateTemplateInterface;

  public:
    TemplateInterface();
    virtual ~TemplateInterface();

    /**
     * Parses @p templateString for macros in the form [$%]{NAME} and finds
     * the value corresponding to NAME if any. The NAME string may contain
     * any non-whitespace character execpt '}'
     * @param initialValues a map with the keys for the macros to expand. 
     * keys with a value are ignored.
     * @param parentWindow is used if dialogs have to be shown
     * @return true if all macros was sucessfully expanded
     * @see insertTemplateText for a list of supported macros
     */
    static bool expandMacros( QMap<QString, QString> &initialValues, QWidget *parentWindow );

    uint templateInterfaceNumber () const;

  protected:
    void setTemplateInterfaceDCOPSuffix (const QCString &suffix);

  public:

    /**
     * Inserts an interactive ediable template text at line "line", column "col".
     * @p parentWindow is used if dialogs have to be shown
     * @return true if inserting the string succeeded
     *
     * Use insertTemplateText(numLines(), ...) to append text at end of document
     * Template  strings look like
     * "for( int ${index}=0;${index}<10;${index}++) { ${cursor} };"
     * or "%{date}"
     *
     * This syntax is somewhat similiar to the one found in the Eclipse editor.
     *
     * There are certain common placeholders (macros), which get assigned a
     * default initialValue, If the second parameter does not a given value.
     * For all others the initial value is the name of the placeholder.
     *
     * Placeholder names may only consist of a-zA-Z0-9_
     * Common placeholders and values are
     *
     * - index: "i"
     * - loginname: The current users's loginname
     * - firstname: The current user's first name retrieved from kabc
     * - lastname: The current user's last name retrieved from kabc
     * - fullname: The current user's first and last name retrieved from kabc
     * - email: The current user's primary email adress  retrieved from kabc
     * - date: current date
     * - time: current time
     * - year: current year
     * - month: current month
     * - day: current day
     * - hostname: hostname of the computer
     * - cursor: at this position the cursor will be after editing of the
     *   template has finished, this has to be taken care of by the actual
     *   implementation. The placeholder gets a value of "|" assigned.
     *
     * If a macro is started with a % (persent sign) like "%{date}" it isn't added
     * to the list editable strings ( for example TAB key navigation) if a value
     * differing from the macro name is found.
     *
     * If the editor supports some kind of smart indentation, the inserted code
     * should be layouted by the indenter.
     */
    bool insertTemplateText ( uint line, uint column, const QString &templateString, const QMap<QString,QString> &initialValues, QWidget *parentWindow=0);

protected:
    /**
     * You must implement this, it is called by insertTemplateText, after all
     * default values are inserted. If you are implementing this interface,
     * this method should work as described in the documentation for
     * insertTemplateText above.
     * @return true if any text was inserted.
     */
    virtual bool insertTemplateTextImplementation ( uint line, uint column, const QString &templateString, const QMap<QString,QString> &initialValues, QWidget *parentWindow=0 )=0;

  /**
  * only for the interface itself - REAL PRIVATE
  */
  private:
    class PrivateTemplateInterface *d;
    static uint globalTemplateInterfaceNumber;
    uint myTemplateInterfaceNumber;
};

KTEXTEDITOR_EXPORT TemplateInterface *templateInterface (Document *doc);

}

#endif
