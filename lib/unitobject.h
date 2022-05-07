#ifndef UNITOBJECT_H
#define UNITOBJECT_H

#include <QString>

class UnitObject
{
public:
    enum MessageValuesEnum : int
    {
        Time = 0, // Тайминг лога
        UnitTitle, // Как тип юнита называется в игре
        UnitName, // Уникальное имя юнита - ID
        UnitTypeLvl1, // Тип юнита 1: int
        UnitTypeLvl2, // Тип юнита 2: int
        UnitTypeLvl3, // Тип юнита 3: int
        UnitTypeLvl4, // Тип юнита 4: int
        CoalitionID,
        Latitude,
        Longitude,
        Altitude,
        Heading
    };

    enum CoalitionEnum : int
    {
        Red = 0,
        Blue,
        Neutral
    };

    enum Level1UnitType : int
    {
        Air = 1,
        Ground = 2,
        Navy = 3
    };

    enum Level2UnitType : int
    {
        Airplane = 1,
        Helicopter = 2,
        Ship = 12,
        SAM = 16
    };

    enum Level3UnitType : int
    {
        Fighter = 1,
        F_Bomber = 2,
        Interceptor = 3,
        Intruder = 4,
        Cruiser = 5,
        BattlePlane = 6,
        AirCarrier = 12,
        HeliCarrier = 13,
        ArmedShip = 14,
        CivilShip = 15,
        Submarine = 16,
    };

    struct UnitType
    {
        // 1 - Air, 2 - Ground, 3 - Navy
        int Level1 = 0;
        // 1 - Airplane, 2 - Helicopter, 12 - Ship, 16 - SAM
        int Level2 = 0;
        // 1 - Fighter, 2 - F_Bomber, 3 - Interceptor, 4 - Intruder, 5 - Cruiser, 6 - BattlePlane
        // 12 - AirCarrier, 13 - HeliCarrier, 14 - ArmedShip, 15 - CivilShip, 16 - Submarine
        int Level3 = 0;
        // Конкретный юнит
        int Level4 = 0;
    };

    explicit UnitObject();
    explicit UnitObject(const QString& message);

    QString getImageSource();

    void readDataFromMessage(const QString& message);

    QString lastUpdateTime() const;
    void setLastUpdateTime(const QString &lastUpdateTime);

    QString unitTitle() const;
    QString unitName() const;
    UnitType unitType() const;
    CoalitionEnum coalition() const;

    double latitude() const;
    void setLatitude(double latitude);

    double longitude() const;
    void setLongitude(double longitude);

    double altitude() const;
    void setAltitude(double altitude);

    double heading() const;
    void setHeading(double heading);

private:
    QString _lastUpdateTime;
    QString _unitTitle;
    QString _unitName; // ID
    UnitType _unitType;
    CoalitionEnum _coalition = Neutral;

    double _latitude = 0;
    double _longitude = 0;
    double _altitude = 0;
    double _heading = 0;
};

#endif // UNITOBJECT_H
