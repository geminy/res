import QtQuick 2.3
import QtQuick.Controls 1.2

Item {
    readonly property int srnWidth: screenWidth
    readonly property int srnHeight: screenHeight
    property alias eRestart: start.enabled
    property alias eEnBack: back.enabled

    signal eStart()
    signal eBack()

    width: srnWidth
    height: (srnHeight - srnWidth) / 2 - 25 // gub

    Button {
        id: start
        width: parent.width / 2; height: parent.height
        text: qsTr("Start")
        onClicked: parent.eStart()
    }

    Button {
        id: back
        enabled: false
        x: width
        width: parent.width / 2; height: parent.height
        text: qsTr("Back")
        onClicked: parent.eBack()
    }

    Text {
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        text: "natruEarth"
        font.italic: true
        font.underline: true
        color: "blue"
    }
}
