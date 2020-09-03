import QtQuick 2.12
import Engine 1.0

GridView {
    id: root
    property int score: 0
    property int moves: 0
    property int preesedIndex: -1
    signal restart()
    interactive: false
    clip: true

    model: BubblesModel {
        id: _model
    }

    delegate: Bubble {
        id: _bubble
        width: root.cellWidth - 2
        height: root.cellHeight - 2
        colorDelegate: display
        onSetIndex: {
            root.preesedIndex = index
        }
        onCollapse: {

        }

        onMove: {
            _model.move(root.preesedIndex, index)
            root.preesedIndex = -1
        }
    }
        GridView.delayRemove: true
    move: Transition {
        SequentialAnimation{
            NumberAnimation { properties: "x,y"; duration: 400; }
            ScriptAction { script:  _model.add()}
            ScriptAction { script:  _model.update()}
        }
    }


    onRestart: {
        _model.generateBoard()
        score = 0
        moves = 0
    }
}
