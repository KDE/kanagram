/***************************************************************************
 *   Copyright (C) 2005 by Joshua Keel <joshuakeel@gmail.com>              *
 *             (C) 2007 by Jeremy Whiting <jeremy@scitools.com>            *
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

#include "mainsettings.h"

#include <qdir.h>
#include <qcheckbox.h>
#include <qcombobox.h>
#include <qpushbutton.h>

#include <kdebug.h>
#include <kconfig.h>
#include <klocale.h>
#include <kstandarddirs.h>
#include <kconfigdialog.h>
#include <kmessagebox.h>
#include <kio/netaccess.h>

#include "kanagramsettings.h"

MainSettings::MainSettings(QWidget *parent) : QWidget(parent)
{
	setupUi( this );
	m_parent = (KConfigDialog*)parent;

	connect(parent, SIGNAL(applyClicked()), this, SLOT(slotUpdateLanguage()));
	connect(cboxTranslation, SIGNAL(activated(int)), this, SLOT(slotSetDirty()));
	
	populateLanguageBox();

	//the language code/name
	KConfig entry(KStandardDirs::locate("locale", "all_languages"));
	QString code = KanagramSettings::dataLanguage();
	KConfigGroup group = entry.group(code);
	// select the current language
	cboxTranslation->setCurrentIndex(cboxTranslation->findText(group.readEntry("Name")));
}

MainSettings::~MainSettings()
{
}

void MainSettings::slotSetDirty()
{
	m_parent->enableButtonApply(true);
}

void MainSettings::populateLanguageBox()
{
	QSet<QString> languages;
	QStringList temp_languages;
	
	//the program scans in kdereview/data/ to see what languages data is found
	QStringList mdirs = KGlobal::dirs()->findDirs("appdata", "data/");

	if (mdirs.isEmpty()) return;
	
	for (int i = 0; i < mdirs.size(); ++i)
	{
		QDir dir(mdirs[i]);
		temp_languages = dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot, QDir::Name);
		languages.unite(QSet<QString>::fromList(temp_languages));
	}
	
	if (languages.isEmpty())
		return;

	//the language code/name
	KConfig entry(KStandardDirs::locate("locale", "all_languages"));
	QStringList languageList = languages.toList();
	const QStringList::ConstIterator itEnd = languageList.end();
	for (QStringList::ConstIterator it = languageList.begin(); it != itEnd; ++it) 
	{
		KConfigGroup group = entry.group(*it);
		cboxTranslation->addItem(group.readEntry("Name"), *it);
	}
}

void MainSettings::slotUpdateLanguage()
{
	int index = cboxTranslation->currentIndex();
	QString language = cboxTranslation->itemData(index).toString();
	kDebug() << "Writing new default language: " << language << endl;
	KanagramSettings::setDataLanguage(language);
	KanagramSettings::self()->writeConfig();
}

#include "mainsettings.moc"
