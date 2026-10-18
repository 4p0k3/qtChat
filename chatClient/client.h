#ifndef CLIENT_H
#define CLIENT_H

#include <QMainWindow>

#include <QTcpSocket>

#include "networkmanager.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class Client;
}
QT_END_NAMESPACE

class Client : public QMainWindow
{
    Q_OBJECT

public:
    explicit Client(QWidget *parent = nullptr);
    ~Client() override;
private slots:
    void onNewMessageRecieved(QString text);
    void onActionSuccessful(ActionType type);

    void on_btnCreateChat_clicked();

    void on_btnConnect_clicked();

    void on_btnMenu_clicked();

    void on_btnSendMessage_clicked();

    void on_btnDeleteChat_clicked();

    void on_btnReturn_clicked();

    void on_btnLeaveChat_clicked();

private:
    Ui::Client *ui;
    QString myNickname;
    NetworkManager *networkManager;
};
#endif // CLIENT_H
