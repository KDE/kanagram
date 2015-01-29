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
#ifdef BUILD_WITH_SPEECH
#include <kspeech.h>
#include <ktoolinvocation.h>
#endif

#include <KLocalizedString>
#include <sonnet/speller.h>

#include <QLocale>
#include <QFileInfo>
#include <QStandardPaths>

KanagramGame::KanagramGame()
    : m_fileIndex(-1)
      ,m_document(NULL)
#ifdef BUILD_WITH_SPEECH
      ,m_kspeech(NULL)
#endif
      ,m_totalScore(0)
      ,m_totalScore2(0)
      ,m_currentPlayerNumber(1)
      ,m_speller(NULL)
{
    loadSettings();

    // Get the list of vocabularies
    refreshVocabularyList();

    // Load the default vocabulary
    loadDefaultVocabulary();
#ifdef BUILD_WITH_SPEECH
    setupJovie();
#endif

    m_speller = new Sonnet::Speller();
    m_speller->setLanguage(sanitizedDataLanguage());
}

KanagramGame::~KanagramGame()
{
    // Save any settings that may have changed
    KanagramSettings::self()->save();

    delete m_document;
    m_document = NULL;
    delete m_speller;
    m_speller = NULL;
}

bool KanagramGame::checkFile()
{
    if (!QFile::exists(m_filename) && !QFile::exists(QStandardPaths::locate(QStandardPaths::GenericDataLocation, m_filename)))
    {
        emit fileError(m_filename);
        return false;
    }

    return true;
}

QString KanagramGame::sanitizedDataLanguage() const
{
    QString dataLanguage = KanagramSettings::dataLanguage();
    QStringList languageCodes = SharedKvtmlFiles::languages();

    if (dataLanguage.isEmpty() || !languageCodes.contains(dataLanguage)) {
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
    int index = KanagramSettings::currentVocabulary();
    if (index == -1)
        index = 0;
    useVocabulary(index);
    nextAnagram();
}

void KanagramGame::setSinglePlayerMode(bool singlePlayer)
{
    KanagramSettings::setSinglePlayerMode(singlePlayer);
    emit singlePlayerChanged();
}

bool KanagramGame::singlePlayerMode()
{
  return KanagramSettings::singlePlayerMode();
}

int KanagramGame::getPlayerNumber()
{
    return m_currentPlayerNumber;
}

void KanagramGame::setPlayerNumber(int pnumber)
{
    m_currentPlayerNumber = pnumber;
    emit currentPlayerChanged();
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
    int previous = m_fileIndex;
    if (index < 0 && m_fileList.size() > 0)
    {
        // Use the last
        index = m_fileList.size() - 1;
    }
    else if (index >= m_fileList.size())
    {
        index = 0;
    }

    m_fileIndex = index;
    m_filename = m_fileList.size() > index  && index >= 0 ? m_fileList.at(index) : QString();

    if (m_fileIndex != previous && checkFile()) {
        delete m_document;
        m_document = new KEduVocDocument(this);
        ///@todo open returns KEduVocDocument::ErrorCode
        m_document->open(QUrl::fromLocalFile(m_filename), KEduVocDocument::FileIgnoreLock);
        m_answeredWords.clear();
        // Save the setting
        KanagramSettings::setCurrentVocabulary(index);
        KanagramSettings::self()->save();
        emit titleChanged();
    }
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
    if (checkFile())
    {
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
        emit userAnswerChanged();
        emit wordChanged();
    }
}

QString KanagramGame::filename() const
{
    return m_fileList.isEmpty() ? m_filename : m_fileList.at(m_fileIndex);
}

QStringList KanagramGame::anagram() const
{
    QStringList resultList;
    foreach (const QChar &userLetter, m_anagram)
    {
        resultList.append(userLetter);
    }

    return resultList;
}

QString KanagramGame::hint() const
{
    return m_hint;
}

QString KanagramGame::word() const
{
    return m_originalWord;
}

QStringList KanagramGame::userAnswer() const
{
    QStringList returnList;
    foreach (const QChar &letter, m_userAnswer)
    {
        returnList.append(letter);
    }
    return returnList;
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
        m_userAnswer.clear();
    } else {
        m_anagram = m_originalWord;
        m_userAnswer.clear();
    }
}

bool KanagramGame::useSounds()
{
    return KanagramSettings::useSounds();
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
    KConfig entry(QStandardPaths::locate(QStandardPaths::GenericDataLocation, QLatin1String("locale/") + "all_languages"));

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
    KanagramSettings::self()->save();
    // Update the speller's language accordingly
    m_speller->setLanguage(sanitizedDataLanguage());
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

#ifdef BUILD_WITH_SPEECH
void KanagramGame::wordRevealed()
{
    if (KanagramSettings::enablePronunciation())
    {
        say(m_originalWord);
    }
}
#endif

#ifdef BUILD_WITH_SPEECH
void KanagramGame::setupJovie()
{
    // If KTTSD not running, start it.
    QDBusReply<bool> reply = QDBusConnection::sessionBus().interface()->isServiceRegistered( "org.kde.kttsd" );
    bool kttsdactive = false;
    if ( reply.isValid() )
        kttsdactive = reply.value();
    if ( !kttsdactive )
    {
        QString error;
        if ( KToolInvocation::startServiceByDesktopName( "kttsd", QStringList(), &error ) )
        {
            QMessageBox::warning(NULL, i18n("Speech System Failure"), i18n( "Starting Jovie Text-to-Speech service Failed: %1", error ) );
        }
        else
        {
            kttsdactive = true;
        }
    }
    if ( kttsdactive && m_kspeech==0)
    {
        // creating the connection to the kspeech interface
        m_kspeech = new org::kde::KSpeech( "org.kde.kttsd", "/KSpeech", QDBusConnection::sessionBus() );
        m_kspeech->setParent(this);
        m_kspeech->setApplicationName( "Kanagram" );
    }
}

void KanagramGame::say(QString text)
{
    if ( text.isEmpty() )
        return;

    setupJovie();
    if ( this->m_kspeech )
    {
        QDBusReply< int > reply = this->m_kspeech->say(text, KSpeech::soPlainText );
    }
}
#endif

int KanagramGame::hintHideTime()
{
    QString hideTimeString = KanagramSettings::hintHideTime();

    int hintHideTime = getNumericSetting(hideTimeString);
    if (hintHideTime)
        return ((hintHideTime * 2) + 1);
    else
        return 0;
}

int KanagramGame::resolveTime()
{
    return KanagramSettings::resolveTime().toInt();
}

int KanagramGame::scoreTime()
{
    QString scoreTimeString = KanagramSettings::scoreTime();

    int scoreTime = getNumericSetting(scoreTimeString);
    return ((scoreTime + 1) * 15);
}

void KanagramGame::moveLetterToUserAnswer(int position)
{
    m_userAnswer.append(m_anagram[position]);
    m_anagram.remove(position, 1);
    emit wordChanged();
    emit userAnswerChanged();
}

void KanagramGame::moveLetterToAnagram(int position)
{
    m_anagram.append(m_userAnswer[position]);
    m_userAnswer.remove(position, 1);
    emit wordChanged();
    emit userAnswerChanged();
}

void KanagramGame::resetAnagram()
{
    m_anagram = m_userAnswer;
    m_userAnswer.clear();
    emit wordChanged();
    emit userAnswerChanged();
}

void KanagramGame::moveLetter(QString letter)
{
    QString small = letter.toLower();
    int index = m_anagram.toLower().indexOf(small);
    if (index != -1)
    {
        moveLetterToUserAnswer(index);
    }
    else
    {
        index = m_userAnswer.toLower().indexOf(letter);
        if (index != -1)
            moveLetterToAnagram(index);
    }
}

int KanagramGame::getNumericSetting(QString settingString)
{
    int indexFound_setting = settingString.size();
    for (int k = 0; k < indexFound_setting; ++k)
    {
        if (!settingString.at(k).isDigit())
        {
            indexFound_setting = k;
            break;
        }
    }
    return settingString.left(indexFound_setting).toInt();
}

void KanagramGame::resetTotalScore()
{
    if (m_currentPlayerNumber == 1)
    {
        m_totalScore = 0;
        m_totalScore2 = 0;
        emit scoreChanged();
    }
}

void KanagramGame::adjustScore(int points)
{
    if (m_currentPlayerNumber == 1)
        m_totalScore += points;
    else
        m_totalScore2 += points;
    emit scoreChanged();
}

int KanagramGame::totalScore()
{
    return m_totalScore;
}

int KanagramGame::totalScore2()
{
    return m_totalScore2;
}

void KanagramGame::revealWord()
{
    m_anagram = m_originalWord;
    emit wordChanged();
}

bool KanagramGame::checkWord()
{
    QString enteredWord = m_userAnswer.toLower().trimmed();
    QString lowerOriginal = m_originalWord.toLower();
    QString strippedOriginal = stripAccents(m_originalWord);
    if (!enteredWord.isEmpty())
    {
        if (enteredWord == lowerOriginal ||
            stripAccents(enteredWord) == strippedOriginal ||
            (m_speller->isCorrect(enteredWord) && m_speller->isValid() &&
               (isAnagram(enteredWord, lowerOriginal) ||
                isAnagram(enteredWord, strippedOriginal))))
        {
#ifdef BUILD_WITH_SPEECH
            if (KanagramSettings::enablePronunciation())
            {
                // User wants words spoken
            }
#endif
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }
}

bool KanagramGame::isAnagram(QString& enteredword, QString& word)
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

QString KanagramGame::stripAccents(QString& original)
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

void KanagramGame::reloadSettings()
{
    loadSettings();
    if (refreshVocabularyList())
    {
        nextVocabulary();
        nextAnagram();
    }
}

void KanagramGame::loadSettings()
{
    QString correctAnswerScore = KanagramSettings::correctAnswerScore();

    m_correctAnswerScore = getNumericSetting(correctAnswerScore);
    m_correctAnswerScore = (m_correctAnswerScore + 1) * 5;

    QString incorrectAnswerScore = KanagramSettings::incorrectAnswerScore();

    m_incorrectAnswerScore = getNumericSetting(incorrectAnswerScore);
    m_incorrectAnswerScore = (m_incorrectAnswerScore + 1) * (-1);

    QString revealAnswerScore = KanagramSettings::revealAnswerScore();

    m_revealAnswerScore = getNumericSetting(revealAnswerScore);
    m_revealAnswerScore = (m_revealAnswerScore + 1) * (-2);

    QString skippedWordScore = KanagramSettings::skippedWordScore();

    m_skippedWordScore = getNumericSetting(skippedWordScore);
    m_skippedWordScore = (m_skippedWordScore + 1) * (-2);

    if (KanagramSettings::dataLanguage().isEmpty())
    {
        QStringList userLanguagesCode = QLocale::system().uiLanguages();
        QStringList sharedKvtmlFilesLanguages = SharedKvtmlFiles::languages();
        QString foundLanguage;
        foreach (const QString &userLanguageCode, userLanguagesCode)
        {
            if (sharedKvtmlFilesLanguages.contains(userLanguageCode))
            {
                foundLanguage = userLanguageCode;
                break;
            }
        }

        KanagramSettings::setDataLanguage(!foundLanguage.isEmpty() ? foundLanguage : "en");
    }
}

void KanagramGame::answerCorrect()
{
    if (m_currentPlayerNumber == 1)
        m_totalScore += m_correctAnswerScore;
    else
        m_totalScore2 += m_correctAnswerScore;
    emit scoreChanged();
}

void KanagramGame::answerIncorrect()
{
    if (m_currentPlayerNumber == 1)
        m_totalScore += m_incorrectAnswerScore;
    else
        m_totalScore2 += m_incorrectAnswerScore;
    emit scoreChanged();
}

void KanagramGame::answerRevealed()
{
    if (m_currentPlayerNumber == 1)
        m_totalScore += m_revealAnswerScore;
    else
        m_totalScore2 += m_revealAnswerScore;
    emit scoreChanged();
}

void KanagramGame::answerSkipped()
{
    if (m_currentPlayerNumber == 1)
        m_totalScore += m_skippedWordScore;
    else
        m_totalScore2 += m_skippedWordScore;
    emit scoreChanged();
}
