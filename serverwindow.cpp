#include "serverwindow.h"
#include "ui_serverwindow.h"
#include "QThread"

ServerWindow::ServerWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ServerWindow)
{
    ui->setupUi(this);
    this->setWindowTitle(tr("STM32 Remote Update Server"));

    mFile = NULL;
    mFinished = true;
    mTcpSocket = NULL;
    mFileSize = 0;
    mFileSent = 0;
    mUpdateCmd = tr("select file first");

    mTcpServer = new QTcpServer(this);
    ui->progressBar_tcp->setMaximum(100);
    ui->progressBar_tcp->setMinimum(0);
    ui->progressBar_tcp->setValue(0);

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

void ServerWindow::generateUpdateCmd(QString filepath)
{
    QFile file(filepath);
    if(!file.open(QIODevice::ReadOnly)) {
        qDebug()<<tr("Failed to open file");
        return;
    }

    QByteArray fileByte = file.readAll();
    QString md5 = QString(QCryptographicHash::hash(fileByte, QCryptographicHash::Md5).toHex());
    qDebug()<<md5;

    QJsonObject json;
    json.insert("cmd", QString("update"));
    json.insert("url", ui->lineEdit_FileUrl->text());
    json.insert("md5", md5);

    QJsonDocument document;
    document.setObject(json);
    QByteArray byte_array = document.toJson(QJsonDocument::Compact);
    mUpdateCmd = QString(byte_array);
    ui->lineEdit_updatecmd->setText(mUpdateCmd);
    ui->lineEdit_updatecmd->show();

    qDebug()<<mUpdateCmd;

    file.close();
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
        generateUpdateCmd(path);
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
        return;
    }

    ui->pushButton_bind->setEnabled(false);
    ui->textBrowser->insertPlainText(tr("TCP server created!\n"));
}

void ServerWindow::on_pushButton_clear_clicked()
{
    ui->textBrowser->clear();
}

void ServerWindow::new_client_request()
{
    mTcpSocket = mTcpServer->nextPendingConnection();

    QString peerinfo = tr("%1:%2").arg(mTcpSocket->peerAddress().toString()).arg(mTcpSocket->peerPort());

    qDebug() << peerinfo;
    mSocketClients.addNewClient(peerinfo, mTcpSocket);
    ui->comboBox->addItem(peerinfo);

    ui->textBrowser->insertPlainText(tr("New client connected!\n"));

    connect(mTcpSocket, SIGNAL(connected()), this, SLOT(new_client_connected()));
    connect(mTcpSocket, SIGNAL(readyRead()), this, SLOT(got_new_data()));
    connect(mTcpSocket, SIGNAL(disconnected()), mTcpSocket, SLOT(deleteLater()));
    connect(mTcpSocket, SIGNAL(bytesWritten(qint64)), this, SLOT(on_socket_wirten(qint64)));
}

void ServerWindow::new_client_connected()
{
    ui->textBrowser->insertPlainText(tr("New client connected!\n"));
}

void ServerWindow::got_new_data()
{
    QString res;
    res += mTcpSocket->readAll();
    ui->textBrowser->insertPlainText(res);
}

void ServerWindow::on_pushButton_clear_in_clicked()
{
    ui->lineEdit_out->clear();
}

void ServerWindow::on_pushButton_send_clicked()
{
    if(!mTcpSocket) {
        ui->textBrowser->insertPlainText(tr("No client yet!!!\n"));
    } else {
        QTextStream tOutStream(mTcpSocket);
        tOutStream << ui->lineEdit_out->text();
    }
}

void ServerWindow::on_socket_wirten(qint64)
{
    QThread::sleep(5);
    if(mFinished) {
        return;
    }

    if(!mTcpSocket) {
        ui->textBrowser->insertPlainText(tr("No client yet!\n"));
        return;
    }

    if(!mFile) {
        ui->textBrowser->insertPlainText(tr("No client yet!\n"));
        return;
    }

    char tFileBuf[FILE_BLOCK_SIZE] = {0};
    int readLen =  mFile->read(tFileBuf, FILE_BLOCK_SIZE);
    if(readLen != FILE_BLOCK_SIZE) {
        mFinished = true;
        ui->progressBar_tcp->setValue(100);
    }

    mTcpSocket->write(tFileBuf, readLen);

    mFileSent += readLen;
    ui->progressBar_tcp->setValue(100 * (mFileSent/mFileSize));
}

void ServerWindow::on_pushButton_send_file_clicked()
{
    mFile = new QFile(ui->lineEdit_file->text());
    if(!(mFile->open(QFile::ReadOnly))){
        ui->textBrowser->insertPlainText(tr("Please input file path!\n"));
    } else {
        ui->progressBar_tcp->setValue(0);
        ui->textBrowser->insertPlainText(tr("Successed to open update file!\n"));
        mFinished = false;
        mFileSize = mFile->size();
        mFileSent = 0;
        on_socket_wirten(0);
    }
}

void ServerWindow::on_pushButton_update_clicked()
{
    if(!mTcpSocket) {
        ui->textBrowser->insertPlainText(tr("No client yet!!!\n"));
    } else {
        QTextStream tOutStream(mTcpSocket);
        tOutStream << tr("update");
    }
}

void ServerWindow::on_comboBox_currentIndexChanged(const QString &arg1)
{
    qDebug() << arg1;
    mTcpSocket = mSocketClients.getClientFromInfo(arg1);
    if(mTcpSocket != nullptr) {
        qDebug() << mTcpSocket;
    } else {
        qDebug() << tr("No clients yet");
    }
}
