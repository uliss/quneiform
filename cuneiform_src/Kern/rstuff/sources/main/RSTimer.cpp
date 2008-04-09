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

/**********  Çàãîëîâîê  *******************************************************/
/*  Àâòîð,                                                                    */
/*  êîììåíòàðèè                                                               */
/*  è äàëíåéøàÿ                                                               */
/*  ïðàâêà     :  Àëåêñåé Êîíîïëåâ                                            */
/*  Ðåäàêöèÿ   :  08.06.00                                                    */
/*  Ôàéë       :  'RSTimer.cpp'                                               */
/*  Ñîäåðæàíèå :  Êàëáýêè puma.dll äëÿ ïðèâÿçêè âðåìåíè è îòëàä÷èêà           */
/*  Íàçíà÷åíèå :                                                              */
/*----------------------------------------------------------------------------*/
#include "RStuff.h"
#include "RSGlobalData.h"
#include "RSDefines.h"
#include "RSFunc.h"
//////////////////////////////////////////////////////////////////////////////////////
//
Bool32 SetCBProgressPoints(void * pData)
{
	PRSCBProgressPoints pPoints = (PRSCBProgressPoints)pData;

#define SET_CB_POINTS(a,b)  ProgressPoints.p##b = a->p##b 
	SET_CB_POINTS(pPoints, ProgressStart);
	SET_CB_POINTS(pPoints, ProgressFinish);
	SET_CB_POINTS(pPoints, RestorePRGTIME);
	SET_CB_POINTS(pPoints, SetUpdate);
	SET_CB_POINTS(pPoints, ProgressStep);
	SET_CB_POINTS(pPoints, ProgressStepLines);
	SET_CB_POINTS(pPoints, ProgressStepTables);
	SET_CB_POINTS(pPoints, InitPRGTIME);
	SET_CB_POINTS(pPoints, StorePRGTIME);
	SET_CB_POINTS(pPoints, DonePRGTIME);
	SET_CB_POINTS(pPoints, DPumaSkipComponent);
	SET_CB_POINTS(pPoints, DPumaSkipTurn);
	SET_CB_POINTS(pPoints, rexcProgressStep);
	SET_CB_POINTS(pPoints, GetModulePath);
	//SET_CB_POINTS(pPoints,                   );
#undef SET_CB_POINTS
	return true;
}
#define DEF_CB_FUNC(a,b,c,d,e)       a b##c \
{\
	DEC_CB_TYPE(b) pfFunc; \
	a ret = e ; \
	if ( ProgressPoints.p##b ) \
	{ \
		pfFunc = ( DEC_CB_TYPE(b) )ProgressPoints.p##b; \
		return pfFunc##d ; \
	} \
	return ret; \
}
#define DEF_CB_VOID_FUNC(b,c,d)       void b##c \
{ \
	DEC_CB_TYPE(b) pfFunc; \
	if ( ProgressPoints.p##b ) \
	{ \
		pfFunc = ( DEC_CB_TYPE(b) )ProgressPoints.p##b; \
		pfFunc##d; \
	}\
}

DEF_CB_VOID_FUNC(ProgressStart,   (void),() )
DEF_CB_VOID_FUNC(ProgressFinish,  (void),() )
DEF_CB_VOID_FUNC(RestorePRGTIME,  (PRGTIME	prev),(prev) )
DEF_CB_VOID_FUNC(SetUpdate,       (Word32 flgAdd,Word32 flgRemove),(flgAdd, flgRemove) )
DEF_CB_FUNC(Bool32,  ProgressStep,       (Word32 step, Word32 percent), (step, percent), FALSE  )
DEF_CB_FUNC(Bool32,  ProgressStepLines,  (Word32 step, Word32 percent), (step, percent), FALSE  )
DEF_CB_FUNC(Bool32,  ProgressStepTables, (Word32 step, Word32 percent), (step, percent), FALSE  )
DEF_CB_FUNC(Bool32,  InitPRGTIME,        (void), (), FALSE  )
DEF_CB_FUNC(PRGTIME, StorePRGTIME,       (Word32 beg, Word32 end), (beg, end), {0}  )
DEF_CB_FUNC(Bool32,  DonePRGTIME,        (void), (), FALSE  )
DEF_CB_FUNC(Bool32,  DPumaSkipComponent, (void), (), FALSE  )
DEF_CB_FUNC(Bool32,  DPumaSkipTurn,      (void), (), FALSE  )
DEF_CB_FUNC(Bool32,  rexcProgressStep,   (Word32 step), (step), FALSE  )
DEF_CB_FUNC(char *,  GetModulePath,      (void), (), NULL  )

#undef DEF_CB_FUNC
#undef DEF_CB_VOID_FUNC
//////////////////////////////////////////////////////////////////////////////////////
//end of file