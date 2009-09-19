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

/*************************************************************************/
/*              VOCABULARY: dynamic vocabulary access .                  */
/*                                                                       */
/*                                                                       */
/*************************************************************************/
  #include "spelmode.h"

  #if defined(TURBO_C)
    #include "tc_types.h"
  #elif defined(WATCOM)
    #include "spelwatc.h"
  #elif defined (BC_FOR_WIN)
    #include "bcwtypes.h"
    typedef long signed int  int32_t;
  #else
    #error   NO TOOOL SPECIFIED
  #endif

  #include <ctype.h>

  #include "speldefs.h"
  #include "spelfunc.h"
  #include "tigeremulate.h"

/*-----------------------------------------------------------------------*/

#define SET_WRDTERM  ((vert  *) \
		       V_POINT(voc->vocseg,voc->path[voc->lev]))->wrdterm=1;

/*************************************************************************/
/*             Preliminary functions definition                          */
/*************************************************************************/

static uint16_t voc_alloc ( voc_state *, int16_t );
static int16_t voc_read ( voc_state *, LTIMG **, int16_t * );
static int16_t voc_write ( voc_state *, LTIMG **, int16_t * );
static int16_t make_tail ( voc_state *, LTIMG **, int16_t * );
static int16_t insert_brother( voc_state *, LTIMG ** );
static int16_t set_voc_accnt( voc_state *, int16_t );

static int16_t fillgap( vert  * currvert, voc_state * voc, int16_t cnt,
                    LTIMG **wrddef, int16_t gapcont );
static void chkfill( int16_t cnt, LTIMG * wrddef[], int16_t gapcont,
                     int16_t *fillfind, uchar symb, LT  **gaplt );
 static WTOKEN  * get_wtoken(voc_state *voc);
 static WTOKEN  * alloc_wtoken(voc_state *voc);
 static int16_t AcceptAndCheck ( vert  *currvert, voc_state *voc);
 static int16_t Found ( uchar *str, voc_state *voc);

/*************************************************************************/
/*                      Main function                                    */
/*************************************************************************/

int16_t voc_  (
       voc_state * voc,       /* variable of vocabulary state            */
       LTIMG ** wrddef,/* ptr to user's word def start elem       */
       int16_t *lth,              /* length of word                          */
       int16_t dcount,            /* change (+n,0,-n) of word usage counter  */
       int16_t acs,                /* access : write or read                  */
       WTOKEN  *  * wtoken
	  )

{ int16_t respons;               /* function's respons                       */

  switch ( acs ) {

    case VOC_R  :   /* search word in voc & return account               */
	     respons=voc_read(voc ,wrddef,lth);
	     if ( respons > 0 ) --*lth;
	     if((respons>0)&&wtoken)
		  *wtoken=get_wtoken(voc);
	     if ( respons > 0  &&  dcount != 0 )
              { respons= set_voc_accnt( voc, dcount);   /* set vert accnt */
              voc -> state |= VOC_CHANGED;
              }
          break;

    case VOC_W :    /* write word in voc & return account                */
	     respons=voc_write(voc ,wrddef,lth);
	     if((respons>=0)&&wtoken)
		 if(!(*wtoken=alloc_wtoken(voc))) return(VOC_MEM_OVERFLOW);

	     if ( respons >= 0  &&  dcount != 0 )
               respons= set_voc_accnt( voc, dcount);   /* set vert accnt */
         if (respons >= 0)
              voc -> state |= VOC_CHANGED;
	     break;

    default : return ( VOC_WRONG_ACCESS );        /* unused modes        */
  }

  return ( respons );
}

/*************************************************************************/
/*  Function initialise vocabulary: initialise voc_state * voc structure */
/*   ( lev=path[]=vocfree=0 ) and write root vertex with zeroed fields.  */
/*                                                                       */
/* input: voc_state * voc - structure of current state of vocabulary     */
/*                                                                       */
/* output: Zero, if all right.                                           */
/*         Error message VOC_TOO_SMALL, if MAX_VOC_SIZE less then even   */
/*         size of vertex.                                               */
/*                                                                       */
/*************************************************************************/

int16_t voc_init (
      voc_state * voc         /*structure of current state of vocabulary */
      )
{ vert  * currvert;        /* serv pointer to current vertex in RAM   */

  voc_open(voc);
  MOVENEARTOFAR ( V_POINT(voc -> vocseg,0), DYN_DICT_HEADER,
					       strlen(DYN_DICT_HEADER ));
  voc->vocfree= sizeof(DYN_DICT_HEADER);
/*
#if ( sizeof(vert) > MAX_VOC_SIZE )
#error Constant MAX_VOC_SIZE is too small
#endif
*/
  voc->path[0]=voc_alloc(voc,sizeof(vert));
  currvert=(vert  *)V_POINT(voc->vocseg,voc->path[0]);
  currvert->left=currvert->right=currvert->down=currvert->wtoken=0;
  currvert->wrdterm=currvert->accnt=currvert->key=0;

  return(0);
}

/*************************************************************************/
/*   Function initialise variables of vocabulary state lev & path[]      */
/*    by beginig values, e.i. open vocabulary.                           */
/*                                                                       */
/* input : voc_state * voc - structure of current state of vocabulary    */
/*                                                                       */
/* output : none                                                         */
/*                                                                       */
/*************************************************************************/

void voc_open (
      voc_state * voc         /*structure of current state of vocabulary */
      )
{ register int16_t i;

  voc->lev=-1;
  for ( i=0; i < MAX_WORD_SIZE ; i++ ) voc->path[i]=0;
  voc -> state = 0;
}

/*************************************************************************/
/*   Function search character sequense wrddef[]->lt->code with length   */
/*   ( *lth + 1 ) (!) in vocabulary.                                     */
/*                                                                       */
/* input :                                                               */
/*   voc_state * voc        - structure of current state of vocabulary.  */
/*   LTIMG ** wrddef - ptr to user's wrddef start elem.           */
/*   int16_t * lth              - ptr to length of user's input wrddef       */
/*                            sequence.                                  */
/* output:                                                               */
/*   if word was found, then returned value of accnt field of the last   */
/*   letter of wrddef sequence vertex, else 0 returned.                  */
/*   if error, then error code ( < 0 ) would be returned.                */
/*                                                                       */
/* non direct output:                                                    */
/*   current state of vocabulary will be changed and will point to path  */
/*   of found (!) part of word.  *lth will point to the first non found  */
/*   element of wrddef.                                                  */
/*************************************************************************/

static int16_t voc_read (
      voc_state * voc,        /*structure of current state of vocabulary */
      LTIMG ** wrddef, /* ptr to user's wrddef start elem         */
      int16_t *lth                /* ptr to length of user's input wrddef    */
      )                       /* sequence ( in sense C array index !)    */
{
  vert  * currvert;                 /* pointer to current vertex      */
  int16_t cnt;                             /* index in input wrddef          */
  int16_t rest;                            /* rest of input sequence         */

      /* -- loop through the input character sequence */
  if ( voc->lev == -1 )
   { currvert=(vert  *)V_POINT(voc->vocseg,sizeof(DYN_DICT_HEADER));
    if ( currvert -> down ==0)  {cnt =0;    goto Fail;}

   }
   else
     currvert=(vert  *)V_POINT(voc->vocseg,voc->path[voc->lev]);

  currvert=(vert  *)V_POINT(voc->vocseg,currvert->down);

  for ( cnt=0,rest=*lth ; rest >= 0 ; rest--,cnt++ )
  {
    if (wrddef[cnt]->blank==1)
      if ( !fillgap(currvert,voc,cnt,wrddef,0) )
	{
	  cnt--;
	  goto Fail;
	}

      /* -- define the direction of move & search through level */
    if ( currvert->key < _2cap(wrddef[cnt]->lt->code) )
      while ( currvert->key < _2cap(wrddef[cnt]->lt->code)
	       && currvert->right != 0 )
        currvert=(vert  *)V_POINT(voc->vocseg,currvert->right);
    else
      while ( currvert->key > _2cap(wrddef[cnt]->lt->code)
	       && currvert->left != 0 )
        currvert=(vert  *)V_POINT(voc->vocseg,currvert->left);

      /* -- if needed vertex not found on level: break loop and return 0 */
    if ( currvert->key!=_2cap(wrddef[cnt]->lt->code) ) goto Fail;

       /* -- change vocabulary state */
    voc->lev++;
    voc->path[voc->lev]=V_OFF(currvert,voc->vocseg);/* save ptr to last found vertex*/
    if ( currvert->down != 0 )
      currvert=(vert  *)V_POINT(voc->vocseg,currvert->down); /* -- pointer to son     */
    else if ( rest == 0 )
      { rest--; break; }
    else  goto Down_Fail;
  }

  if ( rest < 0 )
    { ++*lth;
      return ((vert  *)V_POINT(voc->vocseg,voc->path[voc->lev]))->accnt ;
    }
  else goto Fail;

Fail:      *lth=cnt;
	   return 0;

Down_Fail: *lth=cnt+1;
	   return 0;
}

/*************************************************************************/
/*   Function write character sequense wrddef[]->lt->code with length    */
/*   ( *lth + 1 ) (!) in vocabulary.                                     */
/*                                                                       */
/* input :                                                               */
/*   voc_state * voc        - structure of current state of vocabulary.  */
/*   LTIMG ** wrddef - ptr to user's wrddef start elem.           */
/*   int16_t * lth              - ptr to length of user's input wrddef       */
/*                            sequence.                                  */
/* output:                                                               */
/*   if word was written, returned value of accnt field of the last      */
/*   letter of wrddef sequence vertex, which set 0+dcount.               */
/*   if error, then error code ( < 0 ) would be returned.                */
/*                                                                       */
/* non direct output:                                                    */
/*   current state of vocabulary will be changed and will point to path  */
/*   of found (!) part of word.  *lth will point to the first non written*/
/*   element of wrddef.                                                  */
/*                                                                       */
/* Note  : if all right, the last vertex of written sequence have flag   */
/*         wrdterm == 1. if error, the whole written sequence of         */
/*         vertexes contains flags wrdterm == 0.                         */
/*                                                                       */
/*************************************************************************/

static int16_t voc_write (
       voc_state * voc,       /*structure of current state of vocabulary */
       LTIMG ** wrddef,/* ptr to user's wrddef start elem         */
       int16_t *lth               /* ptr to length of user's input wrddef    */
       )                      /* sequence ( in sense C array index !)    */
{ int16_t respons=0;
  int16_t mylth;
  int16_t buf;

  mylth=*lth;

  if ( (respons=voc_read(voc,wrddef,lth)) > 0 )
    return respons ;                /* - word doesn't exist in vocabulary */
  else
    if ( respons < 0 ) goto Out;    /* -- error in voc_read()            */
  else                              /* - path or whole word absent in voc*/
   { buf=*lth;                      /* - set shift to new wrddef subseq. */
     mylth=mylth-buf;               /* -- set new length of input subseq.*/
     if ( mylth<0 )
       { SET_WRDTERM;
       return respons; /* - word reached, but haven't accnt */
       }
   if (
       (
        ( voc -> lev == -1) &&
	((vert  *)
	    V_POINT(voc->vocseg,sizeof(DYN_DICT_HEADER))
	) -> down == 0
       )
    ||
    (((vert  *)V_POINT(voc->vocseg,voc->path[voc->lev]))->down == 0  )
    )
      { /* -- no brothers on down level => write tail to current vertex  */
	respons=make_tail(voc,wrddef+buf,&mylth);
	if ( respons >= 0 ) SET_WRDTERM;
	goto Out;
      }
    else
      { /* -- the last found vertex have sons => insert brother to sons */
        respons=insert_brother(voc,wrddef+buf);
        if ( respons >= 0 )  /*  -- brother inserted => change readed    */
	  { buf++,mylth--;   /*     length and tail length => write tail */
	    respons=make_tail(voc,wrddef+buf,&mylth);
	    if ( respons >= 0 ) SET_WRDTERM;
          }
        goto Out;
      }
   }
Out: *lth=buf+mylth+1;       /* -- set pointer to the first non worked   */
     return (respons);       /*    element & return                      */
}

/*************************************************************************/
/*  Function write sequense of input letters from 'wrddef' in vocabulary */
/*  with length 'lth' from level 'voc->lev'+1. Function used if vertex   */
/*  at address 'voc->path[voc->lev]' have downe-pointer equels zero,i.e. */
/*  on level 'voc->lev'+1 in current subtree we haven't vertexes.        */
/*                                                                       */
/* input : voc-state * voc        - current state of vocabulary.         */
/*         LTIMG ** wrddef - input part of word.                  */
/*         int16_t * lth              - ptr to length of input word.         */
/*                                                                       */
/* output: zero, if all right.                                           */
/*         VOC_MEM_OVERFLOW ( <0 ), if can not write whole sequence, as  */
/*         vocabulary memory overflow.                                   */
/*                                                                       */
/*************************************************************************/

static int16_t make_tail (
       voc_state * voc,       /* - structure of current state of voc     */
       LTIMG ** wrddef,/* -- ptr to user's wrddef start elem      */
       int16_t *lth               /* - ptr to length of user's input wrddef  */
       )                      /*   sequence ( in sense C array index !)  */
{ int16_t newlth;                 /* -- length of worked part of input       */
  vert  * newvert;         /* - real (  ) pointer to vertex        */
  vert  * oldvert;         /* - real ( far ) pointer to vertex        */

  if ( *lth < 0 )
    goto Success;    /* -- whole word was written above, we have nothing */
  if ( voc->vocfree+sizeof(vert) > MAX_VOC_SIZE )
     goto Fail;      /* -- we cann't write even only vertex: haven't RAM */

     /* -- zeroing wrdterm flag in vertex, which was term in tree -- */
/*
  if ( voc->lev == -1 )
    ((vert  *)V_POINT(voc->vocseg,sizeof(DYN_DICT_HEADER)))->wrdterm=0;
  else
    ((vert  *)V_POINT(voc->vocseg,voc->path[voc->lev]))->wrdterm=0;
 */
       /* -- race through input sequence of letters -- */
  for ( newlth=0; newlth <= *lth ; newlth++ ) {
    voc->lev++;         /* -- change level & allocate memory for new vert */
    voc->path[voc->lev]=voc_alloc(voc,sizeof(vert));
    if ( voc->path[voc->lev] == 0 )
      {
    oldvert=(vert  *)V_POINT(voc->vocseg, /*     HERE I try to close Mike's hole.*/
	voc->path[voc->lev-1]);    /*                                     */
	oldvert->wrdterm=1;        /*                                     */
	oldvert->down=0;           /*                    Lepik.17.05.90.  */
	oldvert->accnt=1;           /*                                     */
	 voc->lev--;                      /* -- Error : memory overflow    */
	*lth=newlth;                     /* non ended sequence isn't ended*/
	goto Fail;                       /* by flag wrdterm ( wrdterm==0 )*/
      }
                              /* -- set pointer to new vert. from parent */
    if ( voc->lev-1 < 0 ) oldvert=(vert  *)V_POINT(voc->vocseg,
                                               sizeof(DYN_DICT_HEADER));
      else    oldvert=(vert  *)V_POINT(voc->vocseg,voc->path[voc->lev-1]);
    oldvert->down=voc->path[voc->lev];
                              /* -- initialise new vertex                */
    newvert=(vert  *)V_POINT(voc->vocseg,voc->path[voc->lev]);
    newvert->key=_2cap(wrddef[newlth]->lt->code);
    newvert->left=newvert->right=0;
    newvert->wrdterm=newvert->accnt=0;
    newvert->wtoken=0;
  }
                               /* -- initialise the last of new vertexes */
  newvert->down=0;             /* - don't change lth : whole written !   */

Success : return ( 0 );          /* -- new account field contains 0      */

Fail    : return ( VOC_MEM_OVERFLOW );
}

/*************************************************************************/
/*  Function  write one vertex in level 'voc->lev'+1 and change level,   */
/*  i.e. only first letter from 'wrddef' will be written in vocabulary.  */
/*  Function used , if vertex at address 'voc->path[voc->lev]' has non   */
/*  zero down-pointer, i.e. on level 'voc->lev'+1 of current subtree     */
/*  we have one or more vertexes.                                        */
/*                                                                       */
/* input : voc-state * voc        - current state of vocabulary.         */
/*         LTIMG ** wrddef - input part of word.                  */
/*                                                                       */
/* output: zero, if all right.                                           */
/*         VOC_MEM_OVERFLOW ( <0 ), if can not write whole sequence, as  */
/*         vocabulary memory overflow.                                   */
/*                                                                       */
/*************************************************************************/

static int16_t insert_brother(
       voc_state * voc,       /*structure of current state of vocabulary */
       LTIMG ** wrddef /* ptr to wrddef start elem (def by user)  */
       )
{ vert  * currvert;        /* -- serv poINTer to vertex               */
  vert  * newvert;         /* - pointer to new vert allocated in RAM  */

	    /* -- allocate new vertex        */
  newvert=(vert  *)V_POINT(voc->vocseg,voc_alloc(voc,sizeof(vert)));
  if ( V_OFF(newvert,voc->vocseg) == 0 ) goto Fail;
  newvert->wtoken=0;
	    /* -- step to next level in tree */
  if ( voc->lev == -1 )  currvert=(vert  *)V_POINT(voc->vocseg,
                                            sizeof(DYN_DICT_HEADER));
    else   currvert=(vert  *)V_POINT(voc->vocseg,voc->path[voc->lev]);
  currvert=(vert  *)V_POINT(voc->vocseg,currvert->down);

	   /* -- select direction of stepping -- */
  if ( currvert->key < _2cap((*wrddef)->lt->code ))
    {      /* -- go through level  -- */
      while ( currvert->key < _2cap((*wrddef)->lt->code) && currvert->right != 0 )
    currvert=(vert  *)V_POINT(voc->vocseg,currvert->right);

      if ( currvert->key >_2cap( (*wrddef)->lt->code ))
	{    /* -- insert between vertexes -- */
	  newvert->left=currvert->left;
	  newvert->right=V_OFF(currvert,voc->vocseg);
          newvert->down=0;
	  currvert->left=V_OFF(newvert,voc->vocseg);
	  if ( newvert->left != 0 )
            ((vert  *)V_POINT(voc->vocseg,
			       newvert->left))->right=V_OFF(newvert,voc->vocseg);
        }
      else
	{   /* -- insert rightest brother -- */
	  newvert->left=V_OFF(currvert,voc->vocseg);
          newvert->right=newvert->down=0;
	  currvert->right=V_OFF(newvert,voc->vocseg);
        }
    }
  else
    {         /* -- go through level -- */
      while ( currvert->key > _2cap((*wrddef)->lt->code)
		  && currvert->left != 0 )
    currvert=(vert  *)V_POINT(voc->vocseg,currvert->left);

      if ( currvert->key < _2cap((*wrddef)->lt->code ))
	{      /* -- insert between vertexes -- */
	  newvert->right=currvert->right;
	  newvert->left=V_OFF(currvert,voc->vocseg);
          newvert->down=0;
	  currvert->right=V_OFF(newvert,voc->vocseg);
	  if ( newvert->right != 0 )
            ((vert  *)V_POINT(voc->vocseg,
			       newvert->right))->left=V_OFF(newvert,voc->vocseg);
        }
      else
	{     /* -- insert most left brother -- */
	  newvert->right=V_OFF(currvert,voc->vocseg);
          newvert->left=newvert->down=0;
	  currvert->left=V_OFF(newvert,voc->vocseg);
        }
    }
       /* -- init new vertex and change path & lev -- */
  newvert->key=_2cap((*wrddef)->lt->code);
  newvert->accnt=0;
  voc->lev++;
  voc->path[voc->lev]=V_OFF(newvert,voc->vocseg);
/*
Success :
 */
  return ( 0 );

Fail    : return( VOC_MEM_OVERFLOW );
}

/*************************************************************************/
/*   Function change account value of current vertex by dcount.          */
/*                                                                       */
/* input : voc_state * voc - structure of current state of vocabulary    */
/*         int16_t dcount      - delta value of accnt field                  */
/*                                                                       */
/* output: new value of account field in range 0:0x7F ( 7 bits, uint16_t)*/
/*   if new value will be more 0x7f, returned VOC_ACCNTOVERFLOW ( < 0 ). */
/*   if new value will be less 0, returned VOC_ACCNTZERO ( < 0 ).        */
/*************************************************************************/

static int16_t set_voc_accnt( voc_state * voc,int16_t dcount)
{
  int16_t buf;                 /*                                            */
  vert  * currvert;     /*                                            */

  currvert=(vert  *)V_POINT(voc->vocseg,voc->path[voc->lev]);
  buf=currvert->accnt;
  buf+=dcount;

  if ( buf >0x7F )
   { currvert -> accnt = 0x7f;
     return ( VOC_ACCNT_OVERFLOW );     /* field overflow: only 7 bits !  */
    }
  else if ( buf < 0 )
    { currvert->accnt = 0;
    return ( VOC_ACCNT_ZERO );         /* changed acnt <0: haven't sense */
    }
  else
    {
      currvert->accnt=/*(NATURAL)*/(uchar)buf;    /* set & return new value         */
      return (buf);
    }
}

/*************************************************************************/
/*   Function allocate vocabulary's memory for vertex with size of       */
/*   formal parameter 'size'.                                            */
/*                                                                       */
/* input : voc_state * voc - structure of current state of vocabulary    */
/*         int16_t size        - size of memory, which will be allocated     */
/*                                                                       */
/* output: offset from vocabulary base for new vertex.                   */
/*         if have not enought space, error message VOC_MEM_OVERFLOW (<0)*/
/*         will be returned.                                             */
/*                                                                       */
/* non direct : change pointer voc->vocfree to free space of vocabulary  */
/*  output      memory by value 'size'.                                  */
/*                                                                       */
/*************************************************************************/

static uint16_t voc_alloc (
	voc_state * voc,
        int16_t size
	)
{ uint16_t pointer;

  if ( voc->vocfree+size > MAX_VOC_SIZE ) return 0;
  else
    { pointer=voc->vocfree;
      voc->vocfree+=size;
      return ( pointer );
    }
}

/*************************************************************************/
/*    FILLGAP for dynamic vocabulary.   10.17.90                         */
/*************************************************************************/

static void chkfill (
      int16_t cnt, LTIMG * wrddef[],
      int16_t gapcont, int16_t *fillfind, uchar symb,
      LT  ** gaplt
    )
 {
   LT  * xxgaplt;

  if (gapcont==0) goto Gap_add;    /* fill gap from the beginning */

  for (xxgaplt=(LT  *)&wrddef[cnt]->std; xxgaplt!=*gaplt; xxgaplt++)
    {
      if (xxgaplt->code == symb)  break;
    }

  if (xxgaplt == *gaplt)      /* symb not found in the available list */
    {
Gap_add:  if ( *gaplt != (LT  *)&wrddef[cnt]->std )
	    {
	      (*gaplt-1) -> attr = 0;
	    }
	  (*gaplt) -> code=symb;
	  (*gaplt) -> attr=1;
	  (*gaplt)++;
    }
  *fillfind=1;

/*         -- word[] is not used in voc-system!*/
/*  if ( *fillfind == 0 )
    { word[cnt]=symb; *fillfind=1; }
 */
 }

static int16_t fillgap (
      vert  * currvert,
      voc_state * voc,
      int16_t cnt,
      LTIMG **wrddef,
      int16_t gapcont
    )
 {
   LT  * gaplt;
   int16_t fillfind=0;

  if (gapcont == 0)                /* fill gap from the beginning */
    gaplt = wrddef[cnt]->lt = (LT  *)&wrddef[cnt]->std;
  else {                          /* continue filling:           */
      gaplt=wrddef[cnt]->lt;
      while ( !(gaplt->attr & EOLIST) ) gaplt++;
      gaplt++;
    }

   while ( currvert->left != 0 )            /* go to level begin */
     currvert=(vert  *)V_POINT(voc->vocseg,currvert->left);

   do {
     chkfill( cnt,wrddef,gapcont,&fillfind,_2cap(currvert->key),&gaplt );
     if ( currvert->right == 0 )  break;
     currvert=(vert  *)V_POINT(voc->vocseg,currvert->right);
   } while ( 1);            /* steps through level */
   if ( fillfind == 0 ) goto Fail;
/*
Success:
*/
    wrddef[cnt]->blank=0;
   return(OK);

Fail:     if ( gapcont != 0 ) { wrddef[cnt]->blank=0;
				return(OK) ; }
	  else return(FAIL);
 }
/*************************************************************************/
 WTOKEN  * get_wtoken(voc_state *voc)
 {  return (WTOKEN  *)V_POINT(voc->vocseg,
 ((vert  *)V_POINT(voc->vocseg,voc->path[voc->lev]))->wtoken);
 }

 WTOKEN  * alloc_wtoken(voc_state *voc)
 {
  uint16_t wtoken,w;
  WTOKEN  *wp;
   wtoken=((vert  *)V_POINT(voc->vocseg,voc->path[voc->lev]))->wtoken;
   if(wtoken)
    { while(w=((WTOKEN *)(V_POINT(voc->vocseg,wtoken)))->next)
	 wtoken=w;
     if((((WTOKEN *)(V_POINT(voc->vocseg,wtoken)))->next=w=voc_alloc(voc,sizeof(WTOKEN)))
       ==0) return NULL;
     }
    else
     if(( ((vert  *)V_POINT(voc->vocseg,voc->path[voc->lev]))->wtoken
	=w=voc_alloc(voc,sizeof(WTOKEN)))==0) return NULL;

  wp=(WTOKEN  *)V_POINT(voc->vocseg,w);
  wp->next=0;
  return wp;
 }

/* ------------------------------------------------------------------ */

int16_t next_word_from_voc(uchar *str, voc_state *voc)
 { vert  * currvert;
  if ( voc->lev == -1 )
    currvert=(vert  *)V_POINT(voc->vocseg,sizeof(DYN_DICT_HEADER));
  else
    currvert=(vert  *)V_POINT(voc->vocseg,voc->path[voc->lev]);
do {
  if (currvert -> down)
    do
     { currvert = (vert  *)V_POINT(voc ->vocseg,currvert->down);
       while (currvert -> left)
          currvert = (vert  *)V_POINT(voc ->vocseg,currvert->left);
       voc -> lev ++;
       if(AcceptAndCheck(currvert, voc))
                                           return  Found(str, voc);
      } while (currvert -> down);
   if( currvert -> right)
    { currvert = (vert  *)V_POINT(voc ->vocseg,currvert->right);
      if(AcceptAndCheck(currvert, voc))
                                           return  Found(str, voc);
    }
   else
      { do
        {
         voc -> lev --;
         if ( voc-> lev ==-1)                  return -1;
         currvert = (vert  *)V_POINT(voc ->vocseg,voc -> path [voc -> lev]);
         } while ( currvert-> right == 0);
         currvert = (vert  *)V_POINT(voc ->vocseg, currvert -> right);
          if(AcceptAndCheck(currvert, voc))
                                               return  Found(str, voc);
      }
  } while (1);

 }

/* ------------------------------------------------------------------ */

int16_t AcceptAndCheck ( vert  *currvert, voc_state *voc)
 {
      voc -> path[voc->lev] =V_OFF(currvert, voc ->vocseg);
      return ( currvert -> wrdterm) ? 1 : 0;
 }

/* ------------------------------------------------------------------ */

int16_t Found ( uchar *str, voc_state *voc)
 {  int16_t i;
  for ( i = 0; i <= voc ->lev; i++)
    str[i] = ((vert *)V_POINT(voc->vocseg, voc -> path[i]))->key;
  str[i] = 0;
  return ((vert  *)V_POINT(voc -> vocseg, voc -> path[voc->lev])) -> accnt;
 }
/*************************************************************************/
/*                           end of file                                 */
/*************************************************************************/
