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

#include <stdio.h>
#include "globus.h"
#include "struct.h"
//#include "func.h"
#include "v1comp.h"
#define Ecom struct main_memory_str
//#include "status.h"
#include "msgerr.h"

extern uchar double_fax;
extern BOX *boxchain, *dl_last_in_chain;
extern MN * main_number_ptr;
extern struct main_memory_str Q;

extern Bool16 component_account(void);
extern void alone_comp(void);
extern void picture_process(MN * mn);
extern void SpecCompPut (MN * mn);

void set_active_MN(MN * mnnew,MN * mnold); // pic_apx.c
MN * accept_greate_picture(MN * mn);        // pic_apx.c

static BWS *op, *np;    // pointers to segment representation
static int16_t ol,nl;       // location counters

static void simple_cont();
static void dead_line();
static void new_line();
static void merge_lines();
static void new_line_cont();
static void no_box();

void analise()
{
 op = Q.oldline; np = Q.newline; ol = nl = 0;
 if (op->b == 0) ol+=(op++)->w;
 if (np->b == 0) nl+=(np++)->w;
 if (ol < 0) goto restnewline_b;
 if (nl < 0) goto restoldline_b;

gencase:
 if (ol != nl) goto actnewold;
 ol += op->b; nl += np->b; simple_cont();

intersect_step: // compare segment ends nl and ol
 if (nl == ol)
  {
   nl += (np++)->w; if (nl < 0) goto restoldline;
   ol += (op++)->w; if (ol > 0) goto gencase; else goto restnewline_b;
  }

 if (ol < nl)
  {
   ol += (op++)->w; if (ol < 0) goto restnewline;
   if (ol > nl)
    { nl += (np++)->w; if (nl > 0) goto gencase; else goto restoldline_b; }
   ol += op->b; merge_lines(); goto intersect_step;
  }
 else
  {
   nl += (np++)->w; if (nl < 0) goto restoldline;
   if (nl > ol)
    { ol += (op++)->w; if (ol > 0) goto gencase; else goto restnewline_b; }
   new_line_cont(); goto intersect_step;
  }

actnewold:
 if (ol < nl)
  {
   ol += op->b;
   if (ol < nl)
   {
    dead_line();
    ol += (op++)->w; if (ol > 0) goto gencase; else goto restnewline_b;
   }
   nl += np->b; simple_cont(); goto intersect_step;
  }
 else
  {
   nl += np->b;
   if (nl < ol)
    {
     new_line();
     nl += (np++)->w; if (nl > 0) goto gencase; else goto restoldline_b;
    }
   ol += op->b; simple_cont(); goto intersect_step;
  }

restnewline_b:
 if (nl < 0) return;
restnewline_1: nl += np->b; new_line();
restnewline: nl += (np++)->w; if (nl > 0) goto restnewline_1; return;

restoldline_b:
 if (ol < 0) return;
restoldline_1: ol += op->b; dead_line();
restoldline: ol += (op++)->w; if (ol > 0) goto restoldline_1; return;
}

static void simple_cont()
{
 BOX * bp, *bpp;
 BOXINT * ip;
 MN * mn;

reset:
 bp = op->box;
 ip = (BOXINT *) ((uchar*)bp + bp->boxptr);
 if (bp->boxptr > BOXBOUNDARY) goto fullbox;
fret:
 ip->l = np->b; ip->d = nl-ol; bp->boxptr += sizeof(BOXINT);
 if (nl > bp->boxright) bp->boxright = nl;
 if (nl - ip->l < bp->boxleft) bp->boxleft = nl-ip->l;
 np->box = bp; return;

fullbox:
 if (bp->boxptr == BOXSIZE)     // double fax forcing
  { bp->boxptr = sizeof(BOX) + sizeof(LNSTRT); np->box = bp; return; }

 ip->l = -1;
 bpp = Q.boxalloc; if (bpp == NULL) goto compress; Q.boxalloc = bpp->boxnext;
 bpp->boxnext = bp->boxnext; bp->boxnext = bpp;
 mn = bpp->boxmain = bp->boxmain; mn->mnboxcnt++; bp = bpp;
 bp->boxleft = (bp->boxright = nl) - np->b; bp->boxflag = 0;
 bp->boxptr = sizeof(BOX); ip = (BOXINT *) ((uchar*)bp + sizeof (BOX));
 goto fret;

compress: no_box(); goto reset;
}

static void new_line()
{
 BOX * bp;
 MN * mn;
 LNSTRT * lp;
reset:
 if ((bp = Q.boxalloc) == NULL) goto compress;
 if ((mn = Q.mainalloc) == NULL) error_exit (ERR_comp, 3);
 Q.mainalloc = (MN *) (mn->mnfirstbox);

 mn->mnfirstbox = bp; mn->mncounter = mn->mnboxcnt = 1; mn->mnflag = 0;
 mn->mnlines = mn->mnbegs = 1; mn->mnends = 0;

 Q.boxalloc = bp->boxnext; bp->boxnext = NULL; np->box = bp;
 bp->boxflag = BOXFREEBEG; bp->boxptr = sizeof(BOX) + sizeof(LNSTRT);
 lp = (LNSTRT *)((uchar*)bp + sizeof(BOX));
 mn->mnupper = lp->y = Q.lineno; bp->boxmain = mn;
 bp->boxright = lp->x = nl; bp->boxleft = nl - (lp->l = np->b);
 if (double_fax)  { mn->mnupper++; lp->y++; bp->boxptr = BOXSIZE; }
 return;

compress:
 no_box();      goto reset;
}

static void new_line_cont()
{
 MN * mn;
 BOX *bp, *bpp;
 LNSTRT *lp;
 mn = (bpp = (np-1)->box)->boxmain;
reset:
 bp = Q.boxalloc; if (bp == NULL) goto compress;
 Q.boxalloc = bp->boxnext; bp->boxnext = bpp->boxnext; bpp->boxnext = bp;
 np->box = bp;

 mn->mnlines++; mn->mncounter++; mn->mnboxcnt++;

 bp->boxptr = sizeof(BOX) + sizeof(LNSTRT); bp->boxmain = mn;
 bp->boxflag = BOXBEG; bp->boxleft = nl;
 lp = (LNSTRT *)((uchar*)bp + sizeof(BOX));
 nl += (lp->l = np->b); bp->boxright = lp->x = nl; lp->y = Q.lineno;
 return;

compress: no_box(); goto reset;
}

static void merge_lines()
{
 BOX *bp, *bpp;
 MN *mn, *mnn, *mnw;
 BOXINT *ip;
 uint16_t n;

 bp = op->box; ip = (BOXINT *)((uchar*)bp + (bp->boxptr)++); ip->l = 0;
 bp->boxflag |= BOXEND;
 bp->boxey = Q.lineno; bp->boxel = op->b; bp->boxex = ol;

 mn = bp->boxmain; bpp = np->box; mnn = bpp->boxmain;
 if (mn == mnn) { mn->mncounter--; return; }

 if ((n = mn->mnboxcnt) > usual_box_count)
  {if (n > mnn->mnboxcnt) {n = mnn->mnboxcnt; mnw = mnn; mnn = mn; mn = mnw;}}

 set_active_MN(mnn,mn); // Vald for picture extraction

 mnn->mnflag |= mn->mnflag; mnn->mncounter += mn->mncounter - 1;
 mnn->mnlines += mn->mnlines; mnn->mnbegs += mn->mnbegs;
 mnn->mnends += mn->mnends; mnn->mnboxcnt += n;
 if (mn->mnupper < mnn->mnupper) mnn->mnupper = mn->mnupper;

 bp = mn->mnfirstbox; bpp = mnn->mnfirstbox; mnn->mnfirstbox = bp;
 mn->mnfirstbox = (BOX *) Q.mainalloc; Q.mainalloc = mn;
 while (--n) { bp->boxmain = mnn; bp = bp->boxnext; }
 bp->boxmain = mnn; bp->boxnext = bpp;
 return;
}


static void dead_line()
{
 BOX *bp, *bpp;
 BOXINT *ip;
 MN *mn;
 uint16_t lnum;
 Bool16 lcomp;

//      Double Fax preprocessing

resume_fax:
 bp = op->box; ip = (BOXINT *)((uchar*)bp + bp->boxptr); lnum = Q.lineno;

 if (double_fax)
  {
   lnum--;
   if (bp->boxptr > sizeof(BOX) + sizeof (LNSTRT))
    { (ip-1)->l = 0; bp->boxptr -= sizeof (BOXINT) + 1; goto faxend;}
   if (bp->boxptr == sizeof(BOX) + sizeof(LNSTRT))
    { ip->l = 0; mn = bp->boxmain;
      if (mn->mnupper == lnum)
       {mn->mnupper--; ((LNSTRT *)((uchar*)bp + sizeof(BOX)))->y--; }
      goto faxend;
    }
   mn = bp->boxmain; mn->mnboxcnt--; bpp = mn->mnfirstbox;
   while (bpp->boxnext != bp) bpp = bpp->boxnext;
   bpp->boxnext = bp->boxnext; bp->boxnext = Q.boxalloc;
   Q.boxalloc = bp; bp = bpp; op->box = bp; goto resume_fax;
  }
 else
  {
   ip->l = 0; bp->boxptr++;     // if regukar case
  }

faxend:

//      Finish the line
 bp->boxey = lnum; bp->boxex = ol; bp->boxel = op->b;
 bp->boxflag |= BOXFREEEND;
 mn = bp->boxmain; mn->mnends++;
 if (--(mn->mncounter)) return;

//      Component dies
 mn->mnlower = lnum; main_number_ptr = mn;
 lcomp = component_account();
 bp = Q.boxalloc; Q.boxalloc = boxchain; dl_last_in_chain->boxnext = bp;
 if (lcomp)
    alone_comp();
 else
    picture_process (mn);
 mn->mnfirstbox = (BOX *)(Q.mainalloc); Q.mainalloc = mn; return;
}

static void no_box()
{
 BWS * bwp;
 BOX *bp, *bpp, *bww;
 MN *mn, *mmax;
 LNSTRT *lp;
 int16_t left, right;
 uint16_t n, i;
 //     Clear references to box in the new line
 bwp = np; do bwp->box = NULL; while ((bwp++)->w > 0);
 //     Look for main number with maximal box number
 for (i = 0, n=0, mn = mmax = Q.mnstart; i<SEG_MAX_NUM; i++, mn++)
  {
   if ((mn->mnboxcnt > n) && mn->mncounter )
    { n = mn->mnboxcnt; mmax = mn; }
   else
    if (mn->mnboxcnt == 0) break;
  }
 SpecCompPut(mmax);     // Save information about component to delete
 accept_greate_picture(mmax); // Vald for picture extaction

 // Mark all boxes within component
 for (i=0, bp = mmax->mnfirstbox; i<n; i++, bp = bp->boxnext) bp->boxwf = 1;

 for (i=0, bwp = Q.oldline; i<2; i++, bwp=Q.newline)
  {
   n=0; do
    {
     if ((bp=bwp->box) != NULL)
       if (bp->boxmain == mmax)
        {
         bp->boxwf = 0; bp->boxptr = sizeof(BOX) + sizeof(LNSTRT);
         lp = (LNSTRT *)((uchar*)bp + sizeof(BOX));
         lp->y = Q.lineno - 1 + i; lp->x = n + (lp->l = bwp->b);
         bp->boxflag = BOXFREEBEG;
        }
     n += bwp->b + bwp->w;
    } while ((bwp++)->w > 0);
  }


 n= mmax->mnboxcnt; bpp = NULL; mmax->mnflag = mnpicture; mmax->mnboxcnt = 0;
 bp = mmax->mnfirstbox; left = bp->boxleft; right = bp->boxright;

sep_loop:
 if (bp->boxleft <= left) left = bp->boxleft;
 if (bp->boxright >= right) right = bp->boxright;
 if (bp->boxwf)
  {
   bp->boxmain = NULL;
   bww = bp->boxnext; bp->boxnext = Q.boxalloc; Q.boxalloc = bp;
   if ((bp = bww) != NULL) goto sep_loop; else goto sep_end;
  }
 if (bpp) bpp->boxnext = bp; else mmax->mnfirstbox = bp;
 bpp = bp; bp = bp->boxnext; mmax->mnboxcnt++;
 if (--n == 0) error_exit(ERR_comp,4);
 if (bp) goto sep_loop;

sep_end:
 bpp->boxnext = NULL; bpp = mmax->mnfirstbox;
 bpp->boxleft = left; bpp->boxright = right;
}
