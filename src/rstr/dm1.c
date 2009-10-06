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
/*                           module DM1                              */
/*                                                                   */
/*********************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "struct.h"
#include "cuthdr.h"
#include "dmconst.h"
#include "func.h"
#include "ligas.h"
#include "lang.h"
#include "linutil.h"
#include "tuner.h"
#include "func.h"
#include "tm.h"     //10.4.95
#include "status.h"
#include "p2libr.h"

#include "compat_defs.h"

static struct cut_elm cut_pnts[64];
static struct {int16_t row,col,count;} debug_try_cut ={0};

struct cut_elm *my_cut_points;  //ALIK 01-31-96 04:56pm
int16_t    show_cut_points;
char   StopPoint;
int16_t    flag_cut_point=0;
int16_t    flag_ShiftF9=0;

static SVERS sv00, svown;
static B_LINES my_bases;
static int16_t bs12, bs2m, bsm3, bs34;
static char glob_diff;
static char glob_dflg;
static char wch[80];
static char madeBOX;
#ifdef NEW_TM    //10.4.95
 static int16_t TM_suspect(int16_t position, cell *c);
#endif
static Bool is_comma(cell *c, int16_t bases3);
static void delcels(all_cells *);
static int16_t colcels(all_cells *,MN *,cell *,cell *,int16_t);
static int16_t forest(cell *,int16_t,int16_t);
static int16_t pen_badust;  // penalty for "bad-->dust" transform
int16_t    try_cut_accent(cell *, B_LINES *, int16_t);
cell *dot_ij(cell *c);
static int16_t dust_is_dot(all_cells *,cell *);
static int16_t owned_dust(cell *,cell *);
//static int16_t dust_is_cut(cell *,all_cells *);
cell * finpat(cell *,s_glue *, int16_t, uchar, uchar);
void svcell_to_cell(cell *c);
void cell_to_svcell(cell *c);
static int16_t rec_own(cell *,s_glue *,int16_t,int16_t,int16_t,pint16_t);
static int16_t forbid_stick_cut(cell *,cut_pos *,int16_t);

#define TUR_PALKA_POROG 211

extern uchar db_status;	// snap presence byte
extern uchar db_trace_flag;  // 2 - more detailed estimate (ALT-F7)
extern uchar fax1x2;
extern char db_pass;
extern uchar language;
static int16_t  dust_monus;
static uchar dust_mon_flag;

char fbg=0;
extern int16_t line_number;
extern FILE *dbg_f;
char Ldb_flag = 0;
static void corr_cut();
c_comp * env_to_show;
int16_t w_to_show, h_to_show, row_to_show, col_to_show;

static char log_str[256], *log_s;

static void lsnap(char user,cell *C,pchar txt, int16_t p)
{
 char wrk[16];
 if (!db_status) return;
 snap_newcell(C);
 if (snap_activity(user))
  {
  strcat (wch, txt);
  sprintf (wrk,"p=%d ",p);
  strcat (wch,wrk);
  snap_show_text(wch);
  snap_monitor();
  }
}

void est_snap(char user,cell *C,pchar txt)
{
 if (!db_status) return;
 snap_newcell(C);
 if (snap_activity(user))
 {
   cell cWRK;
   if (env_to_show)
   {
     memcpy (&cWRK, C, sizeof (cWRK));
     C->env = env_to_show;
     C->r_row = row_to_show; C->r_col = col_to_show;
     C->w = w_to_show; C->h = h_to_show;
     C->cg_flag &= ~c_cg_noenv;
   }
   snap_show_text(txt);
   snap_monitor();
   if (env_to_show)
     memcpy (C,  &cWRK, sizeof (cWRK));
 }
}


//static int dc_cnt=0;
static void delcels(all_cells *CL)
 {
 int16_t nc;
 cell *wc;

 if( !CL )
	 return;

 for (nc=0; nc < CL->Nb; nc++)
 {
   wc = CL->bcells[nc];
   //if (wc->flg != 0xff)
     del_cell(wc);

 }
 for (nc=0; nc < CL->Nd; nc++)
 {
   wc = CL->dcells[nc];
   //if (wc->flg != 0xff)
     del_cell(wc);
 }
}

static int16_t colcels(all_cells *CL, MN  *mn1, cell *I1,cell *I3, int16_t wd)
 {
 int16_t  f1, fl1, fdust, mincol, maxend, col, colend;
 cell *newcell;
 CL->minlet = CL->maxlet = CL->mincl = CL->maxcl = NULL;
 f1 = fl1 = 0;
 CL->flag = 0;
 mincol = 32000; maxend = -32000;
  pen_badust = (language == LANG_RUSSIAN )? 160:0; // Valdemar
 //pen_badust = 0;
 CL->Nb=CL->Nd=fdust=0;
 CL->minb=CL->mind=CL->minf=32000;
 if (!mn1->mnnext)
  return -1;
 do                 // create cells for all the components got by cut
  {
  newcell=create_cell(mn1,I1,glob_diff,glob_dflg);    newcell->difflg |= 128;
  //est_snap(db_pass,I1,"cut_off cell inserted");
  newcell->cg_flag |= c_cg_just;
  if (!newcell->env)
    {
	  // Nick 15.08.2002 was bug - don't add too many cells !!!
	  if( CL->Nd >= MAX_CELLS_IN_LIST )
	  {
		del_cell(newcell);
		return 0;
	  }

	  CL->dcells[CL->Nd++]=newcell;
      if (CL->minlet == NULL)
        { CL->flag |= (dust_left | dust_right); return 0; }
      if (CL->minlet->col > mincol)
        CL->flag |= dust_left;
      if ((CL->maxlet->col + CL->maxlet->w) < maxend)
        CL->flag |= dust_right;
      return 0;
    }
  col=newcell->col; colend = col + newcell->w;
  if (col < mincol) mincol = col;
  if (colend > maxend) maxend = colend;
  inter_diff(newcell);
  if (!f1)
    { CL->mincl = CL->maxcl = newcell; f1=1;}
  else
  {
    if (CL->mincl->col > col) CL->mincl = newcell;
    if ((CL->maxcl->col + CL->maxcl->w) < colend) CL->maxcl = newcell;
  }
  newcell->cg_flag |= c_cg_noglue;
  if (newcell->flg & (c_f_let + c_f_bad))
  {
    int16_t midbas, wrow;
    midbas = get_bsm();
    wrow = newcell->row;
    if ((wrow > midbas) || (wrow + newcell->h < midbas))
    {
      newcell->flg = c_f_dust;
      newcell->prevl->nextl = newcell->nextl;
      newcell->nextl->prevl = newcell->prevl;
      err_pnlet(newcell);  // AL 940319
      pen_badust += (newcell->w + newcell->h) * 16/def_locbas(newcell);
      goto dustgot;
    }

	// Nick 15.08.2002 was bug - don't add too many cells !!!
	if( CL->Nb >= MAX_CELLS_IN_LIST )
		del_cell(newcell);
	else
	{
		CL->bcells[CL->Nb++]=newcell;
		if (col < CL->minb)
		  CL->minb=col;
		if (!fl1)
		  { CL->minlet = CL->maxlet = newcell; fl1=1;}
		else
		{
		  if (CL->minlet->col > col) CL->minlet = newcell;
		  if ((CL->maxlet->col + CL->maxlet->w) < colend) CL->maxlet = newcell;
		}
	}
  }
  else
  {
 dustgot:
    if(language == LANG_RUSSIAN)
    {int16_t midbas, wrow;
    midbas = get_bsm();
    wrow = newcell->row;
    if( wrow >= midbas-1 && // Valdemar: allow cut for special cases
        newcell->col+newcell->w > I3->col) pen_badust-=160;
    }
    if (col < CL->mind) CL->mind=col;
    if (newcell->w > 2) fdust |= 1;
    if (newcell->w > (wd>>1)) fdust |= 2;

	// Nick 15.08.2002 was bug - don't add too many cells !!!
	if( CL->Nd >= MAX_CELLS_IN_LIST )
		del_cell(newcell);
	else
		CL->dcells[CL->Nd++]=newcell;
  }
 } while ((mn1=mn1->mnnext)!=0) ;    // do_while  mn1 has next mn1

 if (CL->minlet == NULL)
  { CL->flag |= (dust_left | dust_right);
    return 0;
  }
 if (CL->minlet->col > mincol)
  CL->flag |= dust_left;
 if ((CL->maxlet->col + CL->maxlet->w) < maxend)
  CL->flag |= dust_right;
 if (CL->Nb > 1)
   if (CL->minlet->col >= CL->maxlet->col)
     CL->flag |= cl_disorder;
return CL->Nb;
}

static int16_t dust_usage;
//extern char accent_tab[];
int16_t discrid(cell *B1, int16_t mon)
 {
 uchar p; int16_t fl, np, max, dd; version *v;  uchar c;
 dd=dust_usage=0;
 if (B1->nvers==0) return 0;
 max = 0;
 for (v=B1->vers; ((c=v->let) != 0) && ((p=v->prob)!=0); v++)
  {
   np=p; fl = 1;
   if (accent_tab[c]==0)
     { np -= mon; fl = 0; }
   if (max < np) { max = np; dust_usage=fl; }
  }
 return max;
 }


static int16_t forest(cell *B1, int16_t p, int16_t TR)
 {
 char c;

 c= B1->vers[0].let;
 if (((c=='o') || (c=='O') || (c=='0')) && (p >= TR))
  return 1;
 return 0;
 }

static int16_t dust_is_dot(all_cells *CL, cell *C)
{
   cell *dot;
   char l;
   version *v;
   int16_t nv, p;
   for (nv=0, v=C->vers; nv<C->nvers; nv++,v++)
   {
     l = v->let;
     if ((l=='i') || (l=='j') || (l==invers_exm) || (l==invers_qm) ||
         (language == LANG_POLISH && ( l == 'z' || l == 'Z' ) ) ||

		 // 16.07.2001 E.P.
         (language == LANG_LITHUANIAN && ( l == 'e' || l == 'E' ) ) ||

		 // 30.05.2002 E.P.
         (language == LANG_TURKISH && ( l == i_sans_accent || l == 'I' ) )
        )
     { p=v->prob;
       if (p >= 200) goto testdot;
     }
   }
   return 0;   // no solid 'i' 'j' found
 testdot:
   if ((l=='i') || (l=='j'))
       dot = dot_ij(C);
   else if ( language == LANG_POLISH && ( l == 'z' || l == 'Z' ) )
       dot = dot_ij(C);
   // 16.07.2001 E.P.
   else if ( language == LANG_LITHUANIAN && ( l == 'e' || l == 'E' ) )
       dot = dot_ij(C);
   else if ( language == LANG_TURKISH && ( l == i_sans_accent || l == 'I' ) )
       dot = dot_ij(C);
   else
       dot = dot_inv(C);
   if (dot == CL->dcells[0])
     return 1;
   return 0;
}

int16_t decidust(cell *B1)
 {
 int16_t h, w, row, d;
 long cellsz, dustsz;
 cell * wc;

 wc=B1;
pa:
 wc=wc->prev;
 if (wc->flg  & c_f_dust)
 {
   if (wc->cg_flag & c_cg_noglue) goto pa;
   if ( ((row=wc->row)+wc->h >= my_bases.b1) &&
        (row <= my_bases.bm) &&
        ((B1->col - wc->col) <= 2)
      )
     if  (!owned_dust(B1->prevl,wc))
       goto ret2;

 }
 wc=B1;
fa:
 wc=wc->next;
 if (wc->flg & c_f_dust)
 {
   if (wc->cg_flag & c_cg_noglue) goto fa;
   d = (wc->col <= B1->col+B1->w)? 6 : 5;
   if ( ((row=wc->row)+wc->h >= my_bases.b1) &&
        (row <= my_bases.bm) &&
	((wc->col + wc->w - B1->col - B1->w) <= d)   // 93.07.14 was <= 4
      ) // dust in our vicinity
   { cell *BP;
     if  (owned_dust(B1->nextl,wc))    // dust utilized by neighbour
       return 0;
     BP=B1->prevl;
     if (BP->flg & c_f_fict)       // no predessor
       goto ret2;
     if  (!owned_dust(BP,wc))    // not a dust_utilising letter
       goto ret2;
     return 0;
   }
 }
 return 0;
 ret2:
 cellsz = (h=B1->h) * (w=B1->w);
 dustsz = wc->h * wc->w;
 if (w < h)       // doubtful as multi_letter cell
   dust_monus = (int16_t)(dustsz * MONdust * 32 / cellsz);
 else
   dust_monus = (int16_t)(dustsz * MONdust * 256 / cellsz);
 if (dust_monus > MONdust) dust_monus = MONdust;
 return 2;
 }

static int16_t owned_dust(cell *c,cell *cc)
 {
 uchar let;

 let=c->vers[0].let;
 switch(let)
  {
  case 'i':
  case 'j':
   if (cc==dot_ij(c))
    return 1;
   return 0;
  case invers_exm:
  case invers_qm:
   if (cc==dot_inv(c))
    return 1;
   return 0;
  default: return 0;
  }
 }

static cell * try_glue(s_glue *GL, cell *BC)
{

 cell *CC, *LC; int16_t glue_pass, i, diff; MN *mn; char dflag;
 cut_pos cposd;  // place of cut for 1st, 2nd points and dummy
 SVERS  svs, sv0;
 s_glue GLM;
 diff = BC->bdiff; dflag = BC->difflg & 0xf0;
 LC = BC->prev;
 save_vers(BC,&svs);
 if (BC->flg & (c_f_let | c_f_bad))
   LC = BC->prevl;
 for (i=0; i<GL->ncell; i++)
   GL->complist[i]=GL->celist[i]->env;
 glue_pass = 0;
 again:
 mn=glue(GL->complist, glue_pass);
 if (mn == NULL)
   { glsnap(db_pass,cell_f()->next,"collect extrcomp err"); return NULL; }
    // Valdemar : 03-18-96 05:07pm
    if(language == LANG_RUSSIAN && GL->ncell > 1)
    {
    if(GL->celist[0]->stick_inc == NO_INCLINE &&
       (GL->celist[0]->flg&(c_f_bad+c_f_let)))
       stick_center_study(GL->celist[0],NULL,1);
     if(GL->celist[1]->stick_inc == NO_INCLINE &&
       (GL->celist[1]->flg&(c_f_bad+c_f_let)))
       stick_center_study(GL->celist[1],NULL,1);
   if(GL->celist[1]->stick_inc && GL->celist[0]->stick_inc)
     mn->mnnext = (MN*)1; // artifact don't glue sticks
    }
 if (mn->mnnext)  // not glued
 { if (glue_pass)
    { glsnap(db_pass,cell_f()->next,"collection not glued"); return NULL; }
   glue_pass = 1;
   goto again;
 }
 CC=create_cell(mn,LC,(char)diff,dflag);
 inter_diff(CC);
 if (CC->flg&(c_f_let|c_f_bad) && (if_dust(CC)&7))
 {
   CC->flg=c_f_dust;
   CC->prevl->nextl=CC->nextl;
   CC->nextl->prevl=CC->prevl;
   err_pnlet(CC);  // AL 940319
 }
 if (CC->flg & (c_f_let | c_f_bad))
 {
   GLM.flarg = GFcut;               // artifact
   GLM.arg   = GAtigr;
   estcomp('a',CC,&sv0,TRS2,0,0,&GLM,&cposd,&cposd,0,0,0);
   if (svs.vers[0].let != CC->vers[0].let)
   {  uchar p1, p2;
      p1 = svs.vers[0].prob;
      p2 = CC->vers[0].prob;
      if ((p2 < TRS2) || ((p2-p1) < 2))
      // 93.07.30 was "MUCH better" ) //  || ((p2-p1) < 70))
      // glued must be VERY good AND better than estimate
      {
        if (glue_pass)
          rest_vers(CC,&svs);
        else
          { glue_pass=1;
            del_cell (CC);
            goto again;
          }
      }
   }

 }
 glsnap(db_pass,CC,"collected by glue");
 return CC;
}


cell * finpat(cell *BC, s_glue *GL, int16_t var, uchar flag, uchar pen)
 {
 cell  *WC, *GC;
 int16_t  nsc, n, p1;

 cell_to_svcell(BC);
 p1 = BC->vers[0].prob;
 p1-=pen;
 snap_newcell(BC);
 if ((var & 8) && language != LANG_RUSSIAN) // Valdemar
 {
   glsnap(db_pass,BC,"to cut accent");

   // В Литовском много нижних акцентов. 13.08.2001 E.P.
   if(language==LANG_LITHUANIAN ||
	   is_turkish_language(language)	// 18.06.2002 E.P.
	 )
	try_cut_accent(BC, &my_bases, 1);
   else
	try_cut_top_accent(BC, &my_bases, 1);

   if ( debug_try_cut.count == 1 )
      memset (&debug_try_cut, 0, sizeof(debug_try_cut));
   else
      { // Unmatched call
      debug_try_cut.row = debug_try_cut.col = 0;
      debug_try_cut.count = 0;
      }

   return BC;
 }
 glsnap(db_pass,BC,"to collect");
 nsc=crepat(BC,GL,var,flag);              // close and far cells to list
 if (nsc < 2)
 {
   glsnap('a',BC,"??? nothing collected ");
   return BC;
 }
 crecell(BC,GL,6);   // calculate final box, don't restore row, col, w, h
 GC=try_glue(GL,BC);
 if (GC && (GC->flg & (c_f_let|c_f_bad)))
 {
   /*********** restore very bad recog  ****/
   if (GC->vers[0].prob <= p1)  // idiotic attempt
   {
     cut_pos cposd;  // place of cut for 1st, 2nd points and dummy
     SVERS  sv0;
     s_glue GLM;
     svcell_to_cell(BC);
     GLM.flarg = GFcut;               // artifact
     GLM.arg   = GAtigr;
     estcomp('a',BC,&sv0,TRS2,0,0,&GLM,&cposd,&cposd,0,0,0);
     glsnap('a',GC,"Too bad estimate ");
     del_cell (GC);
     return BC;
   }
  /**********************/
   for (n=0; n < nsc; n++)
   {
     WC=GL->celist[n];
     del_cell(WC);
   }
   glsnap('a',GC,"cells delelted");
   return GC;
 }
 /***
 for (n=0; n < nsc; n++)
 {
   WC=GL->celist[n];
   if (WC != BC)
     del_cell(WC);
 }
 ****/
 compose_cell (nsc,GL->celist,BC);
// BC->cg_flag |= c_cg_noenv;
 glsnap('a',BC,"cell envelope corrected");
 return BC;
}

extern char dust_in_pattern;
static int16_t max_var;

static int16_t allow_comp(cell *B1, int16_t bnd1, int16_t bnd2, int16_t p0)
{
  uchar c1;
  if (p0 > bnd1) return 1;
  c1 = B1->vers[0].let;
  if ((p0 > bnd2) &&
      ((c1=='i') || (c1=='j') ||
      (language == LANG_RUSSIAN &&
		((c1==(uchar)'©') || (c1==(uchar)'‰'))
	  ) ||
      (c1==invers_exm) || (c1==invers_qm)))
    return 1;
  if (db_status && (db_trace_flag & 2))
    est_snap(db_pass,B1,"<= bnd - not to be 'best'");
  return 0;
}


void cmp_snap (cell *B1, char *txt, int16_t n, cell **clist)
{
  c_comp *my_list [8];
  if (n > 1)
  {
    int16_t i;
//    est_snap (db_pass, B1, "before");
    if (n >= 8) n=8;  // protect my_list
    for (i=0; i<n; i++)
      my_list[i] = clist[i]->env;
    env_to_show = compose_comp (n,my_list);
    row_to_show = env_to_show->upper;
    col_to_show = env_to_show->left;
    w_to_show = env_to_show->w;
    h_to_show = env_to_show->h;
  }
  est_snap (db_pass, B1, txt);
}


static int16_t rec_own(cell *B1,s_glue *GL,int16_t bnd1,int16_t bnd2, int16_t dupf,int16_t *disd)
 {
 int16_t  wn, cans, ans, ncl, n0, n1, n2, n3, n4, sarg, sff, sfs;
 uchar e, emax, svcg,clet;
 SVERS sv_v[5];
 char dip[5];
 uchar Var, Flg;
 c_comp *KITA;
 uchar let0, let1;	// 16.07.2001 E.P.

 Flg= GL->fres = Var= GL->var = 0;
 svcg=B1->cg_flag;
 max_var = -1;
 emax = (uchar)max_var;
 GL->maxval = 0;
 KITA = ( c_comp*) give_kit_addr();
 env_to_show = 0;
 sarg=GL->arg;
 sff=GL->flarg;
 sfs=sff & ~(GFbadr|GFbadl);

 n0=n1=n2=n3=n4=crepat(B1,GL,0,0);
 ncl=crecell(B1,GL,3);                     // make raster, box
 save_vers(B1,&(sv_v[0]));
 emax=B1->vers[0].prob;
 clet=B1->vers[0].let;
 if (ncl < 0)   { rest_vers(B1,&(sv_v[0])); e=emax; goto rst1; }
    if (db_status && (db_trace_flag & 2))
       est_snap(db_pass,B1,"cell itself");
 dmBOX(B1,GL);
 SBOX_to_static();
 dip[0]=0;			// no dust in pattern
  e=B1->vers[0].prob;
 if ((!e) && (emax))     // all killed
   { rest_vers(B1,&(sv_v[0])); e=emax; }
 else
   {
     wn = GL->maxnc = GL->ncell;
     memcpy(GL->maxlist,GL->celist,sizeof(cell*)*wn);
     save_vers(B1,&(sv_v[0]));
     emax=e;
     if (dupf > 1)
     { int16_t em;
       em=discrid(B1,dust_monus);
       if (dust_usage==0)       // upper dust near cell not used by self
       emax=(uchar)em;
     }
   }
rst1:
 max_var=0;
 Var=0; Flg=0;
 save_vers(B1,&svown);     // best "own" version
 if (sarg & GAbox)
  {
  if (dupf > 1)           // upper dust near cell
    if (dust_usage)       // best letter uses dust itself
      {
        if (db_status && (db_trace_flag & 2))
          est_snap(db_pass,B1,"ignore Dust");
        n1=2;
        goto boxbad;
      }      // so don't make a box with dust;
				   // make n1 as if var 1 done
  n1=n3=crepat(B1,GL,1,sfs);
  if (n1 > n0)
  {
    n1=crecell(B1,GL,3);  // calculate box  (in SBOX environmemt)
    if (n1 < 0)
      goto boxbad;
    if (db_status && (db_trace_flag & 2))
      cmp_snap(B1,"close Dust",n1,GL->celist);
    set_bad_cell(B1);
    dmBOX(B1,GL);
    save_vers(B1,&(sv_v[1]));
    e= B1->vers[0].prob;
    if ((e > emax) &&  allow_comp(B1, bnd1, bnd2, e))
    {
      *disd = 0;
      Var=1; Flg=(uchar)sfs; max_var=1; emax=e;
      SBOX_to_static();
      wn = GL->maxnc = GL->ncell;
      memcpy(GL->maxlist,GL->celist,sizeof(cell*)*wn);
      dip[1]=dust_in_pattern;  // was dust used in box ?
    }
  }
boxbad:
  n2=crepat(B1,GL,1,sff);
  if (n2 > n1)
   {
   n2=crecell(B1,GL,3);
   if (n2<0) goto dosur;
   if (db_status && (db_trace_flag & 2))
      cmp_snap(B1,"close Bad & Dust",n2,GL->celist);
//     est_snap(db_pass,B1,"close Bad & Dust");
   set_bad_cell(B1);
   dmBOX(B1,GL);
   save_vers(B1,&(sv_v[2]));
   e= B1->vers[0].prob;
   if ((dupf > 1) && (dust_in_pattern==0))   // upper dust near cell not used by self
       e=(uchar)discrid(B1,(uchar)dust_monus);
   if ((e > emax) &&  allow_comp(B1, bnd1, bnd2, e))
    {
    Var=1; Flg=(uchar)sff; max_var=2; emax=e;
    SBOX_to_static();
    wn = GL->maxnc = GL->ncell;
    memcpy(GL->maxlist,GL->celist,sizeof(cell*)*wn);
    dip[2]=dust_in_pattern;
    }
   }
  }
dosur:
 if (sarg & GAsur)
 {
   n3=crepat(B1,GL,2,sfs);
   if (n3 > n1)
   {
     n3=crecell(B1,GL,3);
     if (n3<0) goto surbad;
     if (db_status && (db_trace_flag & 2))
//       est_snap(db_pass,B1,"far Dust");
	cmp_snap(B1,"far Dust",n3,GL->celist);
     set_bad_cell(B1);
     dmBOX(B1,GL);
     save_vers(B1,&(sv_v[3]));
     e= B1->vers[0].prob;
     if ((dupf > 1) && (dust_in_pattern==0))   // upper dust near cell not used by self
       e=(uchar)discrid(B1,(uchar)dust_monus);
     if ((e > emax) &&  allow_comp(B1, bnd1, bnd2, e))
     {
       Var=2; Flg=(uchar)sfs; max_var=3; emax=e;
       SBOX_to_static();
       dip[3]=dust_in_pattern;
       wn = GL->maxnc = GL->ncell;
       memcpy(GL->maxlist,GL->celist,sizeof(cell*)*wn);
     }
   }
surbad:
   n4=crepat(B1,GL,2,sff);
   if (n4 > MAX(n2,n3))
   {
     n4=crecell(B1,GL,3);
     if (n4 < 0) goto fin;
     if (db_status && (db_trace_flag & 2))
//       est_snap(db_pass,B1,"far Bad & Dust");
       cmp_snap(B1,"far Bad & Dust",n4,GL->celist);
     set_bad_cell(B1);
     dmBOX(B1,GL);
     save_vers(B1,&(sv_v[4]));
     e= B1->vers[0].prob;
     if ((dupf > 1) && (dust_in_pattern==0))   // upper dust near cell not used by self
       e=(uchar)discrid(B1,(uchar)dust_monus);
     if ((e > emax) &&  allow_comp(B1, bnd1, bnd2, e))
     {
       Var=2; Flg=(uchar)sff; max_var=4; emax=e;
       SBOX_to_static();
       dip[4]=dust_in_pattern;
       wn = GL->maxnc = GL->ncell;
       memcpy(GL->maxlist,GL->celist,sizeof(cell*)*wn);
     }
   }
 }
fin:
 dens_to_cell(B1);
 B1->cg_flag = svcg;
 ans=B1->vers[0].prob;
 GL->maxval = emax;
 if (max_var < 0)          // no new box nor new vers
 {
    GL->var=0; GL->fres=0;
    if (dupf > 1)       // upper dust near cell
      ans=discrid(B1,dust_monus);
 }
 else
 {
   rest_vers(B1,&(sv_v[max_var]));
   GL->var  = Var;  GL->fres = Flg;
   static_to_SBOX();
   SBOX_to_GL(GL);
   ans=B1->vers[0].prob;
   if (dip[max_var]==0)    // dust not used in best box ?
     if (dupf > 1)       // upper dust near cell
       ans=discrid(B1,dust_monus);
  }

  let0 = B1->vers[0].let;
  let1 = B1->vers[1].let;

  if (
       (ans < 220 ||
        language == LANG_POLISH &&
            ( let0==z_dot_accent  &&
                    ( let1==z_right_accent || let1=='2') ||
              let1==z_dot_accent  &&
                    ( let0==z_right_accent || let0=='2') ||
              let0==ZZ_dot_accent &&
                    ( let1==ZZ_right_accent || let1=='2') ||
              let1==ZZ_dot_accent &&
                    ( let0==ZZ_right_accent || let0=='2')
            ) ||
         language == LANG_CROATIAN &&
            ( let0==CROAT_cr  && let1==CROAT_ca  ||
              let0==CROAT_CR  && let1==CROAT_CA  ||
              let1==CROAT_cr  && let0==CROAT_ca  ||
              let1==CROAT_CR  && let0==CROAT_CA
            ) ||
		 // 05.09.2000 E.P.
         language == LANG_CZECH &&
            ( let0==c_right_accent  && let1==c_inv_roof  ||
              let0==CC_right_accent && let1==CC_inv_roof ||
              let1==c_right_accent  && let0==c_inv_roof  ||
              let1==CC_right_accent && let0==CC_inv_roof
			  ||
              let0==e_right_accent  && let1==e_inv_roof  ||
              let0==EE_right_accent && let1==EE_inv_roof ||
              let1==e_right_accent  && let0==e_inv_roof  ||
              let1==EE_right_accent && let0==EE_inv_roof
            ) ||
		 // 05.09.2000 E.P.
         language == LANG_ROMAN &&
            ( let0==a_semicircle    && let1==a_roof_accent||
              let0==AA_semicircle   && let1==AA_roof_accent||
              let0==a_roof_accent   && let1==a_semicircle ||
              let0==AA_roof_accent  && let1==AA_semicircle
            ) ||
		 // 05.09.2000 E.P.
         language == LANG_HUNGAR &&
            ( let0==o_2dot_accent   && let1==o_double_right||
              let0==OO_2dot_accent  && let1==OO_double_right||
              let0==o_double_right  && let1==o_2dot_accent ||
              let0==OO_double_right && let1==OO_2dot_accent
            ) ||

		 // 16.07.2001 E.P.
         language == LANG_LATVIAN &&
            ( let0==II_macron		&& let1=='I'			||
              let0=='I'			    && let1==II_macron		||
              let0==i_macron		&& let1==liga_i			||
              let0==liga_i			&& let1==i_macron
            ) ||

         language == LANG_ESTONIAN &&
            (
              let0==OO_2dot_accent  && let1==OO_tild_accent	||
              let0==OO_tild_accent  && let1==OO_2dot_accent	||
              let0==o_2dot_accent  && let1==o_tild_accent	||
              let0==o_tild_accent  && let1==o_2dot_accent
            ) ||

         language == LANG_TURKISH &&
            (
              let0==II_dot_accent  && let1=='I'	||
              let0=='I' && let1==II_dot_accent	||
              let0==i_sans_accent  && let1=='i'	||
              let0=='i'  && let1==i_sans_accent
            ) ||

		 0
       )  &&
       (language != LANG_ENGLISH && language != LANG_RUSSIAN)
     )
  {
    if (debug_try_cut.count)
      { // Unmatched call
      debug_try_cut.row = debug_try_cut.col = 0;
      debug_try_cut.count = 0;
      }

   // В Литовском много нижних акцентов. 13.08.2001 E.P.
   if(language==LANG_LITHUANIAN ||
	   is_turkish_language(language)	// 18.06.2002 E.P.
	 )
    cans = try_cut_accent(B1,&my_bases,0);
   else
    cans = try_cut_top_accent(B1,&my_bases,0);

    // brings 0 for not an accentted letter
    if (cans > 220)
        {
        ans = cans;
        GL->var = 8;
        debug_try_cut.row = B1->row;
        debug_try_cut.col = B1->col;
        debug_try_cut.count++;
        GL->fres = 0;
        }
  }
  if (db_status && (db_trace_flag & 2))
  {  char b[32];
     sprintf(b,"best var %d",max_var);
     est_snap(db_pass,B1,b);
  }
  take_kit_addr((void*)KITA);
  env_to_show = 0;
  return ans;
}

static char ms220ready[]={"m > 220 --> ready"};
static char msw3ready[]={"w <= 3  --> ready"};
static char msf170bef[]={"f supports i,j"};
static char msnat220[]={"native >220 --> ready"};
static char ms29500[]={"stick > 29500"};
static char msingl[]={"single nondiscrim ready"};

int16_t estcomp(char user, cell *B1, SVERS *save, int16_t trs, int16_t bnd1, int16_t bnd2,
    s_glue *GL, cut_pos *cpos1, cut_pos *cpos2, char pnt1 ,char pnt2, char iv)
 {
 uchar c, c_sacc, ct, svarg;
 int16_t flag_m, flg_own, disd;
 uchar  p1, pans, dup;
 char wrk[32];
 char *pmsg;
 cut_pos *wcp;   // to cover dummy arg warning;
 extern int16_t best_answer_BOX;

 get_b_lines(B1,&my_bases);
 bs12=(my_bases.b1+my_bases.b2)/2;
 bs2m=(my_bases.b2+my_bases.bm)/2;
 bsm3=(my_bases.bm+my_bases.b3)/2;
 bs34=(my_bases.b3+my_bases.b4)/2;
 dust_monus = 0;
 dust_mon_flag = 0;
 log_s = log_str;
 wcp=cpos1;
 wcp=cpos2;
 flg_own = 1;   // self-made value (no 5-box estimate)
 dup = 0;
 disd=0;

 if(pass4_in) GL->arg |= GABOXR; // Valdemar 02-09-96 07:43pm

 dup = (uchar)decidust(B1);
 GL->var=0;
 if (GL->arg & GAtigr)                    // request for full estimation
 {   recog_cell(B1);
  if (db_status && (db_trace_flag & 2))
    est_snap(db_pass,B1,"events");
 }
 c = B1->vers[0].let;
 p1= B1->vers[0].prob;


//   send to BOX: bad patterns;
//                miltiple version;
//                all 'm' letters   (m, n, u)
//                all cutten letters with simple structure

 if ((B1->flg & c_f_let) && (B1->nvers==1))
 {
 uchar let = B1->vers[0].let;
   if (
		language!=LANG_ENGLISH &&
		language!=LANG_CROATIAN && language!=LANG_RUSSIAN&&
			memchr ("aoeu",let,4)||
  language == LANG_RUSSIAN && memchr ("Ґ…",let,2)||
  language == LANG_POLISH && memchr ("SCZNOsczno",let,10) ||

  // 05.09.2000 E.P.
  language==LANG_CZECH	&& memchr("AaCcDdEeIiNnOoRrSsTtUuYyZz",let,26) ||
  language==LANG_ROMAN  && memchr("AaIi",let,4) ||
  language==LANG_HUNGAR && memchr("AaEeIiOoUu",let,10) ||
  language==LANG_SLOVENIAN && memchr("CcSsZz",let,6) ||
  // 09.07.2001 E.P.
  language==LANG_LATVIAN    && memchr("AaCcEeGgIiKkLlNnSsUuZz",let,22) ||
  language==LANG_LITHUANIAN && memchr("AaCcEeIiSsUuZz",let,14) ||
  language==LANG_ESTONIAN   && memchr("AaOoSsUuZz",let,10) ||

  // 21.05.2002 E.P.
  language==LANG_TURKISH &&
	(memchr("AaCcIiGgOoSsUu",let,14) || let == i_sans_accent) ||
  0
  )

      { accent(B1);
	if (B1->nvers != 1)
	  goto  not_a_single;
      }
   save_vers(B1,&svown);   // save the whole vector
   abris_reset();
   GL->celist[0]=B1; GL->celist[1]=NULL; GL->ncell=1;

   // 'm' is reliable in events; just test against legs

   flag_m=0;
   c_sacc = let_sans_acc[c];
   if (c_sacc=='m') flag_m=1;	// 'm'
   if (c_sacc=='n') flag_m=2;	// 'n'
   if (c_sacc==liga_ri) flag_m=2;    // "ri"
   if (c_sacc=='u') flag_m=4;	// 'u'
   if (flag_m)
   {
     madeBOX=1;
     atlant(B1,GL,3,flag_m);
     p1=B1->vers[0].prob;
     if (db_status && (db_trace_flag & 2))
       est_snap(db_pass,B1,"atlant");
     if (c_sacc != 'm')
     {
       rest_vers(B1,&svown);              // native u,n,rt -
       goto deciBOX;
     }
     if (p1 >= trs)           // legs OK - ready answer if 'm',
     {
       B1->flg &= ~c_f_bad;  B1->flg |= c_f_let;
       save_vers(B1,&svown);
       pans = p1;
       pmsg = ms220ready;
       goto test_BOX_request;
     }
     //  'm'-legs NOT OK
     B1->flg &= ~c_f_let; B1->flg |= c_f_bad;
     save_vers(B1,&svown);
     pans = p1;
     pmsg = NULL;
     goto test_BOX_request;
   }

   if (B1->w <= 3)
   {
     pans=B1->vers[0].prob;
     pmsg = msw3ready;
     goto test_BOX_request;
   }
   if  (c==liga_rt)    // 'rt'
   { p1 -= 82; if (p1 < 2) p1= 2; B1->vers[0].prob=p1; goto argBOX_ready; }
   if  ((c=='h') || (c=='b'))
   { p1=(uchar)h_filt(B1,p1,GL,c);  goto deciBOX; }
      c_sacc = let_sans_acc[c];
   if ( (language!=LANG_CROATIAN && memchr("sSaoO0QGDMNHURdxq6<>cCkwWBEA",c_sacc,28)) ||
        !B1->accent_leader &&
			(
			language == LANG_CROATIAN && memchr ("scz",c_sacc,3)    ||
			language == LANG_POLISH   && memchr ("sczno",c_sacc,5)  ||
		   // 05.09.2000 E.P.
		    language==LANG_CZECH	  && memchr("AaCcDdEeIiNnOoRrSsTtUuYyZz",let,26) ||
		    language==LANG_ROMAN      && memchr("AaIi",let,4) ||
		    language==LANG_HUNGAR     && memchr("AaEeIiOoUu",let,10) ||
		    language==LANG_SLOVENIAN  && memchr("CcSsZz",let,6) ||

			// 09.07.2001 E.P.
			language==LANG_LATVIAN    && memchr("AaCcEeGgIiKkLlNnSsUuZz",let,22) ||
			language==LANG_LITHUANIAN && memchr("AaCcEeIiSsUuZz",let,14) ||
			language==LANG_ESTONIAN   && memchr("AaOoSsUuZz",let,10) ||

		    // 21.05.2002 E.P.
		    language==LANG_TURKISH &&
				(memchr("AaCcIiGgOoSsUu",let,14) ||
				let == i_sans_accent
				) ||
			0
			) ||
        ( c_sacc == ss_deaf_sound && language != LANG_RUSSIAN) ) // Vademar 2.2.93
    {
    p1=(uchar)abris(GL,B1,c_sacc,p1);
    goto deciBOX;
    }

   if ( memchr("rtfTJ()<>[]LI1il!/F7в’Јѓш|",c,26) &&
		!is_russian_baltic_conflict(c) &&	// 17.07.2001 E.P.
		!is_russian_turkish_conflict(c)		// 21.05.2002 E.P.

	   ||   // 07.01.1993 (see S_TOOLS.C)
      ( language == LANG_CROATIAN && (c==CROAT_d||c=='d') )  ||
      ( language == LANG_POLISH   && (c== POLISH_l || c=='l') )  ||
      ( language != LANG_RUSSIAN  &&
		   (c==liga_i ||  c==liga_j  ||

		// Конфликтные коды 07.09.2000 E.P.
		!is_baltic_language(language) &&
		!is_turkish_language(language) &&
	    !is_cen_language(language) && (
			c==i_left_accent ||
		    c==i_2dot_accent ||
		    c==II_left_accent ||
		    c==II_2dot_accent
		) ||
		   c==i_right_accent  ||
		   c==i_roof_accent   ||
		   c==II_right_accent ||
		   c==II_roof_accent  )
	  ) ||

		// 05.09.2000 E.P.
		language==LANG_CZECH && (c==d_inv_roof || c==t_inv_roof) ||
		language==LANG_ROMAN && c==t_bottom_accent ||

		is_baltic_palka(c) ||	// Балтийские палки. 10.07.2001 E.P.
		is_turkish_palka(c)		// Турецкие палки. 21.05.2002 E.P.
		)
   {
     stick_center_study(B1,GL,1);      // Oleg : 07.08.92.
     p1 = B1->vers[0].prob;
   deciBOX:
     if (db_status && (db_trace_flag & 2))
     {
       sprintf (wrk,"single discrim; p=%d", p1);
       est_snap(db_pass,B1,wrk);
     }
     if (p1 <= 2) p1=2;
     if (p1 < trs)        // single version to be cut
     {
       if ((c=='f') && (p1 >= 170))
       { cell *b1n;
         ct=(b1n=B1->nextl)->vers[0].let;
         if (((ct=='i') || (ct=='j')) /*****  && (b1n->cg_flag & c_cg_rqdot) ****/)
         {
           pans=B1->vers[0].prob;   // weak 'f' provides dot to i,j
           pmsg = msf170bef;
           goto test_BOX_request;
         }
       }
       if (p1 < 170)             // BOX in full ?
       {
         B1->vers[0].prob=0;
         B1->nvers = 0;
         B1->flg = c_f_bad;
         if (db_status && (db_trace_flag & 2))
           est_snap(db_pass,B1,"p<170 to BOX FULL");
         goto estiBOX;
       }
       // not solid, but not too bad  (170 -- trs)
       rest_vers(B1,&svown);
    argBOX_ready:
       if (db_status && (db_trace_flag & 2))
         est_snap(db_pass,B1,"170 < p1 < 220 --> to BOXes");
       goto estiBOX;
     }
     // reliable single >= trs after all discrim.
     if (GL->flarg & GFcut)
     {
       rest_vers(B1,&svown);
 // PERSONAL DISCRIMINATIONS: unreliable cutten letters
       if (c==liga_rt)    // 'rt'
       {  p1-=50; if (p1<2) p1=2; B1->vers[0].prob = p1; }
       if (db_status && (db_trace_flag & 2))
         est_snap(db_pass,B1,"cutten p>220 --> to BOXes");
       goto estiBOX;        // artifact - estimate by BOX with events' value
     }
     B1->vers[0].prob=p1;
     save_vers(B1,&svown);
     pans=p1;
     pmsg = msnat220;
     goto test_BOX_request;
   }  // a stick;

   // cutten letter (stick & others) --> BOX
   if (GL->flarg & GFcut)
   {
      if (db_status && (db_trace_flag & 2))
        est_snap(db_pass,B1,"CUT/GLUE to BOX");
      goto estiBOX;    // cutten version - estimate by BOX
   }
	 if (memchr("1lI()[]{}!",c,10) || c==liga_i ||
		   language==LANG_TURKISH &&  // 30.05.2002 E.P.
			(c==i_sans_accent||c==II_dot_accent)
			||
		 c==liga_exm
		)
    if (signif_line_count(B1) == 1)
    {
     svarg=GL->arg;
     if (db_status && (db_trace_flag & 2))
       est_snap(db_pass,B1,"stick, 1 big line -- test 29500");
     GL->arg |= GABOXs;         // direct request
     crepat(B1,GL,0,0);
     crecell(B1,GL,3);         // make raster, box
     dmBOX(B1,GL);
     SBOX_to_static();
     SBOX_to_GL(GL);
     GL->arg=svarg;
     if (best_answer_BOX >= 29500)  // original stick supported by BOX
     {
       rest_vers(B1,&svown);      // retain events' value
       pans=B1->vers[0].prob;
       pmsg = ms29500;
       goto test_BOX_request;
     }
     // weak stick
     B1->flg = c_f_bad;  B1->vers[0].prob=0;  B1->vers[0].let=bad_char;
     if (db_status && (db_trace_flag & 2))
       est_snap(db_pass,B1,"p < 29500 - BOX FULL");
     goto estiBOX;
   }
   // usual native letter (unknown as to be tested against some discrim. rule
   pans=B1->vers[0].prob;
   pmsg = msingl;
 test_BOX_request:
   if (pmsg && db_status && (db_trace_flag & 2))
     est_snap(db_pass,B1,pmsg);
 if ((GL->arg & GABOXR) == 0)
   goto retp;  // no direct request (for boxes ?)
 }  // let & single
 // not a single version
 not_a_single:
 if ((B1->flg & c_f_bad)      ||        // bad answer
      (B1->nvers > 1)         ||        // multiple versions
      (GL->arg & GABOXR)                // BOX explicitly requested
    )
   {
 estiBOX:
    madeBOX=1;
    if (db_status && (db_trace_flag & 2))
      est_snap(db_pass,B1,"to make all BOXes");
    flg_own = 0;   // estimated by 5-box
    if( !( B1->accent_leader &&
			( is_cen_language(language)    || // 05.09.2000 E.P.
			  is_baltic_language(language) || // 09.07.2001 E.P.
			  is_turkish_language(language)	  // 21.05.2002 E.P.
			)
		 )
	  )
      pans=p1=(uchar)rec_own(B1,GL,bnd1,bnd2,dup,&disd);
    else
      {
      pans=p1=B1->vers[0].let;
      save_vers(B1,&svown);   // save the whole vector
      }
    if (p1 >= trs)
      { B1->flg &= ~c_f_bad;  B1->flg |= c_f_let; }
    else
      { B1->flg &= ~c_f_let; B1->flg |= c_f_bad; }
    if (svown.vers[0].prob >= trs)
      { svown.flg &= ~c_f_bad;  svown.flg |= c_f_let; }
    else
      { svown.flg &= ~c_f_let; svown.flg |= c_f_bad; }
    }
  else
    {
     pans=p1=B1->vers[0].prob;
     if (p1 >= trs)
       { B1->flg &= ~c_f_bad;  B1->flg |= c_f_let; }
     else
       { B1->flg &= ~c_f_let; B1->flg |= c_f_bad; }
     save_vers(B1,&svown);
    }
retp:
 save_vers(B1,save);
 if (db_status)
 {
  if (user=='a')
    sprintf (wch,"(%d,%d) v=%d ",pnt1,pnt2,iv);
  else
    glsnap('b',B1,"");
 }
 if (flg_own && (dup>1))  /* upper dust near letter */
 {
   dust_mon_flag = 1;
   pans=(uchar)discrid(B1,dust_monus);
   pans &= 0xfe ;
   if (db_status && (db_trace_flag & 2))
   {
      sprintf(wrk,"%d: -%d for upper dust",pans,dust_monus);
      est_snap(db_pass,B1,wrk);
   }
 }
 return pans;
}

static uchar thick_crit[33] =
{ 4,4,6,6,8,8,10,10,12,12,16,16,18,18,20,20,
  22,22,24,24,26,26,28,28,30,30,30,30,30,30,30,32,32};

void   croat_reverse(cell *B1,uchar c)
{
/*
	Теперь вызывается для всех центральноевропейских языков 05.09.2000 E.P.

	Здесь есть ошибка: если в model повторяются буквы, то memchr найдет только
	первое вхождение. См. ниже функцию baltic_turkish_reverse(), где эта ошибка изжита.
	16.07.2001 - 21.05.2002 E.P.
*/

uchar model[]=  "sScCzZcCoOzZsSoOoO";
uchar croat[]={CROAT_SR,  CROAT_sr,  CROAT_CA,  CROAT_ca,
              CROAT_ZR,  CROAT_zr,  CROAT_CR,  CROAT_cr,
			  OO_right_accent, o_right_accent, // 05.09.2000 E.P.
			  ZZ_right_accent, z_right_accent, // 05.09.2000 E.P.
			  SS_right_accent, s_right_accent, // 05.09.2000 E.P.
			  OO_double_right, o_double_right, // 05.09.2000 E.P.
			  OO_2dot_accent,  o_2dot_accent   // 05.09.2000 E.P.
			 };
uchar rever[]={CROAT_sr,  CROAT_SR,  CROAT_ca,  CROAT_CA,
              CROAT_zr,  CROAT_ZR,  CROAT_cr,  CROAT_CR,
			  o_right_accent, OO_right_accent, // 05.09.2000 E.P.
			  z_right_accent, ZZ_right_accent, // 05.09.2000 E.P.
			  s_right_accent, SS_right_accent, // 05.09.2000 E.P.
			  o_double_right, OO_double_right, // 05.09.2000 E.P.
			  o_2dot_accent,  OO_2dot_accent   // 05.09.2000 E.P.
			 };
uchar *p;
int16_t  i1, i2;
if(  !B1->nvers )
  return;
p = memchr(croat,B1->vers[0].let,sizeof(croat)/sizeof(croat[0]) ) ;
if( !p )
  return;
i1 = p-croat;
p = memchr(model,c,sizeof(model)/sizeof(model[0]) ) ;
if( !p )
  return;
if ( db_status && (db_trace_flag & 4) &&  snap_activity(db_pass) )
  {
  snap_show_text("Kill linear identical accent");
  snap_monitor();
  }
i2 = p-model;
if( i1==i2 )
  {
  del_version( B1, rever[i1] );
  B1->vers[0].let = rever[i2];
  }

return;
}

void   baltic_turkish_reverse(cell *B1,uchar c)
{
/*
	Исключает линейно идентичную версию с акцентом.
	Приводит к регистру, заданному вторым параметром.

	Вызывается для прибалтийских языков 16.07.2001 E.P.
	А также для турецкого 21.05.2002 E.P.
*/
uchar baltic_letters[]={
	CC_inv_roof, c_inv_roof,
	II_macron, i_macron,
	II_bottom_accent, i_bottom_accent,
	KK_bottom_accent, k_bottom_accent,
	OO_tild_accent, o_tild_accent,
	OO_2dot_accent,  o_2dot_accent,
	SS_inv_roof_baltic, s_inv_roof_baltic,
	UU_2dot_accent, u_2dot_accent,
	UU_bottom_accent, u_bottom_accent,
	UU_macron, u_macron,
	ZZ_inv_roof_baltic, z_inv_roof_baltic,
	0};

uchar turkish_letters[]={
	CC_bottom_accent, c_bottom_accent,
	II_roof_accent, i_roof_accent,
	'I', i_sans_accent,
	II_dot_accent, 'i',
	OO_2dot_accent,  o_2dot_accent,
	SS_bottom_accent, s_bottom_accent,
	UU_roof_accent, u_roof_accent,
	UU_2dot_accent, u_2dot_accent,
	0};

uchar *p;
int16_t  i1, i2;
uchar c1, c2, c3, c4;
uchar *letters = 0;

if (is_baltic_language(language))
	letters = baltic_letters;
else if (is_turkish_language(language))
	letters = turkish_letters;
else return;

if(  !B1->nvers )
  return;
p = strchr(letters,B1->vers[0].let) ;
if( !p )
  return;
i1 = p-letters;

// Парная буква в списке
if (i1&1)
	i2 = i1-1;
else
	i2 = i1+1;

// Какой символ задан
c1 = letters[i1];
c2 = letters[i2];

// Какую версию удалить и какую оставить
if (c==let_sans_acc[c1])
	{
	c3 = c2;	// Удалить
	c4 = c1;	// Оставить
	}
else if (c==let_sans_acc[c2])
	{
	c3 = c1;	// Удалить
	c4 = c2;	// Оставить
	}
else
	return;

if ( db_status && (db_trace_flag & 4) &&  snap_activity(db_pass) )
  {
  snap_show_text("Kill linear identical accent");
  snap_monitor();
  }

del_version(B1, c3);
B1->vers[0].let = c4;

return;
}

void make_all_cuts()
//
//	This procedure organizes the process of decision making.
//	Namely it is responsible to make post recognition.
//	ON ENTRY: start_cell is the address of the beginning of
//		  string of components.
//	ON EXIT : better recognized string.
//
//
 {
#ifdef NEW_TM        //10.4.95
 extern int16_t TM_check_active;
#endif

 void  *CK1, *KITA, *KITA1;
 all_cells CL;
 cell  PROTO ;
 cell  *B1, *I1, *I3, *T1, *T2, *T3, *TD, *NL;
 char n1, n2, nc;
 uchar left_dust_allowed, left_dust_seen, fldust, svcg, fljust;
 int16_t  av_dens, ndens, gv0, gf0, N, mw, mh, mrr, mrc, i, rpn;
 uchar c0, c1, c3, *rp1, *rp2, already_cut;
 uchar acc_p;
 uint16_t fo1, fo2, fo3, sp0;
 int16_t  p0, p1, p2, p3;
 v_val pe;
 cut_pos cpos1, cpos2, cposd, cposc;  // place of cut for 1st, 2nd points and dummy
 char   doubles_allowed,  double_cutsn;
 char bf_d[180], *bf_p;
 SVERS  sv0, sv1, sv2, sv3;
 uchar   csv1[32], csv2[32];               // savearea for cutten bits
 s_glue GL;
 struct cut_elm  *cpnt0, *cpntw, *cpnt1, *cpnt2;
 uchar best_vars[64], best_flags[64];
 cell *done_cells[64];
 int16_t done_num, done_ind;
 MN *mn1;
 int16_t num_shaves=0;       // Oleg : use pimples shaving

 flag_cut_point=0;

 Z=&string;
 cpnt0 = cut_pnts;
 my_cut_points = cut_pnts;
 cposd.flag=8;  // dummy cut position
 snap_newpass('a');
 B1=cell_f()->next; i=0;
 while (B1->next)
 {
   if ( B1->flg & c_f_space )
     B1->col=i;
   else
   {
     mrc=B1->col+B1->w;
     i=MAX(i,mrc);
   }
   B1=B1->next;
 }
 B1=cell_f();
 B1=B1->nextl;
 while (1)
 {
   B1->cg_flag &= ~c_cg_cutdone;
   if (B1->flg & c_f_let)
   {
     if (!B1->nvers)
       p2=0;
     else
       p2 = B1->vers[0].prob;
     if (p2 < TRS1 && !(B1->flg & c_f_solid))   // weak chars treat as bad
       B1->flg ^= (c_f_bad+c_f_let);
   }
   B1=B1->nextl;
   if (!B1)
     break;
 }
 for(B1=cell_f()->next; B1 != cell_l();B1=B1->next)
    if(B1->flg & (c_f_solid|c_f_space))
      B1->cg_flag = c_cg_cutdone+c_cg_noglue; // Vald don't touch this cell
 B1=cell_f();

 Ldb_flag = 0;

forwb:;
 if(flag_cut_point)  { flag_cut_point=0; db_status=1;}

 if (!B1) goto fin;
 if (!B1->next) goto fin;
 B1 = B1->nextl;
 if (!B1)  goto fin;
 if (B1->nextl == NULL) goto fin;
 if (B1->cg_flag & c_cg_cutdone) goto forwb;
 KITA=give_kit_addr();
 B1->cg_flag |= c_cg_cutdone;
 I1=B1->prevl;  I3=B1->nextl;	   // where to insert
 svcg=B1->cg_flag;
 if (!tsimple(B1))
   goto forwb;
 get_b_lines(B1,&my_bases);
 snap_newcell(B1);
 GL.flarg = GFbadr + GFbadl;
 GL.arg = GAallest;
 PROTO = *B1; // Oleg : saving rotate information
 if ((B1->w) > (B1->h*3))
 {
   p0=0; c0=0; gv0=0; gf0=0;
   set_bad_cell(B1);
   save_vers(B1,&sv00);
   goto init_est_done;
 }
 else
 {
   if ((B1->nvers==0) && (B1->env && (B1->env->nl==1) || 3*B1->w<=B1->h))
   {
     if (db_status) glsnap('a',B1,"bring sticks");
     stick_center_study(B1,NULL,1);      // AL, IF : 94.01.07
   }
   madeBOX=0;
   wch[1]=wch[0]=0;  // clear out line
   if (db_status)
   { char bb[64];
     sprintf (bb,"to estimate; bs= %d %d %d %d, Ps=%d, nb= %d %d %d %d",
       my_bases.b1, my_bases.b2, my_bases.b3, my_bases.b4,
       my_bases.ps,
       my_bases.n1, my_bases.n2, my_bases.n3, my_bases.n4);
     glsnap('a',B1,bb);
   }

   try_cut_bot_accent(B1,&my_bases,0);

   // 940329 p0=estcomp('a',B1,&sv0,TRS2,190,100,&GL,&cposd,&cposd,0,0,0);
   p0=estcomp('a',B1,&sv0,TRS2,170,100,&GL,&cposd,&cposd,0,0,0);
   memcpy(&sv00,&svown,sizeof(sv00));
   if (dust_mon_flag)
     lsnap('a',B1,"upper dust ",p0);
   else
     lsnap('a',B1," ",p0);
   c0 = B1->vers[0].let;
   acc_p = B1->vers[0].prob;
   gv0=GL.var & 15; gf0=GL.fres;
 }
init_est_done:
 if ((p0 >= TRS2) || (forest(B1,p0,TRSO1)))  // forbidden to cut reliable 0oOQ
 {

 tryfin:

   if (gv0 > 0)   // best version was with surrounding cells
     {
     rest_vers(B1,&sv0);
     B1=finpat(B1,&GL,gv0,(uchar)gf0,32);
     // Nick&Oleg : 290699 : can be init of accent! for pass2
     }
   else
     rest_vers(B1,&sv00);
   goto forwb;
 }

 if (madeBOX==0)
 {
   GL.flarg = GFbadr + GFbadl;
   GL.arg = GAallest + GABOXR;
   p0=estcomp('a',B1,&sv0,TRS2,190,100,&GL,&cposd,&cposd,0,0,0);

   memcpy(&sv00,&svown,sizeof(sv00));
   if (dust_mon_flag)
     lsnap('a',B1,"upper dust ",p0);
   else
     lsnap('a',B1," ",p0);
   gv0=GL.var; gf0=0;
 }
 glob_diff = B1->bdiff;
 glob_dflg = B1->difflg & 0xf0;
 mh=B1->h; mw=B1->w; mrr=B1->r_row; mrc=B1->r_col;
 if (mw  > 128)                         //  Can't cut large things
  {
	 glsnap('a',B1,"large");
	 goto forwb;
  }
 glsnap('a',B1,"start cut");
 I1=B1->prevl;
 I3=B1->nextl;
 //AK! see 16 line down (N=Alik_cut...)
 // from these jump to followed loop and jpf!
 if (cpnt0)
   for (cpnt1=cpnt0,n1=0; n1<STK_H; n1++,cpnt1++)
    {
     cpnt1->px=0;
     cpnt1->lv.v1=cpnt1->lv.v2=cpnt1->lv.v3=-1;
     cpnt1->var=0;
     cpnt1->x=127;
     cpnt1->duflm=cpnt1->duflr=cpnt1->gvarr=cpnt1->gvarm=(char)0;
    }

 rp1=(uchar*)save_raster(B1);

 if( !cuts_point_methode )
      N=cut_points(mw, mh, rp1, cpnt0);
 else                                   //AK attention
      N=Alik_cut_points(mw, mh, rp1, cpnt0,B1->row);
 switch (N)
 {
   case 0:
     glsnap('a',B1,"no points");
     goto tryfin;
   case -1:
     if( !(B1->flg_new&c_fn_shave) )
     if( num_shaves==0 ) // Oleg : use pimples shaving
      {
      num_shaves=1;
      if( pimpleshave(B1,1,0) )
        {   // pimples deleted
        if( B1->cg_flag&c_cg_cutdone )
            B1->cg_flag-=c_cg_cutdone;
        B1->flg_new|=c_fn_shave;
        if( B1->prev )
          B1 = B1->prevl;
        else
          B1 = cell_f();

        goto forwb;
        }
      }
     glsnap('a',B1,"too much lines");
     goto tryfin;
 }

  //ALIK 10-09-95
 if (snap_activity('a'))
   {
    if(flag_ShiftF9)   { flag_ShiftF9=0;  db_status=1; }
    show_cut_points=1;
    flag_cut_point=0;
   }

 glsnap('a',B1,"show cut points");
 if (snap_activity('a'))
    {
    snap_show_cuts(B1, cpnt0);
    }

 num_shaves=0; // Oleg : use pimples shaving
 N &= 255;
 rp2=(char *)&cell_raster;
 rpn=((mw+7)>>3)*mh;
 for (i=0; i<rpn; i++)
   *(rp2++) = *(rp1++);
 save_vers(B1,&(cpnt0->versm));
 CK1=del_save_cell(B1);                   // cell to kit
 cpnt0->lv.v3=p0;
 cpnt0->lv.v2=254;
 cpnt0->lv.v1=254;
 cpnt0->px=0;   			  // no cut for this value
 av_dens=ndens=0;
 left_dust_allowed = 0;
repeat_with_left_dust:
 doubles_allowed=0;                      // no full cuts
repeat_with_doubles:
 double_cutsn=0;
 left_dust_seen = 0;
 for (n1=1,cpnt1=cpnt0+1; n1<=N; n1++,cpnt1++)
 {
   already_cut = 0;

   if (cpnt1->var & 8)
   {
     double_cutsn++;                                // register doublecuts
     if (!doubles_allowed) continue;                   // 1st pass - avoid doublecuts
   }

   if(flag_cut_point && cpnt1->x >= StopPoint)       //Alik 16.10.95
         {
          flag_cut_point=0;
          db_status=1;
         }


   fo1=fo2=fo3=0;
   p1=p2=p3=0;
   KITA1=give_kit_addr();
 cut_full:
   cut_rast(cell_raster,mw,mh,mrr,mrc,cpnt0,n1,1,csv1,&cpos1);  // make cut
   cpos1.row1 = cpos1.row1 - B1->r_row + B1->row;
   cpos1.row2 = cpos1.row2 - B1->r_row + B1->row;
   if ((cpnt1->lv.v3 >= 0) || (cpnt1->lv.v2 >= 0))    // non-double point already estimated
   /////////// ????????????   VERY strange condition
   {
     fo1=cpnt1->duflm & GRflo;
     fo3=cpnt1->duflr & GRflo;
     p3=cpnt1->rv.v3;                       // right part estimate
     p1=cpnt1->lv.v3;                       // single cut estimate
     goto nocut1;
   }
   mn1=cut_rast(cell_raster,mw,mh,mrr,mrc,cpnt0,n1,3,csv1,&cposc); // create components
   if (mn1==NULL) { glsnap('a',I1,"locomp"); goto next1; }
   if (mn1->mnnext == NULL)
   {
     if (cpnt1->dh == B1->h)
       goto next1;
     cut_rast(cell_raster,mw,mh,mrr,mrc,cpnt0,n1,0,csv1,&cposc);    // glue back
     cpnt1->h=cpnt1->dh=(char)B1->h;
     cpnt1->var |= 32;  // mark as full
     goto cut_full;
   }
   fldust=0;
   nc=(char)colcels(&CL,mn1,I1,I3,mw);
   TD=I1->nextl;    // density-test cell
   T1=CL.minlet;
   T3=CL.maxlet;
   if (nc != 2)
   {
     if (db_status && (db_trace_flag & 2))
     {
       int16_t nc; cell *X;
       est_snap(db_pass,CL.mincl,"1st cut: not 2 large comps got");
       for (nc=0; nc < CL.Nb; nc++)
       {
         X=CL.bcells[nc];
         est_snap(db_pass,X,"large");
       }
       for (nc=0; nc < CL.Nd; nc++)
       {
         X=CL.dcells[nc];
         est_snap(db_pass,X,"small");
       }
     }
   }
   if ((nc == 0) || (nc > 2)) goto reject_1; // no letters generated
   if (nc == 2)                              // 2 comps generated
   {
     if (CL.flag & cl_disorder)   // left cell at right side to right cell
       { reject_1: delcels(&CL); goto next1; }
   }
   else
   { /* nc==1 */                               // one letter, one dust
     //if (CL.minb >= CL.mind)                 // dust at left side
     if (CL.flag & dust_left)                 // dust at left side
     {
       if (CL.Nd > 1)                         // 2 or more  DUSTs FORBIDDEN
       {
       reject_dust:
          delcels(&CL); goto next1;
       }
       if (CL.Nd==0)
       {
       accept_dust:
          fldust |= 1;
          // T3=I1->nextl;
       }
       else                                   // 1 dust cell
       {
         if  (!CL.Nd) goto reject_dust;
         if (is_defis(CL.dcells[0]))
         {
           fldust |= 4;
           goto accept_dust;
         }
         if (CL.dcells[0]->row > bs12)  // not upper left dust
          { // Oleg : in diagital mode enable cut left bottom commas
          if( is_digital_string() && is_comma(CL.dcells[0], my_bases.b3) )
            {
            fldust |= 4;
            goto accept_dust;
            }
          else
            goto reject_dust;
          }
         left_dust_seen = 1;            // allow upper dust at 3rd pass
         if (left_dust_allowed)
           goto accept_dust;
         goto reject_dust;
       }
     } // IF dust at left side
     else  // right side dust allowed
     { fldust |= 2;
       // T1=I1->nextl;
     }
   }
   cpnt1->duflm=cpnt1->duflr=cpnt1->gvarr=cpnt1->gvarm=0;
   if (fldust & 1)  // left side is dust
   {
     fo1=0;
     if (fldust & 4) p1=220;  // defis
     else p1=VD0 - (CL.Nd<<6) - pen_badust;
     cpnt1->gvarm=0;
     cpnt1->duflm |= GRdust;
   }
   else   // left side isn't dust
   {
     GL.flarg = GFcut+GFbadl;               // glue to left bad allowed;
     GL.arg   = GAtigr+GAallest;
     if ((T1->w) > (T1->h*3))               // NOT to ESTIMATE  T1
     {
       p1=0; c1=0;
       set_bad_cell(T1);
       save_vers(T1,&sv1); goto estim_right;
     }
     T1->cg_flag |= c_cg_cutr;
     wch[1]=wch[0]=0;  // clear out line
     Alik_def_cut_flg(T1,cpnt1,cpnt1,mh,1);
     if( PROTO.pos_inc&(erect_rot|erect_zero) )
        {
        T1->pos_inc   = PROTO.pos_inc    ;
        T1->stick_inc = PROTO.stick_inc  ;
        }

     p1=estcomp('a',T1,&sv1,TRS2,0,0,&GL,&cposd,&cpos1,n1,0,cpnt1->var);

	 if( language == LANG_TURKISH )  // Nick 12.06.02
	 {
		 // не отрезать палки у хороших символов !
		 if( //( B1->nvers > 0 && B1->vers[0].prob > TUR_PALKA_POROG ||
			   cpnt0 && cpnt0->versm.nvers > 0 && cpnt0->versm.vers[0].prob > TUR_PALKA_POROG
			 // )
			 &&
			 T1->nvers > 0 &&
			 T1->w < mw &&   // not full
			 ( T1->vers[0].let == i_sans_accent ||
			   T1->vers[0].let == II_dot_accent
			 )
			)
		 {
			p1=0; c1=0;
			set_bad_cell(T1);
			save_vers(T1,&sv1);
			goto estim_right;
		 }
	 }

#ifdef NEW_TM        //10.4.95
     if (TM_check_active)
        p1=TM_suspect(1,T1);
#endif
     if (dust_mon_flag)
       lsnap('a',T1,"upper dust ",p1);
     else
       lsnap('a',T1," ",p1);
     if (T1->dens != 255)   // calc average density
     { av_dens+=T1->dens;
       ndens++;
       if (!(cpnt1->var & 32))  // not a full cut --> check cut height
       {
         if ((cpnt1->dh << 5) > mh * thick_crit[T1->dens])
           { delcels(&CL); goto next1;  }     // not full, too thick
        // thickness criterion  in 1/32 of cell_height
       }
     }
     cpnt1->duflm |= GL.fres; cpnt1->gvarm = GL.var &15 ;
     if (forest(T1,p1,TRSO2))
     { fo1=1; cpnt1->duflm |= GRflo; }     // 0,o,O not to be cut
     c1 = T1->vers[0].let;

     if (forbid_stick_cut(T1,&cpos1,1) && (p1 > 70))
     { p1 = MAX(p1-MONstick,2);              // near stick
       lsnap('a',T1,"SR ",p1);
     }
   }
estim_right:
   if (fldust & 2)                         // right part IS dust
   {
		       // try to use the dust as dot for 'i' at right side
     fo3=0;
     cpnt1->gvarr=0;
     cpnt1->duflr |= GRdust;
     if (CL.Nd == 1)
     {
       if (dust_is_dot(&CL,T1->nextl))
          {  p3=240; goto estimrp; }
       if (is_defis(CL.maxcl))
          {  p3=220; goto estimrp; }
     }
     if (CL.Nd > 1)    // **************** IT WAS  > 0
     {
       p1 = MAX(p1 - (CL.Nd<<6)-pen_badust,0);
       lsnap('a',T1,"D ",p1);
     }
     p3=MAX(VD0-(CL.Nd<<6)-pen_badust,0);
   }
   else  // right part isn't dust
   {
     if (CL.Nd)          // dusts at any side
     {
       if (!(fldust & 4))  // left wasn't defis
       {
         if (left_dust_allowed)
           lsnap('a',T3,"LD OK ",p1);
         else
         {
           p1 = MAX(p1 - (CL.Nd<<6)-pen_badust,0);
            // all penalty for dust - at 1-st part
           lsnap('a',T1,"LD Mon ",p1);
         }
       }
       else
         lsnap('a',T3,"left defis ",p1);
     }
     GL.flarg = GFcut+GFbadr;
     GL.arg   = GAtigr+GAallest;
     if (T3->w > T3->h*3)
     {
     avoidp3:
       p3=0; c3=0;
       set_bad_cell(T3);
       goto savers3;
     }
     if (p1 < cpnt0->lv.v3)
     {
       for (n2=1,cpnt2=cpnt0+1; n2<n1; n2++,cpnt2++)
         if ((cpnt2->lv.v3 < 0) || (cpnt2->lv.v3 > p1)) goto estimp3;
         // not defined yet or can make better estimate
      goto avoidp3;
     }
     estimp3:
     wch[1]=wch[0]=0;  // clear out line
     T3->cg_flag |= c_cg_cutl;
     Alik_def_cut_flg(T3,cpnt1,cpnt1,mh,0);
     if( PROTO.pos_inc&(erect_rot|erect_zero) )
        {
        T3->pos_inc   = PROTO.pos_inc    ;
        T3->stick_inc = PROTO.stick_inc  ;
        }

     p3=estcomp('a',T3,&sv3,TRS2,0,0,&GL,&cpos1,&cposd,0,n1,cpnt1->var);

	 if( language == LANG_TURKISH )  // Nick 12.06.02
	 {
		 // не отрезать палки у хороших!
		 if( //( B1->nvers > 0 && B1->vers[0].prob > TUR_PALKA_POROG ||
			   cpnt0 && cpnt0->versm.nvers > 0 && cpnt0->versm.vers[0].prob > TUR_PALKA_POROG
              //)
			  &&
			 T3->nvers > 0 &&
			 T3->w < mw &&   // not full
			 ( T3->vers[0].let == i_sans_accent ||
			   T3->vers[0].let == II_dot_accent
			 )
			)
		 {
			p3=0; //c3=0;
			set_bad_cell(T3);
			//goto savers3;
		 }
	 }

#ifdef NEW_TM        //10.4.95
     if (TM_check_active)
        p3=TM_suspect(2,T3);
#endif
     if (T3->dens != 255) {av_dens+=T3->dens; ndens++; }
     if (dust_mon_flag)
       lsnap('a',T3,"upper dust ",p3);
     else
       lsnap('a',T3," ",p3);
     cpnt1->duflr |= GL.fres; cpnt1->gvarr = GL.var & 15;
     c3 = T3->vers[0].let;
     if (forbid_stick_cut(T3,&cpos1,2) && (p3 > 70))
     {  p3 = MAX(p3-MONstick,2);              // near stick
        lsnap('a',T3," SL ",p3);
     }
     if (forest(T3,p3,TRSO2))
     { fo3=1; cpnt1->duflr |= GRflo; }     // 0,o,O not to be cut
     if (p3 < 2) p3=2;
   savers3:
     memcpy(&(cpnt1->versr),&sv3,sizeof(SVERS));
   }
   estimrp:
   cpnt1->rv.v3=MAX(p3,2);                 // right part estimate
   cpnt1->rv.v2=254;
   cpnt1->rv.v1=254;
   cpnt1->lv.v3=MAX(p1,2);                 // single cut estimate
   cpnt1->lv.v2=254;
   cpnt1->lv.v1=254;
   cpnt1->px = 0;                          // indicate single cut
   memcpy(&(cpnt1->versm),&sv1,sizeof(SVERS));
   min3(&cpnt1->lv,(uchar)cpnt1->rv.v3,&pe);
   delcels(&CL);
   // already_cut = 1;          // assume CL ready (don't cut at "celcut")
   if (av_dens >= 16*ndens)  // semibold
   {
     if (pe.v3 >= TRS5B)
       goto next1; // should be: goto replg;   for not-to-repeat made cut
   }
   else
   {
     if (pe.v3 >= TRS5T)
       goto next1; // replg;
   }
   ///////// delcels(&CL);

   already_cut = 0;
   if ((c1=='v') && (c3=='v') && (c0=='w') && (p1+p3 >= TRSw-20) && !gv0)
     goto  restore_cell;
   if ((c1=='V') && (c3=='V') && (c0=='W') && (p1+p3 >= TRSw-20) && !gv0)
     goto  restore_cell;
nocut1:
   if (fo1 && (p1 >= TRSO3)) goto next1;    // O's not to cut */
   // 2nd level cuts - all points at left side of the 1st level point
   for (n2=1,cpnt2=cpnt0+1; n2<n1; n2++,cpnt2++)
   {
     if ((cpnt1->x - cpnt2->x) < 4)
       continue;                             // other cut version in this point */
     if (compare_vect(&cpnt2->lv,&cpnt0->lv) <= 0)
       continue;                             // already worse
     if (compare_vect(&cpnt2->lv,&cpnt1->lv) <= 0)
       continue;                             // already worse
     if ((!doubles_allowed) && (cpnt2->var & 8))
       continue;
     mn1=cut_rast(cell_raster,mw,mh,mrr,mrc,cpnt0,n2,2,csv2,&cpos2);
     // make 2nd cut & create components
     if (mn1==NULL) { glsnap('a',I1,"locomp"); goto next2; }
     cpos2.row1 = cpos2.row1 - B1->r_row + B1->row;
     cpos2.row2 = cpos2.row2 - B1->r_row + B1->row;
     nc=(uchar)colcels(&CL,mn1,I1,I3,mw);
     if (nc < 2)
       goto next2;
     if (nc == 3)      // T1-T2-T3
       goto acc2;
     if (cpnt2->duflm & GRdust)
     { T2=I1->nextl; goto acc2d; }         // (dust-T2-T3)
     if (cpnt1->duflr & GRdust)
     { T2=I1->nextl->nextl; goto acc2d; } //  (T1-T2-dust)
     if (!CL.Nd) goto next2;
     if (!is_defis(CL.dcells[0])) goto next2;
     p2=220;
     min3(&cpnt2->lv,(uchar)p2,&pe);
     if (compare_vect(&cpnt1->lv,&pe) < 0)
     {
       cpnt1->lv.v1 = pe.v1; cpnt1->lv.v2=pe.v2; cpnt1->lv.v3=pe.v3;
       cpnt1->px=n2;
       cpnt1->gvarm = GL.var & 15;
       cpnt1->versm.nvers=100;               // indicate no versions stored
     }
     goto next2;
acc2:
     T2=I1->nextl->nextl;
     if(T2->flg&c_f_fict )
        goto next2; // OLEG
acc2d:
    if(T2->flg&c_f_fict )
        goto next2; // OLEG
     GL.flarg=GFcut;
     GL.arg=GAtigr+GAallest;
     if ((T2->w) > (T2->h*3))
     {
       p2=0;
       set_bad_cell(T2);
       save_vers(T2,&sv2);
       goto estim;
     }
     wch[1]=wch[0]=0;  // clear out line
     T2->cg_flag |= (c_cg_cutr|c_cg_cutl);
     Alik_def_cut_flg(T2,cpnt2,cpnt1,mh,2);
     if( PROTO.pos_inc&(erect_rot|erect_zero) )
        {
        T2->pos_inc   = PROTO.pos_inc    ;
        T2->stick_inc = PROTO.stick_inc  ;
        }

	 // 18.06.2002 E.P.
	 if (is_turkish_language(language))
		try_cut_accent(T2,&my_bases,0);

     p2=estcomp('a',T2,&sv2,TRS2,0,0,&GL,&cpos2,&cpos1,n1,n2,cpnt2->var);

	 if( language == LANG_TURKISH )  // Nick 12.06.02
	 {
		 // не отрезать палки без точек у хороших!
		 if( cpnt1->versm.nvers > 0 && cpnt1->versm.vers[0].prob > TUR_PALKA_POROG &&
			 T2->nvers > 0 &&
			 T2->vers[0].let == i_sans_accent
			)
		 {
			p2=0; //c3=0;
			set_bad_cell(T2);
		 }
	 }

     if (T2->dens != 255) {av_dens+=T2->dens; ndens++; }
     if (dust_mon_flag)
       lsnap('a',T2,"upper dust ",p2);
     else
       lsnap('a',T2," ",p2);
     if (CL.Nd)
     {
       strcat(wch,"D ");
       p2 -= CL.Nd<<4;
     }
     if  (p2 > 70)
     {
       if (forbid_stick_cut(T2,&cpos1,1))       // right side
       { p2=MAX(p2-MONstick,2);                // near stick
         lsnap('a',T2,"SR ",p2);
       }
       if (p2 <= 70) goto p2mdust;
       if (forbid_stick_cut(T2,&cpos2,2))       // lrft side
       { p2=MAX(p2-MONstick,2);                // near stick
         lsnap('a',T2,"SL ",p2);
       }
     }
 p2mdust:
     p2 -= CL.Nd<<4;
     if (p2 < 0) p2=0;
estim:
     min3(&cpnt2->lv,(uchar)p2,&pe);
     if (pe.v3 <= MINlet)
       goto next2;
     if (compare_vect(&cpnt1->lv,&pe) < 0)
     {
        cpnt1->lv.v1 = pe.v1; cpnt1->lv.v2=pe.v2; cpnt1->lv.v3=pe.v3;
        cpnt1->px=n2;
        cpnt1->duflm = GL.fres;
        cpnt1->gvarm = GL.var & 15;
        memcpy(&(cpnt1->versm),&sv2,sizeof(SVERS));
     }
     fo2=forest(T2,p2,TRSO2);               // 0,o,O not to be cut
next2:;
     delcels(&CL);
     cut_rast(cell_raster,mw,mh,mrr,mrc,cpnt0,n2,0,csv2,&cposc);   // glue back
     if (fo2 && (p2 >= TRSO2))
     goto next1;                       // this attempt is the last one
   } // FOR internal cuts

next1:
//   if(flag_cut_point)  { flag_cut_point=0; db_status=1;}
   cut_rast(cell_raster,mw,mh,mrr,mrc,cpnt0,n1,0,csv1,&cposc);    // glue back
   take_kit_addr(KITA1);
   min3(&cpnt1->lv,(uchar)cpnt1->rv.v3,&pe);
   if (compare_vect(&cpnt0->lv,&pe) < 0)
   goto replg;
   if (fo3 && (cpnt1->rv.v3 >= TRSO3))      // don't try to cut solid 'O'
     goto finad;
   if (pe.v3 <= MINlet)
     continue;
   if (compare_vect(&cpnt0->lv,&pe) > 0)
     continue;
   cpnt2=cpnt0+cpnt0->px;
   if ((cpnt2->duflr & GRdust) && (cpnt1->duflr & GRdust)) // both dusts
   {
     if (compare_vect(&cpnt2->lv,&cpnt1->lv) < 0)
       goto replg;
     continue;
   }
   if (cpnt1->duflr & GRdust)              // new dust -- old wasn't
     continue;
replg:
   cpnt0->lv.v1=pe.v1; cpnt0->lv.v2=pe.v2; cpnt0->lv.v3=pe.v3;
   cpnt0->px=n1;
   if (av_dens >= 16*ndens)  // semibold
   {
     if (pe.v3 >= TRS4B)
       goto finad;
   }
   else
   {
     if (pe.v3 >= TRS4T)
       goto finad;
   }
 } // FOR external cuts
finad:                                    // final decision
//   if(flag_cut_point)  { flag_cut_point=0; db_status=1;}


 if (av_dens >= 16*ndens)  // semibold
 {
   if (cpnt0->lv.v3 >= TRS5B)
   goto decide;
 }
 else
 {
   if (cpnt0->lv.v3 >= TRS5T)
     goto decide;
 }
 if (!double_cutsn) goto decide;                // no doublecuts
 if (!doubles_allowed)                          // 1-st pass --> switch
   { doubles_allowed=1; goto repeat_with_doubles; }  // start pass 2
decide:
 if ((cpnt0->lv.v3 > MINlet) && cpnt0->px) goto celcut;  // cut OK
 if ((!left_dust_seen) || left_dust_allowed)
   goto restore_cell;
 left_dust_allowed = 1;
 goto repeat_with_left_dust;
restore_cell:
 B1=rest_cell(CK1,I1);
 B1->cg_flag |= c_cg_cutdone;
 glsnap ('a',B1,"cell restored");
 take_kit_addr(KITA);
 if (gv0 & 15)    // estimate made with BOXes or accent_cut ?
 {
   rest_vers(B1,&sv0);
   B1=finpat(B1,&GL,gv0,(uchar)gf0,32);
   B1->cg_flag |= c_cg_cutdone;
 }
 else
   rest_vers(B1,&sv00);
 T1=B1->prevl;
 if (!((T1->flg & c_f_fict) || (T1->cg_flag & c_cg_cutdone)))
   B1=T1->prevl;

 if( is_cen_language(language) // 05.09.2000 E.P.
	 && acc_p>200 )

	 // Теперь вызывается для всех центральноевропейских языков
    croat_reverse(B1,c0);

 if( ( is_baltic_language(language) ||
	   is_turkish_language(language) // 21.05.2002 E.P.
	 ) && acc_p>200
   )
	baltic_turkish_reverse(B1,c0);

 goto forwb;
celcut:
 if (already_cut)
   goto dontcut;
 take_kit_addr(KITA);
 p0=0;
 while (1)                                // make all cuts
 {
   cpntw=cpnt0+p0;
   p0=cpntw->px;
   if (!p0) break;
   cut_rast(cell_raster,mw,mh,mrr,mrc,cpnt0,(char)p0,1,csv1,&cposc);
 }
 mn1=cut_rast(cell_raster,mw,mh,mrr,mrc,cpnt0,0,3,csv1,&cposc); // create components
 if (mn1==NULL)
   { glsnap('a',I1,"final locomp"); goto restore_cell; }
 colcels(&CL,mn1,I1,I3,mw);

dontcut:  // cut was done in T1-T3 parts
 NL = NULL;
 // make array of new cells
 done_num = 0;
 T2 = cell_l();
 while (1)
 {
   T2 = T2 -> prevl;
   if (T2->flg & c_f_fict)
     break;
   if (T2->cg_flag & c_cg_just)    // just made letter
   {
     done_cells[done_num++] = T2;
     if (NL==NULL)
       NL=T2;
   }
 }

 // bring versions
 cpnt1=cpnt0;
 sp0 = 0;
 done_ind = 0;
 while (1)
 {
   p0=cpnt1->px;
   if ((!p0) || (done_ind >= done_num))  break;
   cpnt1=cpnt0+p0;
   if (!sp0)                                // rightmost part
   {
     sp0 = 1;
     if ((cpnt1->duflr & GRdust) == 0) // rightmost part NOT dust; recall vers
     {
       T2=done_cells[done_ind];
       rest_vers(T2,&(cpnt1->versr));
       best_vars [done_ind] = cpnt1->gvarr;
       best_flags[done_ind] = cpnt1->duflr;
       if( PROTO.pos_inc&(erect_rot|erect_zero) )
        {
        T2->pos_inc   = PROTO.pos_inc    ;
        T2->stick_inc = PROTO.stick_inc  ;
        }
       else
        {
        T2->pos_inc   = erect_no         ;
        T2->stick_inc = NO_INCLINE       ;
        }
       done_ind++;
     }
   }
   if (cpnt1->duflm & GRdust)              // middle or leftmost part was dust
     continue;                              //   don't recall vers
   if (cpnt1->versm.nvers == 100)          // middle-dust
     continue;
   T2=done_cells[done_ind];
   rest_vers(T2,&(cpnt1->versm));
   best_vars [done_ind] = cpnt1->gvarm;
   best_flags[done_ind] = cpnt1->duflm;
   if( PROTO.pos_inc&(erect_rot|erect_zero) )
    {
    T2->pos_inc   = PROTO.pos_inc    ;
    T2->stick_inc = PROTO.stick_inc  ;
    }
   else
    {
    T2->pos_inc   = erect_no         ;
    T2->stick_inc = NO_INCLINE       ;
    }
   done_ind++;
 }

 // collect broken cells
 for (done_ind=0; done_ind < done_num; done_ind++)
 {
   int16_t var, flag;
   // int16_t  col;
   cell *CF, *CFN;
   CFN=CF = done_cells [done_ind];
   var = best_vars[done_ind];
   CF->cg_flag |= c_cg_just;  // to inform finpat
   if (var & 15)
   {
     // cell *WC; int16_t c_num;
     flag = best_flags [done_ind];
     CF=finpat(CF,&GL,var,(uchar)flag,32);
   }
   if (CFN==NL)
     NL=CF;
   CF->cg_flag |= c_cg_just;
   CF->cg_flag |= c_cg_cutdone;
 }

 // print cutpoints' numbers
 cpnt1=cpnt0;
 bf_p = bf_d;
 sprintf (bf_p,"cell cut: ");
 bf_p += strlen(bf_d);
 while (1)
 {
   p0=cpnt1->px;
   if (!p0)  break;
   sprintf (bf_p,"%d ",p0);
   bf_p = bf_d + strlen(bf_d);
   cpnt1=cpnt0+p0;
   if (cpnt1->duflm & GRdust)              // middle or leftmost part was dust
     continue;                              //   don't recall vers
   if (cpnt1->versm.nvers == 100)          // middle-dust
     continue;
 }
 glsnap ('a',I1,bf_d);

 fljust = 0;
 T1=cell_f()->next;
 T3=cell_l();
 while (1)
 {
   if ((T1 == T3) || (T1==NULL))  break;
   if (T1->cg_flag & c_cg_just)
   {
     T1->cg_flag &= ~c_cg_just;
     if (fljust==0)
     {
       fljust = 1;
       if (svcg & c_cg_cutl)
         T1->cg_flag |= c_cg_cutl;
     }
     else
       T1->cg_flag |= c_cg_cutl;
     T1->cg_flag |= c_cg_cutr;
     T2=T1;
   }
   T1=T1->next;
 }
 if ((svcg & c_cg_cutr)==0) T2->cg_flag &= ~c_cg_cutr;

 if (NL)
   B1 = NL;
 else
   B1=cell_f();
 if ((B1->flg & c_f_fict) == 0)
   B1 = B1->prevl;
 goto forwb;

/************************
 sp0=0;
 cpnt1=cpnt0;
 NC1=CL.minlet->prevl;         // start here in case of equal columns of 2 cells
 while (1)                                // make all cuts
 {
   p0=cpnt1->px;
   if (!p0)  break;
   cpnt1=cpnt0+p0;
   if (!sp0)                                // rightmost part
   {
     sp0=1;
     if (cpnt1->duflr & GRdust) // rightmost part was dust; don't recall vers
       { T2=CL.maxlet->nextl; goto part2; }
     T2=CL.maxlet;
     T2->cg_flag |= c_cg_cutdone;
     rest_vers(T2,&(cpnt1->versr));
     if (cpnt1->gvarr & 15)                 // need to glue
     {
       cell *oldt2;
       oldt2 = T2;
       T2=finpat(T2,&GL,cpnt1->gvarr,cpnt1->duflr,32);
       if (oldt2 != T2)  // collected ??
       {
         T2->cg_flag |= c_cg_just;
         if (oldt2 == CL.mincl)
           CL.mincl = T2;
         if (oldt2 == CL.maxcl)
           CL.maxcl = T2;
        }
     }
     T2->cg_flag |= c_cg_cutdone;
   }
part2:
   if (cpnt1->duflm & GRdust)              // middle or leftmost part was dust
     continue;                              //   don't recall vers
   if (cpnt1->versm.nvers == 100)          // middle-dust
     continue;
   T2=T2->prevl;
   T2->cg_flag |= c_cg_cutdone;
   rest_vers(T2,&(cpnt1->versm));
   if (cpnt1->gvarm & 15)                  // need to glue
   { cell *oldt2;
     oldt2 = T2;
     T2=finpat(T2,&GL,cpnt1->gvarm,cpnt1->duflm,32);
     if (oldt2 != T2)  // collected ??
     {
       T2->cg_flag |= c_cg_just;
       if (oldt2 == CL.mincl)
         CL.mincl = T2;
       if (oldt2 == CL.maxcl)
         CL.maxcl = T2;
     }
   }
   T2->cg_flag |= c_cg_cutdone;
   c1=T2->vers[0].let;
 }

 cpnt1=cpnt0;
 bf_p = bf_d;
 sprintf (bf_p,"cell cut: ");
 bf_p += strlen(bf_d);
 while (1)
 {
   p0=cpnt1->px;
   if (!p0)  break;
   sprintf (bf_p,"%d ",p0);
   bf_p = bf_d + strlen(bf_d);
   cpnt1=cpnt0+p0;
   if (cpnt1->duflm & GRdust)              // middle or leftmost part was dust
     continue;                              //   don't recall vers
   if (cpnt1->versm.nvers == 100)          // middle-dust
     continue;
 }
 glsnap ('a',I1,bf_d);

 T1=T2=I1=CL.mincl;
 I3=CL.maxcl->next;
 while (1)
 {
   if ((T1 == I3) || (T1==NULL))  break;
   if (T1->cg_flag & c_cg_just)
   {
     T1->cg_flag &= ~c_cg_just;
     if (T1==I1)
       { if (svcg & c_cg_cutl) I1->cg_flag |= c_cg_cutl; }
     else    T1->cg_flag |= c_cg_cutl;
     T1->cg_flag |= c_cg_cutr;
     T2=T1;
   }
   T1=T1->next;
 }
 if ((svcg & c_cg_cutr)==0) T2->cg_flag &= ~c_cg_cutr;

 B1 = NC1;
 if ((B1->flg & c_f_fict) == 0)
   B1 = B1->prevl;
 goto forwb;
*************************************************/


fin:
 B1=cell_f()->nextl;
 glsnap('a',B1, "cut end, corr start");
 corr_cut();
 B1=cell_f()->nextl;
 glsnap('a',B1, "corr end");
 while (B1->next)
 { // Poul
   if ( B1->flg & c_f_space )  B1->col=0;
   B1=B1->next;
 }

 //Alik 16.10.95
   if(flag_cut_point)    { flag_cut_point=0;  db_status=1; }


}

static int16_t have_upper_dot(cell *c)
{
 cell *cc;
 int16_t H;
 H=my_bases.ps;
 cc=c->prev;
 if ((cc->flg & c_f_dust) &&
      (c->w>4 && cc->h>=2 && cc->w>=2 &&
	(abs(cc->h-cc->w)<=H/6 || cc->h<cc->w && cc->w-cc->h<=H/4) &&
	cc->col+1>=c->col && cc->col+cc->w-5<=c->col+c->w ||
       c->w<=4 && abs(c->col-cc->col+(c->w-cc->w)/2)<=2) &&
      cc->row+cc->h-2<=my_bases.b2)
   return 1;
 cc=c->next;
 if ((cc->flg & c_f_dust) &&
      (c->w>4 && cc->h>=2 && cc->w>=2 &&
	(abs(cc->h-cc->w)<=H/6 || cc->h<cc->w && cc->w-cc->h<=H/4) &&
	cc->col+1>=c->col && cc->col+cc->w-5<=c->col+c->w ||
       c->w<=4 && abs(c->col-cc->col+(c->w-cc->w)/2)<=2) &&
      cc->row+cc->h-2<=my_bases.b2)
   return 1;
 return 0;
}
static int16_t forbid_stick_cut(cell *c, cut_pos *cpos, int16_t edge)
 {
 uchar l, prob;
 int16_t  wd, bm3, bm, b3;
 int16_t  row1, row2;

  l = c->vers[0].let;
  prob = c->vers[0].prob;

  // is it a stick ?
  if ((l!='i') && (l!='l') && (l!='1') && (l!='I') &&
	  (l!=liga_i) &&
	   !(language==LANG_TURKISH &&  // 30.05.2002 E.P.
			(l==i_sans_accent||l==II_dot_accent)
		)
	 ) return 0;
  wd = (row2=cpos->row2)-(row1=cpos->row1);  // cut width
  if ((wd > c->h-2) && (wd < cpos->cuth+2))  // full cut
    return 1;
  //93.01.28 allow the non_full cut for strong 'i' (with dot)
  if ((l=='i') && (prob >= 200)) return 0;
  b3=my_bases.b3 + c->bdiff;
  ///////////
  if (2*(b3-row2)-1 > my_bases.ps) return 0;  // above middle line (b2/b3)
  ///////////
  bm3=bsm3+c->bdiff;  bm = my_bases.bm + c->bdiff;

  // first try to retain, then to abort;
  // before 01.06 the 2 lines of code were after 'thickness' check
  ///////
  ///////   LOOK ABOVE
  ///////   if (row2 <= bm) return 0;
  ///////   the cut is entirely upper than middle
  ///////
  if (row1 >= bm3) return 0;     // or lower than (middle--b3)
  if (cpos->flag & 8) return 1;  // thick nonsimple cut
  if (cpos->flag & 4) return 0;  // sophisticated cut (double, horizontal)
  if (3*wd > c->h)    return 1;  // simple thick
  /////// // 93/01/25 very thin cut --> not a 'k' -- NOT INVENTED
  ////////   NOT INVENTED :  if (5*wd < c->h) return 0;
  ////////   stda17/31  making  ki--> ld
  ////////   stdc8/29   dunkle  kl--> ld
  if (row2 >= (b3-2)) return 0;  //  till very bottom
  if (edge == 1)  return 1;      // at right side

  // SO BEFORE 01.06 stick cuts were:
  //    thick - anywhere
  //    starting and ending 'in the middle' AND at right side of stick
  // AFTER 01.06 stick cut is:
  //
  return 0;
//

}

void promote (uchar sn, cell *cl, uchar let, int16_t delta)
{
 uchar wl, wl_sacc, let_sacc;
 version *vp1, *vp2; uchar p1, pw; int16_t pwi, dlt, dlv, fld;
 if (cl->nvers==0) return;
 let_sacc = let_sans_acc[let];
 dlt=delta;
 fld=0;
 if (dlt > 512) { fld=1; dlv=dlt=dlt-512; }  // absolute value (not a promotion)
 for (vp2=vp1=cl->vers; (((wl=vp1->let) != 0) && ((p1=vp1->prob) != 0)); vp1++)
 {
   wl_sacc = let_sans_acc[wl];
   if (wl_sacc==let_sacc)
   {
     if (delta > 0)
     {
       if (vp1==vp2) dlt=8;
       if (fld)  // make known value (not less than present)
       {
         pw=vp1->prob; pwi=pw;
         if (pwi < dlv) pwi=dlv;
       }
       else     // make the version to be the 1st
       {
         pw=vp2->prob; pwi=pw;
         pwi += dlt;
       }
       if (pwi > 254) pwi = 254;
       vp1->prob=(uchar)pwi;  // give (max+delta) to promoted version
       sort_vers(cl);
       if (sn)
         glsnap(sn,cl,"promoted ");
	return;
     }
     else
     {
       pwi=p1; pwi += delta;
       if (pwi <= 2) pwi=2;
       vp1->prob=(uchar)pwi;
       sort_vers(cl);
       if (sn)
         glsnap(sn,cl,"monused ");
	return;
     }
   } //  IF letter found
 } // FOR versions
 if (delta < 0) return;
 if (cl->nvers == (VERS_IN_CELL-1)) vp1--;
 else cl->nvers++;
 vp1->let=let; pw=vp2->prob; pwi=pw;
 if (fld)
   { if (pwi < dlv) pwi=dlv; }
 else pwi += dlt;
 if (pwi > 254) pwi=254; vp1->prob=(uchar)pwi;
 cl->vers[cl->nvers].let=cl->vers[cl->nvers].prob=0;
 sort_vers(cl);
 if (sn)
   glsnap((char)(sn>'a'?sn:'a'),cl,"insvers");
}

static void corr_cut()
{ cell *b1, *b2, *b3;
  uchar c1,c2,c3;
  uchar p2;
 b2=cell_f();
 while ((b2=b2->nextl)->nextl != NULL)
 {
   if (b2->flg & c_f_bad)
   {
     p2 = b2->vers[0].prob;
     if (p2 >= TRSG)                        // for not to glue good char
       b2->flg ^= (c_f_bad+c_f_let);
   }
   if ((b2->flg & c_f_let) && (b2->nvers == 0))
   {
      b2->flg ^= (c_f_bad+c_f_let);
      b2->vers[0].let = bad_char;
      b2->vers[0].prob=0;
   }
   if (b2->nvers != 0 && b2->cg_flag & c_cg_cut && tsimple(b2))
   { // was cutted, has envelope (can be glued)
     b1=b2->prevl; b3=b2->nextl;
     c2=b2->vers[0].let;
     c1=b1->vers[0].let;
     c3=b3->vers[0].let;
     // try to recover "stick_to_something" producing 'k', 'd', 'b'
     if (memchr("lI1/J)!",c2,7) ||         // stick character ?
		c2==liga_i ||
		   language==LANG_TURKISH &&  // 30.05.2002 E.P.
			(c2==i_sans_accent||c2==II_dot_accent) ||
		c2==liga_exm
		)
       // in combinations  (vI) (WI) (uI)  promote  't' in place of 'I'
       if ((b2->cg_flag & c_cg_cutr) && ((c1=='v') || (c3=='w') || (c3=='u')))
       {
         promote('a',b2,'t',16);
         continue;
       }
     if ((c1=='r') && (c2=='t') && have_upper_dot(b1))
       promote ('a',b1,'i',16);
   } // cutten letter with envelope
 }   // while all cells
}

#ifdef NEW_TM        //10.4.95
static int16_t TM_suspect(int16_t position, cell *c)
{
  int16_t nvers, i;
  uchar let;

  if (nvers=c->nvers)
     for (i=0; i<nvers; i++)
         {
           let=c->vers[i].let;
           switch (position)
                  {
                    case 1:
                        if ( (let=='T' || let=='t' ||
							  let==(uchar)'’' || let==(uchar)'в' ) &&
						     !is_russian_baltic_conflict(let) &&// 17.07.2001 E.P.
							 !is_russian_turkish_conflict(let)	// 21.05.2002 E.P.
						   )

                           {
                             c->vers[i].prob=MAX_TM_PROB;
                             sort_vers(c);
                             return MAX_TM_PROB;
                           }
                    case 2:
                       if (let=='M' || let=='m' || let==(uchar)'Њ' || let==(uchar)'¬')
                           {
                             c->vers[i].prob=MAX_TM_PROB;
                             sort_vers(c);
                             return MAX_TM_PROB;
                           }
                  }
         }
return c->vers[0].prob;
}
#endif

Bool is_comma(cell *c, int16_t bases3)
{
 uint32_t    d;
 int16_t      ll, n, h, i;
 lnhead   *line;
 interval *inter;


  if( c->row>bases3-1 || c->row+c->h<bases3+2 )
    return FALSE;
  if( c->w*4>c->h*3 )
    {
    if( c->w>c->h )
      return FALSE;
    else
      {
      for (line=(lnhead *)((pchar)(c->env)+c->env->lines+sizeof(int16_t)),n=0,d=0;
        (ll=line->lth)>0; line=(lnhead *)((pchar)line+ll))
        {
        h=line->h;
        i=line->row;
        for( inter=(interval *)((pchar)line+sizeof(lnhead));h;
          n++,h--,inter++,i++)
            if( inter->l!=1||(i!=0&&i!=c->h-1) ) // non pimple or middel line
              d += inter->l;
        }
      if( n>c->h+1 || n<=c->h-2 || n==0 )
        return FALSE;
      d /= n; // average width
      return( d*4<=(uint32_t)(c->h*3) );
      }
    }
return TRUE;
}
