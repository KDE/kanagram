#include "vocabedit.h"

#include <qpushbutton.h>
#include <qlistbox.h>
#include <qlineedit.h>

#include <qmap.h>

#include <kdebug.h>

#include <iostream>
using namespace std;

VocabEdit::VocabEdit(QWidget *parent) : VocabEditWidget(parent)
{
	connect(btnSave, SIGNAL(clicked()), this, SLOT(slotSave()));
	connect(btnNewWord, SIGNAL(clicked()), this, SLOT(slotNewWord()));
	connect(btnRemoveWord, SIGNAL(clicked()), this, SLOT(slotRemoveWord()));
	connect(btnCancel, SIGNAL(clicked()), this, SLOT(slotCancel()));
	connect(txtWord, SIGNAL(lostFocus()), this, SLOT(wordLostFocus()));
	connect(txtWord, SIGNAL(textChanged(const QString &)), this, SLOT(wordTextChanged(const QString &)));
	connect(lboxWords, SIGNAL(selectionChanged()), this, SLOT(wordsSelectionChanged()));
}

VocabEdit::~VocabEdit()
{
}

void VocabEdit::slotSave()
{
	for(it = m_wordMap.begin(); it != m_wordMap.end(); it++)
	{
		cout << it.key() << " " << it.data() << endl;
	}
}

void VocabEdit::wordsSelectionChanged()
{
	m_wordMap.insert(txtWord->text(), txtHint->text());
}

void VocabEdit::slotNewWord()
{
	lboxWords->insertItem("New Word");
	lboxWords->setSelected(lboxWords->numColumns(), true);
}

void VocabEdit::slotRemoveWord()
{
	lboxWords->removeItem(lboxWords->currentItem());
}

void VocabEdit::slotCancel()
{
	close();
}

void VocabEdit::wordTextChanged(const QString &changedText)
{
	lboxWords->changeItem(changedText, lboxWords->currentItem());
}

void VocabEdit::wordLostFocus()
{
}