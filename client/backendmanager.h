#ifndef BACKENDMANAGER_H
#define BACKENDMANAGER_H

#include <QObject>
#include <QTcpSocket>
#include "utils.h"

class BackendManager : public QObject
{
    Q_OBJECT
public:
    static BackendManager* Instance(QObject* parent = nullptr);
    ~BackendManager();

signals:
    void sendData(const QByteArray& data);
    void disconnectedFromServer();

public slots:
    bool connectToHost(const QString& ip, int port, const QString& password, const Coalition &coalition);
    void disconnectSocket();

private slots:
    void readyRead();

private:
    explicit BackendManager(QObject* parent = nullptr);
    static BackendManager* _instance;

    QString _ip;
    int _port = 0;
    Coalition _coalition = Coalition::Neutral;
    QString _password;

    QTcpSocket* _socket = nullptr;
};


#endif // BACKENDMANAGER_H
