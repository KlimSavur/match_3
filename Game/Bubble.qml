import QtQuick 2.12

Item {
    id: root
    GridView.onRemove: SequentialAnimation {
            PropertyAction { target: root; property: "GridView.delayRemove"; value: true }
            NumberAnimation { target: root; property: "scale"; to: 0; duration: 1000; easing.type: Easing.InElastic }
            PropertyAction { target: root; property: "GridView.delayRemove"; value: false }
            ScriptAction { script: colapse() }
            NumberAnimation { target: root; property: "opacity"; to: 0; duration: 200}

            ScriptAction { script: remove() }
    }

    required property color display
    required property int index
    required property Item dragParent
    signal move(int from, int to)
    signal remove()
    signal colapse()
    DropArea {
        id: _dropArea
        property color display : root.display
        property int index : root.index
        property Item dragParent : root.dragParent

        onDropped: function(drop){
            remove()
        }
        onEntered: function(drag){
        var from = (drag.source as Rectangle).visualIndex
            var to = _bubble.visualIndex
            move(from, to)
        }

        width: root.width
        height: root.height

        MouseArea {
            id: _dragArea
            anchors.fill: _bubble
            drag.target: _bubble
            onReleased: _bubble.Drag.drop()
        }

        Rectangle {
            id: _bubble
            property Item dragParent: _dropArea.dragParent
            property int visualIndex: _dropArea.index
            radius: Math.max(height, width) / 2
            border.color: Qt.darker(color)
            border.width: 1
            width: _dropArea.height
            height: _dropArea.width
            color: _dropArea.display
            Drag.active: _dragArea.drag.active
            Drag.source: _bubble
            Drag.hotSpot.x: width / 2
            Drag.hotSpot.y: height / 2

            gradient: Gradient {
                orientation: Gradient.Horizontal
                GradientStop { position: 0.0; color: _bubble.color }
                GradientStop { position: 0.5; color: Qt.lighter(_bubble.color)}
            }

            states: [
                State {
                    when: _dragArea.drag.active
                    ParentChange {
                        target: _bubble
                        parent: _bubble.dragParent
                    }
                }
            ]

        }
    }
}
