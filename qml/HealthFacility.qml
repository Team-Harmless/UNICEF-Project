import QtQuick 2.0
import QtQuick.Shapes 1.0

Image
{

    width: 130; height: 100
    fillMode: Image.PreserveAspectFit

    source: "healthFacility.svg"
}

/*
Shape {
    width: 200
    height: 150
    anchors.centerIn: parent
    ShapePath {
        strokeWidth: 4
        fillGradient: LinearGradient {
            x1: 20; y1: 20
            x2: 180; y2: 130
            GradientStop { position: 0; color: "blue" }
            GradientStop { position: 0.2; color: "green" }
            GradientStop { position: 0.4; color: "red" }
            GradientStop { position: 0.6; color: "yellow" }
            GradientStop { position: 1; color: "cyan" }
        }
        strokeStyle: ShapePath.DashLine
        dashPattern: [ 1, 4 ]
        startX: 20; startY: 20
        PathLine { x: 180; y: 130 }
        PathLine { x: 20; y: 130 }
        PathLine { x: 20; y: 20 }
    }
}
*/
