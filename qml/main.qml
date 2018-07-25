import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import QtQuick.Shapes 1.0



Frame {
    visible: true
    width: 640
    height: 800

    Loader
    {
        anchors.centerIn: parent
        width: 300
        height: 300
        source: "School.qml"
    }

    Loader
    {
        source: "HealthFacility.qml"
    }
}

