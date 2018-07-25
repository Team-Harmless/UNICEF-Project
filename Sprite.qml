import QtQuick 2.0

Image{
    signal updateXY
    onUpdateXY: {
        x: calcX()
        y: calcY()
    }

    id: sprite
    property string spriteType: "hosp"
    property real angle: 0;
    property real distance: 0;

    function getFilePath() {
        if (spriteType == "hosp") {
            console.log("Hosp image")
            return "hosp.png"
        }
        else {
           return "school.png";
        }
    }

    function calcX() {
        return parent.width/2 + (distance * Math.sin(angle));
    }

    function calcY() {
        return parent.height/2 + distance * (Math.cos(angle))
    }

    x: calcX()
    y: calcY()


    source: getFilePath()

    width: 50
    height: 50
}
