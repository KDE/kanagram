#ifndef VOCABSETTINGS_H
#define VOCABSETTINGS_H

#include "ui_vocabsettingswidget.h"

class VocabSettings : public QDialog, public Ui::VocabSettingsWidget
{
Q_OBJECT
	public:
		VocabSettings(QWidget *parent);
		virtual ~VocabSettings();
		void refreshView();

	private:
		QStringList m_fileList;
		QMap<const QTreeWidgetItem*, int> m_itemMap;

	private slots:
		void slotCreateNew();
		void slotEdit();
		void slotDelete();
		void slotSelectionChanged(QTreeWidgetItem *item);
};

#endif

