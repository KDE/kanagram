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
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.             *
 ***************************************************************************/

#include <tqdir.h>
#include <tqcheckbox.h>
#include <tqcombobox.h>
#include <tqpushbutton.h>

#include <kdebug.h>
#include <kconfig.h>
#include <klocale.h>
#include <kstandarddirs.h>
#include <kconfigdialog.h>
#include <kmessagebox.h>
#include <kio/netaccess.h>

#include "mainsettings.h"
#include "kanagramsettings.h"

MainSettings::MainSettings(TQWidget *parent) : MainSettingsWidget(parent)
{
	m_parent = (KConfigDialog*)parent;

	connect(parent, TQT_SIGNAL(applyClicked()), this, TQT_SLOT(slotChangeTranslation()));
	connect(cboxTranslation, TQT_SIGNAL(activated(int)), this, TQT_SLOT(slotUpdateParent()));
	
	setupTranslations();

	TQStringList languageNames = m_languageCodeMap.keys();
	languageNames.sort();
	cboxTranslation->insertStringList(languageNames);
	
	//the language code/name
	KConfig entry(locate("locale", "all_languages"));
	TQString code = KanagramSettings::dataLanguage();
	entry.setGroup(code);
	if (code == "sr")
		cboxTranslation->setCurrentText(entry.readEntry("Name")+" ("+i18n("Cyrillic")+")");
	else if (code == "sr@Latn")
	{
		entry.setGroup("sr");
		cboxTranslation->setCurrentText(entry.readEntry("Name")+" ("+i18n("Latin")+")");
	}
	else
		cboxTranslation->setCurrentText(entry.readEntry("Name"));
	
	TQFont f("squeaky chalk sound");
	if (KanagramSettings::justGotFont())
	{
			getFontsButton->hide();
			kcfg_useStandardFonts->setEnabled(false);
	}
	else
	{
		if (!TQFontInfo(f).exactMatch())
		{
			kcfg_useStandardFonts->setEnabled(false);
			connect(getFontsButton, TQT_SIGNAL(pressed()), this, TQT_SLOT(getAndInstallFont()));
		}
		else
		{
			getFontsButton->hide();
		}
	}
}

MainSettings::~MainSettings()
{
}

void MainSettings::slotUpdateParent()
{
	m_parent->enableButtonApply(true);
}

void MainSettings::setupTranslations()
{
	m_languageCodeMap.clear();
	TQStringList languages, temp_languages;
	
	//the program scans in kdereview/data/ to see what languages data is found
	TQStringList mdirs = KGlobal::dirs()->findDirs("appdata", "data/");

	if (mdirs.isEmpty()) return;
	
	for (TQStringList::const_iterator it = mdirs.begin(); it != mdirs.end(); ++it )
	{
		TQDir dir(*it);
		temp_languages = dir.entryList(TQDir::Dirs, TQDir::Name);
		temp_languages.remove(".");
		temp_languages.remove("..");
		for (TQStringList::const_iterator it2 = temp_languages.begin(); it2 != temp_languages.end(); ++it2 )
		{
			if (!languages.tqcontains(*it2)) languages.append(*it2);
		}
	}
	
	if (languages.isEmpty())
		return;

	//the language code/name
	KConfig entry(locate("locale", "all_languages"));
	const TQStringList::ConstIterator itEnd = languages.end();
	for (TQStringList::ConstIterator it = languages.begin(); it != itEnd; ++it) {
		entry.setGroup(*it);
		if (*it == "sr")
			m_languageCodeMap.insert(entry.readEntry("Name")+" ("+i18n("Cyrillic")+")", "sr");
		else if (*it == "sr@Latn")
		{
			entry.setGroup("sr");
			m_languageCodeMap.insert(entry.readEntry("Name") + " ("+i18n("Latin")+")", "sr@Latn");
		}
		else
			m_languageCodeMap.insert(entry.readEntry("Name"), *it);
	}
}

void MainSettings::getAndInstallFont()
{
	bool success = KIO::NetAccess::copy("http://www.kde-edu.org/kanagram/chalk.ttf", "fonts:/Personal/", 0);
	if (success)
	{
		getFontsButton->hide();
		KMessageBox::information(this, i18n("Please restart Kanagram to activate the new font."));
		kcfg_useStandardFonts->setChecked(false);
		KanagramSettings::setUseStandardFonts(false);
		KanagramSettings::setJustGotFont(true);
		KanagramSettings::writeConfig();
	}
	else
	{
		KMessageBox::error(this, i18n("The font could not be installed. Please check that you are properly connected to the Internet."));
	}
}

void MainSettings::slotChangeTranslation()
{
	kdDebug() << "Writing new default language: " << m_languageCodeMap[cboxTranslation->currentText()] << endl;
	KanagramSettings::setDataLanguage(m_languageCodeMap[cboxTranslation->currentText()]);
	KanagramSettings::writeConfig();
}

#include "mainsettings.moc"
