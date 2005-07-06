#ifndef VOCABEDIT_H
#define VOCABEDIT_H

#include "vocabeditwidget.h"

#include <vector>

using namespace std;

class VocData;

class VocabEdit : public VocabEditWidget
{
Q_OBJECT
	public:
		VocabEdit(QWidget *parent);
		~VocabEdit();
	
	public slots:
		void slotSave();
		void slotNewWord();
		void slotRemoveWord();
		void slotCancel();
		//void slotItemClicked(QListBoxItem *item);
		void slotSelectionChanged();
		void slotWordTextChanged(const QString &changes);
		void slotHintTextChanged(const QString &changes);

	private:
		vector<VocData> m_vocabList;
};

#endif