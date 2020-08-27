import QtQuick 2.12

Rectangle {
    id: rootB
    required property color display
    required property int index
    required property Item dragParent
    DropArea {
        id: root
        property color display : parent.display
        property int index : parent.index
        property Item dragParent : parent.dragParent
        property alias scale: _bubble.scale
        onDropped: function(drop){
            var from = (drag.source as Rectangle).visualIndex
            var to = _bubble.visualIndex
            console.log("from " + from + " to " + to)
            _model.move(from, to)
        }
        width: parent.width
        height: parent.height
        Rectangle {
            id: _bubble
            property Item dragParent: root.dragParent
            property int visualIndex: root.index

            radius: Math.max(height, width) / 2
            border.color: Qt.darker(color)
            border.width: 1
            width: root.height
            height: root.width
    //        opacity: 0.7
            color: root.display
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
        MouseArea {
            id: _dragArea
            anchors.fill: _bubble
            drag.target: _bubble
            onReleased: _bubble.Drag.drop()
        }
    }

//    GridView.onRemove: SequentialAnimation {
//                           PropertyAction { target: rootB; property: "z"; value: 100}
//                           PropertyAction { target: rootB; property: "GridView.delayRemove"; value: true }
//                           NumberAnimation { target: rootB; property: "y"; to: 2000; duration: 2500; easing.type: Easing.InOutQuad }
//                           PropertyAction { target: rootB; property: "GridView.delayRemove"; value: false }
//                       }
//    GridView.onAdd : SequentialAnimation {
//        NumberAnimation { target: rootB; property: "y"; from: -200; duration: 2500}
//    }
}
