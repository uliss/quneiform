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

#include "cstr.h"
#include "cor_incl.h"
Bool32  incl_test_fragment(int32_t version, int32_t fragm)
{
int32_t       old_bottom;
CSTR_line   line;
CSTR_attr   attr;
//return TRUE;
line = CSTR_FirstLineFragm( fragm, version );
CSTR_GetLineAttr(line,&attr);
old_bottom = attr.row+attr.hei;
do
    {
    line = CSTR_NextLineFragm(line);
    if( line )
        {
        CSTR_GetLineAttr(line,&attr);
        if( attr.row<old_bottom )
            return FALSE;
        old_bottom = attr.row+attr.hei;
        }
    }while( line );
return TRUE;
}

int32_t   incl_calculate(int32_t version, int32_t fragm)
{
int32_t   hist_incl[4096]={0},incline,maxi=-1,mini=4097, av, sav, i;
CSTR_line   line;
CSTR_attr   attr;

line = CSTR_FirstLineFragm( fragm, version );
CSTR_GetLineAttr(line,&attr);
incline = attr.incline;
if( 2048+incline>=0 && 2048+incline<4096 )
    {
    hist_incl[2048+incline]++;
    if( 2048+incline>maxi ) maxi =2048+incline ;
    if( 2048+incline<mini ) mini =2048+incline ;
    }
do
    {
    line = CSTR_NextLineFragm(line);
    if( line )
        {
        CSTR_GetLineAttr(line,&attr);
        incline = attr.incline;
        if( 2048+incline>=0 && 2048+incline<4096 )
            {
            hist_incl[2048+incline]++;
            if( 2048+incline>maxi ) maxi =2048+incline ;
            if( 2048+incline<mini ) mini =2048+incline ;
            }
        }
    }while( line );
for(sav=av=0,i=mini;i<=maxi;i++)
    {
    if( hist_incl[i] )
        {
        av  += hist_incl[i]*i;
        sav += hist_incl[i];
        }
    }
if( !sav )
    return -2048;
av/=sav;
return av-2048;
}

void incl_recalc_incline(CSTR_line   line, int32_t nIncline)
{
CSTR_attr       la;
CSTR_rast_attr  a;
CSTR_rast       r=CSTR_GetFirstRaster(line);
int16_t           mincol, minrow, maxcol, maxrow;

CSTR_GetLineAttr(line,&la);
mincol=32000;
minrow=32000;
maxcol=-16000;
maxrow=-16000;
for(r=CSTR_GetNext(r);r;r=CSTR_GetNext(r))
    {
    CSTR_GetAttr(r,&a);
    if( a.flg & (CSTR_f_let|CSTR_f_bad|CSTR_f_dust|CSTR_f_punct) )
        {
        a.row=a.r_row-(int16_t)(nIncline*a.r_col/2048);
        a.col=a.r_col+(int16_t)(nIncline*a.r_row/2048);
        if( mincol>a.col ) mincol = a.col;
        if( minrow>a.row ) minrow = a.row;
        if( maxcol<a.col+a.w ) maxcol = a.col+a.w;
        if( maxrow<a.row+a.h ) maxrow = a.row+a.h;
        }
    }
la.col=mincol;
la.row=minrow;
la.wid=maxcol-mincol;
la.hei=maxrow-minrow;
la.incline= nIncline;
CSTR_SetLineAttr(line,&la);
return;
}

void    incl_ordering(int32_t version, int32_t fragm, int32_t incline)
{
CSTR_line   line;

if( incline==-2048 )
    return;

line = CSTR_FirstLineFragm( fragm, version );
incl_recalc_incline(line,incline);
do
    {
    line = CSTR_NextLineFragm(line);
    if( line )
        {
        incl_recalc_incline(line,incline);
        }
    }while( line );
return;
}
