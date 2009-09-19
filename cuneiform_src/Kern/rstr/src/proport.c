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
#include "lang.h"
#include "ligas.h"

//		Common variables

extern c_comp wcomp;
//extern version * start_rec;
//extern uchar records_change;

//
//		Proportional criteria
//
//
//

extern int16_t prop_l_delta, prop_r_delta;
extern uchar * letters_pidx_table;
void pidx_crit ()
{
	/*
 int16_t pidx;
 version *v;
 int16_t i;
 uint16_t let;
 pidx = prop_index (wcomp.h, wcomp.w);
 wcomp.pidx = (uchar)pidx;
 for (i=0, v=start_rec; i<wcomp.nvers; i++, v++)
  {
   let = (uchar)(v->let) * 2;
   if (pidx + prop_l_delta < letters_pidx_table[let])
    {
     wcomp.reasno |= c_rn_left;
false:
     v->prob = 0; records_change = 1; continue;
    }
   if (pidx - prop_r_delta > letters_pidx_table[let+1])
    { wcomp.reasno |= c_rn_right;	goto false; }
  }
  */
}

void v2_pidx_crit (cell *c)
{
 int16_t pidx;
 version *v, *wv;
 uint16_t let;
 // Paul 07-12-99
 if (c->nvers==0)
   return;
 //
 c->reasno=0; pidx = prop_index(c->h, c->w); wv = v = c->vers;
 while (v->let)
  {
   if( !c->accent_leader &&
	   (is_cen_language(language)	||
	    is_baltic_language(language)||	// 09.07.2001 E.P.
	    is_turkish_language(language)	// 21.05.2002 E.P.
	   )
	 )
    let = (wv->let=v->let) * 2;
   else
    let = (let_sans_acc[wv->let=v->let]) * 2;
   wv->prob = v->prob;
   if (pidx + prop_l_delta < letters_pidx_table[let])
	{ c->reasno |= c_rn_left; v++; continue; }
   if (pidx - prop_r_delta > letters_pidx_table[let+1])
	{ c->reasno |= c_rn_right; v++; continue; }
   v++; wv++;
  }
 wv->let = 0;
 if ((c->nvers -= v-wv) == 0) set_bad_cell(c);
}

Bool pidx_skip(int16_t h, int16_t w,uchar t_let)
{
  int16_t pidx;
  uint16_t let;

  pidx=prop_index(h,w);
  let = t_let <<1;
  if( (pidx + prop_l_delta < letters_pidx_table[let]) ||
    (pidx - prop_r_delta > letters_pidx_table[let+1]) ) return FALSE;
  else return TRUE;
}

uint16_t el_pidx_crit(uint16_t t_let,int16_t pidx)
{
  uint16_t let;

  let = (uchar)t_let * 2;
  if( (pidx + prop_l_delta < letters_pidx_table[let]) ||
    (pidx - prop_r_delta > letters_pidx_table[let+1]) ) return FALSE;
  else return TRUE;
}
