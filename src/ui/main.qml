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

import QtQuick
import QtQuick.Controls
import QtMultimedia

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

    Transition {
        id: easingTransition

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
        width: parent.width
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
            height: Math.round(parent.height * 0.8)
            width: headerArea.width - playericon.width - powerButton.width - 80
            anchors {
                verticalCenter: parent.verticalCenter
                horizontalCenter: parent.horizontalCenter
            }
            source: "images/header.png"
            smooth: true
            fillMode: Image.PreserveAspectFit
        }

        Image {
            id: playericon
            smooth: true
            height: parent.height / 1.5
            anchors {
                verticalCenter: parent.verticalCenter
                left: parent.left
                leftMargin: parent.width / 42
            }
            source: kanagramGame.singlePlayer ? "icons/1player.png" : "icons/2player.png"
            fillMode: Image.PreserveAspectFit

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
                topMargin: 12
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
            width: playericon.width + playericon.anchors.leftMargin
            wrapMode: Text.WordWrap
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
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
            width: blackboard.width / 2.5
            rightPadding: nextAnagramIcon.width * 1.5
            fontSizeMode: Text.HorizontalFit
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            font.pixelSize: screen.width / 68.5
        }

        Image {
            id: nextAnagramIcon
            smooth: true
            height: nextAnagramButton.height / 2
            anchors {
                verticalCenter: nextAnagramButton.verticalCenter
                horizontalCenter: nextAnagramButton.horizontalCenter
            }
            source: "icons/arrow-light.svgz"
            fillMode: Image.PreserveAspectFit
        }
    } // End of nextAnagram

    Rectangle {
        id: playerButton
        width: blackboard.width / 3
        height: blackboard.height / 7
        radius: 8
        color: "black"

        visible: !kanagramGame.singlePlayer

        anchors {
            top: blackboard.top
            right: blackboard.left
            topMargin: blackboard.height / 7
        }

        MouseArea {
            anchors.fill: parent
        }

        Text {
            id: currentPlayerText
            anchors {
                verticalCenter: playerButton.verticalCenter
                horizontalCenter: playerButton.horizontalCenter
            }
            color : "white"
            text: kanagramGame.currentPlayer == 1 ? i18n("1st Player") : i18n("2nd Player")
            font.pixelSize: screen.width / 68.5
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
            font.pixelSize: screen.width / 68.5
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

            MouseArea {
                anchors.fill: parent
                hoverEnabled: true
                onEntered: helpButton.state = "onEntered"
                onExited: helpButton.state = "onExited"
                onClicked: moreOptionsButton.visible = !moreOptionsButton.visible
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
            
        Rectangle {
            id: moreOptionsButton

            width: blackboard.width / 2.5
            height: blackboard.height / 7
            
            anchors {
                top: helpButton.bottom
                topMargin: blackboard.height / 50
                left: parent.left
            }
            
            visible: false

            radius: 8
            color: Qt.rgba(0, 0, 0, 0.5)  // Using rgba prevents opacity from being transferred to children
            
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
                font.pixelSize: screen.width / 91
            }
            
            Row {
                anchors.centerIn: moreOptionsButton
                spacing: moreOptionsButton.width / 8
                
                anchors {
                    horizontalCenter: moreOptionsButton.horizontalCenter
                    verticalCenter: moreOptionsButton.verticalCenter
                }

                IconButton {
                    id: kanagramHandbookIcon
                    source: "icons/kanagram-handbook.png"
                    iconContainer: moreOptionsButton
                    onClicked: mainwindow.showHandbook()
                    
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
                
                    transitions: easingTransition
                }

                IconButton {
                    id: aboutKanagramIcon
                    source: "icons/about-kanagram.png"
                    iconContainer: moreOptionsButton
                    onClicked: mainwindow.showAboutKanagram()
                    
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

                    transitions: easingTransition
                }
                
                IconButton {
                    id: aboutKdeIcon
                    source: "icons/about-kde.png"
                    iconContainer: moreOptionsButton
                    onClicked: mainwindow.showAboutKDE()
                    
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

                    transitions: easingTransition
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
                    if (countDownTimerValue == 0)
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
        audioOutput: AudioOutput {}
        source: "sounds/chalk.wav"
    }

    MediaPlayer {
        id: rightSound
        audioOutput: AudioOutput {}
        source: "sounds/right.wav"
    }

    MediaPlayer {
        id: wrongSound
        audioOutput: AudioOutput {}
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
        if (countDownTimerValue != 0)
            return;

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
