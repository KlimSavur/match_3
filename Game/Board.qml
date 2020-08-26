import QtQuick 2.12

GridView {
    id: root
    interactive: false
    clip: true
    delegate: Bubble {
        width: root.cellWidth - 2
        height: root.cellHeight - 2
    }
}
