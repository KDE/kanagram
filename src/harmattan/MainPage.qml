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

    Column {
        id: mainPageColumn;
        width: settingsPageMainRectangle.width;
        anchors {
            margins: 5;
        }

        spacing: 20;

        Button {
            width: parent.width;
            anchors {
                horizontalCenter; parent.horizontalCenter;
            }

            text: qsTr("Play Game");
            font.pixelSize: 48;

            MouseArea {
                anchors.fill: parent;
                hoverEnabled: true;

                onClicked: {
                    chalkSoundEffect.play();
                    pageStack.push(gamePage);
                }
            }
        }

        Button {
            width: parent.width;
            anchors {
                horizontalCenter; parent.horizontalCenter;
            }

            text: qsTr("Settings");
            font.pixelSize: 48;

            MouseArea {
                anchors.fill: parent;
                hoverEnabled: true;

                onClicked: {
                    chalkSoundEffect.play();
                    pageStack.push(mainSettingsPage);
                }
            }
        }

        Button {
            width: parent.width;
            anchors {
                horizontalCenter; parent.horizontalCenter;
            }

            text: qsTr("Help");
            font.pixelSize: 48;

            MouseArea {
                anchors.fill: parent;
                hoverEnabled: true;

                onClicked: {
                    chalkSoundEffect.play();
                    pageStack.push(helpPage);
                }
            }
        }
    }
}
