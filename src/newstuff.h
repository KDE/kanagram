#ifndef NEWSTUFF_H
#define NEWSTUFF_H

#include "ui_newstuffwidget.h"

class NewStuff : public QDialog, public Ui::NewStuffWidget
{
Q_OBJECT
	public:
		NewStuff(QWidget *parent);
		~NewStuff();

	private slots:
		void slotGetNewVocabs();
};

#endif

