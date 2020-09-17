import QtQuick 2.3

Item {
    readonly property int srnWidth: screenWidth
    property var eNums: repeater
    property int pressX: 0
    property int pressY: 0
    property int releaseX: 0
    property int releaseY: 0

    signal toRight()
    signal toLeft()
    signal toUp()
    signal toDown()

    function chooseDirection() {
        if (Math.abs(releaseX - pressX) > Math.abs(releaseY - pressY)) {
            if (releaseX > pressX) {
                toRight();
            }
            else if (releaseX < pressX) {
                toLeft();
            }
        }
        else if (Math.abs(releaseX - pressX) < Math.abs(releaseY - pressY)) {
            if (releaseY > pressY) {
                toDown();
            }
            else if (releaseY < pressY) {
                toUp();
            }
        }
        else {
            // invalid direction
        }
    }

    width: srnWidth; height: width
    enabled: false

    Rectangle {
        id: container
        width: parent.width - radius * 2
        height: width
        anchors.centerIn: parent
        color: "lightblue"
        radius: 20

        Grid {
            id: grid
            anchors.centerIn: parent
            spacing: 10

            Repeater {
                id: repeater
                model: 16

                EItem {
                    width: (container.width - grid.spacing * 5) / 4
                }
            }
        }

        MouseArea {
            anchors.fill: parent
            onPressed: {
                pressX = mouse.x
                pressY = mouse.y
            }
            onReleased: {
                releaseX = mouse.x
                releaseY = mouse.y
                chooseDirection()
            }
        }
    }
}
