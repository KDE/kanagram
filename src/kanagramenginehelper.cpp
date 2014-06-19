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
    ,m_speller(NULL)
    , m_insertCounter(0)
{
    m_speller = new Sonnet::Speller();
    m_speller->setLanguage(m_kanagramGame->sanitizedDataLanguage());
}

KanagramEngineHelper::~KanagramEngineHelper()
{
    delete m_kanagramGame;
    delete m_speller;
    m_speller=NULL;
}

QString KanagramEngineHelper::createNextAnagram()
{
    m_kanagramGame->nextAnagram();
    QString anagram;
    anagram = m_kanagramGame->anagram();
    return anagram;
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

QString KanagramEngineHelper::anagramOriginalWord() const
{
    QString originalWord = m_kanagramGame->word();
    return originalWord;
}

QString KanagramEngineHelper::showHint() const
{
    QString hint = m_kanagramGame->hint();
    return hint;
}

QString KanagramEngineHelper::categoryName() const
{
    QString categoryTitle = m_kanagramGame->documentTitle();
    return categoryTitle;
}

QString KanagramEngineHelper::nextVocabulary()
{
    m_kanagramGame->nextVocabulary();
    return m_kanagramGame->documentTitle();
}

QString KanagramEngineHelper::previousVocabulary()
{
    m_kanagramGame->previousVocabulary();
    return m_kanagramGame->documentTitle();
}


bool KanagramEngineHelper::checkWord(QString& answer)
{
    QString enteredWord = answer.toLower().trimmed();
    QString word = m_kanagramGame->word().toLower().trimmed();
    if (!enteredWord.isEmpty())
    {
        if (enteredWord == word || stripAccents(enteredWord) == stripAccents(word) ||
           (m_speller->isCorrect(enteredWord) && isAnagram(enteredWord, word)))
        {
            return true;
        }
        else
            return false;
    }
    else
        return false;
}

bool KanagramEngineHelper::isAnagram(QString& enteredword, QString& word)
{
    QString test = word;
    if (enteredword.length() <= word.length())
    {
        for (int i=0; i < enteredword.length(); i++)
        {
            int found = test.indexOf(enteredword[i]);

            if (found != -1)
            {
                test.remove(found, 1);
            }
            else
                break;
        }

        if (test.isEmpty())
            return true;
        else
            return false;
    }
    else
        return false;
}

QString KanagramEngineHelper::stripAccents(QString& original)
{
    QString noAccents;
    QString decomposed = original.normalized(QString::NormalizationForm_D);
    for (int i = 0; i < decomposed.length(); ++i) {
        if ( decomposed[i].category() != QChar::Mark_NonSpacing ) {
            noAccents.append(decomposed[i]);
        }
    }
    return noAccents;
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
