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

    function pushPage(file) {
        var component = Qt.createComponent(file)
        if (component.status == Component.Ready)
            pageStack.push(component);
        else
            console.log("Error loading component:", component.errorString());
    }

    tools: commonTools;

    Column {
        Row {
            id: anagramRow;
            Repeater {
                id: anagramLetterRepeater
                model: kanagramEngineHelper.createNextAnagram();
                Rectangle {
                    Text {
                        id: anagramLetter + parent.index;
                        text: modelData;
                    }

                    width: 40;
                    height: 40;
                    border.width: 1;

                    MouseArea {
                        anchors.fill: parent;
                        hoverEnabled: true;

                        drag.target: letterRectangle;
                        drag.axis: Drag.XandYAxis;
                        drag.minimumX: 0;
                        drag.maximumX: mainPage.width - letterRectangle.width;
                        drag.minimumY: 0;

                        onClicked: {
                            anagramLetter + parent.index = "";
                       }
                    }
                }
            }
        }

        Row {
            id: originalWordRow;
            Repeater {
                id: originalWordLetterRepeater;
                model: kanagramEngineHelper.createNextAnagram();
                Rectangle {
                    Text {
                        text: "";
                    }

                    width: 40;
                    height: 40;
                    border.width: 1;

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
