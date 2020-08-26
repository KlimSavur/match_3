import QtQuick 2.12
import Engine 1.0

GridView {
    id: root
    property int wRows: 0
    property int wColumns: 0
    interactive: false
    clip: true

    model: BublesModel{
        id: _model
    }

    delegate: Bubble {
        width: root.cellWidth - 2
        height: root.cellHeight - 2
        bubbleColor: display
    }
}
