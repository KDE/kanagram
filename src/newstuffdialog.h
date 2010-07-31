#ifndef NEWSTUFFDIALOG_H
#define NEWSTUFFDIALOG_H

#include <knewstuff/knewstuff.h>

class NewStuffDialog : public KNewStuff
{
	public:
		NewStuffDialog();
		~NewStuffDialog();
		bool install(const TQString &fileName);
		bool createUploadFile(const TQString &fileName);
		TQString downloadDestination(KNS::Entry *entry);
};

#endif

