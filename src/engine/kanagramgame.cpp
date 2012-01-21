/***************************************************************************
 *   Copyright (C) 2005 by Joshua Keel <joshuakeel@gmail.com>              *
 *             (C) 2007 by Jeremy Whiting <jpwhiting@kde.org>              *
 *                                                                         *
 *   Portions of this code taken from KMessedWords by Reuben Sutton        *
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

#include "kanagramgame.h"

#include <QtCore/QFileInfo>

#include <kurl.h>
#include <kstandarddirs.h>
#include <klocale.h>

#include <sharedkvtmlfiles.h>
#include <keduvocdocument.h>
#include <keduvocexpression.h>

#include "kanagramsettings.h"


KanagramGame::KanagramGame() : m_index(0), m_document(NULL)
{
    // first get the list of vocabularies
    refreshVocabularyList();

    // then load the default vocab
    loadDefaultVocabulary();
}

KanagramGame::~KanagramGame()
{
    delete m_document;
    m_document = NULL;
}

bool KanagramGame::checkFile()
{
    if (!QFile::exists(KStandardDirs::locate("data", m_filename)))
    {
        emit fileError(m_filename);
        return false;
    }

    return true;
}

void KanagramGame::loadDefaultVocabulary()
{
    m_filename = KanagramSettings::defaultVocabulary();
    if (m_filename.isEmpty() || !QFileInfo(m_filename).exists())
    {
        refreshVocabularyList();
        nextVocabulary();
    }

    delete m_document;
    m_document = new KEduVocDocument(this);

    ///@todo open returns KEduVocDocument::ErrorCode
    int result = m_document->open(KUrl(KStandardDirs::locate("data", m_filename)));
    if (result != 0) {
        kDebug() << m_document->errorDescription(result);
    }
    nextAnagram();
}

bool KanagramGame::refreshVocabularyList()
{
    QString oldFilename = m_filename;
    m_fileList = SharedKvtmlFiles::fileNames(KanagramSettings::dataLanguage());
    if ( m_document ) {
        useVocabulary(m_document->title());
    }
    return oldFilename != m_filename;
}

/** get the list of vocabularies */
QStringList KanagramGame::vocabularyList()
{
    return SharedKvtmlFiles::titles(KanagramSettings::dataLanguage());
}

/** set the vocab to use */
void KanagramGame::useVocabulary(const QString &vocabularyname)
{
    QStringList titles = vocabularyList();
    int vocabulary = titles.indexOf(vocabularyname);
    if (vocabulary > 0)
    {
        m_index = vocabulary;
        m_filename = m_fileList.at(vocabulary);
    }
    else
    {
        m_index = 0;
        m_filename = m_fileList.first();
    }
}

void KanagramGame::previousVocabulary()
{
    if (--m_index < 0)
    {
        m_index = m_fileList.size() - 1;
    }

    m_filename = m_fileList.at(m_index);
    checkFile();
    delete m_document;
    m_document = new KEduVocDocument(this);
    ///@todo open returns KEduVocDocument::ErrorCode
    m_document->open(KUrl(KStandardDirs::locate("data", m_filename)));
    m_answeredWords.clear();
}

void KanagramGame::nextVocabulary()
{
    if (++m_index >= m_fileList.size())
    {
        m_index = 0;
    }

    if (!m_fileList.isEmpty())
    {
        m_filename = m_fileList.at(m_index);
        checkFile();
        delete m_document;
        m_document = new KEduVocDocument(this);
        ///@todo open returns KEduVocDocument::ErrorCode
        m_document->open(KUrl(KStandardDirs::locate("data", m_filename)));
        m_answeredWords.clear();
    }
}

void KanagramGame::nextAnagram()
{
    checkFile();

    int totalWords = m_document->lesson()->entryCount(KEduVocLesson::Recursive);
    int randomWordIndex = m_random.getLong(totalWords);

    if (totalWords == (int)m_answeredWords.size())
    {
        m_answeredWords.clear();
    }

    if (totalWords > 0)
    {
        KEduVocTranslation *translation = m_document->lesson()->entries(KEduVocLesson::Recursive).at(randomWordIndex)->translation(0);

        // Find the next word not used yet
        while (m_answeredWords.contains(translation->text()))
        {
            randomWordIndex = m_random.getLong(totalWords);
            translation =  m_document->lesson()->entries(KEduVocLesson::Recursive).at(randomWordIndex)->translation(0);
        }

        // lowercase the entry text so german words that start capitalized will be lowercased
        m_originalWord = translation->text().toLower();
        m_answeredWords.append(m_originalWord);
        createAnagram();
        m_hint = translation->comment();

        if (m_hint.isEmpty())
        {
            m_hint = i18n("No hint");
        }
    }
    else
    {
        // this file has no entries
        m_originalWord = "";
        m_hint = "";
        // TODO: add some error reporting here
    }
}

QString KanagramGame::filename()
{
    return m_fileList.empty() ? m_filename : m_fileList[m_index];
}

QString KanagramGame::anagram()
{
    return m_anagram;
}

QString KanagramGame::hint()
{
    return m_hint;
}

QString KanagramGame::word()
{
    return m_originalWord;
}

void KanagramGame::restoreWord()
{
    m_anagram = m_originalWord;
}

void KanagramGame::createAnagram()
{
    QString anagram;
    QString letters;
    int randomIndex;

    do {
        letters = m_originalWord;
        while (!letters.isEmpty())
        {
            randomIndex = m_random.getLong(letters.count());
            anagram.append(letters.at(randomIndex));
            letters.remove(randomIndex, 1);
        }
    } while (anagram == m_originalWord);

    m_anagram = anagram;
}

QString KanagramGame::documentTitle()
{
    if (m_document)
    {
        return m_document->title();
    }
    return QString();
}
