#ifndef UNITSMANAGER_H
#define UNITSMANAGER_H

#include <QObject>
#include <QSharedPointer>
#include <QVector>
#include "unitobject.h"

class UnitsManager : public QObject
{
    Q_OBJECT
public:
    using UnitObjPtr = QSharedPointer<UnitObject>;

    static UnitsManager *Instance(QObject* parent = nullptr);
    UnitsManager(UnitsManager &other) = delete;
    void operator=(const UnitsManager &) = delete;

    UnitObjPtr getUnit(const QString& unitName);
    UnitObjPtr getUnit(int index);
    UnitObjPtr getUnitViaMessage(const QString& message);

    int count();
    void clear();

signals:
    void sendUnitData(UnitsManager::UnitObjPtr unit, bool isNewUnit);

public slots:
    void receiveInfo(const QByteArray& json);

private:
    explicit UnitsManager(QObject* parent = nullptr);
    static UnitsManager* _instance;

    QVector<UnitObjPtr> _unitObjects;

    void parseUnitMessage(const QString& json);
};

#endif // UNITSMANAGER_H
