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

#include <QApplication>
#include <QLoggingCategory>

#include <KAboutData>
#include <KLocalizedString>
#include <Kdelibs4ConfigMigrator>

#include "mainwindow.h"

#define KANAGRAM_VERSION "1.2"
Q_LOGGING_CATEGORY(KANAGRAM, "org.kde.kanagram")

int main(int argc, char **argv)
{
    QStringList configFiles;
    configFiles << QLatin1String("kanagramrc");
    Kdelibs4ConfigMigrator migrator(QLatin1String("kanagram"));
    migrator.setConfigFiles(configFiles);
    migrator.migrate();

    QApplication::setApplicationName("kanagram");
    QApplication::setApplicationVersion(KANAGRAM_VERSION);
    QApplication::setOrganizationDomain("kde.org");
    KLocalizedString::setApplicationDomain("kanagram");
    QApplication::setApplicationDisplayName(i18n("kanagram"));
    QApplication app(argc, argv);

    KAboutData about(I18N_NOOP("kanagram"),
                     i18n("Kanagram"),
                     I18N_NOOP(KANAGRAM_VERSION),
                     i18n("An anagram game"),
                     KAboutLicense::GPL,
                     i18n("© 2005 Joshua Keel\n© 2005 Danny Allen\n© 2007 Jeremy Whiting\n© 2014 Debjit Mondal"));
    about.addAuthor(i18n("Joshua Keel"), i18n("Coding"), "joshuakeel@gmail.com");
    about.addAuthor(i18n("Danny Allen"), i18n("Design, Graphics and many Vocabularies"), "danny@dannyallen.co.uk");
    about.addAuthor(i18n("Jeremy Whiting"), i18n("Maintainer"), "jpwhiting@kde.org");
    about.addAuthor(i18n("Debjit Mondal"), i18n("Coding & Design"), "debjitmondal05@gmail.com");
    about.addCredit(i18n("Laszlo Papp"), i18n("Modularization and porting to Mobile"), "lpapp@kde.org");
    about.addCredit(i18n("Artemiy Pavlov"), i18n("Sound effects"), 0, "http://artemiolabs.com");
    about.addCredit(i18n("Pino Toscano"), i18n("Italian Data Files"), "pino@kde.org");
    about.addCredit(i18n("Kris Thomsen"), i18n("Danish Data Files"), "kris@scoutzone.dk");
    about.addCredit(i18n("Patrick Spendrin"), i18n("German Data Files"), "patrick_spendrin@gmx.de");
    about.addCredit(i18n("Eric Krüse"), i18n("British English Data Files"), "bildvontux@yahoo.de");
    about.addCredit(i18n("Hanna Scott"), i18n("Swedish Data Files"), "hanna.et.scott@gmail.com");
    about.addCredit(i18n("Jure Repinc"), i18n("Slovenian Data Files"), "jlp@holodeck1.com");
    about.addCredit(i18n("Yuri Chornoivan"), i18n("Ukranian Data Files"), "");
    about.addCredit(i18n("Kristóf Kiszel"), i18n("Hungarian Data Files"), "ulysses@kubuntu.org");
    about.addCredit(i18n("Souvik Das"), i18n("2-player mode"), "souvikdas728@gmail.com");
    about.addCredit(i18n("Sayan Biswas"), i18n("Letters turned to clickable buttons"), "techsayan01@gmail.com");

    KAboutData::setApplicationData(about);

    MainWindow mainWindow;
    QSize size(800, 600);
    mainWindow.setMinimumSize(size);
    mainWindow.show();

    return app.exec();
}
