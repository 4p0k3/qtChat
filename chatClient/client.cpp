#include "client.h"
#include "./ui_client.h"
#include "networkmanager.h"

Client::Client(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Client)
{

    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(0);
    //ui->btnReturn->hide();

    networkManager = new NetworkManager(this);

    connect(networkManager, &NetworkManager::newMessageRecieved, this, &Client::onNewMessageRecieved);
    connect(networkManager, &NetworkManager::actionSuccessful, this, &Client::onActionSuccessful);
    connect(ui->lineMessageInput, &QLineEdit::returnPressed, this, &Client::on_btnSendMessage_clicked); //linking Enter keyboard button and send app button

}

Client::~Client()
{
    delete ui;
}

void Client::onNewMessageRecieved(QString text)
{
    ui->editMessageHistory->append(text);
}

void Client::onActionSuccessful(ActionType type){
    switch (type){
    case JoinedChat:
        ui->stackedWidget->setCurrentIndex(1);

        if (networkManager->isHost()) {
            ui->btnDeleteChat->setText("Delete chat");
            ui->btnDeleteChat->setVisible(true);
        } else {
            ui->btnDeleteChat->setText("Leave chat");
        }
        break;

    case LeftChat:
        ui->stackedWidget->setCurrentIndex(0);
        break;
    }
}

void Client::on_btnCreateChat_clicked()
{
    QString enteredIp = ui->lineIPInput->text();
    quint16 enteredPort = ui->linePortInput->text().toUShort();

    networkManager->createChat(enteredIp);
}

void Client::on_btnConnect_clicked()
{
    QString enteredIp = ui->lineIPInput->text();
    quint16 enteredPort = ui->linePortInput->text().toUShort();

    networkManager->joinChat(enteredIp, enteredPort);
}

void Client::on_btnSendMessage_clicked()
{
    QString message = ui->lineMessageInput->text();
    if (message.isEmpty()) return;

    networkManager->sendMessage(message);
    ui->lineMessageInput->clear();
}

void Client::on_btnMenu_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void Client::on_btnDeleteChat_clicked()
{
    networkManager->leaveChat();
}
void Client::on_btnReturn_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}


void Client::on_btnLeaveChat_clicked()
{
    networkManager->leaveChat();
}

