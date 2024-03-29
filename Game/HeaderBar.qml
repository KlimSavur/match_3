import QtQuick 2.0
import QtQuick.Controls 2.12

ToolBar {
    id: root
    property int score: 0
    property int moves: 0
    signal restartPressed()
    height: 70
    background: Rectangle {
        anchors.fill: root
        opacity: 0
    }
    Text {
        anchors.left: parent.left
        anchors.verticalCenter: parent.verticalCenter
        anchors.leftMargin: root.height
        text: root.moves
    }
    ToolButton {
        anchors.centerIn: parent
        text: qsTr("Restart")
        onClicked: root.restartPressed()
    }
    Text {
        anchors.right: parent.right
        anchors.rightMargin: parent.height
        anchors.verticalCenter: parent.verticalCenter
        text: root.score
    }
}
