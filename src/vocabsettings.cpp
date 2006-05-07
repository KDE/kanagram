#include "vocabsettings.h"
#include "vocabedit.h"

#include <qpushbutton.h>

#include <kdebug.h>
#include <kstandarddirs.h>
#include <qstringlist.h>
#include <q3listview.h>
#include <q3valuevector.h>
#include <qfile.h>
#include <qfileinfo.h>

#include <kurl.h>

#include "keduvocdocument.h"
#include "kanagramsettings.h"

VocabSettings::VocabSettings(QWidget *parent) : VocabSettingsWidget(parent)
{
	connect(btnCreateNew, SIGNAL(clicked()), this, SLOT(slotCreateNew()));
	connect(btnEdit, SIGNAL(clicked()), this, SLOT(slotEdit()));
	connect(btnDelete, SIGNAL(clicked()), this, SLOT(slotDelete()));
	connect(lviewVocab, SIGNAL(selectionChanged(Q3ListViewItem *)), this, SLOT(slotSelectionChanged(Q3ListViewItem *)));

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
		doc->open(KUrl(m_fileList[i]), false);
		Q3ListViewItem *item = new Q3ListViewItem(lviewVocab, 0);
		item->setText( 0, doc->title() );
		item->setText( 1, doc->docRemark() );
		m_itemMap[item] = i;
	}
}

void VocabSettings::slotEdit()
{
	if(lviewVocab->selectedItem())
	{
		int index = m_itemMap[lviewVocab->selectedItem()];
		VocabEdit *vocabEdit = new VocabEdit(this, m_fileList[index]);
		vocabEdit->show();
	}
}

void VocabSettings::slotDelete()
{
	if(lviewVocab->selectedItem())
	{
		int index = m_itemMap[lviewVocab->selectedItem()];
		/*bool itWorked = */QFile::remove(m_fileList[index]);
	}

	refreshView();
}

void VocabSettings::slotCreateNew()
{
	VocabEdit *vocabEdit = new VocabEdit(this, "");
	vocabEdit->show();
}

void VocabSettings::slotSelectionChanged(Q3ListViewItem *item)
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

