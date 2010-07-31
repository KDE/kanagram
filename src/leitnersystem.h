//
// C++ Interface: leitnersystem
//
// Description: 
//
//
// Author: Martin Pfeiffer <martin-pfeiffer-bensheim@web.de>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include <tqstring.h>
#include <tqstringlist.h>
#include <tqvaluelist.h>
#include "leitnerbox.h"

#ifndef LEITNERSYSTEM_H
#define LEITNERSYSTEM_H


/**
@author Martin Pfeiffer
*/
class LeitnerSystem
{
public:
	LeitnerSystem();
	LeitnerSystem( TQValueList<LeitnerBox>& boxes, TQString name );

	~LeitnerSystem();

	int getNumberOfBoxes();				//returns the number of boxes in the system
	TQStringList getBoxNameList();			//returns a list with the boxes names

	TQString& getSystemName();			//returns the systems name
	void setSystemName( const TQString& name );		//sets the systems name

	LeitnerBox* getBoxWithNumber( int number );	//returns box by number
	LeitnerBox* getBoxWithName( const TQString& name );//returns box by name
	int getNumber( LeitnerBox* box );
	const TQString& getBox( int i );
	
	const TQString& getNextBox( TQString& previousBox );	//returns the next box for the next question
	
	const TQString& getCorrectBox( int box );	//returns the correct word box of "int box"
	const TQString& getWrongBox( int box );		//returns the wrong word box of "int box"
	int getWrongBoxNumber( int box );
	int getCorrectBoxNumber( int box );
	void setCorrectBox( const TQString& box, const TQString& correctWordBox );
	void setWrongBox( const TQString& box, const TQString& wrongWordBox );
	bool setBoxName( int box, const TQString& name );
	bool setBoxName( LeitnerBox* box, const TQString& name );
	
	//inserts a box with number, name, correct and wrong word box
	bool insertBox( const TQString& name, int correctWordBox, int wrongWordBox );
	bool insertBox( const TQString& name );
	void deleteBox( int box );
	void deleteBox( LeitnerBox* box );

private:
	TQString m_systemName;				//the systems name

	TQValueList<LeitnerBox> m_boxes;
};

#endif
