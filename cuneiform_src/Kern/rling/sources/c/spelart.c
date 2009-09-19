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

// 08-13-93 09:27pm, Mike
// FIle has been modified!!!
// Function loadArtBase.


/* spellart.c */
/* *************************************************************** */
/* Spelling checker:
   optional functions responsible for
   artificial changes of words and parts.

   The following kinds of changes can be applied to a uint16_t:
   1. Try to remove special tails ( "'", "'s", ...).
      No ed-file changes will result.

   The following kinds of changes can be applied to a PART:
   1. Try to find and apply special replacements ("m"->"rn", "rn"->"m", ...).
   2. Glueing BLANK with it neighbours when narrow
      or cutting BLANK when wide can be applied to a PART.
   In both cases the part is changed and ed-file changes will result.
								     */
/* ***************************************************************** */

  #include "spelmode.h"
  #include <stdlib.h>
  #include <ctype.h>

  #include "tigeremulate.h"
  #include "speldefs.h"
  #include "spelfunc.h"
//  #include "iolib.h"
  #include "lang.h"

#include "compat_defs.h"

extern STD std;
extern uchar fax1x2;
extern int16_t ABCSize;
extern int16_t vocs_NOK;
LT stddig[] =
  {
    {'0',0}, {'1',0}, {'2',0}, {'3',0}, {'4',0}, {'5',0},
    {'6',0}, {'7',0}, {'8',0}, {'9',0}
  };
/**************************************************************************/
/*************            Function  Prototypes.       *********************/
/**************************************************************************/

static int16_t nextart (SOBJ * obj);
static int16_t setart (SOBJ * obj);
static int16_t  checkart_permit (SOBJ * obj);
static int16_t checkart (SOBJ * obj, int16_t pi1, int16_t pi2, struct artstr * a);
static int16_t checkart1 (SOBJ * obj, int16_t pi1, int16_t pi2, struct artstr * a);
static int16_t  set_act_art (SOBJ * obj, int16_t act_art[]);
static int16_t  setart_new (SOBJ * obj, int16_t * pa, uchar code,uchar type);

static int16_t max_art;      /* last index */

ARTS artbase[MAX_ARTS] =

/*
| what repl  |  by what  |   type  |type_sp  |cond for source  |cond for obj |
|  sr1 | sr2 |obj1 |obj2 |obj1|obj2|obj1|obj2| sr1 | sr2 | sr12|obj12x|obj12r|
*/
{{0    ,0    ,0    ,0    ,   0,   0,   0,   0, ANY_S,ANY_S,ANY_S,ANY_S,ANY_S},
 {'?'  ,BLANK,BLANK,0    ,   0,   0,0x06,   0, ANY_S,ANY_S,ANY_S,ANY_S,ANY_S},
 {BLANK,0    ,BLANK,BLANK,   0,   0,0x06,0x06, ANY_S,ANY_S,ANY_S,ANY_S,ANY_S},
 {BLANK,'?'  ,BLANK,0    ,   0,   0,0x06,   0, ANY_S,ANY_S,EMB_S,ANY_S,ANY_S},
 {BLANK,BLANK,BLANK,0    ,   0,   0,0x06,   0, ANY_S,ANY_S,EMB_S,ANY_S,ANY_S},
 {'?'  ,0    ,BLANK,0    ,   0,   0,0x06,   0, BAD_S,ANY_S,ANY_S,ANY_S,ANY_S},
 {'?'  ,0    ,BLANK,BLANK,   0,   0,0x06,0x06, BAD_S,ANY_S,ANY_S,ANY_S,ANY_S},
 {'?'  ,'?'  ,BLANK,0    ,   0,   0,0x06,   0, BAD_S,ANY_S,EMB_S,ANY_S,ANY_S},
 {'?'  ,'?'  ,BLANK,0    ,   0,   0,0x06,   0, ANY_S,BAD_S,EMB_S,ANY_S,ANY_S},
 {'?'  ,'?'  ,BLANK,0    ,   0,   0,0x06,   0, DEL_S,DEL_S,EMB_S,ANY_S,ANY_S},

#ifdef S_COMMENTS

 {'m'  ,0    ,'r'  ,'n'  ,0x22,0x22,   0,   0, ANY_S,ANY_S,ANY_S,ANY_S,ANY_S},
 {'r'  ,'n'  ,'m'  ,0    ,0x22,   0,   0,   0, ANY_S,ANY_S,ANY_S,ANY_S,ANY_S},
 {'n'  ,'n'  ,'r'  ,'m'  ,0x22,0x22,   0,   0, ANY_S,ANY_S,ANY_S,ANY_S,ANY_S},
 {'d'  ,0    ,'c'  ,'l'  ,0x22,0x22,   0,   0, ANY_S,ANY_S,FAX_M,ANY_S,ANY_S},
 {'c'  ,'l'  ,'d'  ,0    ,0x22,   0,   0,   0, ANY_S,ANY_S,FAX_M,ANY_S,ANY_S},
/* {'v'  ,'v'  ,'w'  ,0    ,0x22,   0,   0,   0, ANY_S,ANY_S,ANY_S,ANY_S,ANY_S},*/
 {'c'  ,0    ,'e'  ,0    ,0x22,   0,   0,   0, ANY_S,ANY_S,ANY_S,ANY_S,ANY_S},
 {'e'  ,0    ,'c'  ,0    ,0x22,   0,   0,   0, ANY_S,ANY_S,ANY_S,ANY_S,ANY_S},
/* {'I'  ,0    ,'l'  ,0    ,0x22,   0,   0,   0,ANY_S,ANY_S,ANY_S,ANY_S,ANY_S},*/
/*{'O'  ,0    ,'0'  ,0    ,0x04,   0,   0,   0,ANY_S,ANY_S,ANY_S,ANY_S,ANY_S},*/
  {'l' ,0    ,'i'  ,0    ,0x22,   0,   0,   0, ANY_S,ANY_S,ANY_S,ANY_S,ANY_S},
/*  {'I' ,0    ,'i'  ,0    ,0x22,   0,   0,   0, ANY_S,ANY_S,ANY_S,ANY_S,ANY_S},*/
/*  {'r' ,'\'' ,'r'  ,0    ,0x22,   0,   0,   0, ANY_S,ANY_S,ANY_S,ANY_S,ANY_S},*/
/*  {'H' ,0,    'f'  ,'f'  ,0x22,0x22,   0,   0, ANY_S,ANY_S,ANY_S,ANY_S,ANY_S},*/
 {'a'  ,','  ,'a'  ,0    ,0x22,   0, 0x0,   0, ANY_S,ANY_S,EMB_S,ANY_S,ANY_S}
#endif

};

/* ------------------------------------------------------------------ */

static int16_t AlreadyLoaded=0;
static int Built_inSize;


// 08-13-93 09:44pm, Mike
// New stream technology.

Bool loadArtBase (int16_t CountryCode)
{
	int16_t h;
	TabFH f;
	int16_t   i;

	// 08-13-93 09:44pm, Mike
	// Load *.tab file.
	if (vocs_NOK)
		return FALSE;

	h = TBOPEN( (int16_t)9, (int16_t)language, (int16_t)(O_RDONLY|O_BINARY), (int16_t)S_IREAD );
	if ( h == -1)
	{
		vocs_NOK |= 1<<9;
		return FALSE;
	}
	if ( TGREAD(h, (char *) (&f), sizeof (f))!= sizeof(f))
	{
		TGCLOSE(h);
		return FALSE;
	}
	if (strcmp (f.id, TABFILE_ID))
	{
		TGCLOSE(h);
		return FALSE;
	}

	if ( !AlreadyLoaded )
	{
		for ( i = 1; i <MAX_ARTS; i++)
		{
			if ( !artbase[i].sr1 )
			{
				break;
			}
		}
		AlreadyLoaded = 1;
		Built_inSize  = i;
	}
	else
	{
		i = Built_inSize;
	}

	if (MAX_ARTS -i < f.rep_no)
	{
		TGCLOSE(h);
		return FALSE;
	}

	//memset( artbase+i, 0, sizeof(ARTS)*(MAX_ARTS-i) ); // Bound
	memset( &artbase[i], 0, (&artbase[MAX_ARTS] - &artbase[i]) ); // Bound

	if ( TGREAD( h, (char *) (artbase+i), sizeof(ARTS)*f.rep_no ) != (int32_t)(sizeof(ARTS)*f.rep_no) )
	{
		TGCLOSE(h);
		return FALSE;
	}

	max_art = i +f.rep_no -1;
	TGCLOSE(h);

	CountryCode=0;
	return TRUE;
}


/* ********************************************************************** */
/* This procedure changes a word under treatment, trying to
   find a new one in a dictionary.
   The 1-st kind of change is applied here to a word:
   1. Try to remove special tails ( "'", "'s", ...).
      No ed-file changes will result.
									  */
/* ********************************************************************** */
int16_t wordchange (SOBJ * obj)

{
 if (!(obj->word->type_art & T_CHANGE))
    { int16_t lth;
      uchar *b;
      lth = obj->word->lth-1;
      b = obj ->wordchar;
      switch (b[lth])
	 {
       case ')' :
                 if (lth >=3)
                    if ( (b[--lth]== 's') && (b[--lth] == '(') )
                       goto Cut;
                    else break;
 /*
	   case 's' :
	   case 'S' :
                 if(( b[--lth] != '\'') && ( b[lth] != '('))
                     break;
 */
Cut:   case '\'':
		      obj->word->type_art |= T_CHANGE;
		      obj->word->lth=lth;
		      return (Ok);
	 }
     }
 return (No);                   /* No: cannot do any change of the word */
}

#ifdef PARTSPEC

/* ********************************************************************** */
/* This procedure looks for any special replacements
   like ("m"->"rn", "rn"->"m", ...) among 1-st alts
   of a part being considered. It stores current repl
   and looks for the next one.
   Reply:
    Ok - repl done;
    No - no more repl can be done
									  */
/* ********************************************************************** */
int16_t partspec (SOBJ * obj, SPART part[])
 {
  if (!(obj->type_art & (T_GC|T_BRK)))    /* is glue-cut being considered ?           */
   {
     if (obj->type_art & T_REPL) /* is it 1-st time of spec repl ?        */
	setpart (obj, part);         /* No: reset part's initial positions*/
      else
       {
	obj->type_art |= T_REPL;     /* flag                            */
	memset (&(obj->art[0]),0, sizeof(obj->art));
	obj->artn = -1;              /* 1-st find art            */
      }
  do
   do
      if (!nextart (obj)) return(No);          /* find next repl               */
	 while (!setart (obj));
	   while(!genpart (obj));         /* successful generation ?                   */
  return (Ok);              /* Ok: spec repl done                        */
 }
return (No);                 /* No: cannot do any more with the spel repls*/

}

#endif

#ifdef GLUE_CUT

/* ************************************************************************ */
/*
									    */
/* ************************************************************************ */
int16_t partgc (SOBJ * obj, SPART part[])

{
if(!(obj->type_art & T_BRK))
 {
 if(!(obj->type_art & T_GC))    /* is glue-cut being considered 1-st time ? */
  {
   obj->type_art |= T_GC;       /* flag                                     */
   memset (&(obj->art[0]),0, sizeof(obj->art));
   obj->artn = -1;              /* 1-st find art                            */
  }
 setpart(obj,part);
   do
    do
      if (!nextart (obj)) return(No);          /* find next repl               */
	 while (!setart (obj));
	   while(!genpart (obj));         /* successful generation ?                   */

   return (Ok);                 /* No: cannot do any changes more           */
}
return(No);
}
/**************************************************************************/
int16_t  partbrk(SOBJ*obj,SPART  part[])

{
 if(!(obj->type_art & T_BRK))    /* is glue-cut being considered 1-st time ? */
  {
   obj->type_art |= T_BRK;       /* flag                                     */
   memset (&(obj->art[0]),0, sizeof(obj->art));
   obj->artn = -1;              /* 1-st find art                            */
  }
 setpart(obj,part);
   do
    do
      if (!nextart (obj)) return(No);          /* find next repl               */
	 while (!setart (obj));
	   while(!genpart (obj));         /* successful generation ?                   */

   return (Ok);                 /* No: cannot do any changes more           */
}

#endif

#ifdef ART

/* ********************************************************************** */
/* This procedure gives next state of artificial change of a part.
    obj->artn stores index of curr obj->part->posn[] modified,
    obj->artn = {0, obj->part->lth}.
   It may be used only for debugging & watch.
   Is essence, the only use is:
    if (obj->artn =-1) => it is 1-st enter, but not next;

   Local variable ind_in_obj = obj->part->posn[obj->artn],
    it specifies index of obj->art[pi],
    each ind_in_art = obj->art[pi1] stores last artificial change applied to this pos
									  */
/* ********************************************************************** */
 int16_t nextart (SOBJ * obj)
 {
  register int16_t  ind_in_part;
  int16_t  ind_in_art;
  int16_t  fst_in_obj;
  int16_t  nxt_in_obj;
  int16_t  ret=No;

  ind_in_part=obj->artn;

  if(ind_in_part==-1)             /* Always -1 ! */
   {
     if(!checkart_permit(obj))
	   return(No);
    obj->max_art=max_art;
/*    ind_in_part=obj->part->lth; */

      if ( obj -> part -> lth < obj -> allowedlth)
             obj -> allowedlth = obj -> part -> lth;

       ind_in_part   =  obj -> allowedlth;
   }
  while((ind_in_part)>=0)
   {
    fst_in_obj=obj->part->posn[ind_in_part];
    nxt_in_obj=(ind_in_part==obj->part->lth) ?
       0  :   obj->part->posn[ind_in_part+1];
    ind_in_art=obj->art[fst_in_obj];

    while(++ind_in_art<=obj->max_art)
	if(ret=checkart(obj,fst_in_obj,nxt_in_obj,&artbase[ind_in_art]))
	   { obj->art[fst_in_obj]=ind_in_art;
	     break;
	   }
    if(ret==Ok)
	break;
    ind_in_part--;
   }
  if(ret==Ok)
      {/* if(obj->type_art & (T_BRK | T_GC))*/ obj->artn=ind_in_part;
	while((++ind_in_part)<=obj->part->lth)
	   obj->art[obj->part->posn[ind_in_part]]=0;
      }
return(ret);
}
/* ********************************************************************** */
/* This procedure checks whether artificial change allowed for the part.
   Reply:
    Ok - allowed;
    No - otherwise
									  */
/* ********************************************************************** */
int16_t  checkart_permit (SOBJ * obj)

{
 int16_t val=0, lth=0, lthmin=0;
 int16_t i;

 /* lth = obj->part->lth+1; */
 for (i = 0; i <= obj -> part ->lth; i++)
   if ( !(obj ->pos[obj -> part ->posn[i]].alt[0].type & T_DELIM))
      lth ++;
 if ( lth == 0) return No;

 if (obj->part->type & T_NAME)
  lthmin = RMpart_art_minlth_nam;
 else
  {
   if ( (obj->part->type & T_CAP) && (!(obj->part->type & T_LOW)) )
    lthmin = RMpart_art_minlth_cap;
 /*  if ( (obj->part->type & T_LOW) && (!(obj->part->type & T_CAP)) ) */
   else
    lthmin = RMpart_art_minlth_low;
  }

 val = (obj->part->blank_nmb * RMpart_blank_pen) /lth ;
 if ( (obj->part->blank_nmb > RMpart_art_max_blank)
    ||(lth            < lthmin)
    ||(val > RMpart_artno)
    )
   return (No);

 return (Ok);
}

/* ********************************************************************** */
/* This procedure checks whether obj fits modification specified by artbase,
   pi - pos to modify,
   a  - artbase[] el which defines modification.
									  */
/* ********************************************************************** */
 int16_t checkart (SOBJ * obj, int16_t pi1, int16_t pi2, struct artstr * a)

{
 uchar sr1,sr2;

 sr1 = a->sr1;
 sr2 = a->sr2;

 if ((obj->type_art&T_REPL)&&(!(obj->type_art&T_GC))) /*certain repl active?*/
  if ( (sr1==BLANK)||(sr2==BLANK) ) /* but it is BLANK modification ?       */
   goto No_checkart;                /* cannot do it                         */
 if ((obj->type_art&T_GC)&&(!(obj->type_art&T_BRK)))    /* blank repl active (i.e. glue-cut) ?  */
  if (! ((sr1==BLANK)||(sr2==BLANK)) ) /* but it is not BLANK modification ?*/
   goto No_checkart;                /* cannot do it                         */
  if(!(obj->type_art & T_BRK))
   {
    if((a->cond_sr1==BAD_S)||(a->cond_sr2==BAD_S))
     goto No_checkart;
   } else
    if(!((a->cond_sr1==BAD_S)||(a->cond_sr2==BAD_S)))
     goto No_checkart;
 if (sr1!=0)
  if (sr1!='?')
   if (obj->pos[pi1].orig.code!=sr1)
    goto No_checkart;
 if (sr2!=0)
  {
   if ((pi2 >= obj->part_end)||(pi2 == 0)) /* is 2-nd pos within the part ? */
    goto No_checkart;             /* No - cannot do the change              */
   if (sr2!='?')
    {
     if (obj->pos[pi2].orig.code!=sr2)
      goto No_checkart;
    }
  }

 if ( !(checkart1 (obj, pi1, pi2, a)) )
  goto No_checkart;

 return (Ok);                   /* Ok: object fits the modification       */

No_checkart:
 return (No);                   /* No: cannot do such a modification      */

}

  // Valdemar             ¬    ¦    и    й    о    ®    Ї
static uchar wide_let[]={ 0xec,0xe6,0xf8,0xf9,0xfe,0xce,0xcf };
/* ********************************************************************** */
/* This procedure checks whether pos1|pos2 to be modified
   meets all conditions, specified by artbase.
    pi1,pi2 - pos to modify,
    a  - artbase[] el which defines modification
									  */
/* ********************************************************************** */
 int16_t checkart1 (SOBJ * obj, int16_t pi1, int16_t pi2, struct artstr * a)

{  int16_t  type;
 switch (a->cond_sr1)
  { case  BAD_S : if(obj->pos[pi1].orig.attr>PROB_TO_BRK)
		      return(No);
		  break;
    case  DEL_S:  if((type=symcode((char  *)&(obj->pos[pi1].orig.code)))
		     !=E_DL)if(type!=_APF) return(No);
		  break;
  }
 switch(a->cond_sr2)
  { case  BAD_S : if(obj->pos[pi2].orig.attr>PROB_TO_BRK)
		      return(No);
		  break;
    case  DEL_S:  if(type=symcode((char  *)&(obj->pos[pi2].orig.code))
		     !=E_DL) if(type!=_APF)return(No);
		  break;
    // Le + Vova    01-24-94 03:45pm
    case  WID_S:  if (!(MEMCHR(wide_let,
                            *((char  *)&(obj->pos[pi2].orig.code))
                            ,sizeof wide_let )) ||
                            language != LANG_RUSSIAN ) return (No);
                  break;
  }
 switch(a->cond_sr12)
  {
    case EMB_S :  if((int16_t)obj->pos[pi2].tif_ref.col-
		     (int16_t)obj->pos[pi1].tif_ref.col -
		     (int16_t)obj->pos[pi1].tif_ref.width>1) return(No);
		  break;
    case FAX_M :  if (!fax1x2) return(No);
		  break;
  }
 /*
    The following euristic prevents merging of last 's' with
       previous low-attribute symbol.
                        08/06/92 03:15pm, L.
  */
if ( a->sr2)
   if(
      (
        (obj -> pos[pi2].orig.code == 'S')
       ||
        (obj ->pos[pi2].orig.code == 's')
       )
        &&
       ( pi2 == obj->part -> lth)
        &&
       (obj ->pos[pi2].orig.attr >= PROB_TO_BRK)
    )                                             return NO;


   return(Ok);
}

/* ********************************************************************** */
/* This procedure constructs a part with the modifications,
   specified by obj->alt[].
   Nothing is used from the part, but :
    the pointer obj->part is ready and
    obj->part_beg - beg position of a part;
    obj->part_end - end position of a part.
   Note1: part may include spsp-positions (i.e. SP1, SP2, HYPHEN).
   Note2: all part changes are being stored in obj->part->art[];
	  these are identifiers of changes
	  in respective original (but not new !) part positions.

   Reply:
 I   Ok - part constructed;
    No - otherwise (buff overflowed or object itself is BAD)
								       */
/***********************************************************************/
 int16_t setart (SOBJ * obj)

{
 int16_t pa1;       /* obj->part->art[pa1]  - old position index of obj->art[]  */
 int16_t pa2;       /* obj->part->posn[pa2] - new position index of obj->pos[]  */
 int16_t pi1,       /* curr obj-pos to change                                   */
     ai;        /* current art-change-id                                    */
 int16_t act_art [MAX_VIEW_SIZE]; /* art ids  actually used for changes         */
  SPART savepa;
#ifdef ARTPR_ALL
 printarts(obj);
#endif
 set_act_art(obj,act_art); /* set copy of obj->art[] for setart action:    */
			   /* arts may overlapped, so the least important  */
			   /* of them must be excluded                     */
  memcpy(&savepa,obj->part,sizeof(SPART));
 memset (obj->part,0,sizeof(SPART));  /* part initial state       */
 obj->part->beg = obj->part_beg;  /* part beg                              */
 obj->part->end = obj->part_end;  /* and end                               */
 obj->part->begi = obj->part_begi;/* part beg index in obj->pos_part[]     */
 obj->part->endi = obj->part_endi;/* and  end index in obj->pos_part[]     */
 obj->art_minpos = MAX_VIEW_SIZE; /* curr min pos
			   in obj->pos[] to be used for artificial changes */
			 /* construct all part-positions                   */
 for (pa1=0, pa2=0, pi1=obj->part_beg;
      pi1<obj->part_end;
      pi1++)
  {
   if (!(obj->pos[pi1].type_sp & (T_SP1|T_SP2|T_HYPHEN))) /* spsp-position ?*/
    {     		       /* No, set part pos :                        */
     ai = act_art[pi1];        /* id of artificial change                   */
     if (!(ai))                /* is there change in this pos               */
      {                        /* No, set as in setpart:                    */
       obj->part->posn[pa2] = (uchar)pi1;
       pa1++; pa2++;/*new indxs of obj->part->posn[pa2]&obj->part->art[pa1] */
      }
     else                      /* set change of the part                    */
      {
       obj->part->art[pa1]=(uchar)ai; /* store artificial change id                */
       pa1++;                  /* next old pos                              */
       if (artbase[ai].sr2)    /* two-pos change ?                          */
	{                      /* Yes, find next pos after pi1 :            */
	 do pi1++;             /* skip this & all next spsp&hyphen-positions*/
	  while (obj->pos[pi1].type_sp & (T_SP1|T_SP2|T_HYPHEN));
	}
       if (!(setart_new (obj,&pa2,artbase[ai].obj1,artbase[ai].objt1)))/*pos1*/
	goto No_setart;         /* buff obj->pos[] overflowed                */
       if (!(setart_new (obj,&pa2,artbase[ai].obj2,artbase[ai].objt2)))/*pos2*/
	 goto No_setart;        /* buff obj->pos[] overflowed                */
      }
    }
  }
 obj->part->lth=pa2-1;         /* length of part (last index no.)           */

 /*if (genpart (obj,part,i)) */   /* successful generation ?             */

  return (Ok);                 /* Yes                                       */
No_setart:
 memcpy(obj->part,&savepa,sizeof(SPART));
 return (No);                  /* No - KNMOHO-TO-XEPOBATO                   */
}

/***********************************************************************/
/* This procedure sets copy of obj->art[] for setart action.
   Not all of arts specified in obj->art[] are actually used.
   This is because arts may overlapped, so some of them must
   be excluded. The procedure excludes the least important arts
								       */
/***********************************************************************/
int16_t  set_act_art (SOBJ * obj, int16_t act_art[])

{
 int16_t pa1;       /* obj->part->art[pa1]  - old position index of obj->art[]  */
#ifdef ARTPR_REGECT
 int16_t pa2,       /* obj->part->posn[pa2] - new position index of obj->pos[]  */
	pi2; /* next            obj-pos to change (temporary used)       */
#endif
 int16_t pi1=0, /* Lepik's zero,it needs to examine...*/
 pi1m,  /* curr & previous obj-pos to change                        */
     ai;        /* current art-change-id                                    */
 int16_t aix;

 memset (&(act_art[0]),0,sizeof(int16_t)*MAX_VIEW_SIZE); /* act_art init state */
 for (ai=1; ai /*< MAX_ARTS*/ <=max_art; ai++)
  {
   for (pa1=0; pa1<=obj->part->lth; pa1++)
    {
     pi1m = pi1;                    /* previous pos           */
     pi1  = obj->part->posn[pa1];   /* current pos            */
     if (!(obj->art[pi1]))          /* is art in curr pos     */
      continue;                     /* No                     */
     if (obj->art[pi1]!=ai)         /* is art == curr art considered ? */
      continue;                     /* No                     */
     if (!pa1)                      /* the very left pos ?    */
      goto Actart_copy;             /* Yes - set& check right */
     if (!(act_art[pi1m]))         /* is art already in prev ?*/
      goto Actart_copy;             /* No - can do art        */
				    /* art in prev: check sr2 */
     aix = act_art[pi1m];           /* art of prev pos        */
     if (!(artbase[aix].sr2))       /* the prev is 2-pos art ?*/
      goto Actart_copy;             /* No => can do art       */
				    /* cannot do art here:    */
#ifdef ARTPR_REJECT
     PRINTF (
     "\n %d. WARNING (SPELLART.SET_ACT_ART): Cannot do change in pos %d\n",
	      obj->nmb, pi1);
     pi2 = obj->part->posn[pa1+1];  /* next pos if it were  */
#ifdef ARTPR
     printart1 (obj, pi1, pi2, ai);
#endif
#endif
     obj->art[pi1] = obj->max_art; /* max art id            */
     continue;
Actart_copy:
     act_art[pi1] = obj->art[pi1];   /* copy art if it were    */
    }
  }
return(OK);
}

/* ********************************************************************** */
/* This procedure allocates new position for artificial change of part.
    The art-positions are temporary used as last pos-s of obj->pos[].
    obj->art_minpos - last currently used pos (minimum),
    initially it is = obj->max_pos;

   New pos with alt specified by code = artbase[].{obj1 or obj2}
   is being constructed with its number stored in obj->part->posn[].

   Reply:
    Ok - the change is done;
    No - otherwise (buff obj->pos[] is overflowed)
								       */
/***********************************************************************/
int16_t  setart_new (SOBJ * obj, int16_t * pa,
                uchar code,
                uchar type)

{
 int16_t posn, stdn;
 int16_t i;
 LT  * lt;
 char  * pc;
 uchar   c;              // Valdemar+Lepik for russian

 if (!code)
  goto Ok_setart_new;      /* code=0 => nothing to do                       */
 posn = obj->art_minpos;   /* curr pos-buf used to construct new pos        */
 if (posn <= obj->pos_part[obj->pos_part_nmb]) /* obj->pos[] & []pos<-obj   */
  {	 			               /*     ? overlapped ?        */
#ifdef SYSPR_ERROR
   PRINTF(                                     /* Yes:                      */
     "\n SYSTEM ERROR (SPELLART.SETPART_NEW): No buf for pos to change !\n");
#endif
   goto No_setart_new;
  }
 posn--;

 memset (&(obj->pos[posn]),0,sizeof(SPOS));
 obj->pos[posn].type_art = obj->type_art; /* it's art pos                   */

 if (code==BLANK)                                /* blank  ? */
  {                                              /* Yes:     */
   obj->pos[posn].type_sp|= (T_BLANK|T_MYBLANK); /* it's blank pos                  */
   obj->pos[posn].       orig.code = BLANK;
   obj->pos[posn].alt[0].orig.code = BLANK;
   obj->pos[posn].       lt        = (LT  *)&(std.stdlt[0]);
   obj->pos[posn].alt[0].lt        = (LT  *)&(std.stdlt[0]);
   for (i=0, lt=(LT  *)&(std.stdlt[0]); i<ABCSize; i++,lt++)                          /* set all possible alts A-Z:   */
    obj->pos[posn].alt[i].lt = lt;              /* all ltrs in std              */
  }
 else                           /* No: certain alternative                   */
  {
   obj->pos[posn].       orig.code = code;
   obj->pos[posn].alt[0].orig.code = code;
   obj->pos[posn].       orig.attr = PROB_ALLOWED;
   obj->pos[posn].alt[0].orig.attr = PROB_ALLOWED;
   obj->pos[posn].       type      = type;
   obj->pos[posn].alt[0].type      = type;
   c = code;
   pc = &c;
   if (symcode(pc)==_DIG)                        /* digit  ? */
    {                                            /* Yes      */
     obj->pos[posn].       lt =
         (LT  *)&(stddig[c-'0']);
     obj->pos[posn].alt[0].lt =
         (LT  *)&(stddig[c-'0']);
    }
   else
    {
     c = _2cap(code);
     stdn = getstdn(pc);
     if (std.stdlt[stdn].code == c)              /* letter ? */
      {                                          /* Yes      */
       obj->pos[posn].       lt = (LT  *)&(std.stdlt[stdn]);
       obj->pos[posn].alt[0].lt = (LT  *)&(std.stdlt[stdn]);
      }
     else
      {
#ifdef SYSPR_ERROR
       PRINTF (
	"\n SYSTEM ERROR (SPELLART.SETART_NEW): alt not found in std!!!\n");
#endif
       goto No_setart_new;
      }
    }
  }
 obj->part->posn[*pa] = (uchar)posn;
 (*pa)++;
 obj->art_minpos = posn ;  /* curr pos-buf used to construct new pos         */
Ok_setart_new:
 return (Ok);

No_setart_new:
 return (No);

}

/* ********************************************************************** */
/* This procedure outputs modifications of a part
   in accordance with the changes specified by obj->alt[]
									  */
/* ********************************************************************** */
#ifdef ARTPR_ALL

 printarts (SOBJ * obj)

{
 int16_t pa,
     pi1, pi2, ai,
     i;

 pa = obj->part->lth;         /* part end: index of obj->part->posn[]   */

 PRINTF ("\n ***** %d. Try artificial changes: ***** \n",obj->nmb);

 for (i=0; i<=pa; i++)
  {
   pi1= obj->part->posn[i];   /* ind of pos to modify                  */
   pi2= obj->part->posn[i+1]; /* ind of pos to modify                  */
   ai = obj->art[pi1];        /* id of art to apply to the pos         */
#ifdef ARTPR
   printart1 (obj, pi1, pi2, ai);
#endif
  }

}

#endif

#ifdef ARTPR

/* ********************************************************************** */
/* This procedure outputs one modification of a part
   specified by {obj->pos[pi1], obj->pos[pi2]} and artbase[ai]
									  */
/* ********************************************************************** */
 printart1 (SOBJ * obj, int16_t pi1, int16_t pi2, int16_t ai)

{
 if (ai)                          /* is it real artificial change (id!=0) */
  {                               /* Yes:                                 */
   PRINTF ("      [%d] pos [%d] repl {%c", ai, pi1, obj->pos[pi1].orig.code);
   if (artbase[ai].sr2)
    PRINTF ("%c", obj->pos[pi2].orig.code);
   PRINTF(" } by {%c", artbase[ai].obj1);
   if (artbase[ai].obj2)
    PRINTF("%c", artbase[ai].obj2);
   PRINTF("} \n");
  }
}

#endif
		   /* END OF #ifdef ART : */
#endif

/* ********************************************************************** */
/* This procedure initiates const position obj->pos[SPEC_POS],
   used to store best word alternatives resultant from
   artificial change of part.
								       */
/***********************************************************************/
int16_t init_specpos (SOBJ * obj)

{
 int16_t i;
 LT  * lt;

 memset (&(obj->pos[SPEC_POS]),0,sizeof(SPOS));
 obj->pos[SPEC_POS].type_art |= obj->type_art;
 obj->pos[SPEC_POS].       orig.code = BLANK;
 obj->pos[SPEC_POS].alt[0].orig.code = BLANK;
 obj->pos[SPEC_POS].       lt = (LT  *)&(std.stdlt[0]);
 obj->pos[SPEC_POS].alt[0].lt = (LT  *)&(std.stdlt[0]);
 for (i=0, lt=(LT  *)&(std.stdlt[0]);
      i<ABCSize;
      i++,lt++)                          /* set all possible alts A-Z:   */
  {
   obj->pos[SPEC_POS].alt[i].lt = lt;              /* all ltrs in std    */
   obj->pos[SPEC_POS].alt[i].orig.code = lt->code; /* all ltrs in std    */
/* obj->pos[SPEC_POS].alt[i].orig.attr = ????; */
  }
 return(OK);
}

