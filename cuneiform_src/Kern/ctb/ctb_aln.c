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

/************************************************************/
/* CTB_aln.c : functions for aligne of results **************/
/************************************************************/
#include <string.h>
#include "ctb.h"

static  uchar   local_rst[REC_MAX_RASTER_SIZE];
CTB_FUNC(void) CTB_align8_lines(uchar *bin,int32_t w, int32_t h)
{
int i,ii,iii, wb=(w+7)/8, wb_new=((w+63)/64)*8;
uchar   buf[256];

memset(buf,0,wb_new);
for(iii=(h-1)*wb_new,ii=(h-1)*wb,i=0;i<h;i++,ii-=wb,iii-=wb_new)
    {
    memcpy(buf,&bin[ii],wb);
    memcpy(&bin[iii],buf,wb_new);
    }

return;
}

CTB_FUNC(void) CTB_align1_lines(uchar *bin,int32_t w, int32_t h)
{
int i,ii,iii, wb=((w+63)/64)*8, wb_new=(w+7)/8;
uchar   buf[256];

memset(buf,0,wb_new);
for(iii=ii=i=0;i<h;i++,ii+=wb,iii+=wb_new)
    {
    memcpy(buf,&bin[ii],wb);
    memcpy(&bin[iii],buf,wb_new);
    }

return;
}

CTB_FUNC(void) CTB_align8_124lines(uchar *bin,int32_t w, int32_t h,
                                   int32_t alin, uchar init)
{
int i, wb, wb_new;
uchar   buf[256];
uchar  *bout;

switch( alin )
    {
    case    1:
        wb=(w+7)/8;
        break;
    case    2:
        wb=((w+1)/2)*2;
        break;
    case    4:
        wb=((w+3)/4)*4;
        break;
    default:
        return;
    }
wb_new=((w+7)/8)*8;

memset(buf,init,wb_new);
for(bout=bin+wb_new*(h-1),bin+=wb*(h-1),i=0;i<h;i++,bin-=wb,bout-=wb_new)
    {
    memcpy(buf,     bin,    w);
    memcpy(bout,    buf,    wb_new);
    }

return;
}

// bin : B/W images align to 4 bytes
CTB_FUNC(void) CTB_align41(uchar *sbin,int32_t w, int32_t h)
{
int i, wb_in, wb_out;
uchar   buf[256];
uchar  *bout, *bin=sbin;

wb_in=(w/32+1)*4;
wb_out=(w+7)/8;

for(bout=local_rst+wb_out*(h-1),bin+=wb_in*(h-1),i=0;i<h;i++,bin-=wb_in,bout-=wb_out)
    {
    memcpy(buf,     bin,    wb_out);
    memcpy(bout,    buf,    wb_out);
    }
memcpy( sbin, local_rst, wb_out*h);
return;
}

