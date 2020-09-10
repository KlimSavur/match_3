import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Window 2.12
import Engine 1.0
ApplicationWindow {
    id: _window
    property int cellHeightJSON: 0
    property int cellWidthJSON: 0
    visible: true
    width: 480
    title: qsTr("Match 3")

    background: WindowBG { anchors.fill: parent }

    header: HeaderBar {
        id: _header
        score: _gridView.score
        moves: _gridView.moves
        onRestartPressed: _gridView.restart();
    }

    Board {
        id: _gridView
        anchors.fill: parent
        cellWidth: cellWidthJSON
        cellHeight: cellHeightJSON
        onOpenPopUp: {
            _popup.open()
        }
    }
    LossPopup {
        id: _popup
        anchors.centerIn: parent
        scroreVar: _gridView.score
        movesVar: _gridView.moves
        width: _window.width / 2
        height: _window.height / 2
        onPressed: {
            _gridView.restart()
        }
    }

    Component.onCompleted: {
        if (JSON_loader.columns() > 20){
            width = 600
        }
        height = _header.height + (_header.width / JSON_loader.columns()) * JSON_loader.rows()
        if (height > Screen.desktopAvailableHeight){
            height = Screen.desktopAvailableHeight;
            cellHeightJSON = (height - _header.height) / JSON_loader.rows()
            width = cellHeightJSON * JSON_loader.columns()
        } else {
            cellHeightJSON = width / JSON_loader.columns()
        }
        maximumHeight= height
        maximumWidth= width
        minimumHeight= height
        minimumWidth= width
        cellWidthJSON = width / JSON_loader.columns()
    }
}
