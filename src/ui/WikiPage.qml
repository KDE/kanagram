
/******************************************************************************
 * This file is part of the Kanagram project
 * Copyright 2014 Debjit Mondal <debjitmondal05@gmail.com>
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
import QtQuick 2.0
import QtWebKit 3.0

Rectangle {

    id: page
    property alias url: view.url
    width: parent.width
    height: parent.height

    signal closed

    WebView {
        id: view
        anchors {
            verticalCenter: parent.verticalCenter
            horizontalCenter: parent.horizontalCenter
        }
        width: parent.width * 0.8
        height: parent.height * 0.8
        url: ""
        smooth: true
        scale: 1

        Rectangle {
            id: wikiPageActionBar
            width: parent.width
            height: parent.height / 20
            color: "navy"
            anchors {
                top: parent.top
                horizontalCenter: parent.horizontalCenter
            }

            Image {
                id: closeButton
                smooth: true
                height: parent.height
                anchors {
                    verticalCenter: parent.verticalCenter
                    right: parent.right
                }
                source: "icons/close.png"
                fillMode: Image.PreserveAspectFit

                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        closed()
                        page.visible = false
                    }
                }
            }
        }
    }
}
