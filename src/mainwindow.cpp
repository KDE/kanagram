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
#include "kanagramsettings.h"
#include "kanagramconfigdialog.h"

#include <QQmlContext>
#include <QQmlEngine>

#include <KDeclarative/KDeclarative>
#include <KHelpMenu>
#include <KLocalizedString>
#include <KSharedConfig>

#include <QDebug>

MainWindow::MainWindow()
    : m_game(new KanagramGame())
     ,m_helpMenu(new KHelpMenu(nullptr))
{
    setResizeMode(QQuickView::SizeRootObjectToView);

    qCDebug(KANAGRAM) << "Created game and engine helper";
    rootContext()->setContextProperty(QStringLiteral("kanagramGame"), m_game);
    rootContext()->setContextProperty(QStringLiteral("application"), qApp);
    rootContext()->setContextProperty(QStringLiteral("mainwindow"), this);

    qCDebug(KANAGRAM) << "Set all context properties";

    KDeclarative::KDeclarative kdeclarative;
    kdeclarative.setDeclarativeEngine(engine());
    kdeclarative.setupBindings();

    qCDebug(KANAGRAM) << "Setup declarative engine";

    KConfigGroup windowConfig = config(QStringLiteral("Window"));
    if (windowConfig.hasKey("geometry")) {
        setGeometry(windowConfig.readEntry<QRect>("geometry", QRect()));
        setWindowState(Qt::WindowState(windowConfig.readEntry("windowState").toInt()));
    }

    setSource(QUrl("qrc:/qml/ui/main.qml"));
    qCDebug(KANAGRAM) << "Set qml file location";

    connect(m_game, &KanagramGame::titleChanged, this, &MainWindow::categoryChanged);
    categoryChanged();
}

MainWindow::~MainWindow()
{
    KConfigGroup windowConfig = config(QStringLiteral("Window"));
    windowConfig.writeEntry("geometry", geometry());
    windowConfig.writeEntry("windowState", int(windowState()));

    delete m_helpMenu;
    delete m_game;
}

KConfigGroup MainWindow::config(const QString &group)
{
    return KConfigGroup(KSharedConfig::openConfig(qApp->applicationName() + "rc"), group);
}

void MainWindow::showAboutKanagram()
{
    m_helpMenu->aboutApplication();
}

void MainWindow::showAboutKDE()
{
    m_helpMenu->aboutKDE();
}

void MainWindow::showHandbook()
{
    m_helpMenu->appHelpActivated();
}

void MainWindow::showSettings()
{
    if (!KConfigDialog::showDialog(QStringLiteral("settings")))
    {
        m_configDialog = new KanagramConfigDialog( NULL, QStringLiteral("settings"), KanagramSettings::self() );
        connect(m_configDialog, &KConfigDialog::settingsChanged, m_game, &KanagramGame::reloadSettings);

        connect(m_configDialog, &KConfigDialog::accepted, m_game, &KanagramGame::refreshVocabularyList);

        m_configDialog->resize(600, 500);
        m_configDialog->show();
    }
}

void MainWindow::categoryChanged()
{
    setTitle(m_game->documentTitle());
}

