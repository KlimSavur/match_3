import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Window 2.12
ApplicationWindow {
    id: _window
    property int cellHeightJSON: 0
    property int cellWidthJSON: 0
    property int columns: 0
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
        columns: _window.columns
    }

    Component.onCompleted: {
        var xhr = new XMLHttpRequest;
        xhr.open("GET", ":/../cfg.json");
        xhr.onreadystatechange = function() {
            if (xhr.readyState === XMLHttpRequest.DONE) {
                var js = JSON.parse(xhr.responseText)
                if (js.columns > 20){
                    width = 600
                }
                height = _header.height + (_header.width / js.columns) * js.rows
                if (height > Screen.desktopAvailableHeight){
                    height = Screen.desktopAvailableHeight;
                    cellHeightJSON = (height - _header.height) / js.rows
                    width = cellHeightJSON * js.columns
                } else {
                    cellHeightJSON = width / js.columns
                }
                maximumHeight= height
                maximumWidth= width
                minimumHeight= height
                minimumWidth= width
                columns = js.columns
                cellWidthJSON = width / js.columns
            }
       }
       xhr.send();
    }
}
