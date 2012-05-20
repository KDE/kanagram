/*****************************************************************************
 * This file is part of the Kanagram project
 * Copyright (C) 2012 Laszlo Papp <lpapp@kde.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

import QtQuick 1.0
import org.kde.plasma.components 0.1 as PlasmaComponents

Image {
    id: rootItem
    source: "image://appbackgrounds/standard"
    fillMode: Image.Tile

    property alias currentGameId: gameDetailsTabGroup.gameId

    Row {
        anchors.fill: parent
        anchors.margins: 20
        spacing: 10

        Column {
            height: parent.height
            width: parent.width*0.4
            spacing: 20

            PlasmaComponents.Button {
                id: gamesListStatusChanger

                height: 32
                width: gamesListView.width

                onClicked: rootItem.toggleState()
            }

            ListView {
                id: gamesListView

                height: parent.height - gamesListStatusChanger.height
                width: parent.width

                clip: true
                spacing: 10

                delegate: GameItem { }
            }
        }

        Column {
            height: parent.height
            width: parent.width*0.6

            PlasmaComponents.TabBar {
                id: gameDetailsTabBar
                height: 64
                width: parent.width

                PlasmaComponents.TabButton {
                    tab: gameDetailsItem
                    text: "Details"
                }
                PlasmaComponents.TabButton {
                    tab: gameCommentsView
                    text: "Comments"
                }
                PlasmaComponents.TabButton {
                    tab: userDetailsItem
                    text: "User"
                }
            }

            PlasmaComponents.TabGroup {
                id: gameDetailsTabGroup
                property string gameId

                height: parent.height - gameDetailsTabBar.height
                width: parent.width

                GameDetails {
                    id: gameDetailsItem
                    gameId: gameDetailsTabGroup.gameId
                    height: parent.height
                    width: parent.width
                }

                CommentsView {
                    id: gameCommentsView
                    gameId: gameDetailsTabGroup.gameId
                    height: parent.height
                    width: parent.width
                }

                UserDetails {
                    id: userDetailsItem
                    height: parent.height
                    width: parent.width
                }
            }
        }
    }

    Component.onCompleted: {
        state = "showInstalledGames"
    }

    function toggleState()
    {
        if (rootItem.state == "showInstalledGames")
            rootItem.state = "showDownloadableGames";
        else
            rootItem.state = "showInstalledGames"
    }

    states: [
        State {
            name: "showInstalledGames"

            PropertyChanges { target: gamesListView; model: installedGamesModel }
            PropertyChanges { target: gamesListStatusChanger; text: "Showing Installed Games" }
        },
        State {
            name: "showDownloadableGames"

            PropertyChanges { target: gamesListView; model: downloadableGamesModel }
            PropertyChanges { target: gamesListStatusChanger; text: "Showing Downloadable Games" }
        }
    ]
}
