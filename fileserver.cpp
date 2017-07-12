#include <QCoreApplication>
#include <QThread>
#include <QTimer>
#include <QFile>
#include <QByteArray>
#include <QDebug>
#include <QCryptographicHash>
#include "fileserver.h"

FileServer::FileServer(QObject* parent)
    : QMHDController(parent)
{
    mServer = NULL;
    mRouter = NULL;
    mFilePath = NULL;
}

FileServer::~FileServer()
{

}

FileServer *FileServer::startServer(int port)
{
    QMHDServer* server = new QMHDServer(qApp);
    QMHDRouter* router = new QMHDRouter(qApp);
    FileServer* fileserver = new FileServer(router);

    router->addRoute("GET", "/firmware.bin", fileserver, SLOT(sendFile()));
    router->connect(server, &QMHDServer::newRequest,
                    router, &QMHDRouter::processRequest,
                    Qt::DirectConnection);

    if (!server->listen(port)) {
        qDebug()<<tr("Failed to create server");
    }

    return fileserver;
}

int FileServer::stopServer()
{
    if(mServer) {
        mServer->close();
        mServer->deleteLater();
    }

    if(mRouter) {
        mRouter->deleteLater();
    }

    return 0;
}

int FileServer::setFile(QString *filepath)
{
    mFilePath = filepath;

    return 0;
}

void FileServer::sendFile()
{
    QFile file(*mFilePath);
    if(!file.open(QIODevice::ReadOnly)) {
        qDebug()<<tr("Failed to open file:%1").arg(*mFilePath);
        return;
    }

    QByteArray fileByte = file.readAll();

    file.close();

    response()->setHeader("Content-Type", "application/octet-stream");
    response()->setHeader("X-Thread-Id", QString::number((quint64) QThread::currentThreadId()));
    response()->send(fileByte);
}
