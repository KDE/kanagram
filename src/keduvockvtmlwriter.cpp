/***************************************************************************
                   export a KEduVocDocument to a KVTML file
    -----------------------------------------------------------------------
    copyright            : (C) 1999-2001 Ewald Arnold
                           (C) 2001 The KDE-EDU team
						               (C) 2005 Eric Pignet
    email                : eric at erixpage.com
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include <kdebug.h>
#include <tqdom.h>
#include <tqtextstream.h>

#include "keduvockvtmlwriter.h"
#include "keduvocdocument.h"

KEduVocKvtmlWriter::KEduVocKvtmlWriter(TQFile *file)
{
  // the file must be already open
  m_outputFile = file;
}

KEduVocKvtmlWriter::~KEduVocKvtmlWriter()
{
}

bool KEduVocKvtmlWriter::saveTypeNameKvtMl (TQDomDocument &domDoc, TQDomElement &domElementParent)
{
  if (m_doc->type_descr.size() == 0)
    return true;

  TQDomElement domElementType = domDoc.createElement(KV_TYPE_GRP);

  for (int lfn = 0; lfn < (int) m_doc->type_descr.size(); lfn++)
  {
    if (!(m_doc->type_descr[lfn].isNull()) )
    {
      TQDomElement domElementDesc = domDoc.createElement(KV_TYPE_DESC);
      TQDomText domTextDesc = domDoc.createTextNode(m_doc->type_descr[lfn]);

      domElementDesc.setAttribute(KV_TYPE_NO, lfn+1);
      domElementDesc.appendChild(domTextDesc);
      domElementType.appendChild(domElementDesc);
    }
  }

  domElementParent.appendChild(domElementType);
  return true;
}


bool KEduVocKvtmlWriter::saveTenseNameKvtMl (TQDomDocument &domDoc, TQDomElement &domElementParent)
{
  if (m_doc->tense_descr.size() == 0)
    return true;

  TQDomElement domElementTense = domDoc.createElement(KV_TENSE_GRP);

  for (int lfn = 0; lfn < (int) m_doc->tense_descr.size(); lfn++)
  {
    if (!(m_doc->tense_descr[lfn].isNull()) ) {
      TQDomElement domElementDesc = domDoc.createElement(KV_TENSE_DESC);
      TQDomText domTextDesc = domDoc.createTextNode(m_doc->tense_descr[lfn]);

      domElementDesc.setAttribute(KV_TENSE_NO, lfn+1);
      domElementDesc.appendChild(domTextDesc);
      domElementTense.appendChild(domElementDesc);
    }
  }

  domElementParent.appendChild(domElementTense);
  return true;
}


bool KEduVocKvtmlWriter::saveUsageNameKvtMl (TQDomDocument &domDoc, TQDomElement &domElementParent)
{
  if (m_doc->usage_descr.size() == 0)
    return true;

  TQDomElement domElementUsage = domDoc.createElement(KV_USAGE_GRP);

  for (int lfn = 0; lfn < (int) m_doc->usage_descr.size(); lfn++)
  {
    if (!(m_doc->usage_descr[lfn].isNull()) )
    {
      TQDomElement domElementDesc = domDoc.createElement(KV_USAGE_DESC);
      TQDomText domTextDesc = domDoc.createTextNode(m_doc->usage_descr[lfn]);

      domElementDesc.setAttribute(KV_USAGE_NO, lfn+1);
      domElementDesc.appendChild(domTextDesc);
      domElementUsage.appendChild(domElementDesc);
    }
  }

  domElementParent.appendChild(domElementUsage);
  return true;
}


bool KEduVocKvtmlWriter::saveLessonKvtMl (TQDomDocument &domDoc, TQDomElement &domElementParent)
{
  if (m_doc->lesson_descr.size() == 0)
    return true;

  TQDomElement domElementLesson = domDoc.createElement(KV_LESS_GRP);
  domElementLesson.setAttribute(KV_SIZEHINT, m_doc->getSizeHint(-1));

  for (int lfn = 0; lfn < (int) m_doc->lesson_descr.size(); lfn++)
  {
    if (!(m_doc->lesson_descr[lfn].isNull()) )
    {
      TQDomElement domElementDesc = domDoc.createElement(KV_LESS_DESC);
      TQDomText domTextDesc = domDoc.createTextNode(m_doc->lesson_descr[lfn]);

      domElementDesc.setAttribute(KV_LESS_NO, lfn+1);
      if (m_doc->getCurrentLesson() == lfn+1)
        domElementDesc.setAttribute (KV_LESS_CURR, 1);
      if (lfn < (int) m_doc->lessons_in_query.size() && m_doc->lessons_in_query[lfn])
        domElementDesc.setAttribute (KV_LESS_QUERY, 1);

      domElementDesc.appendChild(domTextDesc);
      domElementLesson.appendChild(domElementDesc);
    }
  }

  domElementParent.appendChild(domElementLesson);
  return true;
}


bool KEduVocKvtmlWriter::saveConjug(TQDomDocument &domDoc, TQDomElement &domElementParent,
                                    const Conjugation &curr_conjug, TQString type)
{
  if (!curr_conjug.pers1Singular(type).isEmpty() )
  {
    TQDomElement domElementP1s = domDoc.createElement(KV_CON_P1S);
    TQDomText domTextP1s = domDoc.createTextNode(curr_conjug.pers1Singular(type));

    domElementP1s.appendChild(domTextP1s);
    domElementParent.appendChild(domElementP1s);
  }

  if (!curr_conjug.pers2Singular(type).isEmpty() )
  {
    TQDomElement domElementP2s = domDoc.createElement(KV_CON_P2S);
    TQDomText domTextP2s = domDoc.createTextNode(curr_conjug.pers2Singular(type));

    domElementP2s.appendChild(domTextP2s);
    domElementParent.appendChild(domElementP2s);
  }

  if (!curr_conjug.pers3FemaleSingular(type).isEmpty() ||
    curr_conjug.pers3SingularCommon(type))
  {
    TQDomElement domElementP3sf = domDoc.createElement(KV_CON_P3SF);
    TQDomText domTextP3sf = domDoc.createTextNode(curr_conjug.pers3FemaleSingular(type));

    if (curr_conjug.pers3SingularCommon(type))
      domElementP3sf.setAttribute(KV_CONJ_COMMON, 1);

    domElementP3sf.appendChild(domTextP3sf);
    domElementParent.appendChild(domElementP3sf);
  }

  if (!curr_conjug.pers3MaleSingular(type).isEmpty() )
  {
    TQDomElement domElementP3sm = domDoc.createElement(KV_CON_P3SM);
    TQDomText domTextP3sm = domDoc.createTextNode(curr_conjug.pers3MaleSingular(type));

    domElementP3sm.appendChild(domTextP3sm);
    domElementParent.appendChild(domElementP3sm);
  }

  if (!curr_conjug.pers3NaturalSingular(type).isEmpty() )
  {
    TQDomElement domElementP3sn = domDoc.createElement(KV_CON_P3SN);
    TQDomText domTextP3sn = domDoc.createTextNode(curr_conjug.pers3NaturalSingular(type));

    domElementP3sn.appendChild(domTextP3sn);
    domElementParent.appendChild(domElementP3sn);
  }

  if (!curr_conjug.pers1Plural(type).isEmpty() )
  {
    TQDomElement domElementP1p = domDoc.createElement(KV_CON_P1P);
    TQDomText domTextP1p = domDoc.createTextNode(curr_conjug.pers1Plural(type));

    domElementP1p.appendChild(domTextP1p);
    domElementParent.appendChild(domElementP1p);
  }

  if (!curr_conjug.pers2Plural(type).isEmpty() )
  {
    TQDomElement domElementP2p = domDoc.createElement(KV_CON_P2P);
    TQDomText domTextP2p = domDoc.createTextNode(curr_conjug.pers2Plural(type));

    domElementP2p.appendChild(domTextP2p);
    domElementParent.appendChild(domElementP2p);
  }

  if (!curr_conjug.pers3FemalePlural(type).isEmpty() ||
    curr_conjug.pers3PluralCommon(type))
  {
    TQDomElement domElementP3pf = domDoc.createElement(KV_CON_P3PF);
    TQDomText domTextP3pf = domDoc.createTextNode(curr_conjug.pers3FemalePlural(type));

    if (curr_conjug.pers3PluralCommon(type))
      domElementP3pf.setAttribute(KV_CONJ_COMMON, 1);

    domElementP3pf.appendChild(domTextP3pf);
    domElementParent.appendChild(domElementP3pf);
  }

  if (!curr_conjug.pers3MalePlural(type).isEmpty() )
  {
    TQDomElement domElementP3pm = domDoc.createElement(KV_CON_P3PM);
    TQDomText domTextP3pm = domDoc.createTextNode(curr_conjug.pers3MalePlural(type));

    domElementP3pm.appendChild(domTextP3pm);
    domElementParent.appendChild(domElementP3pm);
  }

  if (!curr_conjug.pers3NaturalPlural(type).isEmpty() )
  {
    TQDomElement domElementP3pn = domDoc.createElement(KV_CON_P3PN);
    TQDomText domTextP3pn = domDoc.createTextNode(curr_conjug.pers3NaturalPlural(type));

    domElementP3pn.appendChild(domTextP3pn);
    domElementParent.appendChild(domElementP3pn);
  }

  return true;
}

bool KEduVocKvtmlWriter::saveConjugHeader(TQDomDocument &domDoc, TQDomElement &domElementParent,
                                          vector<Conjugation> &curr_conjug)
{
/*
 <conjugation>    used in header for definiton of "prefix"
  <e l="de">      lang determines also lang order in entries !!
   <s1>I</s1>     which must NOT differ
   <s2>you<2>
   <s3f common="0">he</s3f>
   <s3m>she</s3m>
   <s3n>it</s3n>
   <p1>we</p1>
   <p2>you</p2>
   <p3f common="1">they</p3f>
   <p3m>they</p3m>
   <p3n>they</p3n>
  </e>
 </conjugation>

*/
  if (curr_conjug.size() == 0)
    return true;

  TQDomElement domElementConjug = domDoc.createElement(KV_CONJUG_GRP);
  TQString s;

  for (int ent = 0; ent < QMIN((int) curr_conjug.size(), m_doc->numLangs()); ent++)
  {
    TQDomElement domElementEntry = domDoc.createElement(KV_CON_ENTRY);

    if (ent == 0)
    {
      s = m_doc->getOriginalIdent().stripWhiteSpace();  //EPT le Ident doit �re superflu
      if (s.isEmpty() )
        s = "original";
    }
    else
    {
      s = m_doc->getIdent(ent).stripWhiteSpace();
      if (s.isEmpty() )
      {
        s.setNum(ent);
        s.insert(0, "translation ");
      }
    }
    domElementEntry.setAttribute(KV_LANG, s);

    if (!saveConjug (domDoc, domElementEntry, curr_conjug[ent], CONJ_PREFIX))
      return false;

    domElementConjug.appendChild(domElementEntry);
  }

  domElementParent.appendChild(domElementConjug);
  return true;
}


bool KEduVocKvtmlWriter::saveComparison(TQDomDocument &domDoc, TQDomElement &domElementParent,
                                        const Comparison &comp)
/*
 <comparison>
   <l1>good</l1>
   <l2>better</l2>
   <l3>best</l3>
 </comparison>
*/
{
  if (comp.isEmpty())
    return true;

  TQDomElement domElementComparison = domDoc.createElement(KV_COMPARISON_GRP);

  if (!comp.l1().isEmpty() )
  {
    TQDomElement domElementL1 = domDoc.createElement(KV_COMP_L1);
    TQDomText domTextL1 = domDoc.createTextNode(comp.l1());

    domElementL1.appendChild(domTextL1);
    domElementComparison.appendChild(domElementL1);
  }

  if (!comp.l2().isEmpty() )
  {
    TQDomElement domElementL2 = domDoc.createElement(KV_COMP_L2);
    TQDomText domTextL2 = domDoc.createTextNode(comp.l2());

    domElementL2.appendChild(domTextL2);
    domElementComparison.appendChild(domElementL2);
  }

  if (!comp.l3().isEmpty() )
  {
    TQDomElement domElementL3 = domDoc.createElement(KV_COMP_L3);
    TQDomText domTextL3 = domDoc.createTextNode(comp.l3());

    domElementL3.appendChild(domTextL3);
    domElementComparison.appendChild(domElementL3);
  }

  domElementParent.appendChild(domElementComparison);
  return true;
}


bool KEduVocKvtmlWriter::saveMultipleChoice(TQDomDocument &domDoc, TQDomElement &domElementParent,
                                            const MultipleChoice &mc)
/*
 <multiplechoice>
   <mc1>good</mc1>
   <mc2>better</mc2>
   <mc3>best</mc3>
   <mc4>best 2</mc4>
   <mc5>best 3</mc5>
 </multiplechoice>
*/
{
  if (mc.isEmpty())
    return true;

  TQDomElement domElementMC = domDoc.createElement(KV_MULTIPLECHOICE_GRP);

  if (!mc.mc1().isEmpty() )
  {
    TQDomElement domElementMC1 = domDoc.createElement(KV_MC_1);
    TQDomText domTextMC1 = domDoc.createTextNode(mc.mc1());

    domElementMC1.appendChild(domTextMC1);
    domElementMC.appendChild(domElementMC1);
  }

  if (!mc.mc2().isEmpty() )
  {
    TQDomElement domElementMC2 = domDoc.createElement(KV_MC_2);
    TQDomText domTextMC2 = domDoc.createTextNode(mc.mc2());

    domElementMC2.appendChild(domTextMC2);
    domElementMC.appendChild(domElementMC2);
  }

  if (!mc.mc3().isEmpty() )
  {
    TQDomElement domElementMC3 = domDoc.createElement(KV_MC_3);
    TQDomText domTextMC3 = domDoc.createTextNode(mc.mc3());

    domElementMC3.appendChild(domTextMC3);
    domElementMC.appendChild(domElementMC3);
  }

  if (!mc.mc4().isEmpty() )
  {
    TQDomElement domElementMC4 = domDoc.createElement(KV_MC_4);
    TQDomText domTextMC4 = domDoc.createTextNode(mc.mc4());

    domElementMC4.appendChild(domTextMC4);
    domElementMC.appendChild(domElementMC4);
  }

  if (!mc.mc5().isEmpty() )
  {
    TQDomElement domElementMC5 = domDoc.createElement(KV_MC_5);
    TQDomText domTextMC5 = domDoc.createTextNode(mc.mc5());

    domElementMC5.appendChild(domTextMC5);
    domElementMC.appendChild(domElementMC5);
  }

  domElementParent.appendChild(domElementMC);
  return true;
}


bool KEduVocKvtmlWriter::saveConjugEntry( TQDomDocument &domDoc, TQDomElement &domElementParent,
                                          Conjugation &curr_conjug)
/*
 <conjugation>    in entry for definition of tenses of (irreg.) verbs
  <t n="sipa">
   <s1>go</s1>
   <s2>go</s2>
   <s3f>goes</s3f>
   <s3m>goes</s3m>
   <s3n>goes</s3n>
   <p1>go</p1>
   <p2>go</p2>
   <p3f>go</p3f>
   <p3m>go</p3m>
   <p3n>go</p3n>
  </t>
 </conjugation>
*/
{
  curr_conjug.cleanUp();
  if (curr_conjug.numEntries() == 0 )
    return true;

  TQDomElement domElementConjug = domDoc.createElement(KV_CONJUG_GRP);
  TQString type;

  for (int lfn = 0; lfn < (int) curr_conjug.numEntries(); lfn++)
  {
    TQDomElement domElementType = domDoc.createElement(KV_CON_TYPE);

    type = curr_conjug.getType(lfn);
    domElementType.setAttribute(KV_CON_NAME, type);
    
    if (!saveConjug (domDoc, domElementType, curr_conjug, curr_conjug.getType(lfn)) )
      return false;

    domElementConjug.appendChild(domElementType);
  }

  domElementParent.appendChild(domElementConjug);
  return true;
}


bool KEduVocKvtmlWriter::saveArticleKvtMl(TQDomDocument &domDoc, TQDomElement &domElementParent)
/*
 <article>
  <e l="de">    lang determines also lang order in entries !!
   <fi>eine</fi>  which must NOT differ
   <fd>die</fd>
   <mi>ein</mi>
   <md>der</md>
   <ni>ein</ni>
   <nd>das</nd>
  </e>
 </article>
*/
{
  if (m_doc->articles.size() == 0)
    return true;

  TQDomElement domElementArticle = domDoc.createElement(KV_ARTICLE_GRP);
  TQString def, indef, s;

  for (int lfn = 0; lfn < QMIN((int) m_doc->articles.size(), m_doc->numLangs()); lfn++)
  {
    TQDomElement domElementEntry = domDoc.createElement(KV_ART_ENTRY);
    if (lfn == 0)
    {
      s = m_doc->getOriginalIdent().stripWhiteSpace();
      if (s.isEmpty() )
        s = "original";
    }
    else
    {
      s = m_doc->getIdent(lfn).stripWhiteSpace();
      if (s.isEmpty() )
      {
        s.setNum(lfn);
        s.insert(0, "translation ");
      }
    }
    domElementEntry.setAttribute(KV_LANG, s);

    m_doc->articles[lfn].female(def, indef);
    if (!def.isEmpty() )
    {
      TQDomElement domElementFD = domDoc.createElement(KV_ART_FD);
      TQDomText domTextFD = domDoc.createTextNode(def);

      domElementFD.appendChild(domTextFD);
      domElementEntry.appendChild(domElementFD);
    }
    if (!indef.isEmpty() )
    {
      TQDomElement domElementFI = domDoc.createElement(KV_ART_FI);
      TQDomText domTextFI = domDoc.createTextNode(indef);

      domElementFI.appendChild(domTextFI);
      domElementEntry.appendChild(domElementFI);
    }

    m_doc->articles[lfn].male(def, indef);
    if (!def.isEmpty() )
    {
      TQDomElement domElementMD = domDoc.createElement(KV_ART_MD);
      TQDomText domTextMD = domDoc.createTextNode(def);

      domElementMD.appendChild(domTextMD);
      domElementEntry.appendChild(domElementMD);
    }
    if (!indef.isEmpty() )
    {
      TQDomElement domElementMI = domDoc.createElement(KV_ART_MI);
      TQDomText domTextMI = domDoc.createTextNode(indef);

      domElementMI.appendChild(domTextMI);
      domElementEntry.appendChild(domElementMI);
    }

    m_doc->articles[lfn].natural(def, indef);
    if (!def.isEmpty() )
    {
      TQDomElement domElementND = domDoc.createElement(KV_ART_ND);
      TQDomText domTextND = domDoc.createTextNode(def);

      domElementND.appendChild(domTextND);
      domElementEntry.appendChild(domElementND);
    }
    if (!indef.isEmpty() )
    {
      TQDomElement domElementNI = domDoc.createElement(KV_ART_NI);
      TQDomText domTextNI = domDoc.createTextNode(indef);

      domElementNI.appendChild(domTextNI);
      domElementEntry.appendChild(domElementNI);
    }

    domElementArticle.appendChild(domElementEntry);
  }

  domElementParent.appendChild(domElementArticle);
  return true;
}


bool KEduVocKvtmlWriter::saveOptionsKvtMl(TQDomDocument &domDoc, TQDomElement &domElementParent)
{
  TQDomElement domElementOption = domDoc.createElement(KV_OPTION_GRP);
  TQDomElement domElementSort = domDoc.createElement(KV_OPT_SORT);

  domElementSort.setAttribute(KV_BOOL_FLAG, (m_doc->sort_allowed?1:0));
  domElementOption.appendChild(domElementSort);

  domElementParent.appendChild(domElementOption);
  return true;
}


bool KEduVocKvtmlWriter::writeDoc(KEduVocDocument *doc, const TQString &generator)
{
  bool first_expr = true;

  m_doc = doc;

  TQDomDocument domDoc( "KEduVocDocument" );
  TQDomElement domElementKvtml = domDoc.createElement( "kvtml" );

  TQString head( "<?xml version='1.0' encoding='UTF-8' ?><!DOCTYPE kvtml SYSTEM \"kvoctrain.dtd\">" );
  domDoc.setContent( head );

  TQDomComment domComment = domDoc.createComment(TQString(
         "\nThis is a machine generated file.\n"
         "Be careful when editing here.\n"
         "\n"
         "Short definition:\n"
         "\n"
         "lesson     lesson group\n"
         " desc    name\n"
         "   %no     its index\n"
         "   %query  is in query selection\n"
         "   %current  is current lesson\n"
         "type     type group\n"
         " desc    name\n"
         "   %no     its index\n"
         "e      entry of dictionary\n"
         "  %s     is selected\n"
         "  %m     lesson member\n"
         "  %t     common expression type\n"
         " o       original\n"
         "   %q    in query (\"o\" is given, \"t\" is wanted)\n"
         "   %l    language code\n"
         "   %r    remark\n"
         "   %p    pronunciation\n"
         "   %width  column width\n"
         "   %t    expression type (see QueryManager.h)\n"
         "   %tf     false friend from\n"
         "   %ff     false friend to\n"
         "   %a    antonym\n"
         "   %y    synonym\n"
         "   %x    example\n"
         "   %u    usage label\n"
         "   %h    paraphrase\n"
         " t       translation ..\n"
         "   %q    in query (\"t\" is given, \"o\" is wanted)\n"
         "   %l    language code\n"
         "   %r    remark\n"
         "   %p    pronunciation\n"
         "   %width  column width\n"
         "   %t    expression type\n"
         "   %tf     false friend from\n"
         "   %ff     false friend to\n"
         "   %a    antonym\n"
         "   %y    synonym\n"
         "   %x    example\n"
         "   %u    usage label\n"
         "   %h    paraphrase\n"
         "\n"
         "   %d    last query date (from;to)\n"
         "   %w    dito, compressed and deprecated\n"
         "   %g    grade (from;to)\n"
         "   %c    count (from;to)\n"
         "   %b    bad count (from;to)\n"
         "\n"
         "\nValid xml means:\n"
         " - Close all tags\n"
         " - Keep proper hierarchy\n"
         " - All attributes are quoted\n"));

  domDoc.appendChild(domComment);

  domElementKvtml.setAttribute(KV_ENCODING, (TQString)"UTF-8");

  domElementKvtml.setAttribute(KV_GENERATOR, generator);
  domElementKvtml.setAttribute(KV_COLS, m_doc->numLangs() );
  domElementKvtml.setAttribute(KV_LINES, m_doc->numEntries() );
  
  if (!m_doc->doctitle.isEmpty())
    domElementKvtml.setAttribute(KV_TITLE, m_doc->doctitle);

  if (!m_doc->author.isEmpty())
    domElementKvtml.setAttribute(KV_AUTHOR, m_doc->getAuthor() );

  if (!m_doc->license.isEmpty())
    domElementKvtml.setAttribute(KV_LICENSE, m_doc->getLicense() );

  if (!m_doc->doc_remark.isEmpty())
    domElementKvtml.setAttribute(KV_DOC_REM, m_doc->getDocRemark() );

  if (!saveLessonKvtMl(domDoc, domElementKvtml))
    return false;

  if (!saveArticleKvtMl(domDoc, domElementKvtml))
    return false;

  if (!saveConjugHeader(domDoc, domElementKvtml, m_doc->conjugations))
    return false;

  if (!saveOptionsKvtMl(domDoc, domElementKvtml))
    return false;

  if (!saveTypeNameKvtMl(domDoc, domElementKvtml))
    return false;

  if (!saveTenseNameKvtMl(domDoc, domElementKvtml))
    return false;

  if (!saveUsageNameKvtMl(domDoc, domElementKvtml))
    return false;

  TQString q_org, q_trans;
  vector<KEduVocExpression>::const_iterator first =  m_doc->vocabulary.begin ();
  m_doc->getQueryLang(q_org, q_trans);

  int ent_no = 0;
  int ent_percent = (int) m_doc->vocabulary.size () / 100;
  float f_ent_percent = (int) m_doc->vocabulary.size () / 100.0;
//TODO emit progressChanged(this, 0);

  while (first != m_doc->vocabulary.end ())
  {
    TQDomElement domElementExpression = domDoc.createElement(KV_EXPR);

    ent_no++;
    if (ent_percent != 0 && (ent_no % ent_percent) == 0 )
//TODO emit progressChanged(this, ent_no / (int) f_ent_percent);

    if ((*first).getLesson() != 0)
    {
      // entry belongs to lesson x
      TQString ls;
      int lm = (*first).getLesson();
      if (lm > (int) m_doc->lesson_descr.size() )
      {
        // should not be
        kdError() << "index of lesson member too high: " << lm << endl;
        lm = 0;
      }
      ls.setNum (lm);
      domElementExpression.setAttribute (KV_LESS_MEMBER, ls);
    }

    if ((*first).isInQuery())
    {
      // entry was selected for query
      domElementExpression.setAttribute (KV_SELECTED, (TQString) "1");
    }

    if (!(*first).isActive())
    {
      // entry was inactive
      domElementExpression.setAttribute (KV_INACTIVE, (TQString) "1");
    }

    if ((*first).uniqueType() && !(*first).getType(0).isEmpty())
    {
      domElementExpression.setAttribute (KV_EXPRTYPE, (*first).getType(0));
    }

    TQDomElement domElementOriginal = domDoc.createElement(KV_ORG);
    if (first_expr)
    {
      // save space, only tell language in first entry
      TQString s;
      s.setNum (m_doc->getSizeHint (0));
      domElementOriginal.setAttribute(KV_SIZEHINT, s);

      s = m_doc->getOriginalIdent().stripWhiteSpace();
      if (s.isEmpty() )
        s = "original";
      domElementOriginal.setAttribute (KV_LANG, s);
      if (s == q_org)
        domElementOriginal.setAttribute(KV_QUERY, (TQString) KV_O);
      else if (s == q_trans)
        domElementOriginal.setAttribute(KV_QUERY, (TQString) KV_T);

    }

    if (!(*first).getRemark(0).isEmpty() )
      domElementOriginal.setAttribute(KV_REMARK, (*first).getRemark(0));

    if (!(*first).getSynonym(0).isEmpty() )
      domElementOriginal.setAttribute(KV_SYNONYM, (*first).getSynonym(0));

    if (!(*first).getExample(0).isEmpty() )
      domElementOriginal.setAttribute(KV_EXAMPLE, (*first).getExample(0));

    if (!(*first).getUsageLabel(0).isEmpty() )
      domElementOriginal.setAttribute(KV_USAGE, (*first).getUsageLabel(0));

    if (!(*first).getParaphrase(0).isEmpty() )
      domElementOriginal.setAttribute(KV_PARAPHRASE, (*first).getParaphrase(0));

    if (!(*first).getAntonym(0).isEmpty() )
      domElementOriginal.setAttribute(KV_ANTONYM, (*first).getAntonym(0));

    if (!(*first).getPronunce(0).isEmpty() )
      domElementOriginal.setAttribute(KV_PRONUNCE, (*first).getPronunce(0));

    if (!(*first).uniqueType() && !(*first).getType(0).isEmpty())
      domElementOriginal.setAttribute(KV_EXPRTYPE, (*first).getType(0));

    if (!saveMultipleChoice(domDoc, domElementOriginal, (*first).getMultipleChoice(0)))
      return false;

    TQString s;
    TQString entype = s = (*first).getType(0);
    int pos = s.find (QM_TYPE_DIV);
    if (pos >= 0)
      entype = s.left (pos);
    else
      entype = s;

    if (entype == QM_VERB
        && (*first).getConjugation(0).numEntries() > 0)
    {
      Conjugation conj = (*first).getConjugation(0);
      if (!saveConjugEntry(domDoc, domElementOriginal, conj))
        return false;
    }
    else if (entype == QM_ADJ
             && !(*first).getComparison(0).isEmpty())
    {
      Comparison comp = (*first).getComparison(0);
      if (!saveComparison(domDoc, domElementOriginal, comp))
        return false;
    }

    TQDomText domTextOriginal = domDoc.createTextNode((*first).getOriginal());
    domElementOriginal.appendChild(domTextOriginal);
    domElementExpression.appendChild(domElementOriginal);

    int trans = 1;
    while (trans < (int)m_doc->langs.size())
    {
      TQDomElement domElementTranslation = domDoc.createElement(KV_TRANS);
      if (first_expr)
      {
        // save space, only tell language in first entry
        TQString s;
        s.setNum (m_doc->getSizeHint (trans));
        domElementTranslation.setAttribute(KV_SIZEHINT, s);

        s = m_doc->getIdent(trans).stripWhiteSpace();
        if (s.isEmpty() )
        {
          s.setNum (trans);
          s.insert (0, "translation ");
        }
        domElementTranslation.setAttribute(KV_LANG, s);
        if (s == q_org)
          domElementTranslation.setAttribute(KV_QUERY, (TQString) KV_O);
        else if (s == q_trans)
          domElementTranslation.setAttribute(KV_QUERY, (TQString) KV_T);
      }

      TQString s1, s2;

      if ((*first).getGrade(trans, false) != 0
        ||(*first).getGrade(trans, true) != 0)
      {
        domElementTranslation.setAttribute(KV_GRADE, (*first).gradeStr(trans, false)
                  +';'
                  +(*first).gradeStr(trans, true));
      }

      if ((*first).getQueryCount(trans, false) != 0
        ||(*first).getQueryCount(trans, true) != 0)
      {
        s1.setNum((*first).getQueryCount(trans, false));
        s2.setNum((*first).getQueryCount(trans, true));
        domElementTranslation.setAttribute(KV_COUNT, s1 +';' +s2);
      }

      if ((*first).getBadCount(trans, false) != 0
        ||(*first).getBadCount(trans, true) != 0)
      {
        s1.setNum((*first).getBadCount(trans, false));
        s2.setNum((*first).getBadCount(trans, true));
        domElementTranslation.setAttribute(KV_BAD, s1 +';' +s2);
      }

      if ((*first).getQueryDate(trans, false) != 0
        ||(*first).getQueryDate(trans, true) != 0)
      {
        s1.setNum((*first).getQueryDate(trans, false));
        s2.setNum((*first).getQueryDate(trans, true));
        domElementTranslation.setAttribute(KV_DATE, s1 +';' +s2);
      }

      if (!(*first).getRemark(trans).isEmpty() )
        domElementTranslation.setAttribute(KV_REMARK, (*first).getRemark(trans));

      if (!(*first).getFauxAmi(trans, false).isEmpty() )
        domElementTranslation.setAttribute(KV_FAUX_AMI_F, (*first).getFauxAmi(trans, false));

      if (!(*first).getFauxAmi(trans, true).isEmpty() )
        domElementTranslation.setAttribute(KV_FAUX_AMI_T, (*first).getFauxAmi(trans, true));

      if (!(*first).getSynonym(trans).isEmpty() )
        domElementTranslation.setAttribute(KV_SYNONYM, (*first).getSynonym(trans));

      if (!(*first).getExample(trans).isEmpty() )
        domElementTranslation.setAttribute(KV_EXAMPLE, (*first).getExample(trans));

      if (!(*first).getUsageLabel(trans).isEmpty() )
        domElementTranslation.setAttribute(KV_USAGE, (*first).getUsageLabel(trans));

      if (!(*first).getParaphrase(trans).isEmpty() )
        domElementTranslation.setAttribute(KV_PARAPHRASE, (*first).getParaphrase(trans));

      if (!(*first).getAntonym(trans).isEmpty() )
        domElementTranslation.setAttribute(KV_ANTONYM, (*first).getAntonym(trans));

      if (!(*first).getPronunce(trans).isEmpty() )
        domElementTranslation.setAttribute(KV_PRONUNCE, (*first).getPronunce(trans));

      if (!(*first).uniqueType() && !(*first).getType(trans).isEmpty())
        domElementTranslation.setAttribute(KV_EXPRTYPE, (*first).getType(trans));

      // only save conjugations when type == verb

      if (!saveMultipleChoice(domDoc, domElementTranslation, (*first).getMultipleChoice(trans)))
        return false;

      TQString s;
      TQString entype = s = (*first).getType(0);
      int pos = s.find (QM_TYPE_DIV);
      if (pos >= 0)
        entype = s.left (pos);
      else
        entype = s;

      if (entype == QM_VERB
          && (*first).getConjugation(trans).numEntries() > 0)
      {
        Conjugation conj = (*first).getConjugation(trans);
        if (!saveConjugEntry(domDoc, domElementTranslation, conj))
          return false;
      }

      if (entype == QM_ADJ
          && !(*first).getComparison(trans).isEmpty())
      {
        Comparison comp = (*first).getComparison(trans);
        if (!saveComparison(domDoc, domElementTranslation, comp))
          return false;
      }

      TQDomText domTextTranslation = domDoc.createTextNode((*first).getTranslation(trans));
      domElementTranslation.appendChild(domTextTranslation);
      domElementExpression.appendChild(domElementTranslation);

      trans++;
    }

    domElementKvtml.appendChild(domElementExpression);

    first++;
    first_expr = false;
  }

  domDoc.appendChild(domElementKvtml);

  TQTextStream ts( m_outputFile );
  ts.setEncoding( TQTextStream::UnicodeUTF8 );
  ts << domDoc.toString();

// TODO setModified (false);
  return true;

}

