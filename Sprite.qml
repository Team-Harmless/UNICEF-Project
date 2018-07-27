import QtQuick 2.0
import "Scripts.js" as Scripts

Image{
    id: image
    Connections {
        target: root
        onUpdateMap: {image.destroy()}
    }

    function clearPoints() {
        console.log("Hello there")
        this.destroy();
    }

    property string spriteType: "hosp"
    property real angle: 0;
    property real distance: 0;
    property string title: "Place"

    function getFilePath() {
        if (spriteType == "hosp") {
            return "hosp.png"
        }
        else {
           return "school.png";
        }
    }

    function calcX() {
        var ret = parent.width/2 + (Scripts.kmToPx(distance, 2 * parent.radiusKM, (parent.width < parent.height ? parent.width : parent.height) - 20) * Math.sin(angle));
        return ret
    }

    function calcY() {
        var ret = parent.height/2 + (Scripts.kmToPx(-1 * distance, 2 * parent.radiusKM, (parent.width < parent.height ? parent.width : parent.height) - 20) * Math.cos(angle));
        return ret
    }

    x: calcX() - 25
    y: calcY() - 25

    Text {
        y: + 52
        text: title
    }


    source: getFilePath()

    width: 50
    height: 50


}
