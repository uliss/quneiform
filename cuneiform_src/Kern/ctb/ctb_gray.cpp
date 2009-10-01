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

/***************************************************************************/
/* CTB_gray.c : functions for manipulations with gray rasters **************/
/***************************************************************************/
#include <string.h>
#include "ctb.h"

// Level for Binarizing Gray image(wx,wy)
CTB_FUNC(uchar)  CTB_BestLevelInGray(uchar *image,int wx,int wy)
{
    int     GrayHist16Buf[16],GrayHist16Buf1[16];
    int     GrayHist16[16];
    int     m1,im1,m,im=-1;
    int     i,j;
    uchar   *p;

    memset(GrayHist16, 0, 16*sizeof(int));
    for(p=image,j=0;j<wy;j++)
    {
        for(i=0;i<wx;i++)
        {
            GrayHist16[ *p/16 ]++;
            p++;
        }
    }

    GrayHist16Buf[ 0 ] = (2*GrayHist16[0]+GrayHist16[1])/4;
    for(i=1;i<15;i++)
    {
        GrayHist16Buf[ i ] = (GrayHist16[i-1]+2*GrayHist16[i]+GrayHist16[i+1])/4;
        if( GrayHist16Buf[ i ]<2 )
            GrayHist16Buf[ i ]=0;
    }
    GrayHist16Buf[ 255 ] = (GrayHist16[14]+2*GrayHist16[15])/4;
    for(i=0;i<16;i++)
    {
        GrayHist16Buf1[ i ] = GrayHist16[i];
    }

    for(im1=-1,m1=0,i=1;i<10/*128*/;i++)
    {
        if( GrayHist16Buf[i]>m1 )
        {
            m1 = GrayHist16Buf[i];
            im1= i;
        }
    }
    if( im1!=-1  )
    {
        for(im=-1,m=10000,i=im1+1;i<16;i++)
        {
            if(GrayHist16Buf[i]>GrayHist16Buf[i-1] )
            {
                m = GrayHist16Buf[i];
                im= i;
                break;
            }
        }
    }

    if( im==-1 )
        im=128;
    else
        im = (im)*16;

return im;
}

////////////////// Find Bound
static Bool32 CTB_GrayBoundTop(RecRasterEx *PaintRaster,int GLevel)
{
int i,j,h=PaintRaster->lnPixHeight,w=PaintRaster->lnPixWidth, w16;
w16 = w;
for(i=0;i<h;i++)
    {
    for(j=0;j<w;j++)
        if( PaintRaster->Raster[w16*i+j]>GLevel )
            break;
    if( j<w )
        break;
    }
if( i )
    {
    h = h-i;
    for(j=0;j<h;j++)
        memcpy(&PaintRaster->Raster[w16*j],&PaintRaster->Raster[w16*(j+i)],w16);
    PaintRaster->lnPixHeight = h;
    return TRUE;
    }
return FALSE;
}
static Bool32 CTB_GrayBoundBottom(RecRasterEx *PaintRaster,int GLevel)
{
    int i,j,h=PaintRaster->lnPixHeight,w=PaintRaster->lnPixWidth, w16;
    w16 = w;
    for(i=h-1;i>=0;i--)
    {
        for(j=0;j<w;j++)
            if( PaintRaster->Raster[w16*i+j]>GLevel )
                break;
            if( j<w )
                break;
    }
    if( i<h-1 )
    {
        PaintRaster->lnPixHeight = i+1;
        return TRUE;
    }
    return FALSE;
}
static Bool32 CTB_GrayBoundRight(RecRasterEx *PaintRaster,int GLevel)
{
    int i,j,h=PaintRaster->lnPixHeight,w=PaintRaster->lnPixWidth, w16;
    int dist, max_dist, ww;
    uchar   PRRaster[256];
    w16 = w;
    for(max_dist=i=0;i<h;i++)
    {
        for(j=0;j<w16;j++)
        {
            if( PaintRaster->Raster[w16*i+j]>GLevel )
                dist = j;
        }
        if( dist>max_dist )
            max_dist = dist+1;
    }
    if( max_dist<w )
    {
        PaintRaster->lnPixWidth = max_dist;
        ww = max_dist;
        if( ww!=w16 )
        {
            for(i=0;i<h;i++)
            {
                memcpy(PRRaster,&PaintRaster->Raster[w16*i],ww);
                memcpy(&PaintRaster->Raster[ww*i],PRRaster,ww);
            }
        }
        return TRUE;
    }

    return FALSE;
}
static Bool32 CTB_GrayBoundLeft(RecRasterEx *PaintRaster,int GLevel)
{
    int i,j,h=PaintRaster->lnPixHeight,w=PaintRaster->lnPixWidth, w16;
    int dist, min_dist, ww;
    char    buf[256];
    w16 = w;
    for(min_dist=1024,i=0;i<h;i++)
    {
        for(dist =10240, j=0;j<w16;j++)
        {
            if( PaintRaster->Raster[w16*i+j]>GLevel )
            {
                dist = j;
                break;
            }
        }
        if( dist<min_dist )
            min_dist = dist;
    }

    if( min_dist )
    {
        ww = w-min_dist;
        for(i=0;i<h;i++)
        {
            memcpy(buf,&PaintRaster->Raster[w16*i+min_dist],ww);
            memcpy(&PaintRaster->Raster[ww*i],buf,ww);
        }
        PaintRaster->lnPixWidth -= min_dist;
        return TRUE;
    }

    return FALSE;
}

CTB_FUNC(Bool32) CTB_GrayBound(RecRasterEx *PaintRaster,int GLevel)
{
    return  CTB_GrayBoundTop(PaintRaster,GLevel)  | CTB_GrayBoundBottom(PaintRaster,GLevel) |
            CTB_GrayBoundLeft(PaintRaster,GLevel) | CTB_GrayBoundRight(PaintRaster,GLevel) ;

}

