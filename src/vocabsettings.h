#ifndef VOCABSETTINGS_H
#define VOCABSETTINGS_H

#include "vocabsettingswidget.h"

class VocabSettings : public VocabSettingsWidget
{
Q_OBJECT
	public:
		VocabSettings(QWidget *parent);
		~VocabSettings();

	private slots:
		void slotCreateNew();
};

#endif