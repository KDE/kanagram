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

KanagramEngineHelper::KanagramEngineHelper(KanagramGame* kanagramGame, QObject* parent)
    : QObject(parent)
    , m_kanagramGame(kanagramGame)
    , m_anagramCategoryModel(new SelectionModel(this))
{
    m_anagramCategoryModel->setStringList(m_kanagramGame->vocabularyList());
}

KanagramEngineHelper::~KanagramEngineHelper()
{
    delete m_kanagramGame;
    delete m_anagramCategoryModel;
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
    if (m_kanagramGame->word().size() < m_currentOriginalWord.size())
    {
        m_currentOriginalWord.clear();
    }

    int anagramWordSize = m_kanagramGame->word().size();
    while (m_currentOriginalWord.size() < anagramWordSize)
        m_currentOriginalWord.append("");

    m_currentOriginalWord.replace(index, letter);
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

SelectionModel *KanagramEngineHelper::anagramCategoryModel() const
{
    return m_anagramCategoryModel;
}

#include "kanagramenginehelper.moc"
