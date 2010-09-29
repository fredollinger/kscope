    /*

    Copyright (C) 2000 Stefan Westerfeld
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

#ifndef AUDIOSUBSYS_H
#define AUDIOSUBSYS_H
#include <string>

#include "arts_export.h"
#include "pipebuffer.h"
#include "startupmanager.h"

/*
 * BC - Status (2002-03-08): AudioSubSystem, ASProducer, ASConsumer.
 *
 * These classes are kept binary compatible. You can rely on them.
 * AudioSubSystem has a private data pointer to do so. Even if ports to
 * other architectures or hardware will require different organization
 * (i.e. no fragments, direct memory access via mmap), the data members
 * and functions exported here MAY NOT BE CHANGED. Use the private data
 * pointer for adding data members.
 *
 * If ASProducer/ASConsumer doesn't suit the needs any longer, do NOT
 * change them. Add new classes instead.
 */

namespace Arts {

class ASProducer {
public:
	virtual void needMore() = 0;
};

// FIXME: haveMore won't be called currently
class ASConsumer {
public:
	virtual void haveMore() = 0;
};

class ARTS_EXPORT AudioSubSystemStart :public StartupClass {
protected:
	class AudioSubSystem *_instance;
public:
	inline AudioSubSystem *the() { return _instance; };
	void startup();
	void shutdown();
};

class AudioSubSystemPrivate;

class ARTS_EXPORT AudioSubSystem {
	class AudioSubSystemPrivate *d;

	std::string _error;
	char *fragment_buffer;

	int _fragmentCount;
	int _fragmentSize;

	bool _running;
	PipeBuffer wBuffer, rBuffer;
	ASConsumer *consumer;
	ASProducer *producer;

	friend class AudioSubSystemStart;
	AudioSubSystem();
	~AudioSubSystem();

	void close();
	void initAudioIO();

	/**
	 * checks that full duplex doesn't go out of sync
	 */
	void adjustDuplexBuffers();

	/**
	 * creates count empty fragments of input in rBuffer if count > 0
	 * removes count fragments of input from rBuffer if count < 0
	 */
	void adjustInputBuffer(int count);

public:
	enum { ioRead=1, ioWrite=2, ioExcept=4 };

	// singleton
	static AudioSubSystem *the();

	/*
	 * Currently, if you want to use the AudioSubSystem, you need to
	 * 
	 * 1. - attach one producer
	 *    - attach one consumer (only for full duplex)
	 *    - open the audio subsystem using open (watch the fd)
	 *    (in any order)
	 *
	 * 2. react on the callbacks you get for the producer
	 *
	 * 3. if you don't need the audio subsystem any longer, call detach
	 *    both, the producer and the cosumer.
	 *
	 * Be careful that you don't read/write from/to the audio subsystem
	 * when running() is not true.
	 */

	bool attachProducer(ASProducer *producer);
	bool attachConsumer(ASConsumer *consumer);

	void detachProducer();
	void detachConsumer();

	/*
	 * can be used to select the AudioIO class to use, reasonable choices
	 * may be "oss" or "alsa" at this point in time - you need to choose
	 * this before doing anything else
	 */
	void audioIO(const std::string& audioIO);
	std::string audioIO();

	// which device to use for audio output (default /dev/dsp)
	void deviceName(const std::string& deviceName);
	std::string deviceName();

	void fragmentSize(int size);
	int fragmentSize();

	void fragmentCount(int fragments);
	int fragmentCount();

	void samplingRate(int samplingrate);
	int samplingRate();

	void channels(int channels);
	int channels();

	void format(int format);
	int format();

	/**
	 * As opposed to format(), this one returns the number of bits used per
	 * sample. Thats sometimes a difference, for instance 16bit big endian
	 * encoding has the format() 17, whereas bits() would return 16.
	 */
	int bits();

	void fullDuplex(bool newFullDuplex);
	bool fullDuplex();

	bool check();

	/**
	 * Opens the audio device.
	 *
	 * After opening, you must check selectReadFD() and selectWriteFD() to
	 * select() on the appropriate file descriptors. Whenever select()ing is
	 * successful, handleIO needs to be called.
	 *
	 * The type for handleIO must be set to ioRead if fd is ready for
	 * reading, ioWrite if fd is ready for writing, ioExcept if something
	 * special happend or any combination of these using bitwise or.
	 *
	 * @returns true if audio device has been opened successfully,
	 *          false otherwise
	 */
	bool open();

	/**
	 * human readable error message that descibes why opening the audio device
	 * failed
	 */
	const char *error();

	/**
	 * File descriptor to select on for reading (@see open()), -1 if there is
	 * none.
	 */
	int selectReadFD();

	/**
	 * File descriptor to select on for writing (@see open()), -1 if there is
	 * none.
	 */
	int selectWriteFD();

	/**
	 * Needs to be called to handle I/O on the filedescriptors given by
	 * selectReadFD() and selectWriteFD() (@see open()).
	 */
	void handleIO(int type);

	void read(void *buffer, int size);
	void write(void *buffer, int size);

	/**
	 * this returns the time in seconds it will take until everything written
	 * to the AudioSubSystem so far will be played - in other words, it returns
	 * the time it will take until the next sample written by the application
	 * will be heard by the user
	 */
	float outputDelay();

	/**
	 * returns true as long as the audio subsystem is opened and active (that
	 * is, between successful opening, with attaching producer, and the first
	 * detachConsumer/detachProducer)
	 */
	bool running();

	/**
	 * called during crashes, to release the opened device (and other resources)
	 * don't use this method for other purposes than emergencies
	 */
	void emergencyCleanup();
};

}

#endif /* AUDIOSUBSYS_H */
