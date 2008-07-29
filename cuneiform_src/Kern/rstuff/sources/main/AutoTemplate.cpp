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

/*Andrey: whole file moved to RNORM
//-----------------------
#include <memory.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "wRGB.h"
#include "dpuma.h"
#include "CCom.h"
#include "CPage.h"
#include "RNeg.h"
#include "RStuff.H"
#include "evn.h"
#include "ctiimage.h"
#include "Exc.h"
#include "pumadef.h"
#include "AutoTemplate.h"

extern Handle hDebugAutoTemplate;
extern Handle hAutoTemplateBC;
extern Handle hAutoTemplateBCShow;
extern Handle hAutoTemplateMar;
extern Handle hAutoTemplateWrite;

Handle timer;
Handle timerAll;

#define INVARY20(x,y)	(5*(x)>=4*(y) && 4*(x)<=5*(y))	// +-20%


//FUN_IMPO(Bool32) PUMA_XSetTemplate(Rect32 rect);

/*
typedef struct tagCompEx
{
	CCOM_comp * comp;
	Int16		dens;  //плотность
} CompEx;
*-/

static void RecogComp(CCOM_comp *comp)
{
/*
	RecVersions  *vers=comp->vers;
	Word8  lpool[LPOOL_SIZE];
	Int16* linerep=(Int16*)(comp->linerep);
	memcpy (lpool,linerep+1,*linerep);
	Int32 n,i;
	Word8 res[20];
	vers->lnAltCnt = EVNRecog_lp(c->env,lpool,lpool_lth,&res[0]);
 if( n )
     {
     for(i=0;i<n;i++)
            {
            c->vers[i].let=res[i];
            c->vers[i].prob=254;
            }
       c->vers[n].let=0;
       c->flg = c_f_let;
       c->recsource |= c_rs_ev;
       c->history   |= c_rs_ev;
     }
  else
    {
    c->vers[0].let=176;
    c->vers[1].let=0;
    c->flg = c_f_bad;
    }
  c->nvers=n;
*-/
}

static Word8 ExistVers (CCOM_comp * comp)
{
	RecVersions  *vers=comp->vers;
	if (!vers)  RecogComp(comp);
	if (!vers)
		return 0;

	static const char sticks[] = {"/!|1fijIJlLt){}[]r"};
	Int32 notSticks=0,maxNS=(vers->lnAltCnt+2)/4;
	RecAlt *alt=vers->Alt;
	for (int i=0; i<vers->lnAltCnt; i++,alt++)
		if (!strchr(sticks,alt->Code))
			if (++notSticks >= maxNS)
				return alt->Code;

	return 	0;
}

class Interval
{
public:
	Int32 b;
	Int32 e;
	Interval() {}
	Interval(Int32 beg, Int32 end)	{ b=beg; e=end;	}
	Interval operator *(Int32 fact)
	{
		Interval ival=*this;
		ival.b *= fact; ival.e *= fact; return ival;
	}
	Bool32 Include(Int32 point)
	{
		return b<=point && point<=e;
	}
};

class CompInfo
{
#define	MAXBIGLINES	3

public:
	static Word32 lineColors[MAXBIGLINES];
	CCOM_comp *comp;			//сам компонент
	Int32 dens;					//общая плотность
	Int32 hdens;				//плотность по левому и правому	контуру
	Int32 vdens;				//плотность	по верхнему и нижнему контуру
	Int32 w,h;					//размеры растра после масштабирования
	Int32 ww,wh;				//средняя ширина и высота
	SmRect32 rcBay;				//полость с краю
	Int8	side;				//с какого боку находится
#define		SIDE_LEFT	1
#define		SIDE_UP		2
#define		SIDE_RIGHT	4
#define		SIDE_DOWN	8
	Interval habris[MAXHEIGHT];	//контур по левому и правому краю
	Interval vabris[MAXWIDTH];  //контур по верхнему и нижнему краю
	LineWeight	bigLinesWeights[MAXBIGLINES+1];


//	CompInfo() {};
	CompInfo(CCOM_comp *comp);

};

CompInfo::CompInfo(CCOM_comp *pcomp)
{
	Int16 fact = 1<<(pcomp->scale&7);
	comp=pcomp;
	hdens=vdens=0;
	ww=pcomp->w; wh=pcomp->h;
	w=ww/fact; h=wh/fact;
	rcBay=SmRect32(pcomp->left,pcomp->upper,pcomp->left,pcomp->upper);
	side=0;
	memset(bigLinesWeights,0,sizeof(bigLinesWeights));
}

Word32 CompInfo::lineColors[MAXBIGLINES]={wRGB(0,0,255),wRGB(0,255,0),wRGB(255,0,255)};


typedef struct tagSizeGroup
{
	Int32 nComp;	//число компонент в группе
	Point32 cSize;	// (w,h) - размеры компонент в группе
	SmRect32 rRect;	//охватывающий прямоугольник
}	SizeGroup;


class GarbageTestBag
{
	Int32 nGenArea2;		//общая площадь/2

	Int32 nGarbComp;	//число нераспознанных компонент
	Int32 nGarbArea;	//их площадь

#define MAXSIZENUMBER	5	//максимальное число различных размеров символов
	Int32 nLetComp;		//число распознанных компонент
	Int32 nLetArea;		//их площадь
	Int32 nSizeGroup;	//число групп компонент сходного размер
	Int32 nLargestGroup;//номер наибольшей
	SizeGroup sizeGroups[MAXSIZENUMBER];	//сами группы

	void AddToGroup(Int32 nGroup, CCOM_comp *comp);
	void NewGroup(CCOM_comp *comp);
	Int32 FindGroup(CCOM_comp *comp);

public:
	void Snap();
	GarbageTestBag(SmRect32& rect) { Init(rect); };
	void Init(SmRect32& rect);
	void Add(CCOM_comp *comp);
	Int32 IsGarbage();			//1 - мусор, -1 - нет, 0 - неизвестно
	Int32 SureGarbage();		// - то же, но более жесткие условия для досрочного окончания
	Int32 AddAndTest(CCOM_comp *comp=NULL);
};

void GarbageTestBag::Snap()
{
	char *buf[80],*mes=(char *)buf;
	sprintf(mes,"S0=%d символы=(%d,%d) мусор=(%d,%d)\n",nGenArea2,nLetComp,nLetArea,nGarbComp,nGarbArea);
	LDPUMA_Console(mes);
	SizeGroup *pGr=sizeGroups;
	for (Int32 i=0; i<nSizeGroup; i++,pGr++)
	{
		sprintf(mes,"n=%d size=%dx%d\n",pGr->nComp,pGr->cSize.x,pGr->cSize.y);
		LDPUMA_Console(mes);
 	}
}

void GarbageTestBag::Init(SmRect32& rect)
{
	nGenArea2 = rect.Area()/2;
	nGarbComp=nGarbArea=nLetComp=nLetArea=nSizeGroup=nLargestGroup=0;
	memset(sizeGroups,0,sizeof(sizeGroups));
}

Bool32 GarbageTestBag::AddAndTest(CCOM_comp *comp)
{
	if(comp)
	{
		Add(comp);
		return 	SureGarbage();
	}
	else
		return IsGarbage();
}

Int32 GarbageTestBag::FindGroup(CCOM_comp *comp)
{
	SizeGroup *pGr=sizeGroups;
	for (Int32 i=0; i<nSizeGroup; i++,pGr++)
		if (INVARY20(comp->w,pGr->cSize.x) && INVARY20(comp->h,pGr->cSize.y))
			return i;

	return -1;
}

void GarbageTestBag::AddToGroup(Int32 nGroup, CCOM_comp *comp)
{
	SmRect32 rect(comp->left,comp->upper,comp->left+comp->w,comp->upper+comp->h);
	SizeGroup *pGr=sizeGroups+nGroup;
	pGr->cSize.x = pGr->cSize.x*pGr->nComp + comp->w;
	pGr->cSize.y = pGr->cSize.y*pGr->nComp + comp->h;
	pGr->rRect = pGr->rRect | rect;
	pGr->nComp++;
	pGr->cSize.x = DIV_ROUND(pGr->cSize.x,pGr->nComp);
	pGr->cSize.y = DIV_ROUND(pGr->cSize.y,pGr->nComp);

	if (pGr->nComp > sizeGroups[nLargestGroup].nComp)
		nLargestGroup=nGroup;
}

void GarbageTestBag::NewGroup(CCOM_comp *comp)
{
	Point32 size={comp->w,comp->h};
	SmRect32 rect(comp->left,comp->upper,comp->left+comp->w,comp->upper+comp->h);
	SizeGroup *pGr;
	Int32 nMin=nSizeGroup;
	if (nSizeGroup>=MAXSIZENUMBER)	 //все заняты - заменяем меньшую
	{
		pGr=sizeGroups;	 nMin=0;
		Int32 sMin=pGr->nComp;
		for (Int32 i=1; i<nSizeGroup; i++,pGr++)
			if (pGr->nComp<sMin)	{ nMin=i, sMin=pGr->nComp; }
	}
	else
		nSizeGroup++;
	pGr=sizeGroups+nMin;
	pGr->nComp=1;	pGr->cSize=size;
	pGr->rRect=SmRect32(comp->left,comp->upper,comp->left+comp->w,comp->upper+comp->h);
}


void GarbageTestBag::Add(CCOM_comp *comp)
{
	if (ExistVers(comp))
	{
		Int32 nGroup=FindGroup(comp);
		if (nGroup>=0)	AddToGroup(nGroup,comp);
		else			NewGroup(comp);
		nLetComp++;		nLetArea += comp->w*comp->h;
	}
	else
	{
		nGarbComp++;	nGarbArea += comp->w*comp->h;
	}
}

Int32 GarbageTestBag::IsGarbage()
{
	if (nLetComp==0)
		return 1;

	SizeGroup *pGrMax=sizeGroups+nLargestGroup;
	Int32 maxGroupSize=pGrMax->nComp;
	if (maxGroupSize<3 && nLetComp>=5 || nLetComp>=3 && maxGroupSize==1)	//все разные
		return 1;

	if (nLetComp<5)
	{
		if (maxGroupSize<=2 && nGarbArea>10*nLetArea ||
			maxGroupSize<=3 && nGarbArea>100*nLetArea)
			return 1;
	}

	if (maxGroupSize>=5 && 8*maxGroupSize>nLetComp)
		return -1;

	if (maxGroupSize==nLetComp)  //все буквы одинакового размера
	{
		if (nLetComp>=5 || nLetArea>nGarbArea)
			return -1;
	}

	if (maxGroupSize<=2 && nGarbArea>nGenArea2)
		return 1;

	//последние попытки хоть как-то определиться
	if (pGrMax->rRect.Area() > 3*pGrMax->nComp*(pGrMax->cSize.x*pGrMax->cSize.y))  //буквы расположены слишком редко
		return 1;

	if (nLetComp<=2 && maxGroupSize==1 && nGarbArea>nLetArea)
		return 1;


	return 0;
}

Int32 GarbageTestBag::SureGarbage()
{
	Int32 maxGroupSize=sizeGroups[nLargestGroup].nComp;
	if (maxGroupSize>=5 && 8*maxGroupSize>nLetComp)
		return -1;

	if (maxGroupSize<=2 && nGarbArea>nGenArea2)
		return 1;

	return 0;

}


static Word32 key=2;                  //для snap'а
static Word8 buffer[256];
static char *message = (char *)buffer;

static PAGEINFO PageInfo;
static Handle hCCOM;
static Handle hCCOMBig;
static Handle hCPage;
static Int32 compNum;
static CCOM_comp *CompList[MAXCOMPNUMBER];
static Int32 negNum;
static CCOM_comp *NegList[MAXCOMPNUMBER];
static Word8 NegFlag[MAXCOMPNUMBER];
#define NEGFLAG	1
static PRSPreProcessImage Image;
static Rect32 gRectTemplate;


 /*
Bool32 PUMA_XSetTemplate(Rect32 rect)
{
	Rect32 old_rect = gRectTemplate;
	Bool32 rc = FALSE;
	CIMAGEBITMAPINFOHEADER info = {0};

	if(CIMAGE_GetImageInfo((PWord8)PUMA_IMAGE_USER,&info))
	{
		CIMAGE_Rect full = {0,0,info.biWidth,info.biHeight};
		PAGEINFO           PInfo = {0};

		GetPageInfo(hCPAGE,&PInfo);

		if(rect.left<0 && rect.right<0 &&
			rect.bottom<0 && rect.top<0)
		{
			rect.bottom = full.dwHeight;
			rect.left   = full.dwX;
			rect.right  = full.dwWidth;
			rect.top	= full.dwY;
		}

		if(old_rect.bottom == rect.bottom &&
			old_rect.left == rect.left &&
			old_rect.right== rect.right &&
			old_rect.top == rect.top)
		{
			PInfo.X = rect.left;
			PInfo.Y = rect.top;
			SetPageInfo(hCPAGE,PInfo);
			return TRUE;
		}
		if(CIMAGE_AddReadCloseRects((PWord8)PUMA_IMAGE_USER,1, &full))
		{
			if(rect.left>=0 && rect.top>=0 &&
				(rect.right-rect.left)<=info.biWidth &&
				(rect.bottom - rect.top)<=info.biHeight)

			{
				CIMAGE_Rect r = {rect.left,rect.top,rect.right-rect.left,rect.bottom-rect.top};
				rc = CIMAGE_RemoveReadCloseRects((PWord8)PUMA_IMAGE_USER,1, &r);
				PInfo.X = rect.left;
				PInfo.Y = rect.top;
			}
			else
			{
				CIMAGE_Rect r = {0,0,info.biWidth-1,info.biHeight-1};
				rc = CIMAGE_RemoveReadCloseRects((PWord8)PUMA_IMAGE_USER,1, &r);
				PInfo.X = 0;
				PInfo.Y = 0;
			}
			SetPageInfo(hCPAGE,PInfo);
		    SetUpdate(FLG_UPDATE,FLG_UPDATE_NO);
			gRectTemplate = rect;
		}
	}
	return rc;
}
*/
/*
CCOM_comp * dualLP(CCOM_comp *comp)
{
	static CCOM_comp newComp;
	static Word8 lp[6000+2];
	RecRaster turned;
	RecRaster org;
	Int16 lp_size=0,numcomp=0;
	if (!CCOM_GetRaster(comp,&org ))
		return NULL;
	if (!EVN_C_Raster (&org.Raster[0],&turned.Raster[0],org.lnPixWidth,org.lnPixHeight))
		return NULL;
	turned.lnPixWidth=org.lnPixHeight;
	turned.lnPixHeight=org.lnPixWidth;
	turned.lnRasterBufSize=org.lnRasterBufSize;
	if (!CCOM_MakeLP (&turned, (Word8*)&lp, &lp_size, &numcomp))
		return NULL;
	memcpy((void*)&newComp,comp,sizeof(CCOM_comp));
	newComp.h=comp->w;
	newComp.w=comp->h;
	newComp.rw=(newComp.w-1)/8+1;
	newComp.size_linerep=lp_size;
	newComp.linerep=lp;
	newComp.nl=newComp.begs=newComp.ends=-1;
	return &newComp;
}
*-/

static Rect16 toRect16(Rect32 &r32)
{
	Rect16 r16 = {(Int16)r32.left,(Int16)r32.top,(Int16)r32.right,(Int16)r32.bottom};
	return r16;
}

static Rect32 toRect16(Rect16 &r16)
{
	Rect32 r32 = {(Int32)r16.left,(Int32)r16.top,(Int32)r16.right,(Int32)r16.bottom};
	return r32;
}

static Bool32 InRect(Rect32 *r,Point32 *p)
{
	return 	p->x >= r->left && p->x <= r->right && p->y >= r->top && p->y <= r->bottom;
}

static Rect32 rotate(Rect32 *rect, Int32 angle, Point32 *page)
{
	Rect32 res;
	Int32 width=rect->right-rect->left,height=rect->bottom-rect->top;
	Int32 rm=page->x-rect->right,bm=page->y-rect->bottom;
	if (angle<0)	 angle += 360;
	switch(angle)
	{
	case 270:	res.left=rect->top; res.top=rm; res.right=res.left+height; res.bottom=res.top+width;
				break;

	case 180:	res.left=rm; res.top=bm;  res.right=res.left+width; res.bottom=res.top+height;
				break;

	case  90:	res.left=bm; res.top=rect->left;  res.right=res.left+height; res.bottom=res.top+width;
				break;

	default:	res=*rect;
	}
	return res;
}

static Bool32 AddToBigLineList(CompInfo *info, CCOM_lnhead  *lnbeg, Int32 weight)
{
	LineWeight *end=info->bigLinesWeights+MAXBIGLINES,*line=end-1;
	while(line>=info->bigLinesWeights && weight>line->weight)	  line--;
	line++;
	if (line>=end)
		return FALSE;

	memmove(line+1,line,(MAXBIGLINES-(line-info->bigLinesWeights)-1)*sizeof(LineWeight*));
	line->pline=lnbeg;  line->weight=weight;
	return TRUE;
}

static Int32 InList(CCOM_comp * comp, CCOM_comp *list[])
{
	CCOM_comp **pos=&list[0];
	for (Int32 i=0; *pos; i++,pos++)
	{
		CCOM_comp *fromList=*pos;
		if (fromList->left==comp->left && fromList->upper==comp->upper &&
			fromList->h==comp->h && fromList->w==comp->w)
			return i;
	}
	return -1;
}

static Int32 GetCompInfo(CompInfo *info)
{
	CCOM_comp *comp=info->comp;
	Interval *phabris,*habris;	phabris=habris=info->habris;
	Interval *pvabris,*vabris;	pvabris=vabris=info->vabris;
	Int32 i,dark=0,hwhole=0,vwhole=0,height=0,width=0;
	Int16 fact = 1<<(comp->scale&7);
	if (comp->h>MAXHEIGHT)
		return -1;

	for (i=0; i<comp->h; i++,phabris++)
	{
		phabris->b=MAXWIDTH;
		phabris->e=0;
	}
	for (i=0; i<comp->w; i++,pvabris++)
	{
		pvabris->b=MAXHEIGHT;
		pvabris->e=0;
	}

	if (comp->scale & CCOM_LONGLINES)
	{
		CCOM_lnhead  *lnhead;
	//	lnhead = (_lnhead*)((char*)comp + sizeof(CCOM_comp) + sizeof(Word16));
		lnhead = (CCOM_lnhead*)(comp->linerep + sizeof(Word16));
		while( lnhead->lth )						// по линиям
		{
			Int32 lineDark=0;
			Int16 cInter = lnhead->row;
			CCOM_interval16 *inter = (CCOM_interval16*)(lnhead+1);
			while( inter->l && cInter<MAXHEIGHT )	// по интервалам линии
			{
				if (!LDPUMA_Skip(hAutoTemplateBCShow))
				{
					Int16 h=comp->upper+cInter;
					Point16 beg={comp->left+inter->e-inter->l,h},end={comp->left+inter->e,h};
					LDPUMA_DrawLine(hWndTurn,&beg,&end,0,wRGB(0,255,0),PEN_WIDTH,key);
				}
				Int16 beg = inter->e - inter->l;
				habris[cInter].b = MIN(habris[cInter].b,beg);
				habris[cInter].e = MAX(habris[cInter].e,inter->e);
				for (i=inter->l,pvabris=vabris+inter->e-1; i>0; i--,pvabris--)
				{
					if (cInter < pvabris->b)	pvabris->b = cInter;
					else
					if (pvabris->e < cInter)	pvabris->e = cInter;
				}
				width = MAX(width,inter->e);
				lineDark += inter->l;

				inter++;
				cInter++;
				assert(cInter<MAXHEIGHT);
			}
			dark += lineDark;
			AddToBigLineList(info,lnhead,lineDark);
			height = MAX(height,cInter);
			lnhead = (CCOM_lnhead*)((char*)lnhead+lnhead->lth);
		}
	}
	else
	{
		CCOM_lnhead  *lnhead;
	//	lnhead = (_lnhead*)((char*)comp + sizeof(CCOM_comp) + sizeof(Word16));
		lnhead = (CCOM_lnhead*)(comp->linerep + sizeof(Word16));
		while( lnhead->lth )						// по линиям
		{
			Int32 lineDark=0;
			Int16 cInter = lnhead->row;
			CCOM_interval *inter = (CCOM_interval*)(lnhead+1);
			while( inter->l && cInter<MAXHEIGHT )	// по интервалам линии
			{
				if (!LDPUMA_Skip(hAutoTemplateBCShow))
				{
					Int16 h=comp->upper+cInter;
					Point16 beg={comp->left+inter->e-inter->l,h},end={comp->left+inter->e,h};
					LDPUMA_DrawLine(hWndTurn,&beg,&end,0,wRGB(0,255,0),PEN_WIDTH,key);
				}
				Int16 beg = inter->e - inter->l;
				habris[cInter].b = MIN(habris[cInter].b,beg);
				habris[cInter].e = MAX(habris[cInter].e,inter->e);
				for (i=inter->l,pvabris=vabris+inter->e-1; i>0; i--,pvabris--)
				{
					if (cInter < pvabris->b)	pvabris->b = cInter;
					else
					if (pvabris->e < cInter)	pvabris->e = cInter;
				}
				width = MAX(width,inter->e);
				lineDark += inter->l;

				inter++;
				cInter++;
				assert(cInter<MAXHEIGHT);
			}
			dark += lineDark;
			AddToBigLineList(info,lnhead,lineDark);
			height = MAX(height,cInter);
			lnhead = (CCOM_lnhead*)((char*)lnhead+lnhead->lth);
		}
	}
//test
	if (!LDPUMA_Skip(hAutoTemplateBCShow))
	{
		LDPUMA_WaitUserInput(hDebugAutoTemplate,NULL);
		LDPUMA_DeleteLines(hWndTurn,key);
	}

	for (i=0,phabris=habris; i<height; i++,phabris++)
	{
		hwhole += phabris->e - phabris->b;
/*
		Int16 h=comp->upper+fact*(Int16)i,he=h+fact;
		for ( ; h<he; h++)
		{
			Point16 beg={comp->left+fact*phabris->b,h},end={comp->left+fact*phabris->e,h};
			LDPUMA_DrawLine(hWndTurn,&beg,&end,0,wRGB(0,255,0),PEN_WIDTH,key);
		}
*-/
	}
	for (i=0,pvabris=vabris; i<width; i++,pvabris++)
		vwhole += pvabris->e - pvabris->b + 1;

	info->dens = dark;
	info->hdens	= (hwhole==0) ? 0 : dark*100/hwhole;
	info->vdens	= (vwhole==0) ? 0 : dark*100/vwhole;
	info->w = width;	info->h = height;

	if (height==0)
		info->ww = 0;
	else
	{
		Int32 count=0,ww=2*(hwhole+height/2)/height;
		for (i=0,hwhole=0,phabris=habris; i<height; i++,phabris++)
		{
			Int32 w=phabris->e - phabris->b;
			if (w<=ww)	{ hwhole += w; 	 count++; }
		}
		info->ww = hwhole*fact/height;	info->ww = MAX(1,info->ww);
	}

	if (width==0)
		info->wh = 0;
	else
	{
		Int32 count=0,wh=2*(vwhole+width/2)/width;
		for (i=0,vwhole=0,pvabris=vabris; i<width; i++,pvabris++)
		{
			Int32 h=pvabris->e - pvabris->b + 1;
			if (h<=wh)  { vwhole += h; 	 count++; }
		}
		info->wh = vwhole*fact/width;	info->wh = MAX(1,info->wh);
	}
	return TRUE;
}

static Bool32 OnBorder(CompInfo *info)
{
	CCOM_comp *comp=info->comp;
	Int8 side=0;
	Int32 d=MIN(info->ww,info->wh)/2;
	if (comp->left <= d)	side |= SIDE_LEFT;
	if (comp->left+comp->w-1+d >= (Int16)PageInfo.Width)	side |= SIDE_RIGHT;
	if (comp->upper <= d)	 side |= SIDE_UP;
	if (comp->upper+comp->h-1+d >= (Int16)PageInfo.Height)  side |= SIDE_DOWN;
	info->side=side;
	if (side)
	{
		if (comp->w > comp->h)
			return 	side & (SIDE_UP | SIDE_DOWN);
		else
			return 	side & (SIDE_LEFT | SIDE_RIGHT);
	}
	return	FALSE;
}

static SmRect32	*Cpattern(Interval abris[], Int32 len, Int32 side)
{
	assert(len>0);
	if (len<=0)
		return NULL;

	static	SmRect32 rect;
	Interval *pint=abris;
	Int32 ib1=0,ie1=len,ib2,ie2;
	Interval *intb=abris,*inte=abris+len-1;
	Int32 i,sb=0,sm=0,sw=0,se=0,left=MAXWIDTH,right=0;
	for (i=0; i<len; i++,pint++)
	{
		sw += pint->e-pint->b;
		sm += pint->e+pint->b;
		sb += pint->b;
		se += pint->e;
		right = MAX(right,pint->e);
		left  = MIN(left,pint->b);
	}
	sm = DIV_ROUND(sm,len*2);

	if (side&1) //слева
	{
		sb = DIV_ROUND(sb,len);
		if (sb-left>20 || 4*(sm-left)>right-sm)
			return NULL;					   // на С-образный не похоже

		Int32 topmax=0;
		sw = DIV_ROUND(se+sw,len);
		for ( ; ib1<len; ib1++,intb++)		   //ib1 - конец загогулины в начале
		{
			topmax=MAX(topmax,intb->e);
			if (intb->e <= sw) break;
		}
		for (ib2=ib1 ; ib2<len; ib2++,intb++)  //ib2 - начало выступа в середине
			if (intb->e > sw) break;

		Int32 botmax=0;
		for ( ; ie1>ib2; ie1--,inte--)		   //ie1 - начало загогулины в конце
		{
			botmax=MAX(botmax,intb->e);
			if (inte->e <= sw) break;
		}
		for (ie2=ie1 ; ie2>ib2; ie2--,inte--)  //ie2 - коней выступа в середине
			if (inte->e > sw) break;

//		if (ie2-ib2 < len/20 && abs(ie1+ib1-(ie2+ib2)) < (ie1+ib1)/10)
		if (ie2-ib2 < len/20)
		{
			if (2*ib1>topmax-sw)   ib1=0;	       // на С-образный не похоже
			if (2*(len-ie1)>botmax-sw)	ie1=len;
			rect.left=DIV_ROUND(se,len); rect.right=right; rect.top=ib1; rect.bottom=ie1;
			return &rect;
		}
	}

	if (side&2) //справа
	{
		se = DIV_ROUND(se,len);
		if (right-se>20 || 4*(right-sm)>sm-left)
			return NULL;					   // на С-образный не похоже

		Int32 topmin=MAXSIZE;
		sw = DIV_ROUND(sb-sw,len);
		for ( ; ib1<len; ib1++,intb++)			//ib1 - конец загогулины в начале
		{
			topmin=MIN(topmin,intb->b);
			if (intb->b >= sw) break;
		}

		for (ib2=ib1 ; ib2<len; ib2++,intb++)	//ib2 - начало выступа в середине
			if (intb->b < sw) break;

		Int32 botmin=0;
		for ( ; ie1>ib2; ie1--,inte--)			//ie1 - начало загогулины в конце
		{
			botmin=MIN(botmin,intb->b);
			if (inte->b >= sw) break;
		}

		for (ie2=ie1 ; ie2>ib2; ie2--,inte--)	//ie2 - коней выступа в середине
			if (inte->b < sw) break;

//		if (ie2-ib2 < len/20 && abs(ie1+ib1-(ie2+ib2)) < (ie1+ib1)/10)
		if (ie2-ib2 < len/20)
		{
			if (2*ib1>sw-topmin)   ib1=0;	       // на С-образный не похоже
			if (2*(len-ie1)>sw-botmin)	ie1=len;
			rect.left=0; rect.right=DIV_ROUND(sb,len); rect.top=ib1; rect.bottom=ie1;
			return &rect;
		}
	}
	return NULL;
}

static Bool32 Sticklike(Interval abris[], Int32 len, Int32 width, Interval *bounds)
{
	assert(len>0);
	if (len<=0)
		return NULL;

	if (16*width>=len)
		return NULL;

	Interval *pint=abris;
	Int32 i,mb=0,me=0,jut=0;
	for (i=0; i<len; i++,pint++)
	{
		mb += pint->b;
		me += pint->e;
	}

	mb = DIV_ROUND(mb,len);
	me = DIV_ROUND(me,len)+1;

	*bounds=Interval(width,0);
	for (i=0,pint=abris; i<len; i++,pint++)
	{
		Int32 mid=(pint->b+pint->e)/2;
		if (mb<=mid && mid<=me)
		{
			bounds->b = MIN(bounds->b,pint->b);
			bounds->e = MAX(bounds->e,pint->e);
		}
		else
			jut++;
	}

	return (128*jut<len || jut<3);
}

Bool32 AttachComp(CompInfo *info, Bool32 hor)
{
	CCOM_comp *comp=info->comp;
	Int32 side=0;
	Int32 fact = 1<<(comp->scale&7);
	SmRect32 *prcBay;
	if (hor)
	{
		if (info->side & SIDE_UP)	 side +=1;
		if (info->side & SIDE_DOWN)  side +=2;
		if (side)
		{
			if (prcBay=Cpattern(info->vabris,info->w,side))
			{
				info->rcBay = SmRect32(prcBay->top,prcBay->left,prcBay->bottom,prcBay->right)*fact +
					SmPoint32(comp->left,comp->upper);
				return TRUE;
			}
			Interval bounds;
			if (Sticklike(info->vabris,info->w,info->h,&bounds))
			{
				CCOM_comp *comp=info->comp;
				if (side&1)
				{
					Int32 d=bounds.e*fact;	d = MIN(d,comp->h);
					info->rcBay = SmRect32(comp->left,comp->upper+d,comp->left+comp->w,comp->upper+comp->h);
				}
				if (side&2)
				{
					Int32 d=bounds.b*fact;	d = MIN(d,comp->h);
					info->rcBay = SmRect32(comp->left,comp->upper,comp->left+comp->w,comp->upper+d);
				}
				return TRUE;
			}
		}
	}
	else
	{
		if (info->side & SIDE_LEFT)	 side +=1;
		if (info->side & SIDE_RIGHT) side +=2;
		if (side)
		{
			if (prcBay=Cpattern(info->habris,info->h,side))
			{
				info->rcBay = (*prcBay)*fact + SmPoint32(comp->left,comp->upper);
				return TRUE;
			}
			Interval bounds;
			if (Sticklike(info->habris,info->h,info->w,&bounds))
			{
				CCOM_comp *comp=info->comp;
				if (side&1)
				{
					Int32 d=bounds.e*fact;	d = MIN(d,comp->w);
					info->rcBay = SmRect32(comp->left+d,comp->upper,comp->left+comp->w,comp->upper+comp->h);
				}
				if (side&2)
				{
					Int32 d=bounds.b*fact;	d = MIN(d,comp->w);
					info->rcBay = SmRect32(comp->left,comp->upper,comp->left+d,comp->upper+comp->h);
				}
				return TRUE;
			}
		}
	}
	return FALSE;
}

static Bool32 CompFit(CompInfo *info)
{
	if (info->hdens <MIN_DENSITY && info->vdens <MIN_DENSITY || !OnBorder(info))
		return FALSE;

	CCOM_comp *comp=info->comp;
	if (comp->h > comp->w && (info->side & (SIDE_LEFT | SIDE_RIGHT))==0)	//не касается края широкой стороной
		return FALSE;

	if (comp->w > comp->h && (info->side & (SIDE_UP | SIDE_DOWN))==0)		//не касается края узкой стороной
		return FALSE;

	if (info->hdens >= MIN_DENSITY && info->vdens >= MIN_DENSITY)
	{
		Bool32 hor = comp->w > comp->h;
		if (AttachComp(info,hor))
			return TRUE;
		if (hor)
		{
			if (3*comp->h > (Int16)PageInfo.Height && AttachComp(info,0))
				return TRUE;
		}
		else
			if (3*comp->w > (Int16)PageInfo.Width && AttachComp(info,1))
				return TRUE;
	}
	else
	if (info->vdens >= MIN_DENSITY)
	{
		if (3*comp->w > (Int16)PageInfo.Width && AttachComp(info,1))
			return TRUE;
	}
	else
		if (3*comp->h > (Int16)PageInfo.Height && AttachComp(info,0))
			return TRUE;

	return FALSE;
}

static void SetNewRect(CompInfo *info, Rect32 *rect)
{
	Rect32	newRect=*rect, *bay=&info->rcBay;
	if (info->hdens>=info->vdens && (info->side & (SIDE_LEFT | SIDE_RIGHT)))  // С-образный
	{
		if (info->side & SIDE_LEFT)
			newRect.left = MAX(newRect.left,bay->left);
		else
			newRect.right = MIN(newRect.right,bay->right);
		if (info->side & SIDE_UP)
			newRect.top = MAX(newRect.top,bay->top);
		if (info->side & SIDE_DOWN)
			newRect.bottom = MIN(newRect.bottom,bay->bottom);
	}
	else// П-образный
	{
		if (info->side & SIDE_UP)
			newRect.top = MAX(newRect.top,bay->top);
		else
			newRect.bottom = MIN(newRect.bottom,bay->bottom);
		if (info->side & SIDE_LEFT)
			newRect.left = MAX(newRect.left,bay->left);
		if (info->side & SIDE_RIGHT)
			newRect.right = MIN(newRect.right,bay->right);
	}
//	Rect16 box={(Int16)newRect.left,(Int16)newRect.top,(Int16)newRect.right,(Int16)newRect.bottom};
//	LDPUMA_DrawRect(hWndTurn,&box,0,wRGB(255,0,0),PEN_WIDTH,key);
	*rect=newRect;
}

static Handle  ExtractBigComponents()
{
	Handle hCCOM=0;
/*
	if(!REXC_SetImportData(REXC_ProgressStep, rexcProgressStep))
	{
		SetReturnCode_rstuff(REXC_GetReturnCode());
		return FALSE;
	}
*-/
	Word8 w8 = (Word8)Image->gbDotMatrix;
	REXC_SetImportData(REXC_Word8_Matrix,&w8);
	w8 = (Word8)Image->gbFax100;
	REXC_SetImportData(REXC_Word8_Fax1x2,&w8);

//  if(rc && !REXCExtraDIB( exc, lpdata,0,0,0,0) ) // поиск компонент в DIB-e

	PWord8 name = (PWord8)((PageInfo.Angle==0) ? PUMA_IMAGE_USER : PUMA_IMAGE_TURN);
    CIMAGEIMAGECALLBACK clbk;
    if(!CIMAGE_GetCallbackImage(name, &clbk))
		return 0;

	ExcControl      exc = {0};
	exc.Control  = Ex_ExtraComp|Ex_Picture|Ex_PictureLarge|Ex_DisableCut  ;
	exc.MaxScale = 1;
    if(!REXCExtracomp3CB(exc, // поиск компонент by 3CallBacks
        (TImageOpen)clbk.CIMAGE_ImageOpen,
        (TImageClose)clbk.CIMAGE_ImageClose,
        (TImageRead)clbk.CIMAGE_ImageRead)
        )
		return 0;

	hCCOM = (Handle)REXCGetContainer();
	if (hCCOM)
	{
		CCOM_comp * comp;
		comp = CCOM_GetFirst ((Int32)hCCOM, NULL);
		while(comp)
		{
			CCOM_comp * next = CCOM_GetNext (comp, NULL);
			if (comp->h<=BIGHEIGHT && comp->w<=BIGWIDTH)
				CCOM_Delete((CCOM_handle )hCCOM,comp);
			comp = next;
		}
	}
/*
	if(rc)
		SetUpdate(FLG_UPDATE_NO,FLG_UPDATE_CCOM);
*-/
	return hCCOM ;
}

static Bool32 TestForNegative(CCOM_comp * comp, CCOM_handle hCComp, Handle hCPage,Word8* pImageName)
{
	Int32 n=InList(comp,NegList);
	if (n>=0)
		return NegFlag[n]&NEGFLAG;

	Rect16 Rc={comp->left,comp->upper,comp->left+comp->w,comp->upper+comp->h};
	FNRNEG_TestForNegative LRNEG_TestForNegative;
	if (!RNEG_GetExportData(RNEG_FNRNEG_TestForNegative,&LRNEG_TestForNegative))
		return FALSE;

	Bool32 rv=LRNEG_TestForNegative(hCComp,hCPage,pImageName,Rc);
	if (negNum<MAXCOMPNUMBER) NegList[negNum]=comp;
	if (rv)	NegFlag[negNum] |= NEGFLAG;
	negNum++;

	if (!LDPUMA_Skip(hDebugAutoTemplate))
	{
		sprintf(message,"Негатив (%d,%d,%d,%d): %d\n",Rc.left,Rc.top,Rc.right,Rc.bottom,rv);
		LDPUMA_Console(message);
	}
	return rv;
}

static Bool32 LetterOverRect(CCOM_comp * comp, Rect32 *rect, Bool32 mode)
{
	SmRect32 smRect(*rect);
	GarbageTestBag	bag(smRect);
	Int32 rate,size=MAX(rect->right-rect->left,rect->bottom-rect->top)/16;
	comp = (comp==0) ?
		CCOM_GetFirst ((Int32)hCCOM, NULL) :
		CCOM_GetNext (comp, NULL);

	do
	{
		if (InList(comp,CompList)<0)
		{
			Point32 center={comp->left+comp->w/2,comp->upper+comp->h/2};
			Bool32 in = InRect(rect,&center);
			if (!mode && in || mode && !in)
			{
/*
				CompInfo info(comp);
				if (0)
					GetCompInfo(&info);
*-/
				rate=-1;
				if (!mode && MAX(comp->w,comp->h)>size)
					if (TestForNegative(comp,(CCOM_handle)hCCOMBig,hCPage,(PWord8)((PageInfo.Angle==0) ? PUMA_IMAGE_USER : PUMA_IMAGE_TURN)))
							goto end;

				if ((rate=bag.AddAndTest(comp))<0)	//точно не мусор - можно заканчивать
					goto end;
			}
		}
	}
	while(comp = CCOM_GetNext (comp, NULL));
	rate=bag.AddAndTest();

end:
	if (!LDPUMA_Skip(hAutoTemplateMar))
	{
		sprintf(message,"Оценка фрагмента: %d\n",rate);
		LDPUMA_Console(message);
		Word32 color = (rate==0) ? wRGB(255,255,0) :
					   ((rate<0) ? wRGB(0,255,0) :
								   wRGB(255,0,0));
		smRect.Draw(color,key,hWndTurn);
		bag.Snap();
		LDPUMA_WaitUserInput(hAutoTemplateMar,NULL);
		LDPUMA_DeleteLines(hWndTurn, key);
		LDPUMA_DeleteRects(hWndTurn, key);
	}
	return rate<=0;	  //не мусор или не уверен - надо оставить
}


inline static Bool32 LetterInRect(CCOM_comp * comp, Rect32 *rect)
{
	return 	LetterOverRect(comp,rect,0);
}

inline static Bool32 LetterOutRect(CCOM_comp * comp, Rect32 *rect)
{
	return 	LetterOverRect(comp,rect,1);
}

static Bool32 TestOcomp(CCOM_comp * comp, SmRect32 *rect)
{
	Int32 ncond=0;
	if (comp->left==0) 	ncond++;
	if (comp->upper==0)	ncond++;
	if (comp->w>=(Int16)PageInfo.Width)		ncond++;
	if (comp->h>=(Int16)PageInfo.Height)	ncond++;
	if (ncond>=3)
	{
		Ocomp ocomp(comp);
		SmRect32 cav;
		*rect=SmRect32(MAXHEIGHT,MAXHEIGHT,0,0);
		Bool32 found=ocomp.FindCavity(&cav);
		for (Int32 i=0; i<3 && found; i++)
		{
			*rect = *rect | cav;
			if (!LetterOutRect(NULL,rect))
				return TRUE;		// наруже букв нет - больше нечего искать

			cav = *rect;
			found=ocomp.FindCavity(&cav);
		}
/*
		if (ocomp.Test(rect))
			if (!LDPUMA_Skip(hAutoTemplateBC))
				ocomp.ShowCavity();
*-/
	}
	return 	FALSE;
}

static Bool32 TestOOcomp(CCOM_comp * comp)
{
	Int16 wm=(Int16)PageInfo.Width/128,hm=(Int16)PageInfo.Height/128;
	Int32 ncond=0;
	if (comp->left<=wm) 	ncond++;
	if (comp->upper<=hm)	ncond++;
	if (comp->w>=(Int16)PageInfo.Width-wm)	ncond++;
	if (comp->h>=(Int16)PageInfo.Height-hm)	ncond++;
	if (ncond>=1)
	{
		OOcomp ocomp(comp,PageInfo.Width,PageInfo.Height);
		SmRect32 rect;
//		if (ocomp.Test(&rect) && !LetterOutRect(NULL,&rect))
		if (ocomp.Test(&rect))
			return TRUE;
	}
	return 	FALSE;
}

static void DrawBigLines(CompInfo* info)
{
	CCOM_comp * comp=info->comp;
	LineWeight *line=info->bigLinesWeights;
	Word32 *color=info->lineColors;

	sprintf(message,"left=%d upper=%d h*w=%dx%d dens=%d\n",
			comp->left,comp->upper,comp->left+comp->w,comp->upper+comp->h,info->dens);
	LDPUMA_Console(message);

	while (line->pline)								// по линиям из списка
	{
		LDPUMA_SetConsoleProperty(FALSE,FALSE,FALSE, FALSE,0,0,*color,RUSSIAN_CHARSET,NULL);

		sprintf(message,"weight=%d\n",line->weight);
		LDPUMA_Console(message);

		CCOM_lnhead  *lnhead = line->pline;
		Int16 cInter=lnhead->row,h=comp->upper+cInter;
		if (comp->scale & CCOM_LONGLINES)
		{
			CCOM_interval16 *inter = (CCOM_interval16*)(lnhead+1);
			while( inter->l && cInter<MAXHEIGHT )	// по интервалам линии
			{
				Point16 beg={comp->left+inter->e-inter->l,h},end={comp->left+inter->e,h};
				LDPUMA_DrawLine(hWndTurn,&beg,&end,0,*color,PEN_WIDTH,key);
				inter++;  cInter++;  h++;
			}
		}
		else
		{
			CCOM_interval *inter = (CCOM_interval*)(lnhead+1);
			while( inter->l && cInter<MAXHEIGHT )	// по интервалам линии
			{
				Point16 beg={comp->left+inter->e-inter->l,h},end={comp->left+inter->e,h};
				LDPUMA_DrawLine(hWndTurn,&beg,&end,0,*color,PEN_WIDTH,key);
				inter++;  cInter++;  h++;
			}
		}
		line++; color++;
	}
	LDPUMA_WaitUserInput(hDebugAutoTemplate,NULL);
	LDPUMA_DeleteLines(hWndTurn,key);
	LDPUMA_SetConsoleProperty(FALSE,FALSE,FALSE,FALSE,0,0,0,RUSSIAN_CHARSET,NULL);
}



static Bool32 BigComps()
{
	CCOM_comp * pcomp;
	Rect32 newRect=gRectTemplate;
	Int32 maxSize=MIN(PageInfo.Width,PageInfo.Height)/2;
	Int32 minSize=MAX(PageInfo.DPIX,PageInfo.DPIY)/2;
	pcomp = CCOM_GetFirst ((Int32)hCCOMBig, NULL);
	while(pcomp && (compNum<MAXCOMPNUMBER || negNum<MAXCOMPNUMBER))
	{
		Int32 size = pcomp->w+pcomp->h;
//		if (size>minSize)
		if (size>maxSize)
		{
			Int32 denst=0,dens=0;
			Rect16 box={pcomp->left,pcomp->upper,pcomp->left+pcomp->w,pcomp->upper+pcomp->h};
/*
			CompInfo  info(pcomp);
			GetCompInfo(&info);
			dens = MAX(info.hdens,info.vdens);
*-/
			SmRect32 cavRect;
			Bool32 changes;
//			CCOM_comp * turn=dualLP(pcomp);
//			if (turn)	denst = density(turn);
			if (changes=TestOcomp(pcomp,&cavRect))
			{
				if (!TestForNegative(pcomp,(CCOM_handle)hCCOMBig,hCPage,(PWord8)((PageInfo.Angle==0) ? PUMA_IMAGE_USER : PUMA_IMAGE_TURN)))
				{
					if (compNum<MAXCOMPNUMBER) CompList[compNum++]=pcomp;
					newRect.left=MAX(newRect.left,cavRect.left);
					newRect.top=MAX(newRect.top,cavRect.top);
					newRect.right=MIN(newRect.right,cavRect.right);
					newRect.bottom=MIN(newRect.bottom,cavRect.bottom);
				}
			}
			else
//				if (changes=CompFit(&info))
				if (changes=TestOOcomp(pcomp))
				{
					if (!TestForNegative(pcomp,(CCOM_handle)hCCOMBig,hCPage,(PWord8)((PageInfo.Angle==0) ? PUMA_IMAGE_USER : PUMA_IMAGE_TURN)))
					{
						if (compNum<MAXCOMPNUMBER) CompList[compNum++]=pcomp;
//						SetNewRect(&info,&newRect);
					}
				}
			if (changes && !LDPUMA_Skip(hAutoTemplateBC))
			{
				Rect16 showBox={(Int16)newRect.left,(Int16)newRect.top,(Int16)newRect.right,(Int16)newRect.bottom};
				LDPUMA_DrawRect(hWndTurn,&showBox,0,wRGB(0,255,0),PEN_WIDTH,key);
				LDPUMA_WaitUserInput(hDebugAutoTemplate,NULL);
				LDPUMA_DeleteRects(hWndTurn,key);
				sprintf(message,"Rect=(%d,%d,%d,%d) dens=%d\n",box.left,box.top,box.right,box.bottom,dens);
				LDPUMA_Console(message);
			}

		}
//		if (maxSize<size)	{	maxSize=size; maxComp=pcomp; }
		pcomp = CCOM_GetNext (pcomp, NULL);
	}

	gRectTemplate=newRect;
/*
	Rect16 box=toRect16(newRect);
	LDPUMA_DrawRect(hWndTurn,&box,0,wRGB(0,0,255),3,key);
    LDPUMA_Skip(hDebugAutoTemplate);
	LDPUMA_DeleteRects(hWndTurn,key);
*-/
	return TRUE;

}

static Bool32 OutOfRect(CCOM_comp *comp, Rect32 *rectTempl)
{
	SmRect32 compRect(comp->left,comp->upper,comp->left+comp->w,comp->upper+comp->h);
	return compRect.OutOf(*rectTempl);
}

static void GetProfile(Int32 *vprofile,Int32 *hprofile)
{
	CCOM_comp * comp;
	Int32 i,hmax=0,wmax=0,*pos;
	comp = CCOM_GetFirst ((Int32)hCCOM, NULL);
	while(comp)
	{
		if (InList(comp,CompList)<0 && !OutOfRect(comp,&gRectTemplate))
		{
			Int32 size = comp->w + comp->h;
			pos = vprofile + comp->upper;
			for (i=0; i<comp->h; i++,pos++)
			{
				*pos += size; hmax=MAX(hmax,*pos);
			}
			pos = hprofile + comp->left;
			for (i=0; i<comp->w; i++,pos++)
			{
				*pos += size; wmax=MAX(wmax,*pos);
			}
		}
		comp = CCOM_GetNext (comp, NULL);
	}

	if (!LDPUMA_Skip(hAutoTemplateMar))
	{
		for (i=0,pos=vprofile; i<(Int32)PageInfo.Height; i++,pos++)
		{
			*pos = (*pos)*PageInfo.Width/hmax;
			Point16 start={0,(Int16)i},end={(Int16)(*pos),(Int16)i};
			LDPUMA_DrawLine(hWndTurn,&start,&end,0,wRGB(0,255,0),PEN_WIDTH,key);
		}
		LDPUMA_WaitUserInput(hAutoTemplateMar,NULL);
		LDPUMA_DeleteLines(hWndTurn, key);
		for (i=0,pos=hprofile; i<(Int32)PageInfo.Width; i++,pos++)
		{
			*pos = (*pos)*PageInfo.Height/wmax;
			Point16 start={(Int16)i,0},end={(Int16)i,(Int16)(*pos)};
			LDPUMA_DrawLine(hWndTurn,&start,&end,0,wRGB(0,255,0),PEN_WIDTH,key);
		}
		LDPUMA_WaitUserInput(hAutoTemplateMar,NULL);
		LDPUMA_DeleteLines(hWndTurn, key);
	}

}

static Bool32 NextGap(Int32 *profile, Int32 size, Int32 tol, Int32 *pos, Point32 *gap)
{
	Int32 *p=profile+(*pos),*end=profile+size;
//	Int32 tol=PageInfo.Width/50;
	do
		if (p>=end)
			return FALSE;
	while (*p++>=tol);

	gap->x=(p-1)-profile;

	while (*p<tol && p<end) p++;

	gap->y=(p-1)-profile;
	*pos=p-profile;
	return TRUE;
}

static Int32 GetGaps(Int32 size, Int32 tol, Int32 *profile, Point32 *gaps)
{
	Int32 ng=0,pos=0,nmaxs=0;
//	Int32 size=(Int32)PageInfo.Height;
//	Int32 envelope[MAXHEIGHT];
/*
	envelope[nmaxs++]=*profile;
	while(pos<size)
		envelope[nmaxs++]=NextMax(profile,&pos));
	envelope[nmaxs]=-1;
*-/
	Point32 gap;
	pos=0;
	while(ng<MAXGAPS && NextGap(profile,size,tol,&pos,&gap))
		gaps[ng++]=gap;
	return ng;
}

static Int32 FindMaxGap(Point32 gaps[], Int32 ng)
{
	Point32 *gi=gaps;
	Int32 rv=0,i,gmax=0;
	for ( i=0; i<ng; i++,gi++)
	{
		Int32 gap=gi->y-gi->x;
		if (gap>gmax)
		{
			gmax=gap;	rv=i;
		}
	}
	return rv;
}

#define  EXISTVERS(comp) (comp->vers && comp->vers->lnAltCnt>0)


static Bool32 HMargins(Int32 *profile, SmRect32 &newRect)
{
	Point32 gaps[MAXGAPS]={0};
	Int32 ng=GetGaps((Int32)PageInfo.Height,15,profile,gaps);
	if (ng<2)
		return TRUE;

	Point32 *Igap=gaps;
	Int32 beg=0,curNg=ng;
	Int32 j1,j2;
	for (j1=0; j1<ng; j1++)
		if (newRect.top<gaps[j1].y)
		{
			Rect32 gapRect={newRect.left,newRect.top,newRect.right,gaps[j1].y};
			CCOM_comp * comp=NULL;
			if (LetterInRect(comp,&gapRect))
				break;
			newRect.top=MAX(newRect.top,gapRect.bottom);
		}
	;
	for (j2=ng-1; j2>=j1; j2--)
		if (gaps[j2].x<newRect.bottom)
		{
			Rect32 gapRect={newRect.left,gaps[j2].x,newRect.right,newRect.bottom};
			CCOM_comp * comp=NULL;
			if (LetterInRect(comp,&gapRect))
				break;
			newRect.bottom=MIN(newRect.bottom,gapRect.top);
		}
	return TRUE;
}

static Bool32 VMargins(Int32 *profile, SmRect32 &newRect)
{
	Point32 gaps[MAXGAPS]={0};
	Int32 minSize=MAX(PageInfo.DPIX,PageInfo.DPIY)/32;
	Int32 ng=GetGaps((Int32)PageInfo.Width,10,profile,gaps);
	if (ng<2)
		return TRUE;

	Point32 *Igap=gaps;
	Int32 beg=0,curNg=ng;
	Int32 j1,j2;
	for (j1=0; j1<ng; j1++)
		if (newRect.left<gaps[j1].y && (gaps[j1].y-gaps[j1].x)>minSize)
		{
			Rect32 gapRect={newRect.left,newRect.top,gaps[j1].y,newRect.bottom};
			CCOM_comp * comp=NULL;
			if (LetterInRect(comp,&gapRect))
				break;
			newRect.left=MAX(newRect.left,gapRect.right);
		}
	;
	for (j2=ng-1; j2>=j1; j2--)
		if (gaps[j2].x<newRect.right && (gaps[j2].y-gaps[j2].x)>minSize)
		{
			Rect32 gapRect={gaps[j2].x,newRect.top,newRect.right,newRect.bottom};
			CCOM_comp * comp=NULL;
			if (LetterInRect(comp,&gapRect))
				break;
			newRect.right=MIN(newRect.right,gapRect.left);
		}
	return TRUE;
}

static Bool32 Margins()
{
	Bool32 stop=FALSE;

	SmRect32 newRect=gRectTemplate;
	for (Int32 i=0; i<4 && !stop; i++)
	{
		Int32 vprofile[MAXHEIGHT]={0},hprofile[MAXHEIGHT]={0};
		GetProfile(vprofile,hprofile);
		HMargins(vprofile,newRect);		  // вверху и внизу
		VMargins(hprofile,newRect);
		stop = newRect==gRectTemplate;
		gRectTemplate=newRect;
	}
	return TRUE;
}

static Bool32 DoAutoTemplate(PRSPreProcessImage inImage)
{
	Image = inImage;
	hCCOM=*(Image->phCCOM);
    memset(&PageInfo,0,sizeof(PAGEINFO));
	GetPageInfo(hCPage,&PageInfo);
	if (PageInfo.BitPerPixel != 1)
		return TRUE;

	Point32 size={PageInfo.Width,PageInfo.Height};
	gRectTemplate=rotate(&Image->gRectTemplate,PageInfo.Angle,&size);
	if (PageInfo.Angle==90 || PageInfo.Angle==270)
	{
		exchange(PageInfo.Width,PageInfo.Height);
		exchange(size.x,size.y);
	}
	if (hCCOMBig=ExtractBigComponents())
	{
		BigComps();
	}
	Image->gRectTemplate=rotate(&gRectTemplate,-(Int32)PageInfo.Angle,&size);
	(Image->fnXSetTemplate)(Image->gRectTemplate);

   	Margins();

	Image->gRectTemplate=rotate(&gRectTemplate,-(Int32)PageInfo.Angle,&size);
	(Image->fnXSetTemplate)(Image->gRectTemplate);

	PageInfo.status |=	PINFO_AUTOTEMPLATE;
	SetPageInfo(hCPage,PageInfo);

	if (hCCOMBig)  CCOM_DeleteContainer((CCOM_handle)hCCOMBig);

	return TRUE;
}

static Init()
{
	memset(CompList,0,sizeof(CompList));
	memset(NegList,0,sizeof(NegList));
	memset(NegFlag,0,sizeof(NegFlag));
	compNum=negNum=0;
}

Bool32 AutoTemplate(PRSPreProcessImage Image)
{
	Bool32 rc = TRUE;

	if(!LDPUMA_Skip(Image->hDebugCancelAutoTemplate))
	{
	   hCPage=Image->hCPAGE;
	   memset(&PageInfo,0,sizeof(PAGEINFO));
	   GetPageInfo(hCPage,&PageInfo);
 	   Point32 size={PageInfo.Width,PageInfo.Height};
	   Rect32 rRef,*rNew=&Image->gRectTemplate;
	   (Image->fnXGetTemplate)(&rRef);

//	   if (PageInfo.status & PINFO_USERTEMPLATE)
//	   if (PageInfo.status & 1)
	   if (abs((Int32)PageInfo.Width-(rRef.right-rRef.left))>1 ||
		   abs((Int32)PageInfo.Height-(rRef.bottom-rRef.top))>1)
	   {
		   LDPUMA_Console("Использована ранее заданная область распознавания.\n");
		   return rc;
	   }

	   Init();
//		timer=timerAll=LDPUMA_TimeStamp(name,NULL);
	   rc = DoAutoTemplate(Image);
//		LDPUMA_TimeStamp(name,timerAll);
//	   (Image->fnXSetTemplate)(*rect);
	   if (rNew->left>rRef.left || rNew->right<rRef.right ||
		   rNew->top>rRef.top || rNew->bottom<rRef.bottom)
	   {
		   rNew->left = MAX(rNew->left-2,rRef.left);
		   rNew->top = MAX(rNew->top-2,rRef.top);
		   rNew->right = MIN(rNew->right+2,rRef.right);
		   rNew->bottom = MIN(rNew->bottom+2,rRef.bottom);
		   (Image->fnXSetTemplate)(*rNew);
	   }

	   if (!LDPUMA_Skip(hAutoTemplateWrite))
	   {
/*
		   memset(&PageInfo,0,sizeof(PAGEINFO));
		   GetPageInfo(hCPage,&PageInfo);
			Point32 size={PageInfo.Width,PageInfo.Height};
*-/
		    Rect32 rect=rotate(&Image->gRectTemplate,PageInfo.Angle,&size);
			Point32 Vertex[5] = {
				{rect.left,rect.top},
				{rect.right,rect.top},
				{rect.right,rect.bottom},
				{rect.left,rect.bottom},
				{rect.left,rect.top}
			};
			POLY_ poly={0};
			COMMON *com=&poly.com;
			com->type=CPAGE_GetInternalType("TYPE_TEXT");
			com->count=5;
			memcpy(com->Vertex,Vertex,com->count*sizeof(Point32));
			Handle hBlock = CPAGE_CreateBlock(hCPage,poly.com.type,0, 0, &poly, sizeof(poly));

			char fname[256];
			strcpy(fname,Image->szLayoutFileName);
			char *point=strrchr(fname,'.');
			if (point==NULL)
			{
				point=fname+strlen(fname);
				*point='.';
			}
			strcpy(point+1,"tpl");

			CPAGE_SavePage(hCPage,(PInt8)fname);
			point=NULL;
			CPAGE_DeleteBlock(hCPage,hBlock);
	   }

	   if (!LDPUMA_Skip(hDebugAutoTemplate))
	   {
		   	char *buf[80],*mes=(char *)buf;
		    Rect32 rect=Image->gRectTemplate;
			Rect16 box={(Int16)rect.left,(Int16)rect.top,(Int16)rect.right,(Int16)rect.bottom};
			LDPUMA_DrawRect(NULL,&box,0,wRGB(173,255,0),PEN_WIDTH,key);
			sprintf(mes,"template = (ltrb) %d %d %d %d", rect.left, rect.top, rect.right,rect.bottom);
			LDPUMA_Console(mes);
			if (hWndTurn)
			{
				rect = rotate(&rect,(Int32)PageInfo.Angle,&size);
				Rect16 rbox={(Int16)rect.left,(Int16)rect.top,(Int16)rect.right,(Int16)rect.bottom};
				LDPUMA_DrawRect(hWndTurn,&rbox,0,wRGB(173,255,0),PEN_WIDTH,key);
			}
			LDPUMA_WaitUserInput(hDebugAutoTemplate,NULL);
			LDPUMA_DeleteRects(NULL,key);
 			if (hWndTurn)
				LDPUMA_DeleteRects(hWndTurn,key);
	   }
	}
	else
		LDPUMA_Console("Пропущен этап ограничения области распознавания.\n");
	return rc;
}
*/



