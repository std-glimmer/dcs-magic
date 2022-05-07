#include "connectionmanager.h"
#include <QtConcurrent/QtConcurrent>
#include <QDebug>
#include <QTcpSocket>
#include "clientthread.h"

ConnectionManager* ConnectionManager::_instance = nullptr;

ConnectionManager* ConnectionManager::Instance(QObject* parent)
{
    if (_instance == nullptr)
    {
        _instance = new ConnectionManager(parent);
    }

    return _instance;
}

ConnectionManager::ConnectionManager(QObject* parent)
    : QObject(parent)
{
    _server = QSharedPointer<QTcpServer>::create(this);
    if (!_server->listen(QHostAddress::Any, 9085))
    {
        qDebug() << "Server listen error" << _server->errorString();
        return;
    }

    connect(_server.get(), &QTcpServer::newConnection, this, &ConnectionManager::newConnectionReceived);

    qDebug() << "Server started";
}

ConnectionManager::~ConnectionManager()
{
    closeSockets();
}

void ConnectionManager::newConnectionReceived()
{
    auto socketDescriptor = _server->nextPendingConnection()->socketDescriptor();
    auto socketThread = new ClientThread(socketDescriptor, this);
    _clients.insert(socketDescriptor, socketThread);

    qDebug() << socketDescriptor << "New user";
    connect(socketThread, &ClientThread::disconnectSocket, this, &ConnectionManager::clientDisconnected);
    socketThread->start();
}

void ConnectionManager::clientDisconnected(qintptr descriptor)
{
    _clients.remove(descriptor);
}

void ConnectionManager::closeSockets()
{
    for (auto& i : _clients.keys())
    {
        _clients[i]->disconnected();
        _clients.remove(i);
    }
    _server->close();
}
