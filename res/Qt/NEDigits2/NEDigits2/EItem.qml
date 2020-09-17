import QtQuick 2.3

Rectangle {
    property alias eNum: innerNum.text
    property alias eNumColor: innerNum.color
    property alias eNumState: innerNum.state

    width: 100; height: width // changed later
    color: "white" // default
    radius: 10

    Behavior on color {
        PropertyAnimation {
            duration: 500
        }
    }

    Text {
        id: innerNum
        anchors.centerIn: parent
        color: "black" // default       
        state: "hide"

        states: [
            State {
                name: "show"
                PropertyChanges {
                    target: innerNum
                    scale: 1
                }
            },
            State {
                name: "hide"
                PropertyChanges {
                    target: innerNum
                    scale: 0
                }
            }
        ]

        transitions: Transition {
            PropertyAnimation {
                target: innerNum
                property: "scale"
                duration: 500
            }
        }
    }
}
