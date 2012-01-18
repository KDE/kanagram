/***************************************************************************
 *   Copyright (C) 2005 by Joshua Keel <joshuakeel@gmail.com>              *
 *             (C) 2007 by Jeremy Whiting <jpwhiting@kde.org>              *
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

#include "kanagram.h"

#include <kaboutdata.h>
#include <kcmdlineargs.h>
#include <klocale.h>
#include <kapplication.h>

/** setup about data, create the main window, and start */
int main(int argc, char *argv[])
{
    KAboutData about("kanagram", 0, ki18n("Kanagram"), "0.2", ki18n("An anagram game"), 
        KAboutData::License_GPL, 
        ki18n("© 2005 Joshua Keel\n© 2005 Danny Allen\n© 2007 Jeremy Whiting"));
    about.addAuthor(ki18n("Joshua Keel"), ki18n("Coding"), "joshuakeel@gmail.com");
    about.addAuthor(ki18n("Danny Allen"), ki18n("Design, Graphics and many Vocabularies"), "danny@dannyallen.co.uk");
    about.addAuthor(ki18n("Jeremy Whiting"), ki18n("Maintainer"), "jpwhiting@kde.org");
    about.addCredit(ki18n("Laszlo Papp"), ki18n("Modularization and porting to Mobile"), "lpapp@kde.org");
    about.addCredit(ki18n("Artemiy Pavlov"), ki18n("Sound effects"), 0, "http://artemiolabs.com");
    about.addCredit(ki18n("Pino Toscano"), ki18n("Italian Data Files"), "pino@kde.org");
    about.addCredit(ki18n("Kris Thomsen"), ki18n("Danish Data Files"), "kris@scoutzone.dk");
    about.addCredit(ki18n("Patrick Spendrin"), ki18n("German Data Files"), "patrick_spendrin@gmx.de");
    about.addCredit(ki18n("Eric Krüse"), ki18n("British English Data Files"), "bildvontux@yahoo.de");
    about.addCredit(ki18n("Hanna Scott"), ki18n("Swedish Data Files"), "hanna.et.scott@gmail.com");
    about.addCredit(ki18n("Jure Repinc"), ki18n("Slovenian Data Files"), "jlp@holodeck1.com");
    about.addCredit(ki18n("Yuri Chornoivan"), ki18n("Ukranian Data Files"), "");
    KCmdLineArgs::init(argc, argv, &about);
    KApplication * app = new KApplication();

    Kanagram * a = new Kanagram();
    a->show();
    app->setTopWidget(a);
    return app->exec();
}

