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


#ifndef VOCABEDIT_H
#define VOCABEDIT_H

#include "vocabeditwidget.h"

#include <qvaluevector.h>

class KEduVocExpression;

class VocabEdit : public VocabEditWidget
{
Q_OBJECT
	public:
		VocabEdit(QWidget *parent);
		VocabEdit(QWidget *parent, QString fileName);
		~VocabEdit();
	
	public slots:
		void slotSave();
		void slotClose();
		void slotNewWord();
		void slotRemoveWord();
		void slotSelectionChanged();
		void slotWordTextChanged(const QString &changes);
		void slotHintTextChanged(const QString &changes);
		void slotTextChanged(const QString &changes);
		
	private:
		QValueVector<KEduVocExpression> m_vocabList;
		QString m_fileName;
		bool m_textChanged;
};

#endif

