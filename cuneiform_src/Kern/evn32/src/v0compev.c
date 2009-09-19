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


//#include "c_types.h"

#include <string.h>
#include "struct.h"
#include "v1comp.h"

//#include "func.h"
//#include "evsnap.h"
//#include "extrcomp.h"

//	Function description
//void snap_keep (uchar code, uchar* arg1, uint16_t arg2);

//		Common variables
// Oleg
extern  uchar   save_event_txt[],save_event_txts[];
extern Bool32  enable_save_stat;
// Oleg
extern c_comp wcomp;
extern version * start_rec, *rec_ptr;
extern uchar recors_change;
extern uchar lpool[];
extern uchar* events_tree;
extern void ev_lang_filter();
//	Internal function description
static uint16_t net_comp_count();
static void ev_vector_cnt();
static uint16_t seek_events(uchar* ev);
static int16_t first_var();
static void double_events(uint16_t row, uint16_t col);
static void varset (char add);
static void upper_row(int16_t row);
static void upper_col(int16_t col);
static void lower_row(int16_t row);
static void lower_col(int16_t col);


//	Internal working fields
struct ev_vari	{
  uchar ln;  // line number
  char add; // additional byte:
		};
typedef struct ev_vari VAR;

uchar evline[3+12], evline1[3+12];
VAR evvars[64], * evendvar;
uint16_t evrow_b1, evrow_b2, evrow_b3, evrow_b4;
uint16_t evcol_b1, evcol_b2, evcol_b3, evcol_b4, evcol_b5;
uchar* seek_responce;
extern int16_t evfulln;
static uchar ev;
static VAR * vp;

//
//
//		Straight events counting and tables search
//
//

uint16_t events_recog()
{
 int16_t nv;
 uint16_t i,k;
 uchar* p;
 version *v;
 //snap_keep (snap_stright,NULL,0);
 if ((wcomp.nl > NET_COMPONENT) && (net_comp_count() > NET_COMPONENT))
  {/*snap_keep (snap_vers,NULL,0);*/ return 0;}
 ev_vector_cnt();
 if( evline[0]==0 || evline[0]> NET_COMPONENT )
    {
    return 0;
    }
 nv = seek_events(evline);
 if(0)
 { // 12-09-2000 Неправильно сгенерировалась таблица Эвент
	 int i;
	 for(i=0;i<nv;i++)
	 {
		 if( seek_responce[i]==0 )
		 {
			memcpy(seek_responce+i,seek_responce+i+1,nv-i-1);
			nv--;
			i--;
		 }
	 }
	}
 if (nv == 0)
  {
   nv = first_var();
   if (nv < 0) { /*snap_keep (snap_vers,NULL,0);*/ return 0; }
  }
 p = seek_responce; i = nv; v = start_rec;
 k=0;
 while (i--)  { v->let = *p++;
 if( enable_save_stat )
    save_event_txts[k++] = v->let;
 (v++)->prob = 0xfe;
 }
 save_event_txts[k] = 0;
 rec_ptr = v; //snap_keep (snap_vers,(uchar*)start_rec,nv);
 ev_lang_filter();
 return nv;
}

//-------------	Counting the straight events vector ----------------------
//	Structure of events vector:
//	First 3 bytes: Numbers of lines, of free starts and free ends
//	One byte: topological code - will counted in seek_event
//	One byte per line: line start and ebd codes

#define next_var (a) { (a+1)->ln = a->ln; a++;}

static void ev_vector_cnt()
{
 uchar* ep;
 lnhead * lp;
 interval * ip;

 evrow_b2 = (evrow_b4 = wcomp.h)/2;
 evrow_b1 = (evrow_b2+1)/2;
 evrow_b3 = evrow_b2 + (wcomp.h - evrow_b2)/2;
 evcol_b1 = ((evcol_b3 = wcomp.w) + 2)/3;
 evcol_b2 = evcol_b3 - evcol_b1;

 evfulln = wcomp.nl;
 evline[0] = (uchar)wcomp.nl;
 evline[1] = wcomp.begs; evline[2] = wcomp.ends;

 ep = evline+4;
 vp = evvars;
 vp->ln = 0;
 lp = (lnhead *)lpool;

 while (lp->lth)
  {
   ip = (interval *) (lp + 1);
   if (lp->h == 1)
    {
	// One point lines
     if (lp->flg & (l_fbeg + l_fend))
      {
       evline[0]--; evfulln--;
       if (lp->flg & l_fend) evline[2]--;
       if (lp->flg & l_fbeg) evline[1]--;
       lp = (lnhead *)((uchar*)lp + lp->lth); continue;
      }
     else
      {
by_beg:
       double_events((uint16_t)lp->row, (uint16_t)(ip->e - (ip->l + 1)/2));
       *ep++ = ev; lp = (lnhead *)((uchar*)lp + lp->lth); continue;
      }
    }
	// Two points line
   if (lp->h == 2)
    {
     if (lp->flg & l_fbeg) goto by_beg;
     else
      {
       double_events((uint16_t)(lp->row + 2),(uint16_t)((ip+1)->e - ((ip+1)->l + 1)/2));
       *ep++ = ev; lp =(lnhead *)((uchar*)lp + lp->lth); continue;
      }
    }
	// Long line
   vp->ln++;
   upper_row (lp->row);
   if (lp->flg & l_fbeg)
      upper_col ((uint16_t)(ip->e - (ip->l + 1)/2));
   else
      upper_col ((uint16_t)((ip+1)->e - ((ip+1)->l + 1)/2));
   lower_row ((uint16_t)(lp->row + lp->h));
   ip += lp->h;
   if (lp->flg & l_fend)
    lower_col ((uint16_t)((ip-1)->e - ((ip-1)->l + 1)/2));
   else
    lower_col ((uint16_t)((ip-2)->e - ((ip-2)->l + 1)/2));
   *ep++ = ev; lp = (lnhead *)(ip+1);
  }
 evendvar = vp;
}

static void double_events(uint16_t row, uint16_t col)
{
 VAR * wvp;
 vp->ln++;	wvp = vp;
 upper_row(row);	upper_col(col);
 ev += ev >> 4;
 while (wvp != vp) { wvp->add += wvp->add >> 4; wvp++;}
}

static void varset (char add)
{
 vp->add = add; (vp+1)->ln = vp->ln; vp++;
}

static void upper_row(int16_t row)
{
 if (row >= evrow_b2)
  {
   if (row == evrow_b2) {ev = 2*64; varset(-64); return;}
   if (row >= evrow_b3) {ev = 3*64; if (row == evrow_b3) varset(-64); return;}
   ev = 2*64; if (row+1 == evrow_b3) varset(64);
   return;
  }
 else
  {
   if (row >= evrow_b1)
    {
     ev = 1*64;
     if (row == evrow_b1) {varset(-64); return;}
     if (row+1 == evrow_b2) varset(64);
     return;
    }
   ev = 0; if (row+1 == evrow_b1) varset(64);
   return;
  }
}

static void upper_col(int16_t col)
{
 if (col > evcol_b1)
  {
   if (col < evcol_b2) { ev+=16; if (col+1 == evcol_b2) varset(16); return; }
   ev+=2*16; if (col == evcol_b2)  varset(-16); return;
  }
 if (col == evcol_b1) { ev+=16; varset(-16); return; }
 if (col+1 == evcol_b1) varset(16); return;
}

static void lower_row(int16_t row)
{
 if (row >= evrow_b2)
  {
   if (row == evrow_b2) {ev+=2*4; varset(-4); return;}
   if (row >= evrow_b3) {ev+=3*4; if (row == evrow_b3) varset(-4); return;}
   ev+=2*4; if (row+1 == evrow_b3) varset(4);
   return;
  }
 else
  {
   if (row >= evrow_b1)
    {
     ev+=1*4;
     if (row == evrow_b1) {varset(-4); return;}
     if (row+1 == evrow_b2) varset(4);
     return;
    }
   if (row+1 == evrow_b1) varset(4); return;
  }
}

static void lower_col(int16_t col)
{
 if (col > evcol_b1)
  {
   if (col < evcol_b2) { ev++; if (col+1 == evcol_b2) varset(1); return; }
   ev+=2; if (col == evcol_b2)  varset(-1); return;
  }
 if (col == evcol_b1) { ev++; varset(-1); return; }
 if (col+1 == evcol_b1) varset(1); return;
}

static uint16_t net_comp_count()
{
 lnhead * p = (lnhead *)lpool;
 uint16_t s = 0;
 while (p->lth)
  {
   if ((p->h != 1) ||
       ((p->flg & (l_fbeg+l_fend)) == 0)) s++;
   p =(lnhead *)((uchar*)p + p->lth);
  }
 return s;
}

//------------------- Seek in events table -----------------------------
//	Structure of the seek table:
//	10 headers for each number of lines by 256 entries per header.
//	Each entry contains displacement in overall table of start of
//	chain for vectors with same number of lines and hash.
//	Structure of chain element:
//	Byte 1 - number of versions (0 - end of chain)
//	Part of vector includes topological code and lines codes
//	Versions



uchar taba[] = {0,1,3,6,10,15,21,28,36,45,55};
uchar tabb[] = {0,255,0,3,9,19,34,55,83,119,164,219};

static uint16_t rot(uint16_t n) { return ((n<<3) + (n>>13)); }

static uint16_t seek_events (uchar* ep)
{
 uint16_t hash,i,nl;
 uint32_t di;
 uchar* p;

 //if (db_status) snap_keep(snap_vector,ep,*ep+4);
 if(enable_save_stat)
 {
int i,n;
n=save_event_txt[0]=ep[0];
save_event_txt[1]=ep[1];
save_event_txt[2]=ep[2];
for(i=0;i<n;i++)
    save_event_txt[3+i]=ep[4+i];
 }
 nl = *ep;
 if( !nl )	return 0; // OLEG
 hash = *(ep+3) = (tabb[nl] + (nl-*(ep+1)< sizeof(taba)/sizeof(taba[0]) ? taba[nl-*(ep+1)]:0) + *(ep+2)) & 0xff;// Piter add control taba[]
 i = nl;
 p=ep+4;
 while (i--)
 { hash = rot(hash) ^ (uint16_t)(*(p++));
 }
 hash = (hash & 0xff) ^ (hash >> 8);
 p = ((nl-1) * 256 + hash) * sizeof(uint32_t) + events_tree;
 di = *((uint32_t *)p); if (di==0) return 0; p = events_tree+di;
 /* Oleg : 17-08-1994 : changed uint16_t */
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

//-------------------- Variations process --------------------------

static int16_t first_var()
{
 VAR *p=evvars;
 uchar *ev1, *ev2;
 uint16_t nl,n;
 if (p == evendvar) return -1;
 memcpy (evline1,evline,4); ev1 = evline+4; ev2 = evline1+4; nl =1;
var_loop:
 n = p->ln - nl;
 if (n) {nl = p->ln; memcpy (ev2,ev1,n); ev1+=n; ev2+=n;}
 *(ev2++) = *(ev1++) + p->add;
 if( evline[0]<nl )	 return -1; // OLEG
 n=evline[0]-nl; if (n) memcpy (ev2,ev1,n);
 //snap_keep (snap_variation,NULL,0);
 n = seek_events(evline1); if (n > 0) return n;
 if (++p == evendvar) return 0;
 ev1 = evline+3+nl; ev2=evline1+3+nl; goto var_loop;
}
