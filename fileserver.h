#ifndef __FILE_SERVER_H__
#define __FILE_SERVER_H__

#include <QByteArray>
#include <QMutex>
#include <qmhdcontroller.h>
#include <qmhdrequest.h>
#include <qmhdresponse.h>
#include <qmhdrouter.h>
#include <qmhdserver.h>

class FileServer : public QMHDController
{
    Q_OBJECT

public:
    Q_INVOKABLE
    FileServer(QObject* parent = NULL);
    ~FileServer();

    int startServer(int port);
    int stopServer();
    int setFile(QString file);
    void setVersion(int hard, int soft);
    static QString mFirmware;
private:
    QMHDServer* mServer;
    QMHDRouter* mRouter;
    QMutex mMutex;
    QMutex versionMutex;

    static int hardVersion;
    static int softVersion;

public slots:
    void sendFile();
    void sendVersion();
};

#endif // __FILE_SERVER_H__
