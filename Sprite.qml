import QtQuick 2.0

Image{
    signal updateXY
    onUpdateXY: {
        x: calcX()
        y: calcY()
    }
    property string spriteType: "hosp"
    property real angle: 0;
    property real distance: 0;

    function getFilePath() {
        if (spriteType == "hosp") {
            console.log("Hosp image ")
            return "hosp.png"
        }
        else {
            console.log("School image ")
           return "school.png";
        }
    }

    function calcX() {
        var ret = parent.width/2 + (distance * Math.sin(angle));
        console.log("x: " + ret + " From anngle: " + angle + " distance: " + distance);
        return ret
    }

    function calcY() {
        var ret = parent.height/2 + (distance * Math.cos(angle));
        console.log("y: " + ret);
        return ret
    }

    x: calcX()
    y: calcY()


    source: getFilePath()

    width: 50
    height: 50
}
