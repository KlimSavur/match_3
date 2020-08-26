import QtQuick 2.12
import QtQuick.Controls 2.12
ApplicationWindow {
    id: _window

    property int xCell: 0
    property int yCell: 0

    visible: true
    width: 480
    height: _header.height + width / xCell * yCell
    title: qsTr("Match 3")

    background: WindowBG { anchors.fill: parent }

    header: HeaderBar { id: _header }

    Board {
        id: _gridView
        anchors.fill: parent
        cellWidth: _window.width / _window.xCell
        cellHeight: (_window.height - _header.height) / _window.yCell
        model: _window.xCell*_window.yCell
    }

    Component.onCompleted: {
        var xhr = new XMLHttpRequest;
        xhr.open("GET", ":/../cfg.json");
        xhr.onreadystatechange = function() {
            if (xhr.readyState === XMLHttpRequest.DONE) {
                var js = JSON.parse(xhr.responseText)
                _window.xCell = js.width
                _window.yCell = js.height
            }
       }
       xhr.send();
    }
}
