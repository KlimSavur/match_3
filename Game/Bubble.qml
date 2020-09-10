import QtQuick 2.12

Rectangle {
    id: root
    signal setIndex()
    signal release()
    radius: Math.max(height, width) / 2
    border.color: Qt.darker(color)
    border.width: 1
    RoundMouseArea {
        anchors.fill: parent
        onIndexChange: {
            setIndex()
        }
        onRestartPress: {
            if (scale === 1)
            _pressAnimation.restart()
            else{
                _pressAnimation.stop()
                _releaseAnimation.restart()
            }
        }
    }
    onRelease: {
        _pressAnimation.stop()
        _releaseAnimation.restart();
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
