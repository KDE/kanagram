/******************************************************************************
 * This file is part of the Kanagram project
 * Copyright (c) 2012 Laszlo Papp <lpapp@kde.org>
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

#include "kanagramenginehelper.h"

#include <kanagramsettings.h>

#include <sharedkvtmlfiles.h>

#include <KDE/KStandardDirs>
#include <KDE/KLocale>

KanagramEngineHelper::KanagramEngineHelper(KanagramGame* kanagramGame, QObject* parent)
    : QObject(parent)
    , m_kanagramGame(kanagramGame)
    , m_insertCounter(0)
{
}

KanagramEngineHelper::~KanagramEngineHelper()
{
    delete m_kanagramGame;
}

QStringList KanagramEngineHelper::createNextAnagram()
{
    m_kanagramGame->nextAnagram();
    QStringList anagramLetters;

    QString anagram = m_kanagramGame->anagram();

    foreach (const QChar& anagramLetter, anagram)
    {
        anagramLetters.append(anagramLetter);
    }

    return anagramLetters;
}

QStringList KanagramEngineHelper::insertInCurrentOriginalWord(int index, const QString& letter)
{
    int anagramWordSize = m_kanagramGame->word().size();

    if (anagramWordSize != m_currentOriginalWord.size()
            || m_currentOriginalWord.size() == m_insertCounter)
    {
        m_currentOriginalWord.clear();
        m_insertCounter = 0;
    }

    while (m_currentOriginalWord.size() < anagramWordSize)
        m_currentOriginalWord.append("");

    m_currentOriginalWord.replace(index, letter);
    ++m_insertCounter;
    return m_currentOriginalWord;
}

QStringList KanagramEngineHelper::removeInCurrentOriginalWord(int index)
{
    m_currentOriginalWord.replace(index, "");
    --m_insertCounter;
    return m_currentOriginalWord;
}

QStringList KanagramEngineHelper::anagramOriginalWord() const
{
    QStringList originalWordLetters;

    QString originalWord = m_kanagramGame->word();

    foreach (const QChar& originalWordLetter, originalWord)
    {
        originalWordLetters.append(originalWordLetter);
    }

    return originalWordLetters;
}

QStringList KanagramEngineHelper::languageNames() const
{
    QStringList languageCodes = SharedKvtmlFiles::languages();
    if (languageCodes.isEmpty()) {
        QApplication::instance()->quit();
    }

    QStringList languageNames;

    // Get the language names from the language codes

    KConfig entry(KStandardDirs::locate("locale", "all_languages"));

    foreach (const QString& languageCode, languageCodes)
    {
        KConfigGroup group = entry.group(languageCode);

        QString languageName = group.readEntry("Name");
        if (languageName.isEmpty())
        {
            languageName = i18nc("@item:inlistbox no language for that locale", "None");
        }

        languageNames.append(languageName);
    }

    return languageNames;
}

bool KanagramEngineHelper::compareWords() const
{
    return m_currentOriginalWord.join("") == m_kanagramGame->word();
}

int KanagramEngineHelper::hintHideTime()
{
    return KanagramSettings::hintHideTime().toInt();
}

void KanagramEngineHelper::setHintHideTime(int hintHideTime)
{
    KanagramSettings::setHintHideTime(QString::number(hintHideTime));
    emit hintHideTimeChanged();
}

int KanagramEngineHelper::resolveTime()
{
    return KanagramSettings::resolveTime().toInt();
}

void KanagramEngineHelper::setResolveTime(int resolveTime)
{
    KanagramSettings::setResolveTime(QString::number(resolveTime));
    emit resolveTimeChanged();
}

bool KanagramEngineHelper::useSounds()
{
    return KanagramSettings::useSounds();
}

void KanagramEngineHelper::setUseSounds(bool useSounds)
{
    KanagramSettings::setUseSounds(useSounds);
    emit useSoundsToggled();
}

QString KanagramEngineHelper::defaultVocabulary()
{
    return KanagramSettings::defaultVocabulary();
}

void KanagramEngineHelper::setDefaultVocabulary(const QString& defaultVocabulary)
{
    KanagramSettings::setDefaultVocabulary(defaultVocabulary);
    emit defaultVocabularyChanged();
}

QString KanagramEngineHelper::dataLanguage()
{
    return KanagramSettings::dataLanguage();
}

void KanagramEngineHelper::setDataLanguage(const QString& dataLanguage)
{
    KanagramSettings::setDataLanguage(dataLanguage);
    emit dataLanguageChanged();
}

void KanagramEngineHelper::saveSettings()
{
    KanagramSettings::self()->writeConfig();
}

#include "kanagramenginehelper.moc"
