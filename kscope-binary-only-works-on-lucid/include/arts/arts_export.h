/* mcop/arts_export.h.  Generated from arts_export.h.in by configure.  */
/* This file is part of the KDE libraries
    Copyright (c) 2002-2003 KDE Team

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

#ifndef ARTS_EXPORT_H
#define ARTS_EXPORT_H

/* #undef __KDE_HAVE_GCC_VISIBILITY */

/**
 * The ARTS_NO_EXPORT macro marks the symbol of the given variable 
 * to be hidden. A hidden symbol is stripped during the linking step, 
 * so it can't be used from outside the resulting library, which is similar
 * to static. However, static limits the visibility to the current 
 * compilation unit. hidden symbols can still be used in multiple compilation
 * units.
 *
 * \code
 * int ARTS_NO_EXPORT foo;
 * int ARTS_EXPORT bar;
 * \end
 */

#if defined(__KDE_HAVE_GCC_VISIBILITY)
/* Visibility is available for GCC newer than 3.4.
 * See: http://gcc.gnu.org/bugzilla/show_bug.cgi?id=9283 
 */
#define ARTS_NO_EXPORT __attribute__ ((visibility("hidden")))
#define ARTS_EXPORT __attribute__ ((visibility("default")))
#elif defined(_WIN32)
#define ARTS_NO_EXPORT
#define ARTS_EXPORT __declspec(dllexport)
#else
#define ARTS_NO_EXPORT
#define ARTS_EXPORT
#endif

#endif /* ARTS_EXPORTS */
