import QtQuick 2.0
import "Scripts.js" as Scripts

Rectangle {
    id: root
    color: "lightgray"

    Connections {
        target: context
        onSplat: addEl(type, angle, distance);
        onClearScreen: updateMap();
        onChangeRadius: radiusKM = rad;
    }

    property real radiusKM: 1000
    property bool clear: false;
    property real multiplier: 1


    function addEl(type, angle, distence){
        Scripts.createSpriteObjects(type, angle, distence, "Hosp1");
    }

    function clear() {
        updateMap();
    }

    Text {
        id: mapModeIndicatior
        text: "Line Length = Stright Line Distance"
        anchors.top: root.top
        anchors.right: root.right

        function setModeIndicatorText(newText) {
            text = "Line Length = " + newText
        }
    }


    MouseArea{
        anchors.fill: parent
        onClicked: {
            if (!clear) {
            //Scripts.createSpriteObjects("hosp", 0.785398, 800, "Hosp1");
            //Scripts.createSpriteObjects("hosp", -0.785398, 400, "Hosp2");
            //Scripts.createSpriteObjects("school", 0, 0, "Scl1");
            clear = true;
            }
            else {
                updateMap()
                clear = false;
            }
        }
    }

    Rectangle {
         width: (parent.width < parent.height ? parent.width : parent.height) - 20
         height: (parent.width < parent.height ? parent.width : parent.height) - 20
         anchors.horizontalCenter: parent.horizontalCenter
         anchors.verticalCenter: parent.verticalCenter
         color: "lightgray"
         border.color: "black"
         border.width: 1
         radius: width*0.5

    }

    signal updateMap
}
