/****************************************************************************
**
** DCOP Stub Definition created by dcopidl2cpp from uiserver.kidl
**
** WARNING! All changes made in this file will be lost!
**
*****************************************************************************/

#ifndef __UISERVER_STUB__
#define __UISERVER_STUB__

#include <dcopstub.h>
#include <ksslcertdlg.h>
#include <klistview.h>
#include <kdatastream.h>
#include <kmainwindow.h>
#include <kurl.h>
#include <kio/authinfo.h>
#include <kio/global.h>
#include <dcopobject.h>
#include <qtimer.h>
#include <qdatetime.h>
#include <qintdict.h>


class KIO_EXPORT UIServer_stub : virtual public DCOPStub
{
public:
    UIServer_stub( const QCString& app, const QCString& id );
    UIServer_stub( DCOPClient* client, const QCString& app, const QCString& id );
    explicit UIServer_stub( const DCOPRef& ref );
    virtual int newJob( QCString appId, bool showProgress );
    virtual ASYNC jobFinished( int id );
    virtual ASYNC totalSize( int id, unsigned long int size );
    virtual ASYNC totalSize64( int id, KIO::filesize_t size );
    virtual ASYNC totalFiles( int id, unsigned long int files );
    virtual ASYNC totalDirs( int id, unsigned long int dirs );
    virtual ASYNC processedSize( int id, unsigned long int bytes );
    virtual ASYNC processedSize64( int id, KIO::filesize_t bytes );
    virtual ASYNC processedFiles( int id, unsigned long int files );
    virtual ASYNC processedDirs( int id, unsigned long int dirs );
    virtual ASYNC percent( int id, unsigned long int ipercent );
    virtual ASYNC speed( int id, unsigned long int bytes_per_second );
    virtual ASYNC infoMessage( int id, const QString& msg );
    virtual ASYNC copying( int id, KURL from, KURL to );
    virtual ASYNC moving( int id, KURL from, KURL to );
    virtual ASYNC deleting( int id, KURL url );
    virtual ASYNC transferring( int id, KURL url );
    virtual ASYNC creatingDir( int id, KURL dir );
    virtual ASYNC stating( int id, KURL url );
    virtual ASYNC mounting( int id, QString dev, QString point );
    virtual ASYNC unmounting( int id, QString point );
    virtual ASYNC canResume( int id, unsigned long int offset );
    virtual ASYNC canResume64( int id, KIO::filesize_t offset );
    virtual QByteArray openPassDlg( const KIO::AuthInfo& info );
    virtual int messageBox( int id, int type, const QString& text, const QString& caption, const QString& buttonYes, const QString& buttonNo );
    virtual QByteArray open_RenameDlg64( int id, const QString& caption, const QString& src, const QString& dest, int mode, KIO::filesize_t sizeSrc, KIO::filesize_t sizeDest, unsigned long int ctimeSrc, unsigned long int ctimeDest, unsigned long int mtimeSrc, unsigned long int mtimeDest );
    virtual QByteArray open_RenameDlg( int id, const QString& caption, const QString& src, const QString& dest, int mode, unsigned long int sizeSrc, unsigned long int sizeDest, unsigned long int ctimeSrc, unsigned long int ctimeDest, unsigned long int mtimeSrc, unsigned long int mtimeDest );
    virtual int open_SkipDlg( int id, int multi, const QString& error_text );
    virtual void setListMode( bool list );
    virtual void setJobVisible( int id, bool visible );
    virtual void showSSLInfoDialog( const QString& url, const KIO::MetaData& data, int mainwindow );
    virtual void showSSLInfoDialog( const QString& url, const KIO::MetaData& data );
    virtual KSSLCertDlgRet showSSLCertDialog( const QString& host, const QStringList& certList, int mainwindow );
    virtual KSSLCertDlgRet showSSLCertDialog( const QString& host, const QStringList& certList );
protected:
    UIServer_stub() : DCOPStub( never_use ) {}
};


#endif
