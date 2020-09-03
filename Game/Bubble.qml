import QtQuick 2.12

Rectangle {
    id: root
    property color colorDelegate
    signal move()
    signal remove()
    signal setIndex()
    signal deleteIndex()
    signal collapse()
    radius: Math.max(height, width) / 2
    color: colorDelegate
    border.color: Qt.darker(color)
    border.width: 1
    MouseArea{
        anchors.fill: parent
        hoverEnabled: enabled
        onPressed: {
            setIndex()
            _pressAnimation.restart()
        }
        onReleased: {
            _pressAnimation.restart()
        }
        onEntered: {
            move()
        }
    }
    NumberAnimation {
        id: _pressAnimation
        target: root;
        property: "scale";
        to: (scale === 0.9) ? (1) : (0.9)
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
        NumberAnimation { target: root; property: "scale"; to: 0; duration: 700; easing.type: Easing.InElastic }
        PropertyAction { target: root; property: "GridView.delayRemove"; value: false }
        ScriptAction { script: _model.update()}
    }
    GridView.onAdd: SequentialAnimation {
        id: animation1
        alwaysRunToEnd: true
        NumberAnimation {target: root; property: "scale"; from: 0; to: 1; duration: 400}
        ScriptAction {script:  _model.update()}

    }
}
