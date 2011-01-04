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
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.             *
 ***************************************************************************/

#include "vocabedit.h"

#include <tqpushbutton.h>
#include <tqlistbox.h>
#include <tqlineedit.h>
#include <tqfile.h>
#include <tqstring.h>
#include <tqvaluevector.h>

#include <kstandarddirs.h>
#include <kglobal.h>
#include <kurl.h>
#include <kdebug.h>
#include <kmessagebox.h>
#include <klocale.h>

#include "keduvocdocument.h"
#include "keduvocexpression.h"
#include "vocabsettings.h"
#include "kanagramsettings.h"


VocabEdit::VocabEdit(TQWidget *parent, TQString fileName) : VocabEditWidget(parent), m_fileName("")
{
	if(fileName != "")
	{
		m_fileName = fileName;
		KEduVocDocument	*doc = new KEduVocDocument(this);
		doc->open(KURL(m_fileName), false);
		for(int i = 0; i < doc->numEntries(); i++)
		{
			KEduVocExpression expr = *doc->getEntry(i);
			m_vocabList.append(expr);
			lboxWords->insertItem(doc->getEntry(i)->getOriginal());	
		}
		txtVocabName->setText(doc->getTitle());
		txtDescription->setText(doc->getDocRemark());
	}

	connect(btnSave, TQT_SIGNAL(clicked()), this, TQT_SLOT(slotSave()));
	connect(btnNewWord, TQT_SIGNAL(clicked()), this, TQT_SLOT(slotNewWord()));
	connect(btnRemoveWord, TQT_SIGNAL(clicked()), this, TQT_SLOT(slotRemoveWord()));
	connect(btnClose, TQT_SIGNAL(clicked()), this, TQT_SLOT(slotClose()));
	
	connect(txtWord, TQT_SIGNAL(textChanged(const TQString &)), this, TQT_SLOT(slotWordTextChanged(const TQString &)));
	connect(txtHint, TQT_SIGNAL(textChanged(const TQString &)), this, TQT_SLOT(slotHintTextChanged(const TQString &)));

	//Connect the name and description boxes to a general textChanged slot, so that we can keep track of
	//whether they've been changed or not
	connect(txtVocabName, TQT_SIGNAL(textChanged(const TQString &)), this, TQT_SLOT(slotTextChanged(const TQString &)));
	connect(txtDescription, TQT_SIGNAL(textChanged(const TQString &)), this, TQT_SLOT(slotTextChanged(const TQString &)));

	connect(lboxWords, TQT_SIGNAL(selectionChanged()), this, TQT_SLOT(slotSelectionChanged()));

	//Has anything in the dialog changed?
	m_textChanged = false;
}

VocabEdit::~VocabEdit()
{
}

void VocabEdit::slotSave()
{
	KEduVocDocument *doc = new KEduVocDocument(this);
	doc->setTitle(txtVocabName->text());
	doc->setDocRemark(txtDescription->text());
	for(uint i = 0; i < m_vocabList.size(); i++)
	{
		doc->appendEntry(&m_vocabList[i]);
	}
	
	TQString fileName;
	if(m_fileName == "")
	{
		fileName = KGlobal::dirs()->saveLocation("data", "kanagram/data/" + KanagramSettings::dataLanguage()) + txtVocabName->text().lower().tqreplace(" ", "") + ".kvtml";
	}
	else
	{
		fileName = m_fileName;
	}
	doc->saveAs(this, KURL(fileName), KEduVocDocument::automatic, "kanagram");

	VocabSettings *settings = (VocabSettings*)this->tqparentWidget();
	settings->refreshView();

	if(m_textChanged)
		m_textChanged = false;
}

void VocabEdit::slotClose()
{
	//Has anything in the dialog changed?
	if(m_textChanged && lboxWords->count() > 0)
	{
		int code = KMessageBox::warningYesNo(this, i18n("Would you like to save your changes?"), i18n("Save Changes Dialog"));
		if(code == KMessageBox::Yes)
		{
			slotSave();
			close();
		}
		else
			close();
	}
	else
		close();
}

void VocabEdit::slotNewWord()
{
	lboxWords->insertItem("New Item");
	KEduVocExpression expr = KEduVocExpression();
	m_vocabList.append(expr);

	if(m_textChanged == false)
		m_textChanged = true;
}

void VocabEdit::slotSelectionChanged()
{
	//A little hack to make things work right
	disconnect(txtWord, TQT_SIGNAL(textChanged(const TQString &)), this, TQT_SLOT(slotWordTextChanged(const TQString &)));
	disconnect(txtHint, TQT_SIGNAL(textChanged(const TQString &)), this, TQT_SLOT(slotHintTextChanged(const TQString &)));
	if(lboxWords->currentItem() >= 0)
	{
		txtWord->setText(m_vocabList[lboxWords->currentItem()].getOriginal());
		txtHint->setText(m_vocabList[lboxWords->currentItem()].getRemark(0));
	}
	connect(txtWord, TQT_SIGNAL(textChanged(const TQString &)), this, TQT_SLOT(slotWordTextChanged(const TQString &)));
	connect(txtHint, TQT_SIGNAL(textChanged(const TQString &)), this, TQT_SLOT(slotHintTextChanged(const TQString &)));
}

void VocabEdit::slotWordTextChanged(const TQString &changes)
{
	//Make sure there actually is a currentItem()
	if(lboxWords->currentItem() != -1)
	{
		m_vocabList[lboxWords->currentItem()].setOriginal(changes);
		lboxWords->changeItem(changes, lboxWords->currentItem());
	}

	if(m_textChanged == false)
		m_textChanged = true;
}

void VocabEdit::slotHintTextChanged(const TQString &changes)
{
	//Make sure there actually is a currentItem()
	if(lboxWords->currentItem() != -1)
		m_vocabList[lboxWords->currentItem()].setRemark(0, changes);

	if(m_textChanged == false)
		m_textChanged = true;
}

void VocabEdit::slotTextChanged(const TQString &changes)
{
	//Make sure we know when text has been modified and not saved, so we
	//can notify the user
	if(m_textChanged == false)
		m_textChanged = true;

	//Make gcc happy
	(void)changes;
}

void VocabEdit::slotRemoveWord()
{
	if (lboxWords->count()) {
		m_vocabList.erase(m_vocabList.begin() + lboxWords->currentItem());
		lboxWords->removeItem(lboxWords->currentItem());
	}

	if(m_textChanged == false)
		m_textChanged = true;
}

#include "vocabedit.moc"
