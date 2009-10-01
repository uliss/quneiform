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
#include <assert.h>

#include "leo_tune.h"

#define PC_TYPE 0

#include "leo.h"
#include "r35.h"
#include "msk.h"

#include "snptools.h"

static Bool32 leo_ndx_in_rect;
extern int  LEO_error_code;
extern int nNdxWid , nNdxHei;
extern Bool32  no_init_ndx;
extern int32_t try_call_vec;

static SnpTreeNode  stnNdxRecog;       // stops after ndx recog
static SnpTreeNode  stnSnapNdxRecog;
// functions from module MYSORT.C
extern  void stdQsort(void *pMas,int Num, int width,
        int (*compare)(const void *elm1,const void *elm2) );
extern  Bool32  leo_init_ndx(void);

static void Ind_SnpWaitUserInput(SnpTreeNode *stnCharRecog)
{
int sstatus=stnCharRecog->Status;
stnCharRecog->Status|= (STN_STOP | STN_DRAW);
SnpWaitUserInput(stnCharRecog);
if( (stnCharRecog->Status&(STN_STOP | STN_DRAW)) )
stnCharRecog->Status=sstatus;
    return;
}

static int32_t cmp_ndx_prob( const void *a, const void *b)
{
return (int32_t)(((RecAlt *)b)->Prob) - (int32_t)(((RecAlt *)a)->Prob) ;
}
static int leo_ndx_sort_vers_prob(RecVersions *v)
{

stdQsort(v->Alt,v->lnAltCnt,sizeof(RecAlt),cmp_ndx_prob);

return v->lnAltCnt;
}

static Bool32 leo_ndx_big_object(RecObject *object,int wlim, int hlim)
{
return (object->recData.recRaster.lnPixWidth>wlim ||
   object->recData.recRaster.lnPixHeight>hlim );
}

static Bool32 leo_ndx_small_object(RecObject *object,int wlim, int hlim)
{
return (object->recData.recRaster.lnPixWidth<wlim ||
   object->recData.recRaster.lnPixHeight<hlim );
}


static void leo_ndx_AddDigits(int Cnts[],RecVersions *v, int min_prob)
{
int i;
uchar start_let='0';

for(i=0;i<v->lnAltCnt;i++)
	{
	if( isdigit(v->Alt[i].Code) )
	if( v->Alt[i].Prob>min_prob )
		Cnts[ v->Alt[i].Code-start_let ]++;
	}
return;
}

static Bool32 leo_ndx_ReadyDigits(int Cnts[])
{
int i, ii;
for(ii=i=0;i<10;i++)
	{
	switch( Cnts[i] )
		{
		case 4:
		case 5:
		case 3:
			return TRUE;
		default:
			break;
		}

	}
return  FALSE;
}

static int leo_ndx_DigHstToVers(int Cnts[],RecVersions *v)
{
int i, ii;
uchar start_let='0';

for(ii=i=0;i<10;i++)
	{
	switch( Cnts[i] )
		{
		case 4:
		case 5:
			v->Alt[ii].Code   = start_let+i;
			v->Alt[ii].Method = REC_METHOD_FINAL;
			v->Alt[ii].Prob   = 255;
			ii++;
			break;
		case 3:
			v->Alt[ii].Code   = start_let+i;
			v->Alt[ii].Method = REC_METHOD_FINAL;
			v->Alt[ii].Prob   = MIN_GOOD_PROB;
			ii++;
			break;
		case 2:
			v->Alt[ii].Code   = start_let+i;
			v->Alt[ii].Method = REC_METHOD_FINAL;
			v->Alt[ii].Prob   = MIN_PROB;
			ii++;
			break;
		default:
			if( Cnts[i]>5 )
				{
				v->Alt[ii].Code   = start_let+i;
				v->Alt[ii].Method = REC_METHOD_FINAL;
				v->Alt[ii].Prob   = 255;
				ii++;
				}
			break;
		}

	}
v->lnAltCnt = ii;
return  v->lnAltCnt;
}

static void leo_ndx_monus_prob( RecVersions *ver )
{
int i;

for(i=0;i<ver->lnAltCnt;i++)
	{
	ver->Alt[i].Prob>>=1;
	}

return;
}

static uchar leo_ndx_max_prob(RecVersions *v)
{
int i;
uchar prob;

for(prob=i=0;i<v->lnAltCnt&&i<REC_MAX_VERS;i++)
	{
    if( prob<v->Alt[i].Prob )
		prob = v->Alt[i].Prob;
	}

return prob;
}

static void leo_ndx_snapSimpleKey(char *str, SnpTreeNode *stnRecog)
{
SnpLog("%s",str);
SnpLog("");
Ind_SnpWaitUserInput(stnRecog); // pass control to user
SnpHideRects((uint32_t)stnRecog);
return;
}

static void leo_ndx_snapRaster(RecObject* object, SnpTreeNode *stnRecog)
{

SnpDrawRect(&object->recData.rect,
     0, //Skew - zero to real coords
     wRGB(0,0,255),
     -16, // one image pixel width
     (uint32_t)stnRecog    );

SnpDrawRaster( &object->recData.recRaster );
return;
}

static void leo_snapNdx(RecVersions *loc,char *tit,int enable)
{
char buf[256], *t;
int i;

if (!SnpSkip(&stnNdxRecog)||enable|| leo_ndx_in_rect)
		{
		t = buf;
		if( loc->lnAltCnt )
			{
			for(i=0;i<loc->lnAltCnt-1;i++)
				t += sprintf(t,"%c(%d),",
				loc->Alt[i].Code,loc->Alt[i].Prob	);
			t += sprintf(t,"%c(%d)",
					loc->Alt[i].Code,loc->Alt[i].Prob	);
			}
		else
			strcpy(buf,"-");
		SnpLog("%s %s", tit, buf);
		}


return;
}

static Bool leo_ndx_InsideRect( const Rect16* r, const Point16 * p)
{ return !( (p->x < r->left)||(p->x > r->right )||
            (p->y < r->top) ||(p->y > r->bottom)
          );
}

static Bool32 leo_ndx_SnpInRect( Rect16* pRect/*, int32_t nSkew*/ )
{
   Point16 pt;
   if (!SnpIsActive())
      return FALSE;
   if (!SnpGetUserPoint( &pt))
      return FALSE;
   return leo_ndx_InsideRect(pRect, &pt);
}


LEO_FUNC( void   )      LEOIndRegisterSnpTree(
         SnpTreeNode* parent,         // parent Snp Node, may be NULL
         __SnpToolBox* p_snp_tools     // tools complect, may be NULL
                              )
{
   SnpSetTools( p_snp_tools );    // may be NULL, it's OK
   SnpAddNode( &stnNdxRecog,
      "Show Leo NDX Recognize results",  parent);
   SnpAddNode( &stnSnapNdxRecog,
      "Show Leo NDX Extended Recognize results",  &stnNdxRecog);
};


LEO_FUNC(Bool32)  LEORecogInd( RecObject*  object )
{
RecVersions ver, loc;

int  Cnts[10];
char buf[256], *t;
int  i, wlim, hlim;
Bool32 no_vit, bad_35 ;
uchar  let_vit;

if( no_init_ndx )
    {
    if( !leo_init_ndx() )
        return FALSE;
    no_init_ndx=FALSE;
    }

leo_ndx_in_rect=leo_ndx_SnpInRect(&object->recData.rect/*, 0*/);
no_vit  = bad_35= TRUE;
let_vit = 0;
LEO_error_code=ER_LEO_NO_ERROR;
if( !object )
	{
	LEO_error_code=ER_LEO_NULL_OBJECT;
	return FALSE;
	}
wlim = (nNdxWid)?nNdxWid/2:15;
hlim = (nNdxHei)?nNdxHei/2:15;
if( leo_ndx_small_object(object,wlim,hlim) )
	{
	LEO_error_code=ER_LEO_SMALL_OBJECT;
	if (!SnpSkip(&stnNdxRecog)|| leo_ndx_in_rect)



	{
            leo_ndx_snapRaster(object, &stnNdxRecog);
            leo_ndx_snapSimpleKey("TOO SMALL RASTER", &stnNdxRecog);
			}
	return FALSE;
	}
if( leo_ndx_big_object(object,150,150) )
	{
	LEO_error_code=ER_LEO_LARGE_OBJECT;
	if (!SnpSkip(&stnNdxRecog)|| leo_ndx_in_rect)
			{
            leo_ndx_snapRaster(object, &stnNdxRecog);
            leo_ndx_snapSimpleKey("TOO BIG RASTER", &stnNdxRecog);
			}
	return FALSE;
	}
//
object->recResults.lnAltMax=REC_MAX_VERS;
//
if(1)
if( leo_ndx_in_rect )
    {
    leo_ndx_snapRaster(object, &stnNdxRecog);
    leo_ndx_snapSimpleKey("Before recog NDX", &stnNdxRecog);
    }
memset(Cnts,0,sizeof(Cnts));
leo_snapNdx(&object->recResults,"LEO VIT : ",0);

if(object->recResults.lnAltCnt>0)
   {
   no_vit = FALSE;
   let_vit = object->recResults.Alt[0].Code;
   }

if( object->recResults.Alt[0].Prob==255 )
	{
	if (!SnpSkip(&stnNdxRecog)|| leo_ndx_in_rect)
			{
            leo_ndx_snapRaster(object, &stnNdxRecog);
			SnpLog("%s","MAX VITA");
			SnpLog("");
			Ind_SnpWaitUserInput(&stnNdxRecog); // pass control to user
			SnpHideRects((uint32_t)&stnNdxRecog);
			}
	return TRUE;
	}
else if( leo_ndx_max_prob(&object->recResults)>=MIN_REC_VIT )
	{
	memcpy(&loc,&object->recResults,sizeof(RecVersions) );
	R35Pack(&object->recData.recRaster, object->recData.v3x5,3,5);
	R35RecogNdxIm3x5_expert(object->recData.v3x5, &loc);
	leo_snapNdx(&loc,"LEO EXPERT 3x5 : ",0);
	if( leo_ndx_max_prob(&loc)>MIN_REC_VIT1 )
		{
		ver = loc;
		leo_ndx_sort_vers_prob( &ver );

		memcpy(&object->recResults,&ver,sizeof(RecVersions));
		leo_snapNdx(&loc,"LEO RECOG NDX : ",0);
		if (!SnpSkip(&stnNdxRecog)|| leo_ndx_in_rect)
			{
            leo_ndx_snapRaster(object, &stnNdxRecog);
			SnpLog("%s","");
			Ind_SnpWaitUserInput(&stnNdxRecog); // pass control to user
			SnpHideRects((uint32_t)&stnNdxRecog);
			}

		return TRUE;
		}
	}

if( leo_ndx_max_prob(&object->recResults)>MIN_REC_PROB )
	{
	leo_ndx_AddDigits(Cnts,&object->recResults,MIN_DIG_PROB);
	}

memset(&ver,0,sizeof(RecVersions));
 ver.lnAltMax = REC_MAX_VERS;

memset(&loc,0,sizeof(RecVersions));
loc.lnAltMax = REC_MAX_VERS;

R35Pack(&object->recData.recRaster, object->recData.v3x5,3,5);

if( leo_ndx_max_prob(&loc)>MIN_REC_PROB )
	{
	leo_ndx_AddDigits(Cnts,&loc,MIN_DIG_PROB);
	leo_snapNdx(&loc,"LEO NET3x5 : ",0);
	}

R35RecogNdxIm3x5(object->recData.v3x5, &loc);

if( leo_ndx_max_prob(&loc)>MIN_REC_PROB )
	{
	leo_ndx_AddDigits(Cnts,&loc,MIN_DIG_PROB);
	leo_snapNdx(&loc,"LEO 3x5 : ",0);
	}
if( loc.lnAltCnt>0 && loc.Alt[0].Prob>MID_LTR_PROB )
    bad_35=FALSE;

MSKRecogNDX(&object->recData.recRaster, &loc);
leo_ndx_AddDigits(Cnts,&loc,MIN_DIG_PROB);
leo_snapNdx(&loc,"LEO NDX MSK : ",0);

if( !leo_ndx_ReadyDigits(Cnts) )
	{
	assert(try_call_vec);
	}

leo_ndx_DigHstToVers(Cnts,&ver);
if( no_vit && bad_35 ||
    ver.lnAltCnt && let_vit &&let_vit!=ver.Alt[0].Code && bad_35
    )
    {
    leo_ndx_monus_prob( &ver );
    leo_snapNdx(&ver,"LEO DESCR : ",0);
    }

leo_ndx_sort_vers_prob( &ver );

memcpy(&object->recResults,&ver,sizeof(RecVersions));
if (!SnpSkip(&stnNdxRecog)|| leo_ndx_in_rect)
      {
      t = buf;
	  if( ver.lnAltCnt )
		{
		for(i=0;i<ver.lnAltCnt-1;i++)
			t += sprintf(t,"%c(%d),",
				ver.Alt[i].Code,ver.Alt[i].Prob);
		t += sprintf(t,"%c(%d) ",
				ver.Alt[i].Code,ver.Alt[i].Prob);
		}
	  else
		{
		t += sprintf(t,"Nonrecog");
		}

      leo_ndx_snapRaster(object, &stnNdxRecog);
      SnpLog("LEO RECOG NDX: %s", buf);
      SnpLog("%s","");
      Ind_SnpWaitUserInput(&stnNdxRecog); // pass control to user
	  if( !SnpSkip(&stnSnapNdxRecog) )
				{
				t = buf;
				for(i=0;i<10;i++)
					t += sprintf(t,"%c(%d),",i+'0',Cnts[i]);
				SnpLog(buf);
				Ind_SnpWaitUserInput(&stnSnapNdxRecog); // pass control to user
				stnSnapNdxRecog.Status &= ~(STN_DRAW|STN_STOP);
				}
	  SnpHideRects((uint32_t)&stnNdxRecog);
      }
leo_ndx_in_rect=FALSE;
return TRUE;
}
////////////////////////////////////////////
