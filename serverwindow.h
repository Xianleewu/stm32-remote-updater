#ifndef SERVERWINDOW_H
#define SERVERWINDOW_H

#include <QWidget>
#include <QFileDialog>
#include <QTcpSocket>
#include <QTcpServer>
#include <QDebug>
#include <QCryptographicHash>
#include <QHash>
#include <QtMsgHandler>
#include <QMouseEvent>
#include <QJsonObject>
#include <QJsonDocument>
#include <QMessageBox>
#include <assert.h>
#include <QFile>
#include <socketclients.h>
#include <fileserver.h>

#define FILE_BLOCK_SIZE (1024*16)
#define FILE_NAME_NONE  ""
#define CMD_TYPE_UPDATE "update"
#define CMD_TYPE_RUNAPP "runapp"
#define CMD_TYPE_LOADER "loader"

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
    QFile* mFile;
    FileServer* mFilerServer;

    QString mUpdateCmd;

    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    int m_nMouseClick_X_Coordinate;
    int m_nMouseClick_Y_Coordinate;

    void generateUpdateCmd(QString cmd, QString filepath);
    int destroyHttpd();

    void showMessageNoFile();
    void showMessageNoClient();

private slots:
    void on_pushButton_browser_clicked();

    void on_pushButton_close_clicked();

    void on_pushButton_min_clicked();

    void on_pushButton_clear_clicked();

    void on_pushButton_httpd_clicked();

    void on_pushButton_setVersion_clicked();

private:
    Ui::ServerWindow *ui;
};

#endif // SERVERWINDOW_H
