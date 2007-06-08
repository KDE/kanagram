/***************************************************************************
 *   Copyright (C) 2005 by Joshua Keel <joshuakeel@gmail.com>              *
 *             (C) 2007 by Jeremy Whiting <jeremy@scitools.com>            *
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
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.             *
 ***************************************************************************/

#include "kanagramgame.h"

#include <qfile.h>

#include <kurl.h>
#include <kdebug.h>
#include <kmessagebox.h>
#include <klocale.h>

#include "keduvocdocument.h"
#include "keduvocexpression.h"
#include "kanagramsettings.h"


KanagramGame::KanagramGame() : m_index(0)
{
	loadDefaultVocab();
}

KanagramGame::~KanagramGame()
{
}

void KanagramGame::checkFile()
{
	if (!QFile::exists(KStandardDirs::locate("appdata", m_filename))) {
		emit fileError(m_filename);
	}
}

void KanagramGame::loadDefaultVocab()
{
	m_filename = KanagramSettings::defaultVocab();
	if (m_filename.isEmpty())
	{
		// first run
		m_filename = KStandardDirs::locate("appdata", "data/" + KanagramSettings::dataLanguage() + "/objects.kvtml");
		if (m_filename.isEmpty())
		{
			refreshVocabList();
			nextVocab();
		}
	}
	
        kDebug() << "in game " << m_filename <<endl;
	KEduVocDocument *doc = new KEduVocDocument(this);
	doc->open(KUrl(KStandardDirs::locate("appdata", m_filename)));
	m_docTitle = doc->title();
        kDebug() << m_docTitle <<endl; //Animals
	nextAnagram();
}

void KanagramGame::refreshVocabList()
{
	m_fileList = KGlobal::dirs()->findAllResources("appdata", "data/" + KanagramSettings::dataLanguage() + "/*.kvtml");
	//nextVocab();
	m_index = findIndex();
}

int KanagramGame::findIndex()
{
        //this m_filename is wrong
        //you have to use KanagramSettings::defaultVocab() instead of m_filename which is used for something else
        kDebug() <<"m_filename " << m_filename << "\n" << endl;
	int tempIndex = 0;
	for(int i = 0; i < m_fileList.size(); i++)
	{
                kDebug() <<"m_file " << m_fileList[i]<<endl;
		if(m_filename == m_fileList[i])
		{
			tempIndex = i;
		}
	}
        kDebug() << "index founded " << tempIndex <<endl;
	return tempIndex;
}

void KanagramGame::previousVocab()
{
	m_index--;
	if(m_index < 0)
		m_index = m_fileList.size() - 1;
	m_filename = m_fileList[m_index];
	checkFile();
	KEduVocDocument *doc = new KEduVocDocument(this);
	doc->open(KUrl(KStandardDirs::locate("appdata", m_filename)));
	m_docTitle = doc->title();
	m_answeredWords.clear();
}

void KanagramGame::nextVocab()
{
	m_index++;
	if(m_index >= m_fileList.size())
		m_index = 0;
	if( m_fileList.isEmpty())
			return;
	m_filename = m_fileList[m_index];
	checkFile();
	KEduVocDocument *doc = new KEduVocDocument(this);
	doc->open(KUrl(KStandardDirs::locate("appdata", m_filename)));
	m_docTitle = doc->title();
	m_answeredWords.clear();
}

void KanagramGame::nextAnagram()
{
	checkFile();
	KEduVocDocument	*doc = new KEduVocDocument(this);
	doc->open(KUrl(KStandardDirs::locate("appdata", m_filename)));
	int totalWords = doc->entryCount();
	int wordNumber = m_random.getLong(totalWords);
	if(doc->entryCount() == (int)m_answeredWords.size())
	{
		m_answeredWords.clear();
	}
	while(m_answeredWords.indexOf(doc->entry(wordNumber)->original()) != -1)
	{
		wordNumber = m_random.getLong(totalWords);
	}
	m_originalWord = doc->entry(wordNumber)->original();
	m_answeredWords.append(m_originalWord);
	m_anagram = createAnagram(m_originalWord);
	m_hint = doc->entry(wordNumber)->remark(0);
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

QString KanagramGame::createAnagram(const QString &original)
{
	QStringList objData = original.split(QString(""));
	QString insaneData = "";
	int count;
	
	for(int i=0; (count = objData.count()); i++)
	{
		int objChunk;
		if((i == 0) && (count > 1))
			objChunk = 1 + m_random.getLong(count - 1);
		else
			objChunk = m_random.getLong(count);

		QString sd = objData.at(objChunk);
		objData.removeAt(objChunk);
		insaneData += sd;
	}
	return insaneData;
}

#include "kanagramgame.moc"
