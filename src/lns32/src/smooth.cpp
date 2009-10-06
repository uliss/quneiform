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

#include "lnslang.h"

#include "smooth.h"
#include "lns.h"
///////////////////////////////////////////////////////////////////
static int32_t* buff=NULL;
static int nLines=0; // count of lines passed through buffer
static int width_dword =0;

#define DEF_SMOOTH_HEIGHT 4
#define MAX_SMOOTH_HEIGHT 16   // crazy...

static int nSmoothHeight=DEF_SMOOTH_HEIGHT; // count of lines smoothed vertically

Bool     smooth_start(int _width_dword)
{
   nSmoothHeight = LnsGetProfileInt("nSmoothHeight", DEF_SMOOTH_HEIGHT);
   if (nSmoothHeight < 0 || nSmoothHeight > MAX_SMOOTH_HEIGHT)
      nSmoothHeight = DEF_SMOOTH_HEIGHT;

   if (nSmoothHeight == 0)
      return TRUE; // no smoothing

   width_dword = _width_dword;
   buff = (int32_t *)(malloc(4*width_dword*(nSmoothHeight+1) ) );
   if (buff==NULL)
      return FALSE;

   memset(buff, 0xff,  4*width_dword*(nSmoothHeight+1) ); // start - all white
   nLines =0;
   return TRUE;
}

int32_t    smooth_get_height() // count of lines joined lines
{
   return nSmoothHeight;
}

int32_t*   smooth_update(int32_t* new_line)
{
   if (nSmoothHeight==0 || nSmoothHeight==1)
      return new_line;

   int pos = 1 + (nLines % nSmoothHeight);     // 1..nSmoothHeight used to keep lines
                                               // 0 pos used for result
   nLines++;

   memcpy(buff + pos * width_dword, new_line, 4*width_dword); // save line

   // make smooth
   if (nSmoothHeight == 4)
   {
      int32_t* p0=buff;
      int32_t* p1=buff + width_dword;
      int32_t* p2=buff + width_dword*2;
      int32_t* p3=buff + width_dword*3;
      int32_t* p4=buff + width_dword*4;

      int wi = width_dword;
      while (wi--)
      {
         *p0++ = ((*p1++) & (*p2++)) & ((*p3++) & (*p4++));
      }

      return buff;
   }
   if (nSmoothHeight == 3)
   {
      int32_t* p0=buff;
      int32_t* p1=buff + width_dword;
      int32_t* p2=buff + width_dword*2;
      int32_t* p3=buff + width_dword*3;

      int wi = width_dword;
      while (wi--)
      {
         *p0++ = ((*p1++) & (*p2++)) & ((*p3++));
      }
   }
   if (nSmoothHeight == 2)
   {
      int32_t* p0=buff;
      int32_t* p1=buff + width_dword;
      int32_t* p2=buff + width_dword*2;

      int wi = width_dword;
      while (wi--)
      {
         *p0++ = ((*p1++) & (*p2++));
      }
   }
   if (nSmoothHeight > 4)
   {
      int32_t* p0=buff;
      int wi = width_dword;
      while (wi--)
      {
         *p0 = 0xffffffff;
         int sh = nSmoothHeight;
         int32_t* p1=p0;
         while (sh--)
         {
            p1 += width_dword;
            *p0 &= *p1;
         }
         p0++;
      }
   }

   return buff;
}

void     smooth_finish()
{
   if (buff)
   {
      free(buff);
      buff = NULL;
   }
}
