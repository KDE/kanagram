/***************************************************************************
 *   Copyright (C) 2005 by Joshua Keel <joshuakeel@gmail.com>              *
 *             (C) 2007-2014 by Jeremy Whiting <jpwhiting@kde.org>         *
 *             (C) 2012 by Laszlo Papp <lpapp@kde.org>                     *
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

#ifndef KANAGRAMGAME_H
#define KANAGRAMGAME_H

#include <QtCore/QObject>
#include <QtCore/QStringList>
#include <QtCore/QHash>
#include <QtCore/QUrl>

#include <krandomsequence.h>

#ifdef BUILD_WITH_SPEECH
#include "kspeechinterface.h"
#endif

class KEduVocDocument;

namespace Phonon {
    class MediaObject;
}

namespace Sonnet {
    class Speller;
}
/** @brief game api
 * @author Joshua Keel <joshuakeel@gmail.com>
 * @author Jeremy Whiting <jpwhiting@kde.org>
 */
class KanagramGame : public QObject
{
    Q_OBJECT
    // Get the current anagram, word, hint, picture, and audio
    Q_PROPERTY(QString anagram READ anagram NOTIFY wordChanged)
    Q_PROPERTY(QString word READ word NOTIFY wordChanged)
    Q_PROPERTY(QString hint READ hint NOTIFY wordChanged)
    Q_PROPERTY(QUrl picHint READ picHint NOTIFY wordChanged)
    Q_PROPERTY(QUrl audio READ audioFile NOTIFY wordChanged)

    // Get the current title
    Q_PROPERTY(QString title READ documentTitle NOTIFY titleChanged)

    // Get list of vocabularies (document titles), language names, and current language)
    Q_PROPERTY(QStringList vocabularies READ vocabularyList)
    Q_PROPERTY(QStringList languages READ languageNames)
    Q_PROPERTY(QString dataLanguage READ dataLanguage WRITE setDataLanguage NOTIFY dataLanguageChanged)

    Q_PROPERTY(int score READ totalScore NOTIFY scoreChanged)

    public:
        /** Default constructor */
        KanagramGame();

        /** Default destructor */
        ~KanagramGame();

        /** Get the anagram to show */
        QString anagram() const;

        /** Get this anagram's hint */
        QString hint() const;

        /** Get this anagram's answer */
        QString word() const;

        /** Get this anagram's picture hint URL */
        QUrl picHint();

        /** Get this anagram's audio URL */
        QUrl audioFile();

        /** Get the current vocabulary file's title */
        QString documentTitle() const;

        /** Get the current vocabulary file's filename */
        QString filename() const;

        /** Get the list of vocabularies */
        QStringList vocabularyList() const;

        /** Return the language names found available in the system */
        QStringList languageNames();

        /** Get the current data language */
        QString dataLanguage() const;

        /** Get the sanitized data language used */
        QString sanitizedDataLanguage() const;

        // These accessor and mutator methods are not needed once the
        // kconfig_compiler can generate Q_INVOKABLE methods, slots or/and
        // properties

        Q_INVOKABLE int hintHideTime();

        Q_INVOKABLE int resolveTime();

        Q_INVOKABLE int scoreTime();

        /** Get the current score */
        int totalScore();

        /** Check word answer against the current word */
        Q_INVOKABLE bool checkWord(QString answer);

    public Q_SLOTS:

        /** Set the vocabulary to use according to the vocabulary name */
        void useVocabulary(const QString &vocabularyname);

        /** Set the vocabulary to use according to the desired index value*/
        void useVocabulary(int index);

        /** Set the data language */
        void setDataLanguage(const QString& dataLanguage);

        /** Refresh the list of vocabulary files available
         * from what we find on disk
         *@returns true if the current vocabulary has changed so the ui can refresh
         */
        bool refreshVocabularyList();

        /** Load the default vocabulary file */
        void loadDefaultVocabulary();

        /** Set the index to the next word */
        void nextAnagram();

        /** Use the next vocabulary file in the list */
        void nextVocabulary();

        /** Use the previous vocabulary file in the list */
        void previousVocabulary();

        /** The word was revealed (or correctly entered), so play the audio, say it, or play right.ogg */
        void wordRevealed();

        /** Reset the current score */
        void resetTotalScore();

        /** Adjust the current score by points */
        void adjustScore(int points);

        void reloadSettings();

        /** Slots to adjust score accordingly */
        void answerCorrect();
        void answerIncorrect();
        void answerSkipped();
        void answerRevealed();

    Q_SIGNALS:

        /** Signal the ui that a there's a file error of some kind */
        void fileError(const QString &filename);

        /** Signal the ui that the data language has changed */
        void dataLanguageChanged();

        /** Signal the ui that the current document title has changed */
        void titleChanged();

        /** Signal the ui that the anagram, word, hint, picHint, and audioUrl changed */
        void wordChanged();

        /** Signal the ui that the score has changed */
        void scoreChanged();
    private:

        /** Make the word into an anagram */
        void createAnagram();

        /** Check the current file */
        bool checkFile();

        /** Play a media file via Phonon */
        void play(const QString &filename);

#ifdef BUILD_WITH_SPEECH
        /** setup kde text-to-speech daemon */
        void setupJovie();

        /** speak the word
         *@param text the word that is to be converted from text to speech
         */
        void say(QString text);
#endif

        /** Get the value of a numeric setting from it's string */
        int getNumericSetting(QString settingString);

        /** Check if enteredword is an anagram of word */
        bool isAnagram(QString& enteredword, QString& word);

        /** Remove accent characters from a word */
        QString stripAccents(QString& original);

        /** Load score settings into local variables */
        void loadSettings();

        /** Random sequence used to scramble the letters */
        KRandomSequence m_random;

        /** The current scrambled word */
        QString m_anagram;

        /** The current anagram's hint */
        QString m_hint;

        /** The current anagram's picture if any */
        QUrl m_picHintUrl;

        /** The current anagram's audio if any */
        QUrl m_audioUrl;

        /** The current anagram's answer */
        QString m_originalWord;

        /** The list of vocabulary files */
        QStringList m_fileList;

        /** Which index the current filename is in m_fileList */
        int m_fileIndex;

        /** The list of words that have been answered */
        QStringList m_answeredWords;

        /** The current vocabulary's filename */
        QString m_filename;

        /** The current document */
        KEduVocDocument* m_document;

        /** The hash of the language code and name */
        QHash<QString, QString> m_languageCodeNameHash;

        /** audio player to use for playing sounds */
        Phonon::MediaObject *m_player;

#ifdef BUILD_WITH_SPEECH
        /** object to enable text to speech conversion */
        org::kde::KSpeech *m_kspeech;
#endif

        /** current total score */
        int m_totalScore;

        /** Speller object to check correct spelling */
        Sonnet::Speller *m_speller;

        /** Values for settings */
        int m_correctAnswerScore;
        int m_incorrectAnswerScore;
        int m_revealAnswerScore;
        int m_skippedWordScore;
};

#endif

