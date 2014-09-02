/***************************************************************************
 *   This file is part of the Kanagram project                             *
 *   Copyright (c) 2012 Laszlo Papp <lpapp@kde.org>                        *
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


#ifndef KANAGRAM_ENGINE_HELPER_H
#define KANAGRAM_ENGINE_HELPER_H

#include <kanagramgame.h>
#include <sonnet/speller.h>

class KConfigDialog;
class KHelpMenu;
class VocabSettings;

#include "mainsettings.h"
#include "vocabsettings.h"
#ifdef BUIlD_WITH_SPEECH
#include "kspeechinterface.h"
#endif

namespace Phonon
{
    class MediaObject;
}

class KanagramEngineHelper : public QObject
{
    Q_OBJECT
    public:
        explicit KanagramEngineHelper(KanagramGame *kanagramGame, QObject* parent = 0);
        ~KanagramEngineHelper();

        Q_INVOKABLE QString createNextAnagram();
        Q_INVOKABLE QStringList insertInCurrentOriginalWord(int index, const QString& letter);
        Q_INVOKABLE QStringList removeInCurrentOriginalWord(int index);
        Q_INVOKABLE QString anagramOriginalWord();
        Q_INVOKABLE QString showHint() const;
        Q_INVOKABLE QString categoryName() const;
        Q_INVOKABLE QString nextVocabulary();
        Q_INVOKABLE QString previousVocabulary();
        Q_INVOKABLE bool checkWord(QString answer);
        bool isAnagram(QString& enteredword, QString& word);
        int getNumericSetting(QString settingString);
        void loadSettings();
        void refreshVocabularies();
        void play(const QString &filename);
        QString stripAccents(QString& original);
        Q_INVOKABLE bool compareWords() const;
        Q_INVOKABLE void resetTotalScore();
        Q_INVOKABLE void increaseScore(int points);
        Q_INVOKABLE int totalScore();
        Q_INVOKABLE int correctAnswerScore();
        Q_INVOKABLE int incorrectAnswerScore();
        Q_INVOKABLE int skippedWordScore();
        Q_INVOKABLE int revealAnswerScore();

#ifdef BUILD_WITH_SPEECH
        /** setup kde text-to-speech daemon */
        void setupJovie();

        /** speak the word
         *@param text the word that is to be converted from text to speech
         */
        void say(QString text);
#endif

        // These accessor and mutator methods are not needed once the
        // kconfig_compiler can generate Q_INVOKABLE methods, slots or/and
        // properties

        Q_INVOKABLE int hintHideTime();
        void setHintHideTime(int hintHideTime);

        int resolveTime();
        void setResolveTime(int resolveTime);

        Q_INVOKABLE int scoreTime();

        bool useSounds();
        void setUseSounds(bool useSounds);

    public Q_SLOTS:
        void reloadSettings();

    Q_SIGNALS:
        void hintHideTimeChanged();
        void resolveTimeChanged();
        void useSoundsToggled();
        void defaultVocabularyChanged();

    private:
        KanagramGame *m_kanagramGame;
        Sonnet::Speller *m_speller;

        /** audio player to use for playing sounds */
        Phonon::MediaObject *m_player;

#ifdef BUILD_WITH_SPEECH
        /** object to enable text to speech conversion */
        org::kde::KSpeech *m_kspeech;
#endif

        QStringList m_currentOriginalWord;
        int m_insertCounter;

        /** Values for settings */
        int m_hintHideTime;
        int m_resolveTime;
        int m_scoreTime;
        int m_totalScore;
        int m_correctAnswerScore;
        int m_incorrectAnswerScore;
        int m_revealAnswerScore;
        int m_skippedWordScore;
        bool m_useSounds;
        bool m_enablePronunciation;
};

#endif // KANAGRAM_ENGINE_HELPER_H
