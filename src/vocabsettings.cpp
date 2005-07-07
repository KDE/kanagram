#include "vocabsettings.h"
#include "vocabedit.h"

#include <qpushbutton.h>

#include <kdebug.h>

VocabSettings::VocabSettings(QWidget *parent) : VocabSettingsWidget(parent)
{
	connect(btnCreateNew, SIGNAL(clicked()), this, SLOT(slotCreateNew()));
}

VocabSettings::~VocabSettings()
{
}

void VocabSettings::slotCreateNew()
{
	VocabEdit *vocabEdit = new VocabEdit(this);
	vocabEdit->show();
}

#include "vocabsettings.moc"

