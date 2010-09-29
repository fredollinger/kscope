/* This file is part of the KDE libraries
   Copyright (C) 2001 Joseph Wenninger <jowenn@kde.org>

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

#ifndef __ktexteditor_texthintinterface_h__
#define __ktexteditor_texthintinterface_h__

#include <qstring.h>
#include <qstringlist.h>

#include <kdelibs_export.h>

namespace KTextEditor
{

/**
 * This is an interface for the KTextEditor::View class.

 */
class KTEXTEDITOR_EXPORT TextHintInterface
{
	friend class PrivateTextHintInterface;
public:
	TextHintInterface();
	virtual ~TextHintInterface();

	/**
	 * enable Texthints. If they are enabled a signal needTextHint is emitted, if the mouse
	 * changed the position and a new character is beneath the mouse cursor. The signal is delayed
	 * for a certain time, specifiedin the timeout parameter.
	 */
	virtual void	enableTextHints(int timeout)=0;

	/**
	 * Disable texthints. Per default they are disabled.
	 */
	virtual void disableTextHints()=0;
	
	/**
	 * This method returns a number, unique during one application run
	 *
	 */	
	unsigned int textHintInterfaceNumber () const; 


	//signals

	/**
	 * emit this signal, if a tooltip text is needed for displaying.
	 * I you don't want a tooltip to be displayd set text to an emtpy string in a connected slot,
	 * otherwise set text to the string you want the editor to display
	 */
	virtual void needTextHint(int line, int col, QString &text)=0;

  private:
    class PrivateTextHintInterface *d;
    static unsigned int globalTextHintInterfaceNumber;
    unsigned int myTextHintInterfaceNumber;
};

KTEXTEDITOR_EXPORT TextHintInterface *textHintInterface (class View *view);

}

#endif
