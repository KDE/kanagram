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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QQuickView>
#include <KConfigGroup>

class KanagramGame;
class KConfigDialog;
class KHelpMenu;
class QEvent;
class VocabSettings;

class MainWindow : public QQuickView
{
    Q_OBJECT
public:
    MainWindow();
    ~MainWindow();

public slots:
    void showAboutKanagram();
    void showAboutKDE();
    void showHandbook();

    /** invoke the settings dialog */
    void showSettings();

private:
    KConfigGroup config(const QString &group);

    KanagramGame *m_game;

    /** settings dialog */
    KConfigDialog *m_configDialog;

    /** settings page pointers */
    VocabSettings *m_vocabSettings;

    /** help menu for displaying about box */
    KHelpMenu *m_helpMenu;
};

#endif // MAINWINDOW_H
