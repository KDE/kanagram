#include <qstring.h>
#include "vocdata.h"

VocData::VocData() : m_word(""), m_hint("")
{
}

VocData::~VocData()
{
}

void VocData::setWord(QString word)
{
	m_word = word;
}

void VocData::setHint(QString hint)
{
	m_hint = hint;
}

QString VocData::getHint()
{
	return m_hint;
}

QString VocData::getWord()
{
	return m_word;
}
