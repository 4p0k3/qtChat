#include "networkmanager.h"
#include <QMessageBox>

NetworkManager::NetworkManager(QObject *parent) : QObject(parent), ip("127.0.0.1"), socket(nullptr), tcpServer(nullptr) {
}
//CLIENT METHODS
void NetworkManager::joinChat(QString ip, quint16 port){
    if (socket == nullptr) {
        socket = new QTcpSocket(this);
        connect(socket, &QTcpSocket::readyRead, this, &NetworkManager::onReadyRead);
        connect(socket, &QTcpSocket::connected, this, &NetworkManager::onConnection);
        connect(socket, &QTcpSocket::disconnected, this, &NetworkManager::onDisconnection);
    }
    socket->connectToHost(ip, port);
}

void NetworkManager::leaveChat(){
    if (socket != nullptr) {
        socket->disconnectFromHost();
        socket->deleteLater();
        socket = nullptr;
    }

    if (tcpServer != nullptr && tcpServer->isListening()) {
        tcpServer->close();

        while (!clients.isEmpty()) {
            QTcpSocket *guest = clients.takeFirst();
            guest->disconnectFromHost();
            guest->deleteLater();
        }
    }

    emit actionSuccessful(LeftChat);
}

void NetworkManager::createChat(QString ip){
    if (tcpServer == nullptr) {
        tcpServer = new QTcpServer(this);
        connect(tcpServer, &QTcpServer::newConnection, this, &NetworkManager::onNewConnection);
    }
    if (tcpServer->listen(QHostAddress::Any, 0)){
        qDebug() << "Server started!";
        QHostAddress currentIP = tcpServer->serverAddress();
        quint16 currentPort = tcpServer->serverPort();

        isHostMode = true;

        emit newMessageRecieved(QString("Server started on %1:%2!").arg(ip).arg(currentPort));
        joinChat(ip, currentPort);
    }
    else{
        qDebug() << "Starting error: " << tcpServer->errorString();
    }
}

void NetworkManager::onReadyRead(){
    QByteArray data = socket->readAll();
    QString text = QString::fromUtf8(data).trimmed();
    if (text.isEmpty()) {
        return;
    }
    emit newMessageRecieved(text);
}

void NetworkManager::sendMessage(QString text)
{
    if (socket != nullptr && socket->isOpen()) {
        socket->write(text.toUtf8());
    } else {
        emit newMessageRecieved("You aren't connected to the chat!");
    }
}

void NetworkManager::onServerReadyRead()
{
    QTcpSocket *clientSocket = (QTcpSocket*)sender();

    QByteArray data = clientSocket->readAll();

    for(int i = 0; i < clients.size(); i++) {
        clients[i]->write(data);
    }
}

void NetworkManager::onNewConnection(){
    QTcpSocket *clientSocket = tcpServer->nextPendingConnection();

    clients.append(clientSocket);
    connect(clientSocket, &QTcpSocket::readyRead, this, &NetworkManager::onServerReadyRead);
    connect(clientSocket, &QTcpSocket::disconnected, this, &NetworkManager::onDisconnected);

    QString sysMsg = "A new user connected!";
    for(int i = 0; i < clients.size(); i++) {
        clients[i]->write(sysMsg.toUtf8());
    }
}

void NetworkManager::onDisconnected(){
    QTcpSocket *clientSocket = (QTcpSocket*)sender();
    clients.removeOne(clientSocket);
    clientSocket->deleteLater();

    QString sysMsg = "A user disconnected";
    for(int i = 0; i < clients.size(); i++) {
        clients[i]->write(sysMsg.toUtf8());
    }
}

void NetworkManager::onConnection()
{
    qDebug() << "Connected successfully!";
    emit actionSuccessful(JoinedChat);
}

void NetworkManager::onDisconnection()
{
    qDebug() << "Disconnected from server.";
}

bool NetworkManager::isHost() const{
    return isHostMode;
}