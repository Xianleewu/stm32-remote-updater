#ifndef __FILE_SERVER_H__
#define __FILE_SERVER_H__

#include <QByteArray>
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

    static FileServer* startServer(int port);
    int stopServer();
    int setFile(QString* file);
private:
    QMHDServer* mServer;
    QMHDRouter* mRouter;

    QString* mFilePath;

public slots:
    void sendFile();
};

#endif // __FILE_SERVER_H__
