import QtQuick 2.3
import naturEarth 1.0

Item {
    id: root

    property int eCount: 16
    property int eI: 0
    property bool eInitCompleted: false

    function eClear() {
        for (eI = 0; eI < eCount; eI++) {
            panel.eNums.itemAt(eI).color = "white";
            panel.eNums.itemAt(eI).eNumState = "hide";
            panel.eNums.itemAt(eI).eNumColor= "black";
            panel.eNums.itemAt(eI).eNum = "";
        }
    }

    function eShow() {
        eClear();
        for (eI = 0; eI < eCount; eI++) {
            if (numProvider.show(eI)) {
                panel.eNums.itemAt(eI).eNum = numProvider.show(eI);
                panel.eNums.itemAt(eI).eNumColor = numProvider.numColor(eI);
                panel.eNums.itemAt(eI).color = numProvider.bkgColor(eI);
                panel.eNums.itemAt(eI).eNumState = "show";

            }
        }
        infomation.eScore = numProvider.score;
        infomation.eStep = numProvider.step;
        infomation.eBestScore = numProvider.bestScore;
        infomation.eTotalStep = numProvider.totalStep;
        if (!tip.eEnBack && 0 < numProvider.step) {
            tip.eEnBack = true;
        }
    }

    function eShowAnim() {
        if (eCount > eI && 0 <= eI) {
            panel.eNums.itemAt(eI).eNum = numProvider.show(eI);
            panel.eNums.itemAt(eI).eNumColor = numProvider.numColor(eI);
            panel.eNums.itemAt(eI).color = numProvider.bkgColor(eI);
            panel.eNums.itemAt(eI).eNumState = "show";

        }
        else if (eCount + 2 < eI) {
            animStart.stop();
            tip.eRestart = true;
            tip.eStart();
            return;
        }
        eI += 1;
    }

    Timer {
        id: animStart
        interval: 250
        repeat: true
        triggeredOnStart: true
        onTriggered: {
            tip.eRestart = false
            eShowAnim()
        }
    }

    width: screenWidth; height: screenHeight

    Keys.onPressed: {
        switch (event.key) {
        case Qt.Key_Up:
            numProvider.move(NE2048.Move_Up);
            root.eShow();
            break;
        case Qt.Key_Down:
            numProvider.move(NE2048.Move_Down);
            root.eShow();
            break;
        case Qt.Key_Left:
            numProvider.move(NE2048.Move_Left);
            root.eShow();
            break;
        case Qt.Key_Right:
            numProvider.move(NE2048.Move_Right);
            root.eShow();
            break;
        default:
            break;
        }
    }

    NE2048 { id: numProvider }
    Connections {
        target: numProvider
        onGameOver: endAnim.start()
    }

    Rectangle {
        id: container
        anchors.fill: parent
        color: "white"

        EInformation { id: infomation }

        EPanel {
            id: panel
            anchors.top: infomation.bottom

            onToRight: {
                numProvider.move(NE2048.Move_Right);
                root.eShow();
            }
            onToLeft: {
                numProvider.move(NE2048.Move_Left);
                root.eShow();
            }
            onToUp: {
                numProvider.move(NE2048.Move_Up);
                root.eShow();
            }
            onToDown: {
                numProvider.move(NE2048.Move_Down);
                root.eShow();
            }

            Rectangle {
                id: flipRect
                width: parent.width / 2; height: parent.height / 2
                anchors.centerIn: parent
                color: "red"
                radius: 10
                visible: false

                Text {
                    id: flipText
                    text: "game over ^_^"
                    anchors.centerIn: parent
                    scale: 0

                    Behavior on scale {
                        PropertyAnimation {
                            duration: 200
                        }
                    }
                }

                Timer {
                    id: endAnim
                    property int flipCount: 0
                    interval: 300
                    repeat: true
                    triggeredOnStart: true
                    onTriggered: {
                        if (6 <= flipCount) {
                            flipCount = 0
                        }
                        flipRect.visible = true
                        tip.eRestart = false
                        tip.eEnBack = false
                        root.focus = false
                        panel.enabled = false
                        if (flipCount % 2) {
                            flipText.scale -= 1
                        }
                        else {
                            flipText.scale += 2
                        }
                        flipCount += 1
                        if (5 < flipCount) {
                            endAnim.stop()
                            tip.eRestart = true
                            tip.eEnBack = true
                        }
                    }
                }
            }
        }

        ETip {
            id: tip
            anchors.top: panel.bottom

            onEStart: {
                if (flipRect.visible) {
                    flipText.scale = 0
                    flipRect.visible = false
                }
                if (root.eInitCompleted) {
                    numProvider.start()
                    root.eShow()
                    root.focus = true
                    panel.enabled = true
                    tip.eEnBack = false
                }
                else {
                    root.eInitCompleted = true
                    numProvider.initialize()
                    animStart.start()
                }

            }
            onEBack: {
                if (flipRect.visible) {
                    flipText.scale = 0
                    flipRect.visible = false
                    root.focus = true
                    panel.enabled = true
                }
                numProvider.backed()
                root.eShow()
                if (!numProvider.step) {
                    tip.eEnBack = false
                }
            }
        }
    }
}
