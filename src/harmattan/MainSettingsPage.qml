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

    property string hintAppearanceTime: qsTr("5 seconds");
    property string resolveTime: qsTr("120 seconds");
    property string selectedLanguage: qsTr("English");

    function pushPage(file) {
        var component = Qt.createComponent(file)
        if (component.status == Component.Ready)
            pageStack.push(component);
        else
            console.log("Error loading component:", component.errorString());
    }

    SelectionDialog {
        id: hintAppearanceSelectionDialog;
        titleText: qsTr("Hide hints after");
        selectedIndex: 2;

        model: ListModel {
            ListElement { name: "No autohide" }
            ListElement { name: "3 seconds" }
            ListElement { name: "5 seconds" }
            ListElement { name: "7 seconds" }
            ListElement { name: "9 seconds" }
        }

        onSelectedIndexChanged: {
        }
    }

    SelectionDialog {
        id: resolveTimeSelectionDialog;
        titleText: qsTr("Time for resoving the anagram");
        selectedIndex: 3;

        model: ListModel {
            ListElement { name: "No time limit" }
            ListElement { name: "30 seconds" }
            ListElement { name: "60 seconds" }
            ListElement { name: "120 seconds" }
            ListElement { name: "300 seconds" }
        }

        onSelectedIndexChanged: {
        }
    }

    SelectionDialog {
        id: languageSelectionDialog;
        titleText: qsTr("Select a language");
        selectedIndex: 0;

        model: kanagramEngineHelper.languages();

        onSelectedIndexChanged: {
        }
    }

    Rectangle {
        color: "black";
        anchors.fill: parent;

        Column {
            anchors.fill: parent;

            ListItem {
                iconSource: "games-hint.png";
                titleText: qsTr("Hint appearance");
                subtitleText: qsTr(hintAppearanceTime);
                drillDownArrow: true;
                mousePressed: hintAppearanceMouseArea.pressed;

                MouseArea {
                    id: hintAppearanceMouseArea;
                    anchors.fill: parent;
                    onClicked: {
                        hintAppearanceSelectionDialog.open();
                   }
                }
            }

            ListItem {
                iconSource: "games-solve.png";
                titleText: qsTr("Resolve time");
                subtitleText: qsTr(resolveTime);
                drillDownArrow: true;
                mousePressed: resolveTimeMouseArea.pressed;

                MouseArea {
                    id: resolveTimeMouseArea;
                    anchors.fill: parent;
                    onClicked: {
                        resolveTimeSelectionDialog.open();
                   }
                }
            }

            Item {
                width: parent.width;
                height: childrenRect.height;
                Label {
                    anchors.left: parent.left;
                    text: "Use sounds";
                }

                Switch {
                    id: useSoundsSwitch;
                    anchors.right: parent.right;
                    checked: true;
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
