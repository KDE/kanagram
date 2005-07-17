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
#include <qstring.h>
#include <qvaluevector.h>

#include <kstandarddirs.h>
#include <kglobal.h>
#include <kurl.h>
#include <kdebug.h>

#include "keduvocdocument.h"
#include "keduvocexpression.h"


VocabEdit::VocabEdit(QWidget *parent) : VocabEditWidget(parent)
{
	connect(btnSave, SIGNAL(clicked()), this, SLOT(slotSave()));
	connect(btnNewWord, SIGNAL(clicked()), this, SLOT(slotNewWord()));
	connect(btnRemoveWord, SIGNAL(clicked()), this, SLOT(slotRemoveWord()));
	connect(btnClose, SIGNAL(clicked()), this, SLOT(accept()));
	
	connect(txtWord, SIGNAL(textChanged(const QString &)), this, SLOT(slotWordTextChanged(const QString &)));
	connect(txtHint, SIGNAL(textChanged(const QString &)), this, SLOT(slotHintTextChanged(const QString &)));
	connect(lboxWords, SIGNAL(selectionChanged()), this, SLOT(slotSelectionChanged()));
}

VocabEdit::VocabEdit(QWidget *parent, QString fileName = "") : VocabEditWidget(parent)
{
	m_fileName = fileName;
	KEduVocDocument	*doc = new KEduVocDocument(this);
	doc->open(KURL(m_fileName), false);
	kdDebug() << doc->numEntries() << endl;
	for(int i = 0; i < doc->numEntries(); i++)
	{
		KEduVocExpression expr = *doc->getEntry(i);
		m_vocabList.append(expr);
		lboxWords->insertItem(doc->getEntry(i)->getOriginal());	
	}

	connect(btnSave, SIGNAL(clicked()), this, SLOT(slotSave()));
	connect(btnNewWord, SIGNAL(clicked()), this, SLOT(slotNewWord()));
	connect(btnRemoveWord, SIGNAL(clicked()), this, SLOT(slotRemoveWord()));
	connect(btnClose, SIGNAL(clicked()), this, SLOT(accept()));
	
	connect(txtWord, SIGNAL(textChanged(const QString &)), this, SLOT(slotWordTextChanged(const QString &)));
	connect(txtHint, SIGNAL(textChanged(const QString &)), this, SLOT(slotHintTextChanged(const QString &)));
	connect(lboxWords, SIGNAL(selectionChanged()), this, SLOT(slotSelectionChanged()));
}

VocabEdit::~VocabEdit()
{
}

void VocabEdit::slotSave()
{
	KEduVocDocument *doc = new KEduVocDocument(this);
	doc->setTitle(txtVocabName->text());
	doc->setDocRemark(txtDescription->text());
	for(int i = 0; i < m_vocabList.size(); i++)
	{
		doc->appendEntry(&m_vocabList[i]);
	}
	doc->saveAs(this, KURL(KGlobal::dirs()->saveLocation("data", "kanagram/data/") + txtVocabName->text().lower() +".kvtml"), KEduVocDocument::automatic, "kanagram");
}

void VocabEdit::slotNewWord()
{
	lboxWords->insertItem("New Item");
	KEduVocExpression expr = KEduVocExpression();
	m_vocabList.append(expr);
}

void VocabEdit::slotSelectionChanged()
{
	disconnect(txtWord, SIGNAL(textChanged(const QString &)), this, SLOT(slotWordTextChanged(const QString &)));
	disconnect(txtHint, SIGNAL(textChanged(const QString &)), this, SLOT(slotHintTextChanged(const QString &)));
	if(lboxWords->currentItem() >= 0)
	{
		txtWord->setText(m_vocabList[lboxWords->currentItem()].getOriginal());
		txtHint->setText(m_vocabList[lboxWords->currentItem()].getRemark(0));
	}
	connect(txtWord, SIGNAL(textChanged(const QString &)), this, SLOT(slotWordTextChanged(const QString &)));
	connect(txtHint, SIGNAL(textChanged(const QString &)), this, SLOT(slotHintTextChanged(const QString &)));
}

void VocabEdit::slotWordTextChanged(const QString &changes)
{
	m_vocabList[lboxWords->currentItem()].setOriginal(changes);
	lboxWords->changeItem(changes, lboxWords->currentItem());
}

void VocabEdit::slotHintTextChanged(const QString &changes)
{
	m_vocabList[lboxWords->currentItem()].setRemark(0, changes);
}

void VocabEdit::slotRemoveWord()
{
	if (lboxWords->count()) {
		m_vocabList.erase(m_vocabList.begin() + lboxWords->currentItem());
		lboxWords->removeItem(lboxWords->currentItem());
	}
}

#include "vocabedit.moc"
