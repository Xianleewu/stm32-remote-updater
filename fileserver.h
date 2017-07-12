#ifndef __FILE_SERVER_H__
#define __FILE_SERVER_H__

#include <QByteArray>
#include <qmhdcontroller.h>

class FileServer : public QMHDController
{
    Q_OBJECT

public:
    Q_INVOKABLE
    FileServer(QObject* parent = NULL);
    ~FileServer();

    int setFile(QString file);
private:
    QByteArray fileByte;

public slots:
    void sendFile();
};

#endif // __FILE_SERVER_H__
