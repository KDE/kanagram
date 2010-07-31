/***************************************************************************
 *   Copyright (C) 2005 by Albert Astals Cid <tsdgeos@terra.es>            *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include <tqpainter.h>

#include "fontutils.h"

int fontUtils::fontSize(TQPainter &p, const TQString &s1, int w, int h)
{
	TQRect aux1;
	TQFont f = p.font();
	f.setPointSize(28);
	p.setFont(f);
	
	aux1 = p.boundingRect(TQRect(), Qt::AlignAuto, s1);
	
	return QMIN(w * 28 / aux1.width(), h * 28 / aux1.height());
}
