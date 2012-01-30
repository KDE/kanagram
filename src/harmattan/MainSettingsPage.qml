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
    id: mainSettingsPage;

    property string selectedLanguage: qsTr("English");

    function pushPage(file) {
        var component = Qt.createComponent(file)
        if (component.status == Component.Ready)
            pageStack.push(component);
        else
            console.log("Error loading component:", component.errorString());
    }

    SelectionDialog {
        id: languageSelectionDialog;
        titleText: qsTr("Select a language");
        selectedIndex: 0;

        model: kanagramEngineHelper.dataLanguage;

        onSelectedIndexChanged: {
        }
    }

    Rectangle {
        color: "black";
        anchors.fill: parent;

        Column {
            anchors {
                fill: parent;
                margins: 5;
            }

            spacing: 10;

            Label {
                width: parent.width;
                text: qsTr("Kanagram Settings");
            }

            Item {
                width: parent.width;

                Path {
                    startX: parent.x;
                    startY: parent.y;

                    PathLine {
                        x: parent.x + parent.width;
                        y: parent.y;
                    }
                }
            }

            Item {
                width: parent.width;
                height: childrenRect.height;

                Label {
                    id: hintAppearanceLabel;
                    anchors.left: parent.left;
                    text: qsTr("Hint appearance [sec]");
                }

                Slider {
                    id: hintAppearanceSlider;
                    width: parent.width - hintAppearanceLabel.width - 10;
                    stepSize: 1;
                    valueIndicatorVisible: true;
                    minimumValue: 0;
                    maximumValue: 10;
                    anchors.right: parent.right;
                    value: kanagramEngineHelper.hintHideTime;

                    onValueChanged: {
                        kanagramEngineHelper.hintHideTime = value;
                    }
                }
            }

            Item {
                width: parent.width;
                height: childrenRect.height;

                Label {
                    id: resolveTimeLabel;
                    anchors.left: parent.left;
                    text: qsTr("Resolve time [sec]");
                }

                Slider {
                    id: resolveTimeSlider;
                    width: parent.width - resolveTimeLabel.width - 10;
                    stepSize: 15;
                    valueIndicatorVisible: true;
                    minimumValue: 15;
                    maximumValue: 300;
                    anchors.right: parent.right;
                    value: kanagramEngineHelper.resolveTime;

                    onValueChanged: {
                        kanagramEngineHelper.resolveTime = value;
                    }
                }
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
                    checked: kanagramEngineHelper.useSounds;

                    onCheckedChanged: {
                        kanagramEngineHelper.useSounds = checked;
                    }
                }
            }

            ListItem {
                iconSource: "preferences-desktop-locale.png";
                titleText: qsTr("Language");
                subtitleText: qsTr(selectedLanguage);
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

    tools: commonTools;
}
