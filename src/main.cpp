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

#include <QApplication>

#include <K4AboutData>
#include <KLocalizedString>

#include "mainwindow.h"

int main(int argc, char **argv)
{
    K4AboutData about("kanagram", "kanagram", ki18n("Kanagram"), "0.2", ki18n("An anagram game"),
                     K4AboutData::License_GPL, ki18n("© 2005 Joshua Keel\n© 2005 Danny Allen\n© 2007 Jeremy Whiting\n© 2014 Debjit Mondal"));
    about.addAuthor(ki18n("Joshua Keel"), ki18n("Coding"), "joshuakeel@gmail.com");
    about.addAuthor(ki18n("Danny Allen"), ki18n("Design, Graphics and many Vocabularies"), "danny@dannyallen.co.uk");
    about.addAuthor(ki18n("Jeremy Whiting"), ki18n("Maintainer"), "jpwhiting@kde.org");
    about.addAuthor(ki18n("Debjit Mondal"), ki18n("Coding & Design"), "debjitmondal05@gmail.com");
    about.addCredit(ki18n("Laszlo Papp"), ki18n("Modularization and porting to Mobile"), "lpapp@kde.org");
    about.addCredit(ki18n("Artemiy Pavlov"), ki18n("Sound effects"), 0, "http://artemiolabs.com");
    about.addCredit(ki18n("Pino Toscano"), ki18n("Italian Data Files"), "pino@kde.org");
    about.addCredit(ki18n("Kris Thomsen"), ki18n("Danish Data Files"), "kris@scoutzone.dk");
    about.addCredit(ki18n("Patrick Spendrin"), ki18n("German Data Files"), "patrick_spendrin@gmx.de");
    about.addCredit(ki18n("Eric Krüse"), ki18n("British English Data Files"), "bildvontux@yahoo.de");
    about.addCredit(ki18n("Hanna Scott"), ki18n("Swedish Data Files"), "hanna.et.scott@gmail.com");
    about.addCredit(ki18n("Jure Repinc"), ki18n("Slovenian Data Files"), "jlp@holodeck1.com");
    about.addCredit(ki18n("Yuri Chornoivan"), ki18n("Ukranian Data Files"), "");
    about.addCredit(ki18n("Kristóf Kiszel"), ki18n("Hungarian Data Files"), "ulysses@kubuntu.org");

    QApplication app(argc, argv);

    MainWindow * mainWindow = new MainWindow();
    QSize size(800, 600);
    mainWindow->setMinimumSize(size);
    mainWindow->show();

    return app.exec();
}
