import QtQuick 2.12
import Engine 1.0

GridView {
    id: root
    property int current: 0
    property int score: 0
    signal restart()
    interactive: false
    clip: true

    model: BublesModel{
        id: _model
    }

    delegate: Bubble {
        width: root.cellWidth - 2
        height: root.cellHeight - 2
        dragParent: root
        onPressed: {
            current = index;
        }
        onMove: {
            _model.move(from, to)
            score++
        }
    }

    onRestart: {
        _model.generateBoard()
    }
    displaced: Transition{
        NumberAnimation{ properties: "x,y"; easing.type: Easing.OutInBack;}
    }
    GridView.delayRemove: true
    remove: Transition{
        SequentialAnimation{
//            PropertyAction { property: "GridView.delayRemove"; value: true }
            NumberAnimation { property: "scale"; to: 0; duration: 1000; easing.type: Easing.InElastic }
//            PropertyAction { property: "GridView.delayRemove"; value: false }
        }
    }

    move: Transition {
        NumberAnimation { properties: "x, y"; duration: 3000; easing.type:  Easing.InOutBounce}
    }
}
