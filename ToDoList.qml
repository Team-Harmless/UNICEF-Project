import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import QtQuick.Window 2.2

import QtQuick.Dialogs 1.1
import ToDo 1.0




ColumnLayout {
    Frame {
        Layout.fillWidth: true

    Rectangle {
              width: parent.width<parent.height?parent.width:parent.height
              height: width
              color: "red"
              border.color: "black"
              border.width: 100
              radius: width*0.5
    }
    /*
    Rectangle {
        width: 360
        height: 360

        MessageDialog {
            id: msg
            title: "Title"
            text: Screen.width + " " + Screen.height
            onAccepted: visible = false
        }

        Button {
            text: "press me"
            onClicked: msg.visible = true
        }
    }

        ListView {
            implicitWidth: 250
            implicitHeight: 250
            anchors.fill: parent
            clip: true

            model: ToDoModel {
                list: toDoList
            }

            delegate: RowLayout {
                width: parent.width

                CheckBox {
                    checked: model.done
                    onClicked: model.done = checked
                }
                TextField {
                    text: model.description
                    onEditingFinished: model.description = text
                    Layout.fillWidth: true
                }
            }
        }
    }

    RowLayout {
        Button {
            text: qsTr("Add new item")
            onClicked: toDoList.appendItem()
            Layout.fillWidth: true
        }
        Button {
            text: qsTr("Remove completed")
            onClicked: toDoList.removeCompletedItems()
            Layout.fillWidth: true
        }
        Button {
            text: qsTr("Show screen height and width")
            onClicked: toDoList.removeCompletedItems()
            Layout.fillWidth: true
        }

    */
    }
}
