#ifndef VOCDATA_H
#define VOCDATA_H

#include <qstring.h>

class VocData
{
	public:
		VocData();
		~VocData();
		void setWord(QString word);
		void setHint(QString hint);
		QString getHint();
		QString getWord();

	private:
		QString m_word;
		QString m_hint;
};

#endif
