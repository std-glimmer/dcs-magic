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
    : QObject(parent)
{

}

BackendManager::~BackendManager()
{

}

bool BackendManager::connectToHost(const QString &ip, int port, const QString &password, const Coalition& coalition)
{
    disconnectSocket();

    _ip = ip;
    _port = port;
    _coalition = coalition;
    _password = password;

    _socket = new QTcpSocket(this);

    connect(_socket, &QTcpSocket::readyRead, this, &BackendManager::readyRead, Qt::DirectConnection);
    connect(_socket, &QTcpSocket::disconnected, this, &BackendManager::disconnectSocket);
    connect(_socket, &QTcpSocket::disconnected, this, &BackendManager::disconnectedFromServer);

    _socket->connectToHost(ip, port);
    _socket->waitForConnected();

    return _socket->state() == QAbstractSocket::ConnectedState;
}

void BackendManager::readyRead()
{
    if (_socket == nullptr)
    {
        return;
    }

    QByteArray response = _socket->readAll();

    if (response == "connected")
    {
        char coalitionChar = 'g';

        switch(_coalition)
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

        auto authMessage = QStringLiteral("coalition:%1;password:%2").arg(coalitionChar).arg(_password).toUtf8();
        _socket->write(authMessage);
        _socket->flush();
        return;
    }

    if (response.contains("[LOG]"))
    {
        qDebug() << response;
    }
    else
    {
        emit sendData(response);
    }
}

void BackendManager::disconnectSocket()
{
    if (_socket)
    {
        _socket->close();
    }
    delete _socket;
}



