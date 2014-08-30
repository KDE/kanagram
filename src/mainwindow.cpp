/***************************************************************************
 *   This file is part of the Kanagram project                             *
 *   Copyright 2011 Sebastian Kügler <sebas@kde.org>                       *
 *   Copyright 2011 Marco Martin <mart@kde.org>                            *
 *   Copyright 2012 Laszlo Papp <lpapp@kde.org>                            *
 *   Copyright 2014 Jeremy Whiting <jpwhiting@kde.org>                     *
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

#include "mainwindow.h"

#include "kanagramgame.h"
#include "kanagramenginehelper.h"

#include <QQmlContext>
#include <QQmlEngine>

#include <KDeclarative/KDeclarative>
#include <KSharedConfig>

#include <QDebug>

MainWindow::MainWindow()
{
    m_game = new KanagramGame();
    m_engineHelper = new KanagramEngineHelper(m_game,this);

    setResizeMode(QQuickView::SizeRootObjectToView);

    qDebug() << "Created game and engine helper";
    rootContext()->setContextProperty("kanagramGame", m_game);
    rootContext()->setContextProperty("kanagramEngineHelper", m_engineHelper);
    rootContext()->setContextProperty("application", qApp);
    rootContext()->setContextProperty("mainwindow", this);

    qDebug() << "Set all context properties";

    KDeclarative::KDeclarative kdeclarative;
    kdeclarative.setDeclarativeEngine(engine());
    kdeclarative.setupBindings();

    qDebug() << "Setup declarative engine";

    KConfigGroup windowConfig = config("Window");
    if (windowConfig.hasKey("geometry")) {
        setGeometry(windowConfig.readEntry<QRect>("geometry", QRect()));
        setWindowState(Qt::WindowState(windowConfig.readEntry("windowState").toInt()));
    }

    QString location = QStandardPaths::locate(QStandardPaths::DataLocation, "ui/main.qml");
    setSource(QUrl::fromLocalFile(location));
    qDebug() << "Set qml file location";
}

MainWindow::~MainWindow()
{
    delete m_engineHelper;
    delete m_game;
}

bool MainWindow::event(QEvent *e)
{
    int type = e->type();
    if (type == QEvent::Close) {
        KConfigGroup windowConfig = config("Window");
        windowConfig.writeEntry("geometry", geometry());
        windowConfig.writeEntry("windowState", int(windowState()));
    }

    return QQuickView::event(e);
}

KConfigGroup MainWindow::config(const QString &group)
{
    return KConfigGroup(KSharedConfig::openConfig(qApp->applicationName() + "rc"), group);
}

