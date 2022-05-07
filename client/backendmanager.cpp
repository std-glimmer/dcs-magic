#include "backendmanager.h"
#include <QtConcurrent/QtConcurrent>
#include <QDebug>
#include <QTcpSocket>

BackendManager* BackendManager::_instance = nullptr;

BackendManager* BackendManager::Instance(QObject* parent)
{
    if (_instance == nullptr)
    {
        _instance = new BackendManager(parent);
    }

    return _instance;
}

BackendManager::BackendManager(QObject* parent)
    : QObject(parent),
    _socket(QSharedPointer<QTcpSocket>::create(this))
{

}

BackendManager::~BackendManager()
{

}

bool BackendManager::connectToHost(const QString &ip, int port, const QString &username, const QString &password, const Coalition& coalition)
{
    QTcpSocket* socket = new QTcpSocket(this);
    socket->connectToHost(ip, port);
    socket->waitForConnected();

    if (socket->state() == QAbstractSocket::ConnectedState)
    {
        char coalitionChar = 'g';

        switch(coalition)
        {
        case Coalition::GameMaster:
            coalitionChar = 'g';
            break;
        case Coalition::Red:
            coalitionChar = 'r';
            break;
        case Coalition::Blue:
            coalitionChar = 'b';
            break;
        case Coalition::Neutral:
            coalitionChar = 'n';
            break;
        }

        auto authMessage = QStringLiteral("username:%1;password:%2;coalition:%3").arg(username).arg(password).arg(coalitionChar).toUtf8();
        socket->write(authMessage);
        socket->flush();
        socket->waitForBytesWritten();
        return true;
    }

    return false;
}

void BackendManager::disconnect()
{
    _socket->close();
}

