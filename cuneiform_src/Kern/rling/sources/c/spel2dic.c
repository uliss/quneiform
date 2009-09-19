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


/* FINDWORD.C  */




/*************************************************************************/
/*      access to dictionary from program "edconv"                       */
/*************************************************************************/

#include "spelmode.h"

#if defined(TURBO_C)
  #include "tc_types.h"
#elif defined(WATCOM)
  #include "spelwatc.h"
#else
  #error   NO TOOOL SPECIFIED
#endif
#include "speldefs.h"
#include "spelfunc.h"

extern  uchar alphabet[][ABCSIZE];


extern KEYTYPE codetable[256];
extern KEYTYPE codepermit[256];
/*************************************************************************/
/*              decoding & search word in dictionary                     */
/*************************************************************************/
int16_t findstat(int16_t * currw,LTIMG * wrddef[],
             struct dict_state * dict)
{ KEYTYPE wordin[MAX_WORD_SIZE],c;
  register int16_t i;

  /* ---------- decoding input word -----------------------------------*/
  for (i=0; i<=*currw ;i++)
   {
    c=wrddef[i]->lt->code;
    if (codepermit[c])
      wordin[i]=codetable[c];
    else
    { *currw=i;
     return 0;			/* symbol is not in codetable */
     }
   }
  /* ---------- search word in dictionary -----------------------------*/
  return (search(wordin,currw,wrddef,dict));
}

/*************************************************************************/
/*              decoding & search text word in dictionary                */
/*	WARRNING !!! Only for 32-bits mode because load_dict points to	 */
/*			far memory					 */
/*************************************************************************/
extern struct dict_state  * load_dict;
extern int16_t vocs_NOK;   // 0 - vocs found at load

int16_t text_findstat_rling(char * word)
{ KEYTYPE wordin[MAX_WORD_SIZE],c;
  LTIMG * wrddef[MAX_WORD_SIZE+1], *p;
  LTIMG wrdim[MAX_WORD_SIZE];
  struct letter lt[MAX_WORD_SIZE];
  int16_t i;
  if (vocs_NOK) return 0;
  /* ---------- decoding input word -----------------------------------*/
  for (i=0; *word ;i++, word++)
   {
    if (i >= MAX_WORD_SIZE) return 0;
    c=*word; 		if (c == 0) break;
    lt[i].code = c;	lt[i].attr = 255;
    p=&wrdim[i];
    wrddef[i] = p;
    p->lt = lt + i;	p->blank = 0;

    if (codepermit[c])	wordin[i]=codetable[c];
    else 		return 0;	/* symbol is not in codetable */
   }
  /* ---------- search word in dictionary -----------------------------*/
  wrddef[i] = 0;
  i--;
  return (search(wordin,&i,wrddef,load_dict));
}

/*************************************************************************/
/*                           end of file                                 */
/*************************************************************************/
