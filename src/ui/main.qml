/***************************************************************************
 *   This file is part of the Kanagram project                             *
 *   Copyright 2014 Debjit Mondal <debjitmondal05@gmail.com>               *
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

import QtQuick 2.3
import QtQuick.Controls 1.2
import QtMultimedia 5.0

Rectangle {

    id: screen
    focus: true
    property int countDownTimerValue: 0
    property bool flagCorrectAnswer: true

    function nextAnagram() {
        kanagramGame.nextAnagram()
        if (blackboard.activeTimer) {
            kanagramGame.answerSkipped();
        }
        if (kanagramGame.hintHideTime())
            blackboard.showHintTimeInterval = 1
        if (kanagramGame.useSounds)
            if (chalkSound.hasAudio)
                chalkSound.play();
            else
                console.log("unable to play chalk sound " + chalkSound.error);
    }

    function checkSolved() {
        if (kanagramGame.anagram.length == 0) {
            if (kanagramGame.checkWord()) {
                countDownTimerValue = 1;
                flagCorrectAnswer = true;
                if (kanagramGame.useSounds)
                    rightSound.play();
                if (blackboard.activeTimer) {
                    kanagramGame.answerCorrect();
                }
                resetTimer.repeat = true
                resetTimer.start()
            } else {
                countDownTimerValue = 1;
                flagCorrectAnswer = false;
                if (kanagramGame.useSounds)
                    wrongSound.play();
                if (blackboard.activeTimer) {
                    kanagramGame.answerIncorrect();
                }
                resetTimer.repeat = true
                resetTimer.start()
            }
        }
    }

    Image {
        id: background
        anchors.fill: parent
        source: "images/background.jpg"
        smooth: true
        fillMode: Image.PreserveAspectCrop
    }

    Item {
        id: headerArea
        width: parent.width;
        height: parent.height / 6
        anchors.top: screen.top

        Rectangle {
            id: toolBarTop
            anchors.fill: parent
            opacity: .25
            color: "black"
        }

        Image {
            id: header
            width: parent.width / 3.5
            height: parent.height / 1.5
            anchors {
                verticalCenter: parent.verticalCenter
                horizontalCenter: parent.horizontalCenter
            }
            source: "images/header.png"
            smooth: true
            fillMode: Image.PreserveAspectCrop
        }

        Image {
            id: playericon
            width: 60
            height: 60
            anchors.left:parent.left
            anchors.leftMargin:20
            anchors.verticalCenter: parent.verticalCenter
            source: kanagramGame.singlePlayer ? "icons/1player.png" : "icons/2player.png"
            smooth: true
            fillMode: Image.PreserveAspectCrop
            function togglePlayerMode() {
                kanagramGame.singlePlayer = !kanagramGame.singlePlayer;
            }

            MouseArea {
                anchors.fill: parent
                hoverEnabled: true
                onEntered: playericon.state = "onEntered"
                onExited: playericon.state = "onExited"
                onClicked: playericon.togglePlayerMode()
            }
            states: State {
                name: "onEntered"
                PropertyChanges {
                    target: playerModeText
                    opacity: 1
                }
                PropertyChanges {
                    target: playericon
                    source: kanagramGame.singlePlayer ? "icons/2player.png" : "icons/1player.png"
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

        Image {
            id: powerButton
            smooth: true
            height: parent.height / 3
            anchors {
                verticalCenter: parent.verticalCenter
                right: parent.right
                rightMargin: parent.width / 24
            }
            source: "icons/on-off-light.svgz"
            fillMode: Image.PreserveAspectFit

            MouseArea {
                anchors.fill: parent
                hoverEnabled: true
                onEntered: powerButton.state = "onEntered"
                onExited: powerButton.state = "onExited"
                onClicked: application.quit()
            }

            states: State {
                name: "onEntered"
                PropertyChanges {
                    target: quitText
                    opacity: 1
                }
            }
            State {
                name: "onExited"
            }

            transitions: Transition {
                //AnchorAnimation { duration: 200 }
                PropertyAnimation {
                    properties: "x,y,opacity"
                    easing.type: Easing.Linear
                    easing.amplitude: 5.0
                    easing.period: 1
                }
            }
        }

        Text {
            id: quitText
            anchors {
                top: powerButton.bottom
                topMargin: 10
                horizontalCenter: powerButton.horizontalCenter
            }
            color: "white"
            text: i18n("Quit")
            opacity: 0
            font.pixelSize: parent.width / 91
        }

        Text {
            id: playerModeText
            anchors {
                top: playericon.bottom
                topMargin: 2
                horizontalCenter: playericon.horizontalCenter
            }
            color: "white"
            text: i18n("Change mode")
            opacity: 0
            font.pixelSize: parent.width / 91
        }
    } // End of headerArea

    Item {
        id: nextAnagram
        anchors {
            top: blackboard.top
            left: blackboard.right
            topMargin: blackboard.height / 7
        }

        Action {
            id: nextAnagramAction
            onTriggered: screen.nextAnagram();
            shortcut: "Ctrl+N"
        }

        Rectangle {
            id: nextAnagramButton
            width: blackboard.width / 9
            height: blackboard.height / 7
            radius: 8
            color: "black"
            opacity: 0.5

            MouseArea {
                anchors.fill: parent
                hoverEnabled: true
                onEntered: nextAnagramButton.state = "onEntered"
                onExited: nextAnagramButton.state = "onExited"
                onClicked: screen.nextAnagram();
            }

            states: State {
                name: "onEntered"
                PropertyChanges {
                    target: nextAnagramButton
                    width: blackboard.width / 2.5
                }
                PropertyChanges {
                    target: nextAnagramText
                    opacity: 1
                }
                AnchorChanges {
                    target: nextAnagramIcon
                    anchors.horizontalCenter: undefined
                    anchors.right: nextAnagramButton.right
                }
                PropertyChanges {
                    target: nextAnagramIcon
                    anchors.rightMargin: blackboard.width / 50
                }
            }
            State {
                name: "onExited"
            }

            transitions: Transition {
                AnchorAnimation {
                    duration: 250
                }
                PropertyAnimation {
                    properties: "x,y,width,opacity"
                    easing.type: Easing.Linear
                    easing.amplitude: 5.0
                    easing.period: 1
                }
            }
        }

        Text {
            id: nextAnagramText
            anchors {
                verticalCenter: nextAnagramButton.verticalCenter
                horizontalCenter: nextAnagramButton.horizontalCenter
            }
            color: "white"
            text: i18n("Next Anagram")
            opacity: 0
            font.pixelSize: parent.width / 68.5
        }

        Image {
            id: nextAnagramIcon
            smooth: true
            height: nextAnagramButton.height / 2
            width: nextAnagramText.width / 4
            anchors {
                verticalCenter: nextAnagramButton.verticalCenter
                horizontalCenter: nextAnagramButton.horizontalCenter
            }
            source: "icons/arrow-light.svgz"
            fillMode: Image.PreserveAspectFit
        }
    } // End of nextAnagram

    Item {
        id: playerBox
        anchors {
            top: blackboard.top
            right: blackboard.left
            rightMargin: 185
            topMargin: blackboard.height / 7
        }

        visible: kanagramGame.singlePlayer ? false : true

        Rectangle {
            id: playerButton
            width: blackboard.width / 3
            height: blackboard.height / 7
            radius: 8
            color: "black"

            MouseArea {
                anchors.fill: parent
            }

            Text {
                id: currentPlayerText
                anchors{
                    verticalCenter: playerButton.verticalCenter
                    horizontalCenter: playerButton.horizontalCenter
                 }
                color : "white"
                text: kanagramGame.currentPlayer == 1 ? i18n("1st Player") : i18n("2nd Player")
            }
        }
    }

    Item {
        id: configure
        anchors {
            left: blackboard.right
            verticalCenter: blackboard.verticalCenter
        }

        Rectangle {
            id: configureButton
            width: blackboard.width / 9
            height: blackboard.height / 7
            anchors {
                left: parent.left
                verticalCenter: parent.verticalCenter
            }
            radius: 8
            color: "black"
            opacity: 0.5

            MouseArea {
                anchors.fill: parent
                hoverEnabled: true
                onEntered: configureButton.state = "onEntered"
                onExited: configureButton.state = "onExited"
                onClicked: {
                    mainwindow.showSettings()
                }
            }

            states: State {
                name: "onEntered"
                PropertyChanges {
                    target: configureButton
                    width: blackboard.width / 2.5
                }
                PropertyChanges {
                    target: configureText
                    opacity: 1
                }
                AnchorChanges {
                    target: configureIcon
                    anchors.horizontalCenter: undefined
                    anchors.right: configureButton.right
                }
                PropertyChanges {
                    target: configureIcon
                    anchors.rightMargin: blackboard.width / 50
                }
            }
            State {
                name: "onExited"
            }

            transitions: Transition {
                AnchorAnimation {
                    duration: 250
                }
                PropertyAnimation {
                    properties: "x,y,width,opacity"
                    easing.type: Easing.Linear
                    easing.amplitude: 5.0
                    easing.period: 1
                }
            }
        }

        Text {
            id: configureText
            anchors {
                verticalCenter: configureButton.verticalCenter
                horizontalCenter: configureButton.horizontalCenter
            }
            color: "white"
            text: i18n("Configure")
            opacity: 0
            font.pixelSize: parent.width / 68.5
        }

        Image {
            id: configureIcon
            smooth: true
            height: configureButton.height / 2
            width: configureText.width / 3
            anchors {
                verticalCenter: configureButton.verticalCenter
                horizontalCenter: configureButton.horizontalCenter
            }
            source: "icons/spanner-light.svgz"
            fillMode: Image.PreserveAspectFit
        }
    } // End of configure

    Item {
        id: help
        width: blackboard.width / 9
        height: blackboard.height / 7
        anchors {
            bottom: blackboard.bottom
            bottomMargin: blackboard.height / 7
            left: blackboard.right
        }

        Rectangle {
            id: helpButton
            width: blackboard.width / 9
            height: blackboard.height / 7
            radius: 8
            color: "black"
            opacity: 0.5

            property bool flag: true

            function toggleMoreOptions() {
                if (flag) {
                    moreOptionsButton.opacity = 0.5
                    moreOptionsButton.width = blackboard.width / 2.5
                    kanagramHandbookIcon.visible = true
                    aboutKdeIcon.visible = true
                    aboutKanagramIcon.visible = true
                    flag = false
                } else {
                    moreOptionsButton.opacity = 0
                    moreOptionsTextBar.opacity = 0
                    moreOptionsText.opacity = 0
                    kanagramHandbookIcon.visible = false
                    aboutKdeIcon.visible = false
                    aboutKanagramIcon.visible = false
                    flag = true
                }
            }

            MouseArea {
                anchors.fill: parent
                hoverEnabled: true
                onEntered: helpButton.state = "onEntered"
                onExited: helpButton.state = "onExited"
                onClicked: helpButton.toggleMoreOptions()
            }

            states: State {
                name: "onEntered"
                PropertyChanges {
                    target: helpButton
                    width: blackboard.width / 2.5
                }
                PropertyChanges {
                    target: helpText
                    opacity: 1
                }
                AnchorChanges {
                    target: helpIcon
                    anchors.horizontalCenter: undefined
                    anchors.right: helpButton.right
                }
                PropertyChanges {
                    target: helpIcon
                    anchors.rightMargin: blackboard.width / 50
                }
            }
            State {
                name: "onExited"
            }

            transitions: Transition {
                AnchorAnimation {
                    duration: 250
                }
                PropertyAnimation {
                    properties: "x,y,width,opacity"
                    easing.type: Easing.Linear
                    easing.amplitude: 5.0
                    easing.period: 1
                }
            }
        }

        Rectangle {
            id: moreOptionsButton
            width: blackboard.width / 9
            height: blackboard.height / 7
            anchors {
                top: helpButton.bottom
                topMargin: blackboard.height / 50
                left: parent.left
            }
            radius: 8
            color: "black"
            opacity: 0
        }

        Rectangle {
            id: moreOptionsTextBar
            width: blackboard.width / 4
            height: blackboard.height / 16
            anchors {
                top: moreOptionsButton.bottom
                horizontalCenter: moreOptionsButton.horizontalCenter
            }
            radius: 8
            color: "black"
            opacity: 0
        }

        Text {
            id: moreOptionsText
            anchors {
                verticalCenter: moreOptionsTextBar.verticalCenter
                horizontalCenter: moreOptionsTextBar.horizontalCenter
            }
            color: "white"
            text: i18n("  ")
            opacity: 0
            font.pixelSize: parent.width / 91
        }

        Text {
            id: helpText
            anchors {
                verticalCenter: helpButton.verticalCenter
                horizontalCenter: helpButton.horizontalCenter
            }
            color: "white"
            text: i18n("About")
            opacity: 0
            font.pixelSize: screen.width / 68.5
        }

        Image {
            id: helpIcon
            smooth: true
            height: blackboard.height / 14
            anchors {
                verticalCenter: helpButton.verticalCenter
                horizontalCenter: helpButton.horizontalCenter
            }
            source: "icons/question-light.svgz"
            visible: true
            fillMode: Image.PreserveAspectFit
        }

        Image {
            id: aboutKdeIcon
            smooth: true
            height: blackboard.height / 14
            anchors {
                verticalCenter: moreOptionsButton.verticalCenter
                right: moreOptionsButton.right
                rightMargin: moreOptionsButton.width / 8
            }
            source: "icons/about-kde.png"
            visible: false
            fillMode: Image.PreserveAspectFit

            MouseArea {
                anchors.fill: parent
                hoverEnabled: true
                onEntered: aboutKdeIcon.state = "onEntered"
                onExited: aboutKdeIcon.state = "onExited"
                onClicked: mainwindow.showAboutKDE()
                preventStealing: true
            }

            states: State {
                name: "onEntered"
                PropertyChanges {
                    target: moreOptionsTextBar
                    opacity: 0.5
                }
                PropertyChanges {
                    target: moreOptionsText
                    text: i18n("About KDE")
                    opacity: 1
                }
            }
            State {
                name: "onExited"
            }

            transitions: Transition {
                AnchorAnimation {
                    duration: 250
                }
                PropertyAnimation {
                    properties: "x,y,width,opacity"
                    easing.type: Easing.Linear
                    easing.amplitude: 5.0
                    easing.period: 1
                }
            }
        }

        Image {
            id: aboutKanagramIcon
            height: blackboard.height / 14
            anchors {
                verticalCenter: moreOptionsButton.verticalCenter
                horizontalCenter: moreOptionsButton.horizontalCenter
            }
            source: "icons/about-kanagram.png"
            smooth: true
            visible: false
            fillMode: Image.PreserveAspectFit

            MouseArea {
                anchors.fill: parent
                hoverEnabled: true
                onEntered: aboutKanagramIcon.state = "onEntered"
                onExited: aboutKanagramIcon.state = "onExited"
                onClicked: mainwindow.showAboutKanagram()
                preventStealing: true
            }

            states: State {
                name: "onEntered"
                PropertyChanges {
                    target: moreOptionsTextBar
                    opacity: 0.5
                }
                PropertyChanges {
                    target: moreOptionsText
                    text: i18n("About Kanagram")
                    opacity: 1
                }
            }
            State {
                name: "onExited"
            }

            transitions: Transition {
                AnchorAnimation {
                    duration: 250
                }
                PropertyAnimation {
                    properties: "x,y,width,opacity"
                    easing.type: Easing.Linear
                    easing.amplitude: 5.0
                    easing.period: 1
                }
            }
        }

        Image {
            id: kanagramHandbookIcon
            smooth: true
            height: blackboard.height / 14
            anchors {
                verticalCenter: moreOptionsButton.verticalCenter
                left: moreOptionsButton.left
                leftMargin: moreOptionsButton.width / 8
            }
            source: "icons/kanagram-handbook.png"
            visible: false
            fillMode: Image.PreserveAspectFit

            MouseArea {
                anchors.fill: parent
                hoverEnabled: true
                onEntered: kanagramHandbookIcon.state = "onEntered"
                onExited: kanagramHandbookIcon.state = "onExited"
                onClicked: mainwindow.showHandbook()
                preventStealing: true
            }

            states: State {
                name: "onEntered"
                PropertyChanges {
                    target: moreOptionsTextBar
                    opacity: 0.5
                }
                PropertyChanges {
                    target: moreOptionsText
                    text: i18n("Kanagram Handbook")
                    opacity: 1
                }
            }
            State {
                name: "onExited"
            }

            transitions: Transition {
                AnchorAnimation {
                    duration: 250
                }
                PropertyAnimation {
                    properties: "x,y,width,opacity"
                    easing.type: Easing.Linear
                    easing.amplitude: 5.0
                    easing.period: 1
                }
            }
        }
    } // End help

    Rectangle {
        id: inputField
        width: blackboard.width
        height: parent.height / 10
        anchors {
            horizontalCenter: blackboard.horizontalCenter
            bottom: parent.bottom
            bottomMargin: parent.height / 35
        }
        radius: 8
        border.color: "white"
        border.width: 0
        color: "black"
        opacity: 0.35
    }

    Grid {
        id: answerGrid
        anchors {
            centerIn: inputField
        }
        spacing: 5
        columns: 13
        Repeater {
            model: kanagramGame.userAnswer
            LetterButton {
                text: modelData
                onClicked: {
                    kanagramGame.moveLetterToAnagram( index )
                }
            }
        }
    }

    Timer {
        id: resetTimer
        interval: 1000
        repeat: true
        running: false
        triggeredOnStart: false

        onTriggered: {
            if (--countDownTimerValue == 0) {
                if (flagCorrectAnswer) {
                    kanagramGame.nextAnagram();
                    if (kanagramGame.hintHideTime())
                        blackboard.showHintTimeInterval = 1
                } else {
                    kanagramGame.resetAnagram();
                }
                stop()
            }
        }
    }

    MediaPlayer {
        id: chalkSound
        source: "sounds/chalk.wav"
    }

    MediaPlayer {
        id: rightSound
        source: "sounds/right.wav"
    }

    MediaPlayer {
        id: wrongSound
        source: "sounds/wrong.wav"
    }

    Blackboard {
        id: blackboard
        border {
            width: parent.width / 68.5
            color: "#613529"
        }
        width: parent.width / 2
        height: parent.height / 1.5
        anchors {
            horizontalCenter: parent.horizontalCenter
            top: headerArea.bottom
        }
        onNextAnagram: {
            screen.nextAnagram();
        }
    }

    Keys.onPressed: {
        if (event.text.length > 0) {
            kanagramGame.moveLetter(event.text);
            checkSolved();
        }
        if (event.key == Qt.Key_Backspace)
        {
            var length = kanagramGame.userAnswer.length;
            if (length > 0)
                kanagramGame.moveLetterToAnagram(length - 1);
        }
    }
}
