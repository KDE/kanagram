#include "newstuffdialog.h"

#include <knewstuff/knewstuff.h>
#include <knewstuff/entry.h>
#include <klocale.h>
#include <kio/netaccess.h>
#include <kurl.h>
#include <kstandarddirs.h>

#include "kanagramsettings.h"

NewStuffDialog::NewStuffDialog() : KNewStuff("kanagram/vocabulary", "http://kde-edu.org/kanagram/" + KanagramSettings::dataLanguage() + ".xml")
{
}

NewStuffDialog::~NewStuffDialog()
{
}

bool NewStuffDialog::install(const QString &fileName)
{
  return false;
}

QString NewStuffDialog::downloadDestination(KNS::Entry *entry)
{
	QString fileName = entry->payload().path();
	fileName = fileName.section('/', -1);
	return KGlobal::dirs()->saveLocation("data", "kanagram/data/" + KanagramSettings::dataLanguage()) + fileName;
}

bool NewStuffDialog::createUploadFile(const QString &fileName)
{
  return false;
}
