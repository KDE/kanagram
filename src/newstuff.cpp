#include "newstuff.h"

#include <tqwidget.h>
#include <tqpushbutton.h>

#include <kdebug.h>
#include <knewstuff/downloaddialog.h>
#include <knewstuff/knewstuff.h>
#include <klocale.h>

#include "kanagramsettings.h"
#include "newstuffdialog.h"

NewStuff::NewStuff(TQWidget *parent) : NewStuffWidget(parent)
{
	connect(btnGetNew, TQT_SIGNAL(clicked()), this, TQT_SLOT(slotGetNewVocabs()));
}

NewStuff::~NewStuff()
{
}

void NewStuff::slotGetNewVocabs()
{
	NewStuffDialog *nsd = new NewStuffDialog();
	nsd->download();
}

#include "newstuff.moc"
