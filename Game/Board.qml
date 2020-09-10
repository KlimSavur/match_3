import QtQuick 2.13
import Engine 1.0

GridView {
    id: root
    property alias score: _model.cScore
    property int moves: 0
    property int preesedIndex: -1
    property bool moved: true
    signal restart()
    signal openPopUp()
    interactive: false
    clip: true

    model: BubblesModel {
        id: _model
        onOpenPopup: {
            openPopUp()
            root.preesedIndex = -1
        }
    }



    delegate: Bubble {
        id: _bubble
        width: root.cellWidth - 2
        height: root.cellHeight - 2
        color: display
        onSetIndex: {
            if (root.preesedIndex !== -1){
                _gridView.itemAtIndex(index).release()
                _gridView.itemAtIndex(root.preesedIndex).release()
                _bubble.release()
                _model.move(root.preesedIndex, index)
                root.preesedIndex = -1
            }
            else
                root.preesedIndex = index
        }
    }

    move: Transition {
        SequentialAnimation {
            alwaysRunToEnd: true
            NumberAnimation { easing.type: Easing.InBack; properties: "x,y"; duration: 600 }
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
            NumberAnimation { property: "scale"; to: 0; duration: 600 }
            PropertyAction { property: "GridView.delayRemove"; value: false }
            ScriptAction { script: moved = true }
        }
    }
    add: Transition {
        SequentialAnimation {
            alwaysRunToEnd: true
            NumberAnimation { easing.type: Easing.InBack; property: "y"; from: -cellHeight; duration: 600 }
            ScriptAction { script: moved = true }
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
