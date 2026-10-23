#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include <QTcpSocket>
#include <QTcpServer>
#include <QObject>
#include <QString>

enum ActionType{
    JoinedChat,
    LeftChat
};


class NetworkManager : public QObject
{
    Q_OBJECT

public:
    NetworkManager(QObject *parent = nullptr);
    void sendMessage(QString text);
    void joinChat(QString ip, quint16 port);
    void createChat(QString ip);
    void leaveChat();
    bool isHost() const;
    void setNickname(QString name);



signals:
    void newMessageRecieved(QString text);
    void actionSuccessful(ActionType type);
    void messageParsed(const QString &timestamp, const QString &sender, const QString &text);
private:
    QTcpSocket *socket;
    QTcpServer *tcpServer;
    QList<QTcpSocket*> clients;


    QString ip;
    quint16 port;

    QString myNickname;
    bool isHostMode {};

    void initDatabase();

private slots:
    void onReadyRead();
    void onNewConnection();
    void onDisconnected();
    void onServerReadyRead();
    void onConnection();
    void onDisconnection();
};

#endif // NETWORKMANAGER_H
