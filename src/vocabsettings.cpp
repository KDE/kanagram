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
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.           *
 ***************************************************************************/

#include "vocabsettings.h"
#include "vocabedit.h"

#include <qpushbutton.h>

#include <kdebug.h>
#include <kstandarddirs.h>
#include <qstringlist.h>
#include <qlistwidget.h>
#include <qvector.h>
#include <qfile.h>
#include <qfileinfo.h>

#include <kurl.h>

#include "keduvocdocument.h"
#include "kanagramsettings.h"

VocabSettings::VocabSettings(QWidget *parent) : QDialog(parent)
{
	setupUi(this);
	
	connect(btnCreateNew, SIGNAL(clicked()), this, SLOT(slotCreateNew()));
	connect(btnEdit, SIGNAL(clicked()), this, SLOT(slotEdit()));
	connect(btnDelete, SIGNAL(clicked()), this, SLOT(slotDelete()));
	connect(lviewVocab, SIGNAL(currentItemChanged(QTreeWidgetItem *, QTreeWidgetItem *)), this, SLOT(slotSelectionChanged(QTreeWidgetItem *)));

	refreshView();
}

VocabSettings::~VocabSettings()
{
}

void VocabSettings::refreshView()
{
	lviewVocab->clear();

	m_fileList = KGlobal::dirs()->findAllResources("appdata", "data/" + KanagramSettings::dataLanguage() + '/' + "*.kvtml");
	for(int i = 0; i < m_fileList.size(); i++)
	{
		KEduVocDocument *doc = new KEduVocDocument(this);
		doc->open(KUrl::fromPath(m_fileList[i]), false);
		QTreeWidgetItem *item = new QTreeWidgetItem(lviewVocab, 0);
		item->setText( 0, doc->title() );
		item->setText( 1, doc->documentRemark() );
		m_itemMap[item] = i;
	}
}

void VocabSettings::slotEdit()
{
	if(lviewVocab->currentItem())
	{
		int index = m_itemMap[lviewVocab->currentItem()];
		VocabEdit *vocabEdit = new VocabEdit(this, m_fileList[index]);
		vocabEdit->show();
	}
}

void VocabSettings::slotDelete()
{
	if(lviewVocab->currentItem())
	{
		int index = m_itemMap[lviewVocab->currentItem()];
		/*bool itWorked = */QFile::remove(m_fileList[index]);
	}

	refreshView();
}

void VocabSettings::slotCreateNew()
{
	VocabEdit *vocabEdit = new VocabEdit(this, "");
	vocabEdit->show();
}

void VocabSettings::slotSelectionChanged(QTreeWidgetItem *item)
{
	int index = m_itemMap[item];
	QFileInfo info = QFileInfo(m_fileList[index]);
	if(!info.isWritable())
	{
		btnDelete->setEnabled(false);
		btnEdit->setEnabled(false);
	}
	else
	{
		btnDelete->setEnabled(true);
		btnEdit->setEnabled(true);
	}
}

#include "vocabsettings.moc"

