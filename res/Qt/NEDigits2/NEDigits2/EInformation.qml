import QtQuick 2.3

Rectangle {
    id: root

    readonly property int srnWidth: screenWidth
    readonly property int srnHeight: screenHeight
    property alias eScore: score.text
    property alias eBestScore: bestScore.text
    property alias eStep: step.text
    property alias eTotalStep: totalStep.text

    width: srnWidth
    height: (srnHeight - srnWidth) / 2 - 25 // gub
    color: "lightyellow"

    Grid {
        Text {
            width: root.width / 4; height: root.height / 2
            text: "Score:"
            color: "blue"
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
        }
        Text {
            id: score
            width: root.width / 4; height: root.height / 2
            font.bold: true
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignLeft
        }
        Text {
            width: root.width / 4; height: root.height / 2
            text: "Best:"
            color: "red"
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
        }
        Text {
            id: bestScore
            width: root.width / 4; height: root.height / 2
            font.bold: true
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignLeft
        }
        Text {
            width: root.width / 4; height: root.height / 2
            text: "Step:"
            color: "blue"
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
        }
        Text {
            id: step
            width: root.width / 4; height: root.height / 2
            font.bold: true
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignLeft
        }
        Text {
            width: root.width / 4; height: root.height / 2
            text: "Total:"
            color: "red"
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
        }
        Text {
            id: totalStep
            width: root.width / 4; height: root.height / 2
            font.bold: true
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignLeft
        }
    }
}
