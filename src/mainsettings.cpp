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

#include "mainsettings.h"
#include "kanagramsettings.h"

MainSettings::MainSettings(QWidget *parent) : MainSettingsWidget(parent)
{
	m_parent = (KConfigDialog*)parent;

	connect(parent, SIGNAL(applyClicked()), this, SLOT(slotChangeTranslation()));
	connect(cboxTranslation, SIGNAL(activated(int)), this, SLOT(slotUpdateParent()));
	
	setupTranslations();

	QStringList languageNames = m_languageCodeMap.keys();
	languageNames.sort();
	cboxTranslation->insertStringList(languageNames);
	
	//the language code/name
	KConfig entry(locate("locale", "all_languages"));
	QString code = KanagramSettings::dataLanguage();
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
	
	QFont f("squeaky chalk sound");
	if (KanagramSettings::justGotFont())
	{
			getFontsButton->hide();
			kcfg_useStandardFonts->setEnabled(false);
	}
	else
	{
		if (!QFontInfo(f).exactMatch())
		{
			kcfg_useStandardFonts->setEnabled(false);
			connect(getFontsButton, SIGNAL(pressed()), this, SLOT(getAndInstallFont()));
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
	QStringList languages, temp_languages;
	
	//the program scans in kdereview/data/ to see what languages data is found
	QStringList mdirs = KGlobal::dirs()->findDirs("appdata", "data/");

	if (mdirs.isEmpty()) return;
	
	for (QStringList::const_iterator it = mdirs.begin(); it != mdirs.end(); ++it )
	{
		QDir dir(*it);
		temp_languages = dir.entryList(QDir::Dirs, QDir::Name);
		temp_languages.remove(".");
		temp_languages.remove("..");
		for (QStringList::const_iterator it2 = temp_languages.begin(); it2 != temp_languages.end(); ++it2 )
		{
			if (!languages.contains(*it2)) languages.append(*it2);
		}
	}
	
	if (languages.isEmpty())
		return;

	//the language code/name
	KConfig entry(locate("locale", "all_languages"));
	const QStringList::ConstIterator itEnd = languages.end();
	for (QStringList::ConstIterator it = languages.begin(); it != itEnd; ++it) {
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
