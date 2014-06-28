/******************************************************************************
 * This file is part of the Kanagram project
 * Copyright 2011 Sebastian Kügler <sebas@kde.org>
 * Copyright 2011 Marco Martin <mart@kde.org>
 * Copyright 2012 Laszlo Papp <lpapp@kde.org>
 * Copyright 2014 Jeremy Whiting <jpwhiting@kde.org>
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

#include "kanagramgame.h"
#include "kanagramenginehelper.h"

#include <QQmlContext>
#include <QQmlEngine>

#include <KDeclarative/KDeclarative>
#include <KSharedConfig>

MainWindow::MainWindow()
{
    m_game = new KanagramGame();
    m_engineHelper = new KanagramEngineHelper(m_game,this);

    setResizeMode(QQuickView::SizeRootObjectToView);

    rootContext()->setContextProperty("kanagramGame", m_game);
    rootContext()->setContextProperty("kanagramEngineHelper", m_engineHelper);
    rootContext()->setContextProperty("application", qApp);

    KDeclarative::KDeclarative kdeclarative;
    kdeclarative.setDeclarativeEngine(engine());
    kdeclarative.initialize();
    kdeclarative.setupBindings();

    //    restoreWindowSize(config("Window"));

    QString location = QStandardPaths::locate(QStandardPaths::DataLocation, "ui/main.qml");
    setSource(QUrl::fromLocalFile(location));
}

MainWindow::~MainWindow()
{
//    KConfigGroup group = config("Window");
//    saveWindowSize(group);

    delete m_engineHelper;
    delete m_game;
}

KConfigGroup MainWindow::config(const QString &group)
{
    return KConfigGroup(KSharedConfig::openConfig(qApp->applicationName() + "rc"), group);
}

