/*
Copyright (c) 1993-2008, Cognitive Technologies
All rights reserved.

Разрешается повторное распространение и использование как в виде исходного кода,
так и в двоичной форме, с изменениями или без, при соблюдении следующих условий:

      * При повторном распространении исходного кода должны оставаться указанное
        выше уведомление об авторском праве, этот список условий и последующий
        отказ от гарантий.
      * При повторном распространении двоичного кода в документации и/или в
        других материалах, поставляемых при распространении, должны сохраняться
        указанная выше информация об авторском праве, этот список условий и
        последующий отказ от гарантий.
      * Ни название Cognitive Technologies, ни имена ее сотрудников не могут
        быть использованы в качестве средства поддержки и/или продвижения
        продуктов, основанных на этом ПО, без предварительного письменного
        разрешения.

ЭТА ПРОГРАММА ПРЕДОСТАВЛЕНА ВЛАДЕЛЬЦАМИ АВТОРСКИХ ПРАВ И/ИЛИ ДРУГИМИ ЛИЦАМИ "КАК
ОНА ЕСТЬ" БЕЗ КАКОГО-ЛИБО ВИДА ГАРАНТИЙ, ВЫРАЖЕННЫХ ЯВНО ИЛИ ПОДРАЗУМЕВАЕМЫХ,
ВКЛЮЧАЯ ГАРАНТИИ КОММЕРЧЕСКОЙ ЦЕННОСТИ И ПРИГОДНОСТИ ДЛЯ КОНКРЕТНОЙ ЦЕЛИ, НО НЕ
ОГРАНИЧИВАЯСЬ ИМИ. НИ ВЛАДЕЛЕЦ АВТОРСКИХ ПРАВ И НИ ОДНО ДРУГОЕ ЛИЦО, КОТОРОЕ
МОЖЕТ ИЗМЕНЯТЬ И/ИЛИ ПОВТОРНО РАСПРОСТРАНЯТЬ ПРОГРАММУ, НИ В КОЕМ СЛУЧАЕ НЕ
НЕСЁТ ОТВЕТСТВЕННОСТИ, ВКЛЮЧАЯ ЛЮБЫЕ ОБЩИЕ, СЛУЧАЙНЫЕ, СПЕЦИАЛЬНЫЕ ИЛИ
ПОСЛЕДОВАВШИЕ УБЫТКИ, СВЯЗАННЫЕ С ИСПОЛЬЗОВАНИЕМ ИЛИ ПОНЕСЕННЫЕ ВСЛЕДСТВИЕ
НЕВОЗМОЖНОСТИ ИСПОЛЬЗОВАНИЯ ПРОГРАММЫ (ВКЛЮЧАЯ ПОТЕРИ ДАННЫХ, ИЛИ ДАННЫЕ,
СТАВШИЕ НЕГОДНЫМИ, ИЛИ УБЫТКИ И/ИЛИ ПОТЕРИ ДОХОДОВ, ПОНЕСЕННЫЕ ИЗ-ЗА ДЕЙСТВИЙ
ТРЕТЬИХ ЛИЦ И/ИЛИ ОТКАЗА ПРОГРАММЫ РАБОТАТЬ СОВМЕСТНО С ДРУГИМИ ПРОГРАММАМИ,
НО НЕ ОГРАНИЧИВАЯСЬ ЭТИМИ СЛУЧАЯМИ), НО НЕ ОГРАНИЧИВАЯСЬ ИМИ, ДАЖЕ ЕСЛИ ТАКОЙ
ВЛАДЕЛЕЦ ИЛИ ДРУГОЕ ЛИЦО БЫЛИ ИЗВЕЩЕНЫ О ВОЗМОЖНОСТИ ТАКИХ УБЫТКОВ И ПОТЕРЬ.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

    * Redistributions of source code must retain the above copyright notice,
      this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright notice,
      this list of conditions and the following disclaimer in the documentation
      and/or other materials provided with the distribution.
    * Neither the name of the Cognitive Technologies nor the names of its
      contributors may be used to endorse or promote products derived from this
      software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

//
// 08-13-93 07:58pm, Mike
// Comments:  Updating for multi-lingual version of CuneiForm ( release
//            28-June-93 ). Release has been received from Joe by FEDERAL
//            mail.
//
// The main target: To use new stream technology : IOLIB.H.
//
// Process indicator:
//  1. Preliminary updation. 08-13-93 06:40pm, Mike
//


  #include "spelmode.h"

  #if defined(TURBO_C)
    #include "tc_types.h"
  #elif defined(WATCOM)
    #include "spelwatc.h"
  #elif defined(THINK_C)
    #include "spelthin.h"
  #else
    #error   NO TOOOL SPECIFIED
  #endif

  #include <stdlib.h>
  #include <ctype.h>

//  #include "iolib.h"
  #include "speldefs.h"
  #include "spelfunc.h"
  #include "tigeremulate.h"

#define st Q.st

extern dQ  SPQ;      //Q;         /* .... */
extern int16_t searcmode;             /* .... */

/************************************************************************/
extern STD std;
extern uchar all_lt[256];

#ifdef S_COMMENTS
uchar all_lt[]=
{
  /*0    1    2    3    4    5    6    7    8    9    A    B    C    D    E    F */

    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,_SP1,_SP2,
 _SPC,_TRM,E_DL,E_DL,_DIG,E_DL,E_DL,_APF,E_DL,E_DL,E_DL,E_DL,E_DL,_WDV,_TRM,E_DL,
 _DIG,_DIG,_DIG,_DIG,_DIG,_DIG,_DIG,_DIG,_DIG,_DIG,E_DL,E_DL,E_DL,E_DL,E_DL,_TRM,
 E_DL,E_CP,E_CP,E_CP,E_CP,E_CP,E_CP,E_CP,E_CP,E_CP,E_CP,E_CP,E_CP,E_CP,E_CP,E_CP,
 E_CP,E_CP,E_CP,E_CP,E_CP,E_CP,E_CP,E_CP,E_CP,E_CP,E_CP,E_DL,E_DL,E_DL,   0,   0,
    0,E_OD,E_OD,E_OD,E_OD,E_OD,E_OD,E_OD,E_OD,E_OD,E_OD,E_OD,E_OD,E_OD,E_OD,E_OD,
 E_OD,E_OD,E_OD,E_OD,E_OD,E_OD,E_OD,E_OD,E_OD,E_OD,E_OD,E_DL,E_DL,E_DL,   0,   0,

 R_CP,R_CP,R_CP,R_CP,R_CP,R_CP,R_CP,R_CP,R_CP,R_CP,R_CP,R_CP,R_CP,R_CP,R_CP,R_CP,
 R_CP,R_CP,R_CP,R_CP,R_CP,R_CP,R_CP,R_CP,R_CP,R_CP,R_CP,R_CP,R_CP,R_CP,R_CP,R_CP,
 R_OD,R_OD,R_OD,R_OD,R_OD,R_OD,R_OD,R_OD,R_OD,R_OD,R_OD,R_OD,R_OD,R_OD,R_OD,R_OD,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
 R_OD,R_OD,R_OD,R_OD,R_OD,R_OD,R_OD,R_OD,R_OD,R_OD,R_OD,R_OD,R_OD,R_OD,R_OD,R_OD,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0
};
#endif
/* ------------------------------------------------------------------ */


/* ------------------------------------------------------------------ */
dQ *vz=&SPQ;           //Q;

/*=================================================================== */
/*                         functions' prototypes                      */
/*=================================================================== */
static int16_t check_and_look(int16_t *,LTIMG **,
                   voc_state *,int16_t ,int16_t );

int16_t check_art_dict (char word[], int16_t * wordlth, int16_t * vockind);


/****************************************************************************/
/* This procedure is the DICT-VOC interface for spelling checker.
   It is used when the part has a position with a delimiter.
    If it is the word with an alt-delimiter(s), then cut the
    word and check it piece-by-piece.
    Otherwise - check the whole word as usually

   Reply:

									    */
/****************************************************************************/
int16_t check_compound (SOBJ   *    obj,
                    LTIMG ** wrddef,
         struct dict_state *   dict,
              user_voc  voc_array[],
                   int16_t       voc_no )
{int16_t    insect, depth= 0, prob = 1024;
 int16_t    wordlth;
 int16_t    lth;
 char   save_word [sizeof(obj->wordchar)+1];
 int16_t    beg=0,   loclth=0;
 char                *out;
 SWORD   *wrd;
 wrd = obj -> word;
 lth =  wrd -> lth;
 wordlth = lth - 1;

 if ( wrd->voc_prob =
      check_art_dict (obj->wordchar, &wordlth, &(wrd->voc_kind)) )
       {
         obj -> nmb_wrdfound++;
         obj -> lthok  =  wordlth;
         return Ok ;
       }

 memcpy (save_word, obj-> wordchar, sizeof (obj->wordchar));

 out = obj -> wordchar;
 save_word[lth] = '.';
 for ( insect = 0; insect <= lth; insect++)
    switch ( symcode((char  *) (save_word + insect)))
     {
      case    E_DL:
      case    _TRM:
                   if (!loclth)    { depth++; beg++;
                                     out = obj -> wordchar; continue; }
                   *out = 0;
                   wrd -> lth  = loclth;
                   wrd -> voc_prob = 0;
                   check_dict_voc (obj, wrddef + beg, dict, voc_array, voc_no);
                   depth += obj -> lthok;
                   if ( wrd -> voc_prob < prob)
                          prob = wrd -> voc_prob;

		   if ( !prob) {
                                if ( obj -> lthok + 1 == loclth)
                                    depth++;
                                goto GetOut;
                               }
/* The following line has been substituted to cover
   Satisfaction/Ouality not founding
  Le, 02.10.93. Moscow.
*/
/*		   depth ++;                            */
		   depth += 2;

		   loclth = 0;
                   beg = insect + 1;
                   out = obj -> wordchar;
                   continue;

           default:
                   *(out++) = *(obj -> wordchar +insect);
                   loclth ++;
                   continue;
     }
 GetOut:
 memcpy (obj-> wordchar,save_word, lth);
 wrd -> voc_prob = prob;
 wrd -> lth = lth;

 if ( depth >= lth )
                    depth = lth -1;
 obj -> lthok =
    (( wrd -> lth < 6) && !( wrd -> type_sp & T_BLANK))  ?
     lth -1 :  depth;

 return ( wrd -> voc_prob) ? Ok : No;
}

/****************************************************************************/
/* This procedure is the DICT-VOC interface for spelling checker
									    */
/****************************************************************************/
int16_t check_dict_voc (SOBJ * obj,
            LTIMG ** wrddef,
	    struct dict_state * dict,
	user_voc  voc_array[], int16_t voc_no
       )
{
 int16_t wordlth;
 int16_t max_dep = 0;
 int16_t direction;
 SWORD *wrd;
 wrd = obj -> word;
 direction = CheckContext ( obj, wrddef, wrd -> lth);

 wordlth=wrd -> lth-1;

if ( direction == All_dicts)
  {

   obj -> voc_kind =2;

   if (wrd -> voc_prob = findstat(&wordlth,wrddef,dict))
       wrd -> voc_kind = 2;    /* the word found in static voc */
   if (max_dep < wordlth)
                         max_dep = wordlth;
   if ( wrd -> voc_prob)  goto Br;

   if (wrd -> type_sp & T_APF)
    {
     int16_t l, r;
	 if (test_apppostrof (obj -> wordchar, wrd, &l,&r))
	  {
	   wordlth = r-l-1;
	   if (wrd -> voc_prob = findstat(&wordlth,wrddef+l,dict))
            wrd -> voc_kind = 2;    /* the word found in static voc */
       if (max_dep < wordlth+l)
                                max_dep = wordlth+l;
       if (wrd -> voc_prob)     goto Br;
	  }
     }
   }
    else                          /* Not all dicts */
         max_dep = obj -> lthok;

if ( voc_no)
 if (!wrd -> voc_prob)
  { int16_t i, lth;
   obj ->voc_kind = 1;
    for(i =0; i<voc_no; i++)
     {
      lth = wordlth = wrd -> lth-1;
      if (wrd -> voc_prob =
       check_and_look(&wordlth,wrddef,&voc_array[i].voc,VOC_R,0))
          wrd -> voc_kind = 1;   /* the word found in dynamic voc */

      if ( wordlth > lth) wordlth = lth;
      if (max_dep < wordlth)
                          max_dep = wordlth;
       if ( wrd -> voc_prob)  goto Br;

// Le 06-02-95 06:52pm
   if (wrd -> type_sp & T_APF)
    {
     int16_t l, r;
	 if (test_apppostrof (obj -> wordchar, wrd, &l,&r))
	  {
	   wordlth = r-l-1;
     if (wrd -> voc_prob =
       check_and_look(&wordlth,wrddef +l,&voc_array[i].voc,VOC_R,0))
          wrd -> voc_kind = 1;   /* the word found in dynamic voc */

       if (max_dep < wordlth+l)
                                max_dep = wordlth+l;
       if (wrd -> voc_prob)     goto Br;
	  }
    }
 // Le 06-02-95 06:52pm

      }
   }

 wordlth= wrd->lth - 1;
/*
 if((!wordlth) ||
   ((wordlth - obj->part->blank_nmb) >= 1))
 */
 if ( (!( wrd -> type & T_LOW )) || ( wrd -> type & T_DIG ))

   if (wrd -> voc_prob =
      check_art_dict (obj->wordchar, &wordlth, &(wrd -> voc_kind)) )
      {
       if (max_dep < wordlth)
                         max_dep = wordlth;
      }

 Br:
 if (wrd -> voc_prob)
                         obj->nmb_wrdfound++;
  if(
     (obj ->part -> type & T_CHEESE ) &&
     !(wrd -> type_sp & T_BLANK)
    )
              obj->lthok= wrd -> lth-1;
  else
   obj->lthok = max_dep;

return (wrd -> voc_prob) ? Ok : No ;
}

/****************************************************************************/
/* This procedure is the VOC (dynamic) interface for spelling checker       */
/****************************************************************************/
int16_t check_and_look (int16_t *lth,LTIMG **wrddef,
                voc_state *dindict,int16_t status,int16_t incr)
{
#ifdef OVERVOC
 if(CHECKVOC)
  {
   int32_t size;
   size=read_all_file((char *)Q.temp_voc_file,V_POINT(Q.v_state.vocseg,0));
   unlink(Q.temp_voc_file);
   if ( size != -1 )
    {
     dindict->vocfree=size;
     voc_open(dindict);
    }
   else
    if ( voc_init(dindict) < 0 )
     return -3;
  }
#endif
 return(look_through_voc(lth,wrddef,dindict,status,incr,NULL));

}
/****************************************************************************/
/* This procedure sets initial state of wrdimg
   via current part of a current object
									    */
/****************************************************************************/
int16_t setimg (SOBJ * obj,
        LTIMG wrdimg[]
       )
{
 int16_t pi;                       /* curr pos index                             */
 int16_t ii;                       /* wrdimg curr pos index                      */
 int16_t ai;                       /* index of curr alt of curr pos              */
 LT  * alt;     /* pointer to curr alt stored in img-alt-list */

 for (ii=0; ii<=obj->part->lth; ii++)
  {
   pi = obj->part->posn[ii];
   memset (&(wrdimg[ii]),0,sizeof(LTIMG));/*init state of wrdimg item */
   wrdimg[ii].lt=(LT  *)&wrdimg[ii].std;  /*allways alt-list in std !  */
   wrdimg[ii].lt+=0;                            /* normalize !               */
     wrdimg[ii].source =(char *) &(obj->pos[pi].orig);
   if (obj->pos[pi].type_sp & T_BLANK)  /* blank in this position    */
    {
     memcpy(&wrdimg[ii].std,&std,sizeof(STD));
     wrdimg[ii].blank=1;        /* this flag is used for dict access !!! */
    }
   else                           /* not blank, copy all alts of the pos: */
    {
     alt = wrdimg[ii].lt;         /* here alt-list will begin             */
     for (ai=0; ai<=obj->pos[pi].alt_nmb; ai++, alt++)
      {
       alt->code = obj->pos[pi].alt[ai].lt->code;
       alt->attr = obj->pos[pi].alt[ai].lt->attr;
      }
     alt--;                       /* last alt pointer                     */
     alt->attr|= EOLIST;          /* EOlist of alts                       */
    }

  }

 wrdimg[ii].lt=NULL;
 return(OK);
}

/**************************************************************************/
/* This procedure sets initial state of wrddef
   via wrdimg.
   Results :
    wrddef;
    *lth=last wrddef index
									  */
/**************************************************************************/
int16_t setdef (LTIMG wrdimg[],
        LTIMG * wrddef[],
        int16_t * lth
       )
{
 register int16_t i;
 for (i=0; wrdimg[i].lt; i++)
  wrddef[i]=&wrdimg[i];
 wrddef[i]=NULL;
 *lth = i-1;                                        /* last wrddef index */
return(OK);
}

/*************************************************************************/
/* This procedure sets first word via wrddef->...,
   obj->word-> pointer to (SWORD) to be constructed.
   Set obj->wordchar[],
       obj->word->(SWORD),
       obj->word->(SWORD)
									 */
/*************************************************************************/
int16_t setwrd (SOBJ * obj,
            LTIMG * wrddef[]
	   )
{
 int16_t i, pi, ai;
 SWORD * wrd;
 LT  * lt;
 LT  * wlt;

 wrd = obj->word->next;         /* save ref to the next in part-word list */
 memset ( obj->word, 0, sizeof(SWORD) );
 obj->word->next = wrd;         /* restore ref to the next ...            */
 memset ( obj->wordchar, 0, MAX_WORD_SIZE );

 for (i=0; wrddef[i] ; i++)
  {
   obj->wordchar[i] = wrddef[i]->lt->code;     /* word as a char-chain   */
   pi= obj->part->posn[i];                     /* position number        */
   obj->word->pos[i] = &(obj->pos[pi]);        /* ref to pos-item of obj */
   if (obj->word->pos[i]->type_sp & T_BLANK)   /* the pos is BLANK ?     */
    {    /* BLANK => direct access to STD alt-list in obj->pos[]->alt[]: */
     ai = getstdn((char *)&(wrddef[i]->lt->code));
     /*  AL 122892 */
     /* was:
     if (obj->word->pos[i]->alt[ai].lt->code == wrddef[i]->lt->code)
      obj->word->altn[i] = ai;
        :was */
     wlt = obj->word->pos[i]->alt[ai].lt;
     if (wlt == NULL)
       goto mytail1;     /* He DOESN'T caught his tail */
     if (wlt->code == wrddef[i]->lt->code)
       obj->word->altn[i] = (uchar)ai;
     else                                       /* I've caught my tail ! */
      {
#ifdef SYSPR
#ifdef SYSPR_ERROR
    PRINTF ("\n SYSTEM ERROR (SPELLSET.SETWORD): blank alt not found !!!\n");
#endif
#endif
       return (No);
      }
    }
   else                       /* NOT BLANK pos, access via displacement: */
    {
     lt =(LT *) &(wrddef[i]->std);
     lt +=0;
     ai = wrddef[i]->lt - lt;
     /*  AL 122892 */
     /* was:
     if (obj->word->pos[i]->alt[ai].lt->code == wrddef[i]->lt->code)
      obj->word->altn[i] = ai;
        :was */
     wlt = obj->word->pos[i]->alt[ai].lt;
     if (wlt == NULL)
       goto mytail1;     /* He DOESN'T caught his tail */
     if (wlt->code == wrddef[i]->lt->code)
       obj->word->altn[i] = (uchar)ai;
     else                                       /* I've caught my tail ! */
      {
        mytail1:
#ifdef SYSPR
#ifdef SYSPR_ERROR
PRINTF ("\n SYSTEM ERROR (SPELLSET.SETWORD): alt specified not found !!!\n");
#endif
#endif
       return (No);
      }

    }

  }
 obj->word->lth=i;
 obj->wordchar[i]='\0';
  return(Ok);
}

/*************************************************************************/
/* This procedure sets next word via wrddef&obj->lthok (reply of dict/voc)
   Reply:
    Yes  - next word is set;
    Stop - no next word
									 */
/*************************************************************************/
int16_t nextwrd (SOBJ * obj,
             LTIMG * wrddef[]
	    )
{
 int16_t i;                         /* curr index of wrddef[i]              */
 int16_t l;                         /*      index of wrddef[l] for next alt */

 l=obj->lthok;
 while ((l>=0)&& ((wrddef[l]->lt->attr&EOLIST) || wrddef[l] ->blank))
    l--;
 if(l<0)
  return (Stop);
(wrddef[l]->lt)++;        /* next alt in wrddef */

 for (i=0; wrddef[i]/*->lt*/; i++)
  {
   if (i>l)     /* is the pos new in comparison with the previous word ? */
    {           /* yes, altn should be set to zero & STD if blank        */
     wrddef[i]->lt=(LT  *)&(wrddef[i]->std);/*allways alt-list in std!*/
     wrddef[i]->lt+=0;                         /* normalize !            */
     if (*(wrddef[i]->source)==BLANK)          /*blank in this position ?*/
      {                                        /* Yes:                   */
       wrddef[i]->blank = 1;   /* this flag is used for dict access !!!  */
       memcpy(&(wrddef[i]->std),&std,sizeof(STD));
      }
    }
  }

 obj->lthok = i-1;

 return (Ok);

}

/*************************************************************************/
/* This procedure finds part in part[].
   The part to be find is specified by beg & end positions.
   Result : parti - index of part found (part[parti]).
   Reply:
    Ok - part found;
    No - not found
									 */
/*************************************************************************/
int16_t findpart (SOBJ * obj,
              SPART * part,
              int16_t beg, int16_t end,
              int16_t * parti
	     )
{
 int16_t pi;
 for (pi=0; pi<obj->part_max; pi++) /* find the part in part[]            */
  {
   if ( (part[pi].beg == beg)
      &&(part[pi].end == end) )
    goto Ok_findpart;             /* part found                           */
  }
 return (No);
Ok_findpart:
 *parti = pi;
 return(OK);
}

/*************************************************************************/
/* This procedure finds part in part[].
   The part to be find is specified by end position.
   The back serach search is use, beginning with the part[maxpi].
   Result : parti - index of part found (part[parti]).
   Reply:
    Ok - part found;
    No - not found
									 */
/*************************************************************************/
int16_t findparte (
               SPART * part,
               int16_t end,
               int16_t maxpi,
               int16_t * parti
	      )
{
 int16_t pi;

 for (pi=maxpi-1; pi>=0; pi--)    /* find previous part with fixed end    */
  {
   if (part[pi].end == end)
    goto Ok_findparte;            /* part found                           */
  }
 return (No);                     /* no more part with such an end        */
Ok_findparte:
 *parti = pi;
  return(OK);
}
