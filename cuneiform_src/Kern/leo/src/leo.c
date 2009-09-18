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
/*#include <io.h>*/
#include <ctype.h>

#define PC_TYPE 0

#include "leo.h"
#include "alphaset.h"

#include "r35.h"
#include "evn.h"
#include "msk.h"
#include "std.h"
#include "leo_tune.h"
#include "leo_func.h"
#include "snptools.h"

uchar leo_alpha_type, prn_roma_regim;
//extern int nNdxHei;
extern int32_t   GLU_all,
        GLU_rus ,
        GLU_dig ,
        GLU_rom ,
        GLU_rall;
extern int32_t  leo_MSK_ndx[];

unsigned char alphabet[256];

Bool32 leo_add_vers(RecVersions *dst, RecVersions *src)
{
int i,j;
if( !src->lnAltCnt )
	return FALSE;
for(i=dst->lnAltCnt,j=0;i<REC_MAX_VERS && j<src->lnAltCnt; i++, j++)
  dst->Alt[i] = src->Alt[j];
dst->lnAltCnt = i;
return TRUE;
}

int leo_exist_code(RecVersions *ver, uchar code)
{
int i;
for(i=0;i<ver->lnAltCnt;i++)
	{
	if( ver->Alt[i].Code==code )
		return i;
	}
return -1;
}

int leo_exist_codes(RecVersions *ver, uchar *codes)
{
int i;
for(i=0;i<ver->lnAltCnt;i++)
	{
  if( strchr(codes,ver->Alt[i].Code) )
    return 1;
	}
return 0;
}

int32_t cmp_prob_info( const void *a, const void *b)
{
return (int32_t)(((RecAlt *)a)->Info) - (int32_t)(((RecAlt *)b)->Info) ;
}


int leo_sort_vers_info(RecVersions *v)
{
stdQsort(v->Alt,v->lnAltCnt,sizeof(RecAlt),cmp_prob_info);
return v->lnAltCnt;
}

int32_t cmp_prob( const void *a, const void *b)
{
return (int32_t)(((RecAlt *)b)->Prob) - (int32_t)(((RecAlt *)a)->Prob) ;
}

int leo_sort_vers_prob(RecVersions *v)
{
int i,n0,n1;
uchar c0,c1;
RecAlt a0,a1;
stdQsort(v->Alt,v->lnAltCnt,sizeof(RecAlt),cmp_prob);
if( v->lnAltCnt>1 && v->Alt[0].Prob==v->Alt[1].Prob )
	{
	c0 = v->Alt[0].Code;
	c1 = v->Alt[1].Code;
    a0 = v->Alt[0];
	a1 = v->Alt[1];

	for(n1=n0=i=0;i<v->lnAltCnt; i++)
		{
		n0 += (v->Alt[i].Code==c0);
		n1 += (v->Alt[i].Code==c1);
		}
	if( n1>n0 )
		{
        //a0.Code=c1;
		//a1.Code=c0 ;
        v->Alt[0]=a1;
		v->Alt[1]=a0;

		}
	}
return v->lnAltCnt;
}

void leo_clear( RecVersions *v)
{
RecVersions c;
int i,j;

memcpy(&c,v,sizeof(RecVersions));
for(j=i=0;i<v->lnAltCnt;i++)
	{
	if( c.Alt[i].Prob>MIN_PROB )
		{
		memcpy(&v->Alt[j++],&c.Alt[i],sizeof(RecAlt));
		}
	}

v->lnAltCnt = j;

return;
}

void leo_compress( RecVersions *v)
{
uchar alph[256];
RecVersions c;
int i,j;

memcpy(&c,v,sizeof(RecVersions));
memset(alph,0,256);
for(j=i=0;i<v->lnAltCnt;i++)
	{
	if( !alph[c.Alt[i].Code] )
		{
		alph[ c.Alt[i].Code ]=1;
		memcpy(&v->Alt[j++],&c.Alt[i],sizeof(RecAlt));
		}
	}

v->lnAltCnt = j;

return;
}

void leo_decode_to_ansi( RecVersions *v)
{
int i;

for(i=0;i<v->lnAltCnt;i++)
    v->Alt[i].Code = stdAsciiToAnsi(v->Alt[i].Code);

return;
}

void leo_decode_to_acsii( RecVersions *v)
{
int i;

for(i=0;i<v->lnAltCnt;i++)
    v->Alt[i].Code = stdAnsiToAscii(v->Alt[i].Code);

return;
}


void LeoAddOneProb(int Probs[],int Code,int Prob)
{
int p;
p = Probs[Code];
Probs[Code] = p+Prob-p*Prob/255;
}

void LeoAddPVers(int Probs[],RecVersions *v)
{
int i;
RecVersions ver;
memcpy(&ver,v,sizeof(ver));
for(i=0;i<ver.lnAltCnt;i++)
	LeoAddOneProb(Probs,ver.Alt[i].Code,ver.Alt[i].Prob);
return;
}

void LeoAddToVer(int Probs[],RecVersions *v)
{
int i,ii;
for(ii=v->lnAltCnt,i=0;i<256;i++)
	if( Probs[i] )
		{
		v->Alt[ii].Code    = i;
        v->Alt[ii].CodeExt = 0;
		v->Alt[ii].Method  = REC_METHOD_FINAL;
		v->Alt[ii].Prob    = Probs[i];
		ii++;
		}
v->lnAltCnt = ii;
return;
}

void leoProbsToVer_expert(int Probs[],RecVersions *v,RecVersions *mod)
{
int i,ii;
for(ii=i=0;i<mod->lnAltCnt;i++)
	if( Probs[ mod->Alt[i].Code ] )
		{
		v->Alt[ii].Code    = mod->Alt[i].Code;
        v->Alt[ii].CodeExt = 0;
		v->Alt[ii].Method  = REC_METHOD_FINAL;
		v->Alt[ii].Prob    = Probs[ mod->Alt[i].Code ];
		ii++;
		}
v->lnAltCnt = ii;
v->lnAltMax = REC_MAX_VERS;
return;
}

void LeoProbsToVer(int Probs[],RecVersions *v)
{
int i,ii, mmin;

do  {
    for(mmin=255,ii=i=0;i<256;i++)
        {
        if( Probs[i] )
            {
            ii++;

            if( Probs[i]<mmin )
                mmin = Probs[i];
            }
        }

    if( ii>REC_MAX_VERS )
        {
        for(i=0;i<256;i++)
            if( Probs[i]==mmin )
                Probs[i]=0;
        }
    }
    while( ii>REC_MAX_VERS );


for(ii=i=0;i<256;i++)
	if( Probs[i] && ii<v->lnAltMax )
		{
		v->Alt[ii].Code    = i;
        v->Alt[ii].CodeExt = 0;
		v->Alt[ii].Method  = REC_METHOD_FINAL;
		v->Alt[ii].Prob    = Probs[i];
		ii++;
		}
v->lnAltCnt = ii;
v->lnAltMax = REC_MAX_VERS;
return;
}



void leo_kill(RecVersions *v,uchar *kill_list)
{
int i,l=strlen(kill_list);
if( !v->lnAltCnt )
	return ;
for(i=0;i<REC_MAX_VERS && i<v->lnAltCnt; i++)
	{
	if( memchr(kill_list,v->Alt[i].Code,l) )
		v->Alt[i].Prob = 1;
	}
return;
}

void VersToAlph(RecVersions *ver,uchar alphabet0[])
{
int i;

for(i=0;i<ver->lnAltCnt;i++)
	alphabet0[ver->Alt[i].Code ] =1;
return;
}


uchar leo_get_prob(RecVersions *v,uchar let)
{
int i;

for(i=0; i<v->lnAltCnt; i++)
    if( v->Alt[i].Code==let )
        return v->Alt[i].Prob;

return 0;
}

void leo_special_iva( RecVersions *v, RecRaster   *recR )
{
uchar alphabet0[256];
if( leo_alpha_type==ALPH_ROM )
    {
    if( v->lnAltCnt<2 || v->Alt[0].Code==v->Alt[1].Code )
	    return; // too few versions
    if( v->Alt[0].Prob-v->Alt[1].Prob < 5 )
	    {
	    memset(alphabet0,0,256);
	    VersToAlph(v,alphabet0);
        MSKRecogCharExp(leo_MSK_ndx[0],recR, v);
        leo_compress(v);
	    leo_sort_vers_prob( v );
	    }
    }
if( v->lnAltCnt<3 || v->Alt[0].Code==v->Alt[1].Code )
	return; // too few versions
if( v->Alt[0].Prob-v->Alt[2].Prob < 5 )
	{
	memset(alphabet0,0,256);
	VersToAlph(v,alphabet0);
    MSKRecogCharExp(leo_MSK_ndx[0],recR, v);
    leo_compress(v);
	leo_sort_vers_prob( v );
	}
return;
}


void leo_over( RecVersions *ver, RecVersions *alph)
{
int i,k;
RecVersions loc;
if( ver->lnAltCnt<1 )
    return;
memcpy(&loc,ver,sizeof(RecVersions));
for(k=i=0;i<loc.lnAltCnt;i++)
    if( leo_exist_code(alph,loc.Alt[i].Code)!=-1 )
        {
        memcpy(&ver->Alt[k++], &loc.Alt[i],sizeof(RecAlt));
        }
ver->lnAltCnt=k;
return;
}

int leo_expert_recog(uchar Code,uint16_t *v3x5,
    RecRaster *r , uchar prob_3x5, uchar prob_iva)
{

RecVersions loc;
loc.lnAltMax = REC_MAX_VERS;
loc.Alt[0].Code=Code;
loc.lnAltCnt=1;

if( !prob_3x5 )
    {
    R35RecogCharIm3x5_expert(v3x5, &loc);
    prob_3x5 = loc.Alt[0].Prob;
    }
return prob_3x5;

}

int leo_expert_recog_prn(uchar Code, RecRaster *raster,uint16_t *v3x5, uchar prob_3x5,
        Bool32 enable5x3)
{
RecVersions loc;
uchar       wide_letters[] = {  134,140,148,152,153,155,158,
                                166,172,228,232,233,235,238
                             };// ж   м   ф   ш   щ   ы   ю
uint16_t      v5x3[16];
loc.lnAltMax = REC_MAX_VERS;
loc.Alt[0].Code=Code;
loc.lnAltCnt=1;

if( enable5x3 && memchr(wide_letters,Code,sizeof(wide_letters)) )
    {
    R35Pack(  raster, v5x3, 5,3);
    R35RecogPrintCharIm3x5_expert(v5x3, &loc, TRUE); // 5x3
    prob_3x5 = loc.Alt[0].Prob;
    }
else if( !prob_3x5 )
    {
    R35RecogPrintCharIm3x5_expert(v3x5, &loc, FALSE); // 5x3
    prob_3x5 = loc.Alt[0].Prob;
    }

loc.Alt[0].Prob = prob_3x5;

if( loc.Alt[0].Prob<1 )
    loc.Alt[0].Prob=1;

return loc.Alt[0].Prob;
}

void leo_set_max_vers(RecVersions *v,int p)
{
int i, n, pold;
if( p<1 )
    p=1;
n = v->lnAltCnt;
pold = v->Alt[0].Prob;
v->Alt[0].Prob = p;
if( n>1 )
for(i=1;i<n;i++)
    {
    v->Alt[i].Prob = (v->Alt[i].Prob*p)/pold;
    if( v->Alt[i].Prob<1 )
        v->Alt[i].Prob=1;
    }
return;
}

void leo_expert_prob( RecVersions *v,uint16_t *v3x5,
    RecRaster *r , uchar prob_3x5[], uchar prob_iva[], int32_t prn,Bool32 bonus, Bool32 enable5x3)
{
int p, pold;

if( v->lnAltCnt<1 )
    return;

switch( prn )
   {

   case  1:
      pold =  v->Alt[0].Prob;
      p    = leo_expert_recog_prn(v->Alt[0].Code, r, v3x5,
                   prob_3x5[v->Alt[0].Code], enable5x3   );
      if( bonus && pold>p )
        p = (p+pold)/2;
      break;
   case  0:
      p    = leo_expert_recog(v->Alt[0].Code,v3x5, r,
                 prob_3x5[v->Alt[0].Code],
                 prob_iva[v->Alt[0].Code]);
      if( leo_alpha_type!=ALPH_ROM )
            {
            if( leo_alpha_type==ALPH_DIG )
                p    = (p*246)/255;
            else
                p    = (p*200)/255;
            }
      break;
   default:
       p = prn;
       break;
   }

leo_set_max_vers(v,p);
return;
}

void add_to_prob_array(uchar prob[],RecVersions *v)
{
int i,n;
n= v->lnAltCnt;

if( n<1 )
    return;

for(i=0;i<n;i++)
    prob[ v->Alt[i].Code ] = v->Alt[i].Prob;
return;
}

int leo_comp_codes(uchar c1, uchar c2)
{
return stdLeoCompareChar(c1, c2);
}

uchar * leo_strchr_codes(uchar *c1, uchar c2)
{
uchar   *c;
for(c=c1;*c;c++)
    if( leo_comp_codes( *c, c2) )
        return c;
return NULL;
}

uchar * leo_strchr_codes_ansi(uchar *c1, uchar c2)
{
uchar   *c;
for(c=c1;*c;c++)
    if( leo_comp_codes( stdAnsiToAscii(*c), c2) )
        return c;
return NULL;
}

static void leo_tab2ver(int Cnts[],RecVersions *ver)
{
int i,k;
for(k=i=0;i<256;i++)
    if( Cnts[i]>1 )
        {
        ver->Alt[k].Code = i;
        ver->Alt[k].CodeExt = 0;
        ver->Alt[k].Prob = 255;
        ver->Alt[k].Method = REC_METHOD_FINAL;
        k++;
        }
ver->lnAltCnt = k;
return;
}

void leo_evn_pass(  RecObject*  object,
                    uint16_t CompImage16x16[],
                    RecVersions *alphr)
{
int32_t   r;
RecVersions evn;
RecRaster   recR;

memset(&evn,0,sizeof(RecVersions));
evn.lnAltMax = REC_MAX_VERS;

// events

if( R35Pack(&object->recData.recRaster, CompImage16x16,16,16) )
	{
    R35Binarize(&recR, CompImage16x16,16,16);

	r=EVNRecogChar(&recR, &evn);
	}
else
	{
	r=EVNRecogChar(&object->recData.recRaster, &evn);
    }
object->lwStatusMethods |= REC_STATUS_METHODS_EVN;

*alphr = evn;

return;
}


int32_t leo_MSKRecogCharExpert(RecRaster*  rr, RecVersions  *ver, RecVersions  *iva)
{

if( ver->lnAltCnt )
    {
    *iva = *ver;
    MSKRecogCharExp(leo_MSK_ndx[0],rr, iva);
    leo_sort_vers_prob( iva );
    }
else
    MSKRecogChar(leo_MSK_ndx[0],rr, iva);

return iva->lnAltCnt;
}

void leo_add_aux(RecVersions *ver,RecVersions *sav)
{
int i = 0, j = 0, c = 0;
if( !ver->lnAltCnt || !sav->lnAltCnt )
    return;
for(i=0,j=ver->lnAltCnt;i<sav->lnAltCnt;i++)
    {
    c = leo_exist_code(ver, sav->Alt[i].Code);
    if( c==-1 )
        {
        ver->Alt[j] = sav->Alt[i];
        ver->Alt[j].Prob = 1;
        j++;
        }
    }
ver->lnAltCnt = j;
return;
}

Bool32 leo_kill_undefined_prob(RecVersions *per)
{
int i;
if( per->lnAltCnt<2 )
    return FALSE;
if( per->Alt[0].Prob!=per->Alt[1].Prob )
    return FALSE;
for(i=1;i<per->lnAltCnt;i++)
    if( per->Alt[i].Prob>1 )
        per->Alt[i].Prob --;
return TRUE;
}
#define REC_PER_LIMIT 200

Bool32 leo_reverse_perc(RecVersions *per,RecVersions *iva)
{
int i;

if( per->lnAltCnt && iva->lnAltCnt && per->Alt[0].Prob>REC_PER_LIMIT )
        {
        if( iva->lnAltCnt &&!leo_comp_codes( per->Alt[0].Code, iva->Alt[0].Code) )
            {
            for(i=1;i<per->lnAltCnt&&abs(per->Alt[0].Prob-per->Alt[i].Prob)<40;i++)
                {
                if( leo_comp_codes( per->Alt[i].Code, iva->Alt[0].Code) )
                        {
                        per->Alt[i].Code = per->Alt[0].Code;
                        per->Alt[0].Code = iva->Alt[0].Code;
                        return TRUE;
                        }
                 }
            }
         }
return FALSE;
}

void set_result(RecVersions *res,int ind, uchar ch[])
{
int    leo_tab3[]={2    , 3     , 3    , 3    , 3    , 3   , 3  };
uchar  leo_tab0[]={255  , 253   , 251  , 250  , 252  , 249 , 248};
uchar  leo_tab1[]={ 96  ,  96   , 197  , 159  , 112  , 224 , 175};
uchar  leo_tab2[]={  0  ,  80   , 133  , 144  ,  96  , 149 , 154};
int j;

res->lnAltCnt      = leo_tab3[ind];
res->Alt[0].Prob   = leo_tab0[ind];
res->Alt[1].Prob   = leo_tab1[ind];
res->Alt[2].Prob   = leo_tab2[ind];
for(j=0;j<res->lnAltCnt;j++)
    {
    res->Alt[j].Code   = ch[j+1];
    res->Alt[j].CodeExt= 0;
    res->Alt[j].Method = REC_METHOD_FINAL;
    }
return ;
}
int32_t leoMakePropability(RecVersions *per, RecVersions *sce, RecVersions *iva,
RecVersions *res  )
{
uchar per_ch1, per_pr1, per_ch2=0, per_pr2=0; // perceptron
uchar sce_ch1, sce_pr1, sce_ch2=0, sce_pr2=0; // sceleton
uchar iva_ch1, iva_pr1, iva_ch2=0, iva_pr2=0; // ivanisov
uchar ch[4];

res->lnAltCnt = 0;
if( !per->lnAltCnt || !sce->lnAltCnt || !iva->lnAltCnt )
	return 0;
ch[1] = ch[2] = ch[3] = 0;
per_ch1 = per->Alt[0].Code; per_pr1 = per->Alt[0].Prob;
sce_ch1 = sce->Alt[0].Code; sce_pr1 = sce->Alt[0].Prob;
iva_ch1 = iva->Alt[0].Code; iva_pr1 = iva->Alt[0].Prob;

if( per->lnAltCnt>1 )
	{
	per_ch2 = per->Alt[1].Code;
    per_pr2 = per->Alt[1].Prob;
	}
if( sce->lnAltCnt>1 )
	{
	sce_ch2 = sce->Alt[1].Code;
    sce_pr2 = sce->Alt[1].Prob;
	}
if( iva->lnAltCnt>1 )
	{
	iva_ch2 = iva->Alt[1].Code;
    iva_pr2 = iva->Alt[1].Prob;
	}

if( !leo_comp_codes(per_ch1,sce_ch1) && !leo_comp_codes(per_ch1,iva_ch1) )
    {
    if( !(per_pr1==per_pr2 && (leo_comp_codes(per_ch2,sce_ch1) || leo_comp_codes(per_ch2,iva_ch1))) )
	    return 0;
    }

if( leo_comp_codes(per_ch1,sce_ch1) && leo_comp_codes(per_ch1,iva_ch1) )
	{ // over 3
	ch[1] = per_ch1;
	ch[2] = '~';
    set_result(res, 0, ch);
	return res->lnAltCnt;
	}
if( per_pr1==per_pr2 && leo_comp_codes(per_ch2,sce_ch1) && leo_comp_codes(per_ch2,iva_ch1) )
	{ // over 3
	ch[1] = per_ch2;
	ch[2] = '~';
    set_result(res, 0, ch);
	return res->lnAltCnt;
	}
if( per_pr1<200/*150*/ && !(leo_comp_codes(per_ch2,sce_ch1) && sce_pr1>254)
 )
    return 0;
if( leo_comp_codes(per_ch1,sce_ch1) && !leo_comp_codes(per_ch1,iva_ch1) )
	{
	if( per_pr1==255 && !leo_comp_codes(per_ch2,iva_ch1) &&
		!leo_comp_codes(sce_ch2,iva_ch1) )
		{
		ch[1] = per_ch1;
		ch[2] = iva_ch1;
		ch[3] = '~';
        set_result(res, 1, ch);
		return res->lnAltCnt;
		}

	if( per_pr1!=255 && (leo_comp_codes(per_ch2,iva_ch1) ||
		leo_comp_codes(sce_ch2,iva_ch1)) )
		{
		ch[1] = per_ch1;
		ch[2] = iva_ch1;
		ch[3] = '~';
        set_result(res, 2, ch);
		return res->lnAltCnt;
		}

	ch[1] = per_ch1;
	ch[2] = iva_ch1;
	ch[3] = '~';
	res->Alt[0].Code=ch[1];
  	res->Alt[1].Code=ch[2];  // 65398
	res->Alt[2].Code=ch[3];
    set_result(res, 3, ch);
	return res->lnAltCnt;
	}

if( leo_comp_codes(per_ch1,iva_ch1) && !leo_comp_codes(per_ch1,sce_ch1) )
	{
	if( per_pr1==255 && !leo_comp_codes(per_ch2,sce_ch1) &&
		!leo_comp_codes(iva_ch2,sce_ch1) )
		{
		ch[1] = per_ch1;
		ch[2] = sce_ch1;
		ch[3] = '~';
        set_result(res, 4, ch);
		return res->lnAltCnt;
		}

	if( per_pr1!=255 && (leo_comp_codes(per_ch2,sce_ch1) ||
		leo_comp_codes(iva_ch2,sce_ch1)) )
		{
		ch[1] = per_ch1;
		ch[2] = sce_ch1;
		ch[3] = '~';
        set_result(res, 5, ch);
		return res->lnAltCnt;
		}

	ch[1] = per_ch1;
	ch[2] = sce_ch1;
	ch[3] = '~';
	res->Alt[0].Code=ch[1];
	res->Alt[1].Code=ch[2];   // 65263
	res->Alt[2].Code=ch[3];
    set_result(res, 6, ch);
	return res->lnAltCnt;
	}

res->lnAltCnt = 0;
return res->lnAltCnt;
}



Bool32 leoRecogCharRom( RecObject*  object ,  int32_t erect)
{
RecVersions ver, loc, alph, iva, sver, tmp, sce, per;
int32_t		num_horiz_dist, hei;
uint16_t      CompImage16x16[16*16];
//uchar		feat[256];
uchar       prob_3x5[256]={0}, prob_iva[256]={0};
Bool32      bonus=FALSE;

memset(&ver,0,sizeof(RecVersions));
ver.lnAltMax = REC_MAX_VERS;

memset(&sce,0,sizeof(RecVersions));
sce.lnAltMax = REC_MAX_VERS;

memset(&sver,0,sizeof(RecVersions));
sver.lnAltMax = REC_MAX_VERS;

memset(&loc,0,sizeof(RecVersions));
loc.lnAltMax = REC_MAX_VERS;

memset(&tmp,0,sizeof(RecVersions));
tmp.lnAltMax = REC_MAX_VERS;

memset(&alph,0,sizeof(RecVersions));
alph.lnAltMax = REC_MAX_VERS;

memset(&per,0,sizeof(RecVersions));
per.lnAltMax = REC_MAX_VERS;

memset(&iva,0,sizeof(RecVersions));
iva.lnAltMax = REC_MAX_VERS;
hei             = object->recData.recRaster.lnPixHeight;
num_horiz_dist  = leo_stick_horiz_hist(hei);

// ROMA DIGITs recognizer
if( object->recData.lwCompCnt<5)
    {
    // GENERATORS PASS
    leo_evn_pass(  object, CompImage16x16, &alph);
    ver = alph;
    leo_snapChar(&ver,"LEO ROMA Evn : ",0);
    leo_MSKRecogCharExpert(&object->recData.recRaster, &alph, &iva);
    add_to_prob_array(prob_iva,&iva);
    }

if( !(object->recData.lwStatus & REC_STATUS_V3X5) )
	{
	R35Pack(&object->recData.recRaster, object->recData.v3x5,3,5);
	object->recData.lwStatus |= REC_STATUS_V3X5;
	}



if( !ver.lnAltCnt  )

  { // auxilary generator NET3x5
  ver.lnAltCnt=0;
  R35RecogCharIm3x5(object->recData.v3x5, &loc);
  add_to_prob_array(prob_3x5,&loc);
  if( leo_add_vers(&ver,&loc) )
	object->lwStatusMethods |= REC_STATUS_METHODS_3X5;

  MSKRecogChar(leo_MSK_ndx[0],&object->recData.recRaster, &loc);

  add_to_prob_array(prob_iva,&loc);
  if( leo_add_vers(&ver,&loc) )
    object->lwStatusMethods |= REC_STATUS_METHODS_MSK;

  if( ver.lnAltCnt && alph.lnAltCnt )
    leo_over( &ver, &alph);  // over generators
  if( leo_add_vers(&ver,&per) )
		object->lwStatusMethods |= REC_STATUS_METHODS_NCU;
  if( loc.lnAltCnt && per.lnAltCnt &&
        per.Alt[0].Prob==255 && loc.Alt[0].Prob>190 &&
        leo_comp_codes( per.Alt[0].Code, loc.Alt[0].Code) )
        bonus = TRUE;

  leo_sort_vers_prob( &ver );
  leo_snapChar(&ver,"LEO ROMA GEN2 Net3x5+3x5+MSK : ",0);
  }
else
  { // EXPERT PASS
  loc = ver;
  tmp = ver;
  R35RecogCharIm3x5_expert(object->recData.v3x5, &tmp);
  add_to_prob_array(prob_3x5,&tmp);
  if( leo_add_vers(&ver,&tmp) )
    object->lwStatusMethods |= REC_STATUS_METHODS_3X5;

   add_to_prob_array(prob_iva,&iva);
   if( leo_add_vers(&ver,&iva) )
	object->lwStatusMethods |= REC_STATUS_METHODS_MSK;

   if( leo_add_vers(&ver,&per) )
		 object->lwStatusMethods |= REC_STATUS_METHODS_NCU;
   if( iva.lnAltCnt && per.lnAltCnt &&
            per.Alt[0].Prob==255 && iva.Alt[0].Prob>190 &&
            leo_comp_codes( per.Alt[0].Code, iva.Alt[0].Code) )
            bonus = TRUE;

  leo_sort_vers_prob( &ver );
  leo_snapChar(&ver,"LEO ROMA EXPERT Net3x5+3x5+MSK : ",0);
  }


if( !ver.lnAltCnt ||
    ver.lnAltCnt && ver.Alt[0].Prob<100 && prob_3x5[ver.Alt[0].Code]<60 ||
    ver.lnAltCnt && ver.Alt[0].Code=='I' && ver.Alt[0].Prob<200 && prob_3x5['I']<100 &&
        num_horiz_dist*100> hei*35 )
    {
    tmp.lnAltCnt=0;
    R35RecogCharIm3x5(object->recData.v3x5, &loc);
    add_to_prob_array(prob_3x5,&loc);
    if( leo_add_vers(&tmp,&loc) )
	    object->lwStatusMethods |= REC_STATUS_METHODS_3X5;

    MSKRecogChar(leo_MSK_ndx[0],&object->recData.recRaster, &loc);

    add_to_prob_array(prob_iva,&loc);
    if( leo_add_vers(&tmp,&loc) )
    object->lwStatusMethods |= REC_STATUS_METHODS_MSK;
    if( tmp.lnAltCnt && tmp.Alt[0].Prob>100 &&
        !(tmp.lnAltCnt==1 && tmp.Alt[0].Code=='I' && num_horiz_dist*2>hei  ) )
        {
        ver = tmp;
        leo_snapChar(&ver,"LEO ROMA GEN3 Net3x5+3x5+MSK : ",0);
        }
    }


leo_sort_vers_prob( &ver );
// Post-correction
leo_compress( &ver );
//leo_special_iva( &ver, &object->recData.recRaster );
if( ver.lnAltCnt==1 && ver.Alt[0].Code=='I' && ver.Alt[0].Prob<200 &&
    num_horiz_dist*100> hei*35 )
    {
    tmp.lnAltCnt=0;
    R35RecogCharIm3x5(object->recData.v3x5, &loc);
    add_to_prob_array(prob_3x5,&loc);
    if( leo_add_vers(&tmp,&loc) )
	    object->lwStatusMethods |= REC_STATUS_METHODS_3X5;

    MSKRecogChar(leo_MSK_ndx[0],&object->recData.recRaster, &loc);

    add_to_prob_array(prob_iva,&loc);
    if( leo_add_vers(&tmp,&loc) )
    object->lwStatusMethods |= REC_STATUS_METHODS_MSK;
    if( tmp.lnAltCnt && tmp.Alt[0].Prob>100 )
        {
        ver = tmp;
        leo_snapChar(&ver,"LEO ROMA GEN4 Net3x5+3x5+MSK : ",0);
        }
    }
leo_snapChar(&ver,"LEO ROMA RECOG BEFORE RERECOG3x5 : ",0);
if( !(object->recData.lwStatus & REC_STATUS_V3X5) )
	{
	R35Pack(&object->recData.recRaster, object->recData.v3x5,3,5);
	object->recData.lwStatus |= REC_STATUS_V3X5;
	}
if( !bonus )
    leo_expert_prob( &ver, object->recData.v3x5,
                 &object->recData.recRaster,
                 prob_3x5, prob_iva, 0 , FALSE, FALSE);

//XOPOIII:;
if( ver.lnAltCnt>1 && ver.Alt[0].Code=='I' &&
    (ver.Alt[0].Prob<128 && num_horiz_dist*100> hei*35||
     ver.Alt[0].Prob<220 && num_horiz_dist*100> hei*50) )
    {
    ver.Alt[0].Code=ver.Alt[1].Code;
    ver.Alt[1].Code='I';
    leo_snapChar(&ver,"LEO ROMA MONUS I : ",0);
    }
if( ver.lnAltCnt && ver.Alt[0].Code=='I' &&
    ver.Alt[0].Prob<128 && num_horiz_dist*100> hei*35 )
    {
    tmp.Alt[0].Code ='X';
    tmp.Alt[1].Code ='V';
    tmp.lnAltCnt    =2;
    R35RecogCharIm3x5_expert(object->recData.v3x5, &tmp);
    leo_sort_vers_prob( &tmp );
    if( num_horiz_dist*100> hei*50 || tmp.Alt[0].Prob>ver.Alt[0].Prob )
        ver = tmp;
    }

if( ver.lnAltCnt>1 && memchr("XV",ver.Alt[0].Code,2) )
    {
    if( ver.Alt[0].Prob<128 && num_horiz_dist==0 )
        {
        ver.Alt[1].Code=ver.Alt[0].Code;
        ver.Alt[0].Code='I';
        leo_snapChar(&ver,"LEO ROMA BONUS I : ",0);
        }
    }

memcpy(&object->recResults,&ver,sizeof(RecVersions));
return TRUE;
}


