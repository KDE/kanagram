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
        Q_INVOKABLE bool compareWords() const;

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
        QStringList m_currentOriginalWord;
        int m_insertCounter;
};

#endif // KANAGRAM_ENGINE_HELPER_H
