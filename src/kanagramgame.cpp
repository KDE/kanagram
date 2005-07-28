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

#include <kurl.h>
#include <kdebug.h>

#include "keduvocdocument.h"
#include "kanagramsettings.h"


KanagramGame::KanagramGame() : m_index(0)
{
	//m_fileList.append(KanagramSettings::defaultVocab());
	//KEduVocDocument *doc = new KEduVocDocument(this);
	//doc->open(KURL(locate("appdata", m_fileList[m_index])), false);
	//m_docTitle = doc->getTitle();
	nextAnagram();
}

KanagramGame::~KanagramGame()
{
}

void KanagramGame::previousVocab()
{
	m_index--;
	m_fileList = KGlobal::dirs()->findAllResources("appdata", "data/*.kvtml");
	if(m_index < 0)
		m_index = m_fileList.size() - 1;
	KEduVocDocument *doc = new KEduVocDocument(this);
	doc->open(KURL(locate("appdata", m_fileList[m_index])), false);
	m_docTitle = doc->getTitle();
}

void KanagramGame::nextVocab()
{
	if(!m_fileList.empty())
		m_index++;
	m_fileList = KGlobal::dirs()->findAllResources("appdata", "data/*.kvtml");
	if(m_index >= m_fileList.size())
		m_index = 0;
	KEduVocDocument *doc = new KEduVocDocument(this);
	doc->open(KURL(locate("appdata", m_fileList[m_index])), false);
	m_docTitle = doc->getTitle();
}

QString KanagramGame::getDocTitle()
{
	return m_docTitle;
}

QString KanagramGame::getFilename()
{
	if(m_fileList.empty())
		return m_filename;
	else
		return m_fileList[m_index];
}

void KanagramGame::nextAnagram()
{
	KEduVocDocument	*doc = new KEduVocDocument(this);
	if(m_fileList.empty())
	{
		doc->open(KURL(KanagramSettings::defaultVocab()), false);
		m_docTitle = doc->getTitle();
		m_filename = KanagramSettings::defaultVocab();
	}
	else
		doc->open(KURL(locate("appdata", m_fileList[m_index])), false);
	int totalWords = doc->numEntries();
	int wordNumber = m_random.getLong(totalWords);
	while(m_anagram == doc->getEntry(wordNumber)->getOriginal())
	{
		wordNumber = m_random.getLong(totalWords);
	}
	m_originalWord = doc->getEntry(wordNumber)->getOriginal();
	m_anagram = createAnagram(m_originalWord);
	m_hint = doc->getEntry(wordNumber)->getRemark(0);
}

QString KanagramGame::getAnagram()
{
	return m_anagram;
}

QString KanagramGame::getHint()
{
	return m_hint;
}

QString KanagramGame::getWord()
{
	return m_originalWord;
}

void KanagramGame::restoreWord()
{
	m_anagram = m_originalWord;
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
