#ifndef SERVERWINDOW_H
#define SERVERWINDOW_H

#include <QWidget>
#include <QFileDialog>
#include <QTcpSocket>
#include <QTcpServer>
#include <QDebug>
#include <QMouseEvent>

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
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    int m_nMouseClick_X_Coordinate;
    int m_nMouseClick_Y_Coordinate;

private slots:
    void on_pushButton_connect_clicked();

    void on_pushButton_browser_clicked();

    void on_pushButton_close_clicked();

private:
    Ui::ServerWindow *ui;
};

#endif // SERVERWINDOW_H
