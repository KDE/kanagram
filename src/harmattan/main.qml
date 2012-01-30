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

PageStackWindow {
    id: rootWindow;

    // MainPage is what we see when the app starts, it shows up
    // the available games on the mobile handset
    initialPage: MainPage { id: mainPage; }

    // These tools are shared by most sub-pages by assigning the
    // id to a page's tools property
    ToolBarLayout {
        id: commonTools;
        visible: false;
        ToolIcon {
            iconId: "toolbar-back";
            onClicked: { pageStack.pop(); }
        }
    }

    Component.onCompleted: {
        // Use the dark theme.
        theme.inverted = true;
    }

    platformStyle: PageStackWindowStyle {
        background: "kanagram-chalkboard-landscape.png";
        landscapeBackground: "kanagram-chalkboard-landscape.png";
        portraitBackground: "kanagram-chalkboard-portrait.png";
        backgroundFillMode: Image.Tile;
    }
}
