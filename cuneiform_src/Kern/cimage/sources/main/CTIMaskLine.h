/*
Copyright (c) 1993-2008, Cognitive Technologies
All rights reserved.

Ğàçğåøàåòñÿ ïîâòîğíîå ğàñïğîñòğàíåíèå è èñïîëüçîâàíèå êàê â âèäå èñõîäíîãî êîäà,
òàê è â äâîè÷íîé ôîğìå, ñ èçìåíåíèÿìè èëè áåç, ïğè ñîáëşäåíèè ñëåäóşùèõ óñëîâèé:

      * Ïğè ïîâòîğíîì ğàñïğîñòğàíåíèè èñõîäíîãî êîäà äîëæíû îñòàâàòüñÿ óêàçàííîå
        âûøå óâåäîìëåíèå îá àâòîğñêîì ïğàâå, ıòîò ñïèñîê óñëîâèé è ïîñëåäóşùèé
        îòêàç îò ãàğàíòèé.
      * Ïğè ïîâòîğíîì ğàñïğîñòğàíåíèè äâîè÷íîãî êîäà â äîêóìåíòàöèè è/èëè â
        äğóãèõ ìàòåğèàëàõ, ïîñòàâëÿåìûõ ïğè ğàñïğîñòğàíåíèè, äîëæíû ñîõğàíÿòüñÿ
        óêàçàííàÿ âûøå èíôîğìàöèÿ îá àâòîğñêîì ïğàâå, ıòîò ñïèñîê óñëîâèé è
        ïîñëåäóşùèé îòêàç îò ãàğàíòèé.
      * Íè íàçâàíèå Cognitive Technologies, íè èìåíà åå ñîòğóäíèêîâ íå ìîãóò
        áûòü èñïîëüçîâàíû â êà÷åñòâå ñğåäñòâà ïîääåğæêè è/èëè ïğîäâèæåíèÿ
        ïğîäóêòîâ, îñíîâàííûõ íà ıòîì ÏÎ, áåç ïğåäâàğèòåëüíîãî ïèñüìåííîãî
        ğàçğåøåíèÿ.

İÒÀ ÏĞÎÃĞÀÌÌÀ ÏĞÅÄÎÑÒÀÂËÅÍÀ ÂËÀÄÅËÜÖÀÌÈ ÀÂÒÎĞÑÊÈÕ ÏĞÀÂ È/ÈËÈ ÄĞÓÃÈÌÈ ËÈÖÀÌÈ "ÊÀÊ
ÎÍÀ ÅÑÒÜ" ÁÅÇ ÊÀÊÎÃÎ-ËÈÁÎ ÂÈÄÀ ÃÀĞÀÍÒÈÉ, ÂÛĞÀÆÅÍÍÛÕ ßÂÍÎ ÈËÈ ÏÎÄĞÀÇÓÌÅÂÀÅÌÛÕ,
ÂÊËŞ×Àß ÃÀĞÀÍÒÈÈ ÊÎÌÌÅĞ×ÅÑÊÎÉ ÖÅÍÍÎÑÒÈ È ÏĞÈÃÎÄÍÎÑÒÈ ÄËß ÊÎÍÊĞÅÒÍÎÉ ÖÅËÈ, ÍÎ ÍÅ
ÎÃĞÀÍÈ×ÈÂÀßÑÜ ÈÌÈ. ÍÈ ÂËÀÄÅËÅÖ ÀÂÒÎĞÑÊÈÕ ÏĞÀÂ È ÍÈ ÎÄÍÎ ÄĞÓÃÎÅ ËÈÖÎ, ÊÎÒÎĞÎÅ
ÌÎÆÅÒ ÈÇÌÅÍßÒÜ È/ÈËÈ ÏÎÂÒÎĞÍÎ ĞÀÑÏĞÎÑÒĞÀÍßÒÜ ÏĞÎÃĞÀÌÌÓ, ÍÈ Â ÊÎÅÌ ÑËÓ×ÀÅ ÍÅ
ÍÅÑ¨Ò ÎÒÂÅÒÑÒÂÅÍÍÎÑÒÈ, ÂÊËŞ×Àß ËŞÁÛÅ ÎÁÙÈÅ, ÑËÓ×ÀÉÍÛÅ, ÑÏÅÖÈÀËÜÍÛÅ ÈËÈ
ÏÎÑËÅÄÎÂÀÂØÈÅ ÓÁÛÒÊÈ, ÑÂßÇÀÍÍÛÅ Ñ ÈÑÏÎËÜÇÎÂÀÍÈÅÌ ÈËÈ ÏÎÍÅÑÅÍÍÛÅ ÂÑËÅÄÑÒÂÈÅ
ÍÅÂÎÇÌÎÆÍÎÑÒÈ ÈÑÏÎËÜÇÎÂÀÍÈß ÏĞÎÃĞÀÌÌÛ (ÂÊËŞ×Àß ÏÎÒÅĞÈ ÄÀÍÍÛÕ, ÈËÈ ÄÀÍÍÛÅ,
ÑÒÀÂØÈÅ ÍÅÃÎÄÍÛÌÈ, ÈËÈ ÓÁÛÒÊÈ È/ÈËÈ ÏÎÒÅĞÈ ÄÎÕÎÄÎÂ, ÏÎÍÅÑÅÍÍÛÅ ÈÇ-ÇÀ ÄÅÉÑÒÂÈÉ
ÒĞÅÒÜÈÕ ËÈÖ È/ÈËÈ ÎÒÊÀÇÀ ÏĞÎÃĞÀÌÌÛ ĞÀÁÎÒÀÒÜ ÑÎÂÌÅÑÒÍÎ Ñ ÄĞÓÃÈÌÈ ÏĞÎÃĞÀÌÌÀÌÈ,
ÍÎ ÍÅ ÎÃĞÀÍÈ×ÈÂÀßÑÜ İÒÈÌÈ ÑËÓ×ÀßÌÈ), ÍÎ ÍÅ ÎÃĞÀÍÈ×ÈÂÀßÑÜ ÈÌÈ, ÄÀÆÅ ÅÑËÈ ÒÀÊÎÉ
ÂËÀÄÅËÅÖ ÈËÈ ÄĞÓÃÎÅ ËÈÖÎ ÁÛËÈ ÈÇÂÅÙÅÍÛ Î ÂÎÇÌÎÆÍÎÑÒÈ ÒÀÊÈÕ ÓÁÛÒÊÎÂ È ÏÎÒÅĞÜ.

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

// CTIMaskLine.h: interface for the CTIMaskLine class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(_CTIMASKLINE_H_)
#define _CTIMASKLINE_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

///////////////////////////////////////////////////////////////////////////
//
#include "resource.h"
#include "ctidefines.h"
#include "ctiimage.h"
#include "ctimemory.h"
#include "ctimasklinesegment.h"
///////////////////////////////////////////////////////////////////////////
class CTIMaskLine;
typedef CTIMaskLine *PCTIMaskLine, **PPCTIMaskLine;
///////////////////////////////////////////////////////////////////////////
class CTIMaskLine
{
public:
	Bool32        IsSegmentOnLine(PCTIMaskLineSegment pSegm){ return (pSegm->GetStart() >= 0 && pSegm->GetStart() <= (Int32)mwLenght && pSegm->GetEnd() <= (Int32)mwLenght); };
	Bool32        RemoveSegment(PCTIMaskLineSegment pSegm);
	Bool32        AddSegment(PCTIMaskLineSegment pSegm);
	PCTIMaskLine  GetNext() { return mpNext; };
	void          SetNext(PCTIMaskLine pLine) { mpNext = pLine; };
	Word32        SetLineNumber( Word32 nLine ) { return mwLine = nLine; };
	Word32        GetLineNumber( void ) { return mwLine; };
	Word32        GetSegmentsNumber( void ) { return mwSegments; };
	Bool32        IsLine(Word32 nLine) { return ((Int32)nLine == mwLine); };
	Bool32        GetLeftIntersection(PCTIMaskLineSegment pcSegm);

public:
	CTIMaskLine(Word32 Lenght, Word32 nLine, PCTIMaskLineSegment pSegm, PCTIMaskLine pcNextLine);
	CTIMaskLine(Word32 Lenght, Word32 nLine, PCTIMaskLineSegment pSegm);
	CTIMaskLine(Word32 Lenght, PCTIMaskLineSegment pSegm);
	CTIMaskLine(Word32 Lenght);
	CTIMaskLine();
	virtual ~CTIMaskLine();

protected:
    Word32 mwLenght;
    Word32 mwSegments;
    PCTIMaskLine mpNext;
	Int32 mwLine;
	CTIMaskLineSegment mcFirst;
private:
	Bool32 CheckSegments(void);
};

#endif // !defined(_CTIMASKLINE_H_)
