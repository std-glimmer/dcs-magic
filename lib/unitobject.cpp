#include "unitobject.h"
#include <QStringList>

UnitObject::UnitObject()
{

}

UnitObject::UnitObject(const QString &message)
{
    readDataFromMessage(message);
}

QString UnitObject::getImageSource()
{
    QString coalitionImageText;
    if (_coalition == CoalitionEnum::Blue)      coalitionImageText = "blue";
    if (_coalition == CoalitionEnum::Red)       coalitionImageText = "red";
    if (_coalition == CoalitionEnum::Neutral)   coalitionImageText = "neutral";

    switch (_unitType.Level2)
    {
    case Level2UnitType::Airplane:
        return "qrc:///resources/images/units/airplane_" + coalitionImageText + ".png";
    case Level2UnitType::Helicopter:
        return "qrc:///resources/images/units/heli_" + coalitionImageText + ".png";
    case Level2UnitType::Ship:
        return "qrc:///resources/images/units/seasurface_" + coalitionImageText + ".png";
    case Level2UnitType::SAM:
        return "qrc:///resources/images/units/sam_" + coalitionImageText + ".png";
    default:
        return _coalition == Red ? "qrc:///resources/images/units/unknown_red.png" : "qrc:///resources/images/units/unknown.png";
    }
}

void UnitObject::readDataFromMessage(const QString &message)
{
    QStringList unitData = message.split(",");

    if (unitData.count() < 9)
    {
        _isValid = false;
        return;
    }

    // Если объект ранее не создавался -> задаем ему константные параметры
    if (_unitName.isEmpty())
    {
        _unitTitle = unitData.at(UnitTitle);
        _unitName = unitData.at(UnitName);
        _unitType =
            {
                unitData.at(UnitTypeLvl1).toInt(),
                unitData.at(UnitTypeLvl2).toInt(),
                unitData.at(UnitTypeLvl3).toInt(),
                unitData.at(UnitTypeLvl4).toInt()
            };
        _coalition = (unitData.at(CoalitionID).toUInt() - 1) > Neutral ? Neutral : static_cast<CoalitionEnum>(unitData.at(CoalitionID).toUInt() - 1);
    }

    setLastUpdateTime(unitData.at(Time));
    setLatitude(unitData.at(Latitude).toDouble());
    setLongitude(unitData.at(Longitude).toDouble());
    setAltitude(unitData.at(Altitude).toDouble());
    setHeading(unitData.at(Heading).toDouble());

    _isValid = true;
}

QString UnitObject::lastUpdateTime() const
{
    return _lastUpdateTime;
}

void UnitObject::setLastUpdateTime(const QString &lastUpdateTime)
{
    _lastUpdateTime = lastUpdateTime;
}

QString UnitObject::unitTitle() const
{
    return _unitTitle;
}

QString UnitObject::unitName() const
{
    return _unitName;
}

UnitObject::UnitType UnitObject::unitType() const
{
    return _unitType;
}

UnitObject::CoalitionEnum UnitObject::coalition() const
{
    return _coalition;
}

double UnitObject::latitude() const
{
    return _latitude;
}

void UnitObject::setLatitude(double latitude)
{
    _latitude = latitude;
}

double UnitObject::longitude() const
{
    return _longitude;
}

void UnitObject::setLongitude(double longitude)
{
    _longitude = longitude;
}

double UnitObject::altitude() const
{
    return _altitude;
}

void UnitObject::setAltitude(double altitude)
{
    _altitude = altitude;
}

double UnitObject::heading() const
{
    return _heading;
}

void UnitObject::setHeading(double heading)
{
    _heading = heading;
}

bool UnitObject::isValid() const
{
    return _isValid;
}
