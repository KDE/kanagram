/***************************************************************************
 *   Copyright (C) 2005 by Joshua Keel                                     *
 *   joshuakeel@gmail.com                                                  *
 *                                                                         *
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

#include "vocdatautils.h"

#include <qstringlist.h>

#include <iostream>
using namespace std;


QStringList VocDataUtils::parseHeader(const QString &fileName)
{
	QFile file(fileName);
	QStringList header;
	if(file.open(IO_ReadOnly))
	{
		QTextStream stream(&file);
		header.append(stream.readLine());
		header.append(stream.readLine());
	}
	return header;
}

QValueVector<VocData> VocDataUtils::parseWords(const QString &fileName)
{
	cout << (const char*) fileName << endl;
	QValueVector<VocData> wordList;
	VocData data = VocData();
	QFile file(fileName);
	if (file.open(IO_ReadOnly))
	{
		QTextStream stream(&file);
		int i = 0;
		QString line;
		cout << "This is weird.." << endl;
		while (!stream.atEnd())
		{
			data.setWord(stream.readLine());
			data.setHint(stream.readLine());
			wordList.push_back(data);	
		}
		wordList.erase(wordList.begin());
	}
	return wordList;
}