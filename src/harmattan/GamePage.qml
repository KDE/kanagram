/******************************************************************************
 * This file is part of the Kanagram project
 * Copyright (C) 2012 Laszlo Papp <lpapp@kde.org>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

import QtQuick 1.1
import com.nokia.meego 1.0
import com.nokia.extras 1.0
import QtMultimediaKit 1.1

import "array.js" as MyArray

Page {

    orientationLock: PageOrientation.LockLandscape;

    property variant anagram: kanagramEngineHelper.createNextAnagram();
    property int anagramStatus: anagramStatusEnumeration.init;
    property int currentOriginalWordIndex: 0;
    property color originalWordLetterRectangleColor: Qt.rgba(0, 0, 0, 0);
    property int countDownTimerValue: kanagramEngineHelper.resolveTime;

    QtObject {  // status enum hackery :)
      id: anagramStatusEnumeration;
      property int init: 1;
      property int active: 2;
      property int resolved: 3;
    }

    onStatusChanged: {
        if (status == PageStatus.Active) {
            secondTimer.repeat = true;
            secondTimer.restart();
        }
    }

    Connections {
        target: platformWindow;

        onActiveChanged: {
            if (platformWindow.active) {
                secondTimer.repeat = true;
                secondTimer.restart();
            } else {
                anagramHintInfoBanner.hide();

                secondTimer.repeat = false;
                secondTimer.stop();
            }
        }
    }

    function pushPage(file) {
        var component = Qt.createComponent(file)
        if (component.status == Component.Ready)
            pageStack.push(component);
        else
            console.log("Error loading component:", component.errorString());
    }

    function resolveAnagram() {
        originalWordLetterRepeater.model = kanagramEngineHelper.anagramOriginalWord();
        currentOriginalWordIndex = originalWordLetterRepeater.model.length;
        anagramStatus = anagramStatusEnumeration.resolved;
        anagramHintInfoBanner.hide();
    }

    function nextAnagram() {
        anagramHintInfoBanner.hide();
        anagramStatus = anagramStatusEnumeration.init;
        anagram = kanagramEngineHelper.createNextAnagram();
        anagramLetterRepeater.model = anagram;
        originalWordLetterRepeater.model = anagram;
        currentOriginalWordIndex = 0;
        countDownTimerValue = kanagramEngineHelper.resolveTime;
        MyArray.sourceDestinationLetterIndexHash = [];
    }

    // Create an info banner with icon
    InfoBanner {
        id: anagramHintInfoBanner;
        text: qsTr("This is an info banner with icon");
        iconSource: "dialog-information.png";
    }

    SoundEffect {
        id: chalkSoundEffect;
        source: "chalk.wav";
    }

    SoundEffect {
        id: rightSoundEffect;
        source: "right.wav";
    }

    SoundEffect {
        id: wrongSoundEffect;
        source: "wrong.wav";
    }

    // These tools are available for the main page by assigning the
    // id to the main page's tools property
    ToolBarLayout {
        id: mainPageTools;
        visible: false;

        ToolIcon {
            iconSource: "help-hint.png";

            onClicked: {
                anagramHintInfoBanner.text = kanagramGame.hint();
                anagramHintInfoBanner.timerShowTime = kanagramEngineHelper.hintHideTime * 1000;

                // Display the info banner
                anagramHintInfoBanner.show();
            }
        }

        ToolIcon {
            iconSource: "games-solve.png";

            onClicked: {
                resolveAnagram();

                secondTimer.repeat = false;
                secondTimer.stop();
            }
        }

        Button {
            text: categorySelectionDialog.model[categorySelectionDialog.selectedIndex];

            anchors {
                horizontalCenter: parent.horizontalCenter;
            }

            onClicked: {
                categorySelectionDialog.open();
            }
        }

        ToolIcon {
            iconSource: "go-next.png";

            onClicked: {
                if (kanagramEngineHelper.useSounds) {
                    chalkSoundEffect.play();
                }

                nextAnagram();
                secondTimer.repeat = true;
                secondTimer.restart();
            }
        }

        ToolIcon {
            iconSource: "settings.png";

            onClicked: {
                anagramHintInfoBanner.hide();
                pageStack.push(mainSettingsPage);

                secondTimer.repeat = false;
                secondTimer.stop();
            }
        }
    }

    tools: mainPageTools;

    // Create a selection dialog with the vocabulary titles to choose from.
    MySelectionDialog {
        id: categorySelectionDialog;
        titleText: "Choose an anagram category"
        selectedIndex: 1;

        model: kanagramGame.vocabularyList();

        onSelectedIndexChanged: {

            if (kanagramEngineHelper.useSounds) {
                chalkSoundEffect.play();
            }

            kanagramGame.useVocabulary(selectedIndex);
            nextAnagram();
        }
    }

    Timer {
        id: secondTimer;
        interval: 1000;
        repeat: true;
        running: false;
        triggeredOnStart: false;

        onTriggered: {
             if (kanagramEngineHelper.resolveTime != 0 && --countDownTimerValue == 0) {
                 stop();
                 anagramResultTimer.start();
                 originalWordLetterRectangleColor = "red";

                 resolveAnagram();

                 if (kanagramEngineHelper.useSounds) {
                     wrongSoundEffect.play();
                 }
             }
        }
    }

    Timer {
        id: anagramResultTimer;
        interval: 1000;
        repeat: false;
        running: false;
        triggeredOnStart: false;

        onTriggered: {
            originalWordLetterRectangleColor = Qt.rgba(0, 0, 0, 0);
            nextAnagram();

            secondTimer.repeat = true;
            secondTimer.start();
        }
    }

    Row {
        spacing: 5;

        anchors {
            right: parent.right;
            top: parent.top;
            topMargin: 5;
            rightMargin: 5;
        }

        LetterElement {
            letterText: Math.floor(countDownTimerValue / 60 / 10);
            visible: kanagramEngineHelper.resolveTime == 0 ? false : true;
        }

        LetterElement {
            letterText: Math.floor(countDownTimerValue / 60 % 10);
            visible: kanagramEngineHelper.resolveTime == 0 ? false : true;
        }

        LetterElement {
            letterText: ":";
            visible: kanagramEngineHelper.resolveTime == 0 ? false : true;
        }

        LetterElement {
            letterText: Math.floor(countDownTimerValue % 60 / 10);
            visible: kanagramEngineHelper.resolveTime == 0 ? false : true;
        }

        LetterElement {
            letterText: Math.floor(countDownTimerValue % 60 % 10);
            visible: kanagramEngineHelper.resolveTime == 0 ? false : true;
        }
    }

    Column {
        anchors {
            horizontalCenter: parent.horizontalCenter;
            verticalCenter: parent.verticalCenter;
        }

        spacing: 20;

        Row {
            id: originalWordRow;
            anchors {
                horizontalCenter: parent.horizontalCenter;
            }

            spacing: 10;
            Repeater {
                id: originalWordLetterRepeater;
                model: anagram;
                LetterElement {
                    id: originalWordLetterId;
                    color: originalWordLetterRectangleColor;
                    letterText: anagramStatus == anagramStatusEnumeration.init ? "" : modelData;

                    MouseArea {
                        anchors.fill: parent;
                        hoverEnabled: true;

                        onClicked: {
                            if (index + 1 == currentOriginalWordIndex && currentOriginalWordIndex != 0) {

                                var tmpAnagramLetterRepeaterModel = anagramLetterRepeater.model;
                                tmpAnagramLetterRepeaterModel[MyArray.sourceDestinationLetterIndexHash[index]] = originalWordLetterId.letterText;
                                anagramLetterRepeater.model = tmpAnagramLetterRepeaterModel;

                                MyArray.sourceDestinationLetterIndexHash.pop();

                                originalWordLetterRepeater.model = kanagramEngineHelper.removeInCurrentOriginalWord(index);
                                --currentOriginalWordIndex;
                            }
                        }
                    }
                }
            }
        }

        Row {
            id: anagramRow;
            anchors {
                horizontalCenter: parent.horizontalCenter;
            }

            spacing: 10;
            Repeater {
                id: anagramLetterRepeater;
                model: anagram;
                LetterElement {
                    id: anagramLetterId;
                    letterText: modelData;

                    MouseArea {
                        anchors.fill: parent;
                        hoverEnabled: true;

                        // drag.target: parent;
                        // drag.axis: Drag.XandYAxis;
                        // drag.minimumX: 0;
                        // drag.maximumX: mainPage.width - parent.width;
                        // drag.minimumY: 0;

                        onClicked: {
                            if (anagramStatus != anagramStatusEnumeration.resolved)
                            {
                                if (anagramLetterId.letterText != "")
                                {
                                    anagramStatus = anagramStatusEnumeration.active;

                                    originalWordLetterRepeater.model =
                                        kanagramEngineHelper.insertInCurrentOriginalWord(currentOriginalWordIndex, anagramLetterId.letterText);

                                    ++currentOriginalWordIndex;

                                    var tmpAnagramLetterRepeaterModel = anagramLetterRepeater.model;
                                    tmpAnagramLetterRepeaterModel[[index]] = "";
                                    anagramLetterRepeater.model = tmpAnagramLetterRepeaterModel;

                                    MyArray.sourceDestinationLetterIndexHash.push(index);
                                }

                                if (currentOriginalWordIndex == originalWordLetterRepeater.model.length)
                                {
                                    anagramResultTimer.start();
                                    anagramStatus = anagramStatusEnumeration.resolved;
                                    anagramHintInfoBanner.hide();
                                    if (kanagramEngineHelper.compareWords() == true)
                                    {
                                        originalWordLetterRectangleColor = "green";

                                        if (kanagramEngineHelper.useSounds) {
                                            rightSoundEffect.play();
                                        }
                                    }
                                    else
                                    {
                                        originalWordLetterRectangleColor = "red";

                                        if (kanagramEngineHelper.useSounds) {
                                            wrongSoundEffect.play();
                                        }
                                    }
                                }
                            }
                       }
                    }
                }
            }
        }
    }
}
