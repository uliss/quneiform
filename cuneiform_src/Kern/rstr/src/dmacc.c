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
/*                           module DMACC                            */
/*                                                                   */
/*********************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "struct.h"
#include "cuthdr.h"
#include "dmconst.h"
#include "func.h"
#include "ligas.h"
#include "lang.h"
#include "linutil.h"

#include "compat_defs.h"

#define MAX_DUST_CELLS 12
//extern uchar accent_tab[];
extern uchar db_pass;
static char b1b2;
static cell *LET, *I1;
static int16_t n_news;
static cell *new_cells[MAX_DUST_CELLS];

static int16_t col_comps(cell *B1, MN *mn1, int16_t pass);
static int16_t thinlev_top(puchar,int16_t,int16_t);
static int16_t thinlev_bot(puchar,int16_t,int16_t,int16_t); /* extra parameter - height */
/**************************************************************************/
int16_t try_cut_accent(cell *C1, B_LINES *my_bases, int16_t flag)
{
    int16_t ans = 0;

    if (!flag)  // flag==0 - Estimate
        {
        ans = try_cut_top_accent (C1, my_bases, flag);
        if ( !ans )
            ans = try_cut_bot_accent (C1, my_bases, flag);
        }
    else if (C1->bottom_accent)     // Final cut bottom accent
        ans = try_cut_bot_accent (C1, my_bases, flag);
    else                            // Final cut top accent
        ans = try_cut_top_accent (C1, my_bases, flag);

    return ans;
}
/**************************************************************************/
static int16_t col_comps(cell *B1, MN *mn1, int16_t pass)
{
 cell *newcell;
 uchar  just;
 if (pass && mn1->mnnext!=NULL)
   goto restall;
 just = B1->cg_flag & c_cg_just;
 do                 // create cells for all the components got by cut
 {
   newcell=create_cell(mn1,I1,B1->bdiff,B1->difflg);
   // est_snap(db_pass,newcell,"acc: new cell");
   newcell->difflg |= 128;
   newcell->cg_flag |= just;
   if (!newcell->env)
     goto restall;
   if (newcell->flg & (c_f_let + c_f_bad))
   {
     int16_t midbas, wrow;
     midbas = get_bsm();
     wrow = newcell->row;
     if ((wrow > midbas) || (wrow + newcell->h < midbas))
     { // dust, not a letter
       newcell->flg = c_f_dust;
       newcell->prevl->nextl = newcell->nextl;
       newcell->nextl->prevl = newcell->prevl;
       err_pnlet(newcell);  // AL 940319
       goto dust_came;
     }
     else
     {
       if (LET)    // already got big component
       {
         del_cell(newcell);
       restall:
         for (; n_news>0; )
           del_cell(new_cells[--n_news]);
         if (LET)    // can come here by label
           del_cell(LET);
         // est_snap(db_pass,I1,"all deleted");
         return -1;
       }
       LET = newcell;
     }
   } // let OR bad
   else
   {
     dust_came:
     newcell->cg_flag |= c_cg_cutacc;

	 // no space? - go away.   Nick 20.06.2002
	 if( n_news >= MAX_DUST_CELLS )
		 goto restall;

     new_cells [n_news++] = newcell;
   }
 } while ((mn1=mn1->mnnext)!=0) ;    // do_while  mn1 has next mn1
 if (pass)
   if ((LET==NULL) || (n_news==0))
     goto restall;
 return n_news;
}
/**************************************************************************/
int16_t try_cut_top_accent(cell *C1, B_LINES *my_bases, int16_t flag)
{
 cell sv_b1;
 int16_t  b1, b2, ret_ans;
 uchar *rp1, *rp2, *rp3;
 uchar acc_raster[1024];
 uchar let,chr;
 int16_t  prob, dh, rpn, i, cr, cc, cw, ch, mh, ww;
 MN   *mn1;
 s_glue GL;

#define PROBOK 140

 let=C1->vers[0].let;
 prob=C1->vers[0].prob;

 if (language==LANG_ENGLISH ||
     memchr("DIJL17flt",let,9) && C1->vers[0].prob>=PROBOK &&

	 // Попытка отрезать акцент от f ради i_right_accent. 08.09.2000 E.P.
	 !(let=='f' &&
		(language==LANG_CZECH || language==LANG_HUNGAR)
	  ) &&

	 // Попытка отрезать roof_accent от I. 13.09.2000 E.P.
	 !(let=='I' && language==LANG_ROMAN) &&

	 // Попытка отрезать macron от I. 16.07.2001 E.P.
	 !(let=='I' && language==LANG_LATVIAN) &&

	 // Попытка отрезать точку от I1. 30.05.2002 E.P.
	 !(memchr("I1",let,2) && language==LANG_TURKISH)
	)
	return 0;

 Z = &string;
 cr = C1->row; cc = C1->col; cw = C1->w; ch = C1->h;
 if (((b2=my_bases->b2)-MAX(2,my_bases->ps/5)) < cr)
   return 0;
 b1b2 = 2; dh = b2-cr;

 // Резка по b1
 if ( ( (b1=my_bases->b1)-

	 // в балтийских языках есть низенький макрон! Nick 19.08.01
	   MAX(3,my_bases->ps/(
		(
		is_baltic_language(language) ||
		is_turkish_language(language) // Ради точки над I, i_sans_accent 30.05.2002 E.P.
		)?5:4) )
	  )-
	   ((dh<=my_bases->ps/2)?1:0)
		>= cr
		||
		C1->vers[0].prob>=PROBOK &&
        (
		is_cen_language(language) &&	// 08.09.2000 E.P.
			memchr("AECNO0RSZTUY", let, 12) ||
		is_baltic_language(language) &&	// 10.07.2001 E.P.
			memchr("ACEIOSUZ", let, 8) ||
		is_turkish_language(language) &&	// 10.07.2001 E.P.
			memchr("AGIOU", let, 5) ||
		0
		)
    )
    { b1b2 = 1; dh = b1-cr; }

 // check proportions
 mh = ch - dh;
 if (b1b2 == 1)   // capital letter
 {
   if (mh * 3 < cw * 2)   // too thick
     return 0;
   if (mh * 2 > cw * 5)   // too thin
     return 0;
 }
 else
 {
   if (mh * 3 < cw * 2)   // too thick
     return 0;
   if (mh  > cw * 2)   // too thin
     return 0;
 }
 ww = (cw+7)>>3;
 // make raster at standard (work_raster) place
 rp1=(uchar*)save_raster(C1);
 // copy raster to be own
 rp2=(uchar *)&acc_raster;
 rpn = ww * ch;
   for (i=0; i<rpn; i++)
     *(rp2++) = *(rp1++);
 rp2=(uchar *)&acc_raster;
 dh=thinlev_top(rp2,cw,dh);
 if( dh<1 )
    return 0;
 rp3 = rp2 + ww * dh;
 // for (i=0; i<ww; i++,rp3++)
 //   *rp3=0;
 // extrcomp both parts
 mn1 = c_locomp(rp2,ww,dh,C1->r_row,C1->r_col);
 if( mn1==NULL )
  return 0;
 I1 = C1->prevl;
 n_news = 0; LET=NULL;
 if ( col_comps (C1, mn1,0) < 0)
   return 0;
 mn1 = c_locomp(rp3,ww,(int16_t)(ch-dh),(int16_t)(C1->r_row+dh),C1->r_col);
 if( mn1==NULL )
  return 0;
 if ( col_comps (C1, mn1,1) < 0)
   return 0;
 // est_snap(db_pass,C1,"horiz. cut done");
 ret_ans = 0;
 memcpy (&sv_b1,C1,sizeof(cell));
 del_retain_cell(C1);
 // est_snap(db_pass,I1,"old C1 deleted");
 memcpy(C1,LET,sizeof(cell));
 del_cell(LET);
 // est_snap(db_pass,I1,"new LET deleted");
 insert_cell(C1,I1);
 est_snap(db_pass,C1,"try cut top accent");	// 18.06.2002 E.P.
 if (flag)    // final accent's cut ?
 {
   recog_cell (C1);
   chr = C1->vers[0].let;
   if (
		// Конфликтные коды // 07.09.2000 E.P.
		!is_baltic_language(language) && // 17.07.2001 E.P.
		!is_turkish_language(language) && (	// 30.05.2002 E.P.
		!is_cen_language(language) && (
			chr==i_left_accent ||
		    chr==i_2dot_accent ||
		    chr==II_left_accent ||
		    chr==II_2dot_accent
			) ||
		   chr==i_right_accent  ||
		   chr==i_roof_accent   ||
		   chr==II_right_accent ||
		   chr==II_roof_accent
		) ||

		is_baltic_palka(chr) ||	// Балтийские палки. 10.07.2001 E.P.
		is_turkish_palka(chr)	// Турецкие палки. 21.05.2002 E.P.
      )

		stick_center_study(C1,NULL,1);

   est_snap(db_pass,C1,"accent's cut completed");
   return 0;
 }
 // NOW line contains:  C1 (=LET), new_cells
 recog_cell (C1);
 if (C1->nvers > 0)
 {
   chr = C1->vers[0].let;
   if (prob>=PROBOK &&
       (let=='d' &&

		 // Конфликтные коды 17.07.2001 E.P.
		 !is_baltic_language(language) && (	// 17.07.2001 E.P.
		 chr==a_right_accent || chr==u_left_accent ||

		 // Конфликтные коды 07.09.2000 E.P.
		 !is_cen_language(language) &&
			(chr==a_left_accent || chr==a_circle_accent)
		) ||

		(let=='O' || let=='0') &&
		 (chr==u_2dot_accent ||

		// Конфликтные коды // 07.09.2000 E.P.
		!is_cen_language(language) &&
			(chr==u_roof_accent || chr==UU_roof_accent)
	 ) ||
	(let=='U' || let=='V') && let_sans_acc[chr]=='u' ||
	// Конфликтные коды. 17.07.2001 E.P.
    !is_baltic_language(language) && (	// 17.07.2001 E.P.
	let=='2' && chr==i_roof_accent ||
	let=='8' && chr==o_roof_accent ||
	let=='5' &&
     (chr==a_right_accent || chr==a_roof_accent ||
	  chr==o_right_accent || chr==o_roof_accent)
	  ) ||

	  // От I согласен отрезать только roof_accent. 13.09.2000 E.P.
	  is_cen_language(language) &&
	  (let=='I' && chr!=II_roof_accent) ||

	  // От Ii согласен отрезать только macron. 10.07.2000 E.P.
	  is_baltic_language(language) &&
		( let=='I' && chr!=II_macron ||
	      let=='i' && chr!=i_macron  ||

		  // Что может получиться, если от цифры отрезать акцент. 17.07.2001 E.P.
		  let=='2' && chr==i_macron ||
		  let=='8' &&
			( chr==o_tild_accent || chr==o_2dot_accent) ||
		  let=='5' &&
			(chr==a_2dot_accent || chr==a_macron ||
			 chr==o_tild_accent || chr==o_2dot_accent) ||

		  // От чего не следует отрезать макрон над i
		  strchr("tIl1F",let) && chr==i_macron		// 16.08.2001 E.P.
		 ) ||

		// Запрещенные комбинации для турецкого:
		// let - то что было, chr - что получилось после отрезания
		is_turkish_language(language) &&
		  (
		  // От I согласен отрезать точку либо roof_accent.
		  let=='I' && chr != II_dot_accent && chr != II_roof_accent ||
		  0
		  )
	    )
	  )
     ret_ans=0;
   else
    {
    if (
		// Конфликтные коды // 07.09.2000 E.P.
		!is_baltic_language(language) &&	// 17.07.2001 E.P.
		!is_turkish_language(language) && (	// 30.05.2002 E.P.
		!is_cen_language(language) && (
			chr==i_left_accent ||
		    chr==i_2dot_accent ||
		    chr==II_left_accent ||
		    chr==II_2dot_accent
			) ||
	    chr==i_right_accent  ||
        chr==i_roof_accent   ||
	    chr==II_right_accent ||
        chr==II_roof_accent
	    ) ||

		is_baltic_palka(chr) ||	// Балтийские палки. 10.07.2001 E.P.
		is_turkish_palka(chr)	// Турецкие палки. 30.05.2002 E.P.

      )

     stick_center_study(C1,NULL,1);

    if (let_sans_acc[chr]=='u')
     {
     GL.celist[0]=C1; GL.celist[1]=NULL; GL.ncell=1;
     atlant(C1,&GL,1,4);
     }
    ret_ans = C1->vers[0].prob;

    if ( (accent_tab[chr] & ~(ACC_DOT|ACC_SUPERUP) ) == 0 &&  // 31.08.2000 E.P.
        !(language==LANG_POLISH && (chr==z_dot_accent||chr==ZZ_dot_accent)) &&	  // 13.10.97 E.P.
        !(language==LANG_LITHUANIAN && (chr==e_dot_accent||chr==EE_dot_accent))	&&  // 13.08.2001 E.P.
        !(language==LANG_TURKISH && chr==II_dot_accent)	  // 13.08.2001 E.P.
       )
     ret_ans=0;                        //  (don't count dot and superups)
    }
 }
 if (ret_ans)
  est_snap(db_pass,C1,"accent's cut estimated");
 for (; n_news>0; )
   del_cell(new_cells[--n_news]);
 del_retain_cell(C1);
 memcpy(C1,&sv_b1,sizeof(cell));
 insert_cell(C1,I1);
 // est_snap(db_pass,C1,"line restored");
 return ret_ans;
}
/**************************************************************************/
static int16_t thinlev_top(puchar r,int16_t w,int16_t dh)
 {
 int16_t l,d,i,j,s1,s2,min1,min2,i1,i2;

 l=(w+7)/8;
 d=MIN(2,dh/3);

 if (dh<=1)
    return dh;      // 20.07.97 E.P. (instead of nonsense try to cut dot)

 for (min1=min2=w+1,i=MAX(2,dh-d); i<=dh+d; i++)
  {
  for (s1=s2=j=0; j<w; j++)
   if (r[l*(i-1)+j/8]&(128>>(j%8)))
    {
    s1++;
    if (r[l*i+j/8]&(128>>(j%8)))
     s2++;
    }
  if (s1<min1 || s1==min1 && s2<min2)
   {min1=s1; min2=s2; i1=i2=i;}
  else
   if (s1==min1 && s2==min2)
    i2=i;
  }
 if (i1==i2)
  return i1;
 if (i1<=dh && i2>=dh)
  return dh;
 return ((abs(i1-dh)<=abs(i2-dh))?i1:i2);
 }
/**************************************************************************/
static int16_t thinlev_bot(puchar r,int16_t w,int16_t h,int16_t dh)
{
 /* Find height of bottom accent. */
 int16_t l,d,i,j,s1,s2,min1,min2,i1,i2;

 l=(w+7)/8;
 d=MIN(2,dh/3);
 i1=i2=dh;  // Nick 20.06.2002
 for (  min1=min2=w+1,i=MAX(2,dh-d);
        i<=MIN( dh+d, h/2 );   /* restrict to the lower half of raster */
        i++
     )
  {
  for (s1=s2=j=0; j<w; j++)
   if (r[l*(h-i)+j/8]&(128>>(j%8)))    /* i-th line (1-based) from bottom */
    {
    s1++;
    if (r[l*(h-i-1)+j/8]&(128>>(j%8)))  /* (i+1)-th line (1-based) from bottom */
     s2++;
    }
  if (s1<min1 || s1==min1 && s2<min2)
   {min1=s1; min2=s2; i1=i2=i;}
  else
   if (s1==min1 && s2==min2)
    i2=i;
  }
 if (i1==i2)
  return i1;
 if (i1<=dh && i2>=dh)
  return dh;
 return ((abs(i1-dh)<=abs(i2-dh))?i1:i2);
}
/**************************************************************************/
int16_t try_cut_bot_accent(cell *C1, B_LINES *my_bases, int16_t flag)
{
 cell sv_b1;
 int16_t  b1, b3, ret_ans=0;
 uchar *rp1, *rp2, *rp3;
 uchar acc_raster[1024];
 uchar let;
 int16_t  prob, dh, rpn, i, cr, cc, cw, ch, mh, ww;
 MN   *mn1;
 SVERS svers;

#define PROBOK 140

 //est_snap(db_pass,C1,"try cut bottom accent?");
 let=C1->vers[0].let;
 prob=C1->vers[0].prob;

 if (language!=LANG_FRENCH		&&
	 language!=LANG_POLISH		&&
	 language!=LANG_ROMAN		&&	// 08.09.2000 E.P.
	 language!=LANG_LATVIAN		&&	// 10.07.2001 E.P.
	 language!=LANG_LITHUANIAN	&&
	 language!=LANG_TURKISH			// 30.05.2002 E.P.
		||
     memchr("fjQy",let,4) && C1->vers[0].prob>=PROBOK
    )
  return 0;

 Z = &string;
 cr = C1->row; cc = C1->col; cw = C1->w; ch = C1->h;
 if (((b3=my_bases->b3)+MAX(2,my_bases->ps/5)) > cr+ch)
   return 0;

 b1b2 = 2;
 dh = cr+ch-b3;

 if (((b1=my_bases->b1)-MAX(3,my_bases->ps/4))-((dh<=my_bases->ps/2)?1:0)>=
                                      cr
    )
    b1b2 = 1;

 // check proportions
 mh = ch - dh;
 if (b1b2 == 1)   // capital letter
 {
   if (mh * 3 < cw * 2)     // too thick
     return 0;
   if (mh * 2 > cw * 5)     // too thin
     return 0;
 }
 else
 {
   if (mh * 3 < cw * 2)     // too thick
     return 0;

   // too thin
   if ( language!=LANG_LATVIAN	  &&	// Для l_bottom_accent	10.07.2001 E.P.
	    language!=LANG_LITHUANIAN &&	// Для Ii_bottom_accent 10.07.2001 E.P.
	    language!=LANG_TURKISH &&		// Для II_dot_accent 30.05.2002 E.P.
	    mh  > cw * 2 + (language==LANG_ROMAN? 2*cw:0)
	  )
     return 0;
 }
 ww = (cw+7)>>3;
 // make raster at standard (work_raster) place
 rp1=(uchar*)save_raster(C1);
 // copy raster to be own
 rp2=(uchar *)&acc_raster;
 rpn = ww * ch;
   for (i=0; i<rpn; i++)
     *(rp2++) = *(rp1++);
 rp2=(uchar *)&acc_raster;
 dh=thinlev_bot(rp2,cw,ch,dh);

// Для турецких cs полезно отрезать ближе к b3. 18.06.2002 E.P.
if (is_turkish_language(language))
	{
	if ( cr + ch - dh > b3)
		dh++;
	}

 rp3 = rp2 + ww * (ch-dh);
 // for (i=0; i<ww; i++,rp3++)
 //   *rp3=0;
 // extrcomp both parts: first bottom accent, next letter
 mn1 = c_locomp(rp3,ww,dh,(int16_t)(C1->r_row+ch-dh),C1->r_col);
 if( mn1==NULL )
  return 0;
 I1 = C1->prevl;
 n_news = 0; LET=NULL;
 if ( col_comps (C1, mn1,0) < 0)
   return 0;
 mn1 = c_locomp(rp2,ww,(int16_t)(ch-dh),C1->r_row,C1->r_col);
 if( mn1==NULL )
  return 0;
 if ( col_comps (C1, mn1,1) < 0)
   return 0;
 // est_snap(db_pass,C1,"horiz. cut done");
 ret_ans = 0;
 memcpy (&sv_b1,C1,sizeof(cell));
 del_retain_cell(C1);
 // est_snap(db_pass,I1,"old C1 deleted");
 memcpy(C1,LET,sizeof(cell));
 del_cell(LET);
 // est_snap(db_pass,I1,"new LET deleted");
 insert_cell(C1,I1);
 // est_snap(db_pass,I1,"new C1 inserted");
 // NOW line contains:  C1 (=LET), new_cells

 est_snap(db_pass,C1,"try cut bot accent");	// 18.06.2002 E.P.

 recog_cell (C1);

 if (C1->nvers > 0)
   {
   if (C1->bottom_accent ||	// 13.08.2001 E.P.
	   bottom_accent(C1)
	  )
        {
        save_vers(C1,&svers);
        ret_ans = C1->vers[0].prob;
        if (!flag)
            est_snap(db_pass,C1,"bottom accent cut estimated");
        }
   }

 for (; n_news>0; )
   del_cell(new_cells[--n_news]);

 del_retain_cell(C1);
 memcpy(C1,&sv_b1,sizeof(cell));
 insert_cell(C1,I1);
 // Now line restored.

 // Set flag "bottom_accent" and restore accented versions
 if (ret_ans)
    {
    C1->bottom_accent = 1;
    C1->nvers = svers.nvers;
    memcpy(C1->vers, svers.vers, sizeof (C1->vers) );
    }

 if (flag )     // final accent cut ?
    {
    est_snap(db_pass,C1,"bottom accent end");
    return 0;
    }

 return ret_ans;
}
/**************************************************************************/
