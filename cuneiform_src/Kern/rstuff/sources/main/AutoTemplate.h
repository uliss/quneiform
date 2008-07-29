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

#define	 NULL 0
#define  MAXCOMPNUMBER 100
#define ABS  abs
#define MAX  max
#define MIN  min
#define MAXHEIGHT 16384
#define MAXWIDTH  16384
#define	MAXSIZE (max(MAXHEIGHT,MAXWIDTH))
#define MAXGAPS	MAXHEIGHT/50
#define BIGHEIGHT 63
#define BIGWIDTH 128
#define MIN_DENSITY	90

#define PEN_WIDTH -100	//для прорисовки

#include "minmax.h"

#define BEGIN(iv) ((iv).e-(iv).l)
#define SCALE_FACT(comp) (1<<((comp)->scale&7))
#define DIV_ROUND(dividend,divizor) (((dividend)+(divizor)/2)/(divizor))

extern Handle hWndTurn;


template <class T> exchange(T& first, T& second)
{
	T temp=first; first=second; second=temp;
}

class SmPoint32:  public Point32
{
public:
	  SmPoint32(Int32 xi, Int32 yi)
	  {
		  x=xi; y=yi;
	  }
};

class SmRect32: public Rect32
{
public:
	SmRect32()
	{
	   left=top=right=bottom=0;
	}
	SmRect32(Rect32& rect)
	{
	   left=rect.left; top=rect.top; right=rect.right; bottom=rect.bottom;
	}
 	SmRect32(Int32 l,Int32 t,Int32 r,Int32 b)
	{
	   left=l; top=t; right=r; bottom=b;
	}
	SmRect32&  operator =(Rect32& rect)
	{
	   left=rect.left; top=rect.top; right=rect.right; bottom=rect.bottom;
	   return *this;
	}
	SmRect32&  operator |(Rect32& rect)
	{
	   left=min(left,rect.left); top=min(top,rect.top); right=max(right,rect.right); bottom=max(bottom,rect.bottom);
	   return *this;
	}
	SmRect32&  operator *(Int32 fact)
	{
		left *= fact; top *= fact; right *= fact; bottom *= fact;
		return *this;
	}
	SmRect32&  operator +(Point32 p)
	{
		left += p.x; right += p.x; top += p.y; bottom += p.y;
		return *this;
	}
	Bool32  operator >(Rect32& rect)
	{
		return (right-left)*(bottom-top) > (rect.right-rect.left)*(rect.bottom-rect.top);
	}
	Bool32  operator ==(Rect32& rect)
	{
		return right==rect.right && left==rect.left && top==rect.top && bottom==rect.bottom;
	}
	SmRect32& Turn()
	{
		exchange(left,top); exchange(right,bottom);
		return *this;
	}
	Bool32 In(SmRect32& rect)
	{
		return left>=rect.left && top>=rect.top && right<=rect.right && bottom<=rect.bottom;
	}
	Bool32 In(Rect32& rect)
	{
		return left>=rect.left && top>=rect.top && right<=rect.right && bottom<=rect.bottom;
	}
	Bool32 OutOf(SmRect32& rect)
	{
		return left>rect.right || top>=rect.bottom || right<=rect.left || bottom<rect.top;
	}
	Bool32 OutOf(Rect32& rect)
	{
		return left>rect.right || top>=rect.bottom || right<=rect.left || bottom<rect.top;
	}
	Int32 Area()
	{
		return (right-left)*(bottom-top);
	}
	void Draw(Word32 color, Word32 key, Handle hWnd=NULL)
	{
		Rect16 rect = {(Int16)left,(Int16)top,(Int16)right,(Int16)bottom};
		LDPUMA_DrawRect(hWnd,&rect,0,color,1,key);
	}

};


typedef struct tagLineWeight
{
	CCOM_lnhead	*pline;
	Int32 weight;
}  LineWeight;


class Ocomp;

class DoubleInterval
{
public:
	Ocomp *pComp;
	CCOM_interval16 left;
	CCOM_interval16	right;

//	DoubleInterval();
	Int32 Number();
	void SetOne(CCOM_interval16 *add);
	void Add(CCOM_interval16 *add);
	void Add(CCOM_interval *add)
	{
		CCOM_interval16 intv16={add->l,add->e};
		Add(&intv16);
	};
	CCOM_interval16 Cavity();
};

class OOcomp;

class TripleInterval
{

public:
	OOcomp *pComp;
	CCOM_interval16 left;
	CCOM_interval16 middle;
	CCOM_interval16	right;

//	DoubleInterval();
	Int32 Number();
	void SetOne(CCOM_interval16 *add);
	void Add(CCOM_interval16 *add);
	void Add(CCOM_interval *add)
	{
		CCOM_interval16 intv16={add->l,add->e};
		Add(&intv16);
	};
//	CCOM_interval16 Cavity();
};

class Ocomp
{
	CCOM_comp *pComp;
	Bool32 FillIntervals();

public:
	Int32	nWidth;
	Int32	nHeight;
	SmRect32 rCavRect;
	Int32	nCavBeg,nCavEnd,nCavWidth;
	DoubleInterval ints[MAXHEIGHT+1];

	Ocomp (CCOM_comp *comp) { SetComp(comp); };
	CCOM_comp *GetComp() { return pComp; }
	void SetComp(CCOM_comp *comp);
	Bool32 Test(SmRect32 *rect=NULL);
	Bool32 FindCavity(SmRect32 *rect);
	void ShowCavity();

};

class OOcomp
{
	CCOM_comp *pComp;
	Bool32 FillIntervals();

public:
	Int32	nWidth;
	Int32	nHeight;
	Int32	nPageWidth;
	Int32	nPageHeight;
	SmRect32 rCavRect;
	Int32	nCavBeg,nCavEnd,nCavWidth;
	Int32	nDens;
	TripleInterval ints[MAXHEIGHT+1];

	OOcomp (CCOM_comp *comp, Int32 pageWidth, Int32 pageHeight)
		{ SetComp(comp,pageWidth,pageHeight); };
	CCOM_comp *GetComp() { return pComp; }
	void SetComp(CCOM_comp *comp, Int32 pageWidth, Int32 pageHeight);
	Bool32 Test(SmRect32 *rect=NULL);
//	Bool32 FindCavity(SmRect32 *rect);
	void ShowCavity();

};

