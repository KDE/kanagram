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
#include "mainsettings.h"
#include "vocabsettings.h"

#include <QQmlContext>
#include <QQmlEngine>

#include <KConfigDialog>
#include <KDeclarative/KDeclarative>
#include <KHelpMenu>
#include <KLocalizedString>
#include <KSharedConfig>

#include <QDebug>

MainWindow::MainWindow()
    : m_game(new KanagramGame())
     ,m_helpMenu(new KHelpMenu(NULL))
     ,m_actionCollection(NULL)
{
    setResizeMode(QQuickView::SizeRootObjectToView);

    qDebug() << "Created game and engine helper";
    rootContext()->setContextProperty("kanagramGame", m_game);
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

    connect(m_game, SIGNAL(titleChanged()), SLOT(categoryChanged()));
    categoryChanged();
}

MainWindow::~MainWindow()
{
    KConfigGroup windowConfig = config("Window");
    windowConfig.writeEntry("geometry", geometry());
    windowConfig.writeEntry("windowState", int(windowState()));

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
    if (!KConfigDialog::showDialog("settings"))
    {
        m_configDialog = new KConfigDialog( NULL, "settings", KanagramSettings::self() );
        //m_configDialog->setAttribute(Qt::WA_DeleteOnClose);
        connect(m_configDialog, SIGNAL(settingsChanged(QString)), m_game, SLOT(reloadSettings()));

        // add the main settings page
        MainSettings * mainSettingsPage = new MainSettings( m_configDialog );
        connect (mainSettingsPage, SIGNAL(settingsChanged()), m_game, SLOT(reloadSettings()));
        m_configDialog->addPage(mainSettingsPage , i18nc("@title:group main settings page name", "General" ), "preferences-other" );

        // create and add the vocabsettings page
        m_vocabSettings = new VocabSettings( m_configDialog );
        m_configDialog->addPage(m_vocabSettings, i18n("Vocabularies"), "document-properties" );

        // now make and add the shortcuts page
        connect(m_configDialog, SIGNAL(accepted()), m_game, SLOT(refreshVocabularyList()));

        // m_configDialog->setHelp("kanagram/index.html");
        m_configDialog->resize(600, 500);
        m_configDialog->show();
    }
}

void MainWindow::categoryChanged()
{
    setTitle(m_game->documentTitle());
}

