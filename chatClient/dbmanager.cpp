#include "dbmanager.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QCoreApplication>
#include <QDebug>

DbManager::DbManager(QObject *parent) : QObject(parent)
{
    initDatabase();
}

DbManager::~DbManager()
{
    if (db.isOpen()) {
        db.close();
    }
}

bool DbManager::initDatabase()
{
    // Добавляем драйвер SQLite
    db = QSqlDatabase::addDatabase("QSQLITE");

    // Путь к файлу
    QString dbPath = QCoreApplication::applicationDirPath() + "/chat_history.db";
    db.setDatabaseName(dbPath);

    if (!db.open()) {
        qDebug() << "Database error:" << db.lastError().text();
        return false;
    }

    // Создаем таблицу
    QSqlQuery query;
    QString createTable = "CREATE TABLE IF NOT EXISTS messages ("
                          "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                          "timestamp TEXT, "
                          "sender TEXT, "
                          "text TEXT"
                          ");";

    if (!query.exec(createTable)) {
        qDebug() << "Table creation error:" << query.lastError().text();
        return false;
    }

    qDebug() << "Database initialized successfully at:" << dbPath;
    return true;
}

// Заглушка для следующего шага
void DbManager::saveMessage(const QString &timestamp, const QString &sender, const QString &text)
{
    QSqlQuery query;
    // Готовим безопасный шаблон запроса
    query.prepare("INSERT INTO messages (timestamp, sender, text) VALUES (:time, :name, :msg)");

    // Подставляем реальные значения вместо плейсхолдеров
    query.bindValue(":time", timestamp);
    query.bindValue(":name", sender);
    query.bindValue(":msg", text);

    if (!query.exec()) {
        qDebug() << "Failed to save message:" << query.lastError().text();
    } else {
        qDebug() << "Message saved to DB!";
    }
}