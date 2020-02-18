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
#include <QLoggingCategory>

#include <sharedkvtmlfiles.h>
#include "kanagramsettings.h"
#include "kanagramgame.h"

Q_DECLARE_LOGGING_CATEGORY(KANAGRAM)

MainSettings::MainSettings(QWidget *parent) : QWidget(parent)
{
    setupUi( this );

    slotToggleAdvancedSettings();
    populateLanguageBox();
    connect(scoringPointCheckbox, &QCheckBox::toggled, this, &MainSettings::slotToggleAdvancedSettings);

    //the language code/name
    QLocale languageLocale(KanagramSettings::dataLanguage());

    // select the current language
    languageComboBox->setCurrentIndex(languageComboBox->findText(languageLocale.nativeLanguageName()));

    // Connect after we set the current language from settings.
#if QT_VERSION < QT_VERSION_CHECK(5, 15, 0)
    connect(languageComboBox, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &MainSettings::widgetModified);
#else
    connect(languageComboBox, static_cast<void (QComboBox::*)(int, const QString &)>(&QComboBox::currentIndexChanged) , this, &MainSettings::widgetModified);
#endif

#ifndef HAVE_SPEECH
    kcfg_enablePronunciation->hide();
#endif
}

MainSettings::~MainSettings()
{
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
    for (int i = 0; i < languages.count(); ++i)
    {
        QLocale languageLocale(languages[i]);

        // get the language name
        QString languageName = languageLocale.nativeLanguageName();
        if (languageName.isEmpty() || languageName == QLatin1Char('C'))
        {
            languageName = i18nc("@item:inlistbox no language for that locale", "None");
        }
        languageComboBox->addItem(languageName, languages[i]);
    }
}

bool MainSettings::saveLanguage()
{
    int index = languageComboBox->currentIndex();
    QString language = languageComboBox->itemData(index).toString();
    qCDebug(KANAGRAM) << "Writing new default language: " << language;
    if (KanagramSettings::dataLanguage() != language) {
        KanagramSettings::setDataLanguage(language);
        KanagramSettings::self()->save();
        return true;
    }
    return false;
}
