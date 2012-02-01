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

    function pushPage(file) {
        var component = Qt.createComponent(file)
        if (component.status == Component.Ready)
            pageStack.push(component);
        else
            console.log("Error loading component:", component.errorString());
    }

    MySelectionDialog {
        id: languageSelectionDialog;
        titleText: qsTr("Select a language");
        selectedIndex: 0;

        model: kanagramEngineHelper.languageNames();

        onSelectedIndexChanged: {
            kanagramEngineHelper.dataLanguage = model[selectedIndex];
        }
    }

    Connections {
        target: kanagramEngineHelper;

        onHintHideTimeChanged: {
            hintAppearanceSlider.value = kanagramEngineHelper.hintHideTime;
        }
    }

    Connections {
        target: kanagramEngineHelper;

        onResolveTimeChanged: {
            resolveTimeSlider.value = kanagramEngineHelper.resolveTime;
        }
    }

    Connections {
        target: kanagramEngineHelper;

        onUseSoundsChanged: {
            soundsSwitch.checked = kanagramEngineHelper.useSounds;
        }
    }

    Rectangle {
        id: settingsPageMainRectangle;
        color: "black";
        anchors.fill: parent;

        Flickable {
            width: parent.width;
            height: parent.height;
            contentWidth: settingsPageMainColumn.width;
            contentHeight: settingsPageMainColumn.height;

            Column {
                id: settingsPageMainColumn;
                width: settingsPageMainRectangle.width;
                anchors {
                    margins: 5;
                }

                spacing: 10;

                Label {
                    width: parent.width;
                    text: qsTr("Kanagram Settings");
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

                    Label {
                        id: hintAppearanceLabel;
                        anchors.left: parent.left;
                        text: qsTr("Hint appearance in seconds");
                    }

                    Slider {
                        id: hintAppearanceSlider;
                        width: parent.width - 10;
                        stepSize: 1;
                        valueIndicatorVisible: true;
                        minimumValue: 0;
                        maximumValue: 10;
                        anchors.horizontalCenter: parent.horizontalCenter;

                        Component.onCompleted: {
                            value: kanagramEngineHelper.hintHideTime;
                        }

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

                    Label {
                        id: resolveTimeLabel;
                        anchors.left: parent.left;
                        text: qsTr("Resolve time in seconds");
                    }

                    Slider {
                        id: resolveTimeSlider;
                        width: parent.width - 10;
                        stepSize: 15;
                        valueIndicatorVisible: true;
                        minimumValue: 15;
                        maximumValue: 300;
                        anchors.horizontalCenter: parent.horizontalCenter;

                        Component.onCompleted: {
                            value: kanagramEngineHelper.resolveTime;
                        }

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
                    width: parent.width;
                    height: childrenRect.height;

                    Label {
                        anchors.left: parent.left;
                        text: qsTr("Sounds");
                    }

                    Switch {
                        id: soundsSwitch;
                        anchors.right: parent.right;

                        Component.onCompleted: {
                            checked: kanagramEngineHelper.useSounds;
                        }

                        onCheckedChanged: {
                            kanagramEngineHelper.useSounds = checked;
                        }
                    }
                }

                ListItem {
                    iconSource: "preferences-desktop-locale.png";
                    titleText: qsTr("Language");
                    subtitleText: kanagramEngineHelper.dataLanguage ?  kanagramEngineHelper.dataLanguage : "English";
                    drillDownArrow: true;
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
