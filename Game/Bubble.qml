import QtQuick 2.12

Rectangle {
    id: root
    property color colorDelegate
    signal move()
    signal setIndex(int pos)
    radius: Math.max(height, width) / 2
    color: colorDelegate
    border.color: Qt.darker(color)
    border.width: 1
    RoundMouseArea{
        anchors.fill: parent
        onPressed: {
            if (containsMouse()) {
                setIndex(0)
                _pressAnimation.restart()
            }
        }
        onReleased: {
            _releaseAnimation.restart()
            if (containsMouse()){
                setIndex(-1)
            }
        }
        onEntered: {
            move()
        }
    }
    NumberAnimation {
        id: _pressAnimation
        target: root;
        property: "scale";
        to: 0.9
        duration: 100
    }
    NumberAnimation {
        id: _releaseAnimation
        target: root;
        property: "scale";
        to: 1
        duration: 100
    }
    gradient: Gradient {
        orientation: Gradient.Horizontal
        GradientStop { position: 0.0; color: _bubble.color }
        GradientStop { position: 0.5; color: Qt.lighter(_bubble.color)}
    }


}
