/***************************************************************************
 *   Copyright (C) 2005 by Joshua Keel <joshuakeel@gmail.com>              *
 *             (C) 2007 by Jeremy Whiting <jpwhiting@kde.org>              *
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
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.          *
 ***************************************************************************/

#include "vocabsettings.h"
#include "vocabedit.h"

#include <kstandarddirs.h>
#include <kconfigdialog.h>
#include <qfileinfo.h>

#include <knewstuff3/downloaddialog.h>
#include <knewstuff3/knewstuffbutton.h>
#include <sharedkvtmlfiles.h>
#include <keduvocdocument.h>
#include "kanagramsettings.h"

VocabSettings::VocabSettings(QWidget *parent) : QWidget(parent), m_parent(NULL)
{
    setupUi(this);
    m_parent = (KConfigDialog*)parent;

    connect(lviewVocab, SIGNAL(currentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)), this, SLOT(slotSelectionChanged(QTreeWidgetItem*)));

    btnDownloadNew->setIcon(KIcon("get-hot-new-stuff"));

    refreshView();
}

VocabSettings::~VocabSettings()
{
}

void VocabSettings::refreshView()
{
    lviewVocab->clear();

    SharedKvtmlFiles::sortDownloadedFiles();
    m_fileList = SharedKvtmlFiles::fileNames(KanagramSettings::dataLanguage());
    m_titleList = SharedKvtmlFiles::titles(KanagramSettings::dataLanguage());
    m_commentList = SharedKvtmlFiles::comments(KanagramSettings::dataLanguage());

    for(int i = 0; i < m_fileList.size(); i++)
    {
        QTreeWidgetItem *item = new QTreeWidgetItem(lviewVocab, 0);
        item->setText( 0, m_titleList[i] );
        item->setText( 1, m_commentList[i] );
        m_itemMap[item] = i;
    }
    m_parent->enableButtonApply(true);
}

void VocabSettings::on_btnEdit_clicked()
{
    if(lviewVocab->currentItem())
    {
        int index = m_itemMap[lviewVocab->currentItem()];
        VocabEdit *vocabEdit = new VocabEdit(this, m_fileList[index]);
        connect(vocabEdit, SIGNAL(finished(int)), this, SLOT(refreshView()));
        vocabEdit->show();
    }
}

void VocabSettings::on_btnCreateNew_clicked()
{
    VocabEdit *vocabEdit = new VocabEdit(this, "");
    connect(vocabEdit, SIGNAL(finished(int)), this, SLOT(refreshView()));
    vocabEdit->show();
}

void VocabSettings::on_btnDownloadNew_clicked()
{
    KNS3::DownloadDialog hotNewStuffdialog( "kanagram.knsrc" );
    hotNewStuffdialog.exec();
    if ( hotNewStuffdialog.changedEntries().size() > 0 ){
        refreshView();
    }
}

void VocabSettings::slotSelectionChanged(QTreeWidgetItem *item)
{
    int index = m_itemMap[item];
    bool writeable = QFileInfo(m_fileList[index]).isWritable();
    btnEdit->setEnabled(writeable);
}

#include "vocabsettings.moc"

