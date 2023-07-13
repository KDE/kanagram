/***************************************************************************
*   Copyright (C) 2005 by Joshua Keel <joshuakeel@gmail.com>              *
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

#include <kwidgetsaddons_version.h>
#include <KLocalizedString>
#include <KMessageBox>

#include <KEduVocDocument>
#include <KEduVocExpression>

#include <QDir>
#include <QStandardPaths>

#include "kanagramsettings.h"
#include "vocabsettings.h"
#include "kanagramgame.h"

VocabEdit::VocabEdit(QWidget *parent, const QString  &fileName) : QDialog(parent), m_fileName(QLatin1String(""))
{
    setupUi(this);

    if (!fileName.isEmpty())
    {
        m_fileName = fileName;
        KEduVocDocument	*doc = new KEduVocDocument(this);
        doc->open(QUrl::fromLocalFile(m_fileName), KEduVocDocument::FileIgnoreLock);
        for (int i = 0; i < doc->lesson()->entryCount(KEduVocLesson::Recursive); i++)
        {
            KEduVocExpression expr = *doc->lesson()->entries(KEduVocLesson::Recursive).value(i);
            m_vocabList.append(expr);
            lboxWords->addItem(doc->lesson()->entries(KEduVocLesson::Recursive).value(i)->translation(0)->text());
        }
        txtVocabName->setText(doc->title());
        txtDescription->setText(doc->documentComment());
        delete doc;
    }

    connect(btnSave, &QPushButton::clicked, this, &VocabEdit::slotSave);
    connect(btnNewWord, &QPushButton::clicked, this, &VocabEdit::slotNewWord);
    connect(btnRemoveWord, &QPushButton::clicked, this, &VocabEdit::slotRemoveWord);
    connect(btnClose, &QPushButton::clicked, this, &VocabEdit::slotClose);

    connect(txtWord, &QLineEdit::textChanged, this, &VocabEdit::slotWordTextChanged);
    connect(txtHint, &QLineEdit::textChanged, this, &VocabEdit::slotHintTextChanged);

    //Connect the name and description boxes to a general textChanged slot, so that we can keep track of
    //whether they've been changed or not
    connect(txtVocabName, &QLineEdit::textChanged, this, &VocabEdit::slotTextChanged);
    connect(txtDescription, &QLineEdit::textChanged, this, &VocabEdit::slotTextChanged);

    connect(lboxWords, &QListWidget::itemSelectionChanged, this, &VocabEdit::slotSelectionChanged);

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
    doc->setDocumentComment(txtDescription->text());
    KEduVocIdentifier id;
    doc->appendIdentifier(id);
    for (int i = 0; i < m_vocabList.size(); i++)
    {
        doc->lesson()->appendEntry(&m_vocabList[i]);
    }

    const QString fileName = txtVocabName->text().toLower().remove(' ') + ".kvtml";
    const QString path = QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation) +
                   "/apps/kvtml/" + KanagramSettings::dataLanguage();
    QDir dir;
    dir.mkpath(path);
    const QUrl url = QUrl::fromLocalFile(path +
                            QLatin1Char('/') + fileName);
    qCDebug(KANAGRAM) << "Saving file as " << url;
    doc->saveAs(url, KEduVocDocument::Automatic);

    m_textChanged = false;
}

void VocabEdit::slotClose()
{
    //Has anything in the dialog changed?
    if (m_textChanged && lboxWords->count() > 0)
    {
#if KWIDGETSADDONS_VERSION >= QT_VERSION_CHECK(5, 100, 0)
        int code = KMessageBox::warningTwoActions(this,
#else
        int code = KMessageBox::warningYesNo(this,
#endif
                                             i18n("Would you like to save your changes?"),
                                             i18n("Save Changes Dialog"),
                                             KStandardGuiItem::save(),
                                             KStandardGuiItem::discard());
#if KWIDGETSADDONS_VERSION >= QT_VERSION_CHECK(5, 100, 0)
        if (code == KMessageBox::PrimaryAction)
#else
        if (code == KMessageBox::Yes)
#endif
        {
            slotSave();
            close();
        }
        else
        {
            close();
        }
    }
    else
    {
        close();
    }
}

void VocabEdit::slotNewWord()
{
    lboxWords->addItem(i18n("New Item"));
    KEduVocExpression expr = KEduVocExpression();
    m_vocabList.append(expr);
    m_textChanged = true;
}

void VocabEdit::slotSelectionChanged()
{
    //A little hack to make things work right
    disconnect(txtWord, &QLineEdit::textChanged, this, &VocabEdit::slotWordTextChanged);
    disconnect(txtHint, &QLineEdit::textChanged, this, &VocabEdit::slotHintTextChanged);
    if (lboxWords->currentRow() >= 0)
    {
        txtWord->setText(m_vocabList[lboxWords->currentRow()].translation(0)->text());
        txtHint->setText(m_vocabList[lboxWords->currentRow()].translation(0)->comment());
    }
    connect(txtWord, &QLineEdit::textChanged, this, &VocabEdit::slotWordTextChanged);
    connect(txtHint, &QLineEdit::textChanged, this, &VocabEdit::slotHintTextChanged);
}

void VocabEdit::slotWordTextChanged(const QString &changes)
{
    //Make sure there actually is a currentRow()
    if (lboxWords->currentRow() != -1)
    {
        m_vocabList[lboxWords->currentRow()].setTranslation(0, changes);
        lboxWords->currentItem()->setText(changes);
    }
    m_textChanged = true;
}

void VocabEdit::slotHintTextChanged(const QString &changes)
{
    //Make sure there actually is a currentItem()
    if (lboxWords->currentRow() != -1)
        m_vocabList[lboxWords->currentRow()].translation(0)->setComment(changes);
    m_textChanged = true;
}

void VocabEdit::slotTextChanged(const QString &changes)
{
    Q_UNUSED(changes);

    //Make sure we know when text has been modified and not saved, so we
    //can notify the user
    m_textChanged = true;
}

void VocabEdit::slotRemoveWord()
{
    if (lboxWords->count() && lboxWords->currentRow() >= 0) {
        m_vocabList.erase(m_vocabList.begin() + lboxWords->currentRow());
        delete lboxWords->takeItem(lboxWords->currentRow());
    }

    m_textChanged = true;
}

#include "moc_vocabedit.cpp"
