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


#include <setjmp.h>
#include <stdlib.h>
#include "struct.h"
#include "v1comp.h"

BWSS *locomp_seglist(uchar* raster, BWSS *bwsp, BWSS *bwe, int32_t height, int32_t width);
MN * c_locomp (uchar* raster, int32_t bw, int32_t h, int16_t upper, int16_t left);
//      Memory service
#define MAX_BOX_NUMB            100*2
#define MAX_INT_NUMB            32*2
#define LINE_POOL_LENGTH        512*20

static BOX * boxalloc;
static BOX * boxallocend;
static MN * mainalloc;
static MN * first_dead_comp;
static BWSS * segm_repr_end;
static BWSS * op, *np;
static int16_t ol, nl;
static int16_t rast_lc;
static int16_t lineno;


static BWSS lines[LINE_POOL_LENGTH+9];
static uchar boxes[BOXSIZE * MAX_BOX_NUMB];
static MN main_numbers[MAX_INT_NUMB];

   // Oleg : 18-08-1994 : link DIFFRV.C
   // Vald: 10-03-96 07:18pm set it const
uchar* segment_pool=(uchar *)lines;

#define end_line_pool  (lines + LINE_POOL_LENGTH)
#define box_alloc(bp) bp = boxalloc; boxalloc = (BOX *)((uchar*)boxalloc + BOXSIZE)

static jmp_buf locomp_err;

//      Internal functions

static void locomp_begin();
static void analize();
static void simple_cont();
static void new_line();
static void new_line_cont();
static void merge_line();
static void dead_line();

MN * c_locomp (uchar* raster, int32_t bw, int32_t h, int16_t upper, int16_t left)
{
 lineno = upper-1; rast_lc = left;
 if (setjmp(locomp_err)) return NULL;
 segm_repr_end = locomp_seglist (raster,lines,end_line_pool,h,bw);
 locomp_begin();
 do { lineno++; analize(); } while (np != segm_repr_end);
 return first_dead_comp;
}

static void locomp_begin()
{
 int16_t i;
 MN * mn;
 first_dead_comp = NULL;
 segm_repr_end->b = 0; (segm_repr_end++)->w = -0x7000;
 if (segm_repr_end >= end_line_pool)
	 longjmp(locomp_err,3);
 for (i = 0, mn = mainalloc = main_numbers; i < MAX_INT_NUMB-1; i++, mn++)
      mn->mnnext = mn+1;
 mn->mnnext = NULL; np = (op = lines) + 1;
 boxalloc = (BOX *) boxes; // Vald 06-15-96 07:06pm corr old Talalay error
 boxallocend = (BOX *)(boxes + MAX_BOX_NUMB * BOXSIZE);
}

static void analize()
{
 ol = nl = rast_lc;
 if (op->b == 0) ol += (op++)->w;
 if (np->b == 0) nl += (np++)->w;
 if (ol < 0) goto restnewline_b;
 if (nl < 0) goto restoldline_b;

gencase:
 if (ol != nl) { if (ol > nl) goto actnew; else goto actold; }
 ol += op->b; nl += np->b; simple_cont();

intersect_step:
 if (ol != nl) { if (ol > nl) goto nextnewsegm; else goto nextoldsegm; }

 nl += (np++)->w; if (nl < 0) goto restoldline;
 ol += (op++)->w; if (ol > 0) goto gencase; goto restnewline_b;

nextoldsegm:
 ol += (op++)->w; if (ol < 0) goto restnewline;
 if (ol > nl) goto new_segm_step;
 ol += op->b; merge_line(); goto intersect_step;

nextnewsegm:
 nl += (np++)->w; if (nl < 0) goto restoldline;
 if (nl > ol) goto old_segm_step;
 new_line_cont(); goto intersect_step;

linedies:       dead_line();
old_segm_step:
 ol += (op++)->w; if (ol > 0) goto gencase; goto restnewline_b;

newcomp:        new_line();
new_segm_step:
 nl += (np++)->w; if (nl > 0) goto gencase; goto restoldline_b;

actold:
 ol += op->b; if (nl > ol) goto linedies;
 nl += np->b; simple_cont(); goto intersect_step;

actnew:
 nl += np->b; if (ol > nl) goto newcomp;
 ol += op->b; simple_cont(); goto intersect_step;

restnewline_b: if (nl < 0) return;
restnewline_1: nl += np->b; new_line();
restnewline:   nl += (np++)->w; if (nl > 0) goto restnewline_1;
 return;

restoldline_b: if (ol < 0) return;
restoldline_1: ol += op->b; dead_line();
restoldline:   ol += (op++)->w; if (ol > 0) goto restoldline_1;
 return;
}

static void simple_cont()
{
 BOX * bp, *bpw;
 BOXINT * ip;
 MN *mn;
 bp = op->box; ip = (BOXINT *)((uchar*)bp + bp->boxptr);
 if (bp->boxptr > BOXBOUNDARY) goto fullbox; bp->boxptr += sizeof (*ip);
resume:
 np->box = bp; ip->l = np->b; ip->d = nl - ol;
 if (bp->boxright < nl) bp->boxright = nl;
 if (bp->boxleft > nl - ip->l) bp->boxleft = nl - ip->l;
 return;
fullbox:
 ip->l = -1; box_alloc(bpw);
 if (boxalloc == boxallocend) longjmp(locomp_err,1);
 bpw->boxmain = mn = bp->boxmain; mn->mnboxcnt++;
 bpw->boxnext = bp->boxnext; bp->boxnext = bpw;
 bp = bpw; ip = (BOXINT *)((uchar*)bp + sizeof(BOX));
 bp->boxptr = sizeof(BOX) + sizeof(BOXINT); bp->boxflag = 0;
 bp->boxleft = bp->boxright = nl; goto resume;
}

static void new_line()
{
 BOX *bp;
 MN *mn;
 LNSTRT *lp;
 box_alloc(bp); if (boxalloc == boxallocend) longjmp(locomp_err,1);
 if ((mn = mainalloc) == NULL) longjmp(locomp_err,3);
 mainalloc = mn->mnnext;

 mn->mnfirstbox = bp; mn->mncounter = mn->mnboxcnt = 1; mn->mnflag = 0;
 mn->mnlines = mn->mnbegs = 1; mn->mnends = 0;
 np->box = bp;
 bp->boxnext = NULL; bp->boxflag = BOXFREEBEG;
 bp->boxptr = sizeof(BOX) + sizeof(LNSTRT); lp = (LNSTRT *)(bp+1);
 lp->y = mn->mnupper = lineno; bp->boxmain = mn;
 bp->boxleft = nl -  (lp->l = np->b); bp->boxright = lp->x = nl;
}

static void new_line_cont()
{
 BOX *bp, *bwp;
 MN * mn;
 LNSTRT * lp;
 bwp = (np-1)->box; mn = bwp->boxmain;
 box_alloc(bp); if (boxalloc == boxallocend) longjmp (locomp_err,1);
 np->box = bp; bp->boxnext = bwp->boxnext; bwp->boxnext = bp;
 mn->mnlines++; mn->mncounter++; mn->mnboxcnt++;
 bp->boxptr = sizeof(BOX) + sizeof(LNSTRT); lp = (LNSTRT *)(bp+1);
 bp->boxmain = mn; bp->boxflag = BOXBEG;
 lp->y = lineno; bp->boxleft = nl;
 bp->boxright = lp->x = nl += (lp->l = np->b);
}

static void merge_line()
{
 MN *mn, *mno, *mnw;
 BOX *bp, *bpo;
 BOXINT *ip;
 int16_t n;

 bpo = op->box; bpo->boxflag |= BOXEND;
 ip = (BOXINT *)((uchar*)bpo + bpo->boxptr); ip->l = 0; bpo->boxptr++;
 bpo->boxey = lineno; bpo->boxex = ol; bpo->boxel = op->b;

 bp = np->box; mn = bp->boxmain; mno = bpo->boxmain;
 if (mn == mno) { mn->mncounter--; return; }
 n = mno->mnboxcnt;
 if ((n > usual_box_count) && (n > mn->mnboxcnt))
  { n = mn->mnboxcnt; mnw = mno; mno = mn; mn = mnw; }
 mn->mnflag |= mno->mnflag; mn->mncounter += mno->mncounter - 1;
 mn->mnlines += mno->mnlines; mn->mnbegs += mno->mnbegs;
 mn->mnends += mno->mnends; mn->mnboxcnt += n;
 if (mno->mnupper < mn->mnupper) mn->mnupper = mno->mnupper;
 bp = mn->mnfirstbox; mn->mnfirstbox = bpo = mno->mnfirstbox;
 mno->mnnext = mainalloc; mainalloc = mno;
 while (--n) { bpo->boxmain = mn; bpo = bpo->boxnext;}
 bpo->boxmain = mn; bpo->boxnext = bp;
}

static void dead_line()
{
 BOX *bp;
 MN *mn;
 BOXINT *ip;
 bp = op->box; ip = (BOXINT *)((uchar*)bp + bp->boxptr++); ip->l = 0;
 bp->boxey = lineno; bp->boxex = ol; bp->boxel = op->b;
 bp->boxflag |= BOXFREEEND;
 mn = bp->boxmain; mn->mnends++; if (--(mn->mncounter)) return;
 mn->mnnext = first_dead_comp; first_dead_comp = mn;
 mn->mnlower = lineno;
}
