import QtQuick 2.12

Rectangle {
    id: root
    property color colorDelegate
    signal move()
    signal remove()
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

    GridView.onRemove: SequentialAnimation {
        id: animation
        alwaysRunToEnd: true
        PropertyAction { target: root; property: "GridView.delayRemove"; value: true }
        NumberAnimation { target: root; property: "scale"; to: 0; duration: 1000; easing.type: Easing.InElastic }
        PropertyAction { target: root; property: "GridView.delayRemove"; value: false }
        ScriptAction { script: remove()}

    }
    GridView.onAdd: SequentialAnimation {
        id: animation1
        alwaysRunToEnd: true
        NumberAnimation {target: root; property: "y"; from: -300; to: y; duration: 1000}
    }
}
