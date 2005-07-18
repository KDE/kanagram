#ifndef VOCABSETTINGS_H
#define VOCABSETTINGS_H

#include "vocabsettingswidget.h"

class VocabSettings : public VocabSettingsWidget
{
Q_OBJECT
	public:
		VocabSettings(QWidget *parent);
		~VocabSettings();
		void refreshView();

	private:
		QStringList m_fileList;
		QMap<const QListViewItem*, int> m_itemMap;

	private slots:
		void slotCreateNew();
		void slotEdit();
		void slotDelete();
		void slotSelectionChanged(QListViewItem *item);
};

#endif

