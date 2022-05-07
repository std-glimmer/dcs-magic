#include "unitsmanager.h"
#include <QDebug>
#include <QGeoCoordinate>

UnitsManager* UnitsManager::_instance = nullptr;

UnitsManager *UnitsManager::Instance(QObject *parent)
{
    if (_instance == nullptr)
    {
        _instance = new UnitsManager(parent);
    }

    return _instance;
}

void UnitsManager::receiveInfo(const QByteArray &json)
{
    QString message = QString::fromUtf8(json);
    // Разделение на юниты по ;
    QStringList units = message.split(";");

    for (auto& unitStr : units)
    {
        if (unitStr.trimmed().isEmpty())
            continue;

        UnitObjPtr unit = getUnitViaMessage(unitStr);

        bool isNew = false;

        if (unit.isNull())
        {
            isNew = true;
            unit = UnitObjPtr::create(unitStr);
            _unitObjects.append(unit);
        }
        else
        {
            isNew = false;
            unit->readDataFromMessage(unitStr);
        }

        emit sendUnitData(unit, isNew);
    }
}

UnitsManager::UnitsManager(QObject *parent) : QObject(parent)
{
    
}

int UnitsManager::count()
{
    return _unitObjects.count();
}

void UnitsManager::clear()
{
    _unitObjects.clear();
}

UnitsManager::UnitObjPtr UnitsManager::getUnit(const QString &unitName)
{
    auto it = std::find_if(_unitObjects.begin(), _unitObjects.end(), [unitName] (const UnitObjPtr& unit) {
        return unit->unitName() == unitName;
    });

    return (it == _unitObjects.end()) ? nullptr : *it;
}

UnitsManager::UnitObjPtr UnitsManager::getUnit(int index)
{
    if (index >= _unitObjects.count())
    {
        return nullptr;
    }

    return _unitObjects.at(index);
}

UnitsManager::UnitObjPtr UnitsManager::getUnitViaMessage(const QString &message)
{
    QStringList unitData = message.split(",");
    if (unitData.count() < UnitObject::UnitName) return nullptr;
    return getUnit(unitData.at(UnitObject::UnitName));
}
