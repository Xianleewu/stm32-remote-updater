#include <QCoreApplication>
#include <QThread>
#include <QTimer>
#include <QFile>
#include <QByteArray>
#include <QDebug>
#include <QCryptographicHash>

#include <qmhdrequest.h>
#include <qmhdresponse.h>
#include <qmhdrouter.h>
#include <qmhdserver.h>

#include "fileserver.h"

FileServer::FileServer(QObject* parent)
    : QMHDController(parent)
{

}

FileServer::~FileServer()
{

}

int FileServer::setFile(QString filepath)
{
    QFile file(filepath);
    if(!file.open(QIODevice::ReadOnly)) {
        qDebug()<<tr("Failed to open file");
        return -1;
    }

    fileByte = file.readAll();
    QString md5 = QString(QCryptographicHash::hash(fileByte, QCryptographicHash::Md5).toHex());
    qDebug()<<md5;

    file.close();

    return 0;
}

void FileServer::sendFile()
{
    response()->setHeader("Content-Type", "application/octet-stream");
    response()->setHeader("X-Thread-Id", QString::number((quint64) QThread::currentThreadId()));
    response()->send(fileByte);
}

void startRouter()
{
    QMHDServer* server = new QMHDServer(qApp);
    QMHDRouter* router = new QMHDRouter(qApp);
    FileServer* fileserver = new FileServer(router);

    fileserver->setFile("/home/xianlee/doc/wpa.conf");

    router->addRoute("GET", "/firmware.bin", fileserver, SLOT(sendFile()));
    router->connect(server, &QMHDServer::newRequest,
                    router, &QMHDRouter::processRequest,
                    Qt::DirectConnection);
    if (!server->listen(8181))
        exit(-1);
}
