/***************************************************************************
 *   This file is part of the Kanagram project                             *
 *   Copyright 2014 Debjit Mondal <debjitmondal05@gmail.com>               *
 *   Copyright 2014 Jeremy Whiting <jpwhiting@kde.org>                     *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.          *
 ***************************************************************************/

import QtQuick
import QtQuick.Controls

Rectangle {

    id: blackboard
    radius: 4
    property alias showHintTimeInterval: hintButton.countDownTimerValue
    property alias activeTimer: scoreTimer.running

    signal showWiki
    signal nextAnagram

    function wikiClosed() {
        wikiButton.wikiLinkActivated = false
        if (activeTimer) {
            kanagramGame.answerRevealed();
        }
        revealButton.countDownTimerValue = 2
        showAnswerTimer.repeat = true
        showAnswerTimer.start()
    }

    function letterButtonClicked(index) {
        kanagramGame.moveLetterToUserAnswer(index);
        checkSolved();
    }

    Item {
        width: parent.width
        height: parent.height

        Image {
            id: background
            smooth: true
            anchors.fill: parent
            source: "images/chalkboard.png"
        }
    }

    Item {
        id: categoryBar
        anchors.top: parent.top
        width: parent.width
        height: Math.min(parent.height / 5, 4 * categoryName.height)
        Rectangle {
            anchors.fill: parent
            opacity: .25
            color: "black"
        }

        ToolButton {
            height: categoryBar.height
            width: categoryBar.height
            id: nextVocabularyButton
            anchors {
                verticalCenter: categoryBar.verticalCenter
                right: categoryBar.right
                rightMargin: parent.width / 20
            }
            action: Action {
                shortcut: "PgDown"
                onTriggered: {
                    kanagramGame.nextVocabulary();
                    nextAnagram();
                }
                text: i18n("Next Vocabulary")
            }
            Image {
                source: "icons/right-arrow.png"
                anchors.fill: parent
                anchors.margins: 4
                fillMode: Image.PreserveAspectFit
            }
            background: Rectangle { color: "transparent" }
        }

        Text {
            id: categoryName
            anchors {
                verticalCenter: categoryBar.verticalCenter
                horizontalCenter: categoryBar.horizontalCenter
            }
            color: "white"
            text: kanagramGame.title
            font.pixelSize: Math.min(parent.width / 19.5, 32)
        }

        ToolButton {
            id: previousVocabularyButton
            height: categoryBar.height
            width: categoryBar.height
            anchors {
                verticalCenter: categoryBar.verticalCenter
                left: categoryBar.left
                leftMargin: parent.width / 20
            }
            action: Action {
                shortcut: "PgUp"
                onTriggered: {
                    kanagramGame.previousVocabulary();
                    nextAnagram();
                }
                text: i18n("Previous Vocabulary")
            }
            Image {
                source: "icons/left-arrow.png"
                anchors.fill: parent
                anchors.margins: 4
                fillMode: Image.PreserveAspectFit
            }
            background: Rectangle { color: "transparent" }
        }
    }

    Item {
        id: optionsBar
        width: parent.width
        height: categoryBar.height
        anchors.bottom: parent.bottom
        Rectangle {
            anchors.fill: parent
            opacity: .25
            color: "black"
        }

        ToolButton {
            id: scoreButton
            height: optionsBar.height
            width: optionsBar.height
            anchors {
                verticalCenter: optionsBar.verticalCenter
                left: optionsBar.left
            }
            property int countDownTimerValue: 0
            property bool flagToggleTimer: false
            action: Action {
                id: scoreAction
                shortcut: "Ctrl+S"
                onTriggered: {
                    if (!scoreButton.flagToggleTimer) {
                        scoreButton.countDownTimerValue = kanagramGame.scoreTime();
                        scoreTimer.repeat = true;
                        scoreTimer.start();
                        kanagramGame.resetTotalScore();
                        scoreSection.opacity = 0.35;
                        score.opacity = 1;
                    } else {
                        scoreButton.countDownTimerValue = 1
                    }
                    scoreButton.flagToggleTimer = !scoreButton.flagToggleTimer;
                }
                text: i18n("Start Timer");
            }
            Image {
                source: "icons/timer.png"
                anchors.fill: parent
                anchors.margins: 4
                fillMode: Image.PreserveAspectFit
            }
            background: Rectangle { color: "transparent" }
        }

        ToolButton {
            id: hintButton
            action: Action {
                id: hintAction
                shortcut: "Ctrl+H"
                onTriggered: {
                    hintButton.countDownTimerValue = kanagramGame.hintHideTime()
                    hintTimer.repeat = true
                    hintTimer.start()
                }
                text: i18n("Show Hint")
            }
            width: optionsBar.height
            height: optionsBar.height
            property int countDownTimerValue: 0
            anchors {
                verticalCenter: optionsBar.verticalCenter
                horizontalCenter: optionsBar.horizontalCenter
            }
            Image {
                source: "icons/hint.png"
                anchors.fill: parent
                anchors.margins: 4
                fillMode: Image.PreserveAspectFit
            }
            background: Rectangle { color: "transparent" }
        }

        Action {
            id: revealAction
            shortcut: "Ctrl+R"
            onTriggered: {
                kanagramGame.revealWord();
                if (blackboard.activeTimer) {
                    kanagramGame.answerRevealed();
                }
                revealButton.countDownTimerValue = 2
                showAnswerTimer.repeat = true
                showAnswerTimer.start()
            }
            text: i18n("Reveal Word")
        }

        ToolButton {
            id: revealButton
            action: revealAction
            width: optionsBar.height
            height: optionsBar.height
            anchors {
                verticalCenter: optionsBar.verticalCenter
                right: optionsBar.right
            }
            property int countDownTimerValue: 0
            Image {
                source: "icons/reveal.png"
                anchors.fill: parent
                anchors.margins: 4
                fillMode: Image.PreserveAspectFit
            }
            background: Rectangle { color: "transparent" }
        }

        Rectangle {
            id: timerSection
            width: blackboard.width / 9
            height: blackboard.height / 7
            anchors {
                top: blackboard.top
                topMargin: blackboard.height / 7
                right: blackboard.left
                rightMargin: blackboard.width / 68.5
            }
            color: "black"
            opacity: 0
        }
    }

    Text {
        id: timeRemaining
        anchors {
            verticalCenter: timerSection.verticalCenter
            horizontalCenter: timerSection.horizontalCenter
        }
        opacity: 0
        text: i18n(" ")
        color: "white"
        font.pixelSize: parent.width / 32
    }

    Rectangle {
        id: hintSection
        width: parent.width / 3
        height: parent.height / 3
        opacity: 0
        color: "black"
        anchors {
            verticalCenter: blackboard.verticalCenter
            right: blackboard.left
            rightMargin: blackboard.width / 70
            leftMargin: blackboard.width / 70
        }
    }

   Grid {
        id: currentWordGrid;
        anchors {
            centerIn: parent;
        }
        spacing: 5;
        columns: 10;
        Repeater {
            model: kanagramGame.anagram
            LetterButton {
                text: modelData
                onClicked: letterButtonClicked( index );
            }
        }
    }

    Rectangle {
        id: wikiSection
        width: parent.width / 9
        height: parent.height / 7
        opacity: 0
        // FIXME: Set to visible false to avoid crash in WebView
        visible: false
        color: "black"
        anchors {
            verticalCenter: hintSection.verticalCenter
            right: hintSection.left
        }
    }

    Text {
        id: anagramHint
        anchors {
            verticalCenter: hintSection.verticalCenter
            horizontalCenter: hintSection.horizontalCenter
        }
        color: "white"
        width: hintSection.width
        wrapMode: Text.WordWrap
        text: kanagramGame.hint
        opacity: 0
        font.pixelSize: hintSection.width / 10
    }

    Image {
        id: wikiButton
        smooth: true
        height: wikiSection.height / 2
        anchors {
            verticalCenter: wikiSection.verticalCenter
            horizontalCenter: wikiSection.horizontalCenter
        }
        source: "icons/wikipedia.png"
        opacity: 0
        // FIXME: Set to visible false to avoid crash in WebView
        visible: false
        fillMode: Image.PreserveAspectFit
        property bool wikiLinkActivated: false

        MouseArea {
            anchors.fill: parent
            hoverEnabled: true
            onEntered: wikiButton.state = "onEntered"
            onExited: wikiButton.state = "onExited"
            onClicked: {
                wikiButton.wikiLinkActivated = true
                showWiki()
            }
        }

        states: State {
            name: "onEntered"
            PropertyChanges {
                target: wikiText
                opacity: 1
            }
        }
        State {
            name: "onExited"
        }

        transitions: Transition {
            PropertyAnimation {
                properties: "x,y,opacity"
                easing.type: Easing.Linear
                easing.amplitude: 5.0
                easing.period: 1
            }
        }
    }

    Text {
        id: wikiText
        anchors {
            bottom: hintSection.bottom
            horizontalCenter: hintSection.horizontalCenter
        }
        color: "white"
        text: i18n("Show Wiki Content")
        opacity: 0
        font.pixelSize: parent.width / 55
    }

    Rectangle {
        id: scoreSection
        width: blackboard.width / 5
        height: blackboard.height / 7
        anchors {
            bottom: blackboard.bottom
            bottomMargin: blackboard.height / 7
            right: blackboard.left
            rightMargin: blackboard.width / 68.5
        }
        color: "black"
        opacity: 0
    }

    Text {
        id: score
        anchors {
            verticalCenter: scoreSection.verticalCenter
            horizontalCenter: scoreSection.horizontalCenter
        }
        color: "white"
        text: kanagramGame.currentPlayer == 1 ? i18n("Score : ") + kanagramGame.score : i18n("Score : ") + kanagramGame.score2
        opacity: 0
        font.pixelSize: parent.width / 40
    }

    Timer {
        id: hintTimer
        interval: 1000
        repeat: true
        running: false
        triggeredOnStart: false

        onTriggered: {
            if (--hintButton.countDownTimerValue == 0) {
                hintSection.opacity = 0
                wikiSection.opacity = 0
                anagramHint.opacity = 0
                wikiText.opacity = 0
                wikiButton.opacity = 0
                stop()
            } else {
                hintSection.opacity = 0.35
                wikiSection.opacity = 0.35
                wikiButton.opacity = 1
                anagramHint.opacity = 1
            }
        }
    }

    Timer {
        id: scoreTimer
        interval: 1000
        repeat: true
        running: false
        triggeredOnStart: false

        onTriggered: {

            if (!wikiButton.wikiLinkActivated) {
                if (--scoreButton.countDownTimerValue == 0) {
                    stop()
                    timerSection.opacity = 0
                    timeRemaining.opacity = 0
                    scoreTimer.running = false

                    if(!(kanagramGame.singlePlayerMode())) {
                        if((kanagramGame.getPlayerNumber())==1) {
                            kanagramGame.setPlayerNumber(2);
                            score2Box.visible = true;
                            showTurnText.visible = true;
                            score2BoxText.visible = false;
                        }

                        else {
                            kanagramGame.setPlayerNumber(1);
                            score2Box.visible = true;
                            showTurnText.visible = false;
                            score2BoxText.visible = true;
                        }
                }

                } else {
                    scoreTimer.running = true
                    timerSection.opacity = 0.35
                    if (scoreButton.countDownTimerValue > 9) {
                        timeRemaining.text = '00:' + scoreButton.countDownTimerValue
                    } else {
                        timeRemaining.text = '00:0' + scoreButton.countDownTimerValue
                    }
                    timeRemaining.opacity = 1
                }
            }
        }
    }

    Item {
        id: score2Box
        anchors {
            top: blackboard.top
            topMargin: 150
            left: blackboard.left
            leftMargin: 130
        }
        visible: false

        Rectangle {
            id: score2Button
            width: blackboard.width / 2
            height: blackboard.height / 5
            MouseArea {
                anchors.fill: parent
            }
            radius: 8
            color: "black"
        }

        Text {
            id: score2BoxText
            anchors {
                verticalCenter: score2Button.verticalCenter
                horizontalCenter: score2Button.horizontalCenter
            }
            color: "white"
            text: i18n(" Player 1 : ") + kanagramGame.score + i18n("\n Player 2 : ") + kanagramGame.score2 + i18n("\n Close dialog for new game ")
        }

        Text {
            id: showTurnText
            anchors {
                verticalCenter: score2Button.verticalCenter
                horizontalCenter: score2Button.horizontalCenter
            }
            color: "white"
            text: i18n(" Time Up. \n Player 2's Turn")
        }

        ToolButton {
            id: closeButton
            action: closeAction
            width: score2Button.width / 6
            height: score2Button.height / 2
            anchors {
                top: score2Button.top
                right: score2Button.right
            }

            Image {
                source: "icons/close.png"
                anchors.fill: parent
                anchors.margins: 4
                fillMode: Image.PreserveAspectFit
            }
            background: Rectangle { color: "transparent" }
        }

        Action {
            id: closeAction
            onTriggered: {
                score2Box.visible = false;
            }
            text: i18n("Close")
        }
    }

    Timer {
        id: showAnswerTimer
        interval: 1000
        repeat: true
        running: false
        triggeredOnStart: false

        onTriggered: {
            if (--revealButton.countDownTimerValue == 0) {
                kanagramGame.nextAnagram();
                if (kanagramGame.hintHideTime())
                    blackboard.showHintTimeInterval = 1
                stop()
            }
        }
    }
}
