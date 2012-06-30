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

import org.kde.plasma.components 0.1

Page {

    property int settingsPageMargins: 15;

    Dialog {
        id: anagramResolveTimeUserGuideDialog;
        content: i18n("<b>Anagram resolve time</b>: Maximum allowed time for resolving the anagram");
    }

    Dialog {
        id: hintShowDurationUserGuideDialog;
        content: i18n("<b>Hint show duration:</b> The duration for showing the hint for the current anagram");
    }

    Dialog {
        id: soundsUserGuideDialog;
        content: i18n("<b>Sounds:</b> Turn all the sounds on or off inside the game");
    }

    function pushPage(file) {
        var component = Qt.createComponent(file)
        if (component.status == Component.Ready)
            pageStack.push(component);
        else
            console.log("Error loading component:", component.errorString());
    }

    SelectionDialog {
        id: languageSelectionDialog;
        titleText: i18n("Select a language");
        selectedIndex: kanagramGame.dataLanguageSelectedIndex();

        model: kanagramGame.languageNames();

        onSelectedIndexChanged: {
            kanagramGame.dataLanguage = model[selectedIndex];
            rootWindow.languageSelectionChanged = true;
        }
    }

    Connections {
        target: kanagramEngineHelper;

        onHintHideTimeChanged: {
            hintAppearanceSlider.value = kanagramEngineHelper.hintHideTime;
        }

        onResolveTimeChanged: {
            resolveTimeSlider.value = kanagramEngineHelper.resolveTime;
        }

        onUseSoundsChanged: {
            soundsSwitch.checked = kanagramEngineHelper.useSounds;
        }
    }

    Component.onCompleted: {
        hintAppearanceSlider.value = kanagramEngineHelper.hintHideTime;
        resolveTimeSlider.value = kanagramEngineHelper.resolveTime;
        soundsSwitch.checked = kanagramEngineHelper.useSounds;
    }


    Rectangle {
        id: settingsPageMainRectangle;
        color: "black";
        anchors.fill: parent;

        Flickable {
            anchors {
                fill: parent;
                margins: settingsPageMargins;
            }

            contentWidth: settingsPageMainColumn.width;
            contentHeight: settingsPageMainColumn.height;

            Column {
                id: settingsPageMainColumn;
                width: settingsPageMainRectangle.width - 2*settingsPageMargins;

                spacing: 10;

                Label {
                    width: parent.width;
                    text: i18n("Kanagram Settings");
                    font.pixelSize: 32;
                }

                Image {
                    id: separator1;
                    width: parent.width;
                    height: 2;
                    fillMode: Image.TileHorizontally;
                    source: "separator.png";
                }

                Column {
                    width: parent.width;
                    spacing: 5;

                    Item {
                        height: childrenRect.height;
                        width: parent.width;

                        Label {
                            id: hintAppearanceLabel;

                            anchors {
                                left: parent.left;
                                verticalCenter: parent.verticalCenter;
                            }

                            text: i18n("Hint show duration in seconds");
                            font.bold: true;
                        }

                        ToolIcon {
                            iconSource: "image://theme/icon-l-user-guide-main-view"

                            anchors {
                                right: parent.right;
                                verticalCenter: parent.verticalCenter;
                            }

                            onClicked: {
                                hintShowDurationUserGuideDialog.open();
                            }
                        }
                    }

                    Slider {
                        id: hintAppearanceSlider;
                        width: parent.width - 10;
                        stepSize: 1;
                        valueIndicatorVisible: true;
                        minimumValue: 0;
                        maximumValue: 10;
                        anchors.horizontalCenter: parent.horizontalCenter;

                        onValueChanged: {
                            kanagramEngineHelper.hintHideTime = value;
                        }
                    }
                }

                Image {
                    id: separator2;
                    width: parent.width;
                    height: 2;
                    fillMode: Image.TileHorizontally;
                    source: "separator.png";
                }

                Column {
                    width: parent.width;
                    spacing: 5;

                    Item {
                        height: childrenRect.height;
                        width: parent.width;

                        Label {
                            id: resolveTimeLabel;

                            anchors {
                                left: parent.left;
                                verticalCenter: parent.verticalCenter;
                            }

                            text: i18n("Anagram resolve time in seconds");
                            font.bold: true;
                        }

                        ToolIcon {
                            iconSource: "image://theme/icon-l-user-guide-main-view"

                            anchors {
                                right: parent.right;
                                verticalCenter: parent.verticalCenter;
                            }

                            onClicked: {
                                anagramResolveTimeUserGuideDialog.open();
                            }
                        }
                    }

                    Slider {
                        id: resolveTimeSlider;
                        width: parent.width - 10;
                        stepSize: 15;
                        valueIndicatorVisible: true;
                        minimumValue: 0;
                        maximumValue: 300;
                        anchors.horizontalCenter: parent.horizontalCenter;

                        onValueChanged: {
                            kanagramEngineHelper.resolveTime = value;
                        }
                    }
                }

                Image {
                    id: separator3;
                    width: parent.width;
                    height: 2;
                    fillMode: Image.TileHorizontally;
                    source: "separator.png";
                }

                Item {
                    height: childrenRect.height;
                    width: parent.width;

                    Label {
                        anchors {
                            left: parent.left;
                            verticalCenter: parent.verticalCenter;
                        }

                        text: i18n("Sounds");
                        font.bold: true;
                    }

                    ToolIcon {
                        iconSource: "image://theme/icon-l-user-guide-main-view"

                        anchors {
                            right: soundsSwitch.left;
                            verticalCenter: parent.verticalCenter;
                        }

                        onClicked: {
                            soundsUserGuideDialog.open();
                        }
                    }

                    Switch {
                        id: soundsSwitch;
                        anchors {
                            right: parent.right;
                            verticalCenter: parent.verticalCenter;
                        }

                        onCheckedChanged: {
                            kanagramEngineHelper.useSounds = checked;
                        }
                    }
                }

                Image {
                    id: separator4;
                    width: parent.width;
                    height: 2;
                    fillMode: Image.TileHorizontally;
                    source: "separator.png";
                }

                ListItem {
                    iconSource: "preferences-desktop-locale.png";
                    titleText: i18n("Language");
                    subtitleText: kanagramGame.dataLanguage ? kanagramGame.dataLanguage : "English";
                    iconId: "textinput-combobox-arrow";
                    iconVisible: true;
                    mousePressed: languageSelectionMouseArea.pressed;

                    MouseArea {
                        id: languageSelectionMouseArea;
                        anchors.fill: parent;
                        onClicked: {
                            languageSelectionDialog.open();
                       }
                    }
                }
            }
        }
    }

    tools: commonTools;
}
