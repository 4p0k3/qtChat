#include "networkmanager.h"
#include <QJsonObject>
#include <QJsonDocument>
#include <QDataStream>
#include <QDateTime>
#include <QJsonParseError>
#include <QDebug>

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

void NetworkManager::createChat(QString ip)
{
    if (tcpServer == nullptr) {
        tcpServer = new QTcpServer(this);
        connect(tcpServer, &QTcpServer::newConnection, this, &NetworkManager::onNewConnection);
    }

    if (tcpServer->listen(QHostAddress::Any, 0)){
        quint16 currentPort = tcpServer->serverPort();
        isHostMode = true;


        emit newMessageRecieved(QString("Server started on %1:%2!").arg(ip).arg(currentPort));
        joinChat("127.0.0.1", currentPort);
    }
    else {
        qDebug() << "Starting error: " << tcpServer->errorString();
    }
}

void NetworkManager::onReadyRead(){
    QDataStream in(socket);
    in.setVersion(QDataStream::Qt_6_0);

    while (true) {
        in.startTransaction();
        QByteArray data;
        in >> data;

        if (!in.commitTransaction()){
            break; // Если пакет не докачался - ждем дальше
        }

        QJsonParseError error;
        QJsonDocument doc = QJsonDocument::fromJson(data, &error);

        if (error.error == QJsonParseError::NoError && doc.isObject()) {
            QJsonObject json = doc.object();
            QString type = json["type"].toString();

            if (type == "message"){
                QString senderName = json["sender"].toString();
                QString text = json["text"].toString();

                // Читаем время
                qint64 timestamp = json["timestamp"].toVariant().toLongLong();
                QDateTime time = QDateTime::fromMSecsSinceEpoch(timestamp);
                QString timeString = time.toString("HH:mm:ss");

                QString formattedMessage = "[" + timeString + "] " + senderName + ": " + text;
                emit newMessageRecieved(formattedMessage);
                emit messageParsed(timeString, senderName, text);
            }
        }
        else{
            QString text = QString::fromUtf8(data).trimmed();
            if (!text.isEmpty()) {
                emit newMessageRecieved(text);

            }
        }
    }
}

void NetworkManager::sendMessage(QString text)
{
    if (socket != nullptr && socket->isOpen()) {
        QJsonObject json;
        json["type"] = "message";
        json["sender"] = myNickname;
        json["text"] = text;
        QJsonDocument doc(json);
        QByteArray data = doc.toJson(QJsonDocument::Compact);

        QDataStream out(socket);
        out.setVersion(QDataStream::Qt_6_0);
        out << data;
    } else {
        emit newMessageRecieved("You aren't connected to the chat!");

    }
}

void NetworkManager::onServerReadyRead()
{
    QTcpSocket *clientSocket = (QTcpSocket*)sender();

    QDataStream in(clientSocket);
    in.setVersion(QDataStream::Qt_6_0);

    while (true) {
        in.startTransaction();
        QByteArray data;
        in >> data;

        if (!in.commitTransaction()) {
            break;
        }

        // Вскрываем пакет от клиента
        QJsonParseError error;
        QJsonDocument doc = QJsonDocument::fromJson(data, &error);

        if (error.error == QJsonParseError::NoError && doc.isObject()) {
            QJsonObject json = doc.object();

            // СЕРВЕР СТАВИТ ЕДИНОЕ МИРОВОЕ ВРЕМЯ!
            json["timestamp"] = QDateTime::currentMSecsSinceEpoch();

            // Упаковываем обратно
            QJsonDocument newDoc(json);
            QByteArray newData = newDoc.toJson(QJsonDocument::Compact);

            // Рассылаем всем гостям через умную трубу (Framing)
            for(int i = 0; i < clients.size(); i++) {
                QDataStream out(clients[i]);
                out.setVersion(QDataStream::Qt_6_0);
                out << newData;
            }
        }
    }
}

void NetworkManager::onNewConnection(){
    QTcpSocket *clientSocket = tcpServer->nextPendingConnection();

    clients.append(clientSocket);
    connect(clientSocket, &QTcpSocket::readyRead, this, &NetworkManager::onServerReadyRead);
    connect(clientSocket, &QTcpSocket::disconnected, this, &NetworkManager::onDisconnected);

    QString sysMsg = "A new user connected!";
    for(int i = 0; i < clients.size(); i++) {
        QDataStream out(clients[i]);
        out.setVersion(QDataStream::Qt_6_0);
        out << sysMsg.toUtf8();
    }
}

void NetworkManager::onDisconnected(){
    QTcpSocket *clientSocket = (QTcpSocket*)sender();
    clients.removeOne(clientSocket);
    clientSocket->deleteLater();

    QString sysMsg = "A user disconnected";
    for(int i = 0; i < clients.size(); i++) {
        QDataStream out(clients[i]);
        out.setVersion(QDataStream::Qt_6_0);
        out << sysMsg.toUtf8();
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
void NetworkManager::setNickname(QString name){
    if (name.isEmpty() || name.trimmed() == "") {
        myNickname = "Anonymous";
    } else {
        myNickname = name;
    }
}
