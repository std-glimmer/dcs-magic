#include "databasemanager.h"
#include <QFile>
#include <QDebug>
#include <QSqlError>
#include <QSqlQuery>
#include <QFile>

DatabaseManager* DatabaseManager::_instance = nullptr;

DatabaseManager* DatabaseManager::Instance(QObject* parent)
{
    if (_instance == nullptr)
    {
        _instance = new DatabaseManager(parent);
    }

    return _instance;
}

DatabaseManager::DatabaseManager(QObject* parent)
    : QObject(parent)
{
    _db = QSqlDatabase::addDatabase("QSQLITE");
    _db.setDatabaseName("settings.db");

    if (!QFile::exists("./settings.db"))
    {
        qDebug() << "Creating database";
        QFile::copy(":/settings.db", "./settings.db");
    }

    if (_db.open())
    {
        qDebug() << "Database connected.";
    }
    else
    {
        qDebug() << "Database connection failed." << _db.lastError().text();
    }
}

DatabaseManager::~DatabaseManager()
{

}

QList<DatabaseManager::CoalitionPassword> DatabaseManager::selectPasswords()
{
    QList<DatabaseManager::CoalitionPassword> passwords;

    QSqlQuery query;

    if (query.exec(QStringLiteral(u"SELECT Coalition, Password FROM Access")))
    {
        while (query.next())
        {
            passwords.append({ static_cast<Coalition>(query.value(0).toInt()), query.value(1).toString() });
        }
    }
    else
    {
        qDebug() << QStringLiteral(u"[%1]: %2").arg(__FUNCTION__).arg(query.lastError().text().trimmed());
    }

    return passwords;
}

bool DatabaseManager::updatePassword(const Coalition& coalition, const QString& password)
{
    QSqlQuery query;

    query.prepare(QStringLiteral(
        u"UPDATE Access "
        u"SET Password = ?"
        u"WHERE Coalition = ?;"));

    query.bindValue(0, coalition);
    query.bindValue(1, password);

    if (query.exec())
    {
        emit changed();
        return true;
    }

    qDebug() << QStringLiteral(u"[%1]: %2").arg(__FUNCTION__).arg(query.lastError().text().trimmed());
    return false;
}
