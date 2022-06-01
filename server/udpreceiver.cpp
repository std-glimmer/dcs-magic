#include "udpreceiver.h"
#include <QNetworkDatagram>
#include <QDateTime>
#include <QThread>

#include "../lib/recordsmanager.h"

UDPReceiver* UDPReceiver::_instance = nullptr;

UDPReceiver* UDPReceiver::Instance(QObject* parent)
{
    if (_instance == nullptr)
    {
        _instance = new UDPReceiver(parent);
    }

    return _instance;
}

UDPReceiver::~UDPReceiver()
{

}

UDPReceiver::UDPReceiver(QObject *parent)
    :
      QObject(parent),
      _socket(new QUdpSocket(this))
{
    connect(_socket, &QUdpSocket::readyRead, this, &UDPReceiver::readPendingDatagrams);
}

void UDPReceiver::startListening()
{
    connect(this, &UDPReceiver::sendJSON, RecordsManager::Instance(), &RecordsManager::receiveInfo);
    RecordsManager::Instance()->initNewRecord(QDateTime::currentDateTimeUtc().toString("hhmmssddMMyyyy"));

    _socket->bind(QHostAddress::LocalHost, _port);
    _isConnected = true;
}

void UDPReceiver::stopListening()
{
    _socket->abort();
    _isConnected = false;
    RecordsManager::Instance()->stopRecording();
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
    while (_socket->hasPendingDatagrams())
    {
        //qDebug() << "test" << QThread::currentThreadId();
        QNetworkDatagram datagram = _socket->receiveDatagram();
        emit sendJSON(datagram.data());
    }
}
