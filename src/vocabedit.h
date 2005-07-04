#ifndef VOCABEDIT_H
#define VOCABEDIT_H

#include "vocabeditwidget.h"

typedef QMap<QString, QString> WordMap;

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
		void wordLostFocus();
		void wordTextChanged(const QString &changedText);
		void wordsSelectionChanged();

	private:
		WordMap m_wordMap;
		WordMap::Iterator it;
};

#endif