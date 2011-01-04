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
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.           *
 ***************************************************************************/

#include <iostream>
using namespace std;

#include <config.h>

#include <tqcursor.h>
#include <tqpainter.h>
#include <tqpixmap.h>
#include <tqtimer.h>
#include <tqstring.h>
#include <tqfontmetrics.h>
#include <tqdir.h>

#include <kaction.h>
#include <kapplication.h>
#include <kconfig.h>
#include <khelpmenu.h>
#include <kinputdialog.h>
#include <klocale.h>
#include <kmessagebox.h>
#include <kpopupmenu.h>
#include <kstandarddirs.h>
#include <kconfigdialog.h>
#include <kconfigskeleton.h>
#include <krandomsequence.h>
#include <kdebug.h>

#include "kanagram.h"
#include "kanagramsettings.h"
#include "mainsettings.h"
#include "vocabsettings.h"
#include "newstuff.h"

static const char* m_textRevealWord = I18N_NOOP("reveal word");
static const char* m_textHint = I18N_NOOP("hint");

Kanagram::Kanagram() : TQWidget(0, 0, WStaticContents | WNoAutoErase), m_overNext(false), m_overConfig(false), m_overHelp(false), m_overQuit(false), m_overReveal(false), m_overHint(false), m_overUp(false), m_overHintBox(false), m_showHint(false)
{
	m_game = new KanagramGame(this);

	m_back = new TQPixmap(locate("appdata", "images/kanagram.png"));
	m_aboutKDEOverlay = new TQPixmap(locate("appdata", "images/kicon.png"));
	m_aboutKDEOverlayOver = new TQPixmap(locate("appdata", "images/kiconover.png"));
	m_aboutAppOverlay = new TQPixmap(locate("appdata", "images/appicon.png"));
	m_aboutAppOverlayOver = new TQPixmap(locate("appdata", "images/appiconover.png"));
	m_handbookOverlay = new TQPixmap(locate("appdata", "images/handbookicon.png"));
	m_handbookOverlayOver = new TQPixmap(locate("appdata", "images/handbookiconover.png"));
	m_card = new TQPixmap(locate("appdata", "images/card.png"));

	m_next = new TQPixmap(locate("appdata", "images/next.png"));
	m_nextOver = new TQPixmap(locate("appdata", "images/nextover.png"));
	m_config = new TQPixmap(locate("appdata", "images/config.png"));
	m_configOver = new TQPixmap(locate("appdata", "images/configover.png"));
	m_help = new TQPixmap(locate("appdata", "images/help.png"));
	m_helpOver = new TQPixmap(locate("appdata", "images/helpover.png"));
	m_quit = new TQPixmap(locate("appdata", "images/quit.png"));
	m_quitOver = new TQPixmap(locate("appdata", "images/quitover.png"));

	m_up = new TQPixmap(locate("appdata", "images/up.png"));
	m_upOver = new TQPixmap(locate("appdata", "images/upover.png"));
	m_upDisabled = new TQPixmap(locate("appdata", "images/updisabled.png"));

	m_nextRect = TQRect(477, 31, 134, 76);
	m_configRect = TQRect(477, 122, 134, 76);
	m_helpRect = TQRect(477, 212, 134, 76);
	m_quitRect = TQRect(453, 352, 182, 104);

	m_hintBoxRect = TQRect(446, 207, 171, 85);
	m_upRect = TQRect(341, 425, 55, 33);
	m_aboutKDERect = TQRect(567, 213, 44, 44);
	m_aboutAppRect = TQRect(522, 213, 44, 44);
	m_handbookRect = TQRect(478, 213, 44, 44);
	m_arrowRect = TQRect(380, 134, 13, 20);
	m_logoRect = TQRect(76, 24, 297, 50);

	//blackboardRect intentionally wrong to make sure fonts align correctly
	m_blackboardRect = TQRect(41, 116, 366, 255);

	setupRects();

	setMouseTracking(true);
	setFixedSize(650, 471);
	show();
	
	m_chalkColor = TQColor(155, 155, 155);
	m_chalkHighlightColor = TQColor(255, 255, 255);
	m_fillColor = TQColor(45, 45, 45);
	m_fontColor = TQColor(55, 55, 55);
	m_fontHighlightColor = TQColor(99, 99, 99);

	loadSettings();

	m_hintTimer = new TQTimer(this);
	
	m_helpMenu = new KHelpMenu(this, kapp->aboutData());
	
	m_inputBox = new TQLineEdit(this);
	m_inputBox->setGeometry(TQRect(52, 427, 273, 29));
	m_inputBox->setFrame(false);
	
	connect(m_inputBox, TQT_SIGNAL(returnPressed()), this, TQT_SLOT(checkWord()));
	connect(m_hintTimer, TQT_SIGNAL(timeout()), this, TQT_SLOT(hideHint()));
	connect(m_inputBox, TQT_SIGNAL(textChanged(const TQString &)), this, TQT_SLOT(update()));
	
	TQFont f = TQFont();
	f.setPointSize(17);
	m_inputBox->setFont(f);
	m_inputBox->show();

	m_font = KGlobalSettings::generalFont();

	//Initialize the sound server
	#ifndef WITHOUT_ARTS
		m_artsDispatcher = new KArtsDispatcher();
		m_artsServer = new KArtsServer();
		m_artsFactory = new KDE::PlayObjectFactory(m_artsServer->server());
	#endif
}

Kanagram::~Kanagram()
{
}

void Kanagram::loadSettings()
{
	TQString hideTime = KanagramSettings::hintHideTime();
	if(hideTime[0].isDigit())
		m_hintHideTime = hideTime[0].digitValue();
	else
		m_hintHideTime = 0;
	
	m_useSounds = KanagramSettings::useSounds();
	m_useStandardFonts = KanagramSettings::useStandardFonts();

	if(m_useStandardFonts)
	{
		m_blackboardFont = KGlobalSettings::generalFont();
		m_arrow = new TQPixmap(locate("appdata", "images/basicarrow.png"));
		m_arrowOver = new TQPixmap(locate("appdata", "images/basicarrowover.png"));
	}
	else
	{
		m_blackboardFont = TQFont("squeaky chalk sound");
		m_arrow = new TQPixmap(locate("appdata", "images/arrow.png"));
		m_arrowOver = new TQPixmap(locate("appdata", "images/arrowover.png"));
	}

	m_game->refreshVocabList();
}

void Kanagram::setupRects()
{
	TQPainter tmpp(this);
	TQFont font = m_blackboardFont;
	font.setPointSize(14);
	font.setBold(true);
	tmpp.setFont(font);
	int yOffset = 0;
	//set appropriate yOffset for different fonts
	if(m_useStandardFonts) yOffset = 6;
	TQRect r = innerRect(m_blackboardRect, 6, yOffset);
	m_hintRect = tmpp.boundingRect(r, Qt::AlignBottom|Qt::AlignLeft, i18n(m_textHint));
	m_revealRect = tmpp.boundingRect(r, Qt::AlignBottom|Qt::AlignRight, i18n(m_textRevealWord));
	tmpp.end();
}

void Kanagram::paintEvent(TQPaintEvent *)
{
	TQPixmap buf(width(), height());
	TQPainter p(&buf);
	
	p.drawPixmap(0, 0, *m_back);

	if(m_overNext)
		p.drawPixmap(525, 38, *m_nextOver);
	else
		p.drawPixmap(525, 38, *m_next);
	if(m_overConfig)
		p.drawPixmap(525, 130, *m_configOver);
	else
		p.drawPixmap(525, 130, *m_config);
	if(m_overHelp)
		p.drawPixmap(525, 218, *m_helpOver);
	else
		p.drawPixmap(525, 218, *m_help);
	if(m_overQuit)
		p.drawPixmap(520, 362, *m_quitOver);
	else
		p.drawPixmap(520, 362, *m_quit);

	//draw main Anagram
	drawTextNew(p, m_game->getAnagram(), Qt::AlignCenter, 10, 10, m_blackboardRect, true, 28);

	int yOffset = 0;
	//set appropriate yOffset for different fonts
	if(m_useStandardFonts) yOffset = 6;
	//draw text using appropriate yOffset
	drawTextNew(p, i18n(m_textRevealWord), Qt::AlignBottom | Qt::AlignRight, 6, yOffset, m_blackboardRect, m_overReveal, 14);
	drawTextNew(p, i18n(m_textHint), Qt::AlignBottom | Qt::AlignLeft, 6, yOffset, m_blackboardRect, m_overHint, 14);
	
	//draw vocab switcher
	drawSwitcher(p, 9, 8);

	p.setPen(TQPen(black, 3));

	//Draw the border of the input box
	TQRect borderRect = m_inputBox->tqgeometry();
	borderRect.setLeft(borderRect.left() - 2);
	borderRect.setTop(borderRect.top() - 2);
	borderRect.setWidth(borderRect.width() + 2 * 1);
	borderRect.setHeight(borderRect.height() + 2 * 1);
	p.drawRoundRect(borderRect, 10, 5);

	//Draw the border of the Up arrow
	borderRect = m_upRect;
	p.fillRect(borderRect, m_fillColor);
	p.drawRoundRect(borderRect, 10, 5);
	
	if(m_overUp && m_inputBox->text() != "")
		p.drawPixmap(350, 431, *m_upOver);
	else if(m_inputBox->text() == "")
		p.drawPixmap(350, 431, *m_upDisabled);
	else
		p.drawPixmap(350, 431, *m_up);

	if(m_showHint)
	{
		p.drawPixmap(439, 204, *m_hintOverlay);
		TQFont f = TQFont(m_font);
		f.setWeight(TQFont::Bold);
		f.setPointSize(10);
		p.setFont(f);
		p.drawText(459, 217, 148, 67, WordBreak | AlignCenter, m_game->getHint());
	}

	if(m_overHelp && !m_showHint)
	{
		p.drawPixmap(456, 275, *m_card);
		if(m_overAboutApp)
		{
			p.drawPixmap(522, 213, *m_aboutAppOverlay);
			drawHelpText(p, i18n("About Kanagram"));
		}
		else
			p.drawPixmap(522, 213, *m_aboutAppOverlayOver);
		if(m_overAboutKDE)
		{
			p.drawPixmap(567, 213, *m_aboutKDEOverlay);
			drawHelpText(p, i18n("About KDE"));
		}
		else
			p.drawPixmap(567, 213, *m_aboutKDEOverlayOver);
		if(m_overHandbook)
		{
			p.drawPixmap(478, 213, *m_handbookOverlay);
			drawHelpText(p, i18n("Kanagram Handbook"));
		}
		else
			p.drawPixmap(478, 213, *m_handbookOverlayOver);
	}
	else if(m_overNext)
	{
		p.drawPixmap(456, 275, *m_card);
		drawHelpText(p, i18n("Next Word"));
	}
	else if(m_overConfig)
	{
		p.drawPixmap(456, 275, *m_card);
		drawHelpText(p, i18n("Configure Kanagram"));
	}
	else if(m_overQuit)
	{
		p.drawPixmap(456, 275, *m_card);
		drawHelpText(p, i18n("Quit Kanagram"));
	}

	bitBlt(this, 0, 0, &buf);
}

void Kanagram::drawHelpText(TQPainter &p, TQString text)
{
	p.save();
	TQFont font = m_font;
	font.setPointSize(12);
	p.setFont(font);
	p.rotate(-3.29);
	p.setPen(black);
	p.drawText(450, 340, text.section(' ', 0, 0));
	p.drawText(450, 360, text.section(' ', 1));
	p.restore();
}

void Kanagram::drawSwitcherText(TQPainter &p, TQString text)
{
	p.save();
	TQFont font = m_blackboardFont;
	font.setPointSize(14);
	TQFontMetrics fm(font);
	int width = fm.width(text);
	int height = fm.height();
	m_switcherRect = TQRect(380 - width, 150 - height, width, height);
	p.setFont(font);
	if(!m_overSwitcher)
		p.setPen(m_chalkColor);
	else
		p.setPen(m_chalkHighlightColor);
	p.drawText(380 - width, 150, text);
	p.restore();
}

void Kanagram::drawSwitcher(TQPainter &p, const int xMargin, const int yMargin)
{
	const int padding = 5;
	TQString text = m_game->getDocTitle();
	TQFont font = m_blackboardFont;
	font.setPointSize(14);
	TQFontMetrics fm(font);
	TQRect r = innerRect(m_blackboardRect, xMargin, yMargin);
	r.normalize();
	r.moveBy(- padding - (m_overSwitcher ? m_arrowOver : m_arrow )->width(), yMargin);
	r.setHeight( (m_overSwitcher ? m_arrowOver : m_arrow )->height());
	m_switcherRect = p.boundingRect(r, Qt::AlignVCenter|Qt::AlignRight, text);
	p.setFont(font);
	if (m_overSwitcher)
	{
		p.setPen(m_chalkHighlightColor);
		p.drawPixmap(m_switcherRect.right() + padding, m_switcherRect.top(), *m_arrowOver);
	}
	else
	{
		p.setPen(m_chalkColor);
		p.drawPixmap(m_switcherRect.right() + padding, m_switcherRect.top(), *m_arrow);
	}
	m_switcherRect.moveBy(0, -2);
	p.drawText(m_switcherRect, Qt::AlignVCenter|Qt::AlignRight, text);
}

TQRect Kanagram::innerRect(const TQRect &rect, const int xMargin, const int yMargin)
{
	TQRect r = rect;

	if (xMargin>0)
	{
		r.setWidth(r.width() - 2 * xMargin);
		r.moveBy(xMargin, 0);
	}
	if (yMargin>0)
	{
		r.setHeight(r.height() - 2 * yMargin);
		r.moveBy(0, yMargin);
	}

	return r;
}

void Kanagram::mousePressEvent(TQMouseEvent *e)
{
	
	if (m_nextRect.tqcontains(e->pos()))
	{
		hideHint();
		m_game->nextAnagram();
		if(m_useSounds) play("chalk.ogg");
		m_inputBox->unsetPalette();
		update();
	}

	if(m_configRect.tqcontains(e->pos()))
	{
		showSettings();
	}

	if(m_quitRect.tqcontains(e->pos()))
	{
		kapp->quit();
	}

	if(m_revealRect.tqcontains(e->pos()))
	{
		m_game->restoreWord();
		update();
	}

	if(m_logoRect.tqcontains(e->pos()))
	{
		m_helpMenu->aboutApplication();
	}

	if(!m_showHint && m_overHelp)
	{
		if(m_handbookRect.tqcontains(e->pos()))
		{
			m_helpMenu->appHelpActivated();
		}

		if(m_aboutKDERect.tqcontains(e->pos()))
		{
			m_helpMenu->aboutKDE();
		}

		if(m_aboutAppRect.tqcontains(e->pos()))
		{
			m_helpMenu->aboutApplication();
		}
	}

	if(m_hintBoxRect.tqcontains(e->pos()))
	{
		hideHint();
	}

	if(m_switcherRect.tqcontains(e->pos()) || m_arrowRect.tqcontains(e->pos()))
	{
		if(!(e->button() == RightButton))
			m_game->nextVocab();
		else
			m_game->previousVocab();
		hideHint();
		m_game->nextAnagram();
		if(m_useSounds) play("chalk.ogg");
		KanagramSettings::setDefaultVocab(m_game->getFilename());
		KanagramSettings::writeConfig();
		update();
	}

	if(m_hintRect.tqcontains(e->pos()))
	{
		if(m_showHint == true) m_showHint = false;
		else
		{
			if(m_hintHideTime)
			{
				m_hintTimer->start(m_hintHideTime * 1000, TRUE);
			}
			m_showHint = true;
			randomHintImage();
		}
		update();
	}

	if(m_upRect.tqcontains(e->pos()) && m_inputBox->text() != "")
	{
		if(m_inputBox->text().lower().stripWhiteSpace() == m_game->getWord())
		{
			if(m_useSounds) play("right.ogg");
			m_inputBox->setPaletteBackgroundColor(TQColor(0, 255, 0));
			TQTimer::singleShot(1000, this, TQT_SLOT(resetInputBox()));
			m_inputBox->clear();
			hideHint();
			m_game->nextAnagram();
			update();
		}
		else
		{
			if(m_useSounds) play("wrong.ogg");
			m_inputBox->setPaletteBackgroundColor(TQColor(255, 0, 0));
			TQTimer::singleShot(1000, this, TQT_SLOT(resetInputBox()));
			m_inputBox->clear();
			update();
		}
	}
}

void Kanagram::mouseMoveEvent(TQMouseEvent *e)
{
	updateButtonHighlighting(e->pos());
}

void Kanagram::updateButtonHighlighting(const TQPoint &p)
{
	bool haveToUpdate;
	haveToUpdate = false;
	
	if (m_nextRect.tqcontains(p))
	{
		if (!m_overNext)
		{
			m_overNext = true;
			haveToUpdate = true;
		}
	}
	else if (m_overNext)
	{
		m_overNext = false;
		haveToUpdate = true;
	}

	if(m_configRect.tqcontains(p))
	{
		if(!m_overConfig)
		{
			m_overConfig = true;
			haveToUpdate = true;
		}
	}
	else if(m_overConfig)
	{
		m_overConfig = false;
		haveToUpdate = true;
	}

	if(m_helpRect.tqcontains(p))
	{
		if(!m_overHelp)
		{
			m_overHelp = true;
			haveToUpdate = true;
		}
	}
	else if(m_overHelp)
	{
		m_overHelp = false;
		haveToUpdate = true;
	}

	if(m_quitRect.tqcontains(p))
	{
		if(!m_overQuit)
		{
			m_overQuit = true;
			haveToUpdate = true;
		}
	}
	else if(m_overQuit)
	{
		m_overQuit = false;
		haveToUpdate = true;
	}

	if(m_hintRect.tqcontains(p))
	{
		if(!m_overHint)
		{
			m_overHint = true;
			haveToUpdate = true;
		}
	}
	else if(m_overHint)
	{
		m_overHint = false;
		haveToUpdate = true;
	}

	if(m_hintBoxRect.tqcontains(p))
	{
		if(!m_overHintBox)
		{
			m_overHintBox = true;
			haveToUpdate = true;
		}
	}
	else if(m_overHintBox)
	{
		m_overHintBox = false;
		haveToUpdate = true;
	}
	
	if(m_revealRect.tqcontains(p))
	{
		if(!m_overReveal)
		{
			m_overReveal = true;
			haveToUpdate = true;
		}
	}
	else if(m_overReveal)
	{
		m_overReveal = false;
		haveToUpdate = true;
	}

	if(m_upRect.tqcontains(p))
	{
		if(!m_overUp)
		{
			m_overUp = true;
			haveToUpdate = true;
		}
	}
	else if(m_overUp)
	{
		m_overUp = false;
		haveToUpdate = true;
	}

	if(m_switcherRect.tqcontains(p) || m_arrowRect.tqcontains(p))
	{
		if(!m_overSwitcher)
		{
			m_overSwitcher = true;
			haveToUpdate = true;
		}
	}
	else if(m_overSwitcher)
	{
		m_overSwitcher = false;
		haveToUpdate = true;
	}

	if(m_aboutAppRect.tqcontains(p))
	{
		if(!m_overAboutApp)
		{
			m_overAboutApp = true;
			haveToUpdate = true;
		}
	}
	else if(m_overAboutApp)
	{
		m_overAboutApp = false;
		haveToUpdate = true;
	}

	if(!m_showHint)
	{
		if(m_handbookRect.tqcontains(p))
		{
			if(!m_overHandbook)
			{
				m_overHandbook = true;
				haveToUpdate = true;
			}
		}
		else if(m_overHandbook)
		{
			m_overHandbook = false;
			haveToUpdate = true;
		}
	
		if(m_aboutKDERect.tqcontains(p))
		{
			if(!m_overAboutKDE)
			{
				m_overAboutKDE = true;
				haveToUpdate = true;
			}
		}
		else if(m_overAboutKDE)
		{
			m_overAboutKDE = false;
			haveToUpdate = true;
		}
	}

	if(m_overAboutKDE || m_overHandbook || m_overSwitcher || m_overNext || m_overQuit || m_overConfig || m_overReveal || m_overHint || (m_overUp && m_inputBox->text() != "") || m_overAboutApp || m_overHintBox)
		this->setCursor(PointingHandCursor);
	else
		this->unsetCursor();

	if (haveToUpdate) update();
}

void Kanagram::drawText(TQPainter &p, const TQString &text, const TQPoint &center, bool withMargin, int xMargin, int yMargin, TQRect *rect, bool highlight, int fontSize)
{
	TQRect r;
	TQFont font = m_blackboardFont;
	font.setPointSize(fontSize);
	font.setBold(true);
	p.setFont(font);
	
	r = p.boundingRect(TQRect(), Qt::AlignAuto, text);
	r = TQRect(0, 0, r.width() + xMargin, r.height() + yMargin);
	r.moveBy(center.x() - r.width() / 2, center.y() - r.height() / 2);

	if (withMargin)
	{
		p.fillRect(r, m_fillColor);
		p.setPen(TQPen(black, 3));
		p.drawRoundRect(r.left(), r.top(), r.width(), r.height(), 15, 15);
	}
	
	if (!highlight) p.setPen(m_chalkColor);
	else p.setPen(m_chalkHighlightColor);
	p.drawText(r, Qt::AlignCenter, text);
	
	if(rect) *rect = r;
}

void Kanagram::drawTextNew(TQPainter &p, const TQString &text, int textAlign, int xMargin, int yMargin, const TQRect &rect, bool highlight, int fontSize)
{
	TQRect r = innerRect(rect, xMargin, yMargin);
	TQFont font = m_blackboardFont;
	font.setPointSize(fontSize);
	font.setBold(true);
	p.setFont(font);
	
	const bool withMargin = false;
	if (withMargin)
	{
		p.fillRect(r, m_fillColor);
		p.setPen(TQPen(black, 3));
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
	TQPoint p = m_upRect.topLeft() + TQPoint( 1, 1 );
	TQMouseEvent *e = new TQMouseEvent( TQEvent::MouseButtonPress, p, Qt::LeftButton, Qt::NoButton );
	mousePressEvent(e);
}

void Kanagram::randomHintImage()
{
	unsigned long imageNum = m_randomImage.getLong(8);
	TQString dir = "images/eyes" + TQString::number(imageNum + 1) + ".png";
	m_hintOverlay = new TQPixmap(locate("appdata", dir));
}

void Kanagram::showSettings()
{
	if(KConfigDialog::showDialog("settings"))
		return;

	KConfigDialog *configDialog = new KConfigDialog( this, "settings", KanagramSettings::self() );
	configDialog->addPage( new MainSettings( configDialog ), i18n( "General" ), "configure" );
	m_vocabSettings = new VocabSettings( configDialog );
	configDialog->addPage( m_vocabSettings, i18n("Vocabularies"), "edit" );
	configDialog->addPage( new NewStuff( configDialog ), i18n("New Stuff"), "knewstuff" );
	connect(configDialog, TQT_SIGNAL(settingsChanged()), this, TQT_SLOT(loadSettings()));
	connect(configDialog, TQT_SIGNAL(applyClicked()), this, TQT_SLOT(refreshVocabularies()));
	configDialog->exec();
	delete configDialog;
}

void Kanagram::hideHint()
{
	if(m_showHint == true) m_showHint = false;
	update();
}

void Kanagram::resetInputBox()
{
	m_inputBox->unsetPalette();
}

void Kanagram::refreshVocabularies()
{
	kdDebug() << "Refreshing vocab list..." << endl;
	m_game->refreshVocabList();
	m_game->nextVocab();
	hideHint();
	m_game->nextAnagram();
	if(m_useSounds) play("chalk.ogg");
	KanagramSettings::setDefaultVocab(m_game->getFilename());
	KanagramSettings::writeConfig();
	m_vocabSettings->refreshView();
}

void Kanagram::play(TQString filename)
{
	#ifndef WITHOUT_ARTS
		KDE::PlayObject *playobj = m_artsFactory->createPlayObject(locate("appdata", "sounds/" + filename), true);
		playobj->play();
	#else
		(void)filename;
	#endif
}

#include "kanagram.moc"
