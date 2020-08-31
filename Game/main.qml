import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Window 2.12
ApplicationWindow {
    id: _window
    property int cellHeightJSON: 0
    property int cellWidthJSON: 0
    property int score: _gridView.score
    visible: true
    width: 480
    title: qsTr("Match 3")

    background: WindowBG { anchors.fill: parent }

    header: HeaderBar {
        id: _header
        moves: parent.score
        onRestartPressed: _gridView.restart();
    }

    Board {
        id: _gridView
        anchors.fill: parent
        cellWidth: cellWidthJSON
        cellHeight: cellHeightJSON
    }

    Component.onCompleted: {
        var xhr = new XMLHttpRequest;
        xhr.open("GET", ":/../cfg.json");
        xhr.onreadystatechange = function() {
            if (xhr.readyState === XMLHttpRequest.DONE) {
                var js = JSON.parse(xhr.responseText)
                if (js.width > 20){
                    width = 600
                }
                height = _header.height + (_header.width / js.width) * js.height
                if (height > Screen.desktopAvailableHeight){
                    height = Screen.desktopAvailableHeight;
                    cellHeightJSON = (height - _header.height) / js.height
                    width = cellHeightJSON * js.width
                } else {
                    cellHeightJSON = width / js.width
                }
                maximumHeight= height
                maximumWidth= width
                minimumHeight= height
                minimumWidth= width
                cellWidthJSON = width / js.width
            }
       }
       xhr.send();
    }
}
