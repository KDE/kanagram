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

import "array.js" as MyArray

Page {

    orientationLock: PageOrientation.LockLandscape;

    property variant anagram: kanagramEngineHelper.createNextAnagram();
    property int anagramStatus: anagramStatusEnumeration.init;
    property int currentOriginalWordIndex: 0;
    property string originalWordLetterButtonBackground: "image://theme/meegotouch-button-inverted-background";
    property int countDownTimerValue: kanagramEngineHelper.resolveTime;
    property bool initialized: false;

    QtObject {  // status enum hackery :)
        id: anagramStatusEnumeration;
        property int init: 1;
        property int active: 2;
        property int resolved: 3;
    }

    onStatusChanged: {
        if (status == PageStatus.Active && anagramStatus != anagramStatusEnumeration.resolved) {
            if (rootWindow.languageSelectionChanged == true) {

                if (kanagramEngineHelper.useSounds) {
                    chalkSoundEffect.play();
                }

                categorySelectionDialog.model = kanagramGame.vocabularyList();

                // Sanitize the selected index, if the translation is not
                // available for the previously selected index in a different
                // language.

                if (categorySelectionDialog.selectedIndex >= categorySelectionDialog.model.length) {
                    categorySelectionDialog.selectedIndex = 1;
                }

                kanagramGame.setCurrentCategory(categorySelectionDialog.selectedIndex);
                kanagramEngineHelper.saveSettings();

                kanagramGame.useVocabulary(categorySelectionDialog.selectedIndex);
                nextAnagram();
                rootWindow.languageSelectionChanged = false;
            }

            secondTimer.repeat = true;
            secondTimer.restart();
        }
    }

    Connections {
        target: platformWindow;

        onActiveChanged: {
            if (platformWindow.active && status == PageStatus.Active
                    && anagramStatus != anagramStatusEnumeration.resolved) {
                secondTimer.repeat = true;
                secondTimer.restart();
            } else {
                anagramHintInfoBanner.hide();

                secondTimer.repeat = false;
                secondTimer.stop();
            }
        }
    }

    Component.onCompleted: {
        categorySelectionDialog.selectedIndex = kanagramGame.currentCategory();
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
        text: i18n("This is an info banner with icon");
        iconSource: "dialog-information.png";
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
                if (kanagramEngineHelper.useSounds) {
                    chalkSoundEffect.play();
                }

                resolveAnagram();

                secondTimer.repeat = false;
                secondTimer.stop();
            }
        }

        ToolButton {
            text: categorySelectionDialog.model[categorySelectionDialog.selectedIndex];

            anchors {
                centerIn: parent;
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

        model: kanagramGame.vocabularyList();

        onSelectedIndexChanged: {

            if (kanagramEngineHelper.useSounds) {
                initialized == true ? chalkSoundEffect.play() : initialized = true;
            }

            kanagramGame.setCurrentCategory(selectedIndex);
            kanagramEngineHelper.saveSettings();
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
                 originalWordLetterButtonBackground =
                     "image://theme/meegotouch-button-negative-background";

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
            originalWordLetterButtonBackground =
                "image://theme/meegotouch-button-inverted-background";
            nextAnagram();

            secondTimer.repeat = true;
            secondTimer.start();
        }
    }

    Row {
        spacing: 0;

        anchors {
            right: parent.right;
            top: parent.top;
            topMargin: 15;
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

        ToolIcon {
            iconSource: "timer-pause.png";

            onClicked: {
                anagramHintInfoBanner.hide();

                pageStack.pop();

                secondTimer.repeat = false;
                secondTimer.stop();

            }
        }
    }

    Column {
        anchors {
            centerIn: parent;
            verticalCenterOffset: 15;
        }

        spacing: 30;

        Row {
            id: originalWordRow;
            anchors {
                horizontalCenter: parent.horizontalCenter;
            }

            spacing: 8;
            Repeater {
                id: originalWordLetterRepeater;
                model: anagram;
                Button {
                    id: originalWordLetterId;
                    text: anagramStatus == anagramStatusEnumeration.init ? "" : modelData;

                    platformStyle: ButtonStyle {
                        background: originalWordLetterButtonBackground;
                        fontFamily: "Arial";
                        fontPixelSize: 40;
                        fontCapitalization: Font.AllUppercase;
                        fontWeight: Font.Bold;
                        horizontalAlignment: Text.AlignHCenter;
                        textColor: "white";
                        pressedTextColor: "pink";
                        disabledTextColor: "gray";
                        checkedTextColor: "blue";
                        buttonWidth: 45;
                        buttonHeight: 60;
                    }

                    onClicked: {
                        if (index + 1 == currentOriginalWordIndex && currentOriginalWordIndex != 0
                                && anagramStatus != anagramStatusEnumeration.resolved) {
                            if (kanagramEngineHelper.useSounds) {
                                anagramLetterPressSoundEffect.play();
                            }

                            var tmpAnagramLetterRepeaterModel = anagramLetterRepeater.model;
                            tmpAnagramLetterRepeaterModel[MyArray.sourceDestinationLetterIndexHash[index]] = originalWordLetterId.text;
                            anagramLetterRepeater.model = tmpAnagramLetterRepeaterModel;

                            MyArray.sourceDestinationLetterIndexHash.pop();

                            originalWordLetterRepeater.model = kanagramEngineHelper.removeInCurrentOriginalWord(index);
                            --currentOriginalWordIndex;
                        }
                    }
                }
            }
        }

        Image {
            id: chalkSeparator;
            width: parent.width;
            height: 5;
            fillMode: Image.PreserveAspectFit;
            source: "chalk-separator.png";
        }

        Row {
            id: anagramRow;
            anchors {
                horizontalCenter: parent.horizontalCenter;
            }

            spacing: 8;
            Repeater {
                id: anagramLetterRepeater;
                model: anagram;
                Button {
                    id: anagramLetterId;
                    text: anagramStatus == anagramStatusEnumeration.resolved ? "" : modelData;

                    platformStyle: ButtonStyle {
                        background: "image://theme/meegotouch-button-inverted-background";
                        fontFamily: "Arial";
                        fontPixelSize: 40;
                        fontCapitalization: Font.AllUppercase;
                        fontWeight: Font.Bold;
                        horizontalAlignment: Text.AlignHCenter;
                        textColor: "white";
                        pressedTextColor: "pink";
                        disabledTextColor: "gray";
                        checkedTextColor: "blue";
                        buttonWidth: 45;
                        buttonHeight: 60;
                    }

                    onClicked: {
                        if (anagramStatus != anagramStatusEnumeration.resolved)
                        {
                            if (anagramLetterId.text != "")
                            {
                                if (kanagramEngineHelper.useSounds) {
                                    anagramLetterPressSoundEffect.play();
                                }

                                anagramStatus = anagramStatusEnumeration.active;

                                originalWordLetterRepeater.model =
                                    kanagramEngineHelper.insertInCurrentOriginalWord(currentOriginalWordIndex, anagramLetterId.text);

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
                                    originalWordLetterButtonBackground =
                                        "image://theme/meegotouch-button-positive-background";

                                    if (kanagramEngineHelper.useSounds) {
                                        rightSoundEffect.play();
                                    }
                                }
                                else
                                {
                                    originalWordLetterButtonBackground =
                                        "image://theme/meegotouch-button-negative-background";

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
