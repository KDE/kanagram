/***************************************************************************
 *   Copyright (C) 2005 by Joshua Keel <joshuakeel@gmail.com>              *
 *             (C) 2007-2014 by Jeremy Whiting <jpwhiting@kde.org>         *
 *             (C) 2012 by Laszlo Papp <lpapp@kde.org>                     *
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

#include "kanagramsettings.h"

#include <sharedkvtmlfiles.h>
#include <keduvocdocument.h>
#include <keduvocexpression.h>

#include <KDE/KStandardDirs>
#include <KLocalizedString>

#include <QLocale>
#include <QtCore/QFileInfo>

KanagramGame::KanagramGame() : m_fileIndex(0), m_document(NULL)
{
    // Get the list of vocabularies
    refreshVocabularyList();

    // Load the default vocabulary
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

QString KanagramGame::sanitizedDataLanguage() const
{
    QString dataLanguage = KanagramSettings::dataLanguage();

    if (dataLanguage.isEmpty()) {
        QStringList languageCodes = SharedKvtmlFiles::languages();
        if (languageCodes.contains(QLocale::system().uiLanguages().at(0))) {
            dataLanguage = QLocale::system().uiLanguages().at(0);
        } else {
            dataLanguage = "en";
        }

    }

    return dataLanguage;

}

void KanagramGame::loadDefaultVocabulary()
{
    m_filename = KanagramSettings::defaultVocabulary();
    if (m_filename.isEmpty() || !QFileInfo(m_filename).exists())
    {
        refreshVocabularyList();
        nextVocabulary();
    }
    else
    {
        int index = m_fileList.indexOf(m_filename);
        if (index > -1)
        {
            useVocabulary(index);
        }
        else
        {
            delete m_document;
            m_document = new KEduVocDocument(this);

            ///@todo open returns KEduVocDocument::ErrorCode
            int result = m_document->open(QUrl::fromLocalFile(KStandardDirs::locate("data", m_filename)), KEduVocDocument::FileIgnoreLock);
            if (result != 0) {
                qDebug() << m_document->errorDescription(result);
            }
        }
    }
    nextAnagram();
}

bool KanagramGame::refreshVocabularyList()
{
    QString oldFilename = m_filename;
    m_fileList = SharedKvtmlFiles::fileNames(sanitizedDataLanguage());
    if ( m_document ) {
        useVocabulary(m_document->title());
    }
    return oldFilename != m_filename;
}

QStringList KanagramGame::vocabularyList() const
{
    return SharedKvtmlFiles::titles(sanitizedDataLanguage());
}

void KanagramGame::useVocabulary(const QString &vocabularyname)
{
    useVocabulary(vocabularyList().indexOf(vocabularyname));
}

void KanagramGame::useVocabulary(int index)
{
    if (index < 0)
    {
        // Use the last
        index = m_fileList.size() - 1;
    }
    else if (index >= m_fileList.size())
    {
        index = 0;
    }

    m_fileIndex = index;
    m_filename = m_fileList.at(index);

    checkFile();
    delete m_document;
    m_document = new KEduVocDocument(this);
    ///@todo open returns KEduVocDocument::ErrorCode
    m_document->open(QUrl::fromLocalFile(KStandardDirs::locate("data", m_filename)), KEduVocDocument::FileIgnoreLock);
    m_answeredWords.clear();
    // Save the setting
    KanagramSettings::setCurrentVocabulary(index);
}

void KanagramGame::previousVocabulary()
{
    useVocabulary(m_fileIndex - 1);
}

void KanagramGame::nextVocabulary()
{
    useVocabulary(m_fileIndex + 1);
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

        // Make case consistent so german words that start capitalized will not
        // be so easy to guess
        if (KanagramSettings::uppercaseOnly())
        {
           m_originalWord = translation->text().toUpper();
        }
        else
        {
           m_originalWord = translation->text().toLower();
        }
        m_picHintUrl = translation->imageUrl();
        m_audioUrl = translation->soundUrl();

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
        m_picHintUrl = "";
        m_audioUrl = "";
        // TODO: add some error reporting here
    }
}

QString KanagramGame::filename() const
{
    return m_fileList.isEmpty() ? m_filename : m_fileList.at(m_fileIndex);
}

QString KanagramGame::anagram() const
{
    return m_anagram;
}

QString KanagramGame::hint() const
{
    return m_hint;
}

QString KanagramGame::word() const
{
    return m_originalWord;
}

void KanagramGame::createAnagram()
{
    if (m_originalWord.count(m_originalWord.at(0)) < m_originalWord.size()) {
        QString anagram;
        QString letters;
        int randomIndex;

        do {
            anagram.clear();
            letters = m_originalWord;
            while (!letters.isEmpty())
            {
                randomIndex = m_random.getLong(letters.count());
                anagram.append(letters.at(randomIndex));
                letters.remove(randomIndex, 1);
            }
        } while (anagram == m_originalWord);

        m_anagram = anagram;
    } else {
        m_anagram = m_originalWord;
    }
}

QString KanagramGame::documentTitle() const
{
    if (m_document)
    {
        return m_document->title();
    }

    return QString();
}

QStringList KanagramGame::languageNames()
{
    QStringList languageCodes = SharedKvtmlFiles::languages();
    if (languageCodes.isEmpty()) {
        return QStringList();
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
        m_languageCodeNameHash.insert(languageCode, languageName);
    }

    qSort(languageNames);
    return languageNames;
}

QString KanagramGame::dataLanguage() const
{
    return QLocale::languageToString(QLocale(sanitizedDataLanguage()).language());
}

void KanagramGame::setDataLanguage(const QString& dataLanguage)
{
    KanagramSettings::setDataLanguage(m_languageCodeNameHash.key(dataLanguage));
    emit dataLanguageChanged();
}

QUrl KanagramGame::picHint()
{
    return m_picHintUrl;
}

QUrl KanagramGame::audioFile()
{
    return m_audioUrl;
}
