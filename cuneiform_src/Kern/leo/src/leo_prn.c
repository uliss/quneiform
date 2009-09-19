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

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

#define PC_TYPE 0

#include "leo.h"
#include "alphaset.h"

#include "r35.h"
#include "evn.h"
#include "ctb.h"
#include "dif.h"
#include "fon.h"
#include "msk.h"

#include "std.h"
#include "leo_tune.h"
#include "leo_func.h"
#include "snptools.h"

extern Bool32 leo_Snp_In_Rect;
extern int  LEO_error_code;
extern int nNdxWid , nNdxHei;
extern uchar nIsPrint;

// data from module LEO.C
extern unsigned char alphabet[];
extern uchar leo_alpha_type, prn_roma_regim;
extern Bool32 leo_enable_fon_recog;

// data from module LEO_DLL.C
extern Bool32 leo_enable_stored;
extern int32_t  leo_typ_of_font, leo_MSK_ndx[];

static void leo_compress_prn( RecVersions *v)
{
int i;

for(i=0;i<v->lnAltCnt;i++)
    if( up_of_alpha[ v->Alt[i].Code ] )
	    v->Alt[i].Code =  up_of_alpha[ v->Alt[i].Code ];

return;
}
static void leo_kill_3x5_unique(RecVersions *ver,uchar first)
{
do{
     if( ver->Alt[0].Prob>60 )
      ver->Alt[0].Prob -= 60;
     else
      ver->Alt[0].Prob = 1;
     leo_sort_vers_prob(ver);
     }while( first!=ver->Alt[0].Code && ver->Alt[0].Prob>1 &&
                 ver->Alt[0].Method==REC_METHOD_3X5);
return;
}

// return : complemetary case letter if homotetical, otherwhise 0
uchar leo_reverse_case(uchar in)
{
uchar sr[]="©жгЄ­Јий§екдлўЇа®«¤¦нпзб¬ЁвмоЋ‡";
uchar cr[]="‰–“ЉЌѓ™‡•љ”›‚ЏђЋ‹„†ќџ—‘Њ€’њћ03";
uchar *p;

p = strchr(sr,in);
if( p )
    return cr[ p-sr ];

p = strchr(cr,in);
if( p )
    return sr[ p-cr ];

return 0;
}

static void leo_b6_reverse(RecVersions *ver)
{
uchar t;
if( ver->lnAltCnt<2 )
    return ;
t=ver->Alt[1].Code;
ver->Alt[1].Code = ver->Alt[0].Code;
ver->Alt[0].Code = t;
return;
}

static void leo_get_stat_bnd(int32_t *b1,int32_t *b2)
{
*b1 = 25;
*b2 = 35;
return;
}

static int32_t leo_no_hist_leader(uchar over[],RecVersions *ver)
{
int i = 0, m = 0;uchar c = 0;
for(m=i=0;i<256;i++)
    if( over[i]>m )
        {
        m=over[i]+over[leo_reverse_case((uchar)i)];
        c=i;
        }
if( c!=ver->Alt[0].Code )
    return 0;
return m;
}

static void leo_decode_to_roma( RecVersions *ver )
{
int i,n;
n=ver->lnAltCnt;
for(i=0;i<n;i++)
    switch( ver->Alt[i].Code )
        {
        case (uchar)'Х':
            ver->Alt[i].Code='X';
            break;
        case (uchar)'1':
            ver->Alt[i].Code='I';
            break;
        case (uchar)'У':
            ver->Alt[i].Code='V';
            break;
        default:
            break;
        }

return;
}

void leo_reduce_typefaces(RecVersions *v)
{
int i;uchar let;

if( v->lnAltCnt )
    {
    for(i=0;i<v->lnAltCnt;i++)
         {
         let = stdLeoTypefaceChar(v->Alt[i].Code);
         if( alphabet[let] )
            v->Alt[i].Code = let;
         }
    }

leo_compress_prn(v);
leo_compress( v );
return;
}

static uchar bit_cnt[]={
		0,1,1,2,1,2,2,3,
		1,2,2,3,2,3,3,4,
		1,2,2,3,2,3,3,4,
		2,3,3,4,3,4,4,5,
		1,2,2,3,2,3,3,4,
		2,3,3,4,3,4,4,5,
		2,3,3,4,3,4,4,5,
		3,4,4,5,4,5,5,6,
		1,2,2,3,2,3,3,4,
		2,3,3,4,3,4,4,5,
		2,3,3,4,3,4,4,5,
		3,4,4,5,4,5,5,6,
		2,3,3,4,3,4,4,5,
		3,4,4,5,4,5,5,6,
		3,4,4,5,4,5,5,6,
		4,5,5,6,5,6,6,7,
		1,2,2,3,2,3,3,4,
		2,3,3,4,3,4,4,5,
		2,3,3,4,3,4,4,5,
		3,4,4,5,4,5,5,6,
		2,3,3,4,3,4,4,5,
		3,4,4,5,4,5,5,6,
		3,4,4,5,4,5,5,6,
		4,5,5,6,5,6,6,7,
		2,3,3,4,3,4,4,5,
		3,4,4,5,4,5,5,6,
		3,4,4,5,4,5,5,6,
		4,5,5,6,5,6,6,7,
		3,4,4,5,4,5,5,6,
		4,5,5,6,5,6,6,7,
		4,5,5,6,5,6,6,7,
	 	5,6,6,7,6,7,7,8
		};


static int32_t leo_diskr16x16_down(RecRaster  *rr)
{
int i,ii, longl, s, sc;

for(sc=0, ii=8*5,i=5;i<11;i++,ii+=8)
    {
    sc += bit_cnt[ rr->Raster[ii] ] + bit_cnt[ rr->Raster[ii+1] ];
    }
sc /= 6;
sc *= 2;
s = bit_cnt[ rr->Raster[0]&0xF0 ]+
    bit_cnt[ rr->Raster[1]&0x0F ]+
    bit_cnt[ rr->Raster[8*15+0]&0xF0 ]+
    bit_cnt[ rr->Raster[8*15+1]&0x0F ];
if( s<5 )
    return 2;
for(longl=0, ii=8*14,i=14;i<16;i++,ii+=8)
    {
    s = bit_cnt[ rr->Raster[ii] ] + bit_cnt[ rr->Raster[ii+1] ];
    if( s>=sc-1 )
        longl++;
    }
return longl;
}

static int32_t leo_diskr16x16_right_down_hole(RecRaster  *rr)
{
int i,ii, longl, sc;

for(longl=0, ii=8*6,i=6;i<12;i++,ii+=8)
    {
    if( bit_cnt[ rr->Raster[ii] ] + bit_cnt[ rr->Raster[ii+1] ]>14 )
        longl++;
    }
if( longl<1 )
    return 0;

for(sc=0, ii=8*8,i=8;i<12;i++,ii+=8)
    {
    if( !bit_cnt[ rr->Raster[ii+1] ] )
        sc++;
    }

return sc;
}

static int32_t leo_diskr16x16_right(RecRaster  *rr)
{
int i,ii, sc;

for(sc=0, ii=8*6,i=6;i<12;i++,ii+=8)
    {
    if( !bit_cnt[ rr->Raster[ii+1] ] )
        sc++;
    }

return sc;
}

static uchar start_pos[]={
		8,
		7,
		6,6, 					/* 2 dup (6) */
		5,5,5,5, 				/* 4 dup (5) */
		4,4,4,4,4,4,4,4, 			/* 8 dup (4) */
		3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3, 	/* 16 dup (3)*/
		2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
		2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2, 	/* 32 dup (2)*/
                1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
		1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
                1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
		1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1, 	/* 64 dup (1)*/
                0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 	/* 128 dup (0)*/
		};

static int32_t leo_diskr16x16_left(RecRaster  *rr)
{
int i,ii, sc;

for(sc=0, ii=8*4,i=4;i<12;i++,ii+=8)
    {
    sc += start_pos[  rr->Raster[ii] ] ;
    }
sc /= 8;
if( sc &&   (!start_pos[ rr->Raster[15*8] ] ||
            !start_pos[ rr->Raster[14*8] ] ||
            !start_pos[ rr->Raster[13*8] ]) )
    return sc;
return 0;
}



static int32_t leo_diskr3x5_H(int16_t *Im3x5)
{
int ret=1;

if( Im3x5[3*0+1]<(Im3x5[3*0+0]+Im3x5[3*0+2])/16 &&
    Im3x5[3*1+1]<(Im3x5[3*1+0]+Im3x5[3*1+2])/4 &&
    Im3x5[3*3+1]<(Im3x5[3*3+0]+Im3x5[3*3+2])/4 &&
    Im3x5[3*4+1]<(Im3x5[3*4+0]+Im3x5[3*4+2])/16
 )
    ret = 0; // similar to H
return ret;
}

static int32_t leo_diskr3x5_III(int16_t *Im3x5)
{
int ret=1;

if( Im3x5[3*1+1]<(Im3x5[3*1+0]+Im3x5[3*1+2])/6 &&
    Im3x5[3*2+1]<(Im3x5[3*2+0]+Im3x5[3*2+2])/6 &&
    Im3x5[3*3+1]<(Im3x5[3*3+0]+Im3x5[3*3+2])/6
 )
    ret = 0; // similar to III
return ret;
}

static Bool32 leo_diskr_left_up_hole(int16_t *Im3x5)
{
int le = (Im3x5[0]+Im3x5[6]+Im3x5[12])/3;
return (Im3x5[9]>le/2 && Im3x5[3]<le/4 );
}

static Bool32 leo_diskr_right_up_hole(int16_t *Im3x5)
{
int le = (Im3x5[2]+Im3x5[8]+Im3x5[14])/3;
return (Im3x5[5]>le/2 );
}

static Bool32 leo_diskr_center_hole(int16_t *Im3x5)
{
int ce = (Im3x5[3*1+1]+Im3x5[3*2+1]+Im3x5[3*2+1]);
int bn = (Im3x5[3*1+0]+Im3x5[3*2+0]+Im3x5[3*2+0]+Im3x5[3*1+2]+Im3x5[3*2+2]+Im3x5[3*2+2])/2;
return (ce<bn );
}

int32_t LEO_DIFPenaltyCharMTR(RecRaster  *rr, int16_t *Im3x5, RecVersions *loc)
{
int i, num, pen, r;
for(num=i=0;i<loc->lnAltCnt;i++)
switch( stdAsciiToAnsi(loc->Alt[i].Code) )
    {
    case    (uchar)'П': case (uchar)'п':
        pen = 0;
        if( leo_diskr16x16_down(rr)>0 )
            pen += 80;
        if( leo_diskr16x16_right(rr)>3 )
            pen += 80;
        r = leo_diskr16x16_left(rr);
        if( r>0 )
            pen += 40*r;
        num++;
        if( loc->Alt[i].Prob>pen )
            loc->Alt[i].Prob-=pen;
        else
            loc->Alt[i].Prob=1;
        break;

    case    (uchar)'Л': case (uchar)'л':
        r = pen = 0;
        if( i==1 && leo_strchr_codes_ansi("пП",loc->Alt[0].Code) &&
            leo_diskr16x16_down(rr)<1 && leo_diskr16x16_right(rr)<3 )
            r = leo_diskr16x16_left(rr);
        if( r>0 )
            pen += 30*r;
        num++;
        if( (int)loc->Alt[i].Prob+pen<255 )
            loc->Alt[i].Prob+=pen;
        else
            loc->Alt[i].Prob=255;
        break;

    case    (uchar)'О': case (uchar)'о':
        pen = 0;
        if( !(Im3x5[4]>(Im3x5[3]+Im3x5[5])/4 || Im3x5[10]>(Im3x5[9]+Im3x5[11])/4) )
        if( leo_diskr16x16_down(rr)<1 )
            pen += 80;
        if( leo_diskr16x16_right(rr)>3 )
            pen += 80;
        num++;
        if( loc->Alt[i].Prob>pen )
            loc->Alt[i].Prob-=pen;
        else
            loc->Alt[i].Prob=1;
        break;

    case    (uchar)'С': case (uchar)'с':
        pen = 0;
        if( leo_diskr16x16_right(rr)<2 )
            pen += 80;
        num++;
        if( loc->Alt[i].Prob>pen )
            loc->Alt[i].Prob-=pen;
        else
            loc->Alt[i].Prob=1;
        break;

    case    (uchar)'В': case (uchar)'в':
        pen = 0;
        if( leo_diskr16x16_right_down_hole(rr)>1 )
            pen += 40;
        if( leo_diskr_left_up_hole(Im3x5) )
            pen += 30;
        num++;
        if( loc->Alt[i].Prob>pen )
            loc->Alt[i].Prob-=pen;
        else
            loc->Alt[i].Prob=1;
        break;

    case    (uchar)'Н': case (uchar)'н':
        pen = 0;
        if( leo_diskr3x5_H(Im3x5) )
            pen += 80;
        num++;
        if( loc->Alt[i].Prob>pen )
            loc->Alt[i].Prob-=pen;
        else
            loc->Alt[i].Prob=1;
        break;

    case    (uchar)'И': case (uchar)'и':
        pen = 0;
        if( !leo_diskr3x5_H(Im3x5) )
            pen += 80;
        num++;
        if( loc->Alt[i].Prob>pen )
            loc->Alt[i].Prob-=pen;
        else
            loc->Alt[i].Prob=1;
        break;

    case    (uchar)'Ш': case (uchar)'ш':
        pen = 0;
        if( !leo_diskr3x5_III(Im3x5) )
            pen += 80;
        num++;
        if( loc->Alt[i].Prob>pen )
            loc->Alt[i].Prob-=pen;
        else
            loc->Alt[i].Prob=1;
        break;

    case    (uchar)'Э': case (uchar)'э':
        pen = 0;
        if( !leo_diskr3x5_III(Im3x5) )
            pen += 80;
        if( leo_diskr_left_up_hole(Im3x5) )
            pen += 30;
        num++;
        if( loc->Alt[i].Prob>pen )
            loc->Alt[i].Prob-=pen;
        else
            loc->Alt[i].Prob=1;
        break;

    case    (uchar)'З': case (uchar)'з': case   '3':
        pen = 0;
        if( !(Im3x5[3]<100 && Im3x5[6]<100 && Im3x5[10]<100) )
        if( leo_diskr_left_up_hole(Im3x5) )
            pen += 30;
        num++;
        if( loc->Alt[i].Prob>pen )
            loc->Alt[i].Prob-=pen;
        else
            loc->Alt[i].Prob=1;
        break;

     case   '1':
        pen = 0;
        if( rr->lnPixWidth*2>rr->lnPixHeight )
        if( leo_diskr_center_hole(Im3x5) )
            pen += 30;
        num++;
        if( loc->Alt[i].Prob>pen )
            loc->Alt[i].Prob-=pen;
        else
            loc->Alt[i].Prob=1;
        break;
/*
    case    (uchar)'Б':
        pen = 0;
        if( leo_diskr_right_up_hole(Im3x5) )
            pen += 30;
        num++;
        if( loc->Alt[i].Prob>pen )
            loc->Alt[i].Prob-=pen;
        else
            loc->Alt[i].Prob=1;
        break;
*/
    default:
        break;
    }
return num;
}

int32_t leo_prn_undef(RecVersions *ver)
{
if( ver->lnAltCnt<2 )
    return 0;
if( ver->Alt[0].Prob==ver->Alt[1].Prob )
    {
     if( ver->lnAltCnt>2 &&  ver->Alt[2].Prob==ver->Alt[1].Prob)
        return 3;
    return 2; // lnAltCnt==2 Alt or Alt[2].Prob<Alt[1].Prob
    }
if( ver->Alt[0].Prob==ver->Alt[1].Prob+1 )
    return 1;
return 0;
}

void leo_reverse_russian(RecVersions *ver, uchar Code1, uchar Code2)
{
    int32_t i_1=leo_exist_code(ver,Code1),
          i_2=leo_exist_code(ver,Code2), i, i_22;
	uchar p;
    if( ver->lnAltCnt==REC_MAX_VERS )
        return;
    if( i_1!=-1 && i_2!=-1 && i_1>i_2 )
        {
        ver->Alt[i_2].Code=Code1;
        ver->Alt[i_1].Code=Code2;
        i=i_1;
        i_1=i_2;
        i_2=i;
        if( ver->Alt[i_2].Prob==ver->Alt[i_1].Prob )
            ver->Alt[i_2].Prob--;
        }
    if( (i_1==0 && i_2==-1||i_2==0 && i_1==-1)  &&
        ver->Alt[0].Prob>10 )
        {
        ver->Alt[ver->lnAltCnt]=ver->Alt[0];
        if( i_1==0 )
            {
            ver->Alt[ver->lnAltCnt].Prob -= 10;
            ver->Alt[ver->lnAltCnt].Code = Code2  ;
            }
        else
            {
            ver->Alt[i_2].Prob -= 10;
            ver->Alt[ver->lnAltCnt].Code = Code1 ;
            }
        ver->lnAltCnt++;
        leo_sort_vers_prob(ver);
		if( ver->Alt[0].Prob>240 )
			leo_set_max_vers(ver, ver->Alt[0].Prob-5);

        }
	if( i_1!=-1 && i_2!=-1 )
		{
		if( ver->Alt[0].Prob>240 )
			leo_set_max_vers(ver, ver->Alt[0].Prob-5);
		}
	i_22=leo_exist_code(ver,Code2);
	if( i_1==0 && i_22>1 )
		{
		p = ver->Alt[i_22].Prob;
		ver->Alt[i_22].Prob=ver->Alt[1].Prob;
		ver->Alt[1].Prob   = p;
		leo_sort_vers_prob(ver);
		}
return;
}

void leo_kill_double_russian(RecVersions *ver, uchar Code1, uchar Code2)
{
    int32_t i_1=leo_exist_code(ver,Code1),
          i_2=leo_exist_code(ver,Code2);
int32_t i,ii;
RecVersions v={0};
v.lnAltMax = REC_MAX_VERS;
if( i_2==-1 )
    return;
if( i_1==-1 )
    {
    ver->Alt[i_2].Code=Code1;
    return;
    }

for(ii=i=0;i<ver->lnAltCnt;i++)
    {
    if( ver->Alt[i].Code!=Code1 && ver->Alt[i].Code!=Code2 )
        v.Alt[ii++] = ver->Alt[i];
    }

v.Alt[ii]=ver->Alt[i_1];
v.Alt[ii].Prob=MAX(ver->Alt[i_1].Prob,ver->Alt[i_2].Prob);
ii++;
v.lnAltCnt=ii;
leo_sort_vers_prob(&v);
*ver = v;

return;
}

uchar leo_down_prob(uchar prob, uchar dis)
{
if( prob>dis )
    prob -= dis;
else
    prob = 1;
return prob;
}
// Print letters
Bool32 leoRecogPrintAllChar( RecObject*  object )
{
RecVersions ver, loc, per, msk, r35;
uint16_t      CompImage16x16[256];
Bool32      f16x16;
int32_t       over_bonus=0, unter=0;
uchar       prob_3x5[256]={0}, over[256]={0};
RecRaster   recR;
Bool32      disable_dis=FALSE, disable_r35=FALSE, per_nonrec=FALSE;
int32_t       wid, hei;

DIFInit(&object->recData.recRaster,
        (object->recData.lwCompCnt>1),
        (object->recData.lwStatus&REC_STATUS_BROKEN_II),
        (object->recData.lwStatus&REC_STATUS_METHODS_CUT_LEFT),
        (object->recData.lwStatus&REC_STATUS_METHODS_CUT_RIGHT));
if( leo_is_stick( object ) )
    return TRUE;


f16x16=0;

if( leo_enable_fon_recog )
    {
    FONRecogChar(&object->recData.recRaster,&ver,NULL);
    if( ver.lnAltCnt && ver.Alt[0].Prob>200 )
        {
        leo_compress_prn(&ver);
        leo_snapChar(&ver,"LEO PRN FON : ",0);
        leo_decode_to_ansi( &ver );
        memcpy(&object->recResults,&ver,sizeof(RecVersions));
        return TRUE;
        }
    }

wid= object->recData.recRaster.lnPixWidth;
hei= object->recData.recRaster.lnPixHeight;

memset(&ver,0,sizeof(RecVersions));
ver.lnAltMax = REC_MAX_VERS;

memset(&msk,0,sizeof(RecVersions));
msk.lnAltMax = REC_MAX_VERS;

memset(&r35,0,sizeof(RecVersions));
r35.lnAltMax = REC_MAX_VERS;

memset(&loc,0,sizeof(RecVersions));
loc.lnAltMax = REC_MAX_VERS;

memset(&per,0,sizeof(RecVersions));
per.lnAltMax = REC_MAX_VERS;

// GENERATORS PASS

EVNRecogCharPRN(&object->recData.recRaster, &ver);
if( object->recData.lwCompCnt>1 && !ver.lnAltCnt &&
    R35Pack(&object->recData.recRaster, CompImage16x16,16,16) &&
    !(object->recData.lwStatus&REC_STATUS_BROKEN_II))
	{
    f16x16=1;
	R35Binarize(&recR, CompImage16x16,16,16);
	EVNRecogCharPRN(&recR, &ver);
	}
if( object->recData.lwCompCnt==1 && ver.lnAltCnt==0 )
    unter++;

object->lwStatusMethods |= REC_STATUS_METHODS_EVN;
leo_reduce_typefaces(&ver);
leo_compress( &ver );
if( ver.lnAltCnt==1 )
        over[ ver.Alt[0].Code ]++;

if( ver.lnAltCnt && per.lnAltCnt && per.Alt[0].Prob>80 &&
    !leo_strchr_codes_ansi("ьЬ",per.Alt[0].Code))
        if( ver.lnAltCnt==1 && leo_comp_codes(ver.Alt[0].Code,per.Alt[0].Code) ||
            per.Alt[0].Prob>200 && leo_exist_code(&ver, per.Alt[0].Code)!=-1     )
            {

            loc = ver;
            DIFPenaltyChar( &object->recData.recRaster,  &loc);
            if( loc.Alt[0].Prob==per.Alt[0].Prob )
                {
                uchar let=stdAsciiToAnsi(per.Alt[0].Code);
                per.Alt[0].Prob=255;
                if( leo_alpha_type==ALPH_DIG )
                    {
                        switch(let)
                        {
                        case    (uchar)'О':
                            per.Alt[0].Code='0';
                            break;
                        case    (uchar)'З':
                            per.Alt[0].Code='3';
                            break;
                        case    (uchar)'Ч':
                            per.Alt[0].Code='4';
                            break;
                        }
                    }
                leo_snapChar(&ver,"LEO PRN EVN & PER & DIS : ",0);
                ver = per;
                goto XOPOIII_HET3X5;
                }

            }

if( alphabet['N'] )
    {    // KNOT for recog N
    if( ver.lnAltCnt==1 )
        {
        if( leo_exist_code(&ver, (uchar)'N')!=-1   )
            {
            loc = ver;
            loc.Alt[0].Prob=255;
            DIFPenaltyChar( &object->recData.recRaster,  &loc);
            if( loc.Alt[0].Prob>200 )
                {
                ver.Alt[0].Prob=254;
                goto XOPOIII_HET3X5;
                }
            }
        if( leo_exist_code(&ver, (uchar)'#')!=-1 &&
            per.lnAltCnt && per.Alt[0].Prob>150  )
            {
            loc = ver;
            if( loc.Alt[0].Prob>200 )
                {
                ver.Alt[0].Prob=254;
                goto XOPOIII_HET3X5;
                }
            }
        }
    }


leo_snapChar(&ver,"LEO PRN EVN : ",0);

if( !(object->recData.lwStatus & REC_STATUS_V3X5) )
	{
    R35Pack(&object->recData.recRaster, object->recData.v3x5,3,5);
	object->recData.lwStatus |= REC_STATUS_V3X5;
	}

if( ver.lnAltCnt  )
  { // EXPERT PASS
  int32_t ind;
  r35 = ver;
  msk = ver;
  ver.lnAltCnt=0;

  R35RecogPrintCharIm3x5_expert(object->recData.v3x5, &r35, FALSE);
  if( leo_add_vers(&ver,&r35) )
	{
    add_to_prob_array(prob_3x5,&r35);
	object->lwStatusMethods |= REC_STATUS_METHODS_3X5;
    }
  leo_sort_vers_prob(&r35);
  leo_reduce_typefaces(&r35);
  if( r35.lnAltCnt && r35.Alt[0].Prob>200 )
        over[ r35.Alt[0].Code ]++;

  MSKRecogCharExp(leo_MSK_ndx[1],&object->recData.recRaster, &msk);
  leo_sort_vers_prob(&msk);
  leo_reduce_typefaces(&msk);

  if( leo_add_vers(&ver,&msk) )
		object->lwStatusMethods |= REC_STATUS_METHODS_MSK;
  if( msk.lnAltCnt==0 || msk.Alt[0].Prob<150 )
        unter++;
  if( msk.lnAltCnt && msk.Alt[0].Prob>200 )
        over[ msk.Alt[0].Code ]++;
  else if(leo_alpha_type==ALPH_DIG)
    {
    ind = leo_exist_code(&msk,per.Alt[0].Code);
    if( per.lnAltCnt && per.Alt[0].Prob>190 &&  ind!=-1 && msk.Alt[ind].Prob>120 )
        over[ per.Alt[0].Code ]++;
    }
  if( f16x16 )
    {
    if( leo_add_vers(&ver,&per) )
	    object->lwStatusMethods |= REC_STATUS_METHODS_NCU;
    }
  leo_sort_vers_prob( &ver );
  leo_snapChar(&ver,"LEO PRN EXPERT Per+3x5+MSK : ",0);
  }

if( !ver.lnAltCnt               ||
    ver.lnAltCnt && ver.Alt[0].Prob<150
    )
	{  // CONSILIUM
    int32_t ind;
	ver.lnAltCnt=0;

    R35RecogPrintCharIm3x5(object->recData.v3x5, &r35);
    for(ind=0;ind<r35.lnAltCnt;ind++)
        {
        if( wid*2<=hei*3 && strchr("1",r35.Alt[ind].Code) &&
            r35.Alt[ind].Prob>50)
            r35.Alt[ind].Prob -= 50;
        }
    leo_reduce_typefaces(&r35);
    if( r35.lnAltCnt && r35.Alt[0].Prob>200 )
        over[ r35.Alt[0].Code ]++;
	if( leo_add_vers(&ver,&r35) )
		{
        add_to_prob_array(prob_3x5,&r35);
	    object->lwStatusMethods |= REC_STATUS_METHODS_3X5;
        }

    MSKRecogChar(leo_MSK_ndx[1],&object->recData.recRaster, &msk);
    leo_reduce_typefaces(&msk);

    if( msk.lnAltCnt==0 || msk.Alt[0].Prob<150 )
        unter++;
	if( leo_add_vers(&ver,&msk) )
		object->lwStatusMethods |= REC_STATUS_METHODS_MSK;
    if( msk.lnAltCnt && msk.Alt[0].Prob>200 )
        over[ msk.Alt[0].Code ]++;
    else if(leo_alpha_type==ALPH_DIG)
        {
        ind = leo_exist_code(&msk,per.Alt[0].Code);
        if( per.lnAltCnt && per.Alt[0].Prob>190 &&  ind!=-1 && msk.Alt[ind].Prob>120 )
            over[ per.Alt[0].Code ]++;
        }
    if( leo_add_vers(&ver,&per) )
        object->lwStatusMethods |= REC_STATUS_METHODS_NCU;
    leo_sort_vers_prob( &ver );
	leo_snapChar(&ver,"LEO PRN CONSILIUM Per+3x5+MSK : ",0);
	}

disable_dis=FALSE;
if( ver.lnAltCnt && per.lnAltCnt && r35.lnAltCnt && msk.lnAltCnt)
    {
    if( leo_alpha_type==ALPH_ALL )
        {
        if( alphabet[(uchar)'‡'] && alphabet['3'] )
            {
            leo_kill_double_russian(&ver, (uchar)'‡', '3');
            leo_kill_double_russian(&per, (uchar)'‡', '3');
            leo_kill_double_russian(&r35, (uchar)'‡', '3');
            leo_kill_double_russian(&msk, (uchar)'‡', '3');
            }
        if( alphabet[(uchar)'Ћ'] && alphabet['0'] )
            {
            leo_kill_double_russian(&ver, (uchar)'Ћ', '0');
            leo_kill_double_russian(&per, (uchar)'Ћ', '0');
            leo_kill_double_russian(&r35, (uchar)'Ћ', '0');
            leo_kill_double_russian(&msk, (uchar)'Ћ', '0');
            }
        }
    if( ver.Alt[0].Prob>150 && ver.Alt[0].Method==REC_METHOD_3X5 &&
        ver.Alt[0].Code!=per.Alt[0].Code )
        { // kill unique alt from 3x5
        uchar first=per.Alt[0].Code;
        int32_t i_msk=leo_exist_code(&msk,first);
        int32_t i_r35=leo_exist_code(&r35,first);

        if( ver.Alt[0].Prob<245 )
        if( i_msk==0 ||                 // 180                              200
            i_msk==1 && msk.Alt[0].Prob>150 && i_r35==1 && r35.Alt[0].Prob>180 ||
            i_msk==1 && msk.Alt[0].Prob==msk.Alt[1].Prob && per.Alt[0].Prob>100)
            {
            leo_kill_3x5_unique(&ver,first);
            leo_snapChar(&ver,"LEO PRN kill unique 3x5 Alt: ",0);
            }
        }
if( !leo_strchr_codes_ansi("ШЩЖЗЭ", ver.Alt[0].Code) )
    {
    uchar first=ver.Alt[0].Code;
    int32_t i_r35=leo_exist_code(&r35,first);
    int32_t i_msk=leo_exist_code(&msk,first);
    int32_t i_per=leo_exist_code(&per,first);
    if( i_per==0 && i_r35!=-1 && i_msk!=-1 && per.Alt[0].Prob>180 )
        {
        if( i_r35==0 && i_msk==0 )
            disable_dis=TRUE;
        else if( i_r35==0 && i_msk==1 && msk.Alt[1].Prob>100 )
            disable_dis=TRUE;
        else if( i_msk<2 && i_r35==1 &&
            (r35.Alt[1].Prob>180 || msk.Alt[0].Prob>180))
            disable_dis=TRUE;
        else if( i_msk==0 && msk.Alt[0].Prob>100 && r35.Alt[i_r35].Prob>210 )
            disable_dis=TRUE;
        }
    if( i_per==0 && i_r35==0 && i_msk==0  )
        {
        if( msk.Alt[0].Prob>150 &&
            per.Alt[0].Prob>100 &&
            r35.Alt[0].Prob>230)
            disable_dis=TRUE;
        }
    }
if( disable_dis )
    leo_snapChar(&ver,"LEO PRN disable diskrim : ",0);
}
disable_r35=FALSE;

if( ver.lnAltCnt==3 &&
    ver.Alt[0].Code==ver.Alt[1].Code &&
    ver.Alt[0].Code==ver.Alt[2].Code &&
    (   ver.Alt[0].Method==REC_METHOD_NCU && ver.Alt[0].Prob>220 ||
        ver.Alt[0].Method==REC_METHOD_MSK && ver.Alt[0].Prob>230) )
    disable_r35=TRUE;
if( ver.Alt[0].Code==per.Alt[0].Code && per.Alt[0].Prob>250 )
    disable_r35=TRUE;
if( per.Alt[0].Code==r35.Alt[0].Code &&
    per.Alt[0].Code==msk.Alt[0].Code &&
    per.Alt[0].Prob>230 )
    disable_r35=TRUE;
if( per.Alt[0].Code==r35.Alt[0].Code &&
    per.Alt[0].Code==msk.Alt[0].Code &&
    ver.Alt[0].Prob<170 )
    disable_r35=TRUE;
if( disable_r35 )
    leo_snapChar(&ver,"LEO PRN disable rerecog 3x5 : ",0);

leo_compress_prn(&ver);
leo_reduce_typefaces(&ver);


leo_compress( &ver );

over_bonus = leo_no_hist_leader(over,&ver);
if( over_bonus>2 && !disable_r35 )
{
disable_r35=TRUE;
leo_snapChar(&ver,"LEO PRN disable rerecog 3x5 : ",0);
}

if( over_bonus<2 && ver.lnAltCnt>1 &&
    over[ver.Alt[1].Code]>2 && ver.Alt[0].Prob<220    )
    {
    uchar tmp=ver.Alt[1].Code;
    over_bonus = over[ver.Alt[1].Code];
    ver.Alt[1].Code=ver.Alt[0].Code;
    ver.Alt[0].Code=tmp;
    leo_snapChar(&ver,"LEO PRN exclude from tables of bonus : ",0);
    }
if( wid*3<=hei && ver.lnAltCnt>1 &&
    (   leo_strchr_codes_ansi("ЭЗ3", ver.Alt[0].Code) && ver.Alt[1].Code==')' ||
        leo_strchr_codes_ansi("C", ver.Alt[0].Code) && ver.Alt[1].Code=='(' ) )
    {
    uchar tmp=ver.Alt[1].Code;
    ver.Alt[1].Code=ver.Alt[0].Code;
    ver.Alt[0].Code=tmp;
    leo_snapChar(&ver,"LEO PRN up braces : ",0);
    }
if( leo_strchr_codes("1|iI()[]tl", ver.Alt[0].Code) )
    {
    if( !(wid*5>=hei*2 && disable_dis &&
        (ver.lnAltCnt==1||ver.lnAltCnt>1&&ver.Alt[0].Prob>ver.Alt[1].Prob+100) ) )
        {
        leo_snapChar(&ver,"LEO PRN BEFORE DISKRIM STICKS : ",0);
        leo_diskrim_stick(&ver);
        leo_snapChar(&ver,"LEO PRN AFTER DISKRIM STICKS : ",0);
        }
    }
else
    { // NON STICKs
    Bool32 inp=( ver.lnAltCnt>1 && // тут должны быть разделители
            leo_strchr_codes_ansi("МNИНПВминпв", ver.Alt[0].Code) );
    Bool32 ce=( ver.lnAltCnt>1 && // тут должны быть разделители
            leo_strchr_codes_ansi("Ссе", ver.Alt[0].Code) );
    if( (inp && (object->recData.lwStatus&REC_STATUS_BROKEN_II)) )
        {
        leo_snapChar(&ver,"LEO PRN Broken II disable rerecog 3x5 : ",0);
        }
    if( !disable_r35 &&
        ver.lnAltCnt &&
        leo_strchr_codes("%#", ver.Alt[0].Code) &&
        ver.Alt[0].Prob>200 )
        {
        disable_r35=TRUE;
        leo_snapChar(&ver,"LEO PRN disable rerecog 3x5 : ",0);
        }
    if( !disable_r35 &&
        inp &&
        (object->recData.lwStatus&REC_STATUS_BROKEN_II)   )
        {
        disable_r35=TRUE;
        leo_snapChar(&ver,"LEO PRN disable rerecog 3x5 : ",0);
        }
    if( !disable_r35 )
        {
        leo_snapChar(&ver,"LEO PRN BEFORE RERECOG3x5 : ",0);
        if( !(object->recData.lwStatus & REC_STATUS_V3X5) )
	        {
	        R35Pack(&object->recData.recRaster, object->recData.v3x5,3,5);
	        object->recData.lwStatus |= REC_STATUS_V3X5;
	        }
        loc=ver;
        leo_expert_prob(   &ver, object->recData.v3x5, &object->recData.recRaster,
                            prob_3x5, prob_3x5, 1 , (over_bonus>2) , TRUE);
        if( ver.Alt[0].Prob>loc.Alt[0].Prob )
            ver = loc;
        }

    if( !(leo_typ_of_font&(LEO_FONT_TW|LEO_FONT_MTR)) && over_bonus<3 && !disable_dis )
        {
        uchar   pr;

        loc = ver;
        leo_snapChar(&ver,"LEO PRN BEFORE DISKRIMINATORs : ",0);
        DIFPenaltyChar( &object->recData.recRaster,  &loc);
        pr = loc.Alt[0].Prob;
        if( pr!=ver.Alt[0].Prob && !inp && !ce )
            {
            if( ver.lnAltCnt &&
                ver.Alt[0].Prob==ver.Alt[1].Prob &&
                loc.Alt[1].Prob<pr )
                pr = loc.Alt[1].Prob;
            leo_set_max_vers(&ver, pr);
            leo_snapChar(&ver,"LEO PRN AFTER DISKRIMS : ",0);
            }
        else
            { // Тут работа с ИНПами - то есть настоящие перестановки
            uchar oldcode=ver.Alt[0].Code;
            ver=loc;
            leo_sort_vers_prob( &ver );
            if( !leo_strchr_codes("()", ver.Alt[0].Code) )
            if( oldcode!=ver.Alt[0].Code  && // over_bonus<3  &&
                !(leo_strchr_codes("%#", ver.Alt[0].Code)&&ver.Alt[0].Prob>200) &&
                !(inp && (object->recData.lwStatus&REC_STATUS_BROKEN_II)))
                    {
                    leo_snapChar(&ver,"LEO PRN BEFORE RERECOG3x5 : ",0);
                    if( !(object->recData.lwStatus & REC_STATUS_V3X5) )
	                    {
	                    R35Pack(&object->recData.recRaster, object->recData.v3x5,3,5);
	                    object->recData.lwStatus |= REC_STATUS_V3X5;
	                    }

                    leo_expert_prob(   &ver, object->recData.v3x5, &object->recData.recRaster,
                                        prob_3x5, prob_3x5, 1 , (over_bonus>2) , TRUE);

                    }

            leo_snapChar(&ver,"LEO PRN AFTER DISKRIMS : ",0);
            }
        } // end non TW

    } // end non sticks
if( leo_enable_fon_recog )
    {
    leo_set_max_vers(&ver, ver.Alt[0].Prob>60 ? ver.Alt[0].Prob-60 : 1);
    leo_snapChar(&ver,"LEO PRN DOWN PROBs (refuse FONT recog) : ",0);
    }
XOPOIII_HET3X5:;

leo_compress_prn(&ver);
leo_reduce_typefaces(&ver);
leo_compress( &ver );
if( leo_alpha_type==ALPH_ALL )
    {
    if( alphabet[(uchar)'‡'] && alphabet['3'] )
        leo_reverse_russian(&ver, (uchar)'‡', '3');
    if( alphabet[(uchar)'Ћ'] && alphabet['0'] )
        leo_reverse_russian(&ver, (uchar)'Ћ', '0');
    }
if( !( leo_alpha_type==ALPH_ALL && ver.lnAltCnt && leo_strchr_codes_ansi("ОЗ", ver.Alt[0].Code)) )
if( per.lnAltCnt && ver.lnAltCnt && leo_comp_codes(ver.Alt[1].Code,per.Alt[0].Code) )
    {
    switch( leo_prn_undef(&ver) )
        {
        case    1: // C1(P),C2(P-1),...
            if( per.Alt[0].Prob>200 )
                {
                ver.Alt[1].Code = ver.Alt[0].Code;
                ver.Alt[0].Code = per.Alt[0].Code;
                leo_snapChar(&ver,"LEO PRN Per-expert : ",0);
                }
            break;
        case    2: // C1(P),C2(P),...
            if( per.Alt[0].Prob>150 )
                {
                ver.Alt[1].Code = ver.Alt[0].Code;
                ver.Alt[0].Code = per.Alt[0].Code;
                leo_snapChar(&ver,"LEO PRN Per-expert : ",0);
                }
            break;
        case    3: // C1(P),C2(P),C3(P),...
            ver=per;
            leo_compress_prn(&ver);
            leo_reduce_typefaces(&ver);
            leo_compress( &ver );
            leo_snapChar(&ver,"LEO PRN Per-expert : ",0);
            break;
        default:
            break;
        }
    }
if( per.lnAltCnt && ver.lnAltCnt>1 && ver.Alt[0].Prob==ver.Alt[1].Prob && ver.Alt[0].Prob>200 &&
    per.Alt[0].Prob>120 && ver.Alt[1].Code==per.Alt[0].Code )
    {
    ver.Alt[1].Code=ver.Alt[0].Code;
    ver.Alt[0].Code=per.Alt[0].Code;
    leo_snapChar(&ver,"LEO PRN Per-expert : ",0);
    }
if( alphabet['N'] )
    {
    int32_t i_N=leo_exist_code(&ver,'N');
    if( i_N!=-1 && ver.Alt[i_N].Method==REC_METHOD_3X5 &&
        ver.Alt[0].Prob>240 &&  ver.Alt[i_N].Prob>200 )
        {
        ver.Alt[i_N].Prob=199;
        leo_snapChar(&ver,"LEO PRN Down \'N\'-Alt : ",0);
        }
    }
if( per_nonrec && ver.lnAltCnt && ver.Alt[0].Prob>10 )
    {
    ver.Alt[0].Prob -= 10;
    leo_sort_vers_prob(&ver);
    }
leo_decode_to_ansi( &ver );
if( prn_roma_regim )
    {
    leo_decode_to_roma( &ver );
    leo_compress( &ver );
    }

memcpy(&object->recResults,&ver,sizeof(RecVersions));


return TRUE;
}


void leo_smoothing16(RecRaster *rr,RecRaster *rr_sm)
{
int i,ii;
uint16_t p,n,c;
rr_sm->lnPixWidth= rr->lnPixWidth;
rr_sm->lnPixHeight=rr->lnPixHeight;
rr_sm->lnRasterBufSize=rr->lnRasterBufSize;
memset(rr_sm->Raster,0,16*8);
c = *((int16_t*)&rr->Raster[0]);
memcpy(&rr_sm->Raster[0],&c,2);
for(ii=8,i=1;i<15;i++,ii+=8)
    {
    p = *((int16_t*)&rr->Raster[ii-16]);
    c = *((int16_t*)&rr->Raster[ii]);
    n = *((int16_t*)&rr->Raster[ii+16]);
    if( !c )
        c = p|n;
    memcpy(&rr_sm->Raster[ii],&c,2);
    }
c = *((int16_t*)&rr->Raster[ii]);
memcpy(&rr_sm->Raster[ii],&c,2);
return;
}
Bool32 leoRecogPrintDotChar( RecObject*  object )
{
RecVersions ver, loc,  per, tmp, msk, r35;
uint16_t      CompImage16x16[256];
Bool32      f16x16, disable_dis, disable_r35, per_nonrec=FALSE  ;
int32_t       over_bonus=0, not_n=0;
uchar       prob_3x5[256]={0}, over[256]={0};
RecRaster   recR,rr, rr_sm;

if( R35Pack(&object->recData.recRaster, CompImage16x16,16, 16) )
    {
    f16x16=1;
    R35Binarize(&recR, CompImage16x16,16, 16);
    memcpy(&rr, &recR, sizeof(RecRaster));

    leo_smoothing16(&rr,&rr_sm);
    //object->recData.recRaster = rr_sm;
    }


    DIFInit(&object->recData.recRaster,
        (object->recData.lwCompCnt>1),
        (object->recData.lwStatus&REC_STATUS_BROKEN_II),0,0);
if( leo_is_stick( object ) )
    return TRUE;


f16x16=0;

if( leo_enable_fon_recog )
    {
    FONRecogChar(&object->recData.recRaster,&ver,NULL);
    if( ver.lnAltCnt && ver.Alt[0].Prob>200 )
        {
        leo_compress_prn(&ver);
        leo_snapChar(&ver,"LEO PRN FON : ",0);
        leo_decode_to_ansi( &ver );
        memcpy(&object->recResults,&ver,sizeof(RecVersions));
        return TRUE;
        }
    }


memset(&ver,0,sizeof(RecVersions));
ver.lnAltMax = REC_MAX_VERS;

memset(&tmp,0,sizeof(RecVersions));
tmp.lnAltMax = REC_MAX_VERS;

memset(&loc,0,sizeof(RecVersions));
loc.lnAltMax = REC_MAX_VERS;

memset(&r35,0,sizeof(RecVersions));
r35.lnAltMax = REC_MAX_VERS;

memset(&msk,0,sizeof(RecVersions));
msk.lnAltMax = REC_MAX_VERS;

memset(&per,0,sizeof(RecVersions));
per.lnAltMax = REC_MAX_VERS;

// GENERATORS PASS

// events
EVNRecogCharPRN(&object->recData.recRaster, &ver);
if( !ver.lnAltCnt && R35Pack(&object->recData.recRaster, CompImage16x16,16,16) )
	{
    f16x16=1;
	R35Binarize(&recR, CompImage16x16,16,16);
	EVNRecogCharPRN(&recR, &ver);
	}


object->lwStatusMethods |= REC_STATUS_METHODS_EVN;
leo_reduce_typefaces(&ver);

if( alphabet['N'] )
    {    // KNOT for recog N
    if( ver.lnAltCnt )
        {
        if( (leo_exist_code(&ver, (uchar)'N')!=-1 ) && ver.lnAltCnt==1 )
            {
            leo_snapChar(&ver,"LEO PRN MTR EVN No : ",0);
            goto XOPOIII_HET3X5;
            }
        }
    }
NOT_N:;
if( ver.lnAltCnt==1 )
        over[ ver.Alt[0].Code ]++;

ver.lnAltCnt=0;


if( !(object->recData.lwStatus & REC_STATUS_V3X5) )
	{
    R35Pack(&object->recData.recRaster, object->recData.v3x5,3,5);
	object->recData.lwStatus |= REC_STATUS_V3X5;
	}

    {  // CONSILIUM
    int32_t ind;
	ver.lnAltCnt=0;

    if( (leo_typ_of_font&LEO_FONT_MTR) &&
        ver.lnAltCnt && ver.Alt[0].Prob>30 )
        leo_set_max_vers(&ver,ver.Alt[0].Prob-30);

	R35RecogPrintCharIm3x5(object->recData.v3x5, &r35);
    leo_reduce_typefaces(&r35);
	if( leo_add_vers(&ver,&r35) )
		{
        add_to_prob_array(prob_3x5,&r35);
	    object->lwStatusMethods |= REC_STATUS_METHODS_3X5;
        }
    if( r35.lnAltCnt && r35.Alt[0].Prob>200 )
        over[ r35.Alt[0].Code ]++;
    else
        {
    ind = leo_exist_code(&r35,per.Alt[0].Code);
    if( per.lnAltCnt && per.Alt[0].Prob<=240 && per.Alt[0].Prob>190 &&  ind!=-1 && loc.Alt[ind].Prob>180 )
        over[ per.Alt[0].Code ]++;
        }
    MSKRecogChar(leo_MSK_ndx[2],&object->recData.recRaster,&msk);
    leo_reduce_typefaces(&msk);

	if( leo_add_vers(&ver,&msk) )
		object->lwStatusMethods |= REC_STATUS_METHODS_MSK;

    if( msk.lnAltCnt && msk.Alt[0].Prob>200 )
        over[ msk.Alt[0].Code ]++;
    else
        {
    ind = leo_exist_code(&msk,per.Alt[0].Code);
    if( per.lnAltCnt && per.Alt[0].Prob>190 &&  ind!=-1 && loc.Alt[ind].Prob>120 )
        over[ per.Alt[0].Code ]++;
        }

    if( leo_add_vers(&ver,&per) )
        object->lwStatusMethods |= REC_STATUS_METHODS_NCU;
    leo_sort_vers_prob( &ver );
	leo_snapChar(&ver,"LEO PRN MTR CONSILIUM 3x5+MSK+Per : ",0);
	}


leo_compress_prn(&ver);
leo_reduce_typefaces(&ver);
leo_compress( &ver );
disable_dis=FALSE;
if( ver.lnAltCnt && per.lnAltCnt && r35.lnAltCnt && msk.lnAltCnt)
{
if( leo_alpha_type==ALPH_ALL )
    {
    if( alphabet[(uchar)'‡'] && alphabet['3'] )
        {
        leo_kill_double_russian(&ver, (uchar)'‡', '3');
        leo_kill_double_russian(&per, (uchar)'‡', '3');
        leo_kill_double_russian(&r35, (uchar)'‡', '3');
        leo_kill_double_russian(&msk, (uchar)'‡', '3');
        }
    if( alphabet[(uchar)'Ћ'] && alphabet['0'] )
        {
        leo_kill_double_russian(&ver, (uchar)'Ћ', '0');
        leo_kill_double_russian(&per, (uchar)'Ћ', '0');
        leo_kill_double_russian(&r35, (uchar)'Ћ', '0');
        leo_kill_double_russian(&msk, (uchar)'Ћ', '0');
        }
    }
if( ver.Alt[0].Prob>150 && ver.Alt[0].Method==REC_METHOD_3X5 &&
    ver.Alt[0].Code!=per.Alt[0].Code )
    { // kill unique alt from 3x5
    uchar first=per.Alt[0].Code;
    int32_t i_msk=leo_exist_code(&msk,first);

    if( i_msk==0 || i_msk==1 && msk.Alt[0].Prob==msk.Alt[1].Prob && per.Alt[0].Prob>100)
        {
        leo_kill_3x5_unique(&ver,first);
        leo_snapChar(&ver,"LEO PRN MTR kill unique 3x5 Alt: ",0);
        }
    }

if( !leo_strchr_codes_ansi("ШЩЖ", ver.Alt[0].Code) )
    {
    uchar first=ver.Alt[0].Code;
    int32_t i_r35=leo_exist_code(&r35,first);
    int32_t i_msk=leo_exist_code(&msk,first);
    int32_t i_per=leo_exist_code(&per,first);
    if( i_per==0 && i_r35!=-1 && i_msk!=-1 && per.Alt[0].Prob>180 )
        {
        if( i_r35==0 && i_msk==0 )
            disable_dis=TRUE;
        else if( i_r35==0 && i_msk==1 && msk.Alt[1].Prob>100 )
            disable_dis=TRUE;
        else if( i_msk<2 && i_r35==1 &&
            (r35.Alt[1].Prob>180 || msk.Alt[0].Prob>180))
            disable_dis=TRUE;
        else if( i_msk==0 && r35.Alt[i_r35].Prob>200 && msk.Alt[0].Prob>180 )
            disable_dis=TRUE;
        }
    }
if( disable_dis )
    leo_snapChar(&ver,"LEO PRN MTR disable diskrim : ",0);
}
disable_r35=FALSE;

if( ver.lnAltCnt==3 &&
    ver.Alt[0].Code==ver.Alt[1].Code &&
    ver.Alt[0].Code==ver.Alt[2].Code &&
    (   ver.Alt[0].Method==REC_METHOD_NCU && ver.Alt[0].Prob>220 ||
        ver.Alt[0].Method==REC_METHOD_MSK && ver.Alt[0].Prob>230) )
    disable_r35=TRUE;
if( ver.Alt[0].Code==per.Alt[0].Code && per.Alt[0].Prob>250 )
    disable_r35=TRUE;
if( per.Alt[0].Code==r35.Alt[0].Code &&
    per.Alt[0].Code==msk.Alt[0].Code &&
    per.Alt[0].Prob>230 )
    disable_r35=TRUE;
if( per.Alt[0].Code==r35.Alt[0].Code &&
    per.Alt[0].Code==msk.Alt[0].Code &&
    ver.Alt[0].Prob<170 )
    disable_r35=TRUE;
if( disable_r35 )
    leo_snapChar(&ver,"LEO PRN MTR disable rerecog 3x5 : ",0);

over_bonus = leo_no_hist_leader(over,&ver);
if( over_bonus<2 && ver.lnAltCnt>1 && over[ver.Alt[1].Code]>2 )
    {
    uchar tmp=ver.Alt[1].Code;
    over_bonus = over[ver.Alt[1].Code];
    ver.Alt[1].Code=ver.Alt[0].Code;
    ver.Alt[0].Code=tmp;
    }
if( over_bonus>2 && !disable_r35 )
    {
    disable_r35=TRUE;
    leo_snapChar(&ver,"LEO PRN MTR disable rerecog 3x5 : ",0);
    }
if( !disable_r35 &&   ver.lnAltCnt && ver.Alt[0].Prob>200 &&
    leo_strchr_codes("%#", ver.Alt[0].Code) )
        {
        disable_r35=TRUE;
        leo_snapChar(&ver,"LEO PRN MTR disable rerecog 3x5 : ",0);
        }
if( !(object->recData.lwStatus & REC_STATUS_V3X5) )
	        {
	        R35Pack(&object->recData.recRaster, object->recData.v3x5,3,5);
	        object->recData.lwStatus |= REC_STATUS_V3X5;
	        }

    { // NON STICKs
    if( !disable_r35 && !leo_strchr_codes("1|iI()[]tl", ver.Alt[0].Code))
        {
        leo_snapChar(&ver,"LEO PRN MTR BEFORE RERECOG3x5 : ",0);


        leo_expert_prob(   &ver, object->recData.v3x5, &object->recData.recRaster,
                            prob_3x5, prob_3x5, 1 , FALSE, FALSE);
    }

    if( !disable_dis )
    if(  (leo_typ_of_font&(LEO_FONT_MTR)) && over_bonus<3 )
        {
        loc = ver;
        leo_snapChar(&ver,"LEO PRN BEFORE MTR DISKRs : ",0);
        if( LEO_DIFPenaltyCharMTR(&rr, object->recData.v3x5, &loc) )
        if( ver.lnAltCnt>1 )
            {
            ver=loc;
            leo_sort_vers_prob( &ver );
            }
        }
    } // end non sticks

if( ver.lnAltCnt==1 &&
    ver.Alt[0].Code=='N' &&
    ver.Alt[0].Method==REC_METHOD_EVN &&
    ver.Alt[0].Prob<100 &&
    !not_n)
    {
    leo_snapChar(&ver,"LEO PRN REFUSE EVN No : ",0);
    not_n++;
    goto NOT_N;
    }
if( leo_enable_fon_recog )
    {
    leo_set_max_vers(&ver, ver.Alt[0].Prob>60 ? ver.Alt[0].Prob-60 : 1);
    leo_snapChar(&ver,"LEO PRN MTR DOWN PROBs (refuse FONT recog) : ",0);
    }
XOPOIII_HET3X5:;

leo_compress_prn(&ver);
leo_reduce_typefaces(&ver);
leo_compress( &ver );
if( leo_alpha_type==ALPH_ALL )
    {
    if( alphabet[(uchar)'‡'] && alphabet['3'] )
        leo_reverse_russian(&ver, (uchar)'‡', '3');
    if( alphabet[(uchar)'Ћ'] && alphabet['0'] )
        leo_reverse_russian(&ver, (uchar)'Ћ', '0');
    }
if( per_nonrec && ver.lnAltCnt && ver.Alt[0].Prob>10 )
    {
    ver.Alt[0].Prob -= 10;
    leo_sort_vers_prob(&ver);
    }
leo_decode_to_ansi( &ver );
if( prn_roma_regim )
    {
    leo_decode_to_roma( &ver );
    leo_compress( &ver );
    }

memcpy(&object->recResults,&ver,sizeof(RecVersions));

return TRUE;
}

Bool32 leoRecogPrintChar( RecObject*  object )
{
Bool32 ret;
if( leo_typ_of_font&LEO_FONT_MTR )
    ret=leoRecogPrintDotChar( object );
else
    ret=leoRecogPrintAllChar( object );
return ret;
}

Bool32 leoRecogCharPRN_expert(RecRaster*  recRaster,
            RecVersions* over)
{
RecVersions ver;
uint16_t      Im3x5[16];
RecRaster   recR;
uint16_t      CompImage[32*32];
int		    Cnts[256];


memset(Cnts,0,sizeof(Cnts));
memset(&ver,0,sizeof(RecVersions));
ver.lnAltMax = REC_MAX_VERS;

EVNRecogCharPRN(recRaster, &ver);
if( !ver.lnAltCnt && R35Pack(recRaster, CompImage,16,16) )
	{
	R35Binarize(&recR, CompImage,16,16);
	EVNRecogCharPRN(&recR, &ver);
    LeoAddPVers(Cnts,&ver);
    leo_snapChar(&ver,"EXPERT EVN PRN: ",0);
    }

R35Pack(recRaster, Im3x5,3,5);
memset(&ver,0,sizeof(RecVersions));
ver.lnAltMax = REC_MAX_VERS;
R35RecogPrintCharIm3x5(Im3x5, &ver);
LeoAddPVers(Cnts,&ver);
leo_snapChar(&ver,"EXPERT R35 PRN: ",0);

memset(&ver,0,sizeof(RecVersions));
ver.lnAltMax = REC_MAX_VERS;
MSKRecogChar(leo_MSK_ndx[1],recRaster, &ver);
LeoAddPVers(Cnts,&ver);
leo_snapChar(&ver,"EXPERT MSK PRN: ",0);

//leo_snapChar(over,"EXPERT TEST COLLECTION: ",0);
leoProbsToVer_expert(Cnts,&ver,over);
leo_sort_vers_prob(&ver);
leo_compress( &ver );
memcpy(over,&ver,sizeof(RecVersions));

leo_snapChar(&ver,"EXPERT PROP HIST PRN: ",0);
leo_decode_to_ansi(over);

return TRUE;
}

LEO_FUNC(Bool32)  LEORecogPrnMethod( RecObject*  object , int Method, int type)
{
RecVersions ver;
uint16_t      CompImage16x16[16*16];

leo_init_prn();
memset(&ver,0,sizeof(RecVersions));
ver.lnAltMax = REC_MAX_VERS;
ver.lnAltCnt = 0;
switch( Method )
    {
    case    REC_METHOD_MSK:
        MSKRecogChar(leo_MSK_ndx[type],&object->recData.recRaster, &ver);
        break;

    case    REC_METHOD_EVN:
        EVNRecogCharPRN(  &object->recData.recRaster, &ver);
        break;

    case    REC_METHOD_3X5:
        if( R35Pack(&object->recData.recRaster, CompImage16x16,3,5) )
            R35RecogPrintCharIm3x5(CompImage16x16, &ver);
        break;

    default :
        return FALSE;
    }
memcpy(&object->recResults,&ver,sizeof(RecVersions));

return TRUE;
}

void    leo_cuneiform(RecObject *object)
{
RecVersions ver;
uint16_t      CompImage16x16[16*16];


memset(&ver,0,sizeof(RecVersions));
ver.lnAltMax = REC_MAX_VERS;
ver.lnAltCnt = 0;

EVNRecogCharPRN(  &object->recData.recRaster, &ver);

if( R35Pack(&object->recData.recRaster, CompImage16x16,3,5) )
    {
    R35RecogPrintCharIm3x5_expert(CompImage16x16, &ver,FALSE);
    leo_sort_vers_prob(&ver);
    }
memcpy(&object->recResults,&ver,sizeof(RecVersions));

return ;
}
