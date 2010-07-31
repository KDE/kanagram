#ifndef VOCABSETTINGS_H
#define VOCABSETTINGS_H

#include "vocabsettingswidget.h"

class VocabSettings : public VocabSettingsWidget
{
Q_OBJECT
	public:
		VocabSettings(TQWidget *parent);
		~VocabSettings();
		void refreshView();

	private:
		TQStringList m_fileList;
		TQMap<const TQListViewItem*, int> m_itemMap;

	private slots:
		void slotCreateNew();
		void slotEdit();
		void slotDelete();
		void slotSelectionChanged(TQListViewItem *item);
};

#endif

