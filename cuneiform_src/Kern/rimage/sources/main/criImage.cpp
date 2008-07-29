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

#define __RIMAGE__

#include "resource.h"
#include "criimage.h"
#include "crimemory.h"
#include "cridefines.h"

#include "cricontrol.h"

// exteren globals
extern CRIControl * Control_cri;
// extern functions
void SetReturnCode_rimage(Word16 rc);
Word16 GetReturnCode_rimage();

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
RIMAGE_FUNC(Bool32) RIMAGE_SetMargins(PRIMAGEMARGINS pMargins)
{
	SetReturnCode_rimage(IDS_RIMAGE_ERR_NO);

	if ( Control_cri )
		return Control_cri->SetMargins( pMargins );

	SetReturnCode_rimage(IDS_RIMAGE_DLL_NOT_INITIALISING);
	return FALSE;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
RIMAGE_FUNC(Bool32) RIMAGE_SetProgressCB(PRIMAGECBPRogressStart pStart, PRIMAGECBPRogressStep pStep, PRIMAGECBPRogressFinish pFinish)
{
	SetReturnCode_rimage(IDS_RIMAGE_ERR_NO);

	if ( Control_cri )
		return Control_cri->SetProgressCallBacks( pStart, pStep, pFinish );

	SetReturnCode_rimage(IDS_RIMAGE_DLL_NOT_INITIALISING);
	return FALSE;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
RIMAGE_FUNC(Bool32) RIMAGE_Binarise(PWord8 cDIBIn, PWord8 cDIBOut, Word32 wFlag, Word32 UseMargins)
{
	SetReturnCode_rimage(IDS_RIMAGE_ERR_NO);

	if ( Control_cri )
		return Control_cri->Binarise( (PChar8)cDIBIn, (PChar8)cDIBOut, wFlag, UseMargins);

	SetReturnCode_rimage(IDS_RIMAGE_DLL_NOT_INITIALISING);
	return FALSE;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
RIMAGE_FUNC(Bool32) RIMAGE_Rotate(PWord8 cDIBIn, PWord8 cDIBOut, Int32 High, Int32 Low, Word32 UseMargins)
{
	SetReturnCode_rimage(IDS_RIMAGE_ERR_NO);

	if ( Control_cri )
		return Control_cri->Rotate( (PChar8)cDIBIn, (PChar8)cDIBOut, High, Low, UseMargins);

	SetReturnCode_rimage(IDS_RIMAGE_DLL_NOT_INITIALISING);
	return FALSE;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
RIMAGE_FUNC(Bool32) RIMAGE_RotatePoint( PWord8 cDIBIn, Int32 wX, Int32 wY, PInt32 prX, PInt32 prY)
{
	SetReturnCode_rimage(IDS_RIMAGE_ERR_NO);

	if ( Control_cri )
		return Control_cri->RotatePoint( (PChar8)cDIBIn, wX, wY, prX, prY );

	SetReturnCode_rimage(IDS_RIMAGE_DLL_NOT_INITIALISING);
	return FALSE;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
RIMAGE_FUNC(Bool32) RIMAGE_Roll(PWord8 cDIBIn, PWord8 cDIBOut, Int32 High, Int32 Low, Word32 UseMargins)
{
	SetReturnCode_rimage(IDS_RIMAGE_ERR_NO);

	if ( Control_cri )
		return Control_cri->Roll( (PChar8)cDIBIn, (PChar8)cDIBOut, High, Low, UseMargins);

	SetReturnCode_rimage(IDS_RIMAGE_DLL_NOT_INITIALISING);
	return FALSE;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
RIMAGE_FUNC(Bool32) RIMAGE_Turn(PWord8 cDIBIn, PWord8 cDIBOut, Word32 wFlag, Word32 UseMargins)
{
	SetReturnCode_rimage(IDS_RIMAGE_ERR_NO);

	if ( Control_cri )
		return Control_cri->Turn( (PChar8)cDIBIn, (PChar8)cDIBOut, wFlag, UseMargins);

	SetReturnCode_rimage(IDS_RIMAGE_DLL_NOT_INITIALISING);
	return FALSE;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
RIMAGE_FUNC(Bool32) RIMAGE_Inverse(PWord8 cDIBIn, PWord8 cDIBOut, Word32 UseMargins)
{
	SetReturnCode_rimage(IDS_RIMAGE_ERR_NO);

	if ( Control_cri )
		return Control_cri->Inverse( (PChar8)cDIBIn, (PChar8)cDIBOut, UseMargins);

	SetReturnCode_rimage(IDS_RIMAGE_DLL_NOT_INITIALISING);
	return FALSE;
}
////////////////////////////////////////////////////////////////////////////////////////////////////
// end of file
