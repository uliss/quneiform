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


# ifndef __CRI_CONTROL_H_
# define __CRI_CONTROL_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

# if defined (_DEBUG)
# pragma message( __FILE__" : Image processing")
# endif // (_DEBUG)


#include "resource.h"
#include "cridefines.h"
#include "cttypext.h"
#include "criimage.h"
#include "ctiimage.h"
#include "crinvertor.h"	// Added by ClassView
#include "ctdib.h"	// Added by ClassView
#include "rprogressor.h"	// Added by ClassView
#include "cribinarizator.h"
#include "crturner.h"
#include "crinvertor.h"
#include "crrotator.h"
#include "cttypes.h"	// Added by ClassView

class CRIControl
{
private:
	Handle           mhOpenedDIB;
	Handle           mhCreatedDIB;
	// pointer to class CTDIB
	PCTDIB           mpSourceDIB;
	PCTDIB           mpDestinationDIB;


protected:
	// Name of last processed image
	Char8                     mcLastDIBName[256];
	//
	void *                    mp_TurnedDIB;
	// pointer to class ProgressShow
	CRProgressor              mcProgress;
	// pointer to class Binarizator
	PCRIBinarizator           mpBinarizator;
	// pointer to clas  Invertor
	PCRInvertor               mpInvertor;
	// pointer to class Turn
	PCRTurner                 mpTurner;
	// pointer to class Rotate
	PCRRotator                mpRotator;

public:
	CRIControl();
	~CRIControl();

private:
	Bool32          DIBOpeningType;
	RIMAGEMARGINS   mrMargins;
	Bool32          mbMarginsFlag;

private:
	Bool32          WriteDIBtoBMP(const char *cName, PCTDIB pDIB);
	Bool32          GetDIB(PChar8   cDIB, PHandle phDIB);
	Bool32          CloseSourceDIB();
	Bool32          CreateDestinatonDIB(Word32 BitCount);
	Bool32          SetDestinationDIBtoStorage(PChar8  cDIBName);
	Bool32          OpenDestinationDIBfromSource(PChar8  cSDIB);
	Bool32          CloseDestinationDIB(PChar8   cDIBName);
	Bool32          OpenSourceDIB(PChar8   cDIBName);
	Bool32          SetDIB(PChar8   cDIB, Handle hDIB);
	Bool32          WriteDIB(PChar8   cDIB, Handle hDIB);
	Bool32          ReadDIB(PChar8   cDIB, PHandle phDIB);

public:
	Bool32                    RotatePoint(PChar8 cDIB, Int32 iX, Int32 iY, PInt32 prX, PInt32 prY);
	Bool32                    StartProgress(void);
	Bool32                    SetProgressCallBacks(PRIMAGECBPRogressStart pcbStart, PRIMAGECBPRogressStep pcbStep, PRIMAGECBPRogressFinish pcbFinish);
	Bool32                    SetMargins(PRIMAGEMARGINS pMargins);
	Bool32                    Binarise(PChar8   cDIBIn, PChar8   cDIBOut, Word32 wFlag, Word32 UseMargins);
	Bool32                    Rotate(PChar8   cDIBIn, PChar8   cDIBOut, Int32 High, Int32 Low, Word32 UseMargins);
	Bool32                    Roll(PChar8 cDIBIn, PChar8 cDIBOut, Int32 Num, Int32 Denum, Word32 bUseMargins);
	Bool32                    Turn(PChar8   cDIBIn, PChar8   cDIBOut, Word32 wFlag, Word32 UseMargins);
	Bool32                    Inverse(PChar8   cDIBIn, PChar8   cDIBOut, Word32 UseMargins);
};
# endif    //__CRI_CONTROL_H_
//////////////////////////////////////////////////////////////////////////////////////////////////////////
// end of file
