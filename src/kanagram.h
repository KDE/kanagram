/***************************************************************************
 *   Copyright (C) 2005 by Joshua Keel <joshuakeel@gmail.com>              *
 *             (C) 2007 by Jeremy Whiting <jeremy@scitools.com>            *
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

/** @file
 * \brief header file for kanagram class
 * @author Joshua Keel <joshuakeel@gmail.com>
 * @author Jeremy Whiting <jeremy@scitools.com>
 * \par Maintainer: Jeremy Whiting 
 */
#ifndef KANAGRAM_H
#define KANAGRAM_H

#include <QMouseEvent>
#include <QPaintEvent>
#include <QWidget>

#include <KRandomSequence>

using namespace std;

namespace Phonon
{
    class MediaObject;
}

class KanagramGame;
class VocabSettings;

class QSvgRenderer;

class KAction;
class KActionCollection;
class KConfigDialog;
class KHelpMenu;
class KLineEdit;
class KShortcutsEditor;

/** 
 * @brief main interface widget  
 *
 * Draws the window, and reacts to mouse events */
class Kanagram : public QWidget
{
    Q_OBJECT
    public:
        /** default constructor */
        Kanagram();

        /** default destructor */
        ~Kanagram();

    private slots:

        /** check the entered word against the answer, and move on, or reset as necessary */
        void checkWord();

        /** reload the settings, and reload the vocab in kanagramgame */
        void reloadSettings();

        /** hide the hint box and text. */
        void hideHint();

        /** reset the input box in preparation for the next word entry */
        void resetInputBox();

        /** refresh the list of vocabularies in the vocabulary chooser in the config window.
         * This queries the files on disk to see what vocabularies have been added/removed.
         */
        void refreshVocabularies();

        /** invoke the settings dialog */
        void slotShowSettings();

        /** exit with an error message
         * connected to the game's fileError signal
         * so we can display an error message and quit
         * @param filename the file that had the error
         */
        void slotFileError(const QString &filename);

        /** show the popup menu of vocabularies, and allow choosing of one */
        void slotChooseVocabulary();

        /** move on to the next word */
        void slotNextAnagram();

        /** reveal the word */
        void slotRevealWord();

        /** move on to the next vocabulary */
        void slotNextVocabulary();

        /** go back to the previous vocabulary */
        void slotPrevVocabulary();

        /** show/hide the hint */
        void slotToggleHint();

        /** slot to save non kcfg_ settings */
        void slotSaveSettings();

        /** slot to undo non kcfg_ settings */
        void slotSettingsCancelled();

        /** slot to enable the apply button */
        void slotEnableApplyButton();

    private:

        /** load user settings 
         *  loads language choice,
         *  sound settings,
         *  and auto-hide hint time
         */
        void loadSettings();

        /** setup the actions and their shortcut keys */
        void setupActions();

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

        /** sets or clears flag based on whether p is in rect, and set changed if flag changes */
        void CheckRect(const QRect &rect, const QPoint &p, bool &flag, bool &changed);

        /** check the font size and set it to a sane minimum if it's an error size <= 0 */
        void FixFontSize(int &fontSize);

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
        KLineEdit *m_inputBox;

        /** settings dialog */
        KConfigDialog *m_configDialog;

        /** settings page pointers */
        VocabSettings *m_vocabSettings;
        KShortcutsEditor *m_shortcutsEditor;

        /** random number generator 
         * used to choose random eyes elements
         * each time a hint is displayed
         */
        KRandomSequence m_randomImage;

        /** timer used to hide the hint after a delay */
        QTimer *m_hintTimer;

        /** audio player to use for playing sounds */
        Phonon::MediaObject *m_player;

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

        KActionCollection * m_actionCollection;
};

#endif

