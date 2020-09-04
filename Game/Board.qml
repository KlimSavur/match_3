import QtQuick 2.12
import Engine 1.0

GridView {
    id: root
    property int score: 0
    property int moves: 0
    property int preesedIndex: -1
    property bool moved: true
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
            id: _anim
            NumberAnimation { properties: "x,y"; duration: 1000; }
            ScriptAction {
                script: {
                    if (root.moved){
                        _model.remove();
                        root.moved = false
                    }
                }
            }
        }
    }

    addDisplaced: Transition{
        enabled: false
    }

    removeDisplaced: Transition {
        enabled: false
    }
    displaced: Transition{
        enabled: false
    }


    onRestart: {
        _model.generateBoard()
        score = 0
        moves = 0
    }
}
