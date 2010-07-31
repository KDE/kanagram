//
// C++ Interface: leitnerbox
//
// Description: 
//
//
// Author: Martin Pfeiffer <martin-pfeiffer-bensheim@web.de>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef LEITNERBOX_H
#define LEITNERBOX_H

#include <tqstring.h>

/**
@author Martin Pfeiffer
*/
class LeitnerBox
{
public:
	LeitnerBox();

	~LeitnerBox();

	void setCorrectWordBox( LeitnerBox* );	//sets the correct word box
	void setWrongWordBox( LeitnerBox* );	//sets the wrong word box
	void setBoxName( const TQString& );	//sets the boxes name

	void setVocabCount( int count );
	int getVocabCount();
	
	LeitnerBox* getCorrectWordBox();	
	LeitnerBox* getWrongWordBox();
	const TQString& getBoxName() const;
	
	bool operator == ( const LeitnerBox& a ) const;

private:
	TQString m_name;

	int m_vocabCount;
	
	LeitnerBox* m_wrongWordBox;
	LeitnerBox* m_correctWordBox;
};

#endif
