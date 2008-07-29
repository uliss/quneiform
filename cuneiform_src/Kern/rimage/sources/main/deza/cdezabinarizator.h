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

// CDezaBinarizator.h: interface for the CDezaBinarizator class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(_CDEZA_BINARIZATOR_H_)
#define _CDEZA_BINARIZATOR_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

# if defined (_DEBUG)
# pragma message( __FILE__" : Binarizator by Deza")
# endif // (_DEBUG)

#include "stdlib.h"
#include "globus.h"
#include "crimemory.h"

///////////////////////////////////////////////////////////////////////////////////////////////////
//
typedef	struct tagTigerImageInfo
	{
		unsigned short int wImageHeight;       /* Height of the image in lines */
		unsigned short int wImageWidth;        /* Width of the image in pixels */
		unsigned short int wImageByteWidth; /* Size of the image line in bytes */
		unsigned short int wImageDisplacement;
		/* Displacement of the first pixel of image line in pixels, usually 0 */
		unsigned short int wResolutionX;                /* Resolution by X-axe */
		unsigned short int wResolutionY;                /* Resolution by Y-axe */
		unsigned char bFotoMetrics;                             /* White pixel */
		unsigned char bUnused;         /* not used; for alignment purpose only */
		unsigned short int wAddX;
		unsigned short int wAddY;
	} CTBINTigerImageInfo, *PCTBINTigerImageInfo;
///////////////////////////////////////////////////////////////////////////////////////////////////
//
#define   DB_MALLOC(a)        RIMAGEAlloc(a)
#define   DB_FREE(a)          RIMAGEFree(a)
///////////////////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
class CRIBinarizator;
///////////////////////////////////////////////////////////////////////////////////////////////////
//
class CDezaBinarizator
{
	#define BUFS 3*1000
    #define SB 256
    #define LONGBUF 15L*4096L

public:
	Int32 CloseTrackBin(void);
	Int32 GetBinarized(PWord8 ptbl, Word32 lenbl);
	Word32 OpenTrackBin(PCTBINTigerImageInfo Info, CRIBinarizator * pCaller, Word32 wdezaFlag);
	CDezaBinarizator();
	virtual ~CDezaBinarizator();

private:
	Int32 Get1(void);
	Int32 Read_por_first(void);
	Int32 Elem_st(void);
	Int32 Grey_black(void);
	Int32 Xygo(Int32, Int32);
	Int32 Our1(Int32);
	PWord8 Black_file(PWord8, Int32);

private:
	void Ras1_pred(Int32);
	void Raspred(Int32);
	void Stek(void);
	void Left_str(void);
	void Right_str(void);
	Int32 bWhitePixel;
	Int32 colall;
	Int32 chet_color[SB];
	Word8 lg0;
	PWord8 ptbtek;
	PWord8 ptbosn;
	PWord8 ptb;
	PWord8 ptgrey;
	Word32 indend;
	Word32 indbeg;
	Int32 nstrb;
	Int32 nstr;
	Int32 sy1;
	Int32 sdvig;
	Int32 lg0i;
	Int32 spx2;
	Int32 spx1;
	Int32 spy;
	Int32 spx;
	Int32 bufmark[4];
	Int32 sheds;
	Int32 shed;
	Int32 st[BUFS];
	Int32 ypor;
	Int32 tip;
	Int32 indbl;
	Int32 lgn;
	Int32 lg;
	Int32 xx;
	Int32 y;
	Int32 x;
	Int32 urov[2];
	CRIBinarizator * mpBinarizatorControl;
	//////////////////////////////////////////////////////////////////////
	#define  DB_GREYREAD(a,b,c)    mpBinarizatorControl->KronrodImageRead(a,b,c)
};
/////////////////////////////////////////////////////
//
typedef CDezaBinarizator *PCDezaBinarizator;
///////////////////////////////////////////////////
//
#endif // !defined(_CDEZA_BINARIZATOR_H_)
