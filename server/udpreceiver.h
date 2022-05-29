#ifndef UDPRECEIVER_H
#define UDPRECEIVER_H

#include <QObject>
#include <QUdpSocket>

class RecordsManager;

class UDPReceiver : public QObject
{
    Q_OBJECT
public:
    explicit UDPReceiver(QObject* parent = nullptr, bool isPrimary = false);

    void startListening();
    void stopListening();

    bool isConnected();

    int port() const;
    void setPort(int port);

signals:
    void sendJSON(const QByteArray& json);

private slots:
    void readPendingDatagrams();

private:
    int _port = 6026;
    QUdpSocket* _socket = nullptr;
    bool _isConnected = false;
    bool _isPrimary = false;
};

#endif // UDPRECEIVER_H
