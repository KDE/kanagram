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

#ifndef KANAGRAM_ENGINE_HELPER_H
#define KANAGRAM_ENGINE_HELPER_H

#include <kanagramgame.h>
#include <sonnet/speller.h>

#include <KConfigDialog>
#include <KAction>
#include <KActionCollection>
#include <KShortcutsEditor>
#include <kanagramsettings.h>
#include "mainsettings.h"
#include "vocabsettings.h"

namespace Phonon
{
    class MediaObject;
}

class KanagramEngineHelper : public QObject
{
    Q_OBJECT
    Q_PROPERTY( int hintHideTime READ hintHideTime WRITE setHintHideTime NOTIFY hintHideTimeChanged )
    Q_PROPERTY( int resolveTime READ resolveTime WRITE setResolveTime NOTIFY resolveTimeChanged )
    Q_PROPERTY( bool useSounds READ useSounds WRITE setUseSounds NOTIFY useSoundsToggled )
    Q_PROPERTY( QString defaultVocabulary READ defaultVocabulary WRITE setDefaultVocabulary NOTIFY defaultVocabularyChanged )

	public:
        explicit KanagramEngineHelper(KanagramGame *kanagramGame, QObject* parent = 0);
        ~KanagramEngineHelper();

        Q_INVOKABLE QString createNextAnagram();
        Q_INVOKABLE QStringList insertInCurrentOriginalWord(int index, const QString& letter);
        Q_INVOKABLE QStringList removeInCurrentOriginalWord(int index);
        Q_INVOKABLE QString anagramOriginalWord() const;
        Q_INVOKABLE QString showHint() const;
        Q_INVOKABLE QString categoryName() const;
        Q_INVOKABLE QString nextVocabulary();
        Q_INVOKABLE QString previousVocabulary();
        Q_INVOKABLE bool checkWord(QString answer);
        bool isAnagram(QString& enteredword, QString& word);
        int getNumericSetting(QString settingString);
        void loadSettings();
        void refreshVocabularies();
        Q_INVOKABLE void reloadSettings();
        Q_INVOKABLE void slotSaveSettings();
        Q_INVOKABLE void slotSettingsCancelled();
        Q_INVOKABLE void slotEnableApplyButton();
        void play(const QString &filename);
        QString stripAccents(QString& original);
        Q_INVOKABLE bool compareWords() const;

        /** invoke the settings dialog */
        Q_INVOKABLE void slotShowSettings();

        // These accessor and mutator methods are not needed once the
        // kconfig_compiler can generate Q_INVOKABLE methods, slots or/and
        // properties

        int hintHideTime();
        void setHintHideTime(int hintHideTime);

        int resolveTime();
        void setResolveTime(int resolveTime);

        bool useSounds();
        void setUseSounds(bool useSounds);

        QString defaultVocabulary();
        void setDefaultVocabulary(const QString& defaultVocabulary);

    public Q_SLOTS:
        void saveSettings();

    Q_SIGNALS:
        void hintHideTimeChanged();
        void resolveTimeChanged();
        void useSoundsToggled();
        void defaultVocabularyChanged();

	private:
        KanagramGame *m_kanagramGame;
        Sonnet::Speller *m_speller;
        KActionCollection * m_actionCollection;

        /** audio player to use for playing sounds */
        Phonon::MediaObject *m_player;

        /** settings dialog */
        KConfigDialog *m_configDialog;

        /** settings page pointers */
        VocabSettings *m_vocabSettings;
        KShortcutsEditor *m_shortcutsEditor;

        QStringList m_currentOriginalWord;
        int m_insertCounter;

        /** Values for settings */
        int m_hintHideTime;
        int m_resolveTime;
        int m_scoreTime;
        int m_timeLeft;
        int m_totalScore;
        int m_correctAnswerScore;
        int m_incorrectAnswerScore;
        int m_revealAnswerScore;
        int m_skippedWordScore;
        bool m_useSounds;
        bool m_enablePronunciation;
};

#endif // KANAGRAM_ENGINE_HELPER_H
