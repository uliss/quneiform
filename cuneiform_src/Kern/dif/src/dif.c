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

#include<string.h>
#include "dif.h"
#include "stick.h"
extern void init_diskrim(uchar* raster,int16_t height ,int16_t width);
extern int16_t Diskrim(uchar let,uchar* raster,
      int16_t D_X,int16_t dx,int16_t dy,uchar, int16_t );
extern uchar   stick_recog(uchar let, STICK_CHARS *l, STICK_CHARS *r,
                STICK_SIGNUMS *s);
extern uchar lnhead_stick_center_study(lnhead *lin,int16_t dy,int16_t dx,
        STICK_CHARS *res_left_chars,STICK_CHARS *res_right_chars,
        STICK_SIGNUMS *res_signums);
extern int32_t lnhead_stick_get_incline(lnhead *pool ,int dy, int dx );
extern void  clear_right_bites(uchar *RASTER, int16_t NWIDTH, int16_t WBYTE, int16_t NHEIGHT);
extern  uint16_t typ_thin_stick(lnhead *lin,int16_t dy,int16_t dx);
extern  int16_t LeftDistance(uchar *RASTER, int16_t NWIDTH);
extern  int16_t RightDistance(uchar *RASTER, int16_t NWIDTH);

uchar   alphabet[256];
Bool32  digit_mode = FALSE, dif_adding_mode ;
uchar   language;
int32_t   dif_typ_of_font=0;
#include "minmax.h"

extern int16_t broken_ii;
extern uchar broken_flag;
uchar  cutl_flag, cutr_flag;

DIF_FUNC(void)  DIFSetFont( int32_t typ_of_font)
{
dif_typ_of_font=typ_of_font;
return;
}

DIF_FUNC(Bool32)  DIFInit( RecRaster *r, Bool32 broken, Bool32 broken_II,
    Bool32 cut_left, Bool32 cut_right)
{
   broken_ii = (int16_t)(broken_II!=0);
   broken_flag=(uchar)broken;
   cutl_flag=(uchar)(cut_left!=0);
   cutr_flag=(uchar)(cut_right!=0);
   init_diskrim(r->Raster,(int16_t)r->lnPixHeight,(int16_t)r->lnPixWidth);
   return TRUE;
}

DIF_FUNC(Bool32)  DIFPenaltyChar( RecRaster*  r,
                                          RecVersions* res )
{
 int i, pen;
 int16_t  dx,dy ;
 int    maxprob;

 dy=(int16_t)r->lnPixHeight;
 dx=(int16_t)r->lnPixWidth;

 for(maxprob=i=0;i<res->lnAltCnt;i++)
    if( res->Alt[i].Prob>maxprob )
        maxprob = res->Alt[i].Prob;

 for(i=0;i<res->lnAltCnt;i++)
    {

    pen = Diskrim((char)(res->Alt[i].Code),r->Raster,(int16_t)REC_GW_WORD8(dx),(int16_t)dx,(int16_t)dy,0,0);
    if( pen<0 && broken_flag && broken_ii )
        { // broken II
        switch( pen )
            {
            case    -254:
                res->Alt[i].Prob=MIN(255,maxprob+4);
                break;
            case    -252:
                res->Alt[i].Prob=MIN(255,maxprob+2);
                break;
            }
        }
    else
        {
        if( res->Alt[i].Prob<=pen )
            res->Alt[i].Prob=1;
        else
            res->Alt[i].Prob-=pen;
        }
    }

 return TRUE;

}

uchar dif_down_prob(uchar prob, uchar dis)
{
if( prob>dis )
    prob -= dis;
else
    prob = 1;
return prob;
}

DIF_FUNC(Bool32)  DIFStick_expert( int16_t dx,int16_t dy , uchar *pool,
								             RecVersions* res )
{
STICK_CHARS     res_left_chars={0},res_right_chars={0};
STICK_SIGNUMS   res_signums={0};
int16_t           i ;
uchar           ret=0, r;

if( !pool)
    return FALSE;
for(i=0;i<res->lnAltCnt;i++)
    {
    if( res->Alt[i].Prob )
        break;
    }
dif_adding_mode = ( i==res->lnAltCnt );
ret=lnhead_stick_center_study((lnhead *)pool ,dy,dx,
        &res_left_chars, &res_right_chars,&res_signums);
{ // external info for stick
res_signums.cut_l=cutl_flag;
res_signums.cut_r=cutr_flag;
}
if( !ret )
    {
    for(i=0;i<res->lnAltCnt;i++)
	    {
	    switch( res->Alt[i].Code )
		    {
            case '1':
            case '|':
		    case '/':
            case 'l':
                res->Alt[i].Prob = 1;
                break;
		    default:
			    break;
		    }
	    }
    return  TRUE;
    }
if( ret==1 )
    {
    for(i=0;i<res->lnAltCnt;i++)
	    {
	    switch( res->Alt[i].Code )
		    {
            case '1':
            case '|':
		    case '/':
            case 'l':
            case 't':
//            case 'f':
                r = stick_recog(res->Alt[i].Code,
                           &res_left_chars, &res_right_chars,
                           &res_signums);
                if( !res->Alt[i].Prob )
                    {
			        res->Alt[i].Prob = r;
                    }
                else
                    {
                    r = 255-r;
                    if( res->Alt[i].Prob>r )
                        res->Alt[i].Prob -= r;
                    else
                        res->Alt[i].Prob=1;
                    }

			    break;
            case '(':
                if( !alphabet['('] )
                    {
                    res->Alt[i].Prob = 1;
                    break;
                    }
            case ')':
                if( !alphabet[')'] )
                    {
                    res->Alt[i].Prob = 1;
                    break;
                    }
                r=stick_recog(res->Alt[i].Code,
                    &res_left_chars, &res_right_chars, &res_signums);
                if( r>220 )
                    {
                    if( !res->Alt[i].Prob )
                        res->Alt[i].Prob = r;
                    else
                        {
                        r = 255-r;
                        if( res->Alt[i].Prob>r )
                            res->Alt[i].Prob -= r;
                        else
                            res->Alt[i].Prob=1;
                        }
                    }
                else
                    {
                    if( !res->Alt[i].Prob )
                        res->Alt[i].Prob = 1;
                    else
                        {
                        r = 255-r;
                        if( res->Alt[i].Prob>r )
                            res->Alt[i].Prob -= r;
                        else
                            res->Alt[i].Prob=1;
                        }
                    }
                break;
		    default:
			    break;
		    }
	    }
    }
else
    {
    for(i=0;i<res->lnAltCnt;i++)
	    {
	    switch( res->Alt[i].Code )
		    {
            case '(':
                if( dif_adding_mode )
                    res->Alt[i].Prob = 255;
                if( ret!=2 )
                    res->Alt[i].Prob = 1;
                else if( ret==2 )
                    {
                    if( res_signums.cut_l )
                        res->Alt[i].Prob = dif_down_prob(res->Alt[i].Prob , 40);
                    if( res_signums.cut_r )
                        res->Alt[i].Prob = dif_down_prob(res->Alt[i].Prob , 20);
                    }
		        break;
            case ')':
                if( dif_adding_mode )
                    res->Alt[i].Prob = 255;
			    if( ret!=3 )
                    res->Alt[i].Prob = 1;
                else if( ret==3 )
                    {
                    if( res_signums.cut_r )
                        res->Alt[i].Prob = dif_down_prob(res->Alt[i].Prob , 40);
                    if( res_signums.cut_r )
                        res->Alt[i].Prob = dif_down_prob(res->Alt[i].Prob , 20);
                    }

		        break;
            case '1':
            case '|':
		    case '/':
                res->Alt[i].Prob = 1;
                break;
		    default:
			    break;
		    }
	    }
    }

return TRUE;
}


DIF_FUNC(int32_t)  DIFGet_incline( RecRaster*  r, uchar *pool)
{
 int16_t dx,dy ;

 dy=(int16_t)r->lnPixHeight;
 dx=(int16_t)r->lnPixWidth;

 if(!pool) return -1;

return  lnhead_stick_get_incline((lnhead *)pool ,dy,dx );
}

 DIF_FUNC(void)  DIFDone(void)
 {
   return;
 }

 DIF_FUNC(int16_t) DIFGetErr(void)
 {
   return TRUE;
 }


DIF_FUNC(int16_t) DIFLeftDistance(uchar *RASTER, int16_t NWIDTH)
{
return LeftDistance(RASTER, NWIDTH);
}

DIF_FUNC(int16_t) DIFRightDistance(uchar *RASTER, int16_t NWIDTH)
{
return RightDistance(RASTER, NWIDTH);
}

DIF_FUNC(void) DIFClearRightZone(uchar *RASTER, int16_t NWIDTH, int16_t WBYTE, int16_t NHEIGHT)
{
clear_right_bites(RASTER, NWIDTH, WBYTE, NHEIGHT);
return;
}

DIF_FUNC(Bool32) DIFSetAlphabet(char *alpha)
{
int i,d,a;
memcpy(alphabet, alpha, 256);
for(d=a=i=0;i<256;i++)
    {
    if( alphabet[i] )
        {
        if( i>47 && i<58 || i==40 || i==41 )
            d++;
        a++;
        }
    }
digit_mode = FALSE;
if( a==d )
    digit_mode = TRUE;
return TRUE;
}

DIF_FUNC(Bool32) DIF_SetLanguage(uchar lang)
{
language=lang;
return TRUE;
}


DIF_FUNC(uint16_t) DIF_typ_thin_stick(uchar *lin,int16_t dy,int16_t dx)
{
return  typ_thin_stick((lnhead *)lin, dy, dx);
}
