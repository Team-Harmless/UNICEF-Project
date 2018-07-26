import QtQuick 2.0
import "Scripts.js" as Scripts

Rectangle {
    id: line

    Connections {
        target: root
        onUpdateMap: line.destroy()
    }

    property real distance: 0
    property real angle: 0

    color: "darkblue"

    height: Scripts.kmToPx(distance, 2 * parent.radiusKM, (parent.width < parent.height ? parent.width : parent.height) - 20)
    width: 6

    x: parent.width/2 -3
    y: parent.height/2 - height

    transformOrigin: Item.Bottom
    rotation: angle/Math.PI * 180

}
