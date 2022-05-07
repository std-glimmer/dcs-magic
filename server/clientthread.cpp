#include "clientthread.h"
#include "udpreceiver.h"
#include "../lib/unitsmanager.h"

ClientThread::ClientThread(qintptr id, QObject *parent)
    :
    QThread(parent)
{
    _socketDescriptor = id;
}

void ClientThread::run()
{
    _socket = new QTcpSocket();

    if(!_socket->setSocketDescriptor(_socketDescriptor))
    {
        emit error(_socket->error());
        _socket->deleteLater();
        return;
    }

    dataStream.setDevice(_socket);
    dataStream.setAutoDetectUnicode(true);

    connect(_socket, SIGNAL(readyRead()), this, SLOT(readyRead()), Qt::DirectConnection);
    connect(_socket, SIGNAL(disconnected()), this, SLOT(disconnected()));

    exec();
}

void ClientThread::readyRead()
{
    if (_socket == nullptr)
    {
        return;
    }

    qDebug() << _socketDescriptor << "Connecting...";

    QByteArray clientMessage = _socket->readAll();

    // Авторизация (TODO: перенести в отдельный менеджер)
    QString username;
    QString password;
    QString coalitionTxt;

    QString usernameKey = "username:";
    QString passwordKey = "password:";
    QString coalitionKey = "coalition:";

    int usernameStartPos = clientMessage.indexOf(usernameKey) + usernameKey.size();
    int passwordStartPos = clientMessage.indexOf(passwordKey) + passwordKey.size();
    int coalitionPos = clientMessage.indexOf(coalitionKey) + coalitionKey.size();

    if (usernameStartPos == -1 || passwordStartPos == -1)
    {
        dataStream << "unknown message format\n";
        _socket->flush();
        disconnected();
        return;
    }

    for (int i = usernameStartPos; i < clientMessage.size(); i++)
    {
        char c = clientMessage.at(i);
        if (c == ';') break;
        username.append(c);
    }

    for (int i = passwordStartPos; i < clientMessage.size(); i++)
    {
        char c = clientMessage.at(i);
        if (c == ';') break;
        password.append(c);
    }

    /// TODO: костыль с тестовыми логин пароль
    if (username.toLower() != "glimmer" || password.toLower() != "666")
    {
        dataStream << "invalid login or password\n";
        _socket->flush();
        disconnected();
        return;
    }

    _username = username;

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

    connect(UDPReceiver::Instance(), &UDPReceiver::sendJSON, this, &ClientThread::sendData);
    qDebug() << QStringLiteral("User %1 connected.").arg(username);
}

void ClientThread::disconnected()
{
    qDebug() << _socketDescriptor << " Disconnected";
    _socket->deleteLater();
    emit disconnectSocket(_socketDescriptor);
    exit(0);
}

void ClientThread::sendData(const QByteArray &data)
{
    if (_socket == nullptr)
    {
        return;
    }

    auto obj = UnitsManager::Instance()->getUnitViaMessage(data);

    if (
        (obj->coalition() == UnitObject::CoalitionEnum::Red && (_coalition == Coalition::GameMaster || _coalition == Coalition::Red))
        ||
        (obj->coalition() == UnitObject::CoalitionEnum::Blue && (_coalition == Coalition::GameMaster || _coalition == Coalition::Blue))
        )
    {
        dataStream << *data;
        _socket->flush();
    }
}

const QString &ClientThread::username() const
{
    return _username;
}

void ClientThread::setUsername(const QString &newUsername)
{
    _username = newUsername;
}

Coalition ClientThread::coalition() const
{
    return _coalition;
}

void ClientThread::setCoalition(Coalition newCoalition)
{
    _coalition = newCoalition;
}

