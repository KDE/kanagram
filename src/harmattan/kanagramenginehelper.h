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

#include <QtCore/QStringList>

class KanagramEngineHelper : public QObject
{
	Q_OBJECT
	public:
        explicit KanagramEngineHelper(KanagramGame *kanagramGame, QObject* parent = 0);
        ~KanagramEngineHelper();
		
        Q_INVOKABLE QStringList createNextAnagram();
        Q_INVOKABLE QStringList insertInCurrentOriginalWord(int index, const QString& letter);
        Q_INVOKABLE QStringList anagramOriginalWord() const;

	private:
        KanagramGame *m_kanagramGame;
        QStringList m_currentOriginalWord;
};

#endif // KANAGRAM_ENGINE_HELPER_H
