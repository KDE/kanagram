/***************************************************************************
 *   Copyright (C) 2005 by Joshua Keel  *
 *   joshuakeel@gmail.com   *
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
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#ifndef KANAGRAM_H
#define KANAGRAM_H

#include <qwidget.h>
#include <qlineedit.h>

#include <kxmlguiclient.h>

#include "kanagramgame.h"

#include <iostream>

using namespace std;

class KHelpMenu;
class KConfigDialog;
class KRandomSequence;

class Kanagram : public QWidget
{
Q_OBJECT
	public:
		Kanagram();
		~Kanagram();

	private slots:
		void checkWord();

	private:
		void drawText(QPainter &p, const QString &text, const QPoint &center, bool withMargin, int xMargin, int yMargin, QRect *rect, bool highlight, bool bold, QString font, QColor fontColor, QColor fontHighlightColor, int fontSize = 18);
		
		void paintEvent(QPaintEvent *);

		void mousePressEvent(QMouseEvent *e);

		void mouseMoveEvent(QMouseEvent *e);

		void updateButtonHighlighting(const QPoint &p);

		void randomHintImage();

		KanagramGame m_game;	

		QPixmap *m_back, *m_hintOverlay;
		QRect m_newWordRect, m_settingsRect, m_helpRect, m_quitRect, m_revealRect, m_hintRect, m_tryRect;

		bool m_overNewWord, m_overSettings, m_overHelp, m_overQuit, m_overReveal, m_overHint, m_overTry;

		bool m_showHint;

		QColor m_fillColor, m_fontColor, m_fontHighlightColor, m_chalkColor, m_chalkHighlightColor;
		
		KHelpMenu *m_helpMenu;

		QLineEdit *m_inputBox;
		
		KConfigDialog *m_configDialog;

		KRandomSequence m_randomImage;
};

#endif
