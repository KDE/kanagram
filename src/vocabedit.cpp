#include "vocabedit.h"

#include <qpushbutton.h>
#include <qlistbox.h>
#include <qlineedit.h>

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
	//connect(lboxWords, SIGNAL(clicked(QListBoxItem *)), this, SLOT(slotItemClicked(QListBoxItem *)));
	connect(lboxWords, SIGNAL(selectionChanged()), this, SLOT(slotSelectionChanged()));
}

VocabEdit::~VocabEdit()
{
}

void VocabEdit::slotSave()
{
	cout << (const char *)(txtVocabName->text()) << endl;
	cout << (const char *)(txtDescription->text()) << endl;
	for(int i = 0; i < m_vocabList.size(); i++)
	{
		cout << (const char *)(m_vocabList[i].getWord()) << endl;
		cout << (const char *)(m_vocabList[i].getHint()) << endl;
	}
	close();
}

void VocabEdit::slotNewWord()
{
	lboxWords->insertItem("New Item");
	VocData data = VocData();
	m_vocabList.push_back(data);
	//lboxWords->setSelected((lboxWords->count() - 1), true);
}

/*
void VocabEdit::slotItemClicked(QListBoxItem *item)
{
	txtWord->setText(m_vocabList[lboxWords->currentItem()].getWord());
	txtHint->setText(m_vocabList[lboxWords->currentItem()].getHint());
}
*/

void VocabEdit::slotSelectionChanged()
{
	cout << lboxWords->currentItem() << endl;
	txtWord->setText(m_vocabList[lboxWords->currentItem()].getWord());
	txtHint->setText(m_vocabList[lboxWords->currentItem()].getHint());
}

void VocabEdit::slotWordTextChanged(const QString &changes)
{
	m_vocabList[lboxWords->currentItem()].setWord(changes);
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