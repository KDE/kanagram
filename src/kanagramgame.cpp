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

#include <QFile>
#include <QFileInfo>

#include <kurl.h>
#include <kdebug.h>
#include <kmessagebox.h>
#include <kstandarddirs.h>
#include <klocale.h>

#include <sharedkvtmlfiles.h>
#include <keduvocdocument.h>
#include <keduvocexpression.h>

#include "kanagramsettings.h"


KanagramGame::KanagramGame() : m_index(0)
{
	// first get the list of vocabularies
	refreshVocabList();
	
	// then load the default vocab
	loadDefaultVocab();
}

KanagramGame::~KanagramGame()
{
}

void KanagramGame::checkFile()
{
	if (!QFile::exists(KStandardDirs::locate("data", m_filename))) 
	{
		emit fileError(m_filename);
	}
}

void KanagramGame::loadDefaultVocab()
{
	m_filename = KanagramSettings::defaultVocab();
	if (m_filename.isEmpty() || !QFileInfo(m_filename).exists())
	{
        refreshVocabList();
        nextVocab();
	}

	KEduVocDocument *doc = new KEduVocDocument(this);
	doc->open(KUrl(KStandardDirs::locate("data", m_filename)));
	m_docTitle = doc->title();
	nextAnagram();
}

bool KanagramGame::refreshVocabList()
{
	bool retval = false;
	QString oldFilename = m_filename;
	m_fileList = SharedKvtmlFiles::fileNames(KanagramSettings::dataLanguage());
	useVocab(m_docTitle);
    return oldFilename != m_filename;
}

/** get the list of vocabularies */
QStringList KanagramGame::getVocabsList()
{
	return SharedKvtmlFiles::titles(KanagramSettings::dataLanguage());
}
		
/** set the vocab to use */
void KanagramGame::useVocab(const QString &vocabname)
{
	QStringList titles = getVocabsList();
	int vocab = titles.indexOf(vocabname);
	if (vocab > 0)
	{
		m_index = vocab;
		m_filename = m_fileList[vocab];
	}
    else
    {
        m_index = 0;
        m_filename = m_fileList[0];
    }
}

void KanagramGame::updateIndex()
{
	m_index = 0;
	for (int i = 0; i < m_fileList.size(); i++)
	{
		if (m_filename == m_fileList[i])
		{
			m_index = i;
		}
	}
}

void KanagramGame::previousVocab()
{
	if (--m_index < 0)
    {
		m_index = m_fileList.size() - 1;
    }

	m_filename = m_fileList[m_index];
	checkFile();
	KEduVocDocument *doc = new KEduVocDocument(this);
	doc->open(KUrl(KStandardDirs::locate("data", m_filename)));
	m_docTitle = doc->title();
	m_answeredWords.clear();
}

void KanagramGame::nextVocab()
{
	if (++m_index >= m_fileList.size())
    {
		m_index = 0;
    }

	if (!m_fileList.isEmpty())
    {
        m_filename = m_fileList[m_index];
        checkFile();
        KEduVocDocument *doc = new KEduVocDocument(this);
        doc->open(KUrl(KStandardDirs::locate("data", m_filename)));
        m_docTitle = doc->title();
        m_answeredWords.clear();
    }
}

void KanagramGame::nextAnagram()
{
	checkFile();
	KEduVocDocument	*doc = new KEduVocDocument(this);
	doc->open(KUrl(KStandardDirs::locate("data", m_filename)));
	int totalWords = doc->entryCount();
	int wordNumber = m_random.getLong(totalWords);

	if (doc->entryCount() == (int)m_answeredWords.size())
	{
		m_answeredWords.clear();
	}

	while (m_answeredWords.indexOf(doc->entry(wordNumber)->translation(0).translation()) != -1)
	{
		wordNumber = m_random.getLong(totalWords);
	}

	m_originalWord = doc->entry(wordNumber)->translation(0).translation();
	m_answeredWords.append(m_originalWord);
	createAnagram();
	m_hint = doc->entry(wordNumber)->translation(0).comment();
}

QString KanagramGame::getDocTitle()
{
	return m_docTitle;
}

QString KanagramGame::getFilename()
{
    return m_fileList.empty() ? m_filename : m_fileList[m_index];
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

void KanagramGame::createAnagram()
{
	QStringList objData = m_originalWord.split(QString(""));
	QString insaneData = "";
	int count;

	for (int i = 0; (count = objData.count()); i++)
	{
		int objChunk;
		if ((i == 0) && (count > 1))
        {
			objChunk = 1 + m_random.getLong(count - 1);
        }
		else
        {
			objChunk = m_random.getLong(count);
        }

		QString sd = objData.at(objChunk);
		objData.removeAt(objChunk);
		insaneData += sd;
	}
	m_anagram = insaneData;
}

#include "kanagramgame.moc"
