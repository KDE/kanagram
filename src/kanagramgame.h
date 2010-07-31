/***************************************************************************
 *   Copyright (C) 2005 by Joshua Keel                                     *
 *   joshuakeel@gmail.com                                                  *
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
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.             *
 ***************************************************************************/

#ifndef KANAGRAMGAME_H
#define KANAGRAMGAME_H

#include <tqobject.h>

#include <kstandarddirs.h>
#include <krandomsequence.h>

class QWidget;

class KanagramGame : public QObject
{
Q_OBJECT
	public:
		KanagramGame(TQWidget *parent);
		~KanagramGame();
		void refreshVocabList();
		void loadDefaultVocab();
		int findIndex();
		void nextAnagram();
		void nextVocab();
		void previousVocab();
		TQString getAnagram();
		TQString getHint();
		TQString getWord();
		void restoreWord();
		TQString getDocTitle();
		TQString getFilename();
	private:
		TQString createAnagram(TQString original);
		void checkFile();
		TQWidget *m_parent;
		KRandomSequence m_random;
		TQString m_anagram;
		TQString m_hint;
		TQString m_originalWord;

		int m_index;
		TQStringList m_fileList;
		TQStringList m_answeredWords;
		TQString m_docTitle;
		TQString m_filename;
};

#endif
