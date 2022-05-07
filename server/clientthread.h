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
    explicit ClientThread(qintptr id, QObject *parent = nullptr);

    void run();

    const QString &username() const;
    void setUsername(const QString &newUsername);

    Coalition coalition() const;
    void setCoalition(Coalition newCoalition);

signals:
    void error(QTcpSocket::SocketError socketerror);
    void disconnectSocket(qintptr descriptor);

public slots:
    void readyRead();
    void disconnected();
    void sendData(const QByteArray& data);

private:
    QTcpSocket* _socket = nullptr;
    qintptr _socketDescriptor;

    QString _username;
    Coalition _coalition = Coalition::Neutral;

    QTextStream dataStream;
};

#endif // CLIENTTHREAD_H
