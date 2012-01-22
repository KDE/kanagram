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

Page {
    id: mainPage;
    property variant anagram: kanagramEngineHelper.createNextAnagram();
    property int currentOriginalWordIndex: 0;

    function pushPage(file) {
        var component = Qt.createComponent(file)
        if (component.status == Component.Ready)
            pageStack.push(component);
        else
            console.log("Error loading component:", component.errorString());
    }

    // Create an info banner with icon
    InfoBanner {
        id: anagramHint;
        text: "This is an info banner with icon"
        iconSource: "dialog-information.png"
    }

    // These tools are available for the main page by assigning the
    // id to the main page's tools property
    ToolBarLayout {
        id: mainPageTools;
        visible: false;

        Image {
            source: "games-hint.png";
            width: 32;
            height: 32;
            anchors.verticalCenter: parent.verticalCenter;

            MouseArea {
                anchors.fill: parent;
                hoverEnabled: true;

                onClicked: {
                    anagramHint.text = kanagramGame.hint();

                    // Set the display time to 5000 ms (default is 3000 ms)
                    anagramHint.timerShowTime = 5000;

                    // Display the info banner
                    anagramHint.show();
                }
            }
        }

        Image {
            source: "games-solve.png";
            width: 32;
            height: 32;
            anchors.verticalCenter: parent.verticalCenter;

            MouseArea {
                anchors.fill: parent;
                hoverEnabled: true;

                onClicked: {
                    originalWordLetterRepeater.model = kanagramEngineHelper.anagramOriginalWord();
                    anagramLetterRepeater.model = "";
                }
            }
        }

        ToolIcon {
            iconId: "toolbar-tab-next";

            onClicked: {
                anagram = kanagramEngineHelper.createNextAnagram();
                anagramLetterRepeater.model = anagram;
                originalWordLetterRepeater.model = anagram;
            }
        }

        ToolIcon {
            iconId: "toolbar-settings";

            onClicked: {
                pageStack.push("qrc:/MainSettingsPage.qml");
            }
        }
    }

    tools: mainPageTools;

    // Create a selection dialog with the vocabulary titles to choose from.
    MySelectionDialog {
        id: categorySelectionDialog;
        titleText: "Choose an anagram category"
        selectedIndex: 1;

        model: anagramCategoryModel;

        onSelectedIndexChanged: {
            kanagramGame.useVocabulary(selectedIndex);
        }
    }

    Column {
        anchors {
            horizontalCenter: mainPage.horizontalCenter;
        }

        y: mainPage.height / 4;
        spacing: 20;
        Row {
            id: anagramRow;
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

                        drag.target: anagramLetterRectangle;
                        drag.axis: Drag.XandYAxis;
                        drag.minimumX: 0;
                        drag.maximumX: mainPage.width - anagramLetterRectangle.width;
                        drag.minimumY: 0;

                        onClicked: {
                            originalWordLetterRepeater.model =
                                kanagramEngineHelper.insertInCurrentOriginalWord(currentOriginalWordIndex, anagramLetterText.text);
                            ++currentOriginalWordIndex;
                            anagramLetterText.text = "";
                       }
                    }
                }
            }
        }

        Button {
            text: categorySelectionDialog.model[categorySelectionDialog.selectedIndex];
            onClicked: {
                categorySelectionDialog.open();
            }
        }


        Row {
            id: originalWordRow;
            spacing: 10;
            Repeater {
                id: originalWordLetterRepeater;
                model: anagram;
                Rectangle {
                    color: Qt.rgba(0, 0, 0, 0);
                    Text {
                        text: "";
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
