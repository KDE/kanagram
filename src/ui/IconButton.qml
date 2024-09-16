/***************************************************************************
 *   This file is part of the Kanagram project                             *
 *   Copyright 2024 Eamonn Rea <eamonnrea@gmail.com>                       *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.          *
 ***************************************************************************/

import QtQuick
import QtQuick.Controls

Image {
    id: root
    
    signal clicked

    property var iconContainer: parent  // Allows to optionally override the container for anchoring etc
    
    height: iconContainer.height / 2

    smooth: true
    fillMode: Image.PreserveAspectFit
    
    MouseArea {
        id: mouseArea

        anchors.fill: parent
        
        hoverEnabled: true
        preventStealing: true
        
        onClicked: root.clicked()
    }

    state: mouseArea.containsMouse ? "onEntered" : "onExited"
}
