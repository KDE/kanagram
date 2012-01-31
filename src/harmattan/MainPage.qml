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

Page {

    orientationLock: PageOrientation.LockLandscape;

    property variant anagram: kanagramEngineHelper.createNextAnagram();
    property int anagramStatus: anagramStatusEnumeration.init;
    property int currentOriginalWordIndex: 0;
    property color originalWordLetterRectangleColor: Qt.rgba(0, 0, 0, 0);

    QtObject {  // status enum hackery :)
      id: anagramStatusEnumeration;
      property int init: 1;
      property int active: 2;
      property int resolved: 3;
    }

    onStatusChanged: {
        if (status == PageStatus.Active) {
            resolveTimer.start();
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
        anagramStatus = anagramStatusEnumeration.resolved;
        originalWordLetterRepeater.model = kanagramEngineHelper.anagramOriginalWord();
        currentOriginalWordIndex = originalWordLetterRepeater.model.length;
        anagramHintInfoBanner.hide();
    }

    function nextAnagram() {
        anagramHintInfoBanner.hide();
        anagramStatus = anagramStatusEnumeration.init;
        anagram = kanagramEngineHelper.createNextAnagram();
        anagramLetterRepeater.model = anagram;
        originalWordLetterRepeater.model = anagram;
        currentOriginalWordIndex = 0;
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
            iconSource: "games-hint.png";

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
            }
        }

        ToolIcon {
            iconId: "toolbar-tab-next";

            onClicked: {
                if (kanagramEngineHelper.useSounds) {
                    chalkSoundEffect.play();
                }

                nextAnagram();
            }
        }

        ToolIcon {
            iconId: "toolbar-settings";

            onClicked: {
                anagramHintInfoBanner.hide();
                pageStack.push(mainSettingsPage);
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
            kanagramGame.useVocabulary(selectedIndex);
            anagramHintInfoBanner.hide();

            if (kanagramEngineHelper.useSounds) {
                chalkSoundEffect.play();
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
        }
    }

    Timer {
        id: resolveTimer;
        interval: kanagramEngineHelper.resolveTime * 1000;
        repeat: false;
        running: false;
        triggeredOnStart: false;

        onTriggered: {
             resolveAnagram();
        }
    }

    Column {
        anchors {
            horizontalCenter: parent.horizontalCenter;
            verticalCenter: parent.verticalCenter;
        }

        spacing: 20;
        Row {
            id: anagramRow;
            anchors {
                horizontalCenter: parent.horizontalCenter;
            }

            spacing: 10;
            Repeater {
                id: anagramLetterRepeater;
                model: anagram;
                Rectangle {
                    id: anagramLetterRectangle;
                    color: Qt.rgba(0, 0, 0, 0);
                    Text {
                        id: anagramLetterText;
                        text: modelData;
                        color: "white";

                        font {
                            pixelSize: 40;
                            bold: true;
                            capitalization: Font.AllUppercase;
                        }

                        anchors {
                            horizontalCenter: parent.horizontalCenter;
                            verticalCenter: parent.verticalCenter;
                        }
                    }

                    width: 48;
                    height: 48;
                    border.color: "white";
                    border.width: 2;

                    MouseArea {
                        anchors.fill: parent;
                        hoverEnabled: true;

                        // drag.target: anagramLetterRectangle;
                        // drag.axis: Drag.XandYAxis;
                        // drag.minimumX: 0;
                        // drag.maximumX: mainPage.width - anagramLetterRectangle.width;
                        // drag.minimumY: 0;

                        onClicked: {
                            if (anagramStatus != anagramStatusEnumeration.resolved)
                            {
                                if (anagramLetterText.text != "")
                                {
                                    anagramStatus = anagramStatusEnumeration.active;

                                    originalWordLetterRepeater.model =
                                        kanagramEngineHelper.insertInCurrentOriginalWord(currentOriginalWordIndex, anagramLetterText.text);

                                    ++currentOriginalWordIndex;
                                    anagramLetterText.text = "";
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

        Button {
            text: categorySelectionDialog.model[categorySelectionDialog.selectedIndex];

            anchors {
                horizontalCenter: parent.horizontalCenter;
            }

            onClicked: {
                categorySelectionDialog.open();
            }
        }

        Row {
            id: originalWordRow;
            anchors {
                horizontalCenter: parent.horizontalCenter;
            }

            spacing: 10;
            Repeater {
                id: originalWordLetterRepeater;
                model: anagram;
                Rectangle {
                    id: originalWordLetterRectangle;
                    color: originalWordLetterRectangleColor;
                    Text {
                        text: anagramStatus == anagramStatusEnumeration.init ? "" : modelData;
                        color: "white";

                        font {
                            pixelSize: 40;
                            bold: true;
                            capitalization: Font.AllUppercase;
                        }

                        anchors {
                            horizontalCenter: parent.horizontalCenter;
                            verticalCenter: parent.verticalCenter;
                        }
                    }

                    width: 48;
                    height: 48;
                    border.color: "white";
                    border.width: 2;

                    MouseArea {
                        anchors.fill: parent;
                        hoverEnabled: true;

                        onClicked: {
                       }
                    }
                }
            }
        }
    }
}
