#include "clientthread.h"
#include "udpreceiver.h"
#include "databasemanager.h"
#include "../lib/unitsmanager.h"
#include "authmanager.h"
#include <QDateTime>

ClientThread::ClientThread(qintptr socketId, QObject *parent)
    :
    QThread(parent),
    _socketId(socketId)
{

}

void ClientThread::run()
{
    _socket = new QTcpSocket();

    if(!_socket->setSocketDescriptor(_socketId))
    {
        // something's wrong, we just emit a signal
        emit error(_socket->error());
        return;
    }

    qDebug() << _socketId << "New user connected.";

    connect(_socket, &QTcpSocket::readyRead, this, &ClientThread::readyRead, Qt::DirectConnection);
    connect(UDPReceiver::Instance(), &UDPReceiver::sendJSON, this, &ClientThread::sendData, Qt::DirectConnection);
    connect(_socket, &QTcpSocket::disconnected, this, &ClientThread::disconnected);

    qDebug() << "run" << QThread::currentThreadId();

    _socket->write("connected");
    _socket->flush();

    exec();
}

void ClientThread::readyRead()
{
    if (_socket == nullptr)
    {
        return;
    }

    QByteArray clientMessage = _socket->readAll();
    QString password;

    QString coalitionKey = "coalition:";
    QString passwordKey = "password:";

    int coalitionPos = clientMessage.indexOf(coalitionKey) + coalitionKey.size();
    int passwordStartPos = clientMessage.indexOf(passwordKey) + passwordKey.size();

    if (coalitionPos == -1 || passwordStartPos == -1)
    {
        _socket->write("[LOG]: Unknown message format");
        _socket->flush();
        disconnected();
        return;
    }

    for (int i = passwordStartPos; i < clientMessage.size(); i++)
    {
        char c = clientMessage.at(i);
        if (c == ';') break;
        password.append(c);
    }

    switch(clientMessage.at(coalitionPos))
    {
    case 'r':
        _coalition = Coalition::Red;
        break;
    case 'b':
        _coalition = Coalition::Blue;
        break;
    case 'g':
        _coalition = Coalition::GameMaster;
        break;
    case 'n':
    default:
        _coalition = Coalition::Neutral;
    }

    if (!AuthManager::Instance()->authCheck(_coalition, password))
    {
        _socket->write("[LOG]: Invalid login or password");
        _socket->flush();
        disconnected();
        return;
    }

    qDebug() << "Connected" << QThread::currentThreadId();

    emit printLog(QStringLiteral("[%1] %2 GCI %3:%4 connected.")
                      .arg(QDateTime::currentDateTime().toString("dd.MM.yy hh:mm:ss"))
                      .arg(_coalition)
                      .arg(_socket->peerAddress().toString())
                      .arg(_socket->peerPort()));
}

void ClientThread::disconnected()
{
    emit printLog(QStringLiteral("[%1] %2 GCI %3:%4 disconnected.")
                      .arg(QDateTime::currentDateTime().toString("dd.MM.yy hh:mm:ss"))
                      .arg(_coalition)
                      .arg(_socket->peerAddress().toString())
                      .arg(_socket->peerPort()));

    emit disconnectSocket(_socketId);
    _socket->close();
    _socket->deleteLater();

    exit(0);
}

void ClientThread::sendData(const QByteArray &data)
{
    if (_socket == nullptr)
    {
        return;
    }

    if (!_socket->isOpen() || !_socket->isValid())
    {
        return;
    }

    auto obj = UnitObject(data);

    if (!obj.isValid())
    {
        return;
    }

    if (
        (obj.coalition() == UnitObject::CoalitionEnum::Red && (_coalition == Coalition::GameMaster || _coalition == Coalition::Red))
        ||
        (obj.coalition() == UnitObject::CoalitionEnum::Blue && (_coalition == Coalition::GameMaster || _coalition == Coalition::Blue))
        )
    {
        qDebug() << QThread::currentThreadId();
        _socket->write(data);
        _socket->flush();
    }
}

Coalition ClientThread::coalition() const
{
    return _coalition;
}

void ClientThread::setCoalition(Coalition newCoalition)
{
    _coalition = newCoalition;
}

