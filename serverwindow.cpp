#include "serverwindow.h"
#include "ui_serverwindow.h"

ServerWindow::ServerWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ServerWindow)
{
    ui->setupUi(this);
    this->setWindowTitle(tr("STM32 Remote Update Server"));

    mTcpServer = new QTcpServer(this);
    assert(mTcpServer);

    connect(mTcpServer, SIGNAL(newConnection()), this, SLOT(new_client_request()));
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

void ServerWindow::on_pushButton_min_clicked()
{
    this->showMinimized();
}

void ServerWindow::on_pushButton_bind_clicked()
{
    if(!mTcpServer->listen(QHostAddress::Any, ui->lineEdit_port->text().toInt()))
    {
        mTcpServer->close();
        qDebug() << tr("listen error!");
    }

    ui->textBrowser->insertPlainText(tr("TCP server created!\n"));
    ui->pushButton_bind->setFocus();
}

void ServerWindow::on_pushButton_clear_clicked()
{
    ui->textBrowser->clear();
}

void ServerWindow::new_client_request()
{
    QTcpSocket* new_client = mTcpServer->nextPendingConnection();

    connect(new_client, SIGNAL(connected()), this, SLOT(new_client_connected()));
    connect(new_client, SIGNAL(readyRead()), this, SLOT(got_new_data()));
    connect(new_client, &QAbstractSocket::disconnected,new_client, &QObject::deleteLater);
}

void ServerWindow::new_client_connected()
{
    ui->textBrowser->insertPlainText(tr("New client connected!\n"));
}

void ServerWindow::got_new_data()
{
    ui->textBrowser->insertPlainText(tr("Got new data!\n"));
}
