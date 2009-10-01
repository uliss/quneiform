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
#include <string.h>
#include "globus.h"
#include "struct.h"
#include "v1comp.h"
#include "msgerr.h"

extern  uchar   fax1x2;
extern  int32_t   box_number;
//------------------------ Common working fields

extern uchar double_fax;
extern uchar image_disp_mask;
extern uint16_t image_disp_end;
extern uint16_t image_disp_byte;

extern int16_t image_blth    ;  // pixels per line
extern int16_t  image_height  ;  // lines in file number
extern int16_t image_lth     ;  // bytes per line
extern uchar image_black   ;  // mask for black pixels adding
extern uchar image_white   ;  // mask for wite pixels adding
extern struct main_memory_str Q;

void analise();
BWS *extrcomp_seglist(uchar* raster, BWS *bwsp, BWS *bwe, int16_t width);
uint32_t progress_set_percent (uint32_t volume);
int16_t source_read(uchar* start, uchar* ptr, uchar* end);

//---------------------- Internal working fields
// TGCV static int32_t progress_next;

//--------------------- Internal functions
static void enough_memory();
static void initdsect();
static void allocboxes();
static void begin();
static uint16_t readline();
static void exchangelines();
static void fax_double();
static int16_t fax_test();
static void emptyline();
/*
static MN   * Cmnstart;
static BWS  * Cline1;
static BWS  * Cline2;
static uchar * Cscan;*/
/*
comp_mem_alloc()
{
  Cmnstart = malloc ( sizeof (MN)  * SEG_MAX_NUM);
  Cline1   = malloc ( sizeof (BWS) * SEG_MAX_NUM);
  Cline2   = malloc ( sizeof (BWS) * SEG_MAX_NUM);
  Cscan    = malloc (0x4000 + LINE_MAX_LTH);
}
  */

//========================== Extract components pass ====================

void extrcomp()
{
 // TGCV prop_init();
 // TGCV rules_open();
 // TGCV sweeper_ini();
 // TGCV progress_next = progress_set_step (image_height + 1);
 initdsect();
 //enough_memory();
 allocboxes();
 // TGCV save_comp_ini();
 double_fax = fax1x2;
 begin();

 if (double_fax)
  {
fax_loop:
   Q.lineno++;
   if (!readline()) goto lastline;
   analise();
   Q.lineno++;
   if (!readline()) { fax_double(); analise(); goto lastline; }
   if (!fax_test()) error_exit (ERR_comp,14);   // not a fax image
   analise();
// TGCV    if (Q.lineno >= progress_next)
   progress_set_percent (Q.lineno);
   goto fax_loop;
  }

main_loop:
 Q.lineno++;
 if (readline())
  {
   analise();
 // TGCV   if (Q.lineno >= progress_next)
   progress_set_percent (Q.lineno);
   goto main_loop;
  }

lastline:
 exchangelines();
 emptyline();
 analise();
}
  /*
static void enough_memory()
{
 Q.boxalloc = (BOX *)box_pool;
 root_file = (PROOT) (((uchar*)Q.boxalloc) + BOX_NUMBER*BOXSIZE);
 if (memory_pool_end <= (uchar*)root_file) error_exit(ERR_comp,9);
}
    */
static void initdsect()
{
 MN *p;
 int16_t i;
 /*memset (&Q, 0, sizeof(Q));

 Q.mnstart    = Cmnstart;
 Q.line1start = Cline1;
 Q.line2start = Cline2;
 Q.scan_buffer = Cscan;
   */
 memset (Q.mnstart,      0 , sizeof (MN)  * SEG_MAX_NUM);
 memset (Q.line1start,   0 , sizeof (BWS) * SEG_MAX_NUM);
 memset (Q.line2start,   0 , sizeof (BWS) * SEG_MAX_NUM);
 /*memset (Q.scan_buffer,  0 , 0x4000 + LINE_MAX_LTH);
   */
 Q.lineno = 0;
 p = Q.mainalloc = Q.mnstart;
 for (i = 0; i < SEG_MAX_NUM-1; p++, i++) p->mnfirstbox = (BOX *)(p+1);
 p->mnfirstbox = NULL;
 Q.oldline = Q.line1start; Q.newline = Q.line2start;
}

static void allocboxes()
{
 BOX *b;
 int32_t i;
 for (b = Q.boxalloc = Q.boxstart, i=0; i<box_number-1; i++,
      b = (BOX *)((uchar*)b + BOXSIZE))
  b->boxnext = (BOX *)((uchar*)b + BOXSIZE);
 b->boxnext = NULL;
}

static void begin()
{
 Q.dcodeptr = Q.scan_buffer; Q.dcodeend = Q.dcodeptr + image_lth;
 emptyline();
}

static void emptyline()
{
 Q.newline->b=0; Q.newline->w=-0x7000;
}

static void exchangelines()
{
 BWS *p;
 p = Q.oldline; Q.oldline = Q.newline; Q.newline = p;
}

static uint16_t readline()
{
 uchar* p;
 int16_t i;
 p =  Q.dcodeptr + image_lth;
after_read:
 if (p + image_lth > Q.dcodeend) goto rd_source;

 Q.dcodeptr = p; exchangelines();
 p+=image_disp_byte;
 *p &= image_disp_mask; *(p+image_disp_end-1) &= image_black;
 extrcomp_seglist (p,Q.newline,Q.newline + SEG_MAX_NUM,
                   image_disp_end);
 return 1;

rd_source:
 i = source_read (Q.scan_buffer, p, Q.dcodeend);
 if ( i<=0 ) return 0;
 Q.dcodeend = Q.dcodeend - p + i + Q.scan_buffer;
 p = Q.scan_buffer;     goto after_read;
}

static int16_t fax_test()
{
 BWS * old, * new;
 old = Q.oldline; new = Q.newline;
 while (1)
  {
   if ((old->b != new->b) || (old->w != new->w)) return 0;
   if (old->w < 0) return 1;
   old++; new++;
  }
}

static void fax_double()
{
 BWS * old, * new;
 exchangelines();
 old = Q.oldline; new = Q.newline;
 while (1)
  {
   new->b = old->b; new->w = old->w; new->box = NULL;
   if (old->w < 0) return;
   old++; new++;
  }
}
