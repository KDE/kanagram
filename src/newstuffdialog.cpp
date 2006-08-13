/***************************************************************************
 *   Copyright (C) 2005 by Joshua Keel                                     *
 *   joshuakeel@gmail.com                                                  *
 *                                                                         *
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
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.           *
 ***************************************************************************/

#include "newstuffdialog.h"

#include <knewstuff/knewstuff.h>
#include <knewstuff/entry.h>
#include <klocale.h>
#include <kio/netaccess.h>
#include <kurl.h>
#include <kstandarddirs.h>

#include "kanagramsettings.h"

NewStuffDialog::NewStuffDialog() : KNewStuff("kanagram/vocabulary", "http://kde-edu.org/kanagram/" + KanagramSettings::dataLanguage() + ".xml")
{
}

NewStuffDialog::~NewStuffDialog()
{
}

bool NewStuffDialog::install(const QString &/*fileName*/)
{
  return false;
}

QString NewStuffDialog::downloadDestination(KNS::Entry *entry)
{
	QString fileName = entry->payload().path();
	fileName = fileName.section('/', -1);
	return KGlobal::dirs()->saveLocation("data", "kanagram/data/" + KanagramSettings::dataLanguage()) + fileName;
}

bool NewStuffDialog::createUploadFile(const QString &/*fileName*/)
{
  return false;
}

