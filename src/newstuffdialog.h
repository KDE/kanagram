#ifndef NEWSTUFFDIALOG_H
#define NEWSTUFFDIALOG_H

#include <knewstuff/knewstuff.h>
#include <kdelibs_export.h>
class NewStuffDialog : public KNewStuff
{
	public:
		NewStuffDialog();
		~NewStuffDialog();
		bool install(const QString &fileName);
		bool createUploadFile(const QString &fileName);
		QString downloadDestination(KNS::Entry *entry);
};

#endif

