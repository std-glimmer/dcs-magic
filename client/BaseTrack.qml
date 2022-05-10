import QtQuick 2.0
import QtLocation 5.11

MapQuickItem
{
    id: baseTrack

    anchorPoint.x: image.width/4
    anchorPoint.y: image.height

    property string unitName
    property string unitTitle

    property int imageWidth
    property string imageSource
    property bool detailsVisible: false

    sourceItem: Image
    {
        id: image
        source: imageSource
        width: imageWidth
        height: imageWidth

        MouseArea
        {
            anchors.fill: parent
            hoverEnabled: true

            onEntered: {
                detailsVisible = true
            }

            onExited: {
                detailsVisible = false
            }
        }

        Text
        {
            id: unitNameDisplay
            text: unitName
            x: image.width * -1.4
            y: image.height / 10
            width: image.width
            color: "white"
            font.pixelSize: 12
            horizontalAlignment: Text.AlignHCenter
            visible: detailsVisible
        }

        Text
        {
            id: unitTitleDisplay
            text: unitTitle
            x: image.width * 1.7
            y: image.height / 10
            width: image.width
            color: "white"
            font.pixelSize: 12
            horizontalAlignment: Text.AlignHCenter
            visible: detailsVisible
        }

        Text
        {
            id: altitudeDisplay
            text: coordinate.altitude + "m"
            x: image.width * 1.3
            y: image.height / 1.3
            width: image.width
            color: "white"
            font.pixelSize: 12
            horizontalAlignment: Text.AlignHCenter
            visible: detailsVisible
        }
    }
}
