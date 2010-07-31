//
// C++ Interface: leitnersystemview
//
// Description: 
//
//
// Author: Martin Pfeiffer <martin-pfeiffer-bensheim@web.de>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef LEITNERSYSTEMVIEW_H
#define LEITNERSYSTEMVIEW_H

#include <tqscrollview.h>
#include <tqpixmap.h>

class LeitnerSystem;


/**
@author Martin Pfeiffer
*/

class LeitnerSystemView : public QScrollView
{
	Q_OBJECT
		
public:
    LeitnerSystemView(TQWidget* parent = 0, const char* name = 0, WFlags f = 0);

    ~LeitnerSystemView();

    void setSystem(LeitnerSystem* system);	//set a new system to view on
    void highlightBox(int box);			//highlight a box

signals:
	void boxClicked(int box);		//is emited if the user clicks on a box 

protected:
    virtual void drawContents(TQPainter* p, int clipx, int clipy, int clipw, int cliph);
    virtual void mousePressEvent(TQMouseEvent* e);

private:
	LeitnerSystem* m_leitnerSystem;		//the system which is shown

	void drawSystem(TQPainter*);		//paints the boxes
	void drawConnections(TQPainter*);		//paints the arrows between the boxes
	void calculateSize();

	//TQRect m_viewArea;
	int m_imageY; 				//high border of the images
	int m_distPixmap;
	int m_highlightedBox;			//the box which is currently highlighted
};

#endif
