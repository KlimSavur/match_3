import QtQuick 2.12
import QtGraphicalEffects 1.0
Rectangle {
    id: root
    property color bubbleColor: "red"
    radius: Math.max(height, width) / 2
    border.color: Qt.darker(bubbleColor)
    border.width: 1
    opacity: 0.7
    gradient: Gradient {
        orientation: Gradient.Horizontal
        GradientStop { position: 0.0; color: root.bubbleColor }
        GradientStop { position: 0.5; color: Qt.lighter(root.bubbleColor)}
    }
}
