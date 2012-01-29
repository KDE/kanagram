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
	public:
        explicit KanagramEngineHelper(KanagramGame *kanagramGame, QObject* parent = 0);
        ~KanagramEngineHelper();

        Q_INVOKABLE QStringList createNextAnagram();
        Q_INVOKABLE QStringList insertInCurrentOriginalWord(int index, const QString& letter);
        Q_INVOKABLE QStringList anagramOriginalWord() const;
        Q_INVOKABLE bool compareWords() const;

        // These accessor and mutator methods are not needed once the
        // kconfig_compiler can generate Q_INVOKABLE methods or/and slots

        Q_INVOKABLE QString hintHideTime();
        Q_INVOKABLE void setHintHideTime(const QString& hintHideTime);

        Q_INVOKABLE QString resolveTime();
        Q_INVOKABLE void setResolveTime(const QString& resolveTime);

        Q_INVOKABLE bool useSounds();
        Q_INVOKABLE void setUseSounds(bool useSounds);

        Q_INVOKABLE QString defaultVocabulary();
        Q_INVOKABLE void setDefaultVocabulary(const QString& defaultVocabulary);

        Q_INVOKABLE QString dataLanguage();
        Q_INVOKABLE void setDataLanguage(const QString& dataLanguage);

	private:

        KanagramGame *m_kanagramGame;
        QStringList m_currentOriginalWord;

        bool m_useSounds;
};

#endif // KANAGRAM_ENGINE_HELPER_H
