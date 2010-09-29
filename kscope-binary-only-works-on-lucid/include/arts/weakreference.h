    /*

    Copyright (C) 2000-2002 Stefan Westerfeld
                            stefan@space.twc.de

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

/*
 * BC - Status (2002-03-08): WeakReference(Base)
 *
 * Has to be kept binary compatible by not touching it. Add a new class if
 * you need something else.
 */

#ifndef MCOP_WEAKREFERENCE_H
#define MCOP_WEAKREFERENCE_H

#include "arts_export.h"

namespace Arts {

class ARTS_EXPORT WeakReferenceBase {
public:
	virtual void release() = 0;
};

/**
 * The WeakReference class can be used to safely store an object, without
 * disallowing that it gets destroyed. A typical situation where you may want
 * to use this is when you implement a datastructure, where A refers B, and
 * B refers A. Using "normal" references, this structure would never get
 * freed.
 *
 * Usage example:
 *
 *     Synth_WAVE_SIN sin;
 *     WeakReference<Synth_PLAY_WAV> weak_sin;
 *
 *     {
 *       Synth_WAVE_SIN test = weak_sin;
 *         if(test.isNull())
 *           printf("#1 missing object\n");
 *         else
 *           test.start();
 *     }
 *
 *     // now, the strong reference leaves
 *     sin = Synth_WAVE_SIN::null();
 *
 *     {
 *       Synth_WAVE_SIN test = weak_sin;
 *	     if(test.isNull())
 *         printf("#2 missing object\n");
 *       else
 *         test.stop();
 *
 * This would output "#2 missing object".
 *
 */

template<class SmartWrapper>
class WeakReference : public WeakReferenceBase
{
private:
	typename SmartWrapper::_base_class *content;

public:
	WeakReference()
	{
		content = 0;
	}
	WeakReference(const WeakReference<SmartWrapper>& source)
		:WeakReferenceBase(source), content(source.content)
	{
		if(content)
			content->_addWeakReference(this);
	}
	WeakReference(SmartWrapper& source)
	{
		content = source._base();
		if(content)
			content->_addWeakReference(this);
	}
	inline WeakReference<SmartWrapper>& operator=(const
						WeakReference<SmartWrapper>& source)
	{
		release();
		content = source.content;
		if(content)
			content->_addWeakReference(this);
		return *this;
	}
	virtual ~WeakReference()
	{
		release();
	}
	void release()
	{
		if(content)
		{
			content->_removeWeakReference(this);
			content = 0;
		}
	}
	inline operator SmartWrapper() const
	{
		if(content)
			return SmartWrapper::_from_base(content->_copy());
		else
			return SmartWrapper::null();
	}
};

}

#endif /* MCOP_WEAKREFERENCE_H */
