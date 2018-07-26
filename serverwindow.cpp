#include "serverwindow.h"
#include "ui_serverwindow.h"
#include <QSettings>
#include "QThread"

ServerWindow::ServerWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ServerWindow)
{
    ui->setupUi(this);
    this->setWindowTitle(tr("STM32 Remote Update Server"));

    mFile = NULL;
    mFilerServer = NULL;
    mUpdateCmd = tr("select file first");

    QSettings setting(QString::fromStdString("config"),QSettings::IniFormat);
    //此处只拿用一组举例，有多个可以自行加设循环
    setting.beginGroup("updateversion");
    QString hardversion = setting.value("hardversion").toString();
    QString softversion = setting.value("softversion").toString();
    setting.endGroup();

    ui->lineEdit_hardversion->setText(hardversion);
    ui->lineEdit_softversion->setText(softversion);
}

ServerWindow::~ServerWindow()
{
    delete ui;
    destroyHttpd();
}

void ServerWindow::mousePressEvent(QMouseEvent *event) {
    m_nMouseClick_X_Coordinate = event->x();
    m_nMouseClick_Y_Coordinate = event->y();
}

void ServerWindow::mouseMoveEvent(QMouseEvent *event) {
    move(event->globalX()- m_nMouseClick_X_Coordinate,event->globalY()-m_nMouseClick_Y_Coordinate);
}

void ServerWindow::generateUpdateCmd(QString cmd, QString filepath)
{
    QJsonObject json;
    json.insert(QString("cmd"), QString(cmd));

    QFile file(filepath);
    if(filepath.isEmpty() || !file.open(QIODevice::ReadOnly)) {
        json.insert("url", "NONE");
        json.insert("md5", "NONE");
    } else {
        QByteArray fileByte = file.readAll();
        QString md5 = QString(QCryptographicHash::hash(fileByte, QCryptographicHash::Md5).toHex());
        file.close();

        json.insert("url", ui->lineEdit_FileUrl->text());
        json.insert("md5", md5);
    }

    QJsonDocument document;
    document.setObject(json);
    QByteArray byte_array = document.toJson(QJsonDocument::Compact);
    mUpdateCmd = QString(byte_array);

    if(0 == QString::compare(QString(CMD_TYPE_UPDATE), cmd)) {
        ui->lineEdit_updatecmd->setText(mUpdateCmd);
        ui->lineEdit_updatecmd->show();
    }
}

int ServerWindow::destroyHttpd()
{
    if(mFilerServer) {
        mFilerServer->stopServer();
        delete mFilerServer;
        mFilerServer = NULL;
        return 0;
    }

    return -1;
}

void ServerWindow::showMessageNoFile()
{
    QMessageBox::warning(NULL, "warning",
                         "Please set update file first!",
                         QMessageBox::Yes, QMessageBox::Yes);
}

void ServerWindow::showMessageNoClient()
{
    QMessageBox::warning(NULL, "warning",
                         "Please select a valid client",
                         QMessageBox::Yes, QMessageBox::Yes);
}

void ServerWindow::on_pushButton_browser_clicked()
{
    if (!mFilerServer) {
        QMessageBox::warning(NULL, "warning",
                             "Please open http server first",
                             QMessageBox::Yes, QMessageBox::Yes);
        return;
    }

    QStringList filters;
    QFileDialog* tFileDialog = new QFileDialog(this);

    filters << "Binary files (*.bin)";

    tFileDialog->setWindowTitle(tr("Select update binary file"));
    tFileDialog->setAcceptMode(QFileDialog::AcceptOpen);
    tFileDialog->setFileMode(QFileDialog::ExistingFile);
    tFileDialog->setViewMode(QFileDialog::Detail);
    tFileDialog->setNameFilters(filters);
    //tFileDialog->setDirectory(QDir::currentPath());

    if(tFileDialog->exec() == QDialog::Accepted) {
        QString path = tFileDialog->selectedFiles()[0];

        mFilerServer->setFile(path);

        generateUpdateCmd(CMD_TYPE_UPDATE, path);
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

void ServerWindow::on_pushButton_clear_clicked()
{
    ui->textBrowser->clear();
}

void ServerWindow::on_pushButton_httpd_clicked()
{
    if(0 != destroyHttpd()) {
        mFilerServer = new FileServer();

        int ret = mFilerServer->startServer(ui->lineEdit_HttpdPort->text().toInt());
        if (ret < 0) {
            QMessageBox::critical(NULL, "Error",
                                  "Open http server failed",
                                  QMessageBox::Yes, QMessageBox::Yes);
        }

        mFilerServer->setVersion(ui->lineEdit_hardversion->text().toInt(),
                                 ui->lineEdit_softversion->text().toInt());
    }
}

void ServerWindow::on_pushButton_setVersion_clicked()
{
    if (!mFilerServer) {
        QMessageBox::warning(NULL, "warning",
                             "Please open update server first!",
                             QMessageBox::Yes, QMessageBox::Yes);
        return;
    }

    mFilerServer->setVersion(ui->lineEdit_hardversion->text().toInt(),
                             ui->lineEdit_softversion->text().toInt());

    //设置setting
    QSettings setting(QString::fromStdString("config"), QSettings::IniFormat);
    //从配置文件中读全部的Group
    setting.beginGroup("updateversion");
    //存数据前要保存成QString形式的
    setting.setValue("hardversion", ui->lineEdit_hardversion->text());
    setting.setValue("softversion", ui->lineEdit_softversion->text());
    setting.endGroup();
}
