#ifndef DBMANAGER_H
#define DBMANAGER_H

#include <QObject>
#include <QSqlDatabase>

class DbManager : public QObject
{
    Q_OBJECT
public:
    explicit DbManager(QObject *parent = nullptr);
    ~DbManager();

    bool initDatabase();

public slots:
    void saveMessage(const QString &timestamp, const QString &sender, const QString &text);

private:
    QSqlDatabase db;
};

#endif