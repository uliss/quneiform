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

#include <stdlib.h>
#include <string.h>
#include "struct.h"
#include "ligas.h"
#include "v1comp.h"
#include "status.h"		// 06.09.00 E.P.
#include "lang_def.h"	// 06.09.00 E.P.

//  Internal working fields
struct ev_vari	{
  uchar ln;  // line number
  char add; // additional byte:
		};
typedef struct ev_vari VAR;
// Oleg
extern  uchar   save_eventr_txt[],save_eventr_txts[];
extern  Bool32  enable_save_stat;
// Oleg
extern c_comp wcomp;
extern uchar work_raster[],work_raster_1[];
extern version * start_rec, *rec_ptr;
extern uchar recors_change;
extern uchar lpool[];
extern uchar* events_tree_rt;
extern uchar evline[], evline1[];
extern VAR evvars[64], * evendvar;
extern uchar evrow_b1, evrow_b2, evrow_b3, evrow_b4;
extern uchar evcol_b1, evcol_b2, evcol_b3, evcol_b4, evcol_b5;
extern uchar* seek_responce;
extern MN *main_number_ptr;
extern BOX *boxchain, *dl_last_in_chain;
extern char alphabet[];

static uchar ev;
static VAR * vp;

Bool16 boxes_account();
extern void make_raster(void);
extern void c_rastror (uchar *, uchar *, int32_t, int32_t);
extern MN *c_locomp(uchar *,int32_t,int32_t,int32_t,int32_t);

static void save_rot_comp();
static void rest_rot_comp();
static int16_t net_comp (MN *mn);
static void ev_vector_cnt();
static int16_t seek_events(uchar* ev);
static int16_t first_var();
static void upper_row(int16_t row);
static void upper_col(int16_t col);
static void lower_row(int16_t row);
static void lower_col(int16_t col);
void ev_lang_filter();
static int16_t is_english(uchar ch);

extern uchar ev_rt_num_ln;
int16_t events_recog_rt()
{
 MN *mn;
 int16_t n, k;
 version * a,* b;
 ev_rt_num_ln = 0;
 save_rot_comp();
 make_raster();
 c_rastror (work_raster, work_raster_1, wcomp.w, wcomp.h);
 mn = c_locomp(work_raster_1,(wcomp.h+7)/8,wcomp.w,0,-((-wcomp.h) & 7));
 if (mn == NULL || mn->mnnext!=NULL )  // Oleg : 20-01-1999 : mnnext can be zero.
    goto no_answer;                       // for case recog external LP-represenation
 if (mn->mnlines > NET_COMPONENT)
  {if (net_comp(mn) > NET_COMPONENT) goto no_answer;}

 ev_rt_num_ln = MIN(mn->mnlines,15);
 main_number_ptr = mn; boxes_account();
 //snap_keep(snap_rotate,NULL,0);
 ev_vector_cnt();
 rest_rot_comp();
 n = seek_events(evline);
 if (n == 0)
    {
    n = first_var();
    if (n <= 0)
        goto no_vers;
    }
if(0)
 { // 12-09-2000 Неправильно сгенерировалась таблица Эвент
	 int i;
	 for(i=0;i<n;i++)
	 {
		 if( seek_responce[i]==0 )
		 {
			memcpy(seek_responce+i,seek_responce+i+1,n-i-1);
			n--;
			i--;
		 }
	 }
	}

 k=0;a=b=start_rec; while (a != rec_ptr)
  {
   if (memchr (seek_responce,a->let,n))
       {b->let = a->let;
       if( enable_save_stat )
            save_eventr_txts[k++] = a->let;
       (b++)->prob = a->prob;
       }
   a++;
  }
  save_eventr_txts[k] = 0;
 /*** AL 940112 ***   VERIFY WITH AT  ****/
 rec_ptr = b;
 ev_lang_filter();
 n = (b=rec_ptr) - start_rec;
 if (n==0) goto no_vers;  /***     AL 940112 ***/
 //snap_keep(snap_vers,(uchar*)start_rec,n);
 return n;

no_answer:
 rest_rot_comp();
no_vers:
 rec_ptr = start_rec;
 //snap_keep (snap_vers,NULL,0);
 return 0;
}

static void ev_vector_cnt()
{
 int16_t n, nd, x, y;
 uchar* ep;
 MN *mn;
 BOX *bp;
 LNSTRT *lp;
 VAR *vs;
 BOXINT *ip;

 evrow_b1 = (wcomp.h + 3)/4; evrow_b2 = wcomp.h - evrow_b1;
 n = wcomp.w%5; nd = wcomp.w/5;
 evcol_b1 = nd + ((n > 0) ? 1:0);
 evcol_b4 = wcomp.w - nd - ((n > 1) ? 1:0);
 evcol_b3 = evcol_b4 - nd - ((n > 3) ? 1:0);
 evcol_b2 = evcol_b1 + nd + ((n > 2) ? 1:0);

 mn = main_number_ptr;
 evline[0] = mn->mnlines; evline[1] = mn->mnbegs; evline[2] = mn->mnends;

 ep = evline+4; vp = evvars; evvars[0].ln = 0; bp = boxchain;

loop:
 lp = (LNSTRT *) ((uchar*)bp + sizeof(BOX));
 if (bp->boxptr <= sizeof(BOX) + sizeof(LNSTRT) + sizeof(BOXINT) + 2)
  {
   vs = vp;
   if (bp->boxptr <= sizeof(BOX) + sizeof(LNSTRT) + 2)
    {
     if (bp->boxflag & (BOXFREEBEG + BOXFREEEND - BOXBEG - BOXEND))
      {
       evline[0]--;
       if (bp->boxflag & (BOXFREEBEG - BOXBEG)) evline[1]--;
       if (bp->boxflag & (BOXFREEEND - BOXEND)) evline[2]--;
       goto skip;
      }
     else
      {
bybeg:       y = lp->y; x = lp->x - (lp->l+1)/2; goto double_lab;
      }
    }
   else
    {
     if (bp->boxflag & (BOXFREEBEG - BOXBEG)) goto bybeg;
     y = bp->boxey; x = bp->boxex - (bp->boxel+1)/2;
double_lab:
      vp->ln++;
      upper_row(y); upper_col(x);
      ev += ev>>4;
      while (vs != vp) {vs->add += vs->add >> 4; vs++;} goto next_line;
    }
  }

//	Long lines

 vp->ln++;

 upper_row (lp->y);
 if (bp->boxflag & (BOXFREEBEG - BOXBEG))  x = lp->x - (lp->l + 1)/2;
 else  { ip = (BOXINT *)(lp+1);  x = lp->x + ip->d - (ip->l + 1)/2; }
 upper_col (x);

line_loop:
 ip = (BOXINT *)((uchar*)bp + bp->boxptr);
 if ((bp->boxflag & BOXEND) == 0)
   { x = (ip-1)->l; bp = bp->boxnext; goto line_loop; }

 if (bp->boxflag & (BOXFREEEND - BOXEND))
  { x = bp->boxex - (bp->boxel + 1)/2; }
 else
  {
   if (bp->boxptr > sizeof(BOX) + sizeof(BOXINT) + 2)
    x = (((BOXINT *)((uchar*)ip -1)) - 2)->l;
   x = bp->boxex - (((BOXINT *)((uchar*)ip -1)) - 1)->d - (x+1)/2;
  }
 lower_row(bp->boxey); lower_col(x);

next_line:	*(ep++) = ev;
skip:		if (bp != dl_last_in_chain) {bp = bp->boxnext; goto loop;}
 evendvar = vp;
}

static void upper_row(int16_t row)
{
 if (row >= evrow_b2)
  {
   ev = 2*16*5;
   if (row == evrow_b2)  { vp->add = -16*5; (vp+1)->ln = vp->ln; vp++; }
   return;
  }
 if (row >= evrow_b1)
  {
   ev = 1*16*5;
   if (row == evrow_b1) { vp->add = -16*5; (vp+1)->ln = vp->ln; vp++; }
   else if (row+1 == evrow_b2) { vp->add = 16*5; (vp+1)->ln = vp->ln; vp++;}
   return;
  }
 ev = 0;
 if (row+1 == evrow_b1) { vp->add = 16*5; (vp+1)->ln = vp->ln; vp++;}
 return;
}

static void upper_col(int16_t col)
{
 if (col >= evcol_b2)
  {
   if (col == evcol_b2) { ev += 2*16; goto var_minus; }
   if (col <= evcol_b3)
    {
     if (col == evcol_b3) { ev += 3*16; goto var_minus;}
     ev += 2*16; if (col+1 == evcol_b3) goto var_plus;
     return;
    }
   if (col >  evcol_b4) { ev += 4*16; return; }
   if (col == evcol_b4) { ev += 4*16; goto var_minus; }
   ev += 3*16; if (col+1 == evcol_b4) goto var_plus;
   return;
  }
 if (col <= evcol_b1)
  {
   if (col == evcol_b1) { ev += 1*16; goto var_minus; }
   if (col+1 == evcol_b1) goto var_plus;
   return;
  }
 ev += 1*16; if (col+1 == evcol_b2) goto var_plus;
 return;

var_plus: vp->add = 1*16; (vp+1)->ln = vp->ln; vp++; return;
var_minus: vp->add = -1*16; (vp+1)->ln = vp->ln; vp++; return;
}

static void lower_row(int16_t row)
{
 if (row >= evrow_b1)
  {
   if (row == evrow_b1) {ev += 1*5; goto var_minus; }
   if (row >= evrow_b2)
    { ev += 2*5; if (row == evrow_b2) goto var_minus;  return; }
   ev += 1*5; if (row+1 == evrow_b2) goto var_plus; return;
  }
 if (row+1 == evrow_b1) goto var_plus; return;

var_plus: vp->add = 1*5; (vp+1)->ln = vp->ln; vp++; return;
var_minus: vp->add = -1*5; (vp+1)->ln = vp->ln; vp++; return;

}

static void lower_col(int16_t col)
{
 if (col >= evcol_b2)
  {
   if (col == evcol_b2) { ev += 2; goto var_minus; }
   if (col <= evcol_b3)
    {
     if (col == evcol_b3) { ev += 3; goto var_minus;}
     ev += 2; if (col+1 == evcol_b3) goto var_plus;
     return;
    }
   if (col >  evcol_b4) { ev += 4; return; }
   if (col == evcol_b4) { ev += 4; goto var_minus; }
   ev += 3; if (col+1 == evcol_b4) goto var_plus;
   return;
  }
 if (col <= evcol_b1)
  {
   if (col == evcol_b1) { ev += 1; goto var_minus; }
   if (col+1 == evcol_b1) goto var_plus;
   return;
  }
 ev += 1; if (col+1 == evcol_b2) goto var_plus;
 return;

var_plus: vp->add = 1; (vp+1)->ln = vp->ln; vp++; return;
var_minus: vp->add = -1; (vp+1)->ln = vp->ln; vp++; return;
}

extern uchar tabb[], taba[];

static uint16_t rot(uint16_t n) { return ((n<<3) + (n>>13)); }

static int16_t seek_events(uchar* ep)
{
 uint16_t hash,i,nl;
 uint32_t di;
 uchar* p;
 //if (db_status) snap_keep(snap_vector,ep,*ep+4);
 if( enable_save_stat )
 {
int i,n;
n=save_eventr_txt[0]=ep[0];
save_eventr_txt[1]=ep[1];
save_eventr_txt[2]=ep[2];
for(i=0;i<n;i++)
    save_eventr_txt[3+i]=ep[4+i];
 }
 nl = *ep;
 if( !nl )	return 0; // OLEG
 hash = *(ep+3) = (tabb[nl] + taba[nl-*(ep+1)] + *(ep+2)) & 0xff;
 i = nl; p=ep+4; while (i--) hash = rot(hash) ^ (uint16_t)(*(p++));
 hash = (hash & 0xff) ^ (hash >> 8);
 p = ((nl-1) * 256 + hash) * sizeof(uint32_t) + events_tree_rt;
 di = *((uint32_t *)p); if (di==0) return 0; p = events_tree_rt+di;
 nl++;
 while (*p)
  {
   if (memcmp (ep+3,p+1,nl) == 0)
    {
    seek_responce = p+nl+1;
    return *p;
    }
   p += *p+1+nl;
  }
 return 0;

}

static uchar var_answer[128];

static int16_t first_var()
{
 VAR *p=evvars;
 uchar *ev1, *ev2;
 uint16_t nl,n;
 int16_t var_answer_cnt=0;

 if (p == evendvar) return -1;
 memcpy (evline1,evline,4);
 ev1 = evline+4;
 ev2 = evline1+4; nl =1;
var_loop:
 n = p->ln - nl;
 if (n) {nl = p->ln; memcpy (ev2,ev1,n); ev1+=n; ev2+=n;}
 *(ev2++) = *(ev1++) + p->add;
 if( evline[0]<nl )	 return -1; // OELG
 n=evline[0]-nl; if (n) memcpy (ev2,ev1,n);
 //snap_keep (snap_variation,NULL,0);
 n = seek_events(evline1);
 if (n > 0)
  {
   if (var_answer_cnt == 0) { 	memcpy (var_answer,seek_responce,n);
				var_answer_cnt = n; }
   else
    {
     uchar* s = seek_responce;
     while (n--) { if (memchr (var_answer,*s,var_answer_cnt) == NULL)
			{ var_answer[var_answer_cnt++] = *s; }
		   s++; }
    }
  }
 if (++p == evendvar) { seek_responce = var_answer; return var_answer_cnt; }
 ev1 = evline+3+nl; ev2=evline1+3+nl; goto var_loop;
}

static c_comp rot_save;

static void save_rot_comp()
{
 memcpy (&rot_save,&wcomp,sizeof(wcomp));
}

static void rest_rot_comp()
{
 memcpy (&wcomp,&rot_save,sizeof(wcomp));
}

static int16_t net_comp (MN *mn)
{
 BOX *bp;
 int16_t n, c;
 bp = mn->mnfirstbox; n = mn->mnboxcnt; c=0;
 while (n--)
  {
   if (bp->boxflag & BOXBEG)
    {
     if ((bp->boxptr > sizeof(BOX) + sizeof(LNSTRT) + 2) ||
	((bp->boxflag & (BOXFREEBEG + BOXFREEEND - BOXBEG - BOXEND)) == 0))
      c++;
    }
   bp = bp->boxnext;
  }
 return c;
}

void ev_lang_filter()
{
 version *v1,*v2;
/*
 uchar l;

   for (v1=v2=start_rec; v1!=rec_ptr; v1++)
    if( (l=v1->let) != UKR_E && l != UKR_e && l != UKR_g && l != UKR_G &&
        l != liga_i && !memchr("iI",l,2) )
     {v2->let=v1->let; v2->prob=v1->prob; v2++;}
   rec_ptr=v2;

   for (v1=v2=start_rec; v1!=rec_ptr; v1++)
    if( (l=v1->let) != SERB_HH && l != SERB_hh && l != SERB_H && l != SERB_h &&
        l != SERB_L && l != SERB_l && l != SERB_N && l != SERB_n &&
        l != SERB_U && l != SERB_u && l != SERB_J && l != SERB_j &&
        l != liga_j && !memchr("jJ",l,2) )
     {v2->let=v1->let; v2->prob=v1->prob; v2++;}
   rec_ptr=v2;

   for (v1=v2=start_rec; v1!=rec_ptr; v1++)
       if( !(is_english(v1->let) ) ) //||v1->let=='$'  ) )
              { v2->let=v1->let; v2->prob=v1->prob; v2++; }
          goto ret;
*/
   for (v1=v2=start_rec; v1!=rec_ptr; v1++)
       if( alphabet[v1->let] )
              { v2->let=v1->let; v2->prob=v1->prob; v2++; }
          goto ret;

   return;
  ret:
   rec_ptr=v2;
}

static int16_t is_english( uchar ch )
{
return (ch >= 'a' && ch <= 'z')||(ch >= 'A' && ch <= 'Z')||
       (ch>=ligas_beg && ch<=ligas_end && ch!=liga_exm && ch!=liga_qm ) ;
}
