#include "udpreceiver.h"
#include <QNetworkDatagram>
#include <QDateTime>

#include "../lib/recordsmanager.h"

UDPReceiver* UDPReceiver::_instance = nullptr;

UDPReceiver *UDPReceiver::Instance(QObject *parent)
{
    if (_instance == nullptr)
    {
        _instance = new UDPReceiver(parent);
    }

    return _instance;
}

UDPReceiver::UDPReceiver(QObject *parent)
    :
      QObject(parent),
      _socket(new QUdpSocket(this)),
      _recordsManager(RecordsManager::Instance(this))
{
    connect(_socket, &QUdpSocket::readyRead, this, &UDPReceiver::readPendingDatagrams);
    connect(this, &UDPReceiver::sendJSON, _recordsManager, &RecordsManager::receiveInfo);
}

void UDPReceiver::startListening()
{
    /// TODO: сделать систему треков для каждой сессии
    _recordsManager->initNewRecord(QDateTime::currentDateTimeUtc().toString("hhmmssddMMyyyy"));
    _socket->bind(QHostAddress::LocalHost, _port);
    _isConnected = true;
}

void UDPReceiver::stopListening()
{
    _socket->abort();
    _isConnected = false;
    _recordsManager->stopRecording();
}

bool UDPReceiver::isConnected()
{
    return _isConnected;
}

int UDPReceiver::port() const
{
    return _port;
}

void UDPReceiver::setPort(int port)
{
    _port = port;
}

void UDPReceiver::readPendingDatagrams()
{
    while ( _socket->hasPendingDatagrams() )
    {
        QNetworkDatagram datagram = _socket->receiveDatagram();
        emit sendJSON(datagram.data());
    }
}
