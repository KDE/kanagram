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

#include <qcursor.h>
#include <qpainter.h>
#include <qpixmap.h>
#include <qtimer.h>
#include <qstring.h>
#include <qfontmetrics.h>
#include <qdir.h>

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

Kanagram::Kanagram() : QWidget(0, 0, WStaticContents | WNoAutoErase), m_overNext(false), m_overConfig(false), m_overHelp(false), m_overQuit(false), m_overReveal(false), m_overHint(false), m_overUp(false), m_overHintBox(false), m_showHint(false)
{
	m_game = new KanagramGame(this);

	m_back = new QPixmap(locate("appdata", "images/kanagram.png"));
	m_aboutKDEOverlay = new QPixmap(locate("appdata", "images/kicon.png"));
	m_aboutKDEOverlayOver = new QPixmap(locate("appdata", "images/kiconover.png"));
	m_aboutAppOverlay = new QPixmap(locate("appdata", "images/appicon.png"));
	m_aboutAppOverlayOver = new QPixmap(locate("appdata", "images/appiconover.png"));
	m_handbookOverlay = new QPixmap(locate("appdata", "images/handbookicon.png"));
	m_handbookOverlayOver = new QPixmap(locate("appdata", "images/handbookiconover.png"));
	m_card = new QPixmap(locate("appdata", "images/card.png"));

	m_next = new QPixmap(locate("appdata", "images/next.png"));
	m_nextOver = new QPixmap(locate("appdata", "images/nextover.png"));
	m_config = new QPixmap(locate("appdata", "images/config.png"));
	m_configOver = new QPixmap(locate("appdata", "images/configover.png"));
	m_help = new QPixmap(locate("appdata", "images/help.png"));
	m_helpOver = new QPixmap(locate("appdata", "images/helpover.png"));
	m_quit = new QPixmap(locate("appdata", "images/quit.png"));
	m_quitOver = new QPixmap(locate("appdata", "images/quitover.png"));

	m_up = new QPixmap(locate("appdata", "images/up.png"));
	m_upOver = new QPixmap(locate("appdata", "images/upover.png"));
	m_upDisabled = new QPixmap(locate("appdata", "images/updisabled.png"));

	m_nextRect = QRect(477, 31, 134, 76);
	m_configRect = QRect(477, 122, 134, 76);
	m_helpRect = QRect(477, 212, 134, 76);
	m_quitRect = QRect(453, 352, 182, 104);

	m_hintBoxRect = QRect(446, 207, 171, 85);
	m_upRect = QRect(341, 425, 55, 33);
	m_aboutKDERect = QRect(567, 213, 44, 44);
	m_aboutAppRect = QRect(522, 213, 44, 44);
	m_handbookRect = QRect(478, 213, 44, 44);
	m_arrowRect = QRect(380, 134, 13, 20);
	m_logoRect = QRect(76, 24, 297, 50);

	//blackboardRect intentionally wrong to make sure fonts align correctly
	m_blackboardRect = QRect(41, 116, 366, 255);

	setupRects();

	setMouseTracking(true);
	setFixedSize(650, 471);
	show();
	
	m_chalkColor = QColor(155, 155, 155);
	m_chalkHighlightColor = QColor(255, 255, 255);
	m_fillColor = QColor(45, 45, 45);
	m_fontColor = QColor(55, 55, 55);
	m_fontHighlightColor = QColor(99, 99, 99);

	loadSettings();

	m_hintTimer = new QTimer(this);
	
	m_helpMenu = new KHelpMenu(this, kapp->aboutData());
	
	m_inputBox = new QLineEdit(this);
	m_inputBox->setGeometry(QRect(52, 427, 273, 29));
	m_inputBox->setFrame(false);
	
	connect(m_inputBox, SIGNAL(returnPressed()), this, SLOT(checkWord()));
	connect(m_hintTimer, SIGNAL(timeout()), this, SLOT(hideHint()));
	connect(m_inputBox, SIGNAL(textChanged(const QString &)), this, SLOT(update()));
	
	QFont f = QFont();
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
	QString hideTime = KanagramSettings::hintHideTime();
	if(hideTime[0].isDigit())
		m_hintHideTime = hideTime[0].digitValue();
	else
		m_hintHideTime = 0;
	
	m_useSounds = KanagramSettings::useSounds();
	m_useStandardFonts = KanagramSettings::useStandardFonts();

	if(m_useStandardFonts)
	{
		m_blackboardFont = KGlobalSettings::generalFont();
		m_arrow = new QPixmap(locate("appdata", "images/basicarrow.png"));
		m_arrowOver = new QPixmap(locate("appdata", "images/basicarrowover.png"));
	}
	else
	{
		m_blackboardFont = QFont("squeaky chalk sound");
		m_arrow = new QPixmap(locate("appdata", "images/arrow.png"));
		m_arrowOver = new QPixmap(locate("appdata", "images/arrowover.png"));
	}

	m_game->refreshVocabList();
}

void Kanagram::setupRects()
{
	QPainter tmpp(this);
	QFont font = m_blackboardFont;
	font.setPointSize(14);
	font.setBold(true);
	tmpp.setFont(font);
	int yOffset = 0;
	//set appropriate yOffset for different fonts
	if(m_useStandardFonts) yOffset = 6;
	QRect r = innerRect(m_blackboardRect, 6, yOffset);
	m_hintRect = tmpp.boundingRect(r, Qt::AlignBottom|Qt::AlignLeft, i18n(m_textHint));
	m_revealRect = tmpp.boundingRect(r, Qt::AlignBottom|Qt::AlignRight, i18n(m_textRevealWord));
	tmpp.end();
}

void Kanagram::paintEvent(QPaintEvent *)
{
	QPixmap buf(width(), height());
	QPainter p(&buf);
	
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

	p.setPen(QPen(black, 3));

	//Draw the border of the input box
	QRect borderRect = m_inputBox->geometry();
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
		QFont f = QFont(m_font);
		f.setWeight(QFont::Bold);
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

void Kanagram::drawHelpText(QPainter &p, QString text)
{
	p.save();
	QFont font = m_font;
	font.setPointSize(12);
	p.setFont(font);
	p.rotate(-3.29);
	p.setPen(black);
	p.drawText(450, 340, text.section(' ', 0, 0));
	p.drawText(450, 360, text.section(' ', 1));
	p.restore();
}

void Kanagram::drawSwitcherText(QPainter &p, QString text)
{
	p.save();
	QFont font = m_blackboardFont;
	font.setPointSize(14);
	QFontMetrics fm(font);
	int width = fm.width(text);
	int height = fm.height();
	m_switcherRect = QRect(380 - width, 150 - height, width, height);
	p.setFont(font);
	if(!m_overSwitcher)
		p.setPen(m_chalkColor);
	else
		p.setPen(m_chalkHighlightColor);
	p.drawText(380 - width, 150, text);
	p.restore();
}

void Kanagram::drawSwitcher(QPainter &p, const int xMargin, const int yMargin)
{
	const int padding = 5;
	QString text = m_game->getDocTitle();
	QFont font = m_blackboardFont;
	font.setPointSize(14);
	QFontMetrics fm(font);
	QRect r = innerRect(m_blackboardRect, xMargin, yMargin);
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

QRect Kanagram::innerRect(const QRect &rect, const int xMargin, const int yMargin)
{
	QRect r = rect;

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

void Kanagram::mousePressEvent(QMouseEvent *e)
{
	
	if (m_nextRect.contains(e->pos()))
	{
		hideHint();
		m_game->nextAnagram();
		if(m_useSounds) play("chalk.ogg");
		m_inputBox->unsetPalette();
		update();
	}

	if(m_configRect.contains(e->pos()))
	{
		showSettings();
	}

	if(m_quitRect.contains(e->pos()))
	{
		kapp->quit();
	}

	if(m_revealRect.contains(e->pos()))
	{
		m_game->restoreWord();
		update();
	}

	if(m_logoRect.contains(e->pos()))
	{
		m_helpMenu->aboutApplication();
	}

	if(!m_showHint && m_overHelp)
	{
		if(m_handbookRect.contains(e->pos()))
		{
			m_helpMenu->appHelpActivated();
		}

		if(m_aboutKDERect.contains(e->pos()))
		{
			m_helpMenu->aboutKDE();
		}

		if(m_aboutAppRect.contains(e->pos()))
		{
			m_helpMenu->aboutApplication();
		}
	}

	if(m_hintBoxRect.contains(e->pos()))
	{
		hideHint();
	}

	if(m_switcherRect.contains(e->pos()) || m_arrowRect.contains(e->pos()))
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

	if(m_hintRect.contains(e->pos()))
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

	if(m_upRect.contains(e->pos()) && m_inputBox->text() != "")
	{
		if(m_inputBox->text().lower().stripWhiteSpace() == m_game->getWord())
		{
			if(m_useSounds) play("right.ogg");
			m_inputBox->setPaletteBackgroundColor(QColor(0, 255, 0));
			QTimer::singleShot(1000, this, SLOT(resetInputBox()));
			m_inputBox->clear();
			hideHint();
			m_game->nextAnagram();
			update();
		}
		else
		{
			if(m_useSounds) play("wrong.ogg");
			m_inputBox->setPaletteBackgroundColor(QColor(255, 0, 0));
			QTimer::singleShot(1000, this, SLOT(resetInputBox()));
			m_inputBox->clear();
			update();
		}
	}
}

void Kanagram::mouseMoveEvent(QMouseEvent *e)
{
	updateButtonHighlighting(e->pos());
}

void Kanagram::updateButtonHighlighting(const QPoint &p)
{
	bool haveToUpdate;
	haveToUpdate = false;
	
	if (m_nextRect.contains(p))
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

	if(m_configRect.contains(p))
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

	if(m_helpRect.contains(p))
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

	if(m_quitRect.contains(p))
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

	if(m_hintRect.contains(p))
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

	if(m_hintBoxRect.contains(p))
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
	
	if(m_revealRect.contains(p))
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

	if(m_upRect.contains(p))
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

	if(m_switcherRect.contains(p) || m_arrowRect.contains(p))
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

	if(m_aboutAppRect.contains(p))
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
		if(m_handbookRect.contains(p))
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
	
		if(m_aboutKDERect.contains(p))
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

void Kanagram::drawText(QPainter &p, const QString &text, const QPoint &center, bool withMargin, int xMargin, int yMargin, QRect *rect, bool highlight, int fontSize)
{
	QRect r;
	QFont font = m_blackboardFont;
	font.setPointSize(fontSize);
	font.setBold(true);
	p.setFont(font);
	
	r = p.boundingRect(QRect(), Qt::AlignAuto, text);
	r = QRect(0, 0, r.width() + xMargin, r.height() + yMargin);
	r.moveBy(center.x() - r.width() / 2, center.y() - r.height() / 2);

	if (withMargin)
	{
		p.fillRect(r, m_fillColor);
		p.setPen(QPen(black, 3));
		p.drawRoundRect(r.left(), r.top(), r.width(), r.height(), 15, 15);
	}
	
	if (!highlight) p.setPen(m_chalkColor);
	else p.setPen(m_chalkHighlightColor);
	p.drawText(r, Qt::AlignCenter, text);
	
	if(rect) *rect = r;
}

void Kanagram::drawTextNew(QPainter &p, const QString &text, int textAlign, int xMargin, int yMargin, const QRect &rect, bool highlight, int fontSize)
{
	QRect r = innerRect(rect, xMargin, yMargin);
	QFont font = m_blackboardFont;
	font.setPointSize(fontSize);
	font.setBold(true);
	p.setFont(font);
	
	const bool withMargin = false;
	if (withMargin)
	{
		p.fillRect(r, m_fillColor);
		p.setPen(QPen(black, 3));
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
	QPoint p = m_upRect.topLeft() + QPoint( 1, 1 );
	QMouseEvent *e = new QMouseEvent( QEvent::MouseButtonPress, p, Qt::LeftButton, Qt::NoButton );
	mousePressEvent(e);
}

void Kanagram::randomHintImage()
{
	unsigned long imageNum = m_randomImage.getLong(8);
	QString dir = "images/eyes" + QString::number(imageNum + 1) + ".png";
	m_hintOverlay = new QPixmap(locate("appdata", dir));
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
	connect(configDialog, SIGNAL(settingsChanged()), this, SLOT(loadSettings()));
	connect(configDialog, SIGNAL(applyClicked()), this, SLOT(refreshVocabularies()));
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

void Kanagram::play(QString filename)
{
	#ifndef WITHOUT_ARTS
		KDE::PlayObject *playobj = m_artsFactory->createPlayObject(locate("appdata", "sounds/" + filename), true);
		playobj->play();
	#else
		(void)filename;
	#endif
}

#include "kanagram.moc"
