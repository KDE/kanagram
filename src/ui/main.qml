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

    function nextAnagram() {
        kanagramGame.nextAnagram()
        if (blackboard.activeTimer) {
            kanagramGame.answerSkipped();
        }
        if (kanagramGame.hintHideTime())
            blackboard.showHintTimeInterval = 1
        if (kanagramGame.useSounds)
            chalkSound.play();
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
            source: kanagramGame.singlePlayerMode() ? "icons/1player.png" : "icons/2player.png"
            smooth: true
            fillMode: Image.PreserveAspectCrop
            function togglePlayerMode() {
	    if (kanagramGame.singlePlayerMode()){
		source= "icons/2player.png"
		kanagramGame.setSinglePlayerMode(false)
	    }
	    else{
		source= "icons/1player.png"
		kanagramGame.setSinglePlayerMode(true)
	        }
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
	    left: parent.left
	    leftMargin: 20
	    verticalCenter: blackboard.verticalCenter
	}
	visible: !kanagramGame.singlePlayer
	
	Rectangle {
            id: playerButton
            width: blackboard.width / 3
            height: blackboard.height / 7
            anchors {
                left: parent.left
                verticalCenter: parent.verticalCenter
            }
            radius: 8
            color: "black"
            opacity: 0.5
	}
	
	Text { 
	id: currentPlayerText
	anchors {
                verticalCenter: playerButton.verticalCenter
                horizontalCenter: playerButton.horizontalCenter
            }
	color : "white"
	text: kanagramGame.currentPlayer == 1 ? i18n("1st Player") : i18n("2nd Player")
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

    TextInput {
        id: input
        color: "white"
        selectionColor: "white"
        selectedTextColor: "black"
        font.pixelSize: parent.width / 40
        font.bold: true
        width: inputField.width
        anchors.centerIn: inputField
        focus: true
        property int countDownTimerValue: 0
        property bool flagCorrectAnswer: true
        onAccepted: {
            if (kanagramGame.checkWord(text)) {
                input.color = "green"
                input.countDownTimerValue = 1
                input.flagCorrectAnswer = true
                if (kanagramGame.useSounds)
                    rightSound.play();
                if (blackboard.activeTimer) {
                    kanagramGame.answerCorrect();
                }
                showAnswerTimer.repeat = true
                showAnswerTimer.start()
            } else {
                input.color = "red"
                input.countDownTimerValue = 1
                input.flagCorrectAnswer = false
                if (kanagramGame.useSounds)
                    wrongSound.play();
                if (blackboard.activeTimer) {
                    kanagramGame.answerIncorrect();
                }
                showAnswerTimer.repeat = true
                showAnswerTimer.start()
            }
        }
    }

    Timer {
        id: showAnswerTimer
        interval: 1000
        repeat: true
        running: false
        triggeredOnStart: false

        onTriggered: {
            if (--input.countDownTimerValue == 0) {
                input.color = "white"
		if(!(kanagramGame.singlePlayerMode())) {
		    if((kanagramGame.getPlayerNumber())==1)
			kanagramGame.setPlayerNumber(2);
		    else
			kanagramGame.setPlayerNumber(1);
		}
                if (input.flagCorrectAnswer) {
                    kanagramGame.nextAnagram();
                    if (kanagramGame.hintHideTime())
                        blackboard.showHintTimeInterval = 1
                }
		input.text = ""
                stop()
            }
        }
    }

    MediaPlayer {
        id: chalkSound
        source: "sounds/chalk.ogg"
    }

    MediaPlayer {
        id: rightSound
        source: "sounds/right.ogg"
    }

    MediaPlayer {
        id: wrongSound
        source: "sounds/wrong.ogg"
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
        onShowWiki: {
            wikiPage.url = wikiPage.url = "http://en.wikipedia.org/wiki/"
                    + kanagramGame.word
            wikiPage.visible = true
        }
        onNextAnagram: {
            screen.nextAnagram();
        }
    }
//    WikiPage {
//        id: wikiPage
//        visible: false
//        onClosed: {
//            blackboard.wikiClosed()
//        }
//    }
}
