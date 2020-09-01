import QtQuick 2.12
import Engine 1.0

GridView {
    id: root
    property int score: 0
    signal restart()
    interactive: false
    clip: true

    model: BubblesModel {
        id: _model
    }

    delegate: Bubble {
        width: root.cellWidth - 2
        height: root.cellHeight - 2
        dragParent: root
        onRemove: {
            _model.remove();
        }

        onMove: {
            _model.move(from, to)
            score++
        }
        onColapse: {
            _model.colapce()
        }
    }

    onRestart: {
        _model.generateBoard()
        score = 0
    }


}
