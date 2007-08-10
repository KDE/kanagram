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

#include "kanagram.h"

#include <QMouseEvent>
#include <QPainter>
#include <QPaintEvent>
#include <QPixmap>
#include <QString>
#include <QSvgRenderer>
#include <QTimer>

#include <Phonon/MediaObject>

#include <KAction>
#include <KActionCollection>
#include <KApplication>
#include <KComponentData>
#include <KConfigDialog>
#include <KConfigSkeleton>
#include <KGlobalSettings>
#include <KHelpMenu>
#include <KLineEdit>
#include <KMessageBox>
#include <KShortcutsEditor>
#include <KStandardAction>
#include <KStandardDirs>
#include <KStandardShortcut>

#include <sharedkvtmlfiles.h>

#include "kanagramgame.h"
#include "kanagramsettings.h"
#include "mainsettings.h"
#include "vocabsettings.h"
#include "newstuff.h"
#include "fontutils.h"

static const char* m_textRevealWord = I18N_NOOP("reveal word");
static const char* m_textHint = I18N_NOOP("hint");

double kWindowWidth = 1000.0;
double kWindowHeight = 725.0;

double xEyesScale = 270.437 / kWindowWidth;
double yEyesScale = 195.176 / kWindowHeight;

double xScale57Buttons = 57.5 / kWindowWidth;
double yScale57Buttons = 57.5 / kWindowHeight;

double xScale55Buttons = 55.0 / kWindowWidth;
double yScale55Buttons = 55.0 / kWindowHeight;

double xTranslateButtons = 810.053 / kWindowWidth;

double yTranslateNextButton = 59.588 / kWindowHeight;
double yTranslateConfigButton = 199.85 / kWindowHeight;
double yTranslateHelpButton = 337.487 / kWindowHeight;

double xScaleQuitButton = 77.484 / kWindowWidth;
double yScaleQuitButton = 77.5 / kWindowHeight;

Kanagram::Kanagram()
: QWidget(0), m_game(NULL), m_overNext(false), m_overConfig(false), m_overHelp(false),
    m_overQuit(false), m_overReveal(false), m_overHint(false), m_overUp(false), m_overHintBox(false),
    m_showHint(false), m_player(NULL), m_actionCollection(NULL)
{
    setAttribute(Qt::WA_StaticContents);
    m_renderer = new QSvgRenderer(KStandardDirs::locate("appdata", "images/kanagram.svg"));

    m_helpMenu = new KHelpMenu(this, KGlobal::mainComponent().aboutData());

    setupActions();

    loadSettings();

    m_game = new KanagramGame();

    setMouseTracking(true);
    m_chalkColor = QColor(155, 155, 155);
    m_chalkHighlightColor = QColor(255, 255, 255);
    m_fillColor = QColor(45, 45, 45);
    m_fontColor = QColor(55, 55, 55);
    m_fontHighlightColor = QColor(99, 99, 99);

    m_hintTimer = new QTimer(this);
    m_hintTimer->setSingleShot(true);

    m_inputBox = new KLineEdit(this);
    m_inputBox->setFrame(false);

    connect(m_inputBox, SIGNAL(returnPressed()), SLOT(checkWord()));
    connect(m_hintTimer, SIGNAL(timeout()), SLOT(hideHint()));
    connect(m_inputBox, SIGNAL(textChanged(const QString &)), SLOT(update()));
    connect(m_game, SIGNAL(fileError(const QString &)), SLOT(slotFileError(const QString &)));

    QFont f = QFont();
    f.setPointSize(17);
    m_inputBox->setFont(f);
    m_inputBox->show();

    show();

    setMinimumSize(650, 471);
}

Kanagram::~Kanagram()
{
    if (m_player != NULL)
    {
        delete m_player;
        m_player = NULL;
    }

    if (m_game != NULL)
    {
        delete m_game;
        m_game = NULL;
    }

    if (m_renderer != NULL)
    {
        delete m_renderer;
        m_renderer = NULL;
    }
}

void Kanagram::loadSettings()
{
    QString hideTime = KanagramSettings::hintHideTime();
    if (hideTime[0].isDigit())
    {
        m_hintHideTime = hideTime[0].digitValue();
    }
    else
    {
        m_hintHideTime = 0;
    }

    if (KanagramSettings::dataLanguage().isEmpty())
    {
        QStringList userLanguagesCode = KGlobal::locale()->languageList();

        int i = 0;
        bool foundLanguage = false;
        while (i < userLanguagesCode.size() && !foundLanguage)
        {
            if (SharedKvtmlFiles::languages().contains(userLanguagesCode[i]))
            {
                foundLanguage = true;
            }
            else
            {
                ++i;
            }
        }
        // at this point either foundLanguage == true, or i > userLanguagesCode.size()

        KanagramSettings::setDataLanguage(foundLanguage ? userLanguagesCode[i] : "en");
    }

    kDebug() << "Data language: " << KanagramSettings::dataLanguage();

    m_useSounds = KanagramSettings::useSounds();
    m_arrowName = "basicarrow";
}

void Kanagram::reloadSettings()
{
    loadSettings();
    refreshVocabularies();
}

void Kanagram::setupActions()
{
    m_actionCollection = new KActionCollection(this);
    m_actionCollection->setAssociatedWidget(this);

    // next anagram action
    KAction *nextAnagramAction = new KAction(i18n("Next Word"), m_actionCollection);
    nextAnagramAction->setShortcut(Qt::CTRL+Qt::Key_N);
    connect(nextAnagramAction, SIGNAL(triggered(bool)), this, SLOT(slotNextAnagram()));
    m_actionCollection->addAction("nextanagram", nextAnagramAction);
    
    // hint action needs to not be help, as that conflicts with helpContents for shortcut key
    KAction *showHintAction = new KAction(i18n("Show Hint"), m_actionCollection);
    showHintAction->setShortcut(Qt::CTRL+Qt::Key_H);
    connect(showHintAction, SIGNAL(triggered(bool)), this, SLOT(slotToggleHint()));
    m_actionCollection->addAction("showhint", showHintAction);
    
    // reveal word action
    KAction *revealWordAction = new KAction(i18n("Reveal Word"), m_actionCollection);
    revealWordAction->setShortcut(Qt::CTRL+Qt::Key_R);
    connect(revealWordAction, SIGNAL(triggered(bool)), this, SLOT(slotRevealWord()));
    m_actionCollection->addAction("revealword", revealWordAction);
    
    // vocabulary actions
    KStandardAction::prior(this, SLOT(slotPrevVocabulary()), m_actionCollection);
    KStandardAction::next(this, SLOT(slotNextVocabulary()), m_actionCollection);
    
    // help actions
    KStandardAction::aboutApp(m_helpMenu, SLOT(aboutApplication()), m_actionCollection);
    KStandardAction::aboutKDE(m_helpMenu, SLOT(aboutKDE()), m_actionCollection);
    KStandardAction::helpContents(m_helpMenu, SLOT(appHelpActivated()), m_actionCollection);

    // standard actions
    KStandardAction::preferences(this, SLOT(slotShowSettings()), m_actionCollection);
    KStandardAction::quit(kapp, SLOT(quit()), m_actionCollection);
    
    
    // load any user-defined changes to shortcuts
    m_actionCollection->readSettings();
}

void Kanagram::paintEvent(QPaintEvent *)
{
    QPixmap buf(width(), height());
    QPainter p(&buf);
    p.setRenderHint(QPainter::Antialiasing);

    m_renderer->render(&p, "background");

    m_xRatio = width() / kWindowWidth;
    m_yRatio = height() / kWindowHeight;

    if (m_overLogo)
    {
        p.translate(int(112.188 * m_xRatio), int(32.375 * m_yRatio));
        p.scale(466.981 / kWindowWidth, 91.407 / kWindowHeight);
        m_renderer->render(&p, "logo_hover");
        p.resetMatrix();
    }
    else
    {
        p.translate(int(112.188 * m_xRatio), int(32.375 * m_yRatio));
        p.scale(466.981 / kWindowWidth, 91.407 / kWindowHeight);
        m_renderer->render(&p, "logo");
        p.resetMatrix();
    }

    if (m_overNext)
    {
        p.translate(xTranslateButtons * width(), yTranslateNextButton * height());
        p.scale(xScale55Buttons, yScale55Buttons);
        m_renderer->render(&p, "next_hover");
        p.resetMatrix();
    }

    if (m_overConfig)
    {
        p.translate(xTranslateButtons * width(), yTranslateConfigButton * height());
        p.scale(xScale55Buttons, yScale55Buttons);
        m_renderer->render(&p, "config_hover");
        p.resetMatrix();
    }

    if (m_overHelp)
    {
        p.translate(xTranslateButtons * width(), yTranslateHelpButton * height());
        p.scale(xScale55Buttons, yScale55Buttons);
        m_renderer->render(&p, "help_hover");
        p.resetMatrix();
    }

    if (m_overQuit)
    {
        p.translate(798.811 * m_xRatio, 556.803 * m_yRatio);
        p.scale(xScaleQuitButton, yScaleQuitButton);
        m_renderer->render(&p, "quit_hover");
        p.resetMatrix();
    }

    QString anagram = m_game->getAnagram();
    int afontSize = fontUtils::fontSize(p, anagram, m_blackboardRect.width(), m_blackboardRect.height() / 5);
    FixFontSize(afontSize);
    drawTextNew(p, anagram, Qt::AlignCenter, 10, 10, m_blackboardRect, true, afontSize);

    QString reveal = i18n(m_textRevealWord);
    m_cornerFontSize = fontUtils::fontSize(p, reveal, m_blackboardRect.width() / 3, m_blackboardRect.height() / 5);
    FixFontSize(m_cornerFontSize);

    drawTextNew(p, i18n(m_textRevealWord), Qt::AlignBottom | Qt::AlignRight, 6, 0, m_blackboardRect, m_overReveal, m_cornerFontSize);
    drawTextNew(p, i18n(m_textHint), Qt::AlignBottom | Qt::AlignLeft, 6, 0, m_blackboardRect, m_overHint, m_cornerFontSize);

    // update these rects because we have access to the painter and thus the fontsize here
    QFont font = KGlobalSettings::generalFont();
    font.setPointSize(m_cornerFontSize);
    font.setBold(true);
    QFontMetrics fm(font);
    QRect r = innerRect(m_blackboardRect, 6, 0);
    m_hintRect = fm.boundingRect(r, Qt::AlignBottom|Qt::AlignLeft, i18n(m_textHint));
    m_hintBoxRect = QRect(int(684.813 * m_xRatio), int(319.896 * m_yRatio), int(xEyesScale * width()), int(yEyesScale * height()));
    r = innerRect(m_blackboardRect, 6, 0);
    m_revealRect = fm.boundingRect(r, Qt::AlignBottom|Qt::AlignRight, i18n(m_textRevealWord));

    drawSwitcher(p, 9, 8);

    p.setPen(QPen(Qt::black, 3));

    //Draw the border of the input box
    QRect borderRect = m_inputBox->geometry();
    p.drawRoundRect(borderRect, 10, 5);
    int inputBoxFontSize = fontUtils::fontSize(p, "A", borderRect.width(), borderRect.height()) - 1;
    FixFontSize(inputBoxFontSize);
    QFont f = QFont();
    f.setPointSize(inputBoxFontSize);
    m_inputBox->setFont(f);

    //Draw the border of the Up arrow
    borderRect = m_upRect;
    p.fillRect(borderRect, m_fillColor);
    p.drawRoundRect(borderRect, 10, 5);

    QString upArrow = "up";
    if (m_overUp && !m_inputBox->text().isEmpty())
    {
        upArrow = "up_hover";
    }
    else if (m_inputBox->text().isEmpty())
    {
        upArrow = "up_disabled";
    }

    p.translate(m_inputBox->x() + m_inputBox->width() + 27 * m_xRatio, m_inputBox->y() + 12 * m_yRatio);
    p.scale(38 / kWindowWidth, 20 / kWindowHeight);
    m_renderer->render(&p, upArrow);
    p.resetMatrix();

    if (m_showHint)
    {
        p.translate(684.813 * m_xRatio, 319.896 * m_yRatio);
        p.scale(xEyesScale, yEyesScale);
        m_renderer->render(&p, m_hintOverlayName);
        p.resetMatrix();

        // do drawText with svg position and size
        QFont f = KGlobalSettings::generalFont();
        f.setWeight(QFont::Bold);
        QString hint = m_game->getHint();
        int fontSize = fontUtils::fontSize(p, hint, int(400 * m_xRatio), int(150 * m_yRatio));
        FixFontSize(fontSize);
        f.setPointSize(fontSize);
        p.setFont(f);
        p.drawText(int(694 * m_xRatio), int(330 * m_yRatio), int(250 * m_xRatio), int(100 * m_yRatio),
                Qt::TextWordWrap | Qt::AlignCenter, hint);
    }

    if (m_overHelp && !m_showHint)
    {
        if (m_overAboutApp)
        {
            p.translate(808.377 * m_xRatio, 335.352 * m_yRatio);
            p.scale(xScale57Buttons, yScale57Buttons);
            m_renderer->render(&p, "appicon_hover");
            p.resetMatrix();
            drawHelpText(p, i18n("About Kanagram"));
        }
        else
        {
            p.translate(808.377 * m_xRatio, 335.352 * m_yRatio);
            p.scale(xScale57Buttons, yScale57Buttons);
            m_renderer->render(&p, "appicon");
            p.resetMatrix();
        }
        if (m_overAboutKDE)
        {
            p.translate(865.877 * m_xRatio, 335.352 * m_yRatio);
            p.scale(xScale57Buttons, yScale57Buttons);
            m_renderer->render(&p, "kicon_hover");
            p.resetMatrix();
            drawHelpText(p, i18n("About KDE"));
        }
        else
        {
            p.translate(865.877 * m_xRatio, 335.352 * m_yRatio);
            p.scale(xScale57Buttons, yScale57Buttons);
            m_renderer->render(&p, "kicon");
            p.resetMatrix();
        }
        if (m_overHandbook)
        {
            p.translate(750.877 * m_xRatio, 335.352 * m_yRatio);
            p.scale(xScale57Buttons, yScale57Buttons);
            m_renderer->render(&p, "handbook_hover");
            p.resetMatrix();
            drawHelpText(p, i18n("Kanagram Handbook"));
        }
        else
        {
            p.translate(750.877 * m_xRatio, 335.352 * m_yRatio);
            p.scale(xScale57Buttons, yScale57Buttons);
            m_renderer->render(&p, "handbook");
            p.resetMatrix();
        }
    }
    else if (m_overNext)
    {
        drawHelpText(p, i18n("Next Word"));
    }
    else if (m_overConfig)
    {
        drawHelpText(p, i18n("Configure Kanagram"));
    }
    else if (m_overQuit)
    {
        drawHelpText(p, i18n("Quit Kanagram"));
    }

    QPainter p2(this);
    p2.drawPixmap(0, 0, buf);
}

/** check the font size and set it to a sane minimum if it's an error size <= 0 */
void Kanagram::FixFontSize(int &fontSize)
{
    if (fontSize <= 0)
    {
        fontSize = 8;
    }
}

void Kanagram::resizeEvent(QResizeEvent *)
{
    m_xRatio = width() / kWindowWidth;
    m_yRatio = height() / kWindowHeight;

    m_blackboardRect = QRect(int(63.657 * m_xRatio), int(182.397 * m_yRatio), int(563.273 * m_xRatio), int(380.735 * m_yRatio));
    m_inputBox->setGeometry(QRect(int(80 * m_xRatio), int(657.272 * m_yRatio), int(420 * m_xRatio), int(44.639 * m_yRatio)));

    m_upRect = QRect(int(m_inputBox->x() + m_inputBox->width() + 20 * m_xRatio), m_inputBox->y(), int(50 * m_xRatio), m_inputBox->height());
    m_arrowRect = QRect(m_switcherRect.right() + 5, m_switcherRect.top(), int(16.250 * m_xRatio), int(25.0 * m_yRatio));
    m_logoRect = QRect(int(112.188 * m_xRatio), int(32.375 * m_yRatio), int(466.981 * m_xRatio), int(91.407 * m_yRatio));

    m_aboutAppRect = QRect(int(xTranslateButtons * width()), int(yTranslateHelpButton * height()),
            int(xScale57Buttons * width()), int(yScale57Buttons * height()));
    m_aboutKDERect = QRect(int(867 * m_xRatio), int(335.352 * m_yRatio),
            int(xScale57Buttons * width()), int(yScale57Buttons * height()));
    m_handbookRect = QRect(int(753 * m_xRatio), int(335.352 * m_yRatio),
            int(xScale57Buttons * width()), int(yScale57Buttons * height()));

    m_nextRect = QRect(int(735.448 * m_xRatio), int(49.028 * m_yRatio), int(206.142 * m_xRatio), int(117.537 * m_yRatio));
    m_configRect = QRect(int(735.448 * m_xRatio), int(188.264 * m_yRatio), int(206.142 * m_xRatio), int(117.537 * m_yRatio));
    m_helpRect = QRect(int(735.448 * m_xRatio), int(327.5 * m_yRatio), int(206.142 * m_xRatio), int(117.537 * m_yRatio));
    m_quitRect = QRect(int(697.549 * m_xRatio), int(542.337 * m_yRatio), int(279.935 * m_xRatio), int(160.68 * m_yRatio));

    update();
}

void Kanagram::drawHelpText(QPainter &p, const QString &text)
{
    p.translate(700.582 * m_xRatio, 424.176 * m_yRatio);
    p.scale(256.582 / kWindowWidth, 101.150 / kWindowHeight);
    m_renderer->render(&p, "card");
    p.resetMatrix();

    p.save();
    QFont font = KGlobalSettings::generalFont();
    font.setPointSize(m_cornerFontSize);
    p.setFont(font);
    // translate to the same place as the folder plus a margin
    p.translate(720.582 * m_xRatio, 480 * m_yRatio);
    p.rotate(-3.29);
    p.setPen(Qt::black);

    // draw the first word
    p.drawText(0, 0, text.section(' ', 0, 0));
    // draw the second word under the first
    p.drawText(0, int(30 * m_yRatio), text.section(' ', 1));
    p.restore();
}

void Kanagram::drawSwitcher(QPainter &p, const int xMargin, const int yMargin)
{
    const int padding = 5;
    QString text = m_game->getDocTitle();
    QFont font = KGlobalSettings::generalFont();
    font.setPointSize(m_cornerFontSize);
    QFontMetrics fm(font);
    QRect r = innerRect(m_blackboardRect, xMargin, yMargin);
    r = r.normalized();
    r.translate(- padding - int(16.250 * m_xRatio), yMargin);
    r.setHeight(int(25.0 * m_yRatio));
    m_switcherRect = p.boundingRect(r, Qt::AlignVCenter|Qt::AlignRight, text);
    p.setFont(font);
    QString arrow = m_arrowName;
    if (m_overSwitcher)
    {
        p.setPen(m_chalkHighlightColor);
        arrow = m_arrowName + "_hover";
    }
    else
    {
        p.setPen(m_chalkColor);
    }
    p.translate(m_switcherRect.right() + padding, m_switcherRect.top());
    p.scale(16.250 / kWindowWidth, 25.0 / kWindowHeight);
    m_renderer->render(&p, arrow);
    p.resetMatrix();

    m_switcherRect.translate(0, -2);
    p.drawText(m_switcherRect, Qt::AlignVCenter|Qt::AlignRight, text);
}

QRect Kanagram::innerRect(const QRect &rect, const int xMargin, const int yMargin)
{
    QRect r = rect;

    if (xMargin>0)
    {
        r.setWidth(r.width() - 2 * xMargin);
        r.translate(xMargin, 0);
    }
    if (yMargin>0)
    {
        r.setHeight(r.height() - 2 * yMargin);
        r.translate(0, yMargin);
    }

    return r;
}

/** move on to the next word */
void Kanagram::slotNextAnagram()
{
    hideHint();
    m_game->nextAnagram();
    if (m_useSounds)
    {
        play("chalk.ogg");
    }
    m_inputBox->setPalette(QPalette());
    update();
}

void Kanagram::slotRevealWord()
{
    m_game->restoreWord();
    update();
}

/** move on to the next vocabulary */
void Kanagram::slotNextVocabulary()
{
    m_game->nextVocab();
    hideHint();
    m_game->nextAnagram();

    if (m_useSounds)
    {
        play("chalk.ogg");
    }

    KanagramSettings::setDefaultVocab(m_game->getFilename());
    KanagramSettings::self()->writeConfig();
    update();
}

/** go back to the previous vocabulary */
void Kanagram::slotPrevVocabulary()
{
    m_game->previousVocab();
    hideHint();
    m_game->nextAnagram();

    if (m_useSounds)
    {
        play("chalk.ogg");
    }

    KanagramSettings::setDefaultVocab(m_game->getFilename());
    KanagramSettings::self()->writeConfig();
    update();
}

void Kanagram::slotToggleHint()
{
    if (m_showHint)
    {
        m_showHint = false;
    }
    else
    {
        if (m_hintHideTime)
        {
            m_hintTimer->start(m_hintHideTime * 1000);
        }
        m_showHint = true;
        randomHintImage();
    }
    update();
}

void Kanagram::mousePressEvent(QMouseEvent *e)
{
    if (m_nextRect.contains(e->pos()))
    {
        slotNextAnagram();
    }

    if (m_configRect.contains(e->pos()))
    {
        slotShowSettings();
    }

    if (m_quitRect.contains(e->pos()))
    {
        qApp->quit();
    }

    if (m_revealRect.contains(e->pos()))
    {
        slotRevealWord();
    }

    if (m_logoRect.contains(e->pos()))
    {
        m_helpMenu->aboutApplication();
    }

    if (!m_showHint && m_overHelp)
    {
        if (m_handbookRect.contains(e->pos()))
        {
            m_helpMenu->appHelpActivated();
        }

        if (m_aboutKDERect.contains(e->pos()))
        {
            m_helpMenu->aboutKDE();
        }

        if (m_aboutAppRect.contains(e->pos()))
        {
            m_helpMenu->aboutApplication();
        }
    }

    if (m_hintBoxRect.contains(e->pos()))
    {
        hideHint();
    }

    if (m_switcherRect.contains(e->pos()) || m_arrowRect.contains(e->pos()))
    {
        if (!(e->button() == Qt::RightButton))
        {
            slotNextVocabulary();
        }
        else
        {
            slotPrevVocabulary();
        }
    }

    if (m_hintRect.contains(e->pos()))
    {
        slotToggleHint();
    }

    if (m_upRect.contains(e->pos()) && !m_inputBox->text().isEmpty())
    {
        checkWord();
    }
}

/** sets or clears flag based on whether p is in rect, and set changed if flag changes */
void Kanagram::CheckRect(const QRect &rect, const QPoint &p, bool &flag, bool &changed)
{
    if (rect.contains(p))
    {
        changed = !flag;
        flag = true;
    }
    else if (flag)
    {
        flag = false;
        changed = true;
    }
}

void Kanagram::mouseMoveEvent(QMouseEvent *e)
{
    QPoint p = e->pos();
    bool haveToUpdate = false;

    CheckRect(m_nextRect, p, m_overNext, haveToUpdate);
    CheckRect(m_configRect, p, m_overConfig, haveToUpdate);
    CheckRect(m_logoRect, p, m_overLogo, haveToUpdate);
    CheckRect(m_helpRect, p, m_overHelp, haveToUpdate);
    CheckRect(m_quitRect, p, m_overQuit, haveToUpdate);
    CheckRect(m_hintRect, p, m_overHint, haveToUpdate);
    CheckRect(m_hintBoxRect, p, m_overHintBox, haveToUpdate);
    CheckRect(m_revealRect, p, m_overReveal, haveToUpdate);
    CheckRect(m_upRect, p, m_overUp, haveToUpdate);
    CheckRect(m_aboutAppRect, p, m_overAboutApp, haveToUpdate);

    if (m_switcherRect.contains(p) || m_arrowRect.contains(p))
    {
        haveToUpdate = !m_overSwitcher;
        m_overSwitcher = true;
    }
    else if (m_overSwitcher)
    {
        m_overSwitcher = false;
        haveToUpdate = true;
    }

    if (!m_showHint)
    {
        CheckRect(m_handbookRect, p, m_overHandbook, haveToUpdate);
        CheckRect(m_aboutKDERect, p, m_overAboutKDE, haveToUpdate);
    }

    if (m_overAboutKDE || m_overHandbook || m_overSwitcher || m_overNext || m_overQuit
            || m_overConfig || m_overReveal || m_overHint || (m_overUp && !m_inputBox->text().isEmpty())
            || m_overAboutApp || m_overHintBox || m_overLogo)
    {
        this->setCursor(Qt::PointingHandCursor);
    }
    else
    {
        this->unsetCursor();
    }

    if (haveToUpdate)
    {
        update();
    }
}

void Kanagram::drawTextNew(QPainter &p, const QString &text, int textAlign, int xMargin, int yMargin, const QRect &rect, bool highlight, int fontSize)
{
    QRect r = innerRect(rect, xMargin, yMargin);
    QFont font = KGlobalSettings::generalFont();
    font.setPointSize(fontSize);
    font.setBold(true);
    p.setFont(font);

    const bool withMargin = false;
    if (withMargin)
    {
        p.fillRect(r, m_fillColor);
        p.setPen(QPen(Qt::black, 3));
        p.drawRoundRect(r.left(), r.top(), r.width(), r.height(), 15, 15);
    }

    if (highlight)
        p.setPen(m_chalkHighlightColor);
    else
        p.setPen(m_chalkColor);
    p.drawText(r, textAlign, text);
}

void Kanagram::checkWord()
{
    QPalette palette;
    if (m_inputBox->text().toLower().trimmed() == m_game->getWord())
    {
        if (m_useSounds) play("right.ogg");
        palette.setColor(m_inputBox->backgroundRole(), QColor(0, 255, 0));
        QTimer::singleShot(1000, this, SLOT(resetInputBox()));
        m_inputBox->clear();
        hideHint();
        m_game->nextAnagram();
    }
    else
    {
        if (m_useSounds) play("wrong.ogg");
        palette.setColor(m_inputBox->backgroundRole(), QColor(255, 0, 0));
        QTimer::singleShot(1000, this, SLOT(resetInputBox()));
        m_inputBox->clear();
    }
    m_inputBox->setPalette(palette);
    update();
}

void Kanagram::randomHintImage()
{
    unsigned long imageNum = m_randomImage.getLong(8);
    m_hintOverlayName = "eyes" + QString::number(imageNum + 1);
}

void Kanagram::slotShowSettings()
{
    if (!KConfigDialog::showDialog("settings"))
    {
        m_configDialog = new KConfigDialog( this, "settings", KanagramSettings::self() );
        m_configDialog->setAttribute(Qt::WA_DeleteOnClose);
        connect(m_configDialog, SIGNAL(finished()), this, SLOT(reloadSettings()));

        // add the main settings page
        MainSettings * mainSettingsPage = new MainSettings( m_configDialog );
        connect (mainSettingsPage, SIGNAL(settingsChanged()), this, SLOT(reloadSettings()));
        m_configDialog->addPage(mainSettingsPage , i18nc("@title:group main settings page name", "General" ), "configure" );

        // create and add the vocabsettings page
        m_vocabSettings = new VocabSettings( m_configDialog );
        m_configDialog->addPage(m_vocabSettings, i18n("Vocabularies"), "edit" );

        // now make and add the shortcuts page
        m_shortcutsEditor = new KShortcutsEditor(m_actionCollection, m_configDialog);
        m_configDialog->addPage(m_shortcutsEditor, i18n("Shortcuts"), "configure-shortcuts");
        connect(m_configDialog, SIGNAL(accepted()), this, SLOT(slotSaveSettings()));
        connect(m_configDialog, SIGNAL(rejected()), this, SLOT(slotSettingsCancelled()));
        connect(m_shortcutsEditor, SIGNAL(keyChange()), this, SLOT(slotEnableApplyButton()));

        // and add the KNS page
        m_configDialog->addPage( new NewStuff( m_configDialog ), i18n("New Stuff"), "get-hot-new-stuff" );

        m_configDialog->show();
    }
}

void Kanagram::slotSaveSettings()
{
  m_shortcutsEditor->save();
}

void Kanagram::slotSettingsCancelled()
{
  m_shortcutsEditor->undoChanges();
}

void Kanagram::slotEnableApplyButton()
{
  m_configDialog->enableButtonApply(true);
}

void Kanagram::hideHint()
{
    if (m_showHint == true)
    {
        m_showHint = false;
    }
    update();
}

void Kanagram::resetInputBox()
{
    m_inputBox->setPalette(QPalette());
}

void Kanagram::refreshVocabularies()
{
    if (m_game->refreshVocabList())
    {
        // vocab/word are no longer valid, so get new ones and hide the hint
        m_game->nextVocab();
        m_game->nextAnagram();
        hideHint();

        if (m_useSounds)
        {
            play("chalk.ogg");
        }

        // save the default vocab now that it's changed
        KanagramSettings::setDefaultVocab(m_game->getFilename());
        KanagramSettings::self()->writeConfig();
    }
    m_vocabSettings->refreshView();
}

void Kanagram::play(const QString &filename)
{
    if (!filename.isEmpty())
    {
        QString soundFile = KStandardDirs::locate("appdata", "sounds/" + filename);
        if (!soundFile.isEmpty())
        {
            if (!m_player)
            {
                m_player = Phonon::createPlayer(Phonon::GameCategory, soundFile);
            }
            else
            {
                m_player->setCurrentSource(soundFile);
            }
            m_player->play();
        }
    }
}

void Kanagram::slotFileError(const QString &filename)
{
    QString msg = i18n("File %1 cannot be found.\n Please ensure that Kanagram is properly installed.", filename);
    KMessageBox::sorry(this, msg, i18n("Error"));
    exit(0);
}

/** show the popup menu of vocabularies, and allow choosing of one */
void Kanagram::slotChooseVocabulary()
{
}


#include "kanagram.moc"
