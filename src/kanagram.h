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

#include <qwidget.h>
#include <qlineedit.h>
//Added by qt3to4:
#include <QMouseEvent>
#include <QPaintEvent>

#include "krandomsequence.h"

using namespace std;

class QSvgRenderer;

class KHelpMenu;
namespace Phonon
{
class AudioPlayer;
}
class KanagramGame;
class VocabSettings;

/** 
  * @brief Kanagram main class.  
  * @author Joshua Keel <joshuakeel@gmail.com>
  *
  * Draws the window, and reacts to mouse events */
class Kanagram : public QWidget
{
Q_OBJECT
    public:
        /** default ctor */
        Kanagram();

        /** default dtor */
        ~Kanagram();

    private slots:

        /** check the entered word against the answer, and move on, or reset as necessary */
        void checkWord();

        /** load user settings */
        void loadSettings();

        /** hide the hint box and text. */
        void hideHint();

        /** reset the input box in preparation for the next word */
        void resetInputBox();

        /** refresh the list of vocabularies in the vocabulary chooser in the config window.
          * This queries the files on disk to see what vocabularies have been added/removed.
          */
        void refreshVocabularies();

        /** exit with an error message
          * connected to the game's fileError signal
          * so we can display an error message and quit
          * @param filename the file that had the error
          */
        void slotFileError(const QString &filename);

    private:

        /** new method to draw text in a rectangle
          * @param p painter to use
          * @param text text string to draw
          * @param textAlign the alignment to use for the text (Left, right, center)
          * @param xMargin the margin in the x direction
          * @param yMargin the margin in the y direction
          * @param rect the bounding rectangle to draw the text inside of
          * @param highlight whether or not to use the highlight color
          *   if true m_chalkHighlightColor is used, 
          *   otherwise m_chalkColor is used
          * @param fontSize the fontsize to use
          */
        void drawTextNew(QPainter &p, const QString &text, int textAlign, int xMargin, 
                int yMargin, const QRect &rect, bool highlight, int fontSize = 18);

        /** play a sound file
          * @param filename the filename of the sound file in kanagram/sounds/
          */
        void play(const QString &filename);

        /** paint the widget.
          * draws all the elements of the kanagram interface (except the config dialog of course.)
          */
        void paintEvent(QPaintEvent *);
        
        /** called whenever the window is resized
          * update all svg elements calculated positions based on the new window size
          */
        void resizeEvent(QResizeEvent *);

        /** called whenever a mouse click occurs
          * checks the mouse position against svg element positions
          * and reacts accordingly
          */
        void mousePressEvent(QMouseEvent *e);

        /** called when the mouse is moved
          * checks the mouse position against any svg elements that trigger an action
          * or have a hover state, and sets/resets those states
          */
        void mouseMoveEvent(QMouseEvent *e);

        /** chose a random hint image element name
          * used for making the eyes in the hint element different each time
          * a hint is requested
          */
        void randomHintImage();

        /** invoke the settings dialog */
        void showSettings();

        /** draw given text in the helper element
          * @param p painter to use to draw
          * @param text text to write to the painter
          */
        void drawHelpText(QPainter &p, const QString &text);

        /** draw the name of the current vocabulary and the arrow icon
          * @param p painter to use to draw
          * @param xMargin x margin inside of the blackboard rect
          * @param yMargin y margin inside of the blackboard rect
          */
        void drawSwitcher(QPainter &p, const int xMargin, const int yMargin);

        /** get a rectangle inside a rectangle
          * @param rect outer rectangle
          * @param xMargin how much margin to leave on the right and left side
          * @param yMargin how much margin to leave on the top and bottom
          */
        QRect innerRect(const QRect &rect, const int xMargin, const int yMargin);

        /**
          * KanagramGame object to get words,
          * scrambled words,
          * and vocabulary names from
          */
        KanagramGame *m_game;

        /** name of the hint overlay svg element
          * set in randomHintImage
          * used in paintEvent
          */
        QString m_hintOverlayName;

        /** name of the arrow svn element to use
          * set in loadSettingS
          * used in drawSwitcher
          */
        QString m_arrowName;

        /** rectangles to use for detecting hover with the mouse position */
        QRect m_nextRect;
        QRect m_configRect; 
        QRect m_helpRect;
        QRect m_quitRect;
        QRect m_revealRect;
        QRect m_hintRect;
        QRect m_upRect;
        QRect m_aboutKDERect;
        QRect m_aboutAppRect; 
        QRect m_handbookRect; 
        QRect m_switcherRect; 
        QRect m_arrowRect; 
        QRect m_logoRect; 
        QRect m_hintBoxRect; 
        QRect m_blackboardRect;

        /** states to use for drawing hover elements, or regular elements */
        bool m_overNext;
        bool m_overConfig; 
        bool m_overHelp; 
        bool m_overQuit; 
        bool m_overReveal; 
        bool m_overHint; 
        bool m_overUp; 
        bool m_overAboutKDE; 
        bool m_overAboutApp; 
        bool m_overHandbook; 
        bool m_overSwitcher; 
        bool m_overLogo; 
        bool m_overHintBox;

        bool m_showHint;

        /** colors for use in drawing text and lines */
        QColor m_fillColor; 
        QColor m_fontColor; 
        QColor m_fontHighlightColor; 
        QColor m_chalkColor; 
        QColor m_chalkHighlightColor;

        /** Values for settings */
        int m_hintHideTime;
        bool m_useSounds; 

        /** help menu for displaying about box */
        KHelpMenu *m_helpMenu;

        /** input box for getting answer from the user */
        QLineEdit *m_inputBox;
        
        /** settings dialog pointer */
        VocabSettings *m_vocabSettings;

        /** random number generator 
          * used to choose random eyes elements
          * each time a hint is displayed
          */
        KRandomSequence m_randomImage;

        /** timer used to hide the hint after a delay */
        QTimer *m_hintTimer;

        /** audio player to use for playing sounds */
        Phonon::AudioPlayer *m_player;
        
        /** renderer used to render gui elements */
        QSvgRenderer * m_renderer;
        
        /** font size to use for elements in the corner of the blackboard
          * updated in the resize event
          */
        int m_cornerFontSize;
        
        /** ratios to use for drawing elements scaled.
          * based on the current size of the window
          * updated in the resize event
          */
        double m_xRatio;
        double m_yRatio;
};

#endif

