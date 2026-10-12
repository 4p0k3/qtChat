#include "client.h"
#include "./ui_client.h"

Client::Client(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Client)
{
    ui->setupUi(this);

    socket = new QTcpSocket(this);
    connect(socket, &QTcpSocket::readyRead, this, &Client::onReadyRead);

    socket->connectToHost("127.0.0.1",1234);
}

Client::~Client()
{
    delete ui;
}

void Client::on_pushButton_clicked()
{
    QString message = ui->lineEdit_2->text();

    socket->write(message.toUtf8());
    ui->lineEdit_2->clear();
}

void Client::onReadyRead(){
    QByteArray data = socket->readAll();
    ui->textEdit_2->append(QString::fromUtf8(data));
}

