/*
Copyright (c) 1993-2008, Cognitive Technologies
All rights reserved.

Ðàçðåøàåòñÿ ïîâòîðíîå ðàñïðîñòðàíåíèå è èñïîëüçîâàíèå êàê â âèäå èñõîäíîãî êîäà,
òàê è â äâîè÷íîé ôîðìå, ñ èçìåíåíèÿìè èëè áåç, ïðè ñîáëþäåíèè ñëåäóþùèõ óñëîâèé:

      * Ïðè ïîâòîðíîì ðàñïðîñòðàíåíèè èñõîäíîãî êîäà äîëæíû îñòàâàòüñÿ óêàçàííîå
        âûøå óâåäîìëåíèå îá àâòîðñêîì ïðàâå, ýòîò ñïèñîê óñëîâèé è ïîñëåäóþùèé 
        îòêàç îò ãàðàíòèé. 
      * Ïðè ïîâòîðíîì ðàñïðîñòðàíåíèè äâîè÷íîãî êîäà â äîêóìåíòàöèè è/èëè â 
        äðóãèõ ìàòåðèàëàõ, ïîñòàâëÿåìûõ ïðè ðàñïðîñòðàíåíèè, äîëæíû ñîõðàíÿòüñÿ
        óêàçàííàÿ âûøå èíôîðìàöèÿ îá àâòîðñêîì ïðàâå, ýòîò ñïèñîê óñëîâèé è
        ïîñëåäóþùèé îòêàç îò ãàðàíòèé.  
      * Íè íàçâàíèå Cognitive Technologies, íè èìåíà åå ñîòðóäíèêîâ íå ìîãóò 
        áûòü èñïîëüçîâàíû â êà÷åñòâå ñðåäñòâà ïîääåðæêè è/èëè ïðîäâèæåíèÿ 
        ïðîäóêòîâ, îñíîâàííûõ íà ýòîì ÏÎ, áåç ïðåäâàðèòåëüíîãî ïèñüìåííîãî
        ðàçðåøåíèÿ. 

ÝÒÀ ÏÐÎÃÐÀÌÌÀ ÏÐÅÄÎÑÒÀÂËÅÍÀ ÂËÀÄÅËÜÖÀÌÈ ÀÂÒÎÐÑÊÈÕ ÏÐÀÂ È/ÈËÈ ÄÐÓÃÈÌÈ ËÈÖÀÌÈ "ÊÀÊ
ÎÍÀ ÅÑÒÜ" ÁÅÇ ÊÀÊÎÃÎ-ËÈÁÎ ÂÈÄÀ ÃÀÐÀÍÒÈÉ, ÂÛÐÀÆÅÍÍÛÕ ßÂÍÎ ÈËÈ ÏÎÄÐÀÇÓÌÅÂÀÅÌÛÕ, 
ÂÊËÞ×Àß ÃÀÐÀÍÒÈÈ ÊÎÌÌÅÐ×ÅÑÊÎÉ ÖÅÍÍÎÑÒÈ È ÏÐÈÃÎÄÍÎÑÒÈ ÄËß ÊÎÍÊÐÅÒÍÎÉ ÖÅËÈ, ÍÎ ÍÅ
ÎÃÐÀÍÈ×ÈÂÀßÑÜ ÈÌÈ. ÍÈ ÂËÀÄÅËÅÖ ÀÂÒÎÐÑÊÈÕ ÏÐÀÂ È ÍÈ ÎÄÍÎ ÄÐÓÃÎÅ ËÈÖÎ, ÊÎÒÎÐÎÅ 
ÌÎÆÅÒ ÈÇÌÅÍßÒÜ È/ÈËÈ ÏÎÂÒÎÐÍÎ ÐÀÑÏÐÎÑÒÐÀÍßÒÜ ÏÐÎÃÐÀÌÌÓ, ÍÈ Â ÊÎÅÌ ÑËÓ×ÀÅ ÍÅ 
ÍÅÑ¨Ò ÎÒÂÅÒÑÒÂÅÍÍÎÑÒÈ, ÂÊËÞ×Àß ËÞÁÛÅ ÎÁÙÈÅ, ÑËÓ×ÀÉÍÛÅ, ÑÏÅÖÈÀËÜÍÛÅ ÈËÈ 
ÏÎÑËÅÄÎÂÀÂØÈÅ ÓÁÛÒÊÈ, ÑÂßÇÀÍÍÛÅ Ñ ÈÑÏÎËÜÇÎÂÀÍÈÅÌ ÈËÈ ÏÎÍÅÑÅÍÍÛÅ ÂÑËÅÄÑÒÂÈÅ 
ÍÅÂÎÇÌÎÆÍÎÑÒÈ ÈÑÏÎËÜÇÎÂÀÍÈß ÏÐÎÃÐÀÌÌÛ (ÂÊËÞ×Àß ÏÎÒÅÐÈ ÄÀÍÍÛÕ, ÈËÈ ÄÀÍÍÛÅ, 
ÑÒÀÂØÈÅ ÍÅÃÎÄÍÛÌÈ, ÈËÈ ÓÁÛÒÊÈ È/ÈËÈ ÏÎÒÅÐÈ ÄÎÕÎÄÎÂ, ÏÎÍÅÑÅÍÍÛÅ ÈÇ-ÇÀ ÄÅÉÑÒÂÈÉ 
ÒÐÅÒÜÈÕ ËÈÖ È/ÈËÈ ÎÒÊÀÇÀ ÏÐÎÃÐÀÌÌÛ ÐÀÁÎÒÀÒÜ ÑÎÂÌÅÑÒÍÎ Ñ ÄÐÓÃÈÌÈ ÏÐÎÃÐÀÌÌÀÌÈ, 
ÍÎ ÍÅ ÎÃÐÀÍÈ×ÈÂÀßÑÜ ÝÒÈÌÈ ÑËÓ×ÀßÌÈ), ÍÎ ÍÅ ÎÃÐÀÍÈ×ÈÂÀßÑÜ ÈÌÈ, ÄÀÆÅ ÅÑËÈ ÒÀÊÎÉ 
ÂËÀÄÅËÅÖ ÈËÈ ÄÐÓÃÎÅ ËÈÖÎ ÁÛËÈ ÈÇÂÅÙÅÍÛ Î ÂÎÇÌÎÆÍÎÑÒÈ ÒÀÊÈÕ ÓÁÛÒÊÎÂ È ÏÎÒÅÐÜ.

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

#define PEN_WIDTH -100	//äëÿ ïðîðèñîâêè

/* min and max macros */
#define max(a,b)  (((a) > (b)) ? (a) : (b))
#define min(a,b)  (((a) < (b)) ? (a) : (b))

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

