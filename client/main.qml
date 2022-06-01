import QtQuick 2.11
import QtQuick.Window 2.11
import QtLocation 5.15
import QtPositioning 5.15

Item
{
    width: 640
    height: 480
    visible: true
    //visibility: "Maximized"
    //title: qsTr("DCS Situational Awareness")

    Plugin
    {
        id: mapPlugin

        name: "osm" // osm mapboxgl esri
        PluginParameter
        {
            name: "osm.mapping.providersrepository.disabled"
            value: "true"
        }
        PluginParameter
        {
            name: "osm.mapping.providersrepository.address"
            value: "http://maps-redirect.qt.io/osm/5.6/"
        }

//        qml: Cycle Map
//        qml: Transit Map
//        qml: Night Transit Map
//        qml: Terrain Map
//        qml: Hiking Map
    }

    Map
    {
        id: map
        anchors.fill: parent
        plugin: mapPlugin
        center: QtPositioning.coordinate(25.01, 55.21)
        zoomLevel: 5

        MouseArea
        {
            id: mouseArea
            property variant lastCoordinate
            anchors.fill: parent
            acceptedButtons: Qt.LeftButton | Qt.RightButton

            onPressed :
            {
                if (mouse.button === Qt.LeftButton)
                {
                    console.log(map.toCoordinate(Qt.point(mouse.x, mouse.y)))
                }

                if (mouse.button === Qt.RightButton)
                {
                    console.log(map.mapItems);
                }
            }
        }
    }

    // Конструктор
    function init()
    {
        setNightMode(true)
    }

    function setNightMode(isNightMode)
    {
        for (var i = 0; i < map.supportedMapTypes.length; i++)
        {
            if (isNightMode && map.supportedMapTypes[i].name === "Night Transit Map"
                ||
                !isNightMode && map.supportedMapTypes[i].name === "Terrain Map")
            {
                map.activeMapType = map.supportedMapTypes[i];
            }
        }
    }

    // Функция передвижения камеры
    function moveCenter(coordinates, zoomLvl)
    {
        map.center = coordinates
        map.zoomLevel = zoomLvl
    }

    // Обновление данных объектов
    function updateUnitData(unitName, unitTitle, unitImageSource, coordinates)
    {
        var track;

        for (var i = 0; i < map.mapItems.length; i++)
        {
            if (map.mapItems[i].unitName === unitName)
            {
                track = map.mapItems[i]
                break
            }
        }

        if (track === undefined)
        {
            track = Qt.createQmlObject ('BaseTrack {}', map)

            track.unitName = unitName
            track.unitTitle = unitTitle
            track.coordinate = coordinates
            track.imageWidth = 20
            track.imageSource = unitImageSource

            map.addMapItem(track)
        }
        else
        {
            track.coordinate = coordinates
        }
    }

    function clearMap()
    {
        for (var i = 0; i < map.mapItems.length; i++)
        {
            if (map.mapItems[i].unitName === unitName)
            {
                map.removeMapItem(map.mapItems[i]);
                break
            }
        }
    }
}
