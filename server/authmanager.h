#ifndef AUTHMANAGER_H
#define AUTHMANAGER_H

#include <QObject>
#include "utils.h"
#include "databasemanager.h"

class AuthManager : public QObject
{
    Q_OBJECT
public:
    static AuthManager* Instance(QObject* parent = nullptr);
    ~AuthManager();

    bool authCheck(const Coalition& coalition, const QString& password);
    const QList<DatabaseManager::CoalitionPassword> &passwords() const;

signals:
    void updated();

public slots:
    void update();

private:
    explicit AuthManager(QObject* parent = nullptr);
    static AuthManager* _instance;

    QList<DatabaseManager::CoalitionPassword> _passwords;
};
#endif // AUTHMANAGER_H
