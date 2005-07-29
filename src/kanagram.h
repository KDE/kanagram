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
 *   51 Franklin Steet, Fifth Floor, Boston, MA 02110-1301, USA.             *
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
		void loadSettings();
		void hideHint();
		void resetInputBox();

	private:
		void drawText(QPainter &p, const QString &text, const QPoint &center, bool withMargin, int xMargin, int yMargin, QRect *rect, bool highlight, bool bold, QFont &font, QColor fontColor, QColor fontHighlightColor, int fontSize = 18);
		
		void paintEvent(QPaintEvent *);

		void mousePressEvent(QMouseEvent *e);

		void mouseMoveEvent(QMouseEvent *e);

		void updateButtonHighlighting(const QPoint &p);

		void randomHintImage();

		void showSettings();

		void drawHelpText(QPainter &p, QString text);

		void drawSwitcherText(QPainter &p, QString text);

		KanagramGame m_game;

		QPixmap *m_back, *m_hintOverlay, *m_aboutKDEOverlay, *m_aboutAppOverlay, *m_handbookOverlay, *m_aboutKDEOverlayOver, *m_aboutAppOverlayOver, *m_handbookOverlayOver, *m_card, *m_arrow, *m_arrowOver, *m_next, *m_nextOver, *m_config, *m_configOver, *m_help, *m_helpOver, *m_quit, *m_quitOver;

		//Deprecated
		//QRect m_newWordRect, m_settingsRect, m_helpRect, m_quitRect;
		
		QRect m_nextRect, m_configRect, m_helpRect, m_quitRect, m_revealRect, m_hintRect, m_tryRect, m_aboutKDERect, m_aboutAppRect, m_handbookRect, m_switcherRect, m_arrowRect, m_logoRect, m_helpBoxRect;

		bool m_overNext, m_overConfig, m_overHelp, m_overQuit, m_overReveal, m_overHint, m_overTry, m_overAboutKDE, m_overAboutApp, m_overHandbook, m_overSwitcher, m_overHelpBox;

		bool m_showHint;

		QColor m_fillColor, m_fontColor, m_fontHighlightColor, m_chalkColor, m_chalkHighlightColor;

		//Values for settings
		int m_hintHideTime;
		bool m_useSounds, m_standardBlackboardFonts, m_standardInterfaceFonts;
		QString m_defaultVocab;

		QFont m_font, m_blackboardFont, m_buttonFont;
		
		KHelpMenu *m_helpMenu;

		QLineEdit *m_inputBox;
		
		KConfigDialog *m_configDialog;

		KRandomSequence m_randomImage;

		QTimer *m_hintTimer;
};

#endif
