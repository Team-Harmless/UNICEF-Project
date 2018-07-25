import QtQuick 2.0
import "Scripts.js" as Scripts

Rectangle {



    id: root
    color: "lightgray"

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
            Scripts.createSpriteObjects("hosp", 0.5, 100, "Hosp1");
            Scripts.createSpriteObjects("school", 0, 0, "Scl1");
        }
    }

    onHeightChanged: children.updateXY
    onWidthChanged: children.updateXY
}
