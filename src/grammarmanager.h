/***************************************************************************

              manage grammer parts (articles, conjugation)

    -----------------------------------------------------------------------

    begin                : Sat Nov 27 09:50:53 MET 1999

    copyright            : (C) 1999-2001 Ewald Arnold
                           (C) 2001 The KDE-EDU team
    email                : kvoctrain@ewald-arnold.de

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


#ifndef grammarmanager_included
#define grammarmanager_included

#include <tqstring.h>

#include <vector>
using namespace std;

#define CONJ_SIMPLE_PRESENT    "PrSi" // I live at home  what you frequently do
#define CONJ_PRESENT_PROGR     "PrPr" // I am working    what you currently are doing
#define CONJ_PRESENT_PERFECT   "PrPe" // I have cleaned  tell, #that# something has happened

#define CONJ_SIMPLE_PAST       "PaSi" // the train left 2 min ago  when did it happen
#define CONJ_PAST_PROGR        "PaPr" // it was raining  what happen at a given time in the past
#define CONJ_PAST_PARTICIPLE   "PaPa" // I cleaned       tell, #that# it happened

#define CONJ_FUTURE            "FuSi"

#define CONJ_PREFIX            "--"   // definition of prefixes (I, you, ..)

#define UL_USER_TENSE  "#"   // designates number of user tense

class Article
{

public:

   Article() {}

   Article (
           const TQString &fem_def, const TQString &fem_indef,
           const TQString &mal_def, const TQString &mal_indef,
           const TQString &nat_def, const TQString &nat_indef
          );

   void setFemale  (const TQString &def, const TQString &indef);
   void setMale    (const TQString &def, const TQString &indef);
   void setNatural (const TQString &def, const TQString &indef);

   void female  (TQString &def, TQString &indef) const;
   void male    (TQString &def, TQString &indef) const;
   void natural (TQString &def, TQString &indef) const;

protected:

   TQString  fem_def, fem_indef,
            mal_def, mal_indef,
            nat_def, nat_indef;
};


class Comparison
{

public:

   Comparison() {}

   Comparison (
           const TQString &l1,
           const TQString &l2,
           const TQString &l3
          );

   void setL1 (const TQString &s) { ls1 = s; }
   void setL2 (const TQString &s) { ls2 = s; }
   void setL3 (const TQString &s) { ls3 = s; }

   TQString l1 () const { return ls1; }
   TQString l2 () const { return ls2; }
   TQString l3 () const { return ls3; }

   bool isEmpty() const;
   void clear();

protected:

   TQString  ls1, ls2, ls3;
};


class TenseRelation
{
 public:

  TenseRelation (const TQString & _short, const TQString & _long)
    : shortId (_short), longId(_long) {}

  inline TQString shortStr() const { return shortId; }
  inline TQString longStr()  const { return longId;  }

 protected:

  TQString  shortId, longId;
};


class Conjugation
{

public:

   Conjugation () {}

   int numEntries() const;

   static vector<TenseRelation> getRelation ();
   static void setTenseNames (vector<TQString> names);

   static TQString getName (const TQString &abbrev);
   static TQString getName (int index);
   static TQString getAbbrev (const TQString &name);
   static TQString getAbbrev (int index);
   static int numInternalNames();
   static int numTenses();

   TQString getType (int index);
   void setType (int index, const TQString & type);
   void cleanUp();
   bool isEmpty (int idx);

   TQString pers1Singular(const TQString &type) const;
   TQString pers2Singular(const TQString &type) const;
   bool    pers3SingularCommon(const TQString &type) const;
   TQString pers3FemaleSingular(const TQString &type) const;
   TQString pers3MaleSingular(const TQString &type) const;
   TQString pers3NaturalSingular(const TQString &type) const;

   TQString pers1Plural(const TQString &type) const;
   TQString pers2Plural(const TQString &type) const;
   bool    pers3PluralCommon(const TQString &type) const;
   TQString pers3FemalePlural(const TQString &type) const;
   TQString pers3MalePlural(const TQString &type) const;
   TQString pers3NaturalPlural(const TQString &type) const;

   void setPers1Singular(const TQString &type, const TQString &str);
   void setPers2Singular(const TQString &type, const TQString &str);
   void setPers3SingularCommon(const TQString &type, bool f);
   void setPers3FemaleSingular(const TQString &type, const TQString &str);
   void setPers3MaleSingular(const TQString &type, const TQString &str);
   void setPers3NaturalSingular(const TQString &type, const TQString &str);

   void setPers1Plural(const TQString &type, const TQString &str);
   void setPers2Plural(const TQString &type, const TQString &str);
   void setPers3PluralCommon(const TQString &type, bool f);
   void setPers3FemalePlural(const TQString &type, const TQString &str);
   void setPers3MalePlural(const TQString &type, const TQString &str);
   void setPers3NaturalPlural(const TQString &type, const TQString &str);

private:

   struct conjug_t {

      conjug_t() {
        p3common = false;
        s3common = false;
      }

      TQString type;
      bool    p3common,
              s3common;
      TQString pers1_sing,
              pers2_sing,
              pers3_m_sing,
              pers3_f_sing,
              pers3_n_sing,
              pers1_plur,
              pers2_plur,
              pers3_m_plur,
              pers3_f_plur,
              pers3_n_plur;
   };

   struct conjug_name_t {
      const char *abbrev;
      const char *name;
   };

protected:

   vector<conjug_t>       conjugations;

   static conjug_name_t   names [];
   static vector<TQString> userTenses;
};


#endif // grammarmanager_included

