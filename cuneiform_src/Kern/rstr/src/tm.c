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


/****************************************************************************
 *                                                                          *
 *              Recognition for (TM), (C), (R)                              *
 *                                                                          *
 *              Written in 1995 by ...                                      *
 *                                                                          *
 ***************************************************************************/

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "struct.h"
#include "func.h"
#include "status.h"
#include "msgerr.h"
#include "ligas.h"
#include "lang_def.h"
#include "tm_int.h"
#include "linutil.h"

void   make_all_cuts( void);
void   make_all_glues(void);

# define TM_SNAP_PASS 'h'

Bool TM_check_active=0;

// OLEG
static void clear_up_spec(void)
{
cell *  c;
B_LINES bas;


for(c=cell_f()->next;c!=cell_l();c=c->next)
    {
    if( c->prev && c->nvers && c->vers[0].let==liga_TM )
        {
        get_b_lines(c,&bas);
        if( bas.b1-c->row-c->h>c->h/4 )
            {
            c->flg_new |= c_fn_temp;
            c->vers[0].prob=100;
            }
            //c=del_cell(c);
        }
    }
return;
}

Bool  tradeCR( cell *c )
{
	 cell  *nextc,*clist[2];
	 uchar  snap[80],*s=snap,save_flg,save_language;
   uchar  saveN,saveV[VERS_IN_CELL*sizeof(version)];

   int16_t   i,tm=0;

   if( !(c->flg & c_f_let) )
     return  FALSE;
   if( !c->nvers )
     return  FALSE;

//simpleBOX(c,128); only  for  tmp  debug
   nextc = c->next;
   save_flg = (uchar)nextc->flg;
   if( save_flg & (c_f_dust|c_f_space) ){
//   if( (save_flg & c_f_space) ||
//       ( (save_flg & c_f_dust) && (nextc->complist[0].size==1) )
//     ){
      nextc = c->nextl;
      save_flg = (uchar)nextc->flg;
   }
   if( save_flg & c_f_fict )
     return  FALSE;

   save_language = language;

   if( nextc->col - c->col <= c->w/2 && nextc->row - c->row <= c->h/2 ){
      // Oleg : 26-07-1995 : cell without env can't recog
      if( c->env && nextc->env )
      if( c->vers[0].let == (uchar)'o' || c->vers[0].let == (uchar)'O' || c->vers[0].let == (uchar)'0' ||

          c->vers[0].let == (uchar)'®' &&
			!is_russian_turkish_conflict(c->vers[0].let)	// 21.05.2002 E.P.

		  ||
		  c->vers[0].let == (uchar)'Ћ' || c->vers[0].let == (uchar)'Q'
		){
         saveN = (uchar)nextc->nvers;
         memcpy(saveV,nextc->vers,VERS_IN_CELL*sizeof(version));
         language = LANG_ENGLISH;
         short_recog_cell(nextc);
         if( db_status && snap_activity(TM_SNAP_PASS) ){
            snap_newcell(c);
            snap_monitor();
            s += sprintf(s,"OCR : nvers = %d : ",nextc->nvers);
            for(i=0;i<nextc->nvers;i++)
              s += sprintf(s,"%c(%d) ",nextc->vers[i].let,nextc->vers[i].prob);
            snap_show_text(snap);
            snap_newcell(nextc);
            snap_monitor();
         }

         if( nextc->nvers ){

            for(i=0;i<nextc->nvers;i++){
               switch( nextc->vers[i].let ){
               case  'c':
			   case  'C':
               case  (uchar)'б': // Русская "с" - конфликтный код для балтики 17.07.2001
			   case  (uchar)'‘':
				    // Конфликтный код
					if (!is_russian_baltic_conflict(nextc->vers[i].let))	// 17.07.2001 E.P.
						tm = liga_CC;
               break;
               case  'R':
                  tm = liga_CR;
               break;
               }
            }
            if( db_status && snap_activity(TM_SNAP_PASS) ){
               s += sprintf(s,"TM is  found : nvers = %d : ",c->nvers);
               for(i=0;i<c->nvers;i++)
                 s += sprintf(s,"%c(%d) ",c->vers[i].let,c->vers[i].prob);
               snap_show_text(snap);
               snap_newcell(c);
               snap_monitor();
            }
            if( tm ){
               nextc->flg = save_flg;
               clist[0]=c;
               clist[1]=nextc;
               compose_cell(2,clist,c);
               c->nvers = 1;
               c->vers[0].let = (uchar)tm;
               c->vers[0].prob = 254;
            }
         }
         if( !tm ){
            nextc->nvers = saveN;
            memcpy(nextc->vers,saveV,VERS_IN_CELL*sizeof(version));
            nextc->flg = save_flg;
         }
      }
   }

   language = save_language;

   if( tm )
     return  TRUE;
   else
     return  FALSE;
}/*tradeCR*/


void  trade_marks( void )
{
   cell *c;

   if (db_status)
     snap_newpass(TM_SNAP_PASS);

   for (c = cell_f(); c != cell_l(); c = c -> next){
      if(tradeCR(c))
        continue;
   }

   return;
}/*trade_marks*/


extern int16_t nIncline;
int16_t is_square(cell *a)
{
 lnhead *Lp1, *Lp2;
 interval *int1;
 c_comp *cp1;

 int16_t  intbeg, intend, intlth, flsq, dens;
 int16_t  addinc, addinc8, cellh, cellw, cellwa8, cellha8,
      Ln, lc1, lineh, hcur, hcur8, hrest, hrest8, hint;
 int32_t wrkl, cellsq, sumlth;
 uchar was_here;

 was_here=0;
 flsq = 1;   // assume square
 if (((cellw=a->w)*2 > (cellh=a->h)*3) ||
     (cellw*3 < cellh*2)
    )
   flsq = 0;
 wrkl = cellh;
 addinc = (wrkl * nIncline) / 2048;
 addinc8 = 8*addinc;
 cellwa8 = cellw + addinc8;
 cellha8 = cellh + addinc8;
 cellsq = cellw*cellh;
 sumlth=0;

 cp1 = a->env;
 Ln=cp1->nl;        // number of lines in component
 Lp1=(lnhead *) ( (char *)cp1 + cp1->lines + 2); // beginning of first line
 Lp2=Lp1;
 for (lc1=0; lc1 < Ln; lc1++)
 {
   lineh=Lp1->h;
   int1=(interval *)(Lp1+1);     // ptr to first interval
   for ( hint = 0, hcur = Lp1->row, hrest = cellh-hcur;
         hint < lineh;
         int1++, hcur++, hint++, hrest--)
   {
     intend=int1->e;
     intlth=int1->l;
     sumlth += intlth;
     intbeg=intend-intlth;
     if (hint==0)   // 1st interval
     {
       if (lineh==1)
         continue;
     }
     hcur8  = hcur * 8;
     hrest8 = hrest * 8;
     if ( ( hcur8  > cellha8)  && ( hrest8 > cellha8) )  // middle part
     {
       if (intlth * 8 > cellw)     // must be thin
         { flsq = 0; continue; }
       // must be and at left/right side
       if ( (8 * intend)         < cellwa8)  // ends not far at left side
       { was_here |= 4;  continue; }
       if ( (8 * (cellw-intbeg)) < cellwa8)  // begins far at right side
       { was_here |= 8;  continue; }
       flsq = 0;  // thin in the middle
       continue;
     }
     // else: upper or lower part
     if (intlth * 8 < cellw) // thin interval -- OK  (uppermost, lowermost "dustec")
       continue;
     // now: thick intervals
     // thick intervals are allowed at top or at bottom
     if (hcur8 <= cellha8)  // interval is at top
     {
       was_here |= 1;
       continue;
     }
     if (hrest8 <= cellha8)  // interval is at bottom
     {
       was_here |= 2;
       continue;
     }
     flsq = 0;  // thick in middle
   } // intervals
   Lp2=Lp1=(lnhead *) ((char *)Lp2+Lp2->lth);   // next line
 } // all lines
 switch (was_here)
 {
   case 0:
   case 1:
   case 2:
   case 4:
   case 8:
   case 3:
   case 5:
   case 6:
   case 9:
   case 10:
   case 12:
   default:  flsq = 0;
   case 7:
   case 11:
   case 13:
   case 14:
   case 15:  break;
 }
//if( !cellsq ) // Oleg
//    return 0;
 dens = (int16_t)((sumlth*255) / cellsq);
 a->dens = dens/8;  // for calc BOLD
 if (dens > 127)     // too dens - not a sqare
   flsq = 0;
 if( language == LANG_RUSSIAN )   // by Oleg: 24/10/97 && a->nvers>0 && MEMCHR("Ї",a->vers[0].let,1) )
   flsq = 0; // Oleg : 28-03-1995 : russian small "pe" siamilar square
 return flsq;
}


int16_t del_squares()
{ int16_t del_tot;
  cell *BC;
  del_tot = 0;
  for (BC = cell_f (); BC != cell_l(); BC = BC -> next)
    if ((BC->flg & (c_f_bad + c_f_let)) && is_square(BC))
    {
			del_tot++;
			BC=del_cell(BC);
		}
	return del_tot;
}


#ifdef NEW_TM
#include "linear.h"
#include "lang.h"

#define FIRST_AVAILABLE_TM_POSITION 2
     // TM may not be placed before this position (in TEXT string)
#define GREATER_MULTIPLIER 2/3
#define min_TM_comp_h 7   // see create_new_bas()
#define min_TM_comp_w 2   // see create_new_bas()
#define min_TM_Ps     (min_TM_comp_h-2)   // bbs3-bbs2  see create_new_bas()
#define NOT_SMALLEST_SIZE  12  // min Ps value for which produce make_all_glues()
#define min_TM_box_w  12  // min allowed width of the whole TM box

#define TM_DUST_W     4  //9    // If cell is less then TM_DUST_WxTM_DUST_H
#define TM_DUST_H     min_TM_comp_h  //9    // it is supposed to be dust.
/* 18.4.95
#define TM_DUST_MIN   3
#define TM_DUST_MAX   10
*/
#define MIN_SUSPECTED_NUM 1 // 1 for (R), one-digit index, asterix, etc.
			    // otherwise 2
#define MAX_M_IN_T    2   // T may be twice greater in width then M
#define MAX_TM_CONFID 254
#define BAD_TM_CONFID 126
#define BOTTOM_EXP    (MAX_TM_PROB+2)
#define CR128         128   // for (R)
#define DIGITS64       64   // for upper digits
#define MIN_DIGIT_PROB 127  // for upper digits
#define MAXINT 32000
#define c_f_symb (c_f_let | c_f_bad )   // | c_f_punct)

void TM_control (void)
{
 cell *c, *last_cell;
 B_LINES bas;
 int16_t cells, TM_yes;

 last_cell = cell_l();
 c = cell_f();
 TM_yes=0;
 while ( c!=last_cell && (c=c->next)!=last_cell)
		{
		if (!(cells = check_TM (&c)))
	        continue;
		get_b_lines(c,&bas);
        TM_check_active=1;
		try_TM (&c, cells);
        TM_check_active=0;
        TM_yes=1;
		}
if( TM_yes )
    clear_up_spec();
return;
}

static int16_t check_TM (cell **first_cell)
{
	int16_t bottom,
	greater,  // bottom of TM should be less then bbs2+greater
	cell_num=0,  // number of letters before suspected
	suspected_num,
	TM_left, i, j,
	width_of_whole_TM_box;
  cell *c, *c1, *suspect, *first_suspect, *last_at_all;
  cell *predcessor, *successor;

  greater = (bbs3-bbs2)*GREATER_MULTIPLIER;
  bottom = bbs2+greater+minrow;
  last_at_all=cell_l();
  for (c = *first_cell; c != cell_l(); c = c->next)
  {
  //	if (c->row+c->h < bottom && c->flg!=c_f_space && c->flg!=c_f_fict)
	if (c->row+c->h < bottom && (c->flg&c_f_space)!=c_f_space && (c->flg&c_f_fict)!=c_f_fict)
		 {
		for (c1=cell_f() /* *first_cell*/; c1 != c; c1 = c1->next)
		    {
					if (c1->flg & c_f_symb)
				cell_num++;
		      if (cell_num>=FIRST_AVAILABLE_TM_POSITION)
			 break;
		    }
		if (cell_num<FIRST_AVAILABLE_TM_POSITION)
		     continue;

		// correct bases by previous cell  <- HAS A PROOF REASON?
		//
		//
		//
		first_suspect=suspect=c;
		suspected_num=0;

		while (  suspect !=last_at_all  // fictive last cell will be outer list
			 &&
			 (suspect->row+suspect->h < bottom ||
	suspect->h <=TM_DUST_H )  // dust (under TM base)
		      )
					{
			suspected_num++;
			suspect=suspect->next;
		      }
		// Come back excluding the rest DUST cells
		// which are below bottom
		suspect=suspect->prev;
		while ( suspect->row+suspect->h > bottom )  // dust under TM base
		      {
			suspect=suspect->prev;
			suspected_num--;
		      }

	// get the letter-predcessor
	c1=first_suspect->prev;
	while (!(c1->flg && c_f_symb))
	       c1=c1->prev;
	predcessor=c1;

	// get the letter-successor
	successor=suspect->next;
	while (successor!=cell_l() && !(successor->flg && c_f_symb))
	       successor=successor->next;
	if (successor==cell_l())
	    successor=NULL;

		TM_left=first_suspect->col;
		width_of_whole_TM_box=0; i=0; suspect=first_suspect;
		while (i++<suspected_num)
					{
			if ( (j= suspect->col+suspect->w-TM_left)
			       > width_of_whole_TM_box)
				 width_of_whole_TM_box=j;
			suspect=suspect->next;
					}

	// Check vertical projection of supposed TM-group for intersection
	// with predcessor or successor
	if (width_of_whole_TM_box>min_TM_box_w
	    && suspected_num>=MIN_SUSPECTED_NUM)
		   {
			*first_cell = first_suspect;  // first suspected cell
			if (badvertProjectionTM
			       (predcessor,successor, TM_left, (int16_t)(TM_left+width_of_whole_TM_box)))
			   suspected_num=-suspected_num;
			return suspected_num;
		   }
	   }
  }
  /* Otherwise - no suspected cells */
  *first_cell = c;
  return 0;
}

static int16_t badvertProjectionTM (cell *predcessor, cell *successor,
			      int16_t TM_left, int16_t TM_right)
{
  int16_t  left, right;

  left=predcessor->col;
  right=left+predcessor->w;
  if (
       (TM_left-left > TM_right-right)
       ||        // to exclude "russian-yo"-similar cases
       (successor && ((TM_right-successor->col)<<1) > successor->col-TM_left)
		// TM should not too (1/3) intersect with successor
     )  return 1;
  return 0;
}



static int16_t create_new_bas (cell *first, int16_t cmps)
{
  cell *c;
  int16_t i=0,
      no_bases_set=1;  // No base is set
  int16_t top=MAXINT, bottom=-MAXINT,
      Botto,  // components should upper the Botto
      ctop, cbot,
      zones_used=0,
      zone1_top, zone1_bot,
      zone2_top, zone2_bot;

  cbot = (bbs3-bbs2)*GREATER_MULTIPLIER; // see check_TM()
  Botto = bbs2+cbot+minrow;             // see check_TM()
  // Find top & bottom
  for (i=0, c=first; i<cmps; i++, c=c->next)
      {
  if (c->h<=min_TM_comp_h || c->w<min_TM_comp_w  // ignore too little components
      || c->row+c->h>Botto)   // ignore cells which bottom is below Botto
		 continue;
	no_bases_set=0;   // bases will be set below
	ctop=c->row; cbot=c->row+c->h;
	if (ctop < top)
		 top=ctop;
	if (cbot > bottom)
		 bottom=cbot;

	if (!zones_used)
	   {
	     zone1_top=ctop;
	     zone1_bot=cbot;
	     zones_used++;
	   }
	else
	  if (zones_used==1)
	   {
	     if ( (zone1_top>=cbot && zone1_bot<=ctop)
		  ||
			(zone1_top<=cbot && zone1_bot>=ctop)
		)      // zone1 intersects cell
		{
		  if (zone1_top>ctop)
		      zone1_top=ctop;
		  if (zone1_bot<cbot)
		      zone1_bot=cbot;
		}
	     else      // zone1 does not intersect cell
		{
		  zone2_top=ctop;
		  zone2_bot=cbot;
		  zones_used++;
		}
	   }
	else
	   {
	     if ( (zone1_top>=cbot && zone1_bot<=ctop)
		  ||
		  (zone1_top<=cbot && zone1_bot>=ctop)
		)      // zone1 intersects cell
		{
		  if (zone1_top>ctop)
		      zone1_top=ctop;
		  if (zone1_bot<cbot)
		      zone1_bot=cbot;
		}
	     if ( (zone2_top>=cbot && zone2_bot<=ctop)
		  ||
		  (zone2_top<=cbot && zone2_bot>=ctop)
		)      // zone2 intersects cell
		{
		  if (zone2_top>ctop)
		      zone2_top=ctop;
		  if (zone2_bot<cbot)
		      zone2_bot=cbot;
		}
	   }
      }
/*
  if (snap_activity('h'))
     {
       char sna[12];
       sprintf(sna,"NoBases %i", no_bases_set);
       snap_show_text(sna);
       snap_monitor();
     }
*/

  if (no_bases_set) return 1;
	// Check for zone1 & zone2 intersection
	if (zones_used>1 && zone1_bot<zone2_top)  // zone1 & zone2 have no intersection
     return 1;

  minrow=top;
  bbs1=0;
  bbs3=bottom-minrow;

  bbs2=((bbs1<<1)+bbs3)/3;
  bbs4=((bbs3<<2)-bbs1)/3;
  bbsm=(bbs2+bbs3)>>1;
  Ps=Psf=bbs3-bbs2;
  if (Ps<=min_TM_Ps)
     no_bases_set=1;
  return no_bases_set;
}



static int16_t try_TM (cell **first_cell_ptr, int16_t num)
{
  extern int16_t sMALL_SIZE;

	cell *first_next, *first_nextl,  // successors of fictive first cell
			 // If fictive first cell refers into sublist
			 //    I set first_nextl := NULL!
			 *last_prev, *last_prevl,    // pred. of fictive last cell
			 // If fictive last cell refers into sublist
			 //    I set last_prev=last_prevl := NULL!
			 *first,
       *c, *fc,
			 *first_let, *last_let,
			 *last_let_before_list, *last_cell_before_list,
			 *first_let_after_list, *first_cell_after_list,
			 *tc,
			 *clist[MAX_TM_COMPS];

	int16_t i=0, j, max_v, Tprob=0, Mprob=0;
	int16_t shift=2, strange_disposition=0;
	int16_t  obbs1, obbs2, obbs3, obbs4, oPs, oPsf, ominrow;
	int16_t  obbsm;  // Nick 28.01.2002
	uchar omulti_bas, //olang,
       let, TM_found=0, components_may_be_modified=0,
			 confidence=MAX_TM_CONFID,
			 chkTM;
#ifdef UP_DIGITS
	uchar	all_digits=1;
#endif

	if (num<0)
	   {
	     strange_disposition=1;   // TM vert.projection intersect with a letter
	     shift=1;
	     num=-num;
	   }
	first = *first_cell_ptr;
	// Store first cells of the list
	fc=cell_f();
	first_next=fc->next;
	first_nextl=fc->nextl;

	// Find last cell and last letter before first cell of sublist
	c=first; last_let_before_list=fc;
	last_cell_before_list = c->prev;
	while ((c=c->prev)!=fc)       // first - first cell of sub-list
	if (c->flg & c_f_symb)
		 {
			 last_let_before_list = c;
			 break;
		 }

	// Store last cell & letter of the list
	fc=cell_l();
	last_prev=fc->prev;
  last_prevl=fc->prevl;
  if (last_prevl==last_let_before_list)
     last_prevl=NULL;  // the last letter in whole list is before sublist

  // Find first cell and first letter after last cell of sublist
  i=0;

  first_let_after_list=last_let_before_list->nextl;
	c=first;
	while (i++<num)
	{
	  if (c->flg & c_f_symb)
	     {
		first_let_after_list = c->nextl;
		if (first_nextl==c)   // fictive first refers into sublist
				first_nextl=NULL;
		if (last_prevl==c)    // fictive last refers into sublist
		    last_prevl=NULL;
			 }
		c = c->next;
	}
  if (last_prev==c->prev)  // last cell of sublist is last in whole string
      last_prev=NULL;
  first_cell_after_list = c;
  // c - first cell after last cell of sub-list

	cell_f()->next = first;  cell_f()->nextl = cell_l();
  first->prev = cell_f();


// Extraction of sublist
  c=first;
	i=0;
	while (++i<num)
	 c=c->next;
	fc=cell_l();
	c->next = fc;
	fc->prev = c;
	fc->prevl=cell_f();
// Sublist extracted

  if (snap_activity('h'))
     { snap_show_text("Sublist extracted");
       snap_newcell (cell_f()->next);
       snap_monitor();
     }

// Store old bases && language
	 ominrow=minrow;
	 obbs1=bbs1;
	 obbs2=bbs2;
	 obbs3=bbs3;
	 obbs4=bbs4;
	 obbsm = bbsm; // Nick 28.01.2002
	 oPs  =Ps;
	 oPsf =Psf;
	 omulti_bas=multi_bas;
	if (create_new_bas (cell_f()->next,num)) // Change base lines && language
		 goto restore;  // no bases are set
	components_may_be_modified=1;

	init_sublist(0);
  sMALL_SIZE=min_TM_comp_h;
// Sublist is modified
	multi_bas &=0x7f;
			 // When (multi_bas & 128 == 0) def_locbas() does not change bbs*, etc
  if (snap_activity('h'))
      snap_show_text("TM-subrecog 0");
	ReRecogComponents(1);
			 // Set new nextl references BUT IN OTHER ITEMS DON'T DELETE OLD!?
	criteries();
	chkTM=0;
	goto chk_TM;
cont_TM:
  if (snap_activity('h'))
      snap_show_text("TM-subrecog 1");
	make_all_cuts();

//  make_all_glues();  Quite bad for small TMs 23.2.95
	ReRecogComponents(0);
#ifdef topCR
  trade_marks(); // for compose(R)
#endif
  chkTM++;
	Tprob=Mprob=0;
  goto chk_TM;  // 18.4.95

// 18.4.95  below
gogo_TM:
  chkTM++;
  make_all_glues();
  if (snap_activity('h'))
      snap_show_text("TM after glues");
  ReRecogComponents(0);
	Tprob=Mprob=0;
// 18.4.95  eof insertion

chk_TM:    // check for TradeMark
  TM_found=0;

  if (snap_activity('h')) snap_monitor();

	// Check series of recognized letters
	c=cell_f();
	if (c->nextl==cell_l())  // no letters are recognized
		 goto anal;
	i=0;  // # of letters
  while (c=c->nextl)
        if (c->nvers) i++;
	if (i==0 || i>4)
		 goto anal;
	c=cell_f();
#ifdef topCR
	// Check for (R)
	if (i==1 && chkTM)
		 {
				fc=c->nextl;
				if (fc->nvers && fc->vers[0].let==liga_CR)
					 {
						 TM_found=CR128;
						 goto restore;
					 }
		 }
#endif

#ifdef UP_DIGITS
	if (chkTM)
		 {
			while ( (c=c->nextl) &&	c->next) // check for digits
				{
					if (is_dig(c->vers[0].let) && c->vers[0].prob>MIN_DIGIT_PROB)
							continue;
					else
						 {
							all_digits=0;
							break;
						}
				}
			if (all_digits)
				{
					 TM_found=DIGITS64;
					 init_sublist(2);  // delete old cells
					 goto restore_bases;
				}
			c=cell_f();
		 }
#endif

  while ( (c=c->nextl) && c->next) // check for T in TradeMark
	{
		max_v = (c->nvers>2) ? 3 : c->nvers;
    if (!c->nvers)    // 30.3.95
       continue;
		for (j=0; j<max_v; j++)
				{
				let=c->vers[j].let;
	if (let=='t' || let=='T' ||  // let=='1' ||
					( language==LANG_RUSSIAN && (let==(uchar)'в' || let==(uchar)'’')) ) // Russian T
						{
							tc = c;
							TM_found=1;  // T is found
							Tprob += c->vers[j].prob;
						}
				}
		if (TM_found)
		      {
		       if ((Tprob<<shift) < tc->vers[0].prob)
			    TM_found=0;      // exclude T
		       break;
		      }
	}

	if (!TM_found)    // T is not found
		 {
			 if (strange_disposition)
			     goto anal;
			 tc=(cell *) NULL;
			 c = cell_f();

		 }

	fc=c->nextl;
  while (!fc->nvers && fc!=cell_l())  //30.3.95
	fc=fc->nextl;                 //30.3.95

	if (fc==cell_l())   // M will not be found
	   {
	     if (strange_disposition)
		 goto anal;
	     goto hard_case;
	   }

  // 30.3.95 - below  - no T
  if ( !tc && ((fc->col - cell_f()->next->col) > MAX_M_IN_T*fc->w))
       goto anal; //goto hard_case;
  // 30.3.95

  max_v = (fc->nvers>2) ? 3 : fc->nvers;
  for (j=0; j<max_v; j++)
      {
	let=fc->vers[j].let;
	if (let=='m' || let=='M' ||
			(language==LANG_RUSSIAN && (let==(uchar)'¬' || let==(uchar)'Њ') ) ) // Russian M
	   {
	     TM_found|=2;  // M is found
	     Mprob += fc->vers[j].prob;
	   }
      }



  if (TM_found!=3)
     {
       if (strange_disposition)
		 goto anal;
       goto hard_case; // *T*M*  not found
     }

  if ((Mprob<<shift) < fc->vers[0].prob)
     {
       TM_found &= 0xfd;      // exclude M
       if (strange_disposition)
          goto anal;
       goto hard_case;
     }


      // check for punctuations and distance between T and M
      j=fc->col -(tc->col+tc->w);   // distance
      if ( tc->next!=fc &&          // M is not successor of T
	   j>((tc->w)<<1) && j>((fc->w)<<1)  // too great distance
	 )
	 {
	   TM_found &=0xfc;    // exclude 3
	   goto anal;
	 }


  // Otherwise T is first (0) or second (1) in letter sequence.
	// Check left '(' and right ')'
     {
       c=tc->prevl;
       while (c!=cell_f() && !c->prevl->nvers)   //30.3.95
	     c=c->prevl;

       if (c!=cell_f()) // Check left '('
	  {
	    max_v = (c->nvers>2) ? 3 : c->nvers;
	    for (j=0; j<max_v; j++)
		{
		 let=c->vers[j].let;
		 if (let=='(')
				{
			 TM_found|=4;  // '(TM' found
			 break;
				 }
		}
			TM_found|=16;
		}

    c=fc->nextl;                              //30.3.95
    while (c!=cell_l() && !c->nextl->nvers)   //30.3.95
	     c=c->nextl;
    if (c!=cell_l()) // Check right ')'
		{
			max_v = (fc->nvers>2) ? 3 : c->nvers;
			for (j=0; j<max_v; j++)
		{
		 let=c->vers[j].let;
		 if (let==')')
				{
			 TM_found|=8;  // 'TM)' found
			 break;
				}
		}
			TM_found|=16;
		}

//   if ((TM_found&3)!=3)  // TM is not found
//       goto hard_case;
     if (strange_disposition && TM_found!=3)
	 goto anal;
	 goto restore;
		 }

hard_case:    // (1) *T*, no M,    (2) i=-1 - *M*, no T
	 if ((TM_found&16==16) || (TM_found&3)==0  // too much letters or not T nor M
			 ||  (Tprob+Mprob<BOTTOM_EXP) )  // too small expectations
			 goto anal;
	 confidence=BAD_TM_CONFID;

	 if (TM_found&2) //  *M* exists
			{
	if (fc==cell_f()->nextl)
		 {
			 tc = fc->prev;
			 if (tc==cell_f()
		|| tc->row<minrow || tc->row+tc->h > minrow+bbs3
		|| (fc->col-tc->col < TM_DUST_W)             // too thin  "T"
		|| (fc->col-cell_f()->next->col > fc->w+10)  // too width "T"

                || (cell_l()->prev->col+cell_l()->prev->w -(fc->col+fc->w)
                        > fc->w)  // too wide * in M*

		)
		goto anal;
			 cell_f()->nextl=tc;
			 tc->prevl=cell_f();
			 tc->nextl=fc;
			 fc->prevl=tc;
			 tc->flg=c_f_bad;
		 }
	else tc=cell_f()->nextl;  // pseudo-T
			}
	 else            // *T* exists
			{
	if (tc->nextl==cell_l())
		 {
			 fc = tc->next;
			 if (fc==cell_l()
		 || fc->row<minrow || fc->row+fc->h>minrow+bbs3
		 || (fc->col+fc->w-(tc->col+tc->w) < TM_DUST_W)

		 || (tc->col-cell_f()->next->col > tc->w)  // too wide * in *T
		 )
		 goto anal;
			 cell_l()->prevl=fc;
			 fc->nextl=cell_l();
			 fc->prevl=tc;
			 tc->nextl=fc;
			 fc->flg=c_f_bad;
		 }
	else fc=tc->nextl;        // pseudo-M
			}
	 TM_found|=3;

anal:
// 18.4.95  see below
   if (chkTM<2 && (TM_found&3)!=3 && TM_found!=CR128)
      {
	if (!chkTM) goto cont_TM;
	if (Ps>NOT_SMALLEST_SIZE)
	   goto gogo_TM;
      }
// 18.4.95  e of
restore:
     sMALL_SIZE=SMALL_SIZE;
	 if ((TM_found&3)==3
#ifdef topCR
			 || TM_found==CR128
#endif
			 )
			{
	cell *beg, *end,
			 *first_dust=NULL, *dust;
#ifdef topCR
		 if (TM_found==CR128)
				beg=end=cell_f()->nextl;  // (R)
		 else                         // TM
#endif
		 {
		 beg=tc;
		 end=fc;
		 if (TM_found&4)     // '(' before T
				 beg=tc->prevl;
		 if (TM_found&8)
				 end=fc->nextl;  // ')' after M
		 // add small dust after <end>
		 while ((end=end->next)!=cell_l())
		 if (end->col+end->w - (fc->col+fc->w) > fc->w)
				break;  // right of the next is too far from M
		 end=end->prev;
		 }

		 i=0; c=beg;
		 while (i<MAX_TM_COMPS)
		 {
			 clist[i++]=c;
		 omit:
			 if (c==end)
					break;
			 c=c->next;
			 if (c->row<minrow || c->row+c->h>minrow+bbs3)
					{                      // dust upper base 0 or below base 3
			if (first_dust)
				 {
					 dust->next=c;
					 c->prev=dust;
					 dust=c;
				 }
			else
				 first_dust=dust=c;
			goto omit;
					}
		 }
		 c=end->next;

#ifdef topCR
  if (TM_found!=CR128)
     {
#endif
		 compose_cell(i,clist,beg);
		 beg->nvers=1; beg->vers[0].let=liga_TM;
		 beg->vers[0].prob=confidence; beg->vers[1].let=0;
		 beg->flg=c_f_let;
		 beg->left=beg->col;
		 beg->right=beg->col+beg->w;
		 beg->cpos=ce_p_all;
#ifdef topCR
     }
#endif
		 beg->prevl=cell_f();   cell_f()->nextl=beg;
		 if (first_dust)
				{
		dust->next=c;
		c->prev=dust;
		c=dust;
				}
		 beg->next=c;  c->prev=beg;
		 // find nextl for created cell
		 while ((c->flg & c_f_symb)==0 && c!=cell_l())
			c=c->next;
		 beg->nextl=c; c->prevl=beg;
		 init_sublist(2);  // delete old cells
			}
	 else
     if ( components_may_be_modified )
				init_sublist(1);     // delete new cells

#ifdef UP_DIGITS
 restore_bases:
#endif
	 // Restore old bases
	 minrow=ominrow;
	 bbs1=obbs1;
	 bbs2=obbs2;
	 bbs3=obbs3;
	 bbs4=obbs4;
	 bbsm = obbsm; // Nick 28.01.2002

	 Ps  =oPs;
	 Psf =oPsf;
	 multi_bas=omulti_bas;
//   language = olang;  // Restore language

	// Restore first & last cell links
	fc=cell_f();
	// Search for first & last letter in sub-list
	first_let=last_let=NULL;
	first=fc->next;  // now first - current first real cell of sub-list

	c=fc;
	while ((c=c->next)!=cell_l())
		 if (c->flg & c_f_symb)
			{
	if (!first_let)
		 first_let = c;
	last_let=c;
			}

	if (first_let)
		 {
			 last_let_before_list->nextl = first_let;
			 first_let->prevl = last_let_before_list;

			 first_let_after_list->prevl = last_let;
			 last_let->nextl = first_let_after_list;

		 }
	else
		 {
			 last_let_before_list->nextl = first_let_after_list;
			 first_let_after_list->prevl = last_let_before_list;
		 }
	last_cell_before_list->next = first;
	first->prev = last_cell_before_list;

	first_cell_after_list->prev = cell_l()->prev;
	(cell_l()->prev)->next = first_cell_after_list;

	fc->next = first_next;
	if (first_nextl)  // if first letter before list
		 fc->nextl= first_nextl;


	fc=cell_l();
	if (last_prev)  // if last real cell in whole string is after sublist
			 fc->prev = last_prev;
	if (last_prevl) // if last real letter in whole string is after sublist
			fc->prevl= last_prevl;
	*first_cell_ptr=first_cell_after_list;
	if (snap_activity('h'))
	 {snap_show_text("TM-Final"); snap_monitor();}

	return TM_found;

}

static void ReRecogComponents (int16_t mode)
{
	cell *c, *prevl, *lastl;
	int16_t letters=0;

 prevl=c=cell_f();
 lastl=cell_l();
 while(c=c->next)
  {
	if (!c->next)
	    break;
	if (mode)  // initial re-recognition
	   {
	     c->flg=0;
	     if ( c->row<minrow || c->row+c->h>minrow+bbs3
//      || (c->w <TM_DUST_W && c->h<=TM_DUST_H)
      || c->h<=TM_DUST_H        // 18.4.95
		)
		c->flg=c_f_dust;
	     else
		short_recog_cell(c);
	     if (!c->flg)
		 c->flg = c_f_bad;
	   }

/*  comm. 29.3.95 *
	else       // final re-recognition
	   if (!c->nvers)
		c->flg=c_f_dust;
* */
	if (c->flg & c_f_symb)
	   {
			 c->prevl=prevl;
	     c->nextl=lastl;
			 prevl->nextl=c;
	     lastl->prevl=c;
	     prevl=c;
	     letters++;
	   }
	else
	   {
			//if (c->flg==c_f_bad)
			//c->flg=c_f_dust;
		    // was |=c_f_dust. In this case error was occur in adjust_links()
			//AK!
			err_pnlet(c);
	   }
  }

  if (!letters)
		 {             // link between fictive first & fictive last cell
       prevl->nextl=lastl;
			 lastl->prevl=prevl;
     }
}


static void init_sublist (int16_t mode)   // Initialization of sublist
// mode = 0 - creation of new (copied) cells,
//        1 - deleting new & restoring old cells,
//        2 - deleting old cells
{
  static cell *old_first_in_substr, *old_last_in_substr;
  cell *c, *last, *cn, *cprev;

	c=cell_f();
  last=cell_l();
  if (!mode)
     {
	  old_first_in_substr=c->next;
	  old_last_in_substr=last->prev;
	  cprev=c;
	  while ((c=c->next) !=last)
	     if ((c->flg & c_f_space)!=c_f_space)
	     {
	     cn = new_cell();
       	     *cn = *c;
	     cn->prev    = cprev;
	     cprev->next = cn;
	     cn->nextl = NULL;
	     cn->prevl = NULL;
	     cn->basflg= 0;
	     cn->nvers = 0;
       cn->cg_flag=/*0*/c->cg_flag;
	     cn->flg    =0;
	     cn->cpos        =0;
	     cn->reasno      =0;
	     cn->keg         =0;
	     cn->font        =0;
	     cn->recsource   =0;
         cn->history     =0;
	     cn->bas_acc     =0;
	     cprev=cn;
	   }
       last->prev=cprev;
			 cprev->next=last;
     }
	 else
   if (mode==1)  // 1 - deleting new & restoring old cells
     {
       c=cell_f()->next;
       last=cell_l();
       while (c!=last)
	     {
	       cn=c->next;
         del_cell(c);
	       c=cn;
			 }
       cell_f()->next = old_first_in_substr;
       old_first_in_substr->prev = cell_f();
       old_last_in_substr->next = cell_l();
       cell_l()->prev = old_last_in_substr;
     }
   else          // 2 - deleting old cells
     {
			 c=old_first_in_substr;
       old_first_in_substr=cell_f()->next;  // first in new sublist
			 cell_f()->next=c;
       last=cell_l()->prev;   // last in new sublist
       cell_l()->prev=old_last_in_substr;
       old_last_in_substr->next=cell_l();
       while (c!=cell_l())
	  {
		 cn=c->next;
	   del_cell(c);
	   c=cn;
	  }

       cell_l()->prev=last;
       cell_f()->next=old_first_in_substr;
     }
		 snap_newcell(cell_f()->next);
}


#ifdef UP_DIGITS
static uchar is_dig (uchar letter)
{
	uchar *a, digits[]={'0','1','2','3','4','5','6','7','8','9'};
	int16_t i=0;

	a=&digits[0];
	while (i++<10)
				{
					if (letter==*a)
							return 1;
					a++;
				}
	return 0;
}
#endif // UP_DIGITS

#endif // NEW_TM

