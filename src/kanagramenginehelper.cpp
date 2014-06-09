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

#include <QtGui/QApplication>

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
    QString anagram;

    int i = 0;

    // Try to get a not too long word, still visible and looks ok on the
    // chalkboard on the Harmattan device. Do not get stuck in such a loop
    // either, if there are no shorter words available, just quit the
    // application. It is an acceptable behavior for avoiding the messy layout
    // in such cases (long words, that is).
    //
    do {
        anagram = m_kanagramGame->anagram();
        if (anagram.size() < 13) {
            foreach (const QChar& anagramLetter, anagram)
            {
                anagramLetters.append(anagramLetter);
            }

            return anagramLetters;
        }
    } while (++i < 200);

    QApplication::instance()->quit();
    return QStringList();
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

void KanagramEngineHelper::saveSettings()
{
    KanagramSettings::self()->writeConfig();
    m_kanagramGame->refreshVocabularyList();
}

#include "kanagramenginehelper.moc"
