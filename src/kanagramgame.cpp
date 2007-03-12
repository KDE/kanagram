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
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.             *
 ***************************************************************************/

#include <qfile.h>

#include <kurl.h>
#include <kdebug.h>
#include <kmessagebox.h>
#include <kapplication.h>
#include <klocale.h>

#include "kanagramgame.h"
#include "keduvocdocument.h"
#include "kanagramsettings.h"

#include <stdlib.h>

KanagramGame::KanagramGame(QWidget* parent) : m_index(0)
{
	m_parent = parent;
	loadDefaultVocab();
}

KanagramGame::~KanagramGame()
{
}

void KanagramGame::checkFile()
{
	if (!QFile::exists(locate("appdata", m_filename))) {
	        QString msg = i18n("File %1 cannot be found.\n Please ensure that Kanagram is properly installed.")
		  .arg(m_filename);
		KMessageBox::sorry(m_parent, msg, i18n("Error"));
		exit(0);
	}
}

void KanagramGame::loadDefaultVocab()
{
	m_filename = KanagramSettings::defaultVocab();
	if (m_filename.isEmpty())
	{
		// first run
		m_filename = locate("appdata", "data/" + KanagramSettings::dataLanguage() + "/objects.kvtml");
		if (m_filename.isEmpty())
		{
			refreshVocabList();
			nextVocab();
		}
	}
	
        kdDebug() << "in game " << m_filename <<endl;
	KEduVocDocument *doc = new KEduVocDocument(this);
	doc->open(KURL(locate("appdata", m_filename)), false);
	m_docTitle = doc->getTitle();
        kdDebug() << m_docTitle <<endl; //Animals
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
        kdDebug() <<"m_filename " << m_filename << "\n" << endl;
	int tempIndex = 0;
	for(uint i = 0; i < m_fileList.size(); i++)
	{
                kdDebug() <<"m_file " << m_fileList[i]<<endl;
		if(m_filename == m_fileList[i])
		{
			tempIndex = i;
		}
	}
        kdDebug() << "index founded " << tempIndex <<endl;
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
	doc->open(KURL(locate("appdata", m_filename)), false);
	m_docTitle = doc->getTitle();
	m_answeredWords.clear();
}

void KanagramGame::nextVocab()
{
	m_index++;
	if((uint)m_index >= m_fileList.size())
		m_index = 0;
	m_filename = m_fileList[m_index];
	checkFile();
	KEduVocDocument *doc = new KEduVocDocument(this);
	doc->open(KURL(locate("appdata", m_filename)), false);
	m_docTitle = doc->getTitle();
	m_answeredWords.clear();
}

void KanagramGame::nextAnagram()
{
	checkFile();
	KEduVocDocument	*doc = new KEduVocDocument(this);
	doc->open(KURL(locate("appdata", m_filename)), false);
	int totalWords = doc->numEntries();
	int wordNumber = m_random.getLong(totalWords);
	if(doc->numEntries() == (int)m_answeredWords.size())
	{
		m_answeredWords.clear();
	}
	while(m_answeredWords.findIndex(doc->getEntry(wordNumber)->getOriginal()) != -1)
	{
		wordNumber = m_random.getLong(totalWords);
	}
	m_originalWord = doc->getEntry(wordNumber)->getOriginal();
	m_answeredWords.append(m_originalWord);
	m_anagram = createAnagram(m_originalWord);
	m_hint = doc->getEntry(wordNumber)->getRemark(0);
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
