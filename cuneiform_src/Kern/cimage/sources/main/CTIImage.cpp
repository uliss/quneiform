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

#define __CIMAGE__

#include "resource.h"
#include "ctidefines.h"
#include "ctiimage.h"
#include "ctimemory.h"

#include "cticontrol.h"

// exteren globals
extern CTIControl * Control_cti;
// extern functions
void SetReturnCode_cimage(Word16 rc);
Word16 GetReturnCode_cimage();

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
CIMAGE_FUNC(Bool32) CIMAGE_WriteCallbackImage(PWord8                 lpName,
											  CIMAGEIMAGECALLBACK  Cbk)
{
	SetReturnCode_cimage(IDS_CIMAGE_ERR_NO);

	if ( Control_cti )
		return Control_cti->WriteCBImage( (PChar8)lpName, Cbk );

	SetReturnCode_cimage(IDS_CIMAGE_DLL_NOT_INITIALISING);
	return FALSE;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
CIMAGE_FUNC(Bool32) CIMAGE_GetCallbackImage(PWord8                  lpName,
											PCIMAGEIMAGECALLBACK  pCbk)
{
	Bool32 A = FALSE;
	SetReturnCode_cimage(IDS_CIMAGE_ERR_NO);

	if ( Control_cti )
		A = Control_cti->GetCBImage( (PChar8)lpName, pCbk );
	else
		SetReturnCode_cimage(IDS_CIMAGE_DLL_NOT_INITIALISING);

	return A;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
CIMAGE_FUNC(Bool32) CIMAGE_WriteDIB(PWord8    lpName,
									Handle  lpDIB,
									Word32  wFlag)
{
	Bool32 A = FALSE;
	SetReturnCode_cimage(IDS_CIMAGE_ERR_NO);

	if ( Control_cti )
		A = Control_cti->SetDIB( (PChar8)lpName, lpDIB, wFlag );
	else
		SetReturnCode_cimage(IDS_CIMAGE_DLL_NOT_INITIALISING);

	return A;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
CIMAGE_FUNC(Bool32) CIMAGE_ReadDIB(PWord8     lpName,
								   PHandle  lplpDIB,
								   Word32   wFlag)
{
	Bool32 A = FALSE;
	SetReturnCode_cimage(IDS_CIMAGE_ERR_NO);

	if ( Control_cti )
		A = Control_cti->GetDIB( (PChar8)lpName, lplpDIB, wFlag );
	else
		SetReturnCode_cimage(IDS_CIMAGE_DLL_NOT_INITIALISING);

	return A;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
CIMAGE_FUNC(Bool32) CIMAGE_GetData(PWord8                     lpName,
								   PCIMAGE_InfoDataInGet       lpIn,
								   PCIMAGE_InfoDataOutGet      lpOut)
{
	Bool32 A = FALSE;
	SetReturnCode_cimage(IDS_CIMAGE_ERR_NO);

	if ( Control_cti )
		A = Control_cti->GetImage( (PChar8)lpName, lpIn, lpOut );
	else
		SetReturnCode_cimage(IDS_CIMAGE_DLL_NOT_INITIALISING);

	return A;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
CIMAGE_FUNC(Bool32) CIMAGE_GetDIBData(PWord8                      lpName,
								      PCIMAGE_InfoDataInGet       lpIn,
								      PInt8                       *lpDIB)
{
	Bool32 A = FALSE;
	SetReturnCode_cimage(IDS_CIMAGE_ERR_NO);

	if ( Control_cti )
		A = Control_cti->GetDIBFromImage( (PChar8)lpName, lpIn, lpDIB );
	else
		SetReturnCode_cimage(IDS_CIMAGE_DLL_NOT_INITIALISING);

	return A;
}/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
CIMAGE_FUNC(Bool32) CIMAGE_ReplaceData(PWord8                        lpName,
									   PCIMAGE_InfoDataInReplace   lpIn )
{
	Bool32 A = FALSE;
	SetReturnCode_cimage(IDS_CIMAGE_ERR_NO);

	if ( Control_cti )
		A = Control_cti->ReplaceImage( (PChar8)lpName, lpIn );
	else
		SetReturnCode_cimage(IDS_CIMAGE_DLL_NOT_INITIALISING);

	return A;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
CIMAGE_FUNC(Bool32) CIMAGE_GetImageInfo(PWord8                     lpName,
										PCIMAGEBITMAPINFOHEADER  lpBIH)
{
	Bool32 A = FALSE;
	SetReturnCode_cimage(IDS_CIMAGE_ERR_NO);

	if ( Control_cti )
		A = Control_cti->GetImageInfo( (PChar8)lpName, lpBIH );
	else
		SetReturnCode_cimage(IDS_CIMAGE_DLL_NOT_INITIALISING);

	return A;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
CIMAGE_FUNC(Bool32) CIMAGE_DeleteImage( PWord8  lpName)
{
	SetReturnCode_cimage(IDS_CIMAGE_ERR_NO);

	if ( Control_cti )
		return Control_cti->RemoveImage( (PChar8)lpName );

	SetReturnCode_cimage(IDS_CIMAGE_DLL_NOT_INITIALISING);
	return FALSE;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
CIMAGE_FUNC(Bool32) CIMAGE_FreeCopedDIB(Handle hDIB)
{
	SetReturnCode_cimage(IDS_CIMAGE_ERR_NO);

	if ( Control_cti )
		return Control_cti->FreeAlloced( hDIB );

	SetReturnCode_cimage(IDS_CIMAGE_DLL_NOT_INITIALISING);
	return FALSE;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
CIMAGE_FUNC(Bool32) CIMAGE_FreeBuffers()
{
	SetReturnCode_cimage(IDS_CIMAGE_ERR_NO);

	if ( Control_cti )
		return Control_cti->FreeBuffers();

	SetReturnCode_cimage(IDS_CIMAGE_DLL_NOT_INITIALISING);
	return FALSE;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
CIMAGE_FUNC(Bool32) CIMAGE_Reset(void)
{
	SetReturnCode_cimage(IDS_CIMAGE_ERR_NO);

	if ( Control_cti )
	{
		delete Control_cti;

		Control_cti = new CTIControl;

		if ( Control_cti )
			return TRUE;
		return FALSE;
	}

	SetReturnCode_cimage(IDS_CIMAGE_DLL_NOT_INITIALISING);
	return FALSE;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
CIMAGE_FUNC(Bool32) CIMAGE_AddReadCloseRects(PWord8 lpName, Word32 wCount, PCIMAGE_Rect pFirst)
{
	SetReturnCode_cimage(IDS_CIMAGE_ERR_NO);

	if ( Control_cti )
		return Control_cti->AddReadRectangles( (PChar8)lpName, wCount, pFirst );

	SetReturnCode_cimage(IDS_CIMAGE_DLL_NOT_INITIALISING);
	return FALSE;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
CIMAGE_FUNC(Bool32) CIMAGE_RemoveReadCloseRects(PWord8 lpName, Word32 wCount, PCIMAGE_Rect pFirst)
{
	SetReturnCode_cimage(IDS_CIMAGE_ERR_NO);

	if ( Control_cti )
		return Control_cti->RemoveReadRectangles( (PChar8)lpName, wCount, pFirst );

	SetReturnCode_cimage(IDS_CIMAGE_DLL_NOT_INITIALISING);
	return FALSE;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
CIMAGE_FUNC(Bool32) CIMAGE_AddWriteCloseRects(PWord8 lpName, Word32 wCount, PCIMAGE_Rect pFirst)
{
	SetReturnCode_cimage(IDS_CIMAGE_ERR_NO);

	if ( Control_cti )
		return Control_cti->AddWriteRectangles( (PChar8)lpName, wCount, pFirst );

	SetReturnCode_cimage(IDS_CIMAGE_DLL_NOT_INITIALISING);
	return FALSE;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
CIMAGE_FUNC(Bool32) CIMAGE_RemoveWriteCloseRects(PWord8 lpName, Word32 wCount, PCIMAGE_Rect pFirst)
{
	SetReturnCode_cimage(IDS_CIMAGE_ERR_NO);

	if ( Control_cti )
		return Control_cti->RemoveWriteRectangles( (PChar8)lpName, wCount, pFirst );

	SetReturnCode_cimage(IDS_CIMAGE_DLL_NOT_INITIALISING);
	return FALSE;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
CIMAGE_FUNC(Bool32) CIMAGE_EnableMask(PWord8 lpName, PWord8 lpType, Bool32 bEnabler)
{
	SetReturnCode_cimage(IDS_CIMAGE_ERR_NO);

	if ( Control_cti )
		return Control_cti->EnableMask((PChar8)lpName, (PChar8)lpType, bEnabler);

	SetReturnCode_cimage(IDS_CIMAGE_DLL_NOT_INITIALISING);
	return FALSE;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
// For GetCBImage
CIMAGE_CALLBACK_FUNC(CIMAGEBOOL16) CIMAGE_Callback_ImageOpen(PCIMAGE_ImageInfo lpImageInfo)
{
	Bool16 A = FALSE;

	if ( !Control_cti )
	{
		SetReturnCode_cimage(IDS_CIMAGE_DLL_NOT_INITIALISING);
		return FALSE;
	}

	A = (CIMAGEBOOL16)(Control_cti->CBImageOpen(lpImageInfo));

	return A;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
CIMAGE_CALLBACK_FUNC(CIMAGEWORD)   CIMAGE_Callback_ImageRead(PInt8  lpImage, Word16 wMaxSize)
{
	if ( !Control_cti )
	{
		SetReturnCode_cimage(IDS_CIMAGE_DLL_NOT_INITIALISING);
		return 0;
	}

	return (CIMAGEWORD)(Control_cti->CBImageRead((PChar8)lpImage, wMaxSize));
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
CIMAGE_CALLBACK_FUNC(CIMAGEBOOL16) CIMAGE_Callback_ImageClose(void)
{
	if ( !Control_cti )
	{
		SetReturnCode_cimage(IDS_CIMAGE_DLL_NOT_INITIALISING);
		return FALSE;
	}

	return (CIMAGEBOOL16)(Control_cti->CBImageClose());
}
////////////////////////////////////////////////////////////////////////////////////////////////////
// end of file
