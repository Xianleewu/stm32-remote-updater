#ifndef SERVERWINDOW_H
#define SERVERWINDOW_H

#include <QWidget>
#include <QFileDialog>
#include <QTcpSocket>
#include <QTcpServer>
#include <QDebug>
#include <QCryptographicHash>
#include <QtMsgHandler>
#include <QMouseEvent>
#include <QJsonObject>
#include <QJsonDocument>
#include <assert.h>
#include <QFile>

#define FILE_BLOCK_SIZE (1024)

namespace Ui {
class ServerWindow;
}

class ServerWindow : public QWidget
{
    Q_OBJECT

public:
    explicit ServerWindow(QWidget *parent = 0);
    ~ServerWindow();
private:
    QTcpServer* mTcpServer;
    QTcpSocket* mTcpSocket;
    QFile* mFile;

    QString mUpdateCmd;

    quint64 mFileSize;
    quint64 mFileSent;

    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    int m_nMouseClick_X_Coordinate;
    int m_nMouseClick_Y_Coordinate;

    void generateUpdateCmd(QString filepath);

    bool mFinished;

private slots:
    void on_pushButton_browser_clicked();

    void on_pushButton_close_clicked();

    void on_pushButton_min_clicked();

    void on_pushButton_bind_clicked();

    void on_pushButton_clear_clicked();

    void new_client_request();

    void new_client_connected();

    void got_new_data();

    void on_pushButton_clear_in_clicked();

    void on_pushButton_send_clicked();

    void on_socket_wirten(qint64);

    void on_pushButton_send_file_clicked();

    void on_pushButton_update_clicked();

private:
    Ui::ServerWindow *ui;
};

#endif // SERVERWINDOW_H
