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

/*********************************************************************/
/*                                                                   */
/*                           module PERC                             */
/*                  the processing of the 'percent' symbol           */
/*********************************************************************/

/*
 This procedure inspects given cell and makes a decision whether it is
  the percent symbol or not. The object under consideration is 'left
  upper' percent or 'right lower' percent or right slash '/'.

 1. 'left upper' percent case:
     right dust exists, and the box of the dust has the correct
     overlap with the box of given cell. The overlap is correct if:
      a) dust right lower corner can stretch out of given cell by
      BH_K_NOM                             BW_K_NOM
      ---------- * height of the cell and  ----------- * width of the cell;
      BH_K_DENOM                           BW_K_DENOM

						    EW_K_NOM
      b) the width of the dust must be less, than  ---------- of the
						    EW_K_DENOM
	  width of the cell, and the height of the dust must be less,
		  EH_K_NOM
	  than   ----------  of the height of the cell.
		 EH_K_DENOM

  2. 'right lower' percent case:
     the left dust exists, and the box of the dust has the correct
     overlap with the box of given cell. The overlap is correct if:
      a). dust left upper corner can stretch out of given cell by
      BH_K_NOM                             BW_K_NOM
      ---------- * height of the cell and  ----------- * width of the cell;
      BH_K_DENOM                           BW_K_DENOM

						    EW_K_NOM
      b). the width of the dust must be less, than  ---------- of the
						    EW_K_DENOM
	  width of the cell, and the height of the dust must be less,
		  EH_K_NOM
	  than   ----------  of the height of the cell.
		 EH_K_DENOM

  3. 'right slash' case:
     the left and rights dusts exist, and the boxes of the dusts have
     the correct overlaps with the box of given cell. The correctness
     of the overlaps is the same as p.1. and p.2. for right dust
     and left dust respectevly.

*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "struct.h"
#include "func.h"
#include "ligas.h"
#include "lang.h"
#include "leo.h"
#include "std.h"

#include "compat_defs.h"

#define NUMBER      '#'
#define PRO_NUMBER  0xC3
#define PRO_NUM_PART '‹'

#define BW_K_NOM     1
#define BW_K_DENOM   2
#define BH_K_NOM     1
#define BH_K_DENOM   7
#define EW_K_NOM     4
#define EW_K_DENOM   5
#define EH_K_NOM     4   //for percent
#define EH_K_DENOM   5
#define EH_K_NOM_N   11   //for number
#define EH_K_DENOM_N 10
#define  H_K_DENOM_N  8   //for number

#define LeftDust     1
#define RightDust    2

#define RELIABLE_SL  220
#define RELIABLE_O   210
#define RELIABLE_NUM 200
#define LIKE_O       100
#define middle(x) ((x)->r_col + ((x)->w>>1))

extern uchar db_status;


typedef struct bnd_allowed
 {
  INT     bw,                 /* beginning of the widht interval */
		  ew,                 /* end of the widht interval */
		  bh,                 /* beginning of the height interval */
		  eh;                 /* end of the height interval */
 } bnd_allowed;

typedef struct  perc_struct
 {
  cell *          BC;           /* pointer to the currente cell */
  cell *          RC;           /* the first 'right dust' cells */
  cell *          ldcp;         /* pointer to the 'left dust' cell for percent
                                   or 'undeline cell' for number */
  cell *          rdcp;         /* pointer to the 'right dust' cell */
  uchar            FL;           /* existing 'good' dusts flag */
  INT             rcps;         /* the return code of the percent searching */
  INT             n_ver_perc;   /* the number of the found percent version */
  /* the allowed boundaries of the upper dust */
  INT     ubw,          /* beginning of the widht interval */
		  uew,          /* end of the widht interval */
		  ubh,          /* beginning of the height interval */
		  ueh;          /* end of the height interval */
  /* the allowed boundaries of the lower dust */
  INT     lbw,          /* beginning of the widht interval */
		  lew,          /* end of the widht interval */
		  lbh,          /* beginning of the height interval */
		  leh;          /* end of the height interval */
  INT     mw,           /* "middl" of the widht */
          mh;           /* "middl" of the height */
  INT maxh;         /* maximum of height */
  INT like_O;       /* tolerence for circle in number */
 } perc_struct;

/*
	This procedure inspects given cell and makes a desision
	whether it is the percent symbol or not.
*/
/********** Prototypes of the functions ******************/
static INT search_perc_vers();
static INT search_left_dust();
static INT search_right_dust(INT like_o);
static INT compare_bound(cell *,char);
static INT rec_O0(cell *);
static INT change_vers();
static void collect_cell(cell *mainc, cell *c2);
static cell *collect_cell_save(cell *mainc, cell *c2);
static void prc_setup();
static void proc_perc();
static void merge_ld();
static void merge_rd();
static void compute_bnd_lbox(cell *pc,bnd_allowed *bnd);
static void compute_bnd_ubox(cell *pc,bnd_allowed *bnd);
static INT  search_underline_dust();
static cell *rest_composition(cell *cmplx);
static INT test_O0(cell *c);
static void  improve_proN();

Bool test_cell(cell *c);
//void Leo_ContStoreC(cell *c);
/******* Global variable *********/
perc_struct *gpt;


void number( void ) // Andrew for compose number
{
perc_struct glstr;

 snap_newpass('h');

 gpt=&glstr;
 gpt->BC=cell_f();
 while (1)
  {
    gpt->BC=gpt->BC->nextl;
    if (!gpt->BC) break;
    if (!gpt->BC->next) break;   /* dummy cell at end reached  */

    if( language==LANG_CROATIAN )
      continue;

	// Отключаю так как код PRO_NUMBER=0xc3 конфликтует
	// с румынской буквой AA_semicircle. 31.05.2001 E.P.
    if( language==LANG_ROMAN)
      continue;

    if  ((gpt->rcps=search_perc_vers()) != (uchar)PRO_NUMBER &&    /* find the proNumber version */
          gpt->rcps != 'N')
     continue;

    if( db_status && snap_activity('h') ){
       snap_newcell(gpt->BC);
       snap_monitor();
    }

    if (gpt->rcps==(uchar)PRO_NUMBER && gpt->BC->vers[0].prob<RELIABLE_NUM)
      improve_proN();
    else
    if (gpt->rcps==(uchar)PRO_NUM_PART)
    {
      improve_proN();
      if (gpt->BC->vers[0].let != (uchar)PRO_NUMBER)
        continue;
    }

    prc_setup();
    proc_perc();

    if( gpt->BC->vers[0].let != (uchar)NUMBER && gpt->rcps != 'N'){
       gpt->BC->vers[gpt->n_ver_perc].prob = 0; // kill proNumber
       sort_vers(gpt->BC);
       gpt->BC->vers[gpt->BC->nvers].let = gpt->BC->nvers>0? 0:bad_char;
       gpt->BC->vers[gpt->BC->nvers].prob= 0; /* Set correct tail */
    }

  } /* end-while */

  return;
}/*number*/


void perc()
{
perc_struct glstr;

 snap_newpass('h');

 gpt=&glstr;
 gpt->BC=cell_f();
 while (1)
  {
    gpt->BC=gpt->BC->nextl;
    if (!gpt->BC) break;
    if (!gpt->BC->next) break;   /* dummy cell at end reached  */
    if  ((gpt->rcps=search_perc_vers())== 0)    /* find the percent version */
     continue;

    if( db_status && snap_activity('h') ){
       snap_newcell(gpt->BC);
       snap_monitor();
    }

    if (gpt->rcps==(uchar)PRO_NUMBER && gpt->BC->vers[0].prob<RELIABLE_NUM)
      improve_proN();
    else
    if (gpt->rcps==(uchar)PRO_NUM_PART)
    {
      improve_proN();
      if (gpt->BC->vers[0].let != (uchar)PRO_NUMBER)
        continue;
    }

    prc_setup();
    proc_perc();
  } /* end-while */
}

static void prc_setup()
{
cell *prd=gpt->BC;
INT midBC=middle(gpt->BC);

 while (!(prd->next->flg & c_f_fict) && middle(prd)<=midBC)
 {
   prd=prd->next;
 }
 gpt->RC=prd;
 gpt->ldcp = gpt->rdcp = NULL;
 gpt->FL=0;
 gpt->ubw=(gpt->rcps=='/') ?
   (midBC - gpt->BC->h*2/3) :
   (gpt->BC->r_col - (gpt->BC->w*BW_K_NOM)/BW_K_DENOM);
 gpt->ubh=(gpt->BC->r_row - (gpt->BC->h*BH_K_NOM)/BH_K_DENOM);
 gpt->uew=(gpt->BC->r_col + (gpt->BC->w*EW_K_NOM)/EW_K_DENOM);
 gpt->ueh=(gpt->BC->r_row + (gpt->BC->h*EH_K_NOM)/EH_K_DENOM);

 gpt->lbw=(gpt->BC->r_col + (gpt->BC->w*(EW_K_DENOM-EW_K_NOM))/EW_K_DENOM);
 gpt->lbh=(gpt->rcps=='N') ?
   (gpt->BC->r_row) :
   ((gpt->rcps==(uchar)PRO_NUMBER) ?
    (gpt->BC->r_row + (gpt->BC->h*(EH_K_DENOM_N-EH_K_NOM_N))/EH_K_DENOM_N) :
    (gpt->BC->r_row + (gpt->BC->h*(EH_K_DENOM-EH_K_NOM))/EH_K_DENOM)
   );
 gpt->lew=(gpt->rcps=='/') ?
   (midBC + gpt->BC->h*2/3) :
   ((gpt->rcps=='N') ?
    (gpt->BC->r_col + gpt->BC->w*2) :
    (gpt->BC->r_col + gpt->BC->w*(BW_K_DENOM+BW_K_NOM)/BW_K_DENOM)
   );
 gpt->leh=(gpt->BC->r_row + gpt->BC->h*(BH_K_DENOM+BH_K_NOM)/BH_K_DENOM);

 gpt->maxh=gpt->BC->h-(gpt->BC->h+H_K_DENOM_N/2)/H_K_DENOM_N;

 gpt->mw = gpt->BC->r_col + ((gpt->BC->w)/3);
 gpt->mh = gpt->BC->r_row + ((gpt->BC->h)>>1);
 gpt->like_O = LIKE_O;
} /*------------------------------------------------------------------*/

static void proc_perc()

{
INT rcslds;                /* the return code of the left dust searching */
INT rcsrds;                /* the return code of the right dust searching */
uchar p;
 switch (gpt->rcps)
  {
   case  '/':
      rcslds=search_left_dust();
      rcsrds=search_right_dust(gpt->like_O);
//      if ( gpt->ldcp == gpt->rdcp )
//        break;
      if ( (rcslds) && (rcsrds)&& gpt->BC->vers[gpt->n_ver_perc].prob>=RELIABLE_SL ||
           rcslds>=RELIABLE_O && gpt->FL & RightDust ||
           rcsrds>=RELIABLE_O && gpt->FL & LeftDust
         )
        goto compose;
      break;
   case  liga_uperc:
      rcsrds=search_right_dust(gpt->like_O);
      if (rcsrds == 0) break;
      goto compose;
   case  liga_lperc:
      rcslds=search_left_dust();
      if (rcslds == 0) break;
      goto compose;
   case  (uchar)PRO_NUMBER:
      rcslds=search_underline_dust();
      rcsrds=search_right_dust(gpt->like_O);
      p=gpt->BC->vers[gpt->n_ver_perc].prob;
      if (p>=RELIABLE_NUM && rcsrds ||
          rcsrds>=RELIABLE_O && rcslds &&
          gpt->ldcp->r_row > gpt->rdcp->r_row+gpt->rdcp->h)
        goto compose;
      break;
   case  'N':
      rcslds=search_underline_dust();
      rcsrds=search_right_dust(gpt->like_O);
      p=gpt->BC->vers[gpt->n_ver_perc].prob;
      if (p>=RELIABLE_NUM && rcsrds && rcslds ||
          rcsrds>=RELIABLE_O && rcslds &&
          gpt->ldcp->r_row > gpt->rdcp->r_row+gpt->rdcp->h)
        goto compose;
      break;
  } /* end-switch */

//not recognised
  if (gpt->ldcp)  rest_composition(gpt->ldcp);
  if (gpt->rdcp)  rest_composition(gpt->rdcp);

  return;

compose:
  merge_ld();
  merge_rd();
  change_vers();
//  Leo_ContStoreC(gpt->BC);
  return;
} /*------------------------------------------------------------------*/

static INT search_perc_vers()
{   /* searching the percent-symbol version */

INT i,rc;
uchar l=gpt->BC->vers[0].let;
 if (language==LANG_RUSSIAN)
   if (l==(uchar)'«' || l==(uchar)'‹')  return PRO_NUM_PART;

 for (i=0,rc=0; i<=gpt->BC->nvers; i++)
  {
    if ( gpt->BC->vers[i].let == '/' )
     {
      rc='/';
      gpt->n_ver_perc=i;
      break;
     }
    if ( gpt->BC->vers[i].let == liga_uperc )
     {
      rc=liga_uperc;
      gpt->n_ver_perc=i;
      break;
     }
    if ( gpt->BC->vers[i].let == liga_lperc )
     {
      rc=liga_lperc;
      gpt->n_ver_perc=i;
      break;
     }
    if ( gpt->BC->vers[i].let == (uchar)PRO_NUMBER )
     {
      rc=(uchar)PRO_NUMBER;
      gpt->n_ver_perc=i;
      break;
     }
    if ( gpt->BC->vers[i].let == 'N' )
     {
      rc='N';
      gpt->n_ver_perc=i;
     }
  } /* end-for */
 return(rc);
} /*------------------------------------------------------------------*/

static INT search_left_dust()
{
cell *pld;
INT rv=0;

 pld=gpt->RC->prev;
 while (pld->prev && pld->r_col>=gpt->ubw)
 {
   if (pld != gpt->BC && compare_bound(pld,'L'))
   {
     pld->complist=NULL;           //single
     gpt->FL=gpt->FL | LeftDust;
     if ((rv=test_O0(pld))>LIKE_O)
     {
       if (gpt->ldcp)  rest_composition(gpt->ldcp);
       gpt->ldcp=pld;
       break;
     }
     if (gpt->ldcp)
     {
       pld=collect_cell_save(gpt->ldcp,pld);
       if ((rv=test_O0(pld))>LIKE_O)  break;
     }
     else
       gpt->ldcp=pld;
   }
   pld=pld->prev;
 }
 return rv;
} /*------------------------------------------------------------------*/

static INT search_right_dust(INT like_o)
{
cell *prd;
INT rv=0,po=0;

 prd=gpt->RC;
 while (prd->next && prd->r_col<=gpt->lew)
 {
   if (prd != gpt->BC && prd != gpt->ldcp && compare_bound(prd,'R'))
   {
     if (prd->h > gpt->maxh/2 && prd->w > (gpt->lew-gpt->lbw)/5)
       rv=1;
     prd->complist=NULL;           //single
     gpt->FL=gpt->FL | RightDust;
     if ((po=test_O0(prd))>like_o)
     {
       if (gpt->rdcp)  rest_composition(gpt->rdcp);
       gpt->rdcp=prd;
       break;
     }
     if (gpt->rdcp)
     {
       prd=collect_cell_save(gpt->rdcp,prd);
       if ((po=test_O0(prd))>like_o)  break;
     }
     else
       gpt->rdcp=prd;
   }
   prd=prd->next;
 }
 return MAX(rv,po);
} /*------------------------------------------------------------------*/

static INT change_vers()
{
 if ( gpt->rcps == (uchar)PRO_NUMBER || gpt->rcps == 'N' ){
    gpt->BC->vers[0].let = NUMBER;   /* first version is number */
 }
 else{
    gpt->BC->vers[0].let = '%';   /* first version is '%' */
 }
 gpt->BC->dens = 255; // undef for calc BOLD
 gpt->BC->vers[0].prob=254;
 gpt->BC->nvers=1;   /* the number of versions is 1 */
 gpt->BC->vers[1].let=0; /* end of list */
 if (gpt->BC->flg & c_f_bad)  gpt->BC->flg ^= c_f_bad | c_f_let;

 return  1;

} /*------------------------------------------------------------------*/

static void merge_ld()
{

 if (gpt->ldcp)  collect_cell(gpt->BC, gpt->ldcp);
// del_cell(gpt->ldcp);
} /*------------------------------------------------------------------*/

static void merge_rd()
{

 if (gpt->rdcp)  collect_cell(gpt->BC, gpt->rdcp);
 // del_cell(gpt->rdcp);
} /*------------------------------------------------------------------*/

static void collect_cell(cell *mainc, cell *c2)
{
cell * coll[3];
coll[0]=mainc;
coll[1]=c2;
compose_cell (2,coll,mainc);
snap_newcell(mainc);
} /*------------------------------------------------------------------*/

static cell *collect_cell_save(cell *mainc, cell *c2)
{
  cell * coll[3];
  if (!mainc->complist)  // store single cell
  {                      // complex cell is stored by components
    cell *nc=new_cell();
    memcpy(nc,mainc,sizeof(cell));
    insert_cell(nc,mainc);
    mainc->complist=(c_comp *)del_save_cell(nc);
  }
  coll[0]=mainc;
  coll[1]=c2;
  compose_cell_save (2,coll,mainc);
  c2->complist=mainc->complist;
  mainc->complist=(c_comp *)del_save_cell(c2);
  snap_newcell(mainc);
  return mainc;
} /*------------------------------------------------------------------*/

static INT compare_bound(cell *pc, char rorl)
{
bnd_allowed ldbnd;

 if (rorl=='R')
  {                      /* right (lower) 'dust' box case */
   compute_bnd_lbox(pc,&ldbnd);
   if (
       ( (ldbnd.bw >= gpt->lbw) && (ldbnd.bw >= gpt->mw) ) &&
       (ldbnd.bh >= gpt->lbh) &&
       (ldbnd.ew <= gpt->lew) &&
       (ldbnd.eh <= gpt->leh) &&
       (pc->h <= gpt->maxh)
      )      return(1);
   else      return(0);
  }
 if (rorl=='L')
  {                    /* left (upper) 'dust' box case */
   compute_bnd_ubox(pc,&ldbnd);
   if (
       (ldbnd.bw >= gpt->ubw) &&
       ( (ldbnd.bh >= gpt->ubh) && (ldbnd.bh <= gpt->mh) ) &&
       (ldbnd.ew <= gpt->uew) &&
       (ldbnd.eh <= gpt->ueh) &&
       (pc->h <= gpt->maxh)
      )    return(1);
   else    return(0);
  }
 return(0);
} /*------------------------------------------------------------------*/

static void compute_bnd_ubox(cell *pc,bnd_allowed *bnd)
{
  bnd->bw=(pc->r_col);
  bnd->bh=(pc->r_row);
  bnd->ew=(pc->r_col+pc->w);
  bnd->eh=(pc->r_row+pc->h);
} /*------------------------------------------------------------------*/

static void compute_bnd_lbox(cell *pc,bnd_allowed *bnd)
{
  bnd->bw=(pc->r_col);
  bnd->bh=(pc->r_row);
  bnd->ew=(pc->r_col+pc->w);
  bnd->eh=(pc->r_row+pc->h);
} /*------------------------------------------------------------------*/

static INT test_O0(cell *pc)
{
uchar rc=0;
uint16_t save_c_flg;
INT  saveN;
uchar saveV[VERS_IN_CELL*sizeof(version)];
version *v;


  save_c_flg=pc->flg;         /* save flag of the cell */
  saveN = pc->nvers;
  if( saveN>0 )
    memcpy(saveV,pc->vers,VERS_IN_CELL*sizeof(version));
 /* the flag storing is necessary because of short_recog_cell may change
    this flag but doas not correct the lists of the cells  */
  memset(pc->vers,0,4*sizeof(version));
  pc->nvers=3;
  pc->vers[0].let='0';   pc->vers[1].let=(uchar)'Ћ';  pc->vers[2].let='O';
  if (test_cell(pc))
    for (v=pc->vers; v->let; v++)
      rc=MAX(rc,v->prob);

    if( db_status && snap_activity('h') ){
       pc->vers[0].prob=rc;
       snap_newcell(pc);
       snap_monitor();
    }

  pc->flg=save_c_flg;
  pc->nvers = saveN ;
  if( saveN>0 )
    memcpy(pc->vers,saveV,VERS_IN_CELL*sizeof(version));
  else
    set_bad_cell(pc);
  return(rc);
}
//------------------------------------------------

static INT rec_O0(cell *pc)
{
INT wi,rc;
uint16_t save_c_flg;
INT  saveN;
uchar saveV[VERS_IN_CELL*sizeof(version)];


  save_c_flg=pc->flg;         /* save flag of the cell */
  saveN = pc->nvers;
  if( saveN>0 )
    memcpy(saveV,pc->vers,VERS_IN_CELL*sizeof(version));
 /* the flag storing is necessary because of short_recog_cell may change
    this flag but doas not correct the lists of the cells  */
  short_recog_cell(pc);
  for (wi=0; wi < pc->nvers; wi++)
   {
    if (
	(pc->vers[wi].let == 'O') ||
    (pc->vers[wi].let == 'o') ||
        (language==LANG_RUSSIAN && ((pc->vers[wi].let == (uchar)'Ћ')  ||
                                    (pc->vers[wi].let == (uchar)'®')))||
  (pc->vers[wi].let == '0')
       )
       {  rc=pc->vers[wi].prob;goto Rexit;}
   }  /* end-for */
  rc=0;
Rexit:
    if( db_status && snap_activity('h') ){
       snap_newcell(pc);
       snap_monitor();
    }

  pc->flg=save_c_flg;
  pc->nvers = saveN ;
  if( saveN>0 )
    memcpy(pc->vers,saveV,VERS_IN_CELL*sizeof(version));
  return(rc);
} /*------------------------------------------------------------------*/

static INT search_underline_dust()
{
cell *prd=gpt->RC;
Bool weak=FALSE;

 while (prd->next && prd->r_col<=gpt->lew)
 {
   if (prd != gpt->BC && prd->w<<1 > prd->h*3 && prd->w > gpt->BC->w>>2 &&
       compare_bound(prd,'R'))
   {
     gpt->ldcp=prd;
     prd->complist=NULL;           //single
     if (prd->w > prd->h<<1)
       return 1;
     else
       weak=TRUE;
   }
   prd=prd->next;
 }
 if (weak)  gpt->like_O=0;
 return 0;
}

static cell *rest_composition(cell *cmplx)
{
  cell *c=(cell *)cmplx->complist,*cl=c;

  if (c==NULL)  return cl;   // cmplx is single
  do
  {
    cl=rest_cell(c,cmplx);
    c=(cell *)cl->complist;
  }
  while (c);
  del_cell(cmplx);
  return cl;
}

static void  improve_proN()
{
  cell *BC=gpt->BC,*cl=BC->prev,*cr=BC->next;
  INT p=BC->vers[gpt->n_ver_perc].prob;
  INT w3=(5*BC->w+6)/12,h8=BC->h/8;
  INT bot=BC->r_row+BC->h,right=BC->r_col+BC->w;

  BC->complist=NULL;   //single
  while (cl->flg & c_f_dust &&
         cl->r_col>=BC->r_col-w3 && cl->r_row>BC->r_row && cl->r_row+cl->h<bot+h8)
  {
    BC=collect_cell_save(BC,cl);
    cl=BC->prev;
  }

  while (cr->next && middle(cr)<=right)  cr=cr->next;
  while (cr->flg & c_f_dust &&
         cr->r_col+cr->w<=right+w3 &&
         cr->r_row>BC->r_row-h8 && cr->r_row+cr->h<bot)
  {
    BC=collect_cell_save(BC,cr);
    cr=BC->next;
  }

  if (BC->complist != NULL)
  {
    test_cell(BC);
    if (BC->vers[0].let != (uchar)PRO_NUMBER || BC->vers[0].prob<=p)
      BC=rest_composition(BC);
    else
    {
      gpt->rcps = PRO_NUMBER;  gpt->n_ver_perc=0;
    }
    gpt->BC=BC;
  }
}

static RecRaster workRaster;

/* swapbytes takes a 32 bit value and does an endianness change.
 * Since it was only used on Windows (not Mac) I'm assuming it means
 * "swap 32 bytes between big endian and current byte order".
 */
#if defined(WIN32) && defined(_MSC_VER) && (_MSC_VER > 800)
  #define   swapbytes(a) __asm {                      \
                                 __asm   mov   EAX,a  \
                                 __asm   bswap EAX    \
                                 __asm   mov   a,EAX  \
                               }
#elif defined(__GNUC__) /* FIXME: also check that we are on x86. And little-endian. */
    #define swapbytes(a) asm ("bswap %0;" :"=r"(a) :"0"(a));
#else
  #error You must define swapbytes for your platform
#endif

Bool makeRasterC( c_comp* comp, RecRaster** rast )
{
   int          i,cInter,beg,end,beg31,end31,beg32,end32,width32;
   uint32_t       tmp;
   uint32_t*      rast32;
   lnhead*   plnhead;
   interval* inter;

   memset(&workRaster.Raster[0],0,REC_MAX_RASTER_SIZE);
   workRaster.lnPixWidth  = comp->w;
   workRaster.lnPixHeight = comp->h;
   workRaster.lnRasterBufSize = REC_MAX_RASTER_SIZE;
   width32 = REC_GW_WORD32(workRaster.lnPixWidth);
   rast32 = (uint32_t*)&workRaster.Raster[0];

//   assert(4*width32*workRaster.lnPixHeight <= REC_MAX_RASTER_SIZE);  Paul 10-6-97
   if (4*width32*workRaster.lnPixHeight > REC_MAX_RASTER_SIZE)
	   return FALSE;

   plnhead = (lnhead*)((char*)comp + sizeof(c_comp) + sizeof(uint16_t));
   while( plnhead->lth ){
      cInter = plnhead->row;
      inter = (interval*)(plnhead+1);
      while( inter->l ){
         beg = inter->e - inter->l;
         end = inter->e - 1;
         beg32 = cInter*width32 + beg/32;
         end32 = cInter*width32 + end/32;
         beg31 = beg & 31;
         end31 = end & 31;

         for(i = beg32+1; i < end32; i++)
            rast32[i] = 0xFFFFFFFF;
         if( beg32 == end32 ){
            tmp = ( (uint32_t)0xFFFFFFFF >> beg31 ) &
                  ( (uint32_t)0xFFFFFFFF << (31-end31) );
            swapbytes(tmp);
            rast32[beg32] |= tmp;
         }
         else{
            tmp = (uint32_t)0xFFFFFFFF >> beg31;
            swapbytes(tmp);
            rast32[beg32] |= tmp;
            tmp = (uint32_t)0xFFFFFFFF << (31-end31);
            swapbytes(tmp);
            rast32[end32] |= tmp;
         }

         inter++;
         cInter++;
      }
      plnhead = (lnhead*)((char*)plnhead+plnhead->lth);
   }

   *rast = &workRaster;
   return TRUE;
}//makeRasterC

Bool test_cell(cell *c)
{
#ifdef _USE_LEO_

  RecRaster*     rst=NULL;
  RecVersions*   result;
  RecObject   recobj;
  RecAlt* alt;
  version *v;
  c_comp*  comp = c->env;
  INT i;

   // Oleg : 26-07-1995 : TCell without env can't recog
  if( !comp )
    return FALSE;

  memset(&recobj,0,sizeof(recobj));
  if (!makeRasterC(comp,&rst))
    return FALSE;
  memcpy(&recobj.recData.recRaster,rst,sizeof(RecRaster));
  recobj.recData.lwCompCnt = 1;
  if (c->broken_II)  recobj.recData.lwStatus |= REC_STATUS_BROKEN_II;

  {
    Rect16 r;
    r.left  = comp->left -1;//  + 8*(fieldRect.left/8);
    r.top   = comp->upper-1;//  + fieldRect.top ;
    r.right = comp->left  + comp->w;
    r.bottom= comp->upper + comp->h;
    recobj.recData.rect = r;
  }
  result = &recobj.recResults;
  result->lnAltCnt=c->nvers;
  assert(REC_MAX_VERS==VERS_IN_CELL);
  for (i=0,alt=result->Alt,v=c->vers; i<c->nvers; i++,alt++,v++)
    alt->Code=stdAsciiToAnsi(v->let);
//  memcpy(result->Alt,c->vers,sizeof(version)*c->nvers);
  if (!LEORecogCharPRN_expert(&recobj))  return FALSE;
  c->nvers=(INT)result->lnAltCnt;
  for (i=0,alt=result->Alt,v=c->vers; i<c->nvers; i++,alt++,v++)
  {
    v->let=stdAnsiToAscii(alt->Code);
    v->prob=alt->Prob;
  }
  v->let=v->prob=0;
//  memcpy(c->vers,result->Alt,sizeof(version)*c->nvers);
return TRUE;
#else
return FALSE;
#endif

}
