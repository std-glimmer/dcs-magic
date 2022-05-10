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
    auto socket = _server->nextPendingConnection();
    auto socketId = socket->socketDescriptor();
    auto socketThread = new ClientThread(socketId, this);
    _clients.insert(socketId, socketThread);

    connect(socketThread, &ClientThread::printLog, this, &ConnectionManager::printLog);
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
