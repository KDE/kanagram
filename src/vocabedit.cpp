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

#include "vocabedit.h"

#include <qpushbutton.h>
#include <qlistbox.h>
#include <qlineedit.h>
#include <qfile.h>

#include <kstandarddirs.h>

#include "vocdata.h"

#include <iostream>
using namespace std;

VocabEdit::VocabEdit(QWidget *parent) : VocabEditWidget(parent)
{
	connect(btnSave, SIGNAL(clicked()), this, SLOT(slotSave()));
	connect(btnNewWord, SIGNAL(clicked()), this, SLOT(slotNewWord()));
	connect(btnRemoveWord, SIGNAL(clicked()), this, SLOT(slotRemoveWord()));
	connect(btnCancel, SIGNAL(clicked()), this, SLOT(slotCancel()));
	
	connect(txtWord, SIGNAL(textChanged(const QString &)), this, SLOT(slotWordTextChanged(const QString &)));
	connect(txtHint, SIGNAL(textChanged(const QString &)), this, SLOT(slotHintTextChanged(const QString &)));
	connect(lboxWords, SIGNAL(selectionChanged()), this, SLOT(slotSelectionChanged()));
}

VocabEdit::~VocabEdit()
{
}

void VocabEdit::slotSave()
{
	QFile file(txtVocabName->text() + ".kanagram");
	if (file.open(IO_WriteOnly))
	{
		QTextStream stream(&file);
		stream << (const char *)(txtVocabName->text()) << endl;
		stream << (const char *)(txtDescription->text()) << endl;
		for(int i = 0; i < m_vocabList.size(); i++)
		{
			stream << (const char *)(m_vocabList[i].getWord()) << endl;
			stream << (const char *)(m_vocabList[i].getHint()) << endl;
		}
	}
	close();
}

void VocabEdit::slotNewWord()
{
	lboxWords->insertItem("New Item");
	VocData data = VocData();
        data.setWord("New Item");
	m_vocabList.push_back(data);
}

void VocabEdit::slotSelectionChanged()
{
	cout << lboxWords->currentItem() << endl;
	disconnect(txtWord, SIGNAL(textChanged(const QString &)), this, SLOT(slotWordTextChanged(const QString &)));
	disconnect(txtHint, SIGNAL(textChanged(const QString &)), this, SLOT(slotHintTextChanged(const QString &)));
	if(lboxWords->currentItem() >= 0)
	{
		txtWord->setText(m_vocabList[lboxWords->currentItem()].getWord());
		txtHint->setText(m_vocabList[lboxWords->currentItem()].getHint());
	}
	connect(txtWord, SIGNAL(textChanged(const QString &)), this, SLOT(slotWordTextChanged(const QString &)));
	connect(txtHint, SIGNAL(textChanged(const QString &)), this, SLOT(slotHintTextChanged(const QString &)));
	cout << "The end.." << endl;
}

void VocabEdit::slotWordTextChanged(const QString &changes)
{
	m_vocabList[lboxWords->currentItem()].setWord(changes);
	//QListBoxItem *item = lboxWords->selectedItem();
	//item->setText(changes);
	lboxWords->changeItem(changes, lboxWords->currentItem()); 
}

void VocabEdit::slotHintTextChanged(const QString &changes)
{
	m_vocabList[lboxWords->currentItem()].setHint(changes);
}

void VocabEdit::slotRemoveWord()
{
	m_vocabList.erase(m_vocabList.begin() + lboxWords->currentItem());
	lboxWords->removeItem(lboxWords->currentItem());
}

void VocabEdit::slotCancel()
{
	close();
}
