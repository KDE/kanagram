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
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.           *
 ***************************************************************************/

#ifndef VOCABSETTINGS_H
#define VOCABSETTINGS_H

#include "ui_vocabsettingswidget.h"

class KConfigDialog;

/** @brief Vocabulary Settings class
 *  @author Joshua Keel <joshuakeel@gmail.com>
 *
 *  Settings page to choose vocabularies, and buttons to invoke the vocabulary editor
 */
class VocabSettings : public QWidget, public Ui::VocabSettingsWidget
{
Q_OBJECT
    public:
        /** default constructor */
        explicit VocabSettings(QWidget *parent);

        /** default destructor */
        virtual ~VocabSettings();

    public slots:

        /** reload the list of vocabularies from what's on disk */
        void refreshView();

    private:

        /** list of vocabulary files */
        QStringList m_fileList;
        
        /** list of document titles */
        QStringList m_titleList;
        
        /** list of document comments */
        QStringList m_commentList;
        
        QMap<const QTreeWidgetItem*, int> m_itemMap;

        /** cache pointer to config dialog so we can enable the apply button in slotSetDirty */
        KConfigDialog *m_parent;

    private slots:

        /** create a new vocabulary
          * called when the btnCreateNew is clicked
          */
        void on_btnCreateNew_clicked();
        void on_btnDownloadNew_clicked();
        void on_btnEdit_clicked();
        void slotSelectionChanged(QTreeWidgetItem *item);
};

#endif

