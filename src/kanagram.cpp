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

#include <qcursor.h>
#include <qpainter.h>
#include <qpixmap.h>
#include <qtimer.h>

#include <kaction.h>
#include <kapplication.h>
#include <kconfig.h>
#include <khelpmenu.h>
#include <kinputdialog.h>
#include <klocale.h>
#include <kmessagebox.h>
#include <kpopupmenu.h>
#include <kstandarddirs.h>

#include "kanagram.h"
#include "fontutils.h"


Kanagram::Kanagram() : QWidget(0, 0, WStaticContents | WNoAutoErase)
{
	m_back = new QPixmap(locate("appdata", "images/kanagram.png"));
	m_hintOverlay = new QPixmap(locate("appdata", "images/hint.png"));

	m_newWordRect = QRect(477, 31, 134, 76);
	m_settingsRect = QRect(477, 122, 134, 76);
	m_helpRect = QRect(477, 212, 134, 76);
	m_quitRect = QRect(453, 352, 182, 104);
	
	setMouseTracking(true);
	setMinimumSize(650, 471);
	show();
	
	m_fillColor = QColor(40, 40, 40);
	m_fontColor = QColor(0, 0, 0);
	m_fontHighlightColor  = QColor(230, 230, 230);

	KConfig *kc = kapp->config();
	
	m_helpMenu = new KHelpMenu(this, kapp->aboutData());
}

Kanagram::~Kanagram()
{
	delete m_back;
	delete m_hintOverlay;
}

void Kanagram::paintEvent(QPaintEvent *)
{
	QPixmap buf(width(), height());
	QPainter p(&buf);
	
	p.drawPixmap(0, 0, *m_back);
	
	drawText(p, m_game.getAnagram(), QPoint(223, 243), false, 0, 0, 0, false, true, "squeaky chalk sound", QColor(230, 230, 230), 28);
	drawText(p, "New Word", QPoint(543, 62), false, 0, 0, 0, false, true, "Steve", QColor(0, 0, 0));
	drawText(p, "Settings", QPoint(543, 147), false, 0, 0, 0, false, true, "Steve", QColor(0, 0, 0));
	drawText(p, "Help", QPoint(543, 235), false, 0, 0, 0, false, true, "Steve", QColor(0, 0, 0));
	drawText(p, "Quit", QPoint(543, 391), false, 0, 0, 0, false, true, "Steve", QColor(0, 0, 0));
	drawText(p, "(reveal word)", QPoint(330, 348), false, 0, 0, 0, false, true, "squeaky chalk sound", QColor(230, 230, 230), 14);
	drawText(p, "(hint)", QPoint(230, 348), false, 0, 0, 0, false, true, "squeaky chalk sound", QColor(230, 230, 230), 14);

	//p.drawPixmap(439, 204, *m_hintOverlay);
	//drawText(p, m_game.getHint(), QPoint(537, 254), false, 0, 0, 0, false, true, "Steve", QColor(0, 0, 0), 8);

	bitBlt(this, 0, 0, &buf);
}

void Kanagram::mousePressEvent(QMouseEvent *e)
{
	
	if (m_newWordRect.contains(e->pos()))
	{
		m_game.nextAnagram();
		update();
	}

	if(m_settingsRect.contains(e->pos()))
	{
		kapp->quit();
	}

	if(m_helpRect.contains(e->pos()))
	{
		m_helpMenu->aboutApplication();
	}

	if(m_quitRect.contains(e->pos()))
	{
		kapp->quit();
	}
}

void Kanagram::drawText(QPainter &p, const QString &text, const QPoint &center, bool withMargin, int xMargin, int yMargin, QRect *rect, bool highlight, bool bold, QString font, QColor fontColor, int fontSize)
{
	QRect r;
	QFont oldFont = font;
	QFont f = font;
	f.setPointSize(fontSize);
	if (bold) f.setBold(true);
	p.setFont(f);
	
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
	else p.setPen(m_fontHighlightColor);
	p.drawText(r, Qt::AlignCenter, text);
	
	//if (rect) r = &rect;
	//p.setPen(fontColor);
	//p.drawText(r, Qt::AlignCenter, text);
}

#include "kanagram.moc"
