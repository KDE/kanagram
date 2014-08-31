/***************************************************************************
 *   Copyright (C) 2005 by Joshua Keel <joshuakeel@gmail.com>              *
 *             (C) 2007-2014 by Jeremy Whiting <jpwhiting@kde.org>         *
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

#include <KConfig>
#include <KConfigDialog>
#include <KLocalizedString>
#include <KMessageBox>

#include <QPushButton>
#include <QStandardPaths>

#include <sharedkvtmlfiles.h>
#include "kanagramsettings.h"

MainSettings::MainSettings(QWidget *parent) : QWidget(parent)
{
    setupUi( this );
    m_parent = (KConfigDialog*)parent;

    slotToggleAdvancedSettings();
    connect(parent, SIGNAL(settingsChanged(QString)), this, SLOT(slotUpdateLanguage()));
    connect(languageComboBox, SIGNAL(activated(int)), this, SLOT(slotSetDirty()));
    connect(scoringPointCheckbox,SIGNAL(toggled(bool)),this,SLOT(slotToggleAdvancedSettings()));
    populateLanguageBox();

    //the language code/name
    KConfig entry(QStandardPaths::locate(QStandardPaths::GenericDataLocation, QLatin1String("locale/") + "all_languages"));
    QString code = KanagramSettings::dataLanguage();
    KConfigGroup group = entry.group(code);

    // select the current language
    languageComboBox->setCurrentIndex(languageComboBox->findText(group.readEntry("Name")));
}

MainSettings::~MainSettings()
{
}

void MainSettings::slotSetDirty()
{
    m_parent->button(QDialogButtonBox::Apply)->setEnabled(true);
}

void MainSettings::slotToggleAdvancedSettings()
{
    bool enable = scoringPointCheckbox->isChecked();
    textCorrectAnswer->setVisible(enable);
    kcfg_correctAnswerScore->setVisible(enable);
    textIncorrectAnswer->setVisible(enable);
    kcfg_incorrectAnswerScore->setVisible(enable);
    textRevealAnswer->setVisible(enable);
    kcfg_revealAnswerScore->setVisible(enable);
    textSkippedWord->setVisible(enable);
    kcfg_skippedWordScore->setVisible(enable);
}

void MainSettings::populateLanguageBox()
{
    QStringList languages = SharedKvtmlFiles::languages();

    //the language code/name
    KConfig entry(QStandardPaths::locate(QStandardPaths::GenericDataLocation, QLatin1String("locale/") + "all_languages"));
    for (int i = 0; i < languages.count(); ++i)
    {
        KConfigGroup group = entry.group(languages[i]);

        // get the language name
        QString languageName = group.readEntry("Name");
        if (languageName.isEmpty())
        {
            languageName = i18nc("@item:inlistbox no language for that locale", "None");
        }
        languageComboBox->addItem(languageName, languages[i]);
    }
}

void MainSettings::slotUpdateLanguage()
{
    int index = languageComboBox->currentIndex();
    QString language = languageComboBox->itemData(index).toString();
    qDebug() << "Writing new default language: " << language;
    KanagramSettings::setDataLanguage(language);
    KanagramSettings::self()->save();

    emit settingsChanged();
}
