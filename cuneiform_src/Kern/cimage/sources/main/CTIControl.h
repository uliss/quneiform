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


# ifndef __CTI_CONTROL_H_
# define __CTI_CONTROL_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//////////////////////////////////////////
#include "resource.h"
#include "ctidefines.h"
#include "ctiimage.h"
#include "ctimemory.h"
#include "ctdib.h"
#include "ctimask.h"
#include "ctiimagelist.h"
#include "ctiimageheader.h"

#include "minmax.h"

//#define CIMAGE_CBR_ONE_LINE

class CTIControl
{
protected:
	CTIImageList             mlImages;
	Handle                   hCBImage;
	void *                   pCBImage;
	char *                   pCBBuffer;
	Word32                   wCBBufferSize;
	Word32                   wCBLine;
	Word32                   wCBWidth;
	Word32                   wCBLines;
	Word32                   wCBStep;
	Char8                    mCBName[CIMAGE_MAX_IMAGE_NAME];
	Char8                    mCBWName[CIMAGE_MAX_IMAGE_NAME];
	Handle                   mhBitFildFromImage;
	PWord8                   mpBitFildFromImage;
	PCTDIB                   mpDIBFromImage;

public:
	CTIControl();
	~CTIControl();

private:
	Bool32                    ApplayMaskToDIBLine(PCTDIB pcDIB, PCTIMaskLineSegment pSegm, Word32 wLine, Word32 wAtX, Word32 wAtY);
	Bool32                    ApplayMaskToDIB(PCTDIB pDIB, PCTIMask pMask, Word32 wAtX = 0, Word32 wAtY = 0);
	Bool32                    RemoveRectsFromMask(const char *lpName, Word32 wNumber, PCIMAGE_Rect pFirstRect, const char*pcType);
	Bool32                    AddRectsToMask(const char *lpName, Word32 wNumber, PCIMAGE_Rect pFirstRect, const char *pcType);
	Bool32                    OpenDIBFromList(const char *lpName, PCTDIB pDIB);
	Bool32                    SetMaskToList(PChar8 pName, PCTIMask pMask, PChar8 pcType);
	Bool32                    OpenMaskFromList(const char *lpName, PPCTIMask ppMask, PBool32 pEnMask, const char *pcType);
	Bool32                    OpenDIBFromList(PChar8 lpName, PHandle phImage);
	Bool32                    WriteDIBtoBMP(const char *cName, PCTDIB pDIB);
	Bool32                    SetFrame(PCTDIB pSrcDIB, PCTDIB pDscDIB, PCIMAGE_InfoDataInReplace pIn);
	Bool32                    ApplayBitMaskToFrame(PCIMAGE_InfoDataInGet pIn, PCIMAGE_InfoDataOutGet pOut);
	Bool32                    ApplayBitMaskToDIB(PWord8 pMask, PCTDIB pDIB);
	Bool32                    CopyFromFrame(PCTDIB pSrcDIB, PCTDIB pDscDIB, PCIMAGE_InfoDataInReplace pFrameIn);
	Bool32                    CopyToFrame(PCTDIB pSrcDIB, PCTDIB pDscDIB, PCIMAGE_InfoDataInGet pFrameInfo, PWord8 pMask);
	Bool32                    GetFrame(PCTDIB pSrcDIB, PCTDIB pDscDIB, PCIMAGE_InfoDataInGet pIn, PWord8 pMask);
	Bool32                    CopyDIB(Handle hDIB, PHandle hCopyedDib);
	Bool32                    DumpToFile(PChar8 FileName, PWord8 pData, Word32 Size);
	Bool32                    CheckInData(PCTDIB pDIB, PCIMAGE_InfoDataInGet lpIn, PCIMAGE_InfoDataInGet lpNewIn = NULL);

public:
	Bool32 EnableMask(PChar8 pcName, PChar8 pcType, Bool32 bEnable);
	Bool32                    RemoveReadRectangles(PChar8 lpName, Word32 wNumber, PCIMAGE_Rect pFirst);
	Bool32                    AddReadRectangles(PChar8 lpName, Word32 wNumber, PCIMAGE_Rect pFirst);
	Bool32                    RemoveWriteRectangles(PChar8 lpName, Word32 wNumber, PCIMAGE_Rect pFirst);
	Bool32                    AddWriteRectangles(PChar8 lpName, Word32 wNumber, PCIMAGE_Rect pFirst);
	Bool32                    FreeBuffers(void);
	Bool32                    FreeAlloced(Handle hDIB);
	Bool32                    GetDIBFromImage(PChar8 lpName, PCIMAGE_InfoDataInGet lpIn, PInt8 *pDIB);
	Bool32                    RemoveImage(PChar8  lpName);
	Bool32                    GetImageInfo(PChar8  lpImage, PCIMAGEBITMAPINFOHEADER lpBIH);
	Bool32                    ReplaceImage(PChar8  lpName, PCIMAGE_InfoDataInReplace lpIn);
	Bool32                    GetImage(PChar8  lpName, PCIMAGE_InfoDataInGet lpIn, PCIMAGE_InfoDataOutGet lplpOut);
	Bool32                    GetDIB(PChar8  lpName, PHandle phDIB, Word32 wFlag = 0);
	Bool32                    SetDIB(PChar8  lpName, Handle hDIB, Word32 wFlag = 0);
	Bool32                    GetCBImage (PChar8  lpName, PCIMAGEIMAGECALLBACK pCbk);
	Bool32                    WriteCBImage(PChar8  lpName, CIMAGEIMAGECALLBACK Cbk );
	Bool32                    CBImageOpen(PCIMAGE_ImageInfo lpImageInfo);
	Bool32                    CBImageClose(void);
	Word32                    CBImageRead(PChar8  lpName, Word32 wMaxSize);
	Bool32                    CloseDIBFromList(PCTDIB pDIB);

protected:
	Handle mhCopyedDIB;
	Bool32 mbWriteFlag;
	Word32                    mwMemoryErrors;
	Word8                     mwLAWhiteRightMask[8];
	Word8                     mwLAWhiteLeftMask[8];
	Word8                     mwLABlackRightMask[8];
	Word8                     mwLABlackLeftMask[8];
	Word8                     mwIndexMask[8];
	Bool32                    mbSourceDIBCopy;
	PCTIMask                  mpcSrcDIBReadMask;
	PCTIMask                  mpcSrcDIBWriteMask;
	Bool32                    mbEnableDIBReadMask;
	Bool32                    mbEnableDIBWriteMask;
	PCTDIB                    mCBDestianationDIB;
	PCTDIB                    mCBSourceDIB;
	PCTDIB                    mCBWDestianationDIB;
	PCTDIB                    mCBWSourceDIB;
	Bool32                    mCBWInProcess;

};
# endif    //__CTI_CONTROL_H_
//////////////////////////////////////////////////////////////////////////////////////////////////////////
// end of file
