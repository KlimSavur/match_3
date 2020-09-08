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
        onCScoreChanged: {
            root.score = cScore
        }
    }

    delegate: Bubble {
        id: _bubble
        width: root.cellWidth - 2
        height: root.cellHeight - 2
        colorDelegate: display
        onSetIndex: {
            root.preesedIndex = (pos === 0) ? index : -1
        }
        onMove: {
            if (_model.move(root.preesedIndex, index))
                moves++;
            root.preesedIndex = -1
        }
    }

    move: Transition {
        SequentialAnimation{
            alwaysRunToEnd: true
            NumberAnimation { easing.type: Easing.InBack; properties: "x,y"; duration: 600}
        }
        onRunningChanged: {
            if (!running && moved){
                moved = false;
                _model.checkMatch()
            }

        }
    }

    remove: Transition {
        SequentialAnimation {
            alwaysRunToEnd: true
            PropertyAction { property: "GridView.delayRemove"; value: true }
            NumberAnimation { property: "scale"; to: 0; duration: 600; }
            PropertyAction { property: "GridView.delayRemove"; value: false }
            ScriptAction { script: moved = true;}
        }
    }
    add: Transition {
        SequentialAnimation{
            alwaysRunToEnd: true
            NumberAnimation { easing.type: Easing.InBack; property: "y"; from: -cellHeight; duration: 600}
            ScriptAction { script: moved = true;}
        }
        onRunningChanged: {
            if (!running){
                _model.checkMatch()
            }

        }
    }

    onRestart: {
        _model.generateBoard()
        score = 0
        _model.cScore = 0
        moves = 0
    }
}
