/***************************************************************************
 *   Copyright (C) 2005 by Joshua Keel <joshuakeel@gmail.com>              *
 *             (C) 2007 by Jeremy Whiting <jeremy@scitools.com>            *
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

#include "newstuff.h"

#include <qwidget.h>
#include <qpushbutton.h>

#include <kdebug.h>
#include <knewstuff2/engine.h>
#include <klocale.h>

#include <sharedkvtmlfiles.h>

#include "kanagramsettings.h"

NewStuff::NewStuff(QWidget *parent):QDialog(parent)
{
	setupUi(this);
}

NewStuff::~NewStuff()
{
}

void NewStuff::on_btnGetNew_clicked()
{
	KNS::Entry::List entries = KNS::Engine::download();
	
	SharedKvtmlFiles::sortDownloadedFiles();
}

#include "newstuff.moc"
