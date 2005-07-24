/***************************************************************************
 *   Copyright (C) 2005 by Joshua Keel                                     *
 *   joshuakeel@gmail.com                                                  *
 *                                                                         *
 *   Portions of this code taken from KSimon by Albert Astals Cid.         *
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

#include <iostream>
using namespace std;

#include <qcursor.h>
#include <qpainter.h>
#include <qpixmap.h>
#include <qtimer.h>
#include <qstring.h>

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
#include <knewstuff/downloaddialog.h>
#include <knewstuff/knewstuff.h>

#include "kanagram.h"
#include "fontutils.h"
#include "kanagramsettings.h"
#include "mainsettingswidget.h"
#include "vocabsettings.h"


Kanagram::Kanagram() : QWidget(0, 0, WStaticContents | WNoAutoErase), m_overNewWord(false), m_overSettings(false), m_overHelp(false), m_overQuit(false), m_overReveal(false), m_overHint(false), m_overTry(false), m_showHint(false)
{
	m_back = new QPixmap(locate("appdata", "images/kanagram.png"));

	m_newWordRect = QRect(477, 31, 134, 76);
	m_settingsRect = QRect(477, 122, 134, 76);
	m_helpRect = QRect(477, 212, 134, 76);
	m_quitRect = QRect(453, 352, 182, 104);
	m_hintRect = QRect(51, 337, 39, 28);
	m_revealRect = QRect(279, 338, 119, 28);
	m_tryRect = QRect(341, 426, 55, 33);
	
	setMouseTracking(true);
	setFixedSize(650, 471);
	show();
	
	m_chalkColor = QColor(155, 155, 155);
	m_chalkHighlightColor = QColor(255, 255, 255);
	m_fillColor = QColor(40, 40, 40);
	m_fontColor = QColor(55, 55, 55);
	m_fontHighlightColor = QColor(99, 99, 99);
	
	m_helpMenu = new KHelpMenu(this, kapp->aboutData());
	
	m_inputBox = new QLineEdit(this);
	m_inputBox->setGeometry(QRect(52, 427, 273, 29));
	m_inputBox->setFrame(false);
	
	connect(m_inputBox, SIGNAL(returnPressed()), this, SLOT(checkWord()));
	
	QFont f = QFont();
	f.setPointSize(17);
	m_inputBox->setFont(f);
	m_inputBox->show();
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
	m_standardInterfaceFonts = KanagramSettings::standardInterfaceFonts();
	m_standardBlackboardFonts = KanagramSettings::standardBlackboardFonts();

	if(m_standardInterfaceFonts)
		m_font = KGlobalSettings::generalFont();
	else
		m_font = QFont("steve");
	if(m_standardBlackboardFonts)
		m_blackboardFont = KGlobalSettings::generalFont();
	else
		m_blackboardFont = QFont("squeaky chalk sound");
}

void Kanagram::paintEvent(QPaintEvent *)
{
	QPixmap buf(width(), height());
	QPainter p(&buf);
	
	p.drawPixmap(0, 0, *m_back);
	
	drawText(p, m_game.getAnagram(), QPoint(223, 243), false, 0, 0, 0, true, true, m_blackboardFont, m_chalkColor, m_chalkHighlightColor, 28);
	
	drawText(p, i18n("New Word"), QPoint(543, 62), false, 0, 0, 0, m_overNewWord, true, m_font, m_fontColor, m_fontHighlightColor);
	drawText(p, i18n("Settings"), QPoint(543, 147), false, 0, 0, 0, m_overSettings, true, m_font, m_fontColor, m_fontHighlightColor);
	drawText(p, i18n("Help"), QPoint(543, 235), false, 0, 0, 0, m_overHelp, true, m_font, m_fontColor, m_fontHighlightColor);
	drawText(p, i18n("Quit"), QPoint(543, 391), false, 0, 0, 0, m_overQuit, true, m_font, m_fontColor, m_fontHighlightColor);
	drawText(p, i18n("reveal word"), QPoint(336, 353), false, 0, 0, 0, m_overReveal, true, m_blackboardFont, m_chalkColor, m_chalkHighlightColor, 14);
	drawText(p, i18n("hint"), QPoint(70, 353), false, 0, 0, 0, m_overHint, true, m_blackboardFont, m_chalkColor, m_chalkHighlightColor, 14);
	drawText(p, i18n("Try"), QPoint(369, 442), true, 10, 5, &m_tryRect, m_overTry, true, m_font, QColor(126, 126, 126), m_chalkHighlightColor);

	p.setPen(QPen(black, 3));
	
	QRect borderRect = m_inputBox->geometry();
	borderRect.setLeft(borderRect.left() - 2);
	borderRect.setTop(borderRect.top() - 2);
	borderRect.setWidth(borderRect.width() + 2 * 1);
	borderRect.setHeight(borderRect.height() + 2 * 1);
	p.drawRoundRect(borderRect, 10, 5);
	
	if(m_showHint)
	{
		p.drawPixmap(439, 204, *m_hintOverlay);
		QFont f = QFont(m_font);
		f.setPointSize(12);
		p.setFont(f);
		p.drawText(446, 207, 171, 85, WordBreak | AlignCenter, m_game.getHint());
	}

	bitBlt(this, 0, 0, &buf);
}

void Kanagram::mousePressEvent(QMouseEvent *e)
{
	
	if (m_newWordRect.contains(e->pos()))
	{
		m_game.nextAnagram();
		m_inputBox->unsetPalette();
		update();
	}

	if(m_settingsRect.contains(e->pos()))
	{
		showSettings();
	}

	if(m_helpRect.contains(e->pos()))
	{
		m_helpMenu->aboutApplication();
	}

	if(m_quitRect.contains(e->pos()))
	{
		kapp->quit();
	}

	if(m_revealRect.contains(e->pos()))
	{
		m_game.restoreWord();
		update();
	}

	if(m_hintRect.contains(e->pos()))
	{
		if(m_showHint == true) m_showHint = false;
		else
		{
			if(m_hintHideTime)
			{
				QTimer::singleShot( m_hintHideTime * 1000, this, SLOT(hideHint()) );
			}
			m_showHint = true;
			randomHintImage();
		}
		update();
	}

	if(m_tryRect.contains(e->pos()))
	{
		if(m_inputBox->text() == m_game.getWord())
		{
			m_inputBox->unsetPalette();
			cout << "Correct!" << endl;
			m_inputBox->clear();
			m_game.nextAnagram();
			update();
		}
		else
		{
			m_inputBox->setPaletteBackgroundColor(QColor(255, 0, 0));
			cout << "Sorry, try again!" << endl;
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
	
	if (m_newWordRect.contains(p))
	{
		if (!m_overNewWord)
		{
			m_overNewWord = true;
			haveToUpdate = true;
		}
	}
	else if (m_overNewWord)
	{
		m_overNewWord = false;
		haveToUpdate = true;
	}

	if(m_settingsRect.contains(p))
	{
		if(!m_overSettings)
		{
			m_overSettings = true;
			haveToUpdate = true;
		}
	}
	else if(m_overSettings)
	{
		m_overSettings = false;
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

	if(m_tryRect.contains(p))
	{
		if(!m_overTry)
		{
			m_overTry = true;
			haveToUpdate = true;
		}
	}
	else if(m_overTry)
	{
		m_overTry = false;
		haveToUpdate = true;
	}

	if (haveToUpdate) update();
}

void Kanagram::drawText(QPainter &p, const QString &text, const QPoint &center, bool withMargin, int xMargin, int yMargin, QRect *rect, bool highlight, bool bold, QFont &font, QColor fontColor, QColor fontHighlightColor,int fontSize)
{
	QRect r;
	//QFont f = font;
	font.setPointSize(fontSize);
	if (bold) font.setBold(true);
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
	
	if (!highlight) p.setPen(fontColor);
	else p.setPen(fontHighlightColor);
	p.drawText(r, Qt::AlignCenter, text);
}

void Kanagram::checkWord()
{
	QPoint p = m_tryRect.topLeft() + QPoint( 1, 1 );
	QMouseEvent *e = new QMouseEvent( QEvent::MouseButtonPress, p, Qt::LeftButton, Qt::NoButton );
	mousePressEvent(e);
}

void Kanagram::randomHintImage()
{
	unsigned long imageNum = m_randomImage.getLong(8);
	if (imageNum == 0)
	{
		imageNum = 5;
	}
	QString dir = "images/eyes" + QString::number(imageNum) + ".png";
	m_hintOverlay = new QPixmap(locate("appdata", dir));
}

void Kanagram::showSettings()
{
	if(KConfigDialog::showDialog("settings"))
		return;

	m_configDialog = new KConfigDialog( this, "settings", KanagramSettings::self() );
	m_configDialog->addPage( new MainSettingsWidget( m_configDialog ), i18n( "Settings" ), "configure" );
	m_configDialog->addPage( new VocabSettings( m_configDialog ), i18n("Vocabularies"), "edit" );
	connect(m_configDialog, SIGNAL(settingsChanged()), this, SLOT(loadSettings()));
	m_configDialog->show();
}

void Kanagram::hideHint()
{
	if(m_showHint == true) m_showHint = false;
	update();
}

#include "kanagram.moc"
