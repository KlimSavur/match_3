import QtQuick 2.12

MouseArea {
    id:root
    hoverEnabled: enabled
    function containsMouse() {
        var x1 = root.width / 2;
        var y1 = root.height / 2;
        var x2 = mouseX;
        var y2 = mouseY;
        var distanceFromCenter = Math.pow(x1 - x2, 2) + Math.pow(y1 - y2, 2);
        var radiusSquared = Math.pow(Math.min(width, height) / 2, 2);
        var isWithinOurRadius = distanceFromCenter < radiusSquared;
        return isWithinOurRadius;
    }
}
