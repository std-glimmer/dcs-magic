#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QObject>
#include <QSqlDatabase>
#include "utils.h"

class DatabaseManager : public QObject
{
    Q_OBJECT
public:
    static DatabaseManager* Instance(QObject* parent = nullptr);
    ~DatabaseManager();

    struct CoalitionPassword
    {
        Coalition coalition;
        QString password;
    };
    QList<CoalitionPassword> selectPasswords();
    bool updatePassword(const Coalition& coalition, const QString& password);

signals:
    void changed();

private:
    explicit DatabaseManager(QObject* parent);
    static DatabaseManager* _instance;

    QString _databaseName;
    QSqlDatabase _db;
};
#endif // DATABASEMANAGER_H
