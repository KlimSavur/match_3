import QtQuick 2.12

Item {
    id: root
    required property color display
    required property int index
    required property Item dragParent
    signal pressed(int index)
    signal move(int from, int to)
    DropArea {
        id: _dropArea
        property color display : root.display
        property int index : root.index
        property Item dragParent : root.dragParent

        onDropped: function(drop){
            var from = (drag.source as Rectangle).visualIndex
            var to = _bubble.visualIndex
            move(from, to)

        }
        onEntered: function(drag){
        ;
        }

        width: root.width
        height: root.height

        MouseArea {
            id: _dragArea
            anchors.fill: _bubble
            drag.target: _bubble
            onReleased: _bubble.Drag.drop()
            onPressed: { root.pressed(index)}
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
