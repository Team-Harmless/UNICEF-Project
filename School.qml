import QtQuick 2.0

Rectangle {
          width: parent.width<parent.height?parent.width:parent.height
          height: width
          color: "red"
          border.color: "black"
          border.width: 1
          radius: width*0.5
     }
