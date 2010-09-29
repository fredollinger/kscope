/* This file is part of the KDE libraries
    Copyright (C) 1997 Christian Czezakte (e9025461@student.tuwien.ac.at)

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
    the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
    Boston, MA 02110-1301, USA.
*/

#ifndef __kprocess_h__
#define __kprocess_h__

#include <sys/types.h> // for pid_t
#include <sys/wait.h>
#include <signal.h>
#include <unistd.h>
#include <qvaluelist.h>
#include <qcstring.h>
#include <qobject.h>
#include "kdelibs_export.h"

class QSocketNotifier;
class KProcessPrivate;
class KPty;

/**
 * Child process invocation, monitoring and control.
 * This class works only in the application's main thread.
 *
 * <b>General usage and features:</b>\n
 *
 * This class allows a KDE application to start child processes without having
 * to worry about UN*X signal handling issues and zombie process reaping.
 *
 * @see KProcIO
 *
 * Basically, this class distinguishes three different ways of running
 * child processes:
 *
 * @li  DontCare -- The child process is invoked and both the child
 * process and the parent process continue concurrently.
 *
 * The process is started in an own session (see setsid(2)).
 *
 * @li  NotifyOnExit -- The child process is invoked and both the
 * child and the parent process run concurrently.
 *
 * When the child process exits, the KProcess instance
 * corresponding to it emits the Qt signal processExited().
 * Since this signal is @em not emitted from within a UN*X
 * signal handler, arbitrary function calls can be made.
 *
 * Be aware: When the KProcess object gets destructed, the child
 * process will be killed if it is still running!
 * This means in particular, that it usually makes no sense to use
 * a KProcess on the stack with NotifyOnExit.
 *
 * @li  OwnGroup -- like NotifyOnExit, but the child process is started
 * in an own process group (and an own session, FWIW). The behavior of
 * kill() changes to killing the whole process group - this makes
 * this mode useful for implementing primitive job management. It can be
 * used to work around broken wrapper scripts that don't propagate signals
 * to the "real" program. However, use this with care, as you disturb the
 * shell's job management if your program is started from the command line.
 *
 * @li  Block -- The child process starts and the parent process
 * is suspended until the child process exits. (@em Really not recommended
 * for programs with a GUI.)
 * In this mode the parent can read the child's output, but can't send it any
 * input.
 *
 * KProcess also provides several functions for determining the exit status
 * and the pid of the child process it represents.
 *
 * Furthermore it is possible to supply command-line arguments to the process
 * in a clean fashion (no null-terminated stringlists and such...)
 *
 * A small usage example:
 * \code
 *   KProcess *proc = new KProcess;
 *
 *   *proc << "my_executable";
 *   *proc << "These" << "are" << "the" << "command" << "line" << "args";
 *   QApplication::connect(proc, SIGNAL(processExited(KProcess *)),
 *                         pointer_to_my_object, SLOT(my_objects_slot(KProcess *)));
 *   proc->start();
 * \endcode
 *
 * This will start "my_executable" with the commandline arguments "These"...
 *
 * When the child process exits, the slot will be invoked.
 *
 * <b>Communication with the child process:</b>\n
 *
 * KProcess supports communication with the child process through
 * stdin/stdout/stderr.
 *
 * The following functions are provided for getting data from the child
 * process or sending data to the child's stdin (For more information,
 * have a look at the documentation of each function):
 *
 * @li writeStdin()
 *  -- Transmit data to the child process' stdin. When all data was sent, the
 * signal wroteStdin() is emitted.
 *
 * @li When data arrives at stdout or stderr, the signal receivedStdout()
 * resp. receivedStderr() is emitted.
 *
 * @li You can shut down individual communication channels with
 * closeStdin(), closeStdout(), and closeStderr(), resp.
 *
 * @author Christian Czezatke e9025461@student.tuwien.ac.at
 *
 **/
class KDECORE_EXPORT KProcess : public QObject
{
  Q_OBJECT

public:

  /**
   * Modes in which the communication channel can be opened.
   *
   * If communication for more than one channel is required,
   * the values have to be or'ed together, for example to get
   * communication with stdout as well as with stdin, you would
   * specify @p Stdin | @p Stdout
   *
   * If @p NoRead is specified in conjunction with @p Stdout,
   * no data is actually read from @p Stdout but only
   * the signal receivedStdout(int fd, int &len) is emitted.
   *
   * @p CTtyOnly tells setUsePty() to create a PTY for the process
   * and make it the process' controlling TTY, but does not redirect
   * any I/O channel to the PTY.
   *
   * If @p MergedStderr is specified in conjunction with @p Stdout,
   * Stderr will be redirected onto the same file handle as Stdout,
   * i.e., all error output will be signalled with receivedStdout().
   * Don't specify @p Stderr if you specify @p MergedStderr.
   */
  enum Communication {
       NoCommunication = 0,
       Stdin = 1, Stdout = 2, Stderr = 4,
       AllOutput = 6, All = 7,
       NoRead = 8,
       CTtyOnly = NoRead,
       MergedStderr = 16
  };

  /**
   * Run-modes for a child process.
   */
  enum RunMode {
      /**
       * The application does not receive notifications from the subprocess when
       * it is finished or aborted.
       */
       DontCare,
       /**
        * The application is notified when the subprocess dies.
        */
       NotifyOnExit,
       /**
        * The application is suspended until the started process is finished.
        */
       Block,
       /**
        * Same as NotifyOnExit, but the process is run in an own session,
        * just like with DontCare.
        */
       OwnGroup
  };

  /**
   * Constructor
   * @since 3.2
   */
  KProcess( QObject* parent, const char *name = 0 );

  /**
   * Constructor
   */ // KDE4 merge with the above
  KProcess();

  /**
   *Destructor:
   *
   *  If the process is running when the destructor for this class
   *  is called, the child process is killed with a SIGKILL, but
   *  only if the run mode is not of type @p DontCare.
   *  Processes started as @p DontCare keep running anyway.
  */
  virtual ~KProcess();

  /**
     @deprecated
     Use operator<<() instead.

	 Sets the executable to be started with this KProcess object.
	 Returns false if the process is currently running (in that
	 case the executable remains unchanged).

	 @see operator<<()

  */
  bool setExecutable(const QString& proc) KDE_DEPRECATED;


  /**
   * Sets the executable and the command line argument list for this process.
   *
   * For example, doing an "ls -l /usr/local/bin" can be achieved by:
   *  \code
   *  KProcess p;
   *  ...
   *  p << "ls" << "-l" << "/usr/local/bin"
   *  \endcode
   *
   * @param arg the argument to add
   * @return a reference to this KProcess
   **/
  KProcess &operator<<(const QString& arg);
  /**
   * Similar to previous method, takes a char *, supposed to be in locale 8 bit already.
   */
  KProcess &operator<<(const char * arg);
  /**
   * Similar to previous method, takes a QCString, supposed to be in locale 8 bit already.
   * @param arg the argument to add
   * @return a reference to this KProcess
   */
  KProcess &operator<<(const QCString & arg);

  /**
   * Sets the executable and the command line argument list for this process,
   * in a single method call, or add a list of arguments.
   * @param args the arguments to add
   * @return a reference to this KProcess
   **/
  KProcess &operator<<(const QStringList& args);

  /**
   * Clear a command line argument list that has been set by using
   * operator<<.
  */
  void clearArguments();

  /**
   *  Starts the process.
   *  For a detailed description of the
   *  various run modes and communication semantics, have a look at the
   *  general description of the KProcess class. Note that if you use
   * setUsePty( Stdout | Stderr, \<bool\> ), you cannot use Stdout | Stderr
   *  here - instead, use Stdout only to receive the mixed output.
   *
   *  The following problems could cause this function to
   *    return false:
   *
   *  @li The process is already running.
   *  @li The command line argument list is empty.
   *  @li The the @p comm parameter is incompatible with the selected pty usage.
   *  @li The starting of the process failed (could not fork).
   *  @li The executable was not found.
   *
   *  @param runmode The Run-mode for the process.
   *  @param comm  Specifies which communication links should be
   *  established to the child process (stdin/stdout/stderr). By default,
   *  no communication takes place and the respective communication
   *  signals will never get emitted.
   *
   *  @return true on success, false on error
   *  (see above for error conditions)
   **/
  virtual bool start(RunMode  runmode = NotifyOnExit,
  	Communication comm = NoCommunication);

  /**
   * Stop the process (by sending it a signal).
   *
   * @param signo The signal to send. The default is SIGTERM.
   * @return true if the signal was delivered successfully.
  */
  virtual bool kill(int signo = SIGTERM);

  /**
   * Checks whether the process is running.
   * @return true if the process is (still) considered to be running
  */
  bool isRunning() const;

  /** Returns the process id of the process.
   *
   * If it is called after
   * the process has exited, it returns the process id of the last
   *  child process that was created by this instance of KProcess.
   *
   *  Calling it before any child process has been started by this
   *  KProcess instance causes pid() to return 0.
   * @return the pid of the process or 0 if no process has been started yet.
   **/
  pid_t pid() const;

  /**
   * @deprecated
   * Use pid() instead.
   */
  KDE_DEPRECATED pid_t getPid() const { return pid(); }

  /**
   * Suspend processing of data from stdout of the child process.
   */
  void suspend();

  /**
   * Resume processing of data from stdout of the child process.
   */
  void resume();

  /**
   * Suspend execution of the current thread until the child process dies
   * or the timeout hits. This function is not recommended for programs
   * with a GUI.
   * @param timeout timeout in seconds. -1 means wait indefinitely.
   * @return true if the process exited, false if the timeout hit.
   * @since 3.2
   */
  bool wait(int timeout = -1);

  /**
   * Checks whether the process exited cleanly.
   *
   * @return true if the process has already finished and has exited
   *  "voluntarily", ie: it has not been killed by a signal.
   */
  bool normalExit() const;

  /**
   * Checks whether the process was killed by a signal.
   *
   * @return true if the process has already finished and has not exited
   * "voluntarily", ie: it has been killed by a signal.
   *
   * @since 3.2
   */
  bool signalled() const;

  /**
   * Checks whether a killed process dumped core.
   *
   * @return true if signalled() returns true and the process
   * dumped core. Note that on systems that don't define the
   * WCOREDUMP macro, the return value is always false.
   *
   * @since 3.2
   */
  bool coreDumped() const;

  /**
   * Returns the exit status of the process.
   *
   * @return the exit status of the process. Note that this value
   * is not valid if normalExit() returns false.
   */
  int exitStatus() const;

  /**
   * Returns the signal the process was killed by.
   *
   * @return the signal number that caused the process to exit.
   * Note that this value is not valid if signalled() returns false.
   *
   * @since 3.2
   */
  int exitSignal() const;

  /**
   *	 Transmit data to the child process' stdin.
   *
   * This function may return false in the following cases:
   *
   *     @li The process is not currently running.
   * This implies that you cannot use this function in Block mode.
   *
   *     @li Communication to stdin has not been requested in the start() call.
   *
   *     @li Transmission of data to the child process by a previous call to
   * writeStdin() is still in progress.
   *
   * Please note that the data is sent to the client asynchronously,
   * so when this function returns, the data might not have been
   * processed by the child process.
   * That means that you must not free @p buffer or call writeStdin()
   * again until either a wroteStdin() signal indicates that the
   * data has been sent or a processExited() signal shows that
   * the child process is no longer alive.
   *
   * If all the data has been sent to the client, the signal
   * wroteStdin() will be emitted.
   *
   * This function does not work when the process is start()ed in Block mode.
   *
   * @param buffer the buffer to write
   * @param buflen the length of the buffer
   * @return false if an error has occurred
   **/
  bool writeStdin(const char *buffer, int buflen);

  /**
   * Shuts down the Stdin communication link. If no pty is used, this
   * causes "EOF" to be indicated on the child's stdin file descriptor.
   *
   * @return false if no Stdin communication link exists (any more).
   */
  bool closeStdin();

  /**
   * Shuts down the Stdout communication link. If no pty is used, any further
   * attempts by the child to write to its stdout file descriptor will cause
   * it to receive a SIGPIPE.
   *
   * @return false if no Stdout communication link exists (any more).
   */
  bool closeStdout();

  /**
   * Shuts down the Stderr communication link. If no pty is used, any further
   * attempts by the child to write to its stderr file descriptor will cause
   * it to receive a SIGPIPE.
   *
   * @return false if no Stderr communication link exists (any more).
   */
  bool closeStderr();

  /**
   * Deletes the optional utmp entry and closes the pty.
   *
   * Make sure to shut down any communication links that are using the pty
   * before calling this function.
   *
   * @return false if the pty is not open (any more).
   */
  bool closePty();

  /**
   * @brief Close stdin, stdout, stderr and the pty
   * 
   * This is the same that calling all close* functions in a row:
   * @see closeStdin, @see closeStdout, @see closeStderr and @see closePty
   */
  void closeAll();

  /**
   * Lets you see what your arguments are for debugging.
   * @return the list of arguments
   */
  const QValueList<QCString> &args() /* const */ { return arguments; }

  /**
   * Controls whether the started process should drop any
   * setuid/setgid privileges or whether it should keep them.
   * Note that this function is mostly a dummy, as the KDE libraries
   * currently refuse to run with setuid/setgid privileges.
   *
   * The default is false: drop privileges
   * @param keepPrivileges true to keep the privileges
   */
  void setRunPrivileged(bool keepPrivileges);

  /**
   * Returns whether the started process will drop any
   * setuid/setgid privileges or whether it will keep them.
   * @return true if the process runs privileged
   */
  bool runPrivileged() const;

  /**
   * Adds the variable @p name to the process' environment.
   * This function must be called before starting the process.
   * @param name the name of the environment variable
   * @param value the new value for the environment variable
   */
  void setEnvironment(const QString &name, const QString &value);

  /**
   * Changes the current working directory (CWD) of the process
   * to be started.
   * This function must be called before starting the process.
   * @param dir the new directory
   */
  void setWorkingDirectory(const QString &dir);

  /**
   * Specify whether to start the command via a shell or directly.
   * The default is to start the command directly.
   * If @p useShell is true @p shell will be used as shell, or
   * if shell is empty, /bin/sh will be used.
   *
   * When using a shell, the caller should make sure that all filenames etc.
   * are properly quoted when passed as argument.
   * @see quote()
   * @param useShell true if the command should be started via a shell
   * @param shell the path to the shell that will execute the process, or
   *              0 to use /bin/sh. Use getenv("SHELL") to use the user's
   *              default shell, but note that doing so is usually a bad idea
   *              for shell compatibility reasons.
   * @since 3.1
   */
  void setUseShell(bool useShell, const char *shell = 0);

  /**
   * This function can be used to quote an argument string such that
   * the shell processes it properly. This is e. g. necessary for
   * user-provided file names which may contain spaces or quotes.
   * It also prevents expansion of wild cards and environment variables.
   * @param arg the argument to quote
   * @return the quoted argument
   * @since 3.1
   */
  static QString quote(const QString &arg);

  /**
   * Detaches KProcess from child process. All communication is closed.
   * No exit notification is emitted any more for the child process.
   * Deleting the KProcess will no longer kill the child process.
   * Note that the current process remains the parent process of the
   * child process.
   */
  void detach();

#ifdef Q_OS_UNIX
  /**
   * Specify whether to create a pty (pseudo-terminal) for running the
   * command.
   * This function should be called before starting the process.
   *
   * @param comm for which stdio handles to use a pty. Note that it is not
   *  allowed to specify Stdout and Stderr at the same time both here and to
   * start (there is only one pty, so they cannot be distinguished).
   * @param addUtmp true if a utmp entry should be created for the pty
   * @since 3.2
   */
  void setUsePty(Communication comm, bool addUtmp);

  /**
   * Obtains the pty object used by this process. The return value is
   * valid only after setUsePty() was used with a non-zero argument.
   * The pty is open only while the process is running.
   * @return a pointer to the pty object
   * @since 3.2
   */
  KPty *pty() const;
#endif

  /**
   * More or less intuitive constants for use with setPriority().
   */
  enum { PrioLowest = 20, PrioLow = 10, PrioLower = 5, PrioNormal = 0,
    PrioHigher = -5, PrioHigh = -10, PrioHighest = -19 };

  /**
   * Sets the scheduling priority of the process.
   * @param prio the new priority in the range -20 (high) to 19 (low).
   * @return false on error; see setpriority(2) for possible reasons.
   * @since 3.2
   */
  bool setPriority(int prio);

signals:
  /**
   * Emitted after the process has terminated when
   * the process was run in the @p NotifyOnExit  (==default option to
   * start() ) or the Block mode.
   * @param proc a pointer to the process that has exited
   **/
  void processExited(KProcess *proc);


  /**
   * Emitted, when output from the child process has
   * been received on stdout.
   *
   * To actually get this signal, the Stdout communication link
   * has to be turned on in start().
   *
   * @param proc a pointer to the process that has received the output
   * @param buffer The data received.
   * @param buflen The number of bytes that are available.
   *
   * You should copy the information contained in @p buffer to your private
   * data structures before returning from the slot.
   * Example:
   * \code
   *     QString myBuf = QString::fromLatin1(buffer, buflen);
   * \endcode
   **/
  void receivedStdout(KProcess *proc, char *buffer, int buflen);

  /**
   * Emitted when output from the child process has
   * been received on stdout.
   *
   * To actually get this signal, the Stdout communication link
   * has to be turned on in start() and the
   * NoRead flag must have been passed.
   *
   * You will need to explicitly call resume() after your call to start()
   * to begin processing data from the child process' stdout.  This is
   * to ensure that this signal is not emitted when no one is connected
   * to it, otherwise this signal will not be emitted.
   *
   * The data still has to be read from file descriptor @p fd.
   * @param fd the file descriptor that provides the data
   * @param len the number of bytes that have been read from @p fd must
   *  be written here
   **/
  void receivedStdout(int fd, int &len); // KDE4: change, broken API


  /**
   * Emitted, when output from the child process has
   * been received on stderr.
   *
   * To actually get this signal, the Stderr communication link
   * has to be turned on in start().
   *
   * You should copy the information contained in @p buffer to your private
   * data structures before returning from the slot.
   *
   * @param proc a pointer to the process that has received the data
   * @param buffer The data received.
   * @param buflen The number of bytes that are available.
   **/
  void receivedStderr(KProcess *proc, char *buffer, int buflen);

  /**
   * Emitted after all the data that has been
   * specified by a prior call to writeStdin() has actually been
   * written to the child process.
   * @param proc a pointer to the process
   **/
  void wroteStdin(KProcess *proc);


protected slots:

 /**
  * This slot gets activated when data from the child's stdout arrives.
  * It usually calls childOutput().
  * @param fdno the file descriptor for the output
  */
  void slotChildOutput(int fdno);

 /**
  * This slot gets activated when data from the child's stderr arrives.
  * It usually calls childError().
  * @param fdno the file descriptor for the output
  */
  void slotChildError(int fdno);

  /**
   * Called when another bulk of data can be sent to the child's
   * stdin. If there is no more data to be sent to stdin currently
   * available, this function must disable the QSocketNotifier innot.
   * @param dummy ignore this argument
   */
  void slotSendData(int dummy);	// KDE 4: remove dummy

protected:

  /**
   * Sets up the environment according to the data passed via
   * setEnvironment()
   */
  void setupEnvironment();

  /**
   * The list of the process' command line arguments. The first entry
   * in this list is the executable itself.
   */
  QValueList<QCString> arguments;
  /**
   * How to run the process (Block, NotifyOnExit, DontCare). You should
   *  not modify this data member directly from derived classes.
   */
  RunMode run_mode;
  /**
   * true if the process is currently running. You should not
   * modify this data member directly from derived classes. Please use
   * isRunning() for reading the value of this data member since it
   * will probably be made private in later versions of KProcess.
   */
  bool runs;

  /**
   * The PID of the currently running process.
   * You should not modify this data member in derived classes.
   * Please use pid() instead of directly accessing this
   * member since it will probably be made private in
   * later versions of KProcess.
   */
  pid_t pid_;

  /**
   * The process' exit status as returned by waitpid(). You should not
   * modify the value of this data member from derived classes. You should
   * rather use exitStatus() than accessing this data member directly
   * since it will probably be made private in further versions of
   * KProcess.
   */
  int status;


  /**
   * If false, the child process' effective uid & gid will be reset to the
   * real values.
   * @see setRunPrivileged()
   */
  bool keepPrivs;

  /**
   * This function is called from start() right before a fork() takes
   * place. According to the @p comm parameter this function has to initialize
   * the in, out and err data members of KProcess.
   *
   * This function should return 1 if setting the needed communication channels
   * was successful.
   *
   * The default implementation is to create UNIX STREAM sockets for the
   * communication, but you could reimplement this function to establish a
   * TCP/IP communication for network communication, for example.
   */
  virtual int setupCommunication(Communication comm);

  /**
   * Called right after a (successful) fork() on the parent side. This function
   * will usually do some communications cleanup, like closing in[0],
   * out[1] and out[1].
   *
   * Furthermore, it must also create the QSocketNotifiers innot,
   * outnot and errnot and connect their Qt signals to the respective
   * KProcess slots.
   *
   * For a more detailed explanation, it is best to have a look at the default
   * implementation in kprocess.cpp.
   */
  virtual int commSetupDoneP();

  /**
   * Called right after a (successful) fork(), but before an exec() on the child
   * process' side. It usually duplicates the in[0], out[1] and
   * err[1] file handles to the respective standard I/O handles.
   */
  virtual int commSetupDoneC();


  /**
   * Immediately called after a successfully started process in NotifyOnExit
   * mode has exited. This function normally calls commClose()
   * and emits the processExited() signal.
   * @param state the exit code of the process as returned by waitpid()
   */
  virtual void processHasExited(int state);

  /**
   * Cleans up the communication links to the child after it has exited.
   * This function should act upon the values of pid() and runs.
   * See the kprocess.cpp source for details.
   * @li If pid() returns zero, the communication links should be closed
   *  only.
   * @li if pid() returns non-zero and runs is false, all data
   *  immediately available from the communication links should be processed
   *  before closing them.
   * @li if pid() returns non-zero and runs is true, the communication
   *  links should be monitored for data until the file handle returned by
   *  KProcessController::theKProcessController->notifierFd() becomes ready
   *  for reading - when it triggers, runs should be reset to false, and
   *  the function should be immediately left without closing anything.
   *
   * The previous semantics of this function are forward-compatible, but should
   * be avoided, as they are prone to race conditions and can cause KProcess
   * (and thus the whole program) to lock up under certain circumstances. At the
   * end the function closes the communication links in any case. Additionally
   * @li if runs is true, the communication links are monitored for data
   *  until all of them have returned EOF. Note that if any system function is
   *  interrupted (errno == EINTR) the polling loop should be aborted.
   * @li if runs is false, all data immediately available from the
   *  communication links is processed.
   */
  virtual void commClose();

  /* KDE 4 - commClose will be changed to perform cleanup only in all cases *
   * If @p notfd is -1, all data immediately available from the
   *  communication links should be processed.
   * If @p notfd is not -1, the communication links should be monitored
   *  for data until the file handle @p notfd becomes ready for reading.
   */
//  virtual void commDrain(int notfd);

  /**
   * Specify the actual executable that should be started (first argument to execve)
   * Normally the the first argument is the executable but you can
   * override that with this function.
   */
  void setBinaryExecutable(const char *filename);

  /**
   * The socket descriptors for stdout.
   */
  int out[2];
  /**
   * The socket descriptors for stdin.
   */
  int in[2];
  /**
   * The socket descriptors for stderr.
   */
  int err[2];

  /**
   * The socket notifier for in[1].
   */
  QSocketNotifier *innot;
  /**
   * The socket notifier for out[0].
   */
  QSocketNotifier *outnot;
  /**
   * The socket notifier for err[0].
   */
  QSocketNotifier *errnot;

  /**
   * Lists the communication links that are activated for the child
   * process.  Should not be modified from derived classes.
   */
  Communication communication;

  /**
   * Called by slotChildOutput() this function copies data arriving from
   * the child process' stdout to the respective buffer and emits the signal
   * receivedStdout().
   */
  int childOutput(int fdno);

  /**
   * Called by slotChildError() this function copies data arriving from
   * the child process' stderr to the respective buffer and emits the signal
   * receivedStderr().
   */
  int childError(int fdno);

  /**
   * The buffer holding the data that has to be sent to the child
   */
  const char *input_data;
  /**
   * The number of bytes already transmitted
   */
  int input_sent;
  /**
   * The total length of input_data
   */
  int input_total;

  /**
   * KProcessController is a friend of KProcess because it has to have
   * access to various data members.
   */
  friend class KProcessController;

protected:
  virtual void virtual_hook( int id, void* data );
private:
  KProcessPrivate *d;
};

class KShellProcessPrivate;

/**
* @obsolete
*
* Use KProcess and KProcess::setUseShell(true) instead.
*
*   @short A class derived from KProcess to start child
*   	processes through a shell.
*   @author Christian Czezatke <e9025461@student.tuwien.ac.at>
*/
class KDECORE_EXPORT KShellProcess: public KProcess
{
  Q_OBJECT

public:

  /**
   * Constructor
   *
   * If no shellname is specified, the user's default shell is used.
   */
  KShellProcess(const char *shellname=0);

  /**
   * Destructor.
   */
  ~KShellProcess();

  virtual bool start(RunMode  runmode = NotifyOnExit,
		  Communication comm = NoCommunication);

  static QString quote(const QString &arg);

private:
  QCString shell;

protected:
  virtual void virtual_hook( int id, void* data );
private:
  KShellProcessPrivate *d;
};



#endif

