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
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "wRGB.h"
#include "dpuma.h"
#include "CCom.h"
#include "RStuff.H"
#include "AutoTemplate.h"

//#define DRAWTPIPLEINT

extern Handle hDebugAutoTemplate;

static uint32_t key=2;                  //для snap'а
static uchar buffer[256];
static char *message = (char *)buffer;


//============ Функции для интервалов =================//

static Bool32 	intersect  (CCOM_interval16 *f, CCOM_interval16 *s)
{
	if (f->l==0 || s->l==0)
		return FALSE;

	int16_t fbeg=BEGIN(*f),sbeg=BEGIN(*s);
	return fbeg<=sbeg && sbeg<=f->e || fbeg<=s->e && s->e<=f->e || sbeg<=fbeg && f->e<=s->e;
}

static int16_t distance  (CCOM_interval16 *f, CCOM_interval16 *s)
{
	if (f->l==0 || s->l==0)
		return MAXWIDTH;

	int16_t fbeg=BEGIN(*f),sbeg=BEGIN(*s);
	int16_t ro1=sbeg-f->e,ro2=fbeg-s->e;
	return MAX(ro1,ro2);
}


static Bool32 	involve  (CCOM_interval16 *large, CCOM_interval16 *small)
{
	return BEGIN(*large)<=BEGIN(*small) && small->e <= large->e;
}

static CCOM_interval16 join(CCOM_interval16 *f, CCOM_interval16 *s)
{
	if (f->l==0)
		return *s;

	if (s->l==0)
		return *f;

	int16_t fbeg=BEGIN(*f),sbeg=BEGIN(*s);
	int16_t rbeg=MIN(fbeg,sbeg),rend=MAX(f->e,s->e);
	CCOM_interval16	res={rend-rbeg,rend};
	return res;
}

static void reorder(CCOM_interval16 set[], int32_t n)
{
	if (n<=1)
		return;

	Bool32 changed;
	int32_t i;
	CCOM_interval16 *sc,*sp;
	do
	{
		changed=FALSE;
		for (i=1,sp=set,sc=set+1; i<n; i++,sc++,sp++)
			if (sp->e > sc->e)
			{
				exchange(*sp,*sc);
				changed=TRUE;
			}
	}
	while(changed);
}


/*
DoubleInterval::DoubleInterval(Ocomp *comp)
{
	pComp=comp;
	memset(left,0,sizeof(left));
	memset(right,0,sizeof(right));
}
*-/
int32_t DoubleInterval::Number()
{
	int32_t n=0;
	if (left.l)	n++;
	if (right.l)	n++;
	return n;
}

void DoubleInterval::SetOne(CCOM_interval16 *add)
{
	int32_t mid=add->e-add->l/2;
	if (mid > pComp->nWidth/2)
	{
		right=*add;
		memset(&left,0,sizeof(left));
	}
	else
	{
		left=*add;
		memset(&right,0,sizeof(right));
	}
}

void DoubleInterval::Add(CCOM_interval16 *add)
{
	if (Number()==0)
	{
		SetOne(add);  return;
	}

	if (left.l != 0 && intersect(&left,add))
	{
		left=join(&left,add);
		if (intersect(&left,&right))
		{
			left=join(&left,&right);
			SetOne(&left);
		}
		return;
	}

	if (right.l != 0 && intersect(&right,add))
	{
		right=join(&right,add);
		if (intersect(&left,&right))
		{
			right=join(&left,&right);
			SetOne(&right);
		}
		return;
	}

	//не пересекается ни с одним
	CCOM_interval16	 set[3],*l=set,*m=set+1,*r=set+2;
	set[0]=left; set[1]=*add; set[2]=right;
	reorder(set,3);	  // в порядке возрастания концов
	if (Number()==1)
	{
		left=*m;  right=*r;
	}
	else  //Number==2
		if (BEGIN(*m)-l->e <= BEGIN(*r)-m->e)
		{
			left=join(l,m);  right=*r;
		}
		else
		{
			left=*l;  right=join(m,r);
		}
}

CCOM_interval16 DoubleInterval::Cavity()
{
	if (left.l==0 || right.l==0)
	{
		CCOM_interval16	 cav={0,0};
		return cav;
	}

	CCOM_interval16	cav={BEGIN(right)-left.e,BEGIN(right)};
	return cav;
/*
	CCOM_interval16 cavl={0,0},cavm={0,(int16_t)pComp->nWidth},cavr={0,cavm.e};
	int16_t cavmb=0;
	if (left.l>0)
	{
		cavl.l=cavl.e=BEGIN(left);
		cavmb=left.e;
	}
	if (right.l>0)
	{
		cavr.l=pComp->nWidth-right.e;
		cavm.e=BEGIN(right);
	}
	cavm.l=cavm.e-cavmb;

	CCOM_interval16	 res=cavl;
	if (cavm.l>res.l)	res=cavm;
	if (cavr.l>res.l)	res=cavr;

	return res;
*-/
}

void Ocomp::SetComp(CCOM_comp *comp)
{
	if (comp==NULL)
		return;

	pComp=comp;
	nWidth=comp->w; nHeight=comp->h;
	nCavBeg=-1;	 nCavWidth=nCavEnd=0;
	rCavRect=SmRect32();
	memset(ints,0,(nHeight+1)*sizeof(DoubleInterval));
	for (int32_t i=0; i<=nHeight && i<=MAXHEIGHT; i++)
		ints[i].pComp=this;
}

Bool32 Ocomp::FindCavity(SmRect32 *rect)
{
	assert(pComp);
	if (pComp==NULL)
		return FALSE;

	if (rect)
	{
		CCOM_interval16 inter =	{(int16_t)(rect->right-rect->left),(int16_t)rect->right};
		if (inter.l != 0)
		{
			memset(ints,0,(nHeight+1)*sizeof(DoubleInterval));
			for (int32_t i=0; i<=nHeight && i<=MAXHEIGHT; i++)
			{
				ints[i].pComp=this;
				if (i>=rect->top && i<=rect->bottom)
					ints[i].Add(&inter);
			}
		}
	}

	if (!FillIntervals())
		return FALSE;

	rCavRect=SmRect32();
	nCavBeg=-1;	 nCavWidth=nCavEnd=0;
	int32_t i,cLeft=0,cUp=0,cRight=0;
	Bool32 found=FALSE;
	CCOM_interval16 prev={0,0};
	for (i=0; i<=nHeight; i++)	//последний интервал - фиктивный
	{
		CCOM_interval16 curr=ints[i].Cavity();
		Bool32 notcross = !intersect(&curr,&prev);
		Bool32 beg = curr.l>0 && (notcross || prev.l==0);
		Bool32 end = prev.l>0 && (notcross || curr.l==0);
		if (end)
		{
			int32_t h=i-cUp;
			cLeft=DIV_ROUND(cLeft,h);
			cRight=DIV_ROUND(cRight,h);
			if (h>=nHeight/2 && cRight-cLeft>=nWidth/2)
			   if (involve(&(ints[i].left),&prev) || involve(&(ints[i].right),&prev))  //есть дно
			   {
					found=TRUE;
					rCavRect=SmRect32(cLeft,cUp,cRight,i-1);
					break;
			   }

			//ищем дальше
		}
		if (beg)
		{
			cUp=i;  cLeft=curr.e-curr.l;	cRight=curr.e;
		}
		if (!beg && !end)
		{
			cLeft += curr.e-curr.l;	cRight += curr.e;
		}
		prev=curr;
	}
	Point32 disp = {pComp->left,pComp->upper};
	if (rect)	*rect = rCavRect + disp;
	return found;
}

Bool32 Ocomp::FillIntervals()
{
	if (pComp->scale & CCOM_LONGLINES)
	{
		CCOM_lnhead  *lnhead;
		lnhead = (CCOM_lnhead*)(pComp->linerep + sizeof(uint16_t));
		while( lnhead->lth )						// по линиям
		{
			int16_t cInter = lnhead->row;
			CCOM_interval16 *inter = (CCOM_interval16*)(lnhead+1);
			while( inter->l && cInter<MAXHEIGHT )	// по интервалам линии
			{
				if (cInter>=nHeight)
					return FALSE;
				ints[cInter].Add(inter);
				inter++;
				cInter++;
			}
			lnhead = (CCOM_lnhead*)((char*)lnhead+lnhead->lth);
		}
	}
	else
	{
		CCOM_lnhead  *lnhead;
		lnhead = (CCOM_lnhead*)(pComp->linerep + sizeof(uint16_t));
		while( lnhead->lth )						// по линиям
		{
			int16_t cInter = lnhead->row;
			CCOM_interval *inter = (CCOM_interval*)(lnhead+1);
			while( inter->l && cInter<MAXHEIGHT )	// по интервалам линии
			{
				if (cInter>=nHeight)
					return FALSE;
				ints[cInter].Add(inter);
				inter++;
				cInter++;
			}
			lnhead = (CCOM_lnhead*)((char*)lnhead+lnhead->lth);
		}
	}
	CCOM_interval16 fict={pComp->w,pComp->w};
	ints[pComp->h].Add(&fict);

	return TRUE;
}

void Ocomp::ShowCavity()
{
	for (int32_t i=nCavBeg; i<nCavEnd; i++)
	{
		int16_t h=(int16_t)(pComp->upper+i);
		Point16 beg1={pComp->left+ints[i].left.e,h},end1={beg1.x+1,h};
		LDPUMA_DrawLine(hWndTurn,&beg1,&end1,0,wRGB(0,0,255),PEN_WIDTH,key);
		Point16 beg2={pComp->left+BEGIN(ints[i].right),h},end2={beg2.x+1,h};
		LDPUMA_DrawLine(hWndTurn,&beg2,&end2,0,wRGB(0,0,255),PEN_WIDTH,key);
	}
	LDPUMA_WaitUserInput(hDebugAutoTemplate,NULL);
	LDPUMA_DeleteLines(hWndTurn,key);
}

int32_t TripleInterval::Number()
{
	int32_t n=0;
	if (left.l)	n++;
	if (middle.l)	n++;
	if (right.l)	n++;
	return n;
}

void TripleInterval::SetOne(CCOM_interval16 *add)
{
	int32_t Ithird=pComp->nWidth/3,IIthird=2*Ithird;
	int32_t mid=add->e-add->l/2;
	if (mid >= IIthird)
	{
		right=*add;
		memset(&middle,0,sizeof(middle));
		memset(&left,0,sizeof(left));
	}
	else
	if (mid > Ithird)
	{
		middle=*add;
		memset(&right,0,sizeof(right));
		memset(&left,0,sizeof(left));
	}
	else
	{
		left=*add;
		memset(&middle,0,sizeof(middle));
		memset(&right,0,sizeof(right));
	}
}

void TripleInterval::Add(CCOM_interval16 *toAdd)
{
	if (Number()==0)
	{
		SetOne(toAdd);  return;
	}

	int32_t i,Ithird=pComp->nWidth/3,IIthird=2*Ithird;
	CCOM_interval16	 set[4];
	set[0]=left; set[1]=middle; set[2]=right; set[3]=*toAdd;
	CCOM_interval16	*curr,*next;
	CCOM_interval16 *first=set,*sec=set+1,*third=set+2,*add=set+3;

	reorder(set,4);		// в порядке возрастания концов
	if (set[0].l)		// всего 4 интервала - пару надо объединить
	{
		int32_t distmin=MAXWIDTH,imin;
		for (i=0,curr=set+i,next=curr+1; i<3; i++,curr++,next++)
		{
			int16_t dist=distance(curr,next);
			if (dist<distmin) { distmin=dist; imin=i; };
		}
		set[imin+1]=join(set+imin,set+imin+1);
		memmove(set+1,set,imin*sizeof(CCOM_interval16));	//поджимаем вправо
		memset(set,0,sizeof(CCOM_interval16));
	}

	//осталось <= 3 интервала - объединяем, если нужно
	Bool32 joined;
	do
	{
		joined=FALSE;
		for (i=1,curr=set+i,next=curr+1; i<3; i++,curr++,next++)
		{
			int16_t dist=distance(curr,next);
			if (dist<curr->l && dist<next->l)
			{
				joined=TRUE;
				*next=join(curr,next);
				memmove(set+1,set,i*sizeof(CCOM_interval16));	//поджимаем вправо
			}
		}
	}
	while(joined);

	//распределяем по местам
	if (set[1].l)		// 3 интервала - кладем слева направо
	{
		left=set[1];  middle=set[2];  right=set[3];
	}
	else if (set[2].l)	// 2 интервала - распределяем по трем вакансиям
	{
		int32_t mid=set[3].e-set[3].l/2;
		if (mid<IIthird)
		{
			left=set[2];  middle=set[3];  memset(&right,0,sizeof(right));
		}
		else
		{
			right=set[3];
			mid=set[2].e-set[2].l/2;
			if (mid<=Ithird)  { left=set[2];	memset(&middle,0,sizeof(middle)); }
			else			  {	memset(&left,0,sizeof(left));  middle=set[2];     }
		}
	}
	else if (set[3].l)	//один интервал
		SetOne(set+3);
	else
		assert(0);
}

void OOcomp::SetComp(CCOM_comp *comp, int32_t pageWidth, int32_t pageHeight)
{
	if (comp==NULL)
		return;

	pComp=comp;
	nWidth=comp->w; nHeight=comp->h;
	nPageWidth=pageWidth; nPageHeight=pageHeight;
	nCavBeg=-1;	 nCavWidth=nCavEnd=0;
	nDens=0;
	rCavRect=SmRect32();
	memset(ints,0,(nHeight+1)*sizeof(TripleInterval));
	for (int32_t i=0; i<=nHeight && i<=MAXHEIGHT; i++)
		ints[i].pComp=this;
}

Bool32 OOcomp::FillIntervals()
{
	if (pComp->scale & CCOM_LONGLINES)
	{
		CCOM_lnhead  *lnhead;
		lnhead = (CCOM_lnhead*)(pComp->linerep + sizeof(uint16_t));
		while( lnhead->lth )						// по линиям
		{
			int16_t cInter = lnhead->row;
			CCOM_interval16 *inter = (CCOM_interval16*)(lnhead+1);
			while( inter->l && cInter<MAXHEIGHT )	// по интервалам линии
			{
				if (cInter>=nHeight)
					return FALSE;
				ints[cInter].Add(inter);
				nDens += inter->l;
				inter++;
				cInter++;
			}
			lnhead = (CCOM_lnhead*)((char*)lnhead+lnhead->lth);
		}
	}
	else
	{
		CCOM_lnhead  *lnhead;
		lnhead = (CCOM_lnhead*)(pComp->linerep + sizeof(uint16_t));
		while( lnhead->lth )						// по линиям
		{
			int16_t cInter = lnhead->row;
			CCOM_interval *inter = (CCOM_interval*)(lnhead+1);
			while( inter->l && cInter<MAXHEIGHT )	// по интервалам линии
			{
				if (cInter>=nHeight)
					return FALSE;
				ints[cInter].Add(inter);
				nDens += inter->l;
				inter++;
				cInter++;
			}
			lnhead = (CCOM_lnhead*)((char*)lnhead+lnhead->lth);
		}
	}
	CCOM_interval16 fict={pComp->w,pComp->w};
	ints[pComp->h].Add(&fict);

	return TRUE;
}

Bool32 OOcomp::Test(SmRect32 *rect)
{
	assert(pComp);
	if (pComp==NULL)
		return FALSE;

	if (!FillIntervals())
		return FALSE;


	int32_t i,dens=0;
	int32_t lm=0,lc=0,rm=0,rc=0;
	int16_t um=pComp->upper,bm=nPageHeight-(um+pComp->h);
	for (i=0; i<nHeight; i++)
	{
		TripleInterval *intv=ints+i;
#ifdef DRAWTPIPLEINT
				int32_t color=wRGB(0,0,255);
				int16_t h=(int16_t)(pComp->upper+i);
				{
				Point16 beg={pComp->left+intv->left.e-intv->left.l,h},end={pComp->left+intv->left.e,h};
				LDPUMA_DrawLine(hWndTurn,&beg,&end,0,wRGB(0,0,255),PEN_WIDTH,key);
				}
				{
				Point16 beg={pComp->left+intv->middle.e-intv->middle.l,h},end={pComp->left+intv->middle.e,h};
				LDPUMA_DrawLine(hWndTurn,&beg,&end,0,wRGB(255,0,255),PEN_WIDTH,key);
				}
				{
				Point16 beg={pComp->left+intv->right.e-intv->right.l,h},end={pComp->left+intv->right.e,h};
				LDPUMA_DrawLine(hWndTurn,&beg,&end,0,wRGB(0,255,0),PEN_WIDTH,key);
				}
#endif
		if (intv->left.l)	{ lm += BEGIN(intv->left);  lc++; }
		if (intv->right.l)	{ rm += intv->right.e; rc++; }
		dens += intv->left.l + intv->middle.l + intv->right.l;
	}
#ifdef DRAWTPIPLEINT
	sprintf(message,"left=%d upper=%d h*w=%dx%d dens=%d\n",
			pComp->left,pComp->upper,pComp->left+pComp->w,pComp->upper+pComp->h,dens);
	LDPUMA_Console(message);
	LDPUMA_WaitUserInput(hDebugAutoTemplate,NULL);
	LDPUMA_DeleteLines(hWndTurn,key);
#endif
	if (nDens*100/dens<MIN_DENSITY)
		return FALSE;
	lm = lc ? lm/lc : 0;
	rm = rc ? rm/rc : 0;
/*
	if (um>nPageHeight/16 && bm>nPageHeight/16 && lm>nPageWidth/16 && rm<nPageWidth/16)
		return FALSE;
*-/
	*rect=SmRect32(lm,um,nPageWidth-rm,nPageHeight-bm);

	if (pComp->h==nPageHeight && pComp->w<=nPageWidth/50 &&
		10*pComp->left>=3*nPageWidth && 10*(pComp->left+pComp->w)<=7*nPageWidth)
			return TRUE;

	int16_t wm=(int16_t)nPageWidth/128,hm=(int16_t)nPageHeight/128;
	int32_t boundh=0,boundv=0;
	if (pComp->left<=wm)  boundv++;
	if (pComp->upper<=hm) boundh++;
	if (pComp->left+pComp->w>=nPageWidth-wm)	boundv++;
	if (pComp->upper+pComp->h>=nPageHeight-hm)	boundh++;
	if (boundh+boundv>2)
		return TRUE;

	if (boundh+boundv==2 && boundh>0)
		return TRUE;

	if (boundh+boundv==2 && nDens*100/dens >= 95)
		return TRUE;

	return FALSE;

}
*/
