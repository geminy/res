import QtQuick 2.3
import naturEarth 1.0

Item {
    id: root

    property int eI

    function eClear() {
        for(eI = 0; eI < 16; eI++) {
            panel.eNums.itemAt(eI).eNum = "";
            panel.eNums.itemAt(eI).color = "white";
            panel.eNums.itemAt(eI).eNumColor= "black";
        }
    }

    function eShow() {
        eClear();
        for(eI = 0; eI < 16; eI++) {
            if(numProvider.show(eI)) {
                panel.eNums.itemAt(eI).eNum = numProvider.show(eI);
                panel.eNums.itemAt(eI).color = numProvider.color(eI);
                panel.eNums.itemAt(eI).eNumColor = numProvider.numColor(eI);
            }
        }
        infomation.eScore = numProvider.score;
        infomation.eStep = numProvider.step;
        infomation.eBestScore = numProvider.bestScore;
        infomation.eTotalStep = numProvider.totalStep;
        if(0 < numProvider.step) {
            tip.eEnBack = true;
        }
    }

    width: 360; height: 560

    Keys.onPressed: {
        switch(event.key) {
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

    Rectangle {
        id: container
        anchors.fill: parent
        color: "white"

        EInformation { id: infomation }

        EPanel {
            id: panel
            anchors.centerIn: parent
        }

        ETip {
            id: tip
            anchors.bottom: parent.bottom
            onEStart: {
                numProvider.start()
                root.eShow();
                root.focus = true;
                tip.eEnBack = false;
            }
            onEBack: {
                numProvider.backed();
                root.eShow();
                if(!numProvider.step) {
                    tip.eEnBack = false;
                }
            }
        }
    }
}
