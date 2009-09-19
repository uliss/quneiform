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



/*
   spellset.c - object-parts-words construct & setting initial state
*/

  #include "spelmode.h"

  #if defined(TURBO_C)
    #include "tc_types.h"
  #elif defined(WATCOM)
    #include "spelwatc.h"
  #else
    #error   NO TOOOL SPECIFIED
  #endif

  #include <stdlib.h>
  #include <ctype.h>

  #include "speldefs.h"
  #include "spelfunc.h"
  #include "lang.h"
  #include "tuner.h"

  #include "tigeremulate.h"

#ifdef  RUS_ENG_LANG
extern uchar multy_language ;
#endif


extern FILE *dbg_f;
extern dQ SPQ;              //SPQ;                        /* ED-file access common structure */

static char trm_flag=0;  /* turns to 1 if .?! are occured    */

/*NEW*/
extern STD std;
extern int16_t ABCSize;
/*END*/
/*=================================================================== */
/*                   local  functions prototypes                      */
/*=================================================================== */
  static int16_t  analsymb  (SOBJ *, int16_t, int16_t *);
  static int16_t  analspsp  (SOBJ *, int16_t, int16_t *);
#ifdef S_COMMENTS
  static int16_t  analdash  (SOBJ *, int16_t, int16_t *);
#endif
  static int16_t  analother (SOBJ *,int16_t,int16_t);
  static int16_t  checkexline (SOBJ *, int16_t);
  static int16_t  checkline (void);
  static int16_t  truncobj   (SOBJ *, int16_t *);
  static int16_t  nextinobj (SOBJ *, int16_t npos, int16_t nalt);
  static int16_t  trytruncobj (SOBJ *, int16_t *);
  static void setpos (SOBJ *, int16_t, int16_t *, LT  *);
  static void setalt (SOBJ *, int16_t, int16_t *, LT  *);
  static void genpos (SOBJ *, int16_t, int16_t);

/* ************************************************************************ */
/* This procedure reads object from ED-file to the obj (objstr) with
   all alternatives stored so no changes of ED-file could be done.
									    */
///////////////////////////////////////////////////////////////////////////////
int16_t readobj (int16_t status,SOBJ * obj)   /* Two statuses may occur             */
				      /* S_NEWOBJ -when SPQ.ns_symb points on */
				      /* the rest of previous object, and   */
				      /* S_OLDOBJ -when SPQ.ns_symb points on */
				      /* the beginning of object to be read */

{
	int16_t ret;                      /* working return-flag         */
	int16_t nobj;                     /* curr obj nmb */
	int16_t npos=0, nalt=0;           /* curr nmb of position & alternative */

	if (!obj->nmb)	                /* if it is the first obj:            */
		if (!checkline ())   /* is there smth in curr line ?       */
		{
			if(!checkexline(obj,0))
				goto Readno;                 /* No - end of ED-file (empty)        */
		}

/* ----------------- start object treatment ------------------------- */
Begobj:
	nobj = obj->nmb;                               /* old nmb of obj     */
	memset (obj,0,sizeof(SOBJ));          /* obj initial state  */
	npos = 0;
	nalt = 0;
	init_specpos (obj);
	obj->nmb = nobj;                               /* restore            */
	obj->tif_ref.symb=SPQ.ns_symb;    /* curr SPQ.ns_symb */
	obj->tif_ref.segm=SPQ.ns_segm;    /* curr SPQ.ns_segm */
	/* ---------------- */
	if (nobj)                      /* not the first obj considered :     */
	{
#ifdef S_COMMENTS
		if (symcode(SPQ.ns_symb)==_TRM)         /* last time stopped with _TRM ? */
		{                                    /* yes, consequently:         */
			obj->pos[npos].type |= T_STMT;      /* position - beg of sentence */
		}
#endif
		if(trm_flag)
		{
			obj->pos[npos].type |= T_STMT;      /* position - beg of sentence */
		}


		obj->pos_part_nmb=0;

		if(status==S_NEWOBJ)
			if (!next_symb(YES,NO,YES,SPQ.ns_segm,SPQ.ns_symb))  /* step to next symb cannot do it:     */
				if (!checkexline (obj, npos))  /* is there smth in next line ?       */
					goto Readno;                  /* No - end of ED-file                */
	}
	//#ifdef  RUS_ENG_LANG
	//if( !multy_language )
	//#endif
	/* ---------------- step to next symb (1-st obj-symb) has been done --- */
	//Allex   18.04.99
	/*
	if(SPQ.stack->language!=language)
	{
        extern trees_load_rling(void);
        extern Bool FlagMixedText;

        FlagMixedText=TRUE;
        language=SPQ.stack->language;

        trees_load_rling();
        init_stat_dict(&SPQ.d_state);

    }
	*/
	nalt=0;
	/* ----------------- loop through object positions & alts: -------------- */
	do
	{
		if (npos == MAX_VIEW_SIZE)
		/* view field is being overflowed ? */
		{                                   /* YES:                             */
			if (truncobj (obj, &npos))        /* truncate obj if possible         */
				goto Readok;                    /* will be treated                  */
			else
			{
				setpart_blue(obj,0,npos);
				goto Begobj;                    /* skip Bad obj (too long)          */
			}
		}
		/* view field allowed, consider alts for current position: */
		switch (symcode(SPQ.ns_symb))
		{
		case  R_OD:
		case  E_OD:
		case  R_CP:
		case  E_CP:
		case  _DIG:
		case  _BLK:
			if(!nalt)
				trm_flag=0;
Analsymb:
			if (analsymb (obj, npos, &nalt))
				continue;
			else
				trm_flag=0;
			break;
		case _APF:
			if(!nalt)
				trm_flag=0;
			if (!npos)
				continue;
			else
				goto Analsymb;
		case _SPC:
			if (!npos)
				continue;
			if (obj->pos[npos-1].orig.code==WRDDIV)
				continue;
			else
				goto Readok;
#ifdef S_COMMENTS
		case _WDV:
			if (!npos)
				continue;
			analdash (obj, npos, &nalt);
			break;
#endif
		case _SP1:
		case _SP2:
			if(!nalt)
				trm_flag=0;
			if (!npos)
				continue;
			analspsp (obj, npos, &nalt);
			break;
		default:
			ret=analother (obj, npos,nalt);

			if (ret==Ok)
				goto Analsymb;
			else
				goto Readok;
		}
		/* --------------------- end of switch --------------------------- */

		genpos (obj, npos, nalt);           /* generate pos info via its alts  */
		npos++;
		nalt=0;
	}
	while (nextinobj(obj,(int16_t)(npos - 1),(int16_t)(nalt)));     /* next ED-byte, may be in next line*/
	/* ----------------------- end of do-loop ------------------------------- */
Readok:
	if (!trytruncobj(obj, &npos))     /* try to trunc spec symb in obj tail    */
	{                  /* everything has gone, skip empty object, begin next: */
		if (nobj)                       /* not 1-st obj => regular read       */
			goto Begobj;
		if (!next_symb(YES,NO,YES,SPQ.ns_segm,SPQ.ns_symb))  /* step to next symb  cannot do it:     */
			if (!checkexline (obj, npos))  /* is there smth in next line ?       */
				goto Readno;                  /* No - end of ED-file                */
		goto Begobj;
	}
	obj->pos_part_nmb++;              /* EOobj is the last part end position:  */
	obj->pos_part[obj->pos_part_nmb]=npos; /* last part end position           */
	obj->nmb=nobj+1;                  /* nmb of obj read + 1                   */
	/*
	AddAcsentGrps(obj);
	*/
	return (Ok);                      /* Ok: object exists & may be considered */
Readno:
	return (No);                      /* no more objects exist, end of ED-file */
}

/* ------------------------------------------------------------------ */

/*
 AddAcsentGrps (SOBJ *obj)
 { int PosNo;
   int i,j,k;
   STD s;
   PosNo = obj->pos_part[obj->pos_part_nmb];
   for ( i =0; i< PosNo; i++)
	{ memset (&s, -1, sizeof(s));
	  for ( j =0; j < obj -> pos[i].alt_nmb; j++)
	   { if (!codepermit[obj -> pos[i].alt[j].orig] ) continue;
	   	 if (!AcsInd[obj -> pos[i].alt[j].orig]) continue;
	   	 s.stdlt [codetable[obj -> pos[i].alt[j].orig]].attr
		                        =AcsInd[obj -> pos[i].alt[j].orig];
	   }
	  for ( j =0; j < ABCSize; j++)
	  {
		if ( s.stdlt[j].attr==-1) continue;
		for ( k=s.stdlt[j].attr; AcsGrps[k]>0x20; k++)
		   if (s.stdlt[AcsGrps[k]].attr == -1)
								  {
								   obj->pos[i]
								  }
			else
			   s.stdlt[AcsGrps[k]].attr =-1;

	  }
	}
 }
*/
/***********************************************************************/
/* This procedure analyses current alphabetic or digital ED-symbol.
   Alphabetic/digital symbols, may be together with EOLIST-flag,
   as well as blank amd apostrophe are being analysed.
   Other unknown symbols are also allowed.
   Returns:
    Ok - the alternative isn't last in the alt-list
    No - otherwise (end of pos)
								       */
/***********************************************************************/
int16_t analsymb (SOBJ * obj, int16_t npos, int16_t * nalt)

{
 LT  *lt;
 lt=(LT *)SPQ.ns_symb;

 switch (symcode(SPQ.ns_symb))
  {
   case  R_OD:                  /* Russian ordinary letter               */
   case  E_OD:                  /* English ordinary letter               */
   /* ------------------------------------------------------------------ */
    obj->pos[npos].alt[*nalt].type |= T_ALPHA | T_LOW;
    break;

   case  R_CP:                  /* Russian capital letter                */
   case  E_CP:                  /* English capital letter                */
   /* ------------------------------------------------------------------ */
    obj->pos[npos].alt[*nalt].type |= T_ALPHA | T_CAP;
    break;

   case _DIG:                   /* Digital ASCII-symbol                  */
   /* ------------------------------------------------------------------ */
    obj->pos[npos].alt[*nalt].type |= T_DIG;
    break;

   case _BLK:                   /* unrecognized symbol                   */
   /* ------------------------------------------------------------------ */
    obj->pos[npos].           type_sp |= T_BLANK;
    break;

   case _APF:                   /* apostrophe                            */
   /* ------------------------------------------------------------------ */
    obj->pos[npos].alt[*nalt].type |= T_ALPHA ;
    obj->pos[npos].type_sp |= T_APF;

	break;

   default:                     /* other symbols                         */
    /* 08-30-93 05:15pm, Mike                */
    /* Unknown symbol is an alphabet letter. */
    obj->pos[npos].alt[*nalt].type |= T_ALPHA ;
   /* ------------------------------------------------------------------ */
   /* obj->pos[npos].alt[*nalt].type |= T_BAD; */
   /* ------------------------------------------------------------------ */

  }

 /* store alt info : --------------------------------------------------- */
 setalt (obj, npos, nalt, lt);
 /* set initial pos info : --------------------------------------------- */
 setpos (obj, npos, nalt, lt);
 /* -------------------------------------------------------------------- */

 (*nalt)++;                     /* (nmb of alts for curr pos) + 1        */
 if( *nalt>=ABCSIZE )
    return No; // OLEG
 if (lt->attr&EOLIST)           /* EO alt-list ?                         */
  return (No);                  /* Yes, end of pos, process pos          */
 else
  return (Ok);                  /* No, proceed alt-list treatment        */

}

/***********************************************************************/
/* This procedure analyses suspected spaces.
   Next part of obj is being set. No alternatives supposed.
   Always should go to next position
								       */
/***********************************************************************/
int16_t analspsp (SOBJ * obj, int16_t npos, int16_t * nalt)

{
 LT  *lt;
 lt=(LT *)SPQ.ns_symb;

 switch (symcode(SPQ.ns_symb))
  {
   case  _SP1:                  /* 1-st kind                             */
    obj->pos[npos].           type_sp |= T_SP1;
/*  obj->pos[npos].alt[*nalt].type_sp |= T_SP1; */
    break;
   case  _SP2:                  /* 2-nd kind                             */
    obj->pos[npos].           type_sp |= T_SP2;
/*  obj->pos[npos].alt[*nalt].type_sp |= T_SP2; */
    break;
  }

 /* store alt info : --------------------------------------------------- */
 setalt (obj, npos, nalt, lt);
 /* set initial pos info : --------------------------------------------- */
 setpos (obj, npos, nalt, lt);
 /* -------------------------------------------------------------------- */
 (*nalt)++;                     /* (nmb of alts for curr pos) + 1        */

 obj->pos_part_nmb++;              /* one part more                      */
 obj->pos_part[obj->pos_part_nmb]=npos;      /* store part pos           */
 return(OK);
}

#ifdef S_COMMENTS
/***********************************************************************/
/* This procedure analyses dash.
   Here it considered to be an ordinary symbol
   with the only difference that no alternatives supposed.
   Always should go to next position.
   Only in nextinobj it can determined to be hyphen
   (if next line follows and there is smth in the next line)
								       */
/***********************************************************************/
int16_t analdash (SOBJ * obj, int16_t npos, int16_t * nalt)

{
  LT  *lt;
  lt=(LT *)SPQ.ns_symb;

  /* store alt info : -------------------------------------------------- */
  setalt (obj, npos, nalt, lt);
  /* set initial pos info: --------------------------------------------- */
  setpos (obj, npos, nalt, lt);
  /* ------------------------------------------------------------------- */
  (*nalt)++;                     /* (nmb of alts for curr pos) + 1       */

  return (OK);
}
#endif

/***********************************************************************\
*  This procedure analyses current unrecognized ED-byte whether it is
   allowed within a word or has to be considered as a delimiter
   Returns:
    Ok -   symbol allowed within a word
    No -   symbol not allowed within a word and it is  a delimiter
\***********************************************************************/

int16_t analother (SOBJ * obj, int16_t npos,int16_t nalt)

{
  uchar attr;

  switch ( symcode(SPQ.ns_symb) ) {

    case  _TRM: trm_flag=1;
    case  E_DL: obj->pos[npos].type |= T_DELIM;
                obj->pos[npos].alt[nalt].type|=T_DELIM;
                attr=((LT  *)SPQ.ns_symb)-> attr;

                if ( (*SPQ.ns_symb != WRDDIV) && (!nalt) &&
                    (attr & EOLIST) && ( attr > PROB_ALLOWED) &&
                    (npos - obj->pos_part[obj->pos_part_nmb] > 4 )
                   )
                {
                  return No;
                }
                else {
                  return Ok;
                }

    default:    /* 08-30-93 05:13pm, Mike */
                /* return (No);           */
                return Ok;
  }
}

/***********************************************************************\
*  This procedure checks whether next ED-file line exists and
   if exists, is there something in the next ED-file line
\***********************************************************************/
int16_t checkexline (SOBJ * obj, int16_t npos)
{
  do {
    SPQ.ns_segm=next_line(SHEET,SPQ.ns_segm); /* is there next line ?   */
    if ( !SPQ.ns_segm )                     /* No - end of ED-file    */
      return No;
  } while ( !checkline() );       /* is there smth in next line ?   */

  /* next line exists & not empty: */
  obj->pos_line = npos;              /* position of new line beg       */
  obj->pos[npos].type_sp |= T_LINE;  /* this pos is in new line        */

  return Ok;
}

/***********************************************************************/
/* This procedure checks is there something in current ED-file line.
   Used to analyse 1-st symbol of the line
								       */
/***********************************************************************/
int16_t checkline (void)

{
 skip_letter_in_line(SPQ.ns_segm,0);  /* try to find something in the line */
 if (!SPQ.ns_symb)                    /* found ?                        */
  return(No);                       /* No: end of ED, stop everything */
/*??? moved to checkexline
 obj->pos_line = npos;
 obj->pos[npos].type_sp |= T_LINE;
???*/
 return(Ok);                        /* Ok */
}

/***********************************************************************/
/* This procedure truncates obj if there is hyphen or suspected space
								       */
/***********************************************************************/
int16_t truncobj (SOBJ * obj, int16_t * npos)

{
 if (!(obj->pos_part_nmb))         /* is any object partitioning ?        */
  { register i;                   /* No 		                  */
   for(i=MAX_VIEW_SIZE-1;i;i--)
    if(symcode((char *)&(obj->pos[i].orig))==E_DL)
      { *npos=i;
       goto Restore;
      };
 /*  obj->type |= T_BAD;          too long obj                        */
   return (No);                    /* should be skipped                   */
  }
 else                              /* partitioning => obj can be truncated*/
  {
   *npos=obj->pos_part[obj->pos_part_nmb];   /*last part beg will be EOobj*/
   obj->pos_part_nmb--;                      /* there will be 1 part less */
Restore:
   SPQ.ns_segm = obj->pos[*npos].tif_ref.segm; /* restore ED point          */
   SPQ.ns_symb = obj->pos[*npos].tif_ref.symb; /*    -- " --                */
   if(symcode(SPQ.ns_symb)==E_DL)
      while(!(((LT  *)SPQ.ns_symb)->attr&EOLIST))
	 next_symb(YES,NO,YES,SPQ.ns_segm,SPQ.ns_symb);
   return (Ok);                              /* truncation Ok             */
  }

}

/***********************************************************************/
/* This procedure seaks next ED-byte in the same line,
   npos = index of the last position read to obj->pos[npos].
   If found - Ok,
   If not found - checks previous object position:
    If it was WRDDIV (suspected hyphen), try to find smth in next line
    If it was not WRDDIV or no bytes found in next line,
     then End Of object
								       */
/***********************************************************************/
int16_t nextinobj (SOBJ * obj, int16_t npos, int16_t nalt)

{
	char  * symb;                   /* variables to save ED-file last point */
	struct segm  * segm;

	symb=SPQ.ns_symb;
	segm=SPQ.ns_segm;

	if (next_symb(YES,NO,YES,SPQ.ns_segm,SPQ.ns_symb))  /* next byte in ED-line */
	{                                              /* is found             */

		// 11-23-93 06:16pm, Mike : DASH problem ...
		//#ifdef S_COMMENTS
		if(!nalt)     /* Lepik */
		{	/* if the last symb is dash => it is not hyphen: */
			if( (obj->pos[npos].orig.code==WRDDIV) &&
			    (!(obj->pos[npos].type_sp & T_HYPHEN)) )
				goto No_nextinobj;                  /* yes, it is obj boundary      */
#ifdef S_COMMENTS
			if (!(obj->pos[npos].alt_nmb))        /* if the only alt in the pos   */
			{
				if (obj->pos[npos].type & T_DELIM) /* and it is delimiter        */
					goto No_nextinobj;              /* then truncate on last pos read  */
			}
#endif
		}
		//#else
		//   nalt++;                /* Zatichka for WATCOM */
		//#endif
		// 11-23-93 06:16pm, Mike end
		goto Ok_nextinobj;                    /* obj continues - Ok           */
	}
	else                                        /* not found:               */
		if (npos > 0)                                  /* is it not 1-st obj-pos ? */
			if(obj->pos[npos].orig.code==WRDDIV)      /* the last position ='-' ? */
			{                               /* yes: it is suspected to be hyphen */
				if (!checkexline (obj, (int16_t)(npos+1)))/* is there smth in next line ?      */
					goto No_nextinobj;            /* No - end of ED-file               */
				else		            /* smth in new line after '-' => hyphen */
				{
					/*???
					if (obj->type_sp & T_HYPHEN)
					goto No_nextinobj;
					???*/
					/* else */                      /* No  - 1-st hyphen :                */
					{
						obj->pos[npos].type_sp |= T_HYPHEN; /* hyphen flag for npos        */
						/*???
						obj->type_sp |= T_HYPHEN;
						obj->pos_hyphen=npos;
						???*/
						/* hyphen is part delimiter :         */
						obj->pos_part_nmb++;         /* one part more                      */
						obj->pos_part[obj->pos_part_nmb]=npos; /* store part pos           */
						goto Ok_nextinobj;           /* object continues                   */
					}
				}
			}

No_nextinobj:                       /* EOobj, restore ED-file last point: */
 SPQ.ns_symb=symb;
 SPQ.ns_segm=segm;
 return (No);                       /* EOobj                              */
Ok_nextinobj:
 return (Ok);                       /* object continues                   */

}

/***********************************************************************/
/* This procedure truncates special symbols in obj tail,
   npos = last pos chosen + 1
								       */
/***********************************************************************/
int16_t trytruncobj (SOBJ * obj, int16_t * npos)

{
 int16_t i=0;
 char  * symb;                /* variables to save ED-file last point */
 struct segm  * segm;

 symb=SPQ.ns_symb;
 segm=SPQ.ns_segm;

 if (!(*npos))                    /* I'm trying to catch my tail */
  goto No_trytrunc;               /* Obj is already empty        */
/* ????
  {
#ifdef SYSPR
#ifdef SYSPR_ERROR
   PRINTF ("\n SYSTEM ERROR (SPELLSET.TRYTRUNCOBJ): npos<0 !!!\n");
#endif
#endif
   return (No);
  }
???? */

 for (i=(*npos)-1; i>=0; i--)
  {
   switch ( symcode ( (char  *)(&(obj->pos[i].orig.code)) )  )
    {
     /* ------------ ordinary symbols' analysis : ---------------- */
     case  R_OD:                  /* Russian ordinary letter               */
     case  E_OD:                  /* English ordinary letter               */
     case  R_CP:                  /* Russian capital letter                */
     case  E_CP:                  /* English capital letter                */
     case _DIG:                   /* Digital ASCII-symbol                  */
      goto Ok_trytrunc;           /* nothing truncated, Ok                 */
     case _BLK:                   /* unrecognized symbol                   */
      goto Ok_trytrunc;           /* nothing truncated, Ok                 */

     /* ----------- here what is to be truncated : ---------------- */
     case _APF:          /* ??? now is not considered, always truncated:  */
      break;
     case _WDV:          /* dash cannot be tail, but is it hyphen ?       */
      if (!(obj->pos_part[obj->pos_part_nmb]==i))
       break;            /* not hyphen => trunc without pos_part_nmb change */
     case _SP1:          /* suspected space, kind 1      */
     case _SP2:          /* suspected space, kind 2      */
      if (obj->pos_part[obj->pos_part_nmb]==i)
       obj->pos_part_nmb--;  /* one part less                              */
      break;

     /* --------------- other symbols analysis : --------------- */
     default:            /* other symbols are truncated :                  */
      break;
    }
   segm = obj->pos[i].tif_ref.segm;       /* store ED point          */
   symb = obj->pos[i].tif_ref.symb;       /*  -- " --                */

  }

No_trytrunc:
 return (No);                       /* empty object  */

Ok_trytrunc:
 *npos=i+1;                            /* last pos after good       */
 SPQ.ns_segm = segm;                     /* restore ED point          */
 SPQ.ns_symb = symb;                     /*    -- " --                */
 return (Ok);                     /* smth remained */

}

/***********************************************************************/
/* This procedure sets up initial state of an object position
								       */
/***********************************************************************/
void setpos (SOBJ * obj, int16_t npos, int16_t * nalt, LT  * lt)

{
 if (*nalt)                     /* nmb of alts > 1 :                    */
  {
   obj->pos[npos].type_art |= T_ALTS;
   obj->pos[npos].alt_nmb = *nalt;
  }
 else                           /* the 1-st alt in alt-list:            */
  {                             /* store pos info:                      */
   obj->pos[npos].lt = lt;      /* ref to alt in tiff (ltr&attr)        */
   obj->pos[npos].orig.code = lt->code;
   obj->pos[npos].orig.attr = lt->attr & ~EOLIST;

   obj->pos[npos].tif_ref.row=SPQ.stack->map_ref.row;
   obj->pos[npos].tif_ref.col=SPQ.stack->map_ref.col;
   obj->pos[npos].tif_ref.width=SPQ.stack->map_ref.width;
   obj->pos[npos].tif_ref.height=SPQ.stack->map_ref.height;
/* ?????
   obj->pos[npos].tif_ref=SPQ.stack->map_ref;
*/
   obj->pos[npos].tif_ref.segm=SPQ.ns_segm;
   obj->pos[npos].tif_ref.symb=SPQ.ns_symb;
  }

}

/***********************************************************************/
/* This procedure sets up initial state of a one alternative of position
								       */
/***********************************************************************/

void setalt (SOBJ * obj, int16_t npos, int16_t * nalt, LT  * lt)

{
 int16_t i;

 obj->pos[npos].alt[*nalt].lt = lt;        /* ref to alt in tiff (ltr&attr)  */
 obj->pos[npos].alt[*nalt].orig.code =
	      lt->code;                    /* original code                  */
 obj->pos[npos].alt[*nalt].orig.attr =
	      lt->attr & ~EOLIST;          /* original probability           */

			       /* count sequential nmb in original alt-list: */
 i=*nalt;
 /* | Lepik */
 obj->pos[npos].alt[*nalt].dif_wt =      /* it is used as the wt of discrep*/
    obj->pos[npos].alt[0].orig.attr - obj->pos[npos].alt[*nalt].orig.attr;

 if (i)                                    /* skip back all alts with almost */
  for (; i>0; i--)                         /* the same probability: we take  */
  {                                        /* into account only different :  */
   if ( (obj->pos[npos].alt[i-1].orig.attr - obj->pos[npos].alt[i].orig.attr)
        > MIN_DIFF_PROB )
    break;
  }

 obj->pos[npos].alt[*nalt].seqnum = (uchar)i;     /* sequential nmb in orig alt-list*/
 /*
 obj->pos[npos].alt[*nalt].dif_wt = i;
 */

 /*?  actually seqnum may be accumulated in sumnum (new !),
      and dif_wt could be used for the sum of attr (prob) difference
      with the better items                                             */
}

/***********************************************************************/
/* This procedure generates initial state of an object position,
   using its alts info
								       */
/***********************************************************************/
void genpos (SOBJ * obj, int16_t npos, int16_t nalt)

{
 int16_t i;
 LT  * lt;

 obj->pos[npos].type &= T_STMT;    /* only this type-flag is persistent */
 for (i=0; i<nalt; i++)
  {
   obj->pos[npos].type |= obj->pos[npos].alt[i].type; /* accumulate type */
   obj->pos[npos].dif_wt += obj->pos[npos].alt[i].dif_wt; /* accumulate wt */
  }
 if (obj->pos[npos].type_sp & T_BLANK)     /* if blank                     */
  {
   for (i=0, lt=(LT  *)&(std.stdlt[0]);
	i<ABCSize;
	i++,lt++)                          /* set all possible alts A-Z:   */
    {
     obj->pos[npos].alt[i].lt = lt;        /* all ltrs in std              */
    }
  }

 if (! (obj->pos[npos].type_sp &
       (T_BLANK|T_SP1|T_SP2|T_HYPHEN) ) )      /* no fascination         */
  {
    if ( (obj->pos[npos].type & T_ALPHA)
      &&(obj->pos[npos].type & T_DIG)   )      /* both kinds available   */
     obj->pos[npos].type_sp |= T_MIXED;           /* i.e. mixed type        */
  }

}

/***********************************************************************/
/* This procedure generates initial state of obj info, using its pos info,
   npos = nmb of obj pos chosen + 1
								       */
/***********************************************************************/
void genobj (SOBJ * obj)

{
 int16_t i, npos;

 npos = obj->pos_part[obj->pos_part_nmb]; /* last part end position         */
 obj->type = 0;
 obj->type_sp = 0;
 obj->type_art = 0;
 obj->pos_line   = 0;
 obj->pos_hyphen = 0;
 obj->blank_nmb  = 0;
 obj->pos_myblank= 0;
 obj->dif_wt     = 0;
 obj->alt_nmb    = 0;

 memcpy ( &(obj->tif_ref), &(obj->pos[0].tif_ref), sizeof(struct tifref) );
 for (i=0; i<npos; i++)
  {
   obj->type    |= obj->pos[i].type;    /* accumulate type                */
   obj->type_sp |= obj->pos[i].type_sp; /* accumulate type_sp             */
   if (obj->pos[i].type_sp & T_BLANK)
    obj->blank_nmb++;
   if (obj->pos[i].type_sp & T_HYPHEN)
    obj->pos_hyphen = i;
   if (obj->pos[i].type_sp & T_LINE)
    obj->pos_line = i;
   obj->dif_wt += obj->pos[i].dif_wt;
   obj->alt_nmb+= obj->pos[i].alt_nmb;
  }

 if ( (obj->type_sp &
      (T_BLANK|T_SP1|T_SP2|T_HYPHEN) ) )       /* fascination            */
/*  obj->type &= ~T_BAD */;                    /* is not bad             */
 else
  {
   if (! (obj->type &           /* no fascination and no ordinary symb   */
       (T_ALPHA|T_DIG|T_CAP|T_LOW) ) )
  /*  obj->type |= T_BAD */;       /* then: obj cannot be estimated: all BADs */
   else                                        /* if ordinary symbs      */
    if ( (obj->type & T_ALPHA)
      &&(obj->type & T_DIG)   )                /* both kinds available   */
     obj->type_sp |= T_MIXED;                     /* i.e. mixed type        */
  }
 if (
      (obj->pos[0].type & T_CAP)               /* CAP symb in 1-st pos    */
    /* lEPIK'S COMMENTS ... */
    /*
     &&
      (!(obj->pos[0].type & T_LOW))
     &&
      (!(obj->type & T_DIG))
   */
     &&
      (!(obj->pos[0].type & T_STMT))           /* and not beg of sentence */

    )
     obj->type |= T_NAME;                /* then it considered to be NAME */

}

/***********************************************************************/
/* This procedure sets up initial state of object-part.
   Part specified by:
    obj->part_beg - beg position of a part;
    obj->part_end - end position of a part.
   Note: part may include spsp-positions (i.e. SP1, SP2, HYPHEN).
   Reply:
    Ok - part constructed;
    No - otherwise (buff overflowed or object itself is BAD)
								       */
/***********************************************************************/
int16_t setpart (SOBJ * obj, SPART part[])

{
 int16_t i, j, k;

 if (obj->type_art & (T_REPL|T_GC|T_BRK)) /* always reset when part was changed */
  {
   obj->part_max--;               /* new part index                         */
   goto Setpart_new;
  }
 if (obj->part_max == 0)          /* 1-st part is being constructed ?       */
  goto Setpart_new;               /* yes                                    */
 if (findpart(obj, part, obj->part_beg, obj->part_end, &i))
  goto Setpart_old;               /* part found                             */

Setpart_new:
 if ( !(obj->part_max < MAX_PARTS) )
  return (No);           /* part not found in part-buff & buff overflowed   */
 i = obj->part_max;      /* new part index                                  */
 obj->part_max++;        /* one part more in part[]                         */
 memset (&(part[i]),0,sizeof(SPART)); /* part initial state        */
 obj->part = &(part[i]);       /* obj ref to curr part                      */
 part[i].beg = obj->part_beg;  /* part beg                                  */
 part[i].end = obj->part_end;  /* and end                                   */
 part[i].begi = obj->part_begi;/* part beg index in obj->pos_part[]         */
 part[i].endi = obj->part_endi;/* and  end index in obj->pos_part[]         */
			 /* construct all part-positions                    */
 for (k=0, j=part[i].beg; j<part[i].end; j++)
  {
   if (!(obj->pos[j].type_sp & (T_SP1|T_SP2|T_HYPHEN))) /* spsp-position ?  */
    {
     part[i].posn[k]=(uchar)j;                                 /* No, set part pos */
     k++;
    }
   else
    part[i].type_sp |= obj->pos[j].type_sp;             /* or set type_sp   */
  }
 part[i].lth=k-1;            /* length of part                              */
 if (genpart (obj))          /* successful generation ?                     */
Setpart_old:
  {
   obj->part = &(part[i]);/* ref to part under treatment                    */
   obj->part_nmb= i;      /* and the part index                             */
   return (Ok);           /* Yes */
  }
 obj->part_max--;       /* No, something wrong with the part: BAD ltrs,...  */
 return (No);            /* KNMOHO-TO-XEPOBATO */

}

/***********************************************************************/
/* This procedure generates initial state of part info, using its pos info
   Reply:
    Ok - part is ready for treatment;
    No - part is not worthy of treatment (all positions are BAD)
								       */
/***********************************************************************/
int16_t genpart (SOBJ * obj)

{
 int16_t i, ipos;
 int16_t f_type =0;
 obj->part->type     = 0;
 obj->part->type_sp  &= (T_SP1|T_SP2|T_HYPHEN); /* save what setpart sets */
 obj->part->type_art = 0;
 obj->part->blank_nmb  = 0;
 obj->part->dif_wt     = 0;
 obj->part->alt_nmb    = 0;

 for (ipos=0; ipos<=obj->part->lth; ipos++)
  {
   i = obj->part->posn[ipos];      /* position number                   */
   obj->part->type    |= obj->pos[i].type;    /* accumulate type        */
   obj->part->type_sp |= obj->pos[i].type_sp; /* accumulate type_sp     */
   obj->part->type_art|= obj->pos[i].type_art;/* accumulate type_art    */
   f_type |=  obj->pos[i].alt[0].type;
/*?it cannot take place: */
/*   if (obj->pos[i].type_sp & (T_SP1|T_SP2|T_HYPHEN)) *//* spsp-position ?  */
/*    continue;            */                      /* yes, skip it           */
   if (obj->pos[i].type_sp & T_BLANK)
    obj->part->blank_nmb++;
   obj->part->dif_wt += obj->pos[i].dif_wt;
   obj->part->alt_nmb+= obj->pos[i].alt_nmb;
  }

 if ( obj->part->type_sp & T_BLANK )          /* fascination            */
  /* obj->part->type &= ~T_BAD */;                  /* is not bad             */
/* ??? else ??? blank does not matter */
  {
   if (! (obj->part->type &    /* no fascination and no ordinary symb   */
	(T_ALPHA|T_DIG|T_CAP|T_LOW) ) )
    {
   /*  obj->part->type |= T_BAD;  then: part cannot be estimated: all BADs */
#ifdef PARTPR
#ifdef PARTPR_REJECT
     prtpart (obj,0,0);
     PRINTF ("!*!*!*!*!*!*! PART REJECTED !*!*!*!*!*!*!");
#endif
#endif
     return (No);              /* part is not worthy of treatment         */
    }
/* else */                                       /* if ordinary symbs     */
    if ( (obj->part->type & T_ALPHA)
       &&(obj->part->type & T_DIG)  )           /* both kinds available   */
     obj->part->type_sp |= T_MIXED;                /* i.e. mixed type        */

    i = obj->part->posn[0];                     /* 1-st position number   */
    if (
        (obj->pos[i].alt[0].type & T_CAP)             /* CAP symb in 1-st pos    */
       &&
        (!(obj->pos[i].type & T_STMT))
       &&
        ( ( f_type & T_LOW) || ( obj -> part -> lth <=3 ))
       )
     obj->part->type |= T_NAME;          /* then it considered to be NAME */

  }


 return (Ok);                    /* Ok, part can be treated               */

}

/***********************************************************************/
/* This procedure generates initial state of obj->word->(SWORD)
   via its alts.
								       */
/***********************************************************************/
int16_t genwrd (SOBJ * obj)

{
 int16_t i, j;
 int16_t max_wt=0;
 register cur_wt;
 obj->word->type        = 0;
 obj->word->type_art    = obj->type_art;
 max_wt=obj->word->dif_wt      = 0;
 obj->word->low_alt_nmb = 0;
 obj->word->bad_alt_nmb = 0;

 for (i=0; i< obj->word->lth; i++)
  {
   j = obj->word->altn[i];
   obj->word->type    |= obj->word->pos[i]->alt[j].type; /* accumulate type */
   obj->word->type_sp |= obj->word->pos[i]->type_sp;
/*
   obj->word->dif_wt += obj->word->pos[i]->alt[j].dif_wt;
*/
/*Lepik */
   if(max_wt< (cur_wt= obj->word->pos[i]->alt[j].dif_wt))
              max_wt = cur_wt;

   if (obj->word->pos[i]->alt[j].orig.attr < PROB_MIN)
    obj->word->bad_alt_nmb++;
   else
    if (obj->word->pos[i]->alt[j].orig.attr < PROB_ALLOWED)
     obj->word->low_alt_nmb++;
  }
 /*Lepik */
 obj->word->dif_wt=max_wt;
 obj->word->type_sp &= (~T_BLANK);
 if ( (obj->part->type_sp & T_BLANK) )
   obj->word->type_sp|=T_BLANK;
  {
   if (! (obj->word->type &         /* no fascination and no ordinary symb */
       (T_ALPHA|T_DIG|T_CAP|T_LOW) ) )
    {
     /* obj->word->type |= T_BAD;  then: word cannot be estimated: all BADs */
     return (No);              /* word is not worthy of treatment          */
    }
/* else */                                       /* if ordinary symbs      */

  /*  if ( (obj->word->type & T_ALPHA)
       &&(obj->word->type & T_DIG)  )
     obj->word->type |= T_MIXED;
  */
    j = obj->word->altn[0];
    if (
	(obj->word->pos[0]->alt[j].type & T_CAP)  /* CAP symb in 1-st pos  */
       &&
	(!(obj->word->pos[0]->type & T_STMT))  /* and not beg of sentence  */
       &&
	(!(obj->word->type & T_DIG))           /* and no DIGs              */
       )
     obj->word->type |= T_NAME;          /* then it considered to be NAME  */
  }
 if(
    (obj -> word -> type & T_CAP) &&
    (!( obj -> word -> type & (T_LOW | T_DIG)))
   )  obj -> word -> type &= ~T_NAME;


 if (!(obj->word->type_art & (T_ALTS|T_REPL|T_GC|T_BRK))) /* 1-st word of part   */
  obj->type_orig = obj->word->type;    /* and no arts - store orig context */

 return (Ok);                    /* Ok, word can be treated                */

}

/*************************************************************************/

/*************************************************************************/
