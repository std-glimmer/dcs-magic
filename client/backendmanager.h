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

public slots:
    bool connectToHost(const QString& ip, int port, const QString& username, const QString& password, const Coalition &coalition);
    void disconnect();

private:
    explicit BackendManager(QObject* parent = nullptr);
    static BackendManager* _instance;

    QSharedPointer<QTcpSocket> _socket;
};


#endif // BACKENDMANAGER_H
