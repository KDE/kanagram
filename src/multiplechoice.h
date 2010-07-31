/***************************************************************************

              manage multiple choice suggestions for queries

    -----------------------------------------------------------------------

    begin          : Mon Oct 29 18:09:29 1999

    copyright      : (C) 1999-2001 Ewald Arnold <kvoctrain@ewald-arnold.de>
                     (C) 2001 The KDE-EDU team
                     (C) 2005 Peter Hedlund <peter.hedlund@kdemail.net>

    -----------------------------------------------------------------------

 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/


#ifndef MultipleChoice_included
#define MultipleChoice_included

#include <tqstring.h>

#define MAX_MULTIPLE_CHOICE  5  // select one out of x

class MultipleChoice
{

public:

   MultipleChoice() {}

   MultipleChoice (
           const TQString &mc1,
           const TQString &mc2,
           const TQString &mc3,
           const TQString &mc4,
           const TQString &mc5
          );

   void setMC1 (const TQString &s) { muc1 = s; }
   void setMC2 (const TQString &s) { muc2 = s; }
   void setMC3 (const TQString &s) { muc3 = s; }
   void setMC4 (const TQString &s) { muc4 = s; }
   void setMC5 (const TQString &s) { muc5 = s; }

   TQString mc1 () const { return muc1; }
   TQString mc2 () const { return muc2; }
   TQString mc3 () const { return muc3; }
   TQString mc4 () const { return muc4; }
   TQString mc5 () const { return muc5; }

   TQString mc (unsigned idx) const;

   bool isEmpty() const;
   void normalize();
   void clear();
   unsigned size();

protected:

   TQString  muc1, muc2, muc3, muc4, muc5;
};


#endif // MultipleChoice_included
