/*
   This file is part of the KDE libraries
   Copyright (c) 2004 Waldo Bastian <bastian@kde.org>
   
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

#ifndef _KLOCKFILE_H_
#define _KLOCKFILE_H_

#include <qstring.h>
#include <qdatetime.h>

#include <ksharedptr.h>
/**
 * The KLockFile class provides NFS safe lockfiles.
 *
 * @author Waldo Bastian <bastian@kde.org>
 * @since 3.3
 */
class KDECORE_EXPORT KLockFile : public KShared
{
public:
   typedef KSharedPtr<KLockFile> Ptr;

   KLockFile(const QString &file);

   /**
    * Destroys the object, releasing the lock if held
    **/
   ~KLockFile();

   /**
    * Possible return values of the lock function.
    */
   enum LockResult {
     /**
      * Lock was acquired successfully
      */
     LockOK = 0, 

     /**
      * The lock could not be acquired because it is held by another process
      */
     LockFail,
     
     /**
      * The lock could not be acquired due to an error
      */
     LockError,

     /**
      * A stale lock has been detected
      */
     LockStale
   };

   enum LockOptions {
     /**
      * Return immediately, do not wait for the lock to become available
      */
     LockNoBlock = 1,
     
     /**
      * Automatically remove a lock when a lock is detected that is stale
      * for more than staleTime() seconds.
      */
     LockForce = 2
   };

   /**
    * Attempt to acquire the lock
    *
    * @param options A set of @ref LockOptions OR'ed together.
    */
   LockResult lock(int options=0);
   
   /**
    * Returns whether the lock is held or not
    */
   bool isLocked() const;
   
   /**
    * Release the lock
    */
   void unlock();

   /**
    * Return the time in seconds after which a lock is considered stale
    * The default is 30.
    */
   int staleTime() const;
   
   /**
    * Set the time in seconds after which a lock is considered stale
    */
   void setStaleTime(int _staleTime);

   /**
    * Returns the pid, hostname and appname of the process holding
    * the lock after the lock functon has returned with LockStale.
    * @returns false if the pid and hostname could not be determined
    */
   bool getLockInfo(int &pid, QString &hostname, QString &appname);

private:
   class KLockFilePrivate;
   KLockFilePrivate *d;
};

#endif
