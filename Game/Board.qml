import QtQuick 2.12
import Engine 1.0

GridView {
    id: root
    property int score: 0
    property int moves: 0
    property int columns: 0
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
        dragParent: root
        colorDelegate: display
        position: index
        onRemove: {
            _model.remove();
        }
        onCollapse: {
            _model.collapse()
        }
        onScore: {
            root.score++
        }
        onMove: {
            var offset = to - from
            if ((_model.move(from, to)) && (Math.abs(offset) == 1 || Math.abs(offset) == columns)){
                root.moves++
            }
        }
    }

    onRestart: {
        _model.generateBoard()
        score = 0
        moves = 0
    }
}
