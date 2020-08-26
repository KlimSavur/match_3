import QtQuick 2.12
import QtQuick.Controls 2.12
ApplicationWindow {
    id: _window

    property int columns: 0
    property int rows: 0

    visible: true
    width: 480
    height: _header.height + width / columns * rows
    title: qsTr("Match 3")

    background: WindowBG { anchors.fill: parent }

    header: HeaderBar {
        id: _header
        moves: 0
        score: 0
        onRestartPressed: ;
    }

    Board {
        id: _gridView
        anchors.fill: parent
        cellWidth: _window.width / _window.columns
        cellHeight: (_window.height - _header.height) / _window.rows
    }

    Component.onCompleted: {
        var xhr = new XMLHttpRequest;
        xhr.open("GET", ":/../cfg.json");
        xhr.onreadystatechange = function() {
            if (xhr.readyState === XMLHttpRequest.DONE) {
                var js = JSON.parse(xhr.responseText)
                _window.columns = js.width
                _window.rows = js.height
            }
       }
       xhr.send();
    }
}
