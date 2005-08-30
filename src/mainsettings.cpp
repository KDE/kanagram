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
 *   51 Franklin Steet, Fifth Floor, Boston, MA 02110-1301, USA.             *
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

	cboxTranslation->insertStringList(m_languageNames);
	cboxTranslation->setCurrentItem(m_languages.findIndex(KanagramSettings::defaultTranslation()));
	
	QFont f("squeaky chalk sound");
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

MainSettings::~MainSettings()
{
}

void MainSettings::slotUpdateParent()
{
	m_parent->enableButtonApply(true);
}

void MainSettings::setupTranslations()
{
	m_languages.clear();
	m_languageNames.clear();
	
	//the program scans in khangman/data/ to see what languages data is found
	QStringList mdirs = KGlobal::dirs()->findDirs("appdata", "data/");

	if (mdirs.isEmpty()) return;
	
	for (QStringList::Iterator it = mdirs.begin(); it != mdirs.end(); ++it )
	{
		QDir dir(*it);
		m_languages += dir.entryList(QDir::Dirs, QDir::Name);
		m_languages.remove(m_languages.find("."));
		m_languages.remove(m_languages.find(".."));
	}
	
	m_languages.sort();

	if (m_languages.isEmpty())
		return;
	//find duplicated entries in KDEDIR and KDEHOME

	QStringList temp_languages;
	for (uint i = 0;  i < m_languages.count(); i++)
	{
		if (m_languages.contains(m_languages[i]) > 1) 
		{
			temp_languages.append(m_languages[i]);
			m_languages.remove(m_languages[i]);
		}
	}

	for (uint i = 0;  i < temp_languages.count(); i++)
	{
		// Append 1 of the 2 instances found.
		if (i % 2 == 0)
			m_languages.append(temp_languages[i]);
	}
	temp_languages.clear();

	//the language code/name
	KConfig entry(locate("locale", "all_languages"));
	const QStringList::ConstIterator itEnd = m_languages.end();
	for (QStringList::Iterator it = m_languages.begin(); 
		it != m_languages.end(); ++it) {
		entry.setGroup(*it);
		if (*it == "sr")
			m_languageNames.append(entry.readEntry("Name")+" ("+i18n("Cyrillic")+")");
		else if (*it == "sr@Latn")
		{
			entry.setGroup("sr");
			m_languageNames.append(entry.readEntry("Name") + " ("+i18n("Latin")+")");
		}
		else
			m_languageNames.append(entry.readEntry("Name"));
	}
}

void MainSettings::getAndInstallFont()
{
	bool success = KIO::NetAccess::copy("http://www.kde-edu.org/kanagram/chalk.ttf", "fonts:/Personal/", 0);
	if (success)
	{
		getFontsButton->hide();
		KMessageBox::information(this, i18n("The font will not be available until Kanagram is restarted."));
	}
	else
	{
		KMessageBox::error(this, i18n("The font could not be installed, check you are connected to the internet."));
	}
}

void MainSettings::slotChangeTranslation()
{
	kdDebug() << "Writing new default language: " << m_languages[cboxTranslation->currentItem()] << endl;
	KanagramSettings::setDefaultTranslation(m_languages[cboxTranslation->currentItem()]);
	KanagramSettings::writeConfig();
}

#include "mainsettings.moc"
