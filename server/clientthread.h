#ifndef CLIENTTHREAD_H
#define CLIENTTHREAD_H

#include <QThread>
#include <QTcpSocket>
#include <QDebug>
#include "utils.h"

class ClientThread : public QThread
{
    Q_OBJECT
public:
    explicit ClientThread(qintptr socketId, QObject *parent = nullptr);

    void run();

    const QString &username() const;
    void setUsername(const QString &newUsername);

    Coalition coalition() const;
    void setCoalition(Coalition newCoalition);

signals:
    void printLog(const QString& message);
    void error(QTcpSocket::SocketError socketerror);
    void disconnectSocket(qintptr descriptor);

public slots:
    void readyRead();
    void disconnected();
    void sendData(const QByteArray& data);

private:
    QTcpSocket* _socket = nullptr;
    qintptr _socketId;
    Coalition _coalition = Coalition::Neutral;
};

#endif // CLIENTTHREAD_H
