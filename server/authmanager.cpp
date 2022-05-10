#include "authmanager.h"
#include <QtConcurrent/QtConcurrent>
#include <QDebug>
#include <QTcpSocket>
#include "clientthread.h"

AuthManager* AuthManager::_instance = nullptr;

AuthManager* AuthManager::Instance(QObject* parent)
{
    if (_instance == nullptr)
    {
        _instance = new AuthManager(parent);
    }

    return _instance;
}

AuthManager::AuthManager(QObject* parent)
    : QObject(parent)
{
    update();
    connect(DatabaseManager::Instance(), &DatabaseManager::changed, this, &AuthManager::update);
}

const QList<DatabaseManager::CoalitionPassword> &AuthManager::passwords() const
{
    return _passwords;
}

AuthManager::~AuthManager()
{

}

bool AuthManager::authCheck(const Coalition &coalition, const QString &password)
{
    for (auto& i : _passwords)
    {
        if (i.coalition == coalition && i.password == password)
        {
            return true;
        }
    }

    return false;
}

void AuthManager::update()
{
    _passwords = DatabaseManager::Instance()->selectPasswords();
    emit updated();
}
