#include "vocabsettings.h"
#include "vocabedit.h"

#include <tqpushbutton.h>

#include <kdebug.h>
#include <kstandarddirs.h>
#include <tqstringlist.h>
#include <tqlistview.h>
#include <tqvaluevector.h>
#include <tqfile.h>
#include <tqfileinfo.h>

#include <kurl.h>

#include "keduvocdocument.h"
#include "kanagramsettings.h"

VocabSettings::VocabSettings(TQWidget *parent) : VocabSettingsWidget(parent)
{
	connect(btnCreateNew, TQT_SIGNAL(clicked()), this, TQT_SLOT(slotCreateNew()));
	connect(btnEdit, TQT_SIGNAL(clicked()), this, TQT_SLOT(slotEdit()));
	connect(btnDelete, TQT_SIGNAL(clicked()), this, TQT_SLOT(slotDelete()));
	connect(lviewVocab, TQT_SIGNAL(selectionChanged(TQListViewItem *)), this, TQT_SLOT(slotSelectionChanged(TQListViewItem *)));

	refreshView();
}

VocabSettings::~VocabSettings()
{
}

void VocabSettings::refreshView()
{
	lviewVocab->clear();

	m_fileList = KGlobal::dirs()->findAllResources("appdata", "data/" + KanagramSettings::dataLanguage() + "/" + "*.kvtml");
	for(uint i = 0; i < m_fileList.size(); i++)
	{
		KEduVocDocument *doc = new KEduVocDocument(this);
		doc->open(KURL(m_fileList[i]), false);
		TQListViewItem *item = new TQListViewItem(lviewVocab, 0);
		item->setText( 0, doc->getTitle() );
		item->setText( 1, doc->getDocRemark() );
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
		/*bool itWorked = */TQFile::remove(m_fileList[index]);
	}

	refreshView();
}

void VocabSettings::slotCreateNew()
{
	VocabEdit *vocabEdit = new VocabEdit(this, "");
	vocabEdit->show();
}

void VocabSettings::slotSelectionChanged(TQListViewItem *item)
{
	int index = m_itemMap[item];
	TQFileInfo info = TQFileInfo(m_fileList[index]);
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

