/* GSL - Generic Sound Layer
 * Copyright (C) 2001 Tim Janik
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
#ifndef __GSL_WAVE_CHUNK_H__
#define __GSL_WAVE_CHUNK_H__

#include <gsl/gsldefs.h>
#include <gsl/gsldatacache.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/* --- typedefs & structures --- */
typedef enum /*< skip >*/
{
  GSL_WAVE_LOOP_NONE,
  GSL_WAVE_LOOP_JUMP,
  GSL_WAVE_LOOP_PINGPONG
} GslWaveLoopType;
typedef struct
{
  GslLong start, end, length;
  gfloat *mem;
} GslWaveChunkMem;
struct _GslWaveChunk
{
  /* wave chunk data residency */
  GslDataCache   *dcache;
  GslLong	  length;	/* number of per-channel-values * n-channels */

  /* chunk specific parameters */
  gint		  n_channels;
  GslLong	  n_pad_values;	/* guaranteed pad values around blocks */
  GslLong	  wave_length;	/* start + loop duration + end (single channel) */

  /* flags */
  guint		  pploop_ends_backwards : 1;
  guint		  mini_loop : 1;

  /* loop spec */
  GslWaveLoopType loop_type;
  GslLong	  loop_first;
  GslLong	  loop_last;
  guint		  loop_count;

  /* preformatted blocks */
  GslWaveChunkMem head;
  GslWaveChunkMem enter;
  GslWaveChunkMem wrap;
  GslWaveChunkMem ppwrap;
  GslWaveChunkMem leave;
  GslWaveChunkMem tail;
  GslLong	  leave_end_norm;
  GslLong	  tail_start_norm;

  GslWaveLoopType requested_loop_type;
  GslLong         requested_loop_first;
  GslLong         requested_loop_last;
  guint           requested_loop_count;
  guint           ref_count;
  guint           open_count;
  /* legacy */
  gfloat	  mix_freq;	/* recorded with mix_freq */
  gfloat	  osc_freq;	/* while oscillating at osc_freq */
};
struct _GslWaveChunkBlock
{
  /* requisition (in) */
  gint		play_dir;	/* usually +1 */
  GslLong	offset;		/* requested offset into wave */
  /* result (out) */
  GslLong	length;		/* resulting signed? length of block in # values */
  gboolean	is_silent;	/* sample end reached, values are 0 */
  gint		dirstride;	/* >0 => increment, <0 => decrement */
  gfloat       *start;		/* first data value location */
  gfloat       *end;		/* last data value location +1 */
  GslLong	next_offset;	/* offset of next adjunct block */
  /*< private >*/
  gpointer	node;
};


/* --- prototypes --- */
void		gsl_wave_chunk_use_block	(GslWaveChunk		*wave_chunk,
						 GslWaveChunkBlock	*block);
void		gsl_wave_chunk_unuse_block	(GslWaveChunk		*wave_chunk,
						 GslWaveChunkBlock	*block);
GslWaveChunk*	gsl_wave_chunk_new		(GslDataCache		*dcache,
						 gfloat			 osc_freq,
						 gfloat			 mix_freq,
						 GslWaveLoopType	 loop_type,
						 GslLong		 loop_first,
						 GslLong		 loop_end,
						 guint			 loop_count);
GslWaveChunk*	gsl_wave_chunk_ref		(GslWaveChunk		*wchunk);
void		gsl_wave_chunk_unref		(GslWaveChunk		*wchunk);
GslErrorType	gsl_wave_chunk_open		(GslWaveChunk		*wchunk);
void		gsl_wave_chunk_close		(GslWaveChunk		*wchunk);
void		gsl_wave_chunk_debug_block	(GslWaveChunk		*wchunk,
						 GslLong		 offset,
						 GslLong		 length,
						 gfloat			*block);
GslWaveChunk*	_gsl_wave_chunk_copy		(GslWaveChunk		*wchunk);
const gchar*	gsl_wave_loop_type_to_string	(GslWaveLoopType	 wave_loop);
GslWaveLoopType	gsl_wave_loop_type_from_string	(const gchar		*string);


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __GSL_WAVE_CHUNK_H__ */
