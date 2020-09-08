import QtQuick 2.12
import QtQuick.Controls 2.12

Popup {
    id: root
    property int scroreVar: 0
    property int movesVar: 0
    signal pressed()
    closePolicy: Popup.CloseOnEscape
    opacity: 0.88

    contentItem:
        Rectangle {
            anchors.fill: parent
            color: "lightblue"
            Text {
                anchors.centerIn: parent
                font.bold: true
                text: "You loss! \nScore: "  + root.scroreVar + "\nMoves: " + root.movesVar
            }

            Button {
                width: root.width
                height: root.height / 4
                anchors.bottom: parent.bottom
                text: qsTr("Close and Restart")

                background: Rectangle {
                    anchors.fill: parent
                    color: "red"
                }

                onClicked: {
                    root.close()
                    root.pressed()
                }
            }
        }
}
