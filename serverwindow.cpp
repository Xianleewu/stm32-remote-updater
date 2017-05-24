#include "serverwindow.h"
#include "ui_serverwindow.h"

ServerWindow::ServerWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ServerWindow)
{
    ui->setupUi(this);
    this->setWindowTitle(tr("STM32 Remote Update Server"));

    mTcpServer = new QTcpServer(this);
}

ServerWindow::~ServerWindow()
{
    delete ui;
}

void ServerWindow::mousePressEvent(QMouseEvent *event) {
    m_nMouseClick_X_Coordinate = event->x();
    m_nMouseClick_Y_Coordinate = event->y();
}

void ServerWindow::mouseMoveEvent(QMouseEvent *event) {
    move(event->globalX()- m_nMouseClick_X_Coordinate,event->globalY()-m_nMouseClick_Y_Coordinate);
}

void ServerWindow::on_pushButton_connect_clicked()
{

}

void ServerWindow::on_pushButton_browser_clicked()
{
    QFileDialog* tFileDialog = new QFileDialog(this);
    tFileDialog->setWindowTitle(tr("Select update binary file"));
    tFileDialog->setAcceptMode(QFileDialog::AcceptOpen);
    tFileDialog->setFileMode(QFileDialog::AnyFile);
    tFileDialog->setViewMode(QFileDialog::Detail);
    tFileDialog->setDirectory(".");

    if(tFileDialog->exec() == QDialog::Accepted) {
        QString path = tFileDialog->selectedFiles()[0];
        qDebug() << path;
        ui->lineEdit_file->setText(path);
        ui->lineEdit_file->show();
    }

    delete tFileDialog;
}

void ServerWindow::on_pushButton_close_clicked()
{
    this->close();
}
