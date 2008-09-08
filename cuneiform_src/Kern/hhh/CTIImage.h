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

#ifndef __CIMAGE_H
#define __CIMAGE_H
////////////////////////////////////////////////////////////////////////////////////////////////
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

///////////////////////////////////////////////////////////////////////////////////////////////
#ifndef __GLOBUS_H
 #include "globus.h"
#endif
///////////////////////////////////////////////////////////////////////////////////////////////
//#include"CTIDefines.h"
/*
#ifndef __RECDEFS_H
   #include "recdefs.h"
#endif
   #include "memfunc.h"
*/
///////////////////////////////////////////////////////////////////////////////////////////////
#ifdef __CIMAGE__
  #define CIMAGE_FUNC  FUN_EXPO
#else
  #define CIMAGE_FUNC  FUN_IMPO
#endif
///////////////////////////////////////////////////////////////////////////////////////////////
//#define     CIMAGE_CALLBACK_FUNC(a)         CIMAGE_FUNC(a)
#define     CIMAGE_CALLBACK_FUNC(a)         a
///////////////////////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
	extern "C" {
#endif
///////////////////////////////////////////////////////////////////////////////////////////////
#pragma pack (push,8)
///////////////////////////////////////////////////////////////////////////////////////////////
#define CIMAGE_MAXNAME 260
///////////////////////////////////////////////////////////////////////////////////////////////
#if !defined(Handle) & ! defined(PHandle)
	typedef void *Handle, **PHandle;
#endif
#if defined(Handle) & !defined(PHandle)
	typedef void **PHandle;
#endif
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
# define    CIMAGEBOOL16          Bool16
# define    CIMAGEWORD            Word16
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
#ifndef __CIMAGE_STRUCT_H_
#define __CIMAGE_STRUCT_H_
///////////////////////////////////////////////////////////////////////////////////////////////
#ifndef CIMAGE_CALLBACK
	typedef struct
	{
		Word16    wImageHeight;
		Word16    wImageWidth;
		Word16    wImageByteWidth;
		Word16    wImageDisplacement;
		Word16    wResolutionX;
		Word16    wResolutionY;
		Word8     bFotoMetrics;
		Word8     bUnused;
		Word16    wAddX;
		Word16    wAddY;
	}
	CIMAGE_ImageInfo, *PCIMAGE_ImageInfo, **PPCIMAGE_ImageInfo;

	typedef CIMAGEBOOL16 (*PCIMAGE_Callback_ImageOpen)(PCIMAGE_ImageInfo);
	typedef CIMAGEWORD   (*PCIMAGE_Callback_ImageRead)(PInt8 , CIMAGEWORD);
	typedef CIMAGEBOOL16 (*PCIMAGE_Callback_ImageClose)(void);

	typedef struct
	{
		PCIMAGE_Callback_ImageOpen   CIMAGE_ImageOpen;
		PCIMAGE_Callback_ImageRead   CIMAGE_ImageRead;
		PCIMAGE_Callback_ImageClose  CIMAGE_ImageClose;
	}
	CIMAGEIMAGECALLBACK, * PCIMAGEIMAGECALLBACK;
#else
	#include "puma.h"
	typedef PUMA_ImageInfo CIMAGE_ImageInfo, *PCIMAGE_ImageInfo, **PPCIMAGE_ImageInfo;
	typedef PUMA_CallBack_ImageOpen PCIMAGE_Callback_ImageOpen;
	typedef PUMA_Callback_ImageRead PCIMAGE_Callback_ImageRead;
	typedef PUMA_Callback_ImageClose PCIMAGE_Callback_ImageClose;
	typedef PUMAIMAGECALLBACK CIMAGEIMAGECALLBACK, * PCIMAGEIMAGECALLBACK;
#endif

typedef struct CIMAGEInfoDataInGet
{
	Word32 dwX;
	Word32 dwY;
	Word32 dwWidth;
	Word32 dwHeight;
	Word16 wByteWidth;
	Word8  Reserved;
	Word8  MaskFlag;
}
CIMAGE_InfoDataInGet, * PCIMAGE_InfoDataInGet;

typedef struct CIMAGEInfoDataOutGet
{
	Word32   dwWidth;
	Word32   dwHeight;
	Word16   wByteWidth;
	Word16   byBit;
	Word32   wBlackBit;
	PWord8   lpData;
}
CIMAGE_InfoDataOutGet, * PCIMAGE_InfoDataOutGet, ** PPCIMAGE_InfoDataOutGet;

typedef struct CIMAGEInfoDataInReplace
{
	Word16   byBit;
	Word32   dwX;
	Word32   dwY;
	Word32   dwWidth;
	Word32   dwHeight;
	Word16   wByteWidth;
	PWord8   lpData;
	Word8    Reserved;
	Word16   wReserved;
	Word8    MaskFlag;
}
CIMAGE_InfoDataInReplace, * PCIMAGE_InfoDataInReplace;

typedef struct tagCIMAGERECT
{
	Int32    dwX;
	Int32    dwY;
	Word32   dwWidth;
	Word32   dwHeight;
}
CIMAGE_Rect, *PCIMAGE_Rect, **PPCIMAGE_Rect;

///////////////////////////////////////////////////////////////////////////////////
//
#ifndef _USE_WIN_DIB_

	typedef struct tagCIMAGEBITMAPINFOHEADER
	{
		Word32      biSize;
		Int32       biWidth;
		Int32       biHeight;
		Word16      biPlanes;
		Word16      biBitCount;
		Word32      biCompression;
		Word32      biSizeImage;
		Int32       biXPelsPerMeter;
		Int32       biYPelsPerMeter;
		Word32      biClrUsed;
		Word32      biClrImportant;
	} CIMAGEBITMAPINFOHEADER, *PCIMAGEBITMAPINFOHEADER;

	typedef struct tagCIMAGERGBQUAD
	{
			Word8    rgbBlue;
			Word8    rgbGreen;
			Word8    rgbRed;
			Word8    rgbReserved;
	} CIMAGERGBQUAD, *PCIMAGERGBQUAD;

#else

	#define CIMAGEBITMAPINFOHEADER     BITMAPINFOHEADER
	#define PCIMAGEBITMAPINFOHEADER    PBITMAPINFOHEADER
	#define CIMAGERGBQUAD              RGBQUAD
	#define PCIMAGERGBQUAD             *RGBQUAD

#endif  //_USE_WIN_DIB_

#endif  //__CIMAGE_STRUCT_H_
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
CIMAGE_FUNC(Bool32) CIMAGE_Init(Word16 wHeightCode,Handle hStorage);
CIMAGE_FUNC(Bool32) CIMAGE_Done();
CIMAGE_FUNC(Word32) CIMAGE_GetReturnCode();
CIMAGE_FUNC(Int8 *) CIMAGE_GetReturnString(Word32 dwError);
CIMAGE_FUNC(Bool32) CIMAGE_GetExportData(Word32 dwType, void * pData);
CIMAGE_FUNC(Bool32) CIMAGE_SetImportData(Word32 dwType, void * pData);
/////////////////////////////////////////////////////////////


typedef enum
{
		CIMAGE_FN_WriteCallbackImage = 1,
		CIMAGE_FN_GetCallbackImage,
		CIMAGE_FN_WriteDIB,
		CIMAGE_FN_ReadDIB,
		CIMAGE_FN_GetData,
		CIMAGE_FN_GetDIBData,
		CIMAGE_FN_ReplaceData,
		CIMAGE_FN_GetImageInfo,
		CIMAGE_FN_DeleteImage,
		CIMAGE_FN_FreeCopedDIB,
		CIMAGE_FN_FreeBuffers,
		CIMAGE_FN_Reset,
		CIMAGE_FN_AddReadCloseRects,
		CIMAGE_FN_RemoveReadCloseRects,
		CIMAGE_FN_AddWriteCloseRects,
		CIMAGE_FN_RemoveWriteCloseRects,
		CIMAGE_FN_EnableMask
} CIMAGE_EXPORT_ENTRIES;
/////////////////////////////////////////////////////////////////////////////////////////
#define DEC_FUN(a,b,c) typedef a (*FNCIMAGE##b)c; CIMAGE_FUNC(a) CIMAGE_##b c
//////////////////////////////////////////////////////////////////////////////////////////

DEC_FUN(Bool32,  WriteCallbackImage,   (PWord8 , CIMAGEIMAGECALLBACK));
DEC_FUN(Bool32,  GetCallbackImage,     (PWord8 , PCIMAGEIMAGECALLBACK));
DEC_FUN(Bool32,  WriteDIB,             (PWord8 , Handle, Word32));
DEC_FUN(Bool32,  ReadDIB,              (PWord8 , PHandle, Word32));
DEC_FUN(Bool32,  GetData,              (PWord8 , PCIMAGE_InfoDataInGet, PCIMAGE_InfoDataOutGet));
DEC_FUN(Bool32,  GetDIBData,           (PWord8 , PCIMAGE_InfoDataInGet, PInt8*));
DEC_FUN(Bool32,  ReplaceData,          (PWord8 , PCIMAGE_InfoDataInReplace));
DEC_FUN(Bool32,  GetImageInfo,         (PWord8 , PCIMAGEBITMAPINFOHEADER));
DEC_FUN(Bool32,  DeleteImage,          (PWord8 ));
DEC_FUN(Bool32,  FreeCopedDIB,         (Handle));
DEC_FUN(Bool32,  FreeBuffers,          (void));
DEC_FUN(Bool32,  Reset,                (void));
DEC_FUN(Bool32,  AddReadCloseRects,    (PWord8, Word32, PCIMAGE_Rect));
DEC_FUN(Bool32,  RemoveReadCloseRects, (PWord8, Word32, PCIMAGE_Rect));
DEC_FUN(Bool32,  AddWriteCloseRects,   (PWord8, Word32, PCIMAGE_Rect));
DEC_FUN(Bool32,  RemoveWriteCloseRects,(PWord8, Word32, PCIMAGE_Rect));
DEC_FUN(Bool32,  EnableMask,           (PWord8, PWord8, Bool32));

#undef DEC_FUN
///////////////////////////////////////////////////////////////////////////////////
#define DEC_CB_FUN(a,b,c) typedef a (*FNCIMAGE##b)c; CIMAGE_CALLBACK_FUNC(a) CIMAGE_##b c

DEC_CB_FUN(CIMAGEBOOL16, Callback_ImageOpen,  (PCIMAGE_ImageInfo lpImageInfo));
DEC_CB_FUN(CIMAGEWORD,   Callback_ImageRead,  (PInt8  lpImage, Word16 wMaxSize));
DEC_CB_FUN(CIMAGEBOOL16, Callback_ImageClose, (void));

#undef DEC_CB_FUN


#pragma pack (pop)

#ifdef __cplusplus
            }
#endif

#endif
//////////////////////////////////////////////////////////////////////////////////////////////////////////
// end of file
