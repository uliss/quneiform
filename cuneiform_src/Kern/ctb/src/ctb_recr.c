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

#include "ctb.h"

CTB_FUNC(int32_t) CTB_AddRecRaster(char *fname, RecRaster *r,uchar let)
{
CTB_handle  hnd;
int32_t       num;
uchar       raster[REC_MAX_RASTER_SIZE],data[CTB_DATA_SIZE]={0};
int32_t       wb;
Bool32      ret;

if( !CTB_open( fname, &hnd, "w") )
    {
    if( 1 )
        CTB_create(fname,NULL);
    else
        CTB_create_gray(fname,NULL);
    if( !CTB_open( fname, &hnd, "w") )
        {
        return FALSE;
        }
    }
data[1] = (uchar)r->lnPixWidth;
data[2] = (uchar)r->lnPixHeight;
data[3] = let;

if( hnd.signums&(CTB_GRAY_SCALE|CTB_PLANE) )
    {
    wb = r->lnPixWidth;
    memcpy( raster, r->Raster, wb*r->lnPixHeight);
    }
else
    {
    wb = ((r->lnPixWidth+63)/64)*8;
    memcpy( raster, r->Raster, wb*r->lnPixHeight);
    CTB_align1_lines(raster, r->lnPixWidth, r->lnPixHeight);
    }
ret = CTB_write( &hnd, -1, raster,  data);
num = hnd.num;
CTB_close( &hnd);
return ret?num:-1;
}

CTB_FUNC(int32_t) CTB_AddRecRaster_data(char *fname, RecRaster *r,uchar *data)
{
CTB_handle  hnd;
int32_t       num;
uchar       raster[REC_MAX_RASTER_SIZE];
int32_t       wb;
Bool32      ret;

if( !CTB_open( fname, &hnd, "w") )
    {
    if( 1 )
        CTB_create(fname,NULL);
    else
        CTB_create_gray(fname,NULL);
    if( !CTB_open( fname, &hnd, "w") )
        {
        return FALSE;
        }
    }
data[1] = (uchar)r->lnPixWidth;
data[2] = (uchar)r->lnPixHeight;

if( hnd.signums==(CTB_GRAY_SCALE|CTB_PLANE) )
    {
    wb = r->lnPixWidth;
    memcpy( raster, r->Raster, wb*r->lnPixHeight);
    }
else
    {
    wb = ((r->lnPixWidth+63)/64)*8;
    memcpy( raster, r->Raster, wb*r->lnPixHeight);
    CTB_align1_lines(raster, r->lnPixWidth, r->lnPixHeight);
    }
ret = CTB_write( &hnd, -1, raster,  data);
num = hnd.num;
CTB_close( &hnd);
return ret?num:-1;
}


CTB_FUNC(Bool32) CTB_ReadRecRaster(CTB_handle *hnd, int32_t num ,
                 uchar *let, RecRaster *r, uchar *data )
{
uchar       raster[REC_MAX_RASTER_SIZE],w,h;
int32_t       wb,ww,hh;
Bool32      ret;


ret = CTB_read( hnd, num, raster,  data);
if( ret )
    {
    w = data[1];
    h = data[2];
    ww=w; hh=h;
    r->lnRasterBufSize = REC_MAX_RASTER_SIZE;
    r->lnPixWidth  = ww;
    r->lnPixHeight = hh;
    *let = data[3];
    if( hnd->signums==(CTB_GRAY_SCALE|CTB_PLANE) )
        {
        memcpy( r->Raster, raster, ww*hh);
        }
    else
        {
        wb = (w+7)/8;
        memcpy( r->Raster, raster, wb*hh);
        CTB_align8_lines(r->Raster, r->lnPixWidth, r->lnPixHeight);
        }
    }

return ret;
}

CTB_FUNC(Bool32) CTB_GetRecRaster(char *fname, int32_t id_rast ,
        RecRaster *r)
{
uchar       raster[REC_MAX_RASTER_SIZE],w,h,data[CTB_DATA_SIZE];
int32_t       wb,ww,hh;
Bool32      ret;
CTB_handle  hnd;

if( !CTB_open( fname, &hnd, "w") )
    return FALSE;

ret = CTB_read( &hnd, id_rast, raster,  data);
CTB_close(&hnd);

if( ret )
    {
    w = data[1];
    h = data[2];
    ww=w; hh=h;

    r->lnRasterBufSize = REC_MAX_RASTER_SIZE;
    r->lnPixWidth  = ww;
    r->lnPixHeight = hh;
    if( hnd.signums==(CTB_GRAY_SCALE|CTB_PLANE) )
        {
        memcpy( r->Raster, raster, ww*hh);
        }
    else
        {
        wb = (w+7)/8;
        memcpy( r->Raster, raster, wb*hh);
        CTB_align8_lines(r->Raster, r->lnPixWidth, r->lnPixHeight);
        }
    }
return ret;
}


CTB_FUNC(int32_t) CTB_AddRecRasterEx(char *fname, RecRasterEx *r,uchar let)
{
CTB_handle  hnd;
int32_t       num;
uchar       raster[REC_MAX_RASTER_SIZE_EX],data[CTB_DATA_SIZE]={0};
int32_t       wb;
Bool32      ret;

if( !CTB_open( fname, &hnd, "w") )
    {
    if( 0 )
        CTB_create(fname,NULL);
    else
        CTB_create_gray(fname,NULL);
    if( !CTB_open( fname, &hnd, "w") )
        {
        return FALSE;
        }
    }
data[1] = (uchar)r->lnPixWidth;
data[2] = (uchar)r->lnPixHeight;
data[3] = let;

if( hnd.signums&CTB_GRAY_SCALE )
    {
    wb = r->lnPixWidth;
    memcpy( raster, r->Raster, wb*r->lnPixHeight);
    }
else
    {
    wb = ((r->lnPixWidth+63)/64)*8;
    memcpy( raster, r->Raster, wb*r->lnPixHeight);
    CTB_align1_lines(raster, r->lnPixWidth, r->lnPixHeight);
    }
ret = CTB_write( &hnd, -1, raster,  data);
num = hnd.num;
CTB_close( &hnd);
return ret?num:-1;
}
