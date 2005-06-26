/***************************************************************************
 *   Copyright (C) 2005 by Joshua Keel                                     *
 *   joshuakeel@gmail.com                                                  *
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
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include "kanagramgame.h"
#include <iostream>

using namespace std;

KanagramGame::KanagramGame()
{
	nextAnagram();
}

KanagramGame::~KanagramGame()
{
}

void KanagramGame::nextAnagram()
{
	KEduVocDataItemList wordList = KEduVocData::parse(locate("appdata", "data/data.kvtml"));
	int totalWords = wordList.count();
	int wordNumber = m_random.getLong(totalWords);
	while(m_anagram == wordList[wordNumber].originalText())
	{
		wordNumber = m_random.getLong(totalWords);
	}
	m_originalWord = wordList[wordNumber].originalText();
	m_anagram = createAnagram(m_originalWord);
	m_hint = wordList[wordNumber].translatedText();

	cout << "Original word: " << m_originalWord << endl;
}

QString KanagramGame::getAnagram()
{
	return m_anagram;
}

QString KanagramGame::getHint()
{
	return m_hint;
}

QString KanagramGame::createAnagram(QString original)
{
	QStringList objData = QStringList::split(QString(""), original);
	QString insaneData;
	int count;
	
	for(int i=0; count = objData.count(); i++)
	{
		int objChunk;
		if((i == 0) && (count > 1))
		   objChunk = 1 + m_random.getLong(count - 1);
		else
		   objChunk = m_random.getLong(count);

		QStringList::Iterator it = objData.at(objChunk);
		QString sd = *it;
		objData.remove(it);
		if (insaneData.isEmpty())
			insaneData = sd;
		else
			insaneData += sd;
	}
	return insaneData;
}

#include "kanagramgame.moc"
