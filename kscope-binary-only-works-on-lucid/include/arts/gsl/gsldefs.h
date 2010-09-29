/* GSL - Generic Sound Layer
 * Copyright (C) 2001 Stefan Westerfeld and Tim Janik
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General
 * Public License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place, Suite 330,
 * Boston, MA 02111-1307, USA.
 */
#ifndef __GSL_DEFS_H__
#define __GSL_DEFS_H__

/* configure checks */
#include <gsl/gslconfig.h>
#include <glib.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/* --- forward decls --- */
typedef struct _GslMagic		GslMagic;
typedef struct _GslClass		GslClass;
typedef struct _GslComplex		GslComplex;
typedef struct _GslDataCache		GslDataCache;
typedef struct _GslDataHandle		GslDataHandle;
typedef struct _GslDataHandleFuncs	GslDataHandleFuncs;
typedef struct _GslGlueContext          GslGlueContext;
typedef struct _GslJob			GslJob;
typedef struct _GslModule		GslModule;
typedef struct _GslIStream		GslIStream;
typedef struct _GslJStream		GslJStream;
typedef struct _GslLoader		GslLoader;
typedef struct _GslOStream		GslOStream;
typedef struct _GslThread		GslThread;
typedef struct _GslTrans		GslTrans;
typedef struct _GslWaveChunk		GslWaveChunk;
typedef struct _GslWaveChunkBlock	GslWaveChunkBlock;
typedef struct _GslRecMutex		GslRecMutex;
typedef struct _GslRing			GslRing;
typedef union _GslCond			GslCond;
typedef union _GslMutex			GslMutex;
/* ssize_t/off_t type used within Gsl */
typedef glong			  GslLong;
#define	GSL_MAXLONG		  G_MAXLONG


/* --- GSL errors --- */
typedef enum    /*< skip >*/
{
  GSL_ERROR_NONE,
  GSL_ERROR_INTERNAL,
  GSL_ERROR_UNKNOWN,
  /* I/O errors */
  GSL_ERROR_IO,
  GSL_ERROR_PERMS,
  GSL_ERROR_BUSY,
  GSL_ERROR_EXISTS,
  GSL_ERROR_TEMP,
  GSL_ERROR_EOF,
#define GSL_ERROR_FILE_EMPTY	GSL_ERROR_EOF
  GSL_ERROR_NOT_FOUND,
  GSL_ERROR_OPEN_FAILED,
  GSL_ERROR_SEEK_FAILED,
  GSL_ERROR_READ_FAILED,
  GSL_ERROR_WRITE_FAILED,
  /* content errors */
  GSL_ERROR_FORMAT_INVALID,
  GSL_ERROR_FORMAT_UNKNOWN,
  GSL_ERROR_DATA_CORRUPT,
  GSL_ERROR_CONTENT_GLITCH,
  /* miscellaneous errors */
  GSL_ERROR_NO_RESOURCE,
  GSL_ERROR_CODEC_FAILURE,
  GSL_ERROR_LAST        /* administrative */
} GslErrorType;


/* --- functions --- */
typedef void     (*GslAccessFunc)       (GslModule      *module,
					 gpointer        data);
typedef void     (*GslFreeFunc)         (gpointer        data);
typedef void     (*GslModuleFreeFunc)   (gpointer        data,
					 const GslClass	*klass);


#if defined (BSE_COMPILATION) || defined (BSE_PLUGIN_FALLBACK) \
    || (GSL_USE_GSL_GLIB) || defined (GSL_EXTENSIONS)
#  define if_expect(cond)		if (GSL_GCC_EXPECT (cond))
#  define if_reject(cond)		if (GSL_GCC_REJECT (cond))
#endif


/* --- implementation details --- */
#define	GSL_ENGINE_MAX_POLLFDS	(128)
union _GslCond
{
  gpointer cond_pointer;
  guint8   cond_dummy[MAX (8, GSL_SIZEOF_PTH_COND_T)];
};
union _GslMutex
{
  gpointer mutex_pointer;
  guint8   mutex_dummy[MAX (8, GSL_SIZEOF_PTH_MUTEX_T)];
};
struct _GslRecMutex
{
  GslMutex sync_mutex;
  gpointer owner;
  guint    depth;
};
#if __GNUC__ >= 3 && defined __OPTIMIZE__
#  define GSL_GCC_EXPECT(cond)	(__builtin_expect ((cond) != 0, 1))
#  define GSL_GCC_REJECT(cond)	(__builtin_expect ((cond) != 0, 0))
#else
#  define GSL_GCC_EXPECT(cond)	cond
#  define GSL_GCC_REJECT(cond)	cond
#endif

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __GSL_DEFS_H__ */

/* vim:set ts=8 sw=2 sts=2: */
