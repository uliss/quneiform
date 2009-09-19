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


/********************************************************************/
/*                                                                  */
/*                 Sweeping of small garbage                        */
/*                                                                  */
/********************************************************************/

#include <stdlib.h>
#include <string.h>

#include "struct.h"
#include "status.h"
#include "func.h"
#include "ligas.h"
#include "lang.h"

extern uint16_t actual_resolution;
extern int16_t idshv;

static int16_t nx,ny,mstb;
static uchar scsweep[512];
static uchar work_state=0;
static uint32_t offset;

static void stat_garb(PWORD);
static uint16_t delete(PWORD);
static uint16_t to_file();
static void open_comp_file();
static void write_comp(pchar,int32_t);

#define ROK       50
#define NBOXMAX   4096
#define BXSZ      ((int16_t)((64*(int32_t)actual_resolution)/300))
#define SMALL     ((8*actual_resolution)/300)
#define BNDBX     32
#define BND1BX    16


static Bool badcell(cell *);
static Bool near_garb(cell *);
static Bool badglue(cell **,int16_t);

#define BIGANGLE  100
#define NLETBLN    10
#define GARBLETMAX  3
#define DEFBASE    10

extern int16_t nIncline;
extern int16_t page_nIncline;

int16_t delgarb()
{
int16_t s,t,b3,ps,max;
cell *c,*c1,*c2;
B_LINES bl;
extern Bool line_readyBL, line_handfragment;

if (abs(nIncline-page_nIncline)>BIGANGLE)
    {
    for (s=t=0,c=cell_f()->nextl; c->next!=NULL; c=c->nextl,s++)
        {
        if (c->cg_flag&(c_cg_cutr|c_cg_cutl))
            t++;
        }
    if (s<=NLETBLN && 2*t>=s)
        return 0;
    }

for (s=0,c1=c=cell_f()->next; c->next!=NULL; c=c->next)
    {
    if (!(c->cg_flag&c_cg_cutr) &&
        !badcell(c) && (!near_garb(c) || (++s)>GARBLETMAX)
      || c->cg_flag&c_cg_cutr && !badglue(&c,1)
      || ( c->flg&c_f_let &&language==LANG_RUSSIAN)) // Piter
        break;
    if (c->next->flg&c_f_fict)
        return 0;
    get_b_lines(c,&bl);
    b3=bl.b3;
    ps=bl.ps;
    get_b_lines(c->next,&bl);
    if (abs(b3-bl.b3)>=DEFBASE ||
      c->next->col-(c->col+c->w)>=
		      ((language==LANG_FRENCH)?bl.ps+ps:3*(bl.ps+ps)/4))
    c1=c->next;
    }

for (c=cell_f()->next; c!=c1; c=c2)
    {
    c2=c->next;
    get_b_lines(c,&bl);
    if(!(language==LANG_RUSSIAN && c->w>bl.ps/2 && c->w>c->h*5))
        {
        if( line_handfragment )
            set_bad_cell(c);
        else
            del_cell(c);
        }
    }

for (s=0,c1=c=cell_l()->prev; c->prev!=NULL; c=c->prev)
    {
    if (!(c->cg_flag&c_cg_cutl) &&
        !badcell(c) && (!near_garb(c) || (++s)>GARBLETMAX) ||
         c->cg_flag&c_cg_cutl && !badglue(&c,-1) ||
        (c->flg&c_f_let && language==LANG_RUSSIAN)) // Pit
        break;
    if (c->prev->flg&c_f_fict)
        return 0;
    get_b_lines(c,&bl);
    b3=bl.b3;
    if( line_readyBL && /*abs(c->row-b3)<MAX(c->h/2,1)*/
        c->row-1<=b3 && c->row+c->h+2>=b3 )
        break; // OLEG for tables
    ps=bl.ps;
    get_b_lines(c->prev,&bl);
    for (max=-1000,c2=c->prev; c2 && c2->col==c->prev->col; c2=c2->prev)
        {
        if (max<c2->col+c2->w)
            max=c2->col+c2->w;
        }
    if (abs(b3-bl.b3)>=DEFBASE ||
      c->col-max>=((language==LANG_FRENCH)?bl.ps+ps:3*(bl.ps+ps)/4))
        c1=c->prev;
    }

for (c=cell_l()->prev; c!=c1; c=c2)
    {
    c2=c->prev;
    if( line_handfragment )
            set_bad_cell(c);
    else
            del_cell(c);
    }

if ((c=cell_f()->next)->vers[0].let==liga_bull && c->next->flg==c_f_fict)
    return 0;

return 1;
}

extern uchar line_scale;

#define WHPROP     30
#define PRBAD     100
#define PRSTBAD   150
#define DELST      50
#define PROBGL    120

static Bool badcell(cell *c)
 {
 uchar let,prob;

 if (c->flg&(c_f_dust|c_f_punct|c_f_bad) || c->w-c->h/WHPROP<=3 ||
     (prob=c->vers[0].prob)<PRBAD ||
     (strchr("fijlrtIL1/()",(let=c->vers[0].let)) ||
		let==liga_i ||
		 language == LANG_TURKISH &&  // 30.05.2002 E.P.
			(let==i_sans_accent||let==II_dot_accent)
	 ) &&
							 prob<PRSTBAD ||
     strchr("<>%",let))
  return TRUE;
 return FALSE;
 }

static Bool near_garb(cell *c)
 {
 int16_t i,imi,ima,j,jmi,jma,n;

 imi=(c->row<<line_scale)/BXSZ;
 ima=((c->row+c->h-1)<<line_scale)/BXSZ;
 jmi=(c->col<<line_scale)/BXSZ;
 jma=((c->col+c->w-1)<<line_scale)/BXSZ;
 for (i=imi; i<=ima; i++)
  for (j=jmi; j<=jma; j++)
   if (scsweep[(n=i*nx+j)/8]&(128>>(n%8)))
    return TRUE;
 return FALSE;
 }

static Bool badglue(cell **pc,int16_t direct)
 {
 int16_t s,n;
 cell *c;
 uchar let;

 for (s=n=0,c=*pc;
	!(c->flg&c_f_fict) && c->cg_flag&((direct>0)?c_cg_cutr:c_cg_cutl);
					     c=((direct>0)?c->next:c->prev))
  {
  if (!(c->flg&c_f_bad))
   {
   if (strchr("fijlrtIL1/()",(let=c->vers[0].let)) ||
	   let==liga_i ||
		 language == LANG_TURKISH &&  // 30.05.2002 E.P.
			(let==i_sans_accent||let==II_dot_accent)
	  )
    s+=c->vers[0].prob-DELST;
   else
    s+=c->vers[0].prob;
   }
  n++;
  }
 if (s/n<PROBGL)
  {
  *pc=((direct>0)?c->prev:c->next);
  return TRUE;
  }
 return FALSE;
 }
