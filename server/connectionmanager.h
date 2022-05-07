#ifndef CONNECTIONMANAGER_H
#define CONNECTIONMANAGER_H

#include <QObject>
#include <QTcpServer>

class ClientThread;

class ConnectionManager : public QObject
{
    Q_OBJECT
public:
    static ConnectionManager* Instance(QObject* parent = nullptr);
    ~ConnectionManager();

private slots:
    void newConnectionReceived();
    void clientDisconnected(qintptr descriptor);

private:
    explicit ConnectionManager(QObject* parent = nullptr);
    static ConnectionManager* _instance;

    QSharedPointer<QTcpServer> _server;
    QMap<quint64, ClientThread*> _clients;

    void closeSockets();
};

#endif // CONNECTIONMANAGER_H
