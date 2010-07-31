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
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.             *
 ***************************************************************************/

#ifndef KANAGRAM_H
#define KANAGRAM_H

#include <config.h>

#include <tqwidget.h>
#include <tqlineedit.h>

#include <kxmlguiclient.h>

#ifndef WITHOUT_ARTS
#include <arts/kartsdispatcher.h>
#include <arts/kartsserver.h>
#include <arts/kplayobject.h>
#include <arts/kplayobjectfactory.h>
#else
class KArtsDispatcher;
class KArtsServer;
namespace KDE
{
	class PlayObjectFactory;
}
#endif

#include "kanagramgame.h"

#include <iostream>

using namespace std;

class KHelpMenu;
class KConfigDialog;
class KRandomSequence;
class KanagramGame;
class VocabSettings;

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
		void refreshVocabularies();

	private:
		void drawText(TQPainter &p, const TQString &text, const TQPoint &center, bool withMargin, int xMargin, int yMargin, TQRect *rect, bool highlight, int fontSize = 18);
		void drawTextNew(TQPainter &p, const TQString &text, int textAlign, int xMargin, int yMargin, const TQRect &rect, bool highlight, int fontSize = 18);

		void setupRects();

		void play(TQString filename);

		void paintEvent(TQPaintEvent *);

		void mousePressEvent(TQMouseEvent *e);

		void mouseMoveEvent(TQMouseEvent *e);

		void updateButtonHighlighting(const TQPoint &p);

		void randomHintImage();

		void showSettings();

		void drawHelpText(TQPainter &p, TQString text);

		void drawSwitcherText(TQPainter &p, TQString text);
		void drawSwitcher(TQPainter &p, const int xMargin, const int yMargin);

		TQRect innerRect(const TQRect &rect, const int xMargin, const int yMargin);

		KanagramGame *m_game;

		TQPixmap *m_back, *m_hintOverlay, *m_aboutKDEOverlay, *m_aboutAppOverlay, *m_handbookOverlay, *m_aboutKDEOverlayOver, *m_aboutAppOverlayOver, *m_handbookOverlayOver, *m_card, *m_arrow, *m_arrowOver, *m_next, *m_nextOver, *m_config, *m_configOver, *m_help, *m_helpOver, *m_quit, *m_quitOver, *m_up, *m_upOver, *m_upDisabled;

		//Deprecated
		//TQRect m_newWordRect, m_settingsRect, m_helpRect, m_quitRect;
		
		TQRect m_nextRect, m_configRect, m_helpRect, m_quitRect, m_revealRect, m_hintRect, m_upRect, m_aboutKDERect, m_aboutAppRect, m_handbookRect, m_switcherRect, m_arrowRect, m_logoRect, m_hintBoxRect, m_blackboardRect;

		bool m_overNext, m_overConfig, m_overHelp, m_overQuit, m_overReveal, m_overHint, m_overUp, m_overAboutKDE, m_overAboutApp, m_overHandbook, m_overSwitcher, m_overHintBox;

		bool m_showHint;

		TQColor m_fillColor, m_fontColor, m_fontHighlightColor, m_chalkColor, m_chalkHighlightColor;

		//Values for settings
		int m_hintHideTime;
		bool m_useSounds, m_useStandardFonts;

		TQFont m_blackboardFont, m_font;
		
		KHelpMenu *m_helpMenu;

		TQLineEdit *m_inputBox;
		
		VocabSettings *m_vocabSettings;

		KRandomSequence m_randomImage;

		TQTimer *m_hintTimer;

		KArtsDispatcher *m_artsDispatcher;
		KArtsServer *m_artsServer;
		KDE::PlayObjectFactory *m_artsFactory;
};

#endif
