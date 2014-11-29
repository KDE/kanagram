/***************************************************************************
 *   Copyright (C) 2005 by Joshua Keel <joshuakeel@gmail.com>              *
 *             (C) 2007 by Jeremy Whiting <jpwhiting@kde.org>              *
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

#include <KConfigDialog>
#include <KNS3/DownloadDialog>

#include <sharedkvtmlfiles.h>

#include <QIcon>
#include <QFileInfo>
#include <QtCore/QPointer>

#include "kanagramsettings.h"
#include "vocabedit.h"

VocabSettings::VocabSettings(QWidget *parent)
    : QWidget(parent)
{
    setupUi(this);

    connect(lviewVocab, &QTreeWidget::currentItemChanged, this, &VocabSettings::slotSelectionChanged);

    btnDownloadNew->setIcon(QIcon::fromTheme("get-hot-new-stuff"));

    refreshView();
}

VocabSettings::~VocabSettings()
{
}

void VocabSettings::loadView()
{
    lviewVocab->clear();

    SharedKvtmlFiles::sortDownloadedFiles();
    QString language = KanagramSettings::dataLanguage();
    m_fileList = SharedKvtmlFiles::fileNames(language);
    m_titleList = SharedKvtmlFiles::titles(language);
    m_commentList = SharedKvtmlFiles::comments(language);

    for (int i = 0; i < m_fileList.size(); i++)
    {
        QTreeWidgetItem *item = new QTreeWidgetItem(lviewVocab, 0);
        item->setText( 0, m_titleList[i] );
        item->setText( 1, m_commentList[i] );
        m_itemMap[item] = i;
    }
}

void VocabSettings::refreshView()
{
    loadView();
    emit widgetModified();
}

void VocabSettings::on_btnEdit_clicked()
{
    if (lviewVocab->currentItem())
    {
        int index = m_itemMap[lviewVocab->currentItem()];
        VocabEdit *vocabEdit = new VocabEdit(this, m_fileList[index]);
        connect(vocabEdit, &VocabEdit::finished, this, &VocabSettings::refreshView);
        vocabEdit->show();
    }
}

void VocabSettings::on_btnCreateNew_clicked()
{
    VocabEdit *vocabEdit = new VocabEdit(this, "");
    connect(vocabEdit, &VocabEdit::finished, this, &VocabSettings::refreshView);
    vocabEdit->show();
}

void VocabSettings::on_btnDownloadNew_clicked()
{
    QPointer<KNS3::DownloadDialog> dialog = new KNS3::DownloadDialog( "kanagram.knsrc" );
    dialog->exec();
    if ( dialog->changedEntries().size() > 0 ){
        refreshView();
    }

    delete dialog;
}

void VocabSettings::slotSelectionChanged(QTreeWidgetItem *item)
{
    int index = m_itemMap.value(item);
    bool writeable = QFileInfo(m_fileList[index]).isWritable();
    btnEdit->setEnabled(writeable);
}
