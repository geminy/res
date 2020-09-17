import QtQuick 2.3

Rectangle {
    property alias eNum: innerNum.text
    property alias eNumColor: innerNum.color

    width: 100; height: width // changed later
    color: "white" // default
    radius: 10

    Text {
        id: innerNum
        anchors.centerIn: parent
        color: "black" // default
    }
}
