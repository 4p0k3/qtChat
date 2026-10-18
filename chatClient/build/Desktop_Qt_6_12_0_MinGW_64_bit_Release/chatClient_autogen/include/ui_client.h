/********************************************************************************
** Form generated from reading UI file 'client.ui'
**
** Created by: Qt User Interface Compiler version 6.12.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CLIENT_H
#define UI_CLIENT_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Client
{
public:
    QWidget *centralwidget;
    QStackedWidget *stackedWidget;
    QWidget *pageLobbySelect;
    QPushButton *btnCreateChat;
    QPushButton *btnConnect;
    QLineEdit *lineIPInput;
    QLineEdit *linePortInput;
    QPushButton *btnReturn;
    QWidget *pageChat;
    QTextEdit *editMessageHistory;
    QPushButton *btnSendMessage;
    QLineEdit *lineMessageInput;
    QPushButton *btnMenu;
    QPushButton *btnDeleteChat;

    void setupUi(QMainWindow *Client)
    {
        if (Client->objectName().isEmpty())
            Client->setObjectName("Client");
        Client->resize(800, 600);
        centralwidget = new QWidget(Client);
        centralwidget->setObjectName("centralwidget");
        stackedWidget = new QStackedWidget(centralwidget);
        stackedWidget->setObjectName("stackedWidget");
        stackedWidget->setGeometry(QRect(0, 0, 801, 601));
        pageLobbySelect = new QWidget();
        pageLobbySelect->setObjectName("pageLobbySelect");
        btnCreateChat = new QPushButton(pageLobbySelect);
        btnCreateChat->setObjectName("btnCreateChat");
        btnCreateChat->setGeometry(QRect(350, 250, 90, 29));
        btnConnect = new QPushButton(pageLobbySelect);
        btnConnect->setObjectName("btnConnect");
        btnConnect->setGeometry(QRect(340, 340, 121, 29));
        lineIPInput = new QLineEdit(pageLobbySelect);
        lineIPInput->setObjectName("lineIPInput");
        lineIPInput->setGeometry(QRect(310, 300, 171, 28));
        linePortInput = new QLineEdit(pageLobbySelect);
        linePortInput->setObjectName("linePortInput");
        linePortInput->setGeometry(QRect(360, 390, 71, 28));
        btnReturn = new QPushButton(pageLobbySelect);
        btnReturn->setObjectName("btnReturn");
        btnReturn->setGeometry(QRect(20, 20, 111, 29));
        btnReturn->setFlat(false);
        stackedWidget->addWidget(pageLobbySelect);
        pageChat = new QWidget();
        pageChat->setObjectName("pageChat");
        editMessageHistory = new QTextEdit(pageChat);
        editMessageHistory->setObjectName("editMessageHistory");
        editMessageHistory->setGeometry(QRect(130, 30, 641, 471));
        editMessageHistory->setReadOnly(true);
        btnSendMessage = new QPushButton(pageChat);
        btnSendMessage->setObjectName("btnSendMessage");
        btnSendMessage->setGeometry(QRect(680, 540, 90, 29));
        lineMessageInput = new QLineEdit(pageChat);
        lineMessageInput->setObjectName("lineMessageInput");
        lineMessageInput->setGeometry(QRect(130, 540, 531, 28));
        btnMenu = new QPushButton(pageChat);
        btnMenu->setObjectName("btnMenu");
        btnMenu->setGeometry(QRect(20, 30, 90, 29));
        btnDeleteChat = new QPushButton(pageChat);
        btnDeleteChat->setObjectName("btnDeleteChat");
        btnDeleteChat->setGeometry(QRect(20, 540, 90, 29));
        stackedWidget->addWidget(pageChat);
        Client->setCentralWidget(centralwidget);

        retranslateUi(Client);

        stackedWidget->setCurrentIndex(0);
        btnSendMessage->setDefault(false);


        QMetaObject::connectSlotsByName(Client);
    } // setupUi

    void retranslateUi(QMainWindow *Client)
    {
        Client->setWindowTitle(QCoreApplication::translate("Client", "Client", nullptr));
        btnCreateChat->setText(QCoreApplication::translate("Client", "Create chat", nullptr));
        btnConnect->setText(QCoreApplication::translate("Client", "Connect to chat", nullptr));
        lineIPInput->setPlaceholderText(QCoreApplication::translate("Client", "ip address", nullptr));
        linePortInput->setPlaceholderText(QCoreApplication::translate("Client", "port", nullptr));
        btnReturn->setText(QCoreApplication::translate("Client", "Return to chat", nullptr));
        btnSendMessage->setText(QCoreApplication::translate("Client", "Send", nullptr));
        btnMenu->setText(QCoreApplication::translate("Client", "Menu", nullptr));
        btnDeleteChat->setText(QCoreApplication::translate("Client", "Delete chat", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Client: public Ui_Client {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CLIENT_H
