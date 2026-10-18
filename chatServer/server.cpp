#include "server.h"
#include "./ui_server.h"

Server::Server(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Server)
{
    ui->setupUi(this);
    tcpServer = new QTcpServer(this);
    connect(tcpServer, &QTcpServer::newConnection, this, &Server::onNewConnection);
    if (tcpServer->listen(QHostAddress::Any, 1234)){
        qDebug() << "Server started!";
    }
    else{
        qDebug() << "Starting error: " << tcpServer->errorString();
    }
}

Server::~Server()
{
    delete ui;
}

void Server::onNewConnection(){

    QTcpSocket *clientSocket = tcpServer->nextPendingConnection();

    clients.append(clientSocket);

    connect(clientSocket, &QTcpSocket::readyRead, this, &Server::onReadyRead);
    connect(clientSocket, &QTcpSocket::disconnected, this, &Server::onDisconnected);

    qDebug() << "New client just entered the chat!";
}

void Server::onReadyRead(){

    QTcpSocket *clientSocket = (QTcpSocket*)sender();

    QByteArray data = clientSocket->readAll();

    qDebug() << "Message recieved: " << data;
    // Переводим байты в текст и выводим прямо в интерфейс сервера
    ui->textEdit->append(QString::fromUtf8(data));

    for(int i=0; i<clients.size(); i++){
        clients[i]->write(data);
    }
}

void Server::onDisconnected(){

    QTcpSocket *clientSocket = (QTcpSocket*)sender();

    qDebug() << "Someone just disconnected!";

    clients.removeOne(clientSocket);

    clientSocket->deleteLater();
}

void Server::on_pushButton_clicked()
{

}

