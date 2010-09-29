/*
    Copyright (C) 2000 Nicolas Brodu
    nicolas.brodu@free.fr

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
    the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
    Boston, MA 02111-1307, USA.
*/

#ifndef MCOP_CONNECT_H
#define MCOP_CONNECT_H

/*
 * BC - Status (2002-03-08): connect / setValue functions
 *
 * Will be kept as they are (binary compatible).
 */

#include <string>
#include "arts_export.h"

namespace Arts {
class Object;

// Connect function overloaded for components with default port
void ARTS_EXPORT connect(const Object& src, const std::string& output, const Object& dest, const std::string& input);
void ARTS_EXPORT connect(const Object& src, const std::string& output, const Object& dest);
void ARTS_EXPORT connect(const Object& src, const Object& dest, const std::string& input);
void ARTS_EXPORT connect(const Object& src, const Object& dest);
// Same for disconnect function
void ARTS_EXPORT disconnect(const Object& src, const std::string& output, const Object& dest, const std::string& input);
void ARTS_EXPORT disconnect(const Object& src, const std::string& output, const Object& dest);
void ARTS_EXPORT disconnect(const Object& src, const Object& dest, const std::string& input);
void ARTS_EXPORT disconnect(const Object& src, const Object& dest);

// setValue function overloaded for components with default port
void ARTS_EXPORT setValue(const Object& c, const std::string& port, const float fvalue);
void ARTS_EXPORT setValue(const Object& c, const float fvalue);
//void setValue(const Object& c, const std::string& port, const std::string& svalue);
//void setValue(const Object& c, const std::string& svalue);

}

#endif
