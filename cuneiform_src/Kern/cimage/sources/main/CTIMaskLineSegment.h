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

// CTIMaskLineSegment.h: interface for the CTIMaskLineSegment class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(_CTIMASKLINESEGMENT_H_)
#define _CTIMASKLINESEGMENT_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

# if defined (_DEBUG)
# pragma message( __FILE__" : Image mask segment")
# endif // (_DEBUG)
////////////////////////////////////////////////////////////////////////////////////
#include "resource.h"
#include "ctidefines.h"
#include "ctiimage.h"
#include "ctimemory.h"
////////////////////////////////////////////////////////////////////////////////////
class CTIMaskLineSegment;
typedef   CTIMaskLineSegment         *PCTIMaskLineSegment,   **PPCTIMaskLineSegment;
/////////////////////////////////////////////////////////////////////////////////////
class CTIMaskLineSegment  
{
//#define                   CTIMLSEGMINTERSECTNO                  0
#define                   CTIMLSEGMINTERSECTIN                  1
#define                   CTIMLSEGMINTERSECTLEFT                2
#define                   CTIMLSEGMINTERSECTRIGHT               3
#define                   CTIMLSEGMINTERSECTOVER                4
#define                   CTIMLSEGMINTERSECTEQUAL               5
#define                   CTIMLSEGMINTERSECTFULLLEFT            6
#define                   CTIMLSEGMINTERSECTFULLRIGHT           7
#define                   CTIMLSEGMPOINTRIGHT                   1
#define                   CTIMLSEGMPOINTLEF                    -1
#define                   CTIMLSEGMPOINTIN                      0

public:
	void                  SetNext(PCTIMaskLineSegment pNext) { mpNext = pNext; };
	PCTIMaskLineSegment   GetNext(void) { return mpNext; };
	Int32                 GetStart(void) { return mwStart; };
	Int32                 GetEnd(void) { return mwEnd; };
	Bool32                IsPointInSegment(Int32 X) {return (X >= mwStart && X <= mwEnd);};
	Bool32                CutRightTo(PCTIMaskLineSegment pSegm);
	// îáðåçàòü ñ êîíöà
	Bool32                CutLeftTo(PCTIMaskLineSegment pSegm);
	// îáðåçàòü ñ íà÷àëà
	Bool32                AddWith(PCTIMaskLineSegment pSegm);
	// ïåðåñå÷åíèå äàííîãî ñåãìåíòà ñ àðãóìåíòîì
	Bool32                IntersectWith(PCTIMaskLineSegment pSegm);
	// ïîëîæåíèå äàííîãî ñåãìåíòà îòíîñèòåëüíî àðãóìåíòà
	Word32                IsIntersectWith(PCTIMaskLineSegment pSegm);
	// ðàâåíñòâî ñåãìåíòîâ
	Bool32                IsEqual(PCTIMaskLineSegment pSegm) { return (mwEnd == pSegm->GetEnd() && mwStart == pSegm->GetStart()); };

public:
	Int32 GetPointDirect(Word32 X);
	CTIMaskLineSegment();
	CTIMaskLineSegment(Int32 Start, Int32 End);
	CTIMaskLineSegment(PCTIMaskLineSegment pSegm);
	virtual ~CTIMaskLineSegment();
	
protected:
	PCTIMaskLineSegment mpNext;
	Int32 mwEnd;
	Int32 mwStart;
};

#endif // !defined(_CTIMASKLINESEGMENT_H_)
