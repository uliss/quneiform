/*
Copyright (c) 1993-2008, Cognitive Technologies
All rights reserved.

Ğàçğåøàåòñÿ ïîâòîğíîå ğàñïğîñòğàíåíèå è èñïîëüçîâàíèå êàê â âèäå èñõîäíîãî êîäà,
òàê è â äâîè÷íîé ôîğìå, ñ èçìåíåíèÿìè èëè áåç, ïğè ñîáëşäåíèè ñëåäóşùèõ óñëîâèé:

      * Ïğè ïîâòîğíîì ğàñïğîñòğàíåíèè èñõîäíîãî êîäà äîëæíû îñòàâàòüñÿ óêàçàííîå
        âûøå óâåäîìëåíèå îá àâòîğñêîì ïğàâå, ıòîò ñïèñîê óñëîâèé è ïîñëåäóşùèé 
        îòêàç îò ãàğàíòèé. 
      * Ïğè ïîâòîğíîì ğàñïğîñòğàíåíèè äâîè÷íîãî êîäà â äîêóìåíòàöèè è/èëè â 
        äğóãèõ ìàòåğèàëàõ, ïîñòàâëÿåìûõ ïğè ğàñïğîñòğàíåíèè, äîëæíû ñîõğàíÿòüñÿ
        óêàçàííàÿ âûøå èíôîğìàöèÿ îá àâòîğñêîì ïğàâå, ıòîò ñïèñîê óñëîâèé è
        ïîñëåäóşùèé îòêàç îò ãàğàíòèé.  
      * Íè íàçâàíèå Cognitive Technologies, íè èìåíà åå ñîòğóäíèêîâ íå ìîãóò 
        áûòü èñïîëüçîâàíû â êà÷åñòâå ñğåäñòâà ïîääåğæêè è/èëè ïğîäâèæåíèÿ 
        ïğîäóêòîâ, îñíîâàííûõ íà ıòîì ÏÎ, áåç ïğåäâàğèòåëüíîãî ïèñüìåííîãî
        ğàçğåøåíèÿ. 

İÒÀ ÏĞÎÃĞÀÌÌÀ ÏĞÅÄÎÑÒÀÂËÅÍÀ ÂËÀÄÅËÜÖÀÌÈ ÀÂÒÎĞÑÊÈÕ ÏĞÀÂ È/ÈËÈ ÄĞÓÃÈÌÈ ËÈÖÀÌÈ "ÊÀÊ
ÎÍÀ ÅÑÒÜ" ÁÅÇ ÊÀÊÎÃÎ-ËÈÁÎ ÂÈÄÀ ÃÀĞÀÍÒÈÉ, ÂÛĞÀÆÅÍÍÛÕ ßÂÍÎ ÈËÈ ÏÎÄĞÀÇÓÌÅÂÀÅÌÛÕ, 
ÂÊËŞ×Àß ÃÀĞÀÍÒÈÈ ÊÎÌÌÅĞ×ÅÑÊÎÉ ÖÅÍÍÎÑÒÈ È ÏĞÈÃÎÄÍÎÑÒÈ ÄËß ÊÎÍÊĞÅÒÍÎÉ ÖÅËÈ, ÍÎ ÍÅ
ÎÃĞÀÍÈ×ÈÂÀßÑÜ ÈÌÈ. ÍÈ ÂËÀÄÅËÅÖ ÀÂÒÎĞÑÊÈÕ ÏĞÀÂ È ÍÈ ÎÄÍÎ ÄĞÓÃÎÅ ËÈÖÎ, ÊÎÒÎĞÎÅ 
ÌÎÆÅÒ ÈÇÌÅÍßÒÜ È/ÈËÈ ÏÎÂÒÎĞÍÎ ĞÀÑÏĞÎÑÒĞÀÍßÒÜ ÏĞÎÃĞÀÌÌÓ, ÍÈ Â ÊÎÅÌ ÑËÓ×ÀÅ ÍÅ 
ÍÅÑ¨Ò ÎÒÂÅÒÑÒÂÅÍÍÎÑÒÈ, ÂÊËŞ×Àß ËŞÁÛÅ ÎÁÙÈÅ, ÑËÓ×ÀÉÍÛÅ, ÑÏÅÖÈÀËÜÍÛÅ ÈËÈ 
ÏÎÑËÅÄÎÂÀÂØÈÅ ÓÁÛÒÊÈ, ÑÂßÇÀÍÍÛÅ Ñ ÈÑÏÎËÜÇÎÂÀÍÈÅÌ ÈËÈ ÏÎÍÅÑÅÍÍÛÅ ÂÑËÅÄÑÒÂÈÅ 
ÍÅÂÎÇÌÎÆÍÎÑÒÈ ÈÑÏÎËÜÇÎÂÀÍÈß ÏĞÎÃĞÀÌÌÛ (ÂÊËŞ×Àß ÏÎÒÅĞÈ ÄÀÍÍÛÕ, ÈËÈ ÄÀÍÍÛÅ, 
ÑÒÀÂØÈÅ ÍÅÃÎÄÍÛÌÈ, ÈËÈ ÓÁÛÒÊÈ È/ÈËÈ ÏÎÒÅĞÈ ÄÎÕÎÄÎÂ, ÏÎÍÅÑÅÍÍÛÅ ÈÇ-ÇÀ ÄÅÉÑÒÂÈÉ 
ÒĞÅÒÜÈÕ ËÈÖ È/ÈËÈ ÎÒÊÀÇÀ ÏĞÎÃĞÀÌÌÛ ĞÀÁÎÒÀÒÜ ÑÎÂÌÅÑÒÍÎ Ñ ÄĞÓÃÈÌÈ ÏĞÎÃĞÀÌÌÀÌÈ, 
ÍÎ ÍÅ ÎÃĞÀÍÈ×ÈÂÀßÑÜ İÒÈÌÈ ÑËÓ×ÀßÌÈ), ÍÎ ÍÅ ÎÃĞÀÍÈ×ÈÂÀßÑÜ ÈÌÈ, ÄÀÆÅ ÅÑËÈ ÒÀÊÎÉ 
ÂËÀÄÅËÅÖ ÈËÈ ÄĞÓÃÎÅ ËÈÖÎ ÁÛËÈ ÈÇÂÅÙÅÍÛ Î ÂÎÇÌÎÆÍÎÑÒÈ ÒÀÊÈÕ ÓÁÛÒÊÎÂ È ÏÎÒÅĞÜ.

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

CTB_FUNC(Int32) CTB_AddRecRaster(char *fname, RecRaster *r,Word8 let)
{
CTB_handle  hnd;
Int32       num;
Word8       raster[REC_MAX_RASTER_SIZE],data[CTB_DATA_SIZE]={0};
Int32       wb;
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
data[1] = (Word8)r->lnPixWidth;
data[2] = (Word8)r->lnPixHeight;
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

CTB_FUNC(Int32) CTB_AddRecRaster_data(char *fname, RecRaster *r,Word8 *data)
{
CTB_handle  hnd;
Int32       num;
Word8       raster[REC_MAX_RASTER_SIZE];
Int32       wb;
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
data[1] = (Word8)r->lnPixWidth;
data[2] = (Word8)r->lnPixHeight;

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


CTB_FUNC(Bool32) CTB_ReadRecRaster(CTB_handle *hnd, Int32 num , 
                 Word8 *let, RecRaster *r, Word8 *data )
{
Word8       raster[REC_MAX_RASTER_SIZE],w,h;
Int32       wb,ww,hh;
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

CTB_FUNC(Bool32) CTB_GetRecRaster(char *fname, Int32 id_rast , 
        RecRaster *r)
{
Word8       raster[REC_MAX_RASTER_SIZE],w,h,data[CTB_DATA_SIZE];
Int32       wb,ww,hh;
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


CTB_FUNC(Int32) CTB_AddRecRasterEx(char *fname, RecRasterEx *r,Word8 let)
{
CTB_handle  hnd;
Int32       num;
Word8       raster[REC_MAX_RASTER_SIZE_EX],data[CTB_DATA_SIZE]={0};
Int32       wb;
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
data[1] = (Word8)r->lnPixWidth;
data[2] = (Word8)r->lnPixHeight;
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
