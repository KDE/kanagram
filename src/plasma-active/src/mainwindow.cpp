/******************************************************************************
 * This file is part of the Gluon Development Platform
 * Copyright 2011 Sebastian Kügler <sebas@kde.org>
 * Copyright 2011 Marco Martin <mart@kde.org>
 * Copyright 2012 Laszlo Papp <lpapp@kde.org>
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

#include "mainwindow.h"

#include <lib/gamemetadata.h>
#include <lib/gamemanager.h>
#include <lib/models/commentsmodel.h>
#include <lib/serviceprovider.h>

#include "kdeclarativeview.h"

#include <QtDeclarative/QtDeclarative>

MainWindow::MainWindow()
{
    declarativeView()->setPackageName("org.kde.gluon.player");

    declarativeView()->rootContext()->setContextProperty( "installedGamesModel",
                                                          GluonPlayer::GameManager::instance()->installedGamesModel() );
    declarativeView()->rootContext()->setContextProperty( "downloadableGamesModel",
                                                          GluonPlayer::GameManager::instance()->downloadableGamesModel() );
    declarativeView()->rootContext()->setContextProperty( "serviceProvider",
                                                          GluonPlayer::ServiceProvider::instance() );
    qmlRegisterType<GluonPlayer::GameMetadata>( "org.kde.gluon", 1, 0, "GameMetadata" );
    qmlRegisterType<GluonPlayer::CommentsModel>( "org.kde.gluon", 1, 0, "CommentsModel" );
}

#include "mainwindow.moc"
