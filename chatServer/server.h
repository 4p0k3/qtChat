#ifndef SERVER_H
#define SERVER_H

#include <QMainWindow>

#include <QTcpServer>
#include <QTcpSocket>

QT_BEGIN_NAMESPACE
namespace Ui {
class Server;
}
QT_END_NAMESPACE

class Server : public QMainWindow
{
    Q_OBJECT

public:
    explicit Server(QWidget *parent = nullptr);
    ~Server() override;

private slots:
    void onNewConnection();
    void onReadyRead();
    void onDisconnected();
    void on_pushButton_clicked();

private:
    Ui::Server *ui;
    QTcpServer *tcpServer;
    QList<QTcpSocket*> clients;
};
#endif // SERVER_H
