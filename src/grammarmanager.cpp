/***************************************************************************

             manage grammar parts (articles, conjugation)

    -----------------------------------------------------------------------

    begin          : Sat Nov 27 09:50:53 MET 1999

    copyright      : (C) 1999-2001 Ewald Arnold <kvoctrain@ewald-arnold.de>
                     (C) 2001 The KDE-EDU team
                     (C) 2004-2005 Peter Hedlund <peter.hedlund@kdemail.net>

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

#include "grammarmanager.h"

#include <klocale.h>

Conjugation::conjug_name_t
Conjugation::names [] =
{
  { CONJ_SIMPLE_PRESENT,    I18N_NOOP("Simple Present") },
  { CONJ_PRESENT_PROGR,     I18N_NOOP("Present Progressive") },
  { CONJ_PRESENT_PERFECT,   I18N_NOOP("Present Perfect") },

  { CONJ_SIMPLE_PAST,       I18N_NOOP("Simple Past") },
  { CONJ_PAST_PROGR,        I18N_NOOP("Past Progressive") },
  { CONJ_PAST_PARTICIPLE,   I18N_NOOP("Past Participle") },

  { CONJ_FUTURE,            I18N_NOOP("Future") }
};


vector<TQString> Conjugation::userTenses;


//================================================================

Comparison::Comparison (
           const TQString &l1,
           const TQString &l2,
           const TQString &l3
          )
{
   setL1 (l1);
   setL2 (l2);
   setL3 (l3);
}


bool Comparison::isEmpty() const
{
  return   ls1.stripWhiteSpace().isEmpty()
        && ls2.stripWhiteSpace().isEmpty()
        && ls3.stripWhiteSpace().isEmpty();
}


void Comparison::clear()
{
   ls1 = "";
   ls2 = "";
   ls3 = "";
}


//=================================================================


Article::Article
          (const TQString &fem_def, const TQString &fem_indef,
           const TQString &mal_def, const TQString &mal_indef,
           const TQString &nat_def, const TQString &nat_indef
          )
{
   setFemale  (fem_def, fem_indef);
   setMale    (mal_def, mal_indef);
   setNatural (nat_def, nat_indef);
}


void Article::setFemale
  (const TQString &def, const TQString &indef)
{
   fem_def = def;
   fem_indef = indef;
}


void Article::setMale
  (const TQString &def, const TQString &indef)
{
   mal_def = def;
   mal_indef = indef;
}


void Article::setNatural
  (const TQString &def, const TQString &indef)
{
   nat_def = def;
   nat_indef = indef;
}


void Article::female
  (TQString &def, TQString &indef) const
{
   def = fem_def;
   indef = fem_indef;
}


void Article::male
  (TQString &def, TQString &indef) const
{
   def = mal_def;
   indef = mal_indef;
}


void Article::natural
  (TQString &def, TQString &indef) const
{
   def = nat_def;
   indef = nat_indef;
}



//==============================================================


int Conjugation::numEntries() const
{
   return conjugations.size();
}


vector<TenseRelation> Conjugation::getRelation ()
{
  vector<TenseRelation> vec;

  for (int i = 0; i < numInternalNames(); i++) {
    vec.push_back(TenseRelation(names[i].abbrev,
                                i18n(names[i].name)));
  }

  for (int i = 0; i < (int) userTenses.size(); i++) {
    TQString s;
    s.setNum(i+1);
    s.insert(0, UL_USER_TENSE);
    vec.push_back(TenseRelation(s, userTenses[i]));
  }

  return vec;
}


void Conjugation::setTenseNames (vector<TQString> names)
{
  userTenses = names;
}


TQString Conjugation::getName (const TQString &abbrev)
{
   if (abbrev.length() >= 2 && abbrev[0] == TQString(UL_USER_TENSE)) {
     TQString s = abbrev;
     s.remove(0, 1);
     int i = s.toInt() - 1;

     if (i < (int) userTenses.size() )
       return userTenses[i];
     else
       return "";
   }
   else {
     for (int i = 0; i < (int) numInternalNames(); i++)
       if (names[i].abbrev == abbrev) {
         return i18n(names[i].name);
       }
   }

   return "";
}


TQString Conjugation::getName (int idx)
{
   if (idx < numInternalNames() )
     return i18n(names[idx].name);

   else if (idx < numTenses() )
     return userTenses[idx-numInternalNames()];

   else
     return "";
}


TQString Conjugation::getAbbrev (const TQString &name)
{
   for (int i = 0; i < (int) userTenses.size(); i++)
     if (userTenses[i] == name) {
       TQString s;
       s.setNum(i+1);
       s.insert(0, UL_USER_TENSE);
       return s;
     }

   for (int i = 0; i < (int) numInternalNames(); i++)
     if (names[i].name == name)
       return names[i].abbrev;

   return "";
}


TQString Conjugation::getAbbrev (int idx)
{
   if (idx < numInternalNames() )
     return names[idx].abbrev;

   else if (idx < numTenses() ) {
     TQString s;
     s.setNum(idx-numInternalNames()+1);
     s.insert(0, UL_USER_TENSE);
     return s;
   }

   else
     return "";
}


int Conjugation::numInternalNames()
{
   return sizeof(names) / sizeof(names[0]);
}


int Conjugation::numTenses()
{
   return numInternalNames()+userTenses.size();
}


TQString Conjugation::getType (int idx)
{
  if (idx >= (int) conjugations.size() )
    return "";

  return conjugations[idx].type;
}


void Conjugation::setType (int idx, const TQString & type)
{
  if (idx >= (int) conjugations.size() )
    return;

  conjugations[idx].type = type;
}


void Conjugation::cleanUp ()
{
  for (int i = (int)conjugations.size()-1; i >= 0; i--) {
    const conjug_t *ctp = &conjugations[i];
    if (   ctp->pers1_sing.stripWhiteSpace().isEmpty()
        && ctp->pers2_sing.stripWhiteSpace().isEmpty()
        && ctp->pers3_m_sing.stripWhiteSpace().isEmpty()
        && ctp->pers3_f_sing.stripWhiteSpace().isEmpty()
        && ctp->pers3_n_sing.stripWhiteSpace().isEmpty()
        && ctp->pers1_plur.stripWhiteSpace().isEmpty()
        && ctp->pers2_plur.stripWhiteSpace().isEmpty()
        && ctp->pers3_m_plur.stripWhiteSpace().isEmpty()
        && ctp->pers3_f_plur.stripWhiteSpace().isEmpty()
        && ctp->pers3_n_plur.stripWhiteSpace().isEmpty()
       )
     conjugations.erase(conjugations.begin() + i);
  }
}


bool Conjugation::isEmpty (int idx)
{
  if (idx < (int) conjugations.size()) {
    const conjug_t *ctp = &conjugations[idx];
    return ctp->pers1_sing.stripWhiteSpace().isEmpty()
        && ctp->pers2_sing.stripWhiteSpace().isEmpty()
        && ctp->pers3_m_sing.stripWhiteSpace().isEmpty()
        && ctp->pers3_f_sing.stripWhiteSpace().isEmpty()
        && ctp->pers3_n_sing.stripWhiteSpace().isEmpty()
        && ctp->pers1_plur.stripWhiteSpace().isEmpty()
        && ctp->pers2_plur.stripWhiteSpace().isEmpty()
        && ctp->pers3_m_plur.stripWhiteSpace().isEmpty()
        && ctp->pers3_f_plur.stripWhiteSpace().isEmpty()
        && ctp->pers3_n_plur.stripWhiteSpace().isEmpty();
  }
  return true;
}


#define _GET_CON_(elem, type, default) \
   for (int i = 0; i < (int) conjugations.size(); i++) \
     if (conjugations[i].type == type) \
        return conjugations[i].elem; \
   return default;


bool Conjugation::pers3SingularCommon(const TQString &type) const
{
  _GET_CON_(s3common, type, false);
}


bool Conjugation::pers3PluralCommon(const TQString &type) const
{
  _GET_CON_(p3common, type, false);
}


TQString Conjugation::pers1Singular
  (const TQString &type) const
{
  _GET_CON_(pers1_sing, type, "");
}


TQString Conjugation::pers2Singular
  (const TQString &type) const
{
  _GET_CON_(pers2_sing, type, "");
}


TQString Conjugation::pers3FemaleSingular
  (const TQString &type) const
{
  _GET_CON_(pers3_f_sing, type, "");
}


TQString Conjugation::pers3MaleSingular
  (const TQString &type) const
{
  _GET_CON_(pers3_m_sing, type, "");
}


TQString Conjugation::pers3NaturalSingular
  (const TQString &type) const
{
  _GET_CON_(pers3_n_sing, type, "");
}


TQString Conjugation::pers1Plural
  (const TQString &type) const
{
  _GET_CON_(pers1_plur, type, "");
}


TQString Conjugation::pers2Plural
  (const TQString &type) const
{
  _GET_CON_(pers2_plur, type, "");
}


TQString Conjugation::pers3FemalePlural
  (const TQString &type) const
{
  _GET_CON_(pers3_f_plur, type, "");
}


TQString Conjugation::pers3MalePlural
  (const TQString &type) const
{
  _GET_CON_(pers3_m_plur, type, "");
}


TQString Conjugation::pers3NaturalPlural
  (const TQString &type) const
{
  _GET_CON_(pers3_n_plur, type, "");
}


#undef _GET_CON_


#define _SET_CON_(elem, type, str) \
   for (int i = 0; i < (int) conjugations.size(); i++) \
     if (conjugations[i].type == type) { \
       conjugations[i].elem = str; \
       return; \
     } \
   conjug_t ct; \
   ct.type = type; \
   ct.elem = str; \
   conjugations.push_back(ct);


void Conjugation::setPers3PluralCommon(const TQString &type, bool f)
{
  _SET_CON_(p3common, type, f);
}


void Conjugation::setPers3SingularCommon(const TQString &type, bool f)
{
  _SET_CON_(s3common, type, f);
}


void Conjugation::setPers1Singular
  (const TQString &type, const TQString &str)
{
  _SET_CON_(pers1_sing, type, str);
}


void Conjugation::setPers2Singular
  (const TQString &type, const TQString &str)
{
  _SET_CON_(pers2_sing, type, str);
}


void Conjugation::setPers3FemaleSingular
  (const TQString &type, const TQString &str)
{
  _SET_CON_(pers3_f_sing, type, str);
}


void Conjugation::setPers3MaleSingular
  (const TQString &type, const TQString &str)
{
  _SET_CON_(pers3_m_sing, type, str);
}


void Conjugation::setPers3NaturalSingular
  (const TQString &type, const TQString &str)
{
  _SET_CON_(pers3_n_sing, type, str);
}


void Conjugation::setPers1Plural
  (const TQString &type, const TQString &str)
{
  _SET_CON_(pers1_plur, type, str);
}


void Conjugation::setPers2Plural
  (const TQString &type, const TQString &str)
{
  _SET_CON_(pers2_plur, type, str);
}


void Conjugation::setPers3FemalePlural
  (const TQString &type, const TQString &str)
{
  _SET_CON_(pers3_f_plur, type, str);
}


void Conjugation::setPers3MalePlural
  (const TQString &type, const TQString &str)
{
  _SET_CON_(pers3_m_plur, type, str);
}


void Conjugation::setPers3NaturalPlural
  (const TQString &type, const TQString &str)
{
  _SET_CON_(pers3_n_plur, type, str);
}

#undef _SET_CON_

