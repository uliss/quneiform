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



#include "struct.h"
#include "func.h"

uint16_t all_scalarf (uint16_t* v1, uint16_t* v2, uint16_t norm)      //AK 14.03.97
{                              //AK definition from scalar.asm
uint32_t v;
 v = ((uint32_t)(*(v1+0)))  * (*(v2+0))  +
     ((uint32_t)(*(v1+1)))  * (*(v2+1))  +
     ((uint32_t)(*(v1+2)))  * (*(v2+2))  +
     ((uint32_t)(*(v1+3)))  * (*(v2+3))  +
     ((uint32_t)(*(v1+4)))  * (*(v2+4))  +
     ((uint32_t)(*(v1+5)))  * (*(v2+5))  +
     ((uint32_t)(*(v1+6)))  * (*(v2+6))  +
     ((uint32_t)(*(v1+7)))  * (*(v2+7))  +
     ((uint32_t)(*(v1+8)))  * (*(v2+8))  +
     ((uint32_t)(*(v1+9)))  * (*(v2+9))  +
     ((uint32_t)(*(v1+10))) * (*(v2+10)) +
     ((uint32_t)(*(v1+11))) * (*(v2+11)) +
     ((uint32_t)(*(v1+12))) * (*(v2+12)) +
     ((uint32_t)(*(v1+13))) * (*(v2+13)) +
     ((uint32_t)(*(v1+14))) * (*(v2+14));
 return (uint16_t)(v/norm);
}

int32_t all_scalar (uint16_t* v1, uint16_t* v2)               //AK 14.07.97
{                                 //definition from scalar.asm
uint32_t v;
 v = ((uint32_t)(*(v1+0)))  * (*(v2+0))  +
     ((uint32_t)(*(v1+1)))  * (*(v2+1))  +
     ((uint32_t)(*(v1+2)))  * (*(v2+2))  +
     ((uint32_t)(*(v1+3)))  * (*(v2+3))  +
     ((uint32_t)(*(v1+4)))  * (*(v2+4))  +
     ((uint32_t)(*(v1+5)))  * (*(v2+5))  +
     ((uint32_t)(*(v1+6)))  * (*(v2+6))  +
     ((uint32_t)(*(v1+7)))  * (*(v2+7))  +
     ((uint32_t)(*(v1+8)))  * (*(v2+8))  +
     ((uint32_t)(*(v1+9)))  * (*(v2+9))  +
     ((uint32_t)(*(v1+10))) * (*(v2+10)) +
     ((uint32_t)(*(v1+11))) * (*(v2+11)) +
     ((uint32_t)(*(v1+12))) * (*(v2+12)) +
     ((uint32_t)(*(v1+13))) * (*(v2+13)) +
     ((uint32_t)(*(v1+14))) * (*(v2+14));
 return (int32_t)v;
}

static uchar sqrt_tab[] = {
 0,
 1, 1, 1,
 2, 2, 2, 2, 2,
 3, 3, 3, 3, 3, 3, 3,
 4, 4, 4, 4, 4, 4, 4, 4, 4,
 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9,
10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,
11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,
12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,
13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,
14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,
15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15
};

int16_t long_sqrt (uint32_t n)
{
 uint16_t w;
uint32_t d, step, lw;
 uchar sh;
 if ((w = (uint16_t)(n >> 16)) != 0) sh = 8;
 else { w = (uint16_t)n; sh = 0; }

 if ( w & 0xff00 ) { sh +=4; w = w >> 8; }
 d = sqrt_tab[w]+1; d = d << sh; if ((d & 0xffff) == 0) return 0;

sq_loop:
 d -= (step = (lw = (d * d - n))/ (2*d)); if (step) goto sq_loop;
 if (lw >= d-1) d--;
 return (int16_t) d;
}

//	Add component to box raster

extern c_comp wcomp;
extern uchar lpool[];

static void make_box_raster(uint16_t* matr, c_comp * cp,
     int16_t row, int16_t col, int16_t h, int16_t w, lnhead * lp);



void comp_to_box (uint16_t* matr, c_comp * cp,
	uint16_t row, uint16_t col, uint16_t h, uint16_t w)
{
 make_box_raster (matr, cp, row, (int16_t)((cp->left - col)*3), h, w,
   (lnhead *)((puchar)cp + cp->lines + sizeof(uint16_t)));
}

void wcomp_to_box (uint16_t* matr)
{
 make_box_raster (matr, &wcomp, wcomp.upper, 0, wcomp.h, wcomp.w,
   (lnhead *)lpool);
}

static void make_box_raster(uint16_t* matr, c_comp * cp,
     int16_t row, int16_t add_col, int16_t h, int16_t w, lnhead * lp)
{
 int16_t r3, row_rest, w2;
 uint16_t* pm;
uint16_t * p;
 int16_t xs, xe, s1, s2, s3;
 interval * ip;

 w2 = w+w;
next_line:
 r3 = (lp->row + cp->upper - row) * 5;
 row_rest = h - r3 % h; pm = matr + (r3/h)*3;
 ip = (interval *)(lp+1);

 while (ip->l)
  {
//		Count segment dispersion
   xe = ip->e * 3 + add_col; xs = xe - (ip++)->l * 3;
   s1 = s2 = s3 = 0;
   if (xs < w)
    {
     if (xe <= w) { s1 = xe - xs; goto fint; }
     s1 = w - xs; xs = w; goto sec_cell;
    }
   if (xs < w2)
    {
sec_cell:
     if (xe <= w2) {s2 = xe - xs; goto fint; }
     s2 = w2 - xs; s3 = xe - w2;
    }
   else s3 = xe - xs;
fint:;

//	Add segment to box raster
   if ((row_rest -= 5) >= 0)
    { *pm += s1*5; *(pm+1) += s2*5; *(pm+2) += s3*5; continue; }
   p = pm; pm+=3; row_rest+=h;
   switch (h - row_rest)
    {
     case 1: *pm += s1; *(pm+1) += s2; *(pm+2) += s3;
	   *p += s1*4; *(p+1) += s2*4; *(p+2) += s3*4;	break;
     case 2: *pm += s1*2; *(pm+1) += s2*2; *(pm+2) += s3*2;
	   *p += s1*3; *(p+1) += s2*3; *(p+2) += s3*3;	break;
     case 3: *pm += s1*3; *(pm+1) += s2*3; *(pm+2) += s3*3;
	   *p += s1*2; *(p+1) += s2*2; *(p+2) += s3*2;	break;
     case 4: *pm += s1*4; *(pm+1) += s2*4; *(pm+2) += s3*4;
	   *p += s1; *(p+1) += s2; *(p+2) += s3;	break;
     case 5: *pm += s1*5; *(pm+1) += s2*5; *(pm+2) += s3*5;
							break;
    }
  }
 lp = (lnhead *)((puchar)lp + lp->lth);
 if (lp->lth != 0) goto next_line;
}

int16_t proport(uint16_t best_cos,uint16_t next_cos,uint16_t prev_cos,
		int16_t next_res,int16_t full_res)
{
 return
  ((best_cos - prev_cos) * (next_res - full_res))/(next_cos - prev_cos)
	 + full_res;
}
