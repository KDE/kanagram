#ifndef NEWSTUFF_H
#define NEWSTUFF_H

#include "newstuffwidget.h"

class NewStuff : public NewStuffWidget
{
Q_OBJECT
	public:
		NewStuff(TQWidget *parent);
		~NewStuff();

	private slots:
		void slotGetNewVocabs();
};

#endif

