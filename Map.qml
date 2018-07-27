import QtQuick 2.0
import "Scripts.js" as Scripts

Rectangle {
    id: root
    color: "lightgray"

    Connections {
        target: context
        onSplat: addEl(type, angle, distance, name);
        onClearScreen: updateMap();
        onChangeRadius: radiusKM = rad;
        onSetMultiplier: multiplier = mult;
    }

    property real radiusKM: 100
    property bool clear: false;
    property real multiplier: 1


    function addEl(type, angle, distence, name){
        Scripts.createSpriteObjects(type, angle, distence, name);
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


    Text {
        id: radius
        text: "Radius = " + radiusKM * multiplier + (multiplier === 1 ? "km" : "mi");
        anchors.top: root.top
        anchors.left: root.left

        function setModeIndicatorText(newText) {
            text = "Line Length = " + newText
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
