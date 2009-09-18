/*
Copyright (c) 1993-2008, Cognitive Technologies
All rights reserved.

Разрешается повторное распространение и использование как в виде исходного кода,
так и в двоичной форме, с изменениями или без, при соблюдении следующих условий:

      * При повторном распространении исходного кода должны оставаться указанное
        выше уведомление об авторском праве, этот список условий и последующий
        отказ от гарантий.
      * При повторном распространении двоичного кода в документации и/или в
        других материалах, поставляемых при распространении, должны сохраняться
        указанная выше информация об авторском праве, этот список условий и
        последующий отказ от гарантий.
      * Ни название Cognitive Technologies, ни имена ее сотрудников не могут
        быть использованы в качестве средства поддержки и/или продвижения
        продуктов, основанных на этом ПО, без предварительного письменного
        разрешения.

ЭТА ПРОГРАММА ПРЕДОСТАВЛЕНА ВЛАДЕЛЬЦАМИ АВТОРСКИХ ПРАВ И/ИЛИ ДРУГИМИ ЛИЦАМИ "КАК
ОНА ЕСТЬ" БЕЗ КАКОГО-ЛИБО ВИДА ГАРАНТИЙ, ВЫРАЖЕННЫХ ЯВНО ИЛИ ПОДРАЗУМЕВАЕМЫХ,
ВКЛЮЧАЯ ГАРАНТИИ КОММЕРЧЕСКОЙ ЦЕННОСТИ И ПРИГОДНОСТИ ДЛЯ КОНКРЕТНОЙ ЦЕЛИ, НО НЕ
ОГРАНИЧИВАЯСЬ ИМИ. НИ ВЛАДЕЛЕЦ АВТОРСКИХ ПРАВ И НИ ОДНО ДРУГОЕ ЛИЦО, КОТОРОЕ
МОЖЕТ ИЗМЕНЯТЬ И/ИЛИ ПОВТОРНО РАСПРОСТРАНЯТЬ ПРОГРАММУ, НИ В КОЕМ СЛУЧАЕ НЕ
НЕСЁТ ОТВЕТСТВЕННОСТИ, ВКЛЮЧАЯ ЛЮБЫЕ ОБЩИЕ, СЛУЧАЙНЫЕ, СПЕЦИАЛЬНЫЕ ИЛИ
ПОСЛЕДОВАВШИЕ УБЫТКИ, СВЯЗАННЫЕ С ИСПОЛЬЗОВАНИЕМ ИЛИ ПОНЕСЕННЫЕ ВСЛЕДСТВИЕ
НЕВОЗМОЖНОСТИ ИСПОЛЬЗОВАНИЯ ПРОГРАММЫ (ВКЛЮЧАЯ ПОТЕРИ ДАННЫХ, ИЛИ ДАННЫЕ,
СТАВШИЕ НЕГОДНЫМИ, ИЛИ УБЫТКИ И/ИЛИ ПОТЕРИ ДОХОДОВ, ПОНЕСЕННЫЕ ИЗ-ЗА ДЕЙСТВИЙ
ТРЕТЬИХ ЛИЦ И/ИЛИ ОТКАЗА ПРОГРАММЫ РАБОТАТЬ СОВМЕСТНО С ДРУГИМИ ПРОГРАММАМИ,
НО НЕ ОГРАНИЧИВАЯСЬ ЭТИМИ СЛУЧАЯМИ), НО НЕ ОГРАНИЧИВАЯСЬ ИМИ, ДАЖЕ ЕСЛИ ТАКОЙ
ВЛАДЕЛЕЦ ИЛИ ДРУГОЕ ЛИЦО БЫЛИ ИЗВЕЩЕНЫ О ВОЗМОЖНОСТИ ТАКИХ УБЫТКОВ И ПОТЕРЬ.

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

// CTDIB.h: interface for the CTDIB class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(_CTDIB_H_)
#define _CTDIB_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "globus.h"
#include "cttypext.h"
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef __RDIB__
  #define RDIB_FUNC  FUN_EXPO
#else
  #define RDIB_FUNC  FUN_IMPO
#endif
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
#if defined(WIN32) & defined(CTDIB_USE_WIN32_API)
  #include <wingdi.h>
  typedef BITMAPINFOHEADER CTDIBBITMAPINFOHEADER, *PCTDIBBITMAPINFOHEADER, **PPCTDIBBITMAPINFOHEADER;
  typedef BITMAPV4HEADER CTDIBBITMAPV4HEADER, *PCTDIBBITMAPV4HEADER, **PPCTDIBBITMAPV4HEADER;
  typedef BITMAPV5HEADER CTDIBBITMAPV5HEADER, *PCTDIBBITMAPV5HEADER, **PPCTDIBBITMAPV5HEADER;
  #define CTDIB_BI_JPEG    BI_JPEG
#else
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// DIB version 3 header (lenght - 40 bytes)
typedef struct tagCTDIBBITMAPINFOHEADER
{ // bmih
	Word32        biSize;
    Int32         biWidth;
	Int32         biHeight;
	Word16        biPlanes;
    Word16        biBitCount;
    Word32        biCompression;
    Word32        biSizeImage;
    Int32         biXPelsPerMeter;
	Int32         biYPelsPerMeter;
	Word32        biClrUsed;
    Word32        biClrImportant;
} CTDIBBITMAPINFOHEADER, *PCTDIBBITMAPINFOHEADER, **PPCTDIBBITMAPINFOHEADER;
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
typedef Int32     CTDIBPOINTCOORDINATE, *PCTDIBPOINTCOORDINATE;
typedef struct tagCTDIBCIEXYZ
{
        CTDIBPOINTCOORDINATE ciexyzX;
        CTDIBPOINTCOORDINATE ciexyzY;
        CTDIBPOINTCOORDINATE ciexyzZ;
} CTDIBCIEXYZ, *PCTDIBCIEXYZ;
typedef struct tagCTDIBICEXYZTRIPLE
{
        CTDIBCIEXYZ  ciexyzRed;
        CTDIBCIEXYZ  ciexyzGreen;
        CTDIBCIEXYZ  ciexyzBlue;
} CTDIBICEXYZTRIPLE, *PCTDIBICEXYZTRIPLE;
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// DIB version 4 header (lenght - 108 bytes)
typedef struct tagCTDIBBITMAPV4HEADER
{   Word32             bV4Size;
	Int32              bV4Width;
    Int32              bV4Height;
	Word16             bV4Planes;
    Word16             bV4BitCount;
	Word32             bV4V4Compression;
    Word32             bV4SizeImage;
	Int32              bV4XPelsPerMeter;
    Int32              bV4YPelsPerMeter;
	Word32             bV4ClrUsed;
    Word32             bV4ClrImportant;
	Word32             bV4RedMask;
    Word32             bV4GreenMask;
	Word32             bV4BlueMask;
    Word32             bV4AlphaMask;
	Word32             bV4CSType;
    CTDIBICEXYZTRIPLE  bV4Endpoints;
	Word32             bV4GammaRed;
    Word32             bV4GammaGreen;
	Word32             bV4GammaBlue;
} CTDIBBITMAPV4HEADER, *PCTDIBBITMAPV4HEADER, **PPCTDIBBITMAPV4HEADER;
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// DIB version 5 header (lenght - 124 bytes)
typedef struct tagCTDIBBITMAPV5HEADER
{
	Word32             bV5Size;
	Int32              bV5Width;
    Int32              bV5Height;
	Word16             bV5Planes;
    Word16             bV5BitCount;
	Word32             bV5Compression;
    Word32             bV5SizeImage;
	Int32              bV5XPelsPerMeter;
    Int32              bV5YPelsPerMeter;
	Word32             bV5ClrUsed;
    Word32             bV5ClrImportant;
	Word32             bV5RedMask;
    Word32             bV5GreenMask;
	Word32             bV5BlueMask;
    Word32             bV5AlphaMask;
	Word32             bV5CSType;
    CTDIBICEXYZTRIPLE  bV5Endpoints;
	Word32             bV5GammaRed;
    Word32             bV5GammaGreen;
	Word32             bV5GammaBlue;
    Word32             bV5Intent;
	Word32             bV5ProfileData;
    Word32             bV5ProfileSize;
	Word32             bV5Reserved;
} CTDIBBITMAPV5HEADER, *PCTDIBBITMAPV5HEADER, **PPCTDIBBITMAPV5HEADER;
 #define CTDIB_BI_JPEG  4L
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
typedef struct tagCTDIBRGBQUAD
{ // rgbq
	Word8    rgbBlue;
	Word8    rgbGreen;
    Word8    rgbRed;
	Word8    rgbReserved;
} CTDIBRGBQUAD, *PCTDIBRGBQUAD, **PPCTDIBRGBQUAD;
#endif //   !defined(WIN32) | !defined(CTDIB_USE_WIN32_API)
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
	typedef Handle (*PCTDIBMemAlloc)(Word32);
	typedef void   (*PCTDIBMemFree)(Handle);
	typedef PVOID  (*PCTDIBMemLock)(Handle);
	typedef void   (*PCTDIBMemUnlock)(Handle);
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
class CTDIB
{
	// macros etc.
	// CTDIBBITMAPINFOHEADER alloced:
	enum CTDIBALLOC
	{
		noAlloc,
		crtAlloc,
		heapAlloc
	};
	// DIB Version: 3, 4 or 5
	enum CTDIBVersion
	{
		UnknownVersion = 0,
		WindowsVersion = 3,
		FourthVersion,
		FifhtVersion
	};
	enum CTDIBDirection
	{
		TopDown = -1,
		UnknownDirection = 0,
		BottomUp
	};
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	#define CTDIB_VERSION_3_HEADER_SIZE         sizeof(CTDIBBITMAPINFOHEADER) //40
	#define CTDIB_VERSION_4_HEADER_SIZE         sizeof(CTDIBBITMAPV4HEADER)   //108
	#define CTDIB_VERSION_5_HEADER_SIZE         sizeof(CTDIBBITMAPV5HEADER)   //124
	#define DIB_BITS_TO_BYTES(a)                ((((((a) + 7) / 8) + 3) / 4) * 4)
	#define BITS_TO_BYTES(a)                    (((a) + 7) / 8)
	#define CTDIB_IFNODIB(a)                 	if ( !IsDIBAvailable() ) return a;
    #define CTDIB_UNDECONST(a)                  if ( !UnderConstruction ) return a;
	#define CTDIB_READYTOCREAT                  ( pExternalAlloc && pExternalFree && pExternalLock && pExternalUnlock )
	#define CTDIB_DPI_TO_DPM(a)                 (((a) / 2.54) * 100)
	#define CTDIB_DPM_TO_DPI(a)                 ((((a) / 100 ) * 2.54) + 1)
	#define CTDIB_DEFAULT_PLANES                1
	#define CTDIB_DEFAULT_COMPRESSION           0
	#define CTDIB_DEFAULT_COLORSUSED            0
	#define CTDIB_DEFAULT_COLORSIMPORTANT       0
	#define CTDIB_DEFAULT_RESOLUTION            0
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	// members and fuinctions
private:
	// close DIB properties
	Bool32 DetachDIB();
	// open DIB properties
	Bool32 AttachDIB();
	// return number of used RGBQUAD structures
	Word32 UsedColors(Word32 wBitCount, Word32 wClrUsed);

private:
	Bool32 IsFirstQUADEqualSecond(PCTDIBRGBQUAD fQuad, PCTDIBRGBQUAD sQuad);
	Bool32 FirstQUADLighterThenSecond(PCTDIBRGBQUAD fQuad, PCTDIBRGBQUAD sQuad);
	// Hangle of DIB (Global memory)
	Handle              hDIB;
	// pointer to DIB (CRC memory)
	PVOID               pDIB;
	// pointer to DIB header
	PVOID               pDIBHeader;
	// pointer to first RGBQUAD 32 bit fild
	PCTDIBRGBQUAD       pRGBQuads;
	// pointer to BitFild
	PWord8              pBitFild;
	// version of DIB - 3,4 or 5
	CTDIBVersion        wVersion;
	// DIB Direction
	CTDIBDirection      wDirect;
	// TRUE if DIB attached
	Bool32              IsAvailable;
	// pointer to external memory alloc function - need to be set for creating DIB by this class
	PCTDIBMemAlloc      pExternalAlloc;
	// pointer to external memory free function - need to be set for creating DIB by this class
	PCTDIBMemFree       pExternalFree;
	// pointer to external memory lock function - need to be set for creating DIB by this class
	PCTDIBMemLock       pExternalLock;
	// pointer to external memory unlock function - need to be set for creating DIB by this class
	PCTDIBMemUnlock     pExternalUnlock;
	// TRUE if DIB created by this class
	Bool32              UnderConstruction;
	// flag for DIB created by this module
	Bool32              CreatedByMe;

public:
#pragma warning( push )
#pragma warning( disable : 4003)
	// empty constructor
	RDIB_FUNC()    CTDIB();
	// constructor by Handle
	RDIB_FUNC()    CTDIB(Handle hDIB);
	// destructor	virtual
	RDIB_FUNC()    ~CTDIB();
#pragma warning( pop )

public:
	// return black pixel RGBQuad index or 00-00-00
	RDIB_FUNC(Word32) GetBlackPixel();
	// return white pixel RGBQuad index or ff-ff-ff
	RDIB_FUNC(Word32) GetWhitePixel();
	// copy resolution from another DIB at once4
	RDIB_FUNC(Bool32) CopyDPIFromDIB( CTDIB * pSrcDIB);
	// copy line from another DIB from X at once
	RDIB_FUNC(Bool32) SetFuelLineFromDIB(CTDIB * pSrcDIB, Word32 nSrcLine, Word32 nDscLine, Word32 wSrcX);
	// Copy fuel pallete from another DIB
	RDIB_FUNC(Bool32) CopyPalleteFromDIB(CTDIB *pSrcDIB);
	// get DIB vrsion by enum versions
	RDIB_FUNC(CTDIB::CTDIBVersion) GetVersion(void);
	////////////////////////////Creating
	// get 4 external function for class:
	// Handle (*PCTDIBMemAlloc)(Word32);
	// void   (*PCTDIBMemFree)(Handle);
	// PVOID  (*PCTDIBMemLock)(Handle);
	// void   (*PCTDIBMemUnlock)(Handle);
	RDIB_FUNC(Bool32) SetExternals(PCTDIBMemAlloc pfAlloc, PCTDIBMemFree pfFree, PCTDIBMemLock pfLock, PCTDIBMemUnlock pfUnlock);
	// start to create new DIB
	RDIB_FUNC(Handle) CreateDIBBegin(Int32 Width, Int32 Height, Word32 BitCount, Word32 UseColors = 0, CTDIBVersion dVersion = WindowsVersion);
	// end of creating DIB
	RDIB_FUNC(Bool32) CreateDIBEnd(void);
	// Free memory if DIB created by this class
	RDIB_FUNC(Bool32) DestroyDIB();
	////////////////////////////////////////////////////////Existst DIB
	// set DIB by global handle
	RDIB_FUNC(Bool32) SetDIBbyHandle(Handle hDIB);
	// set DIB by memory ptr
	RDIB_FUNC(Bool32) SetDIBbyPtr(PVOID pDIB);
	// remove DIB and set to ready for new once
	RDIB_FUNC(Bool32) ResetDIB(void);
	///////////////////////////////////////////////////////////////////
	// return TRUE if image attached to class and FALSE otherwise
	RDIB_FUNC(Bool32) IsDIBAvailable();
	////////////////////////////////////////////////////////Header Data
	//Get DIB header memory allocation size
	RDIB_FUNC(Word32) GetHeaderSize(void);
	// get pointer to DIB header
	RDIB_FUNC(PVOID) GetPtrToHeader();
	// get version of DIB
	RDIB_FUNC(Word32) GetDIBVersion();
	// return image width in pixels
	RDIB_FUNC(Int32) GetImageWidth();
	// return image height in pixels
	RDIB_FUNC(Int32) GetImageHeight();
	// return image width in pixels
	RDIB_FUNC(Word32) GetLineWidth();
	// return image width in bytes forsed to 4
	RDIB_FUNC(Word32) GetLineWidthInBytes();
	// return image width in bytes
	RDIB_FUNC(Word32) GetUsedLineWidthInBytes();
	// return image height in pixels
	RDIB_FUNC(Word32) GetLinesNumber();
	// return image size in pixels
	RDIB_FUNC(Word32) GetImageSize();
	// return image size in bytes
	RDIB_FUNC(Word32) GetImageSizeInBytes();
	// return bits per pixel
	RDIB_FUNC(Word32) GetPixelSize();
	// Get number of used colors
	// if 0 - DIB is JPEG format
	RDIB_FUNC(Word32) GetActualColorNumber();
	// get resolution of DIB: x,[y]
	RDIB_FUNC(Bool32) GetResolutionDPM(PWord32 pX_Dpm, PWord32 pY_Dpm = 0x0);
	// get resolution of DIB: x,[y]
	RDIB_FUNC(Bool32) GetResolutionDPI(PWord32 pX_Dpi, PWord32 pY_Dpi = 0x0);
	// set resolution of DIB: x,[y]. available if DIB constructed by CTDIB
	RDIB_FUNC(Bool32) SetResolutionDPM(Word32 X_Dpm, Word32 Y_DPM = 0);
	// set resolution of DIB: x,[y]. available if DIB constructed by CTDIB
	RDIB_FUNC(Bool32) SetResolutionDPI(Word32 X_DPI, Word32 Y_DPI = 0);
	// Get allocated memory size for DIBHeader, RGBQuads and Image in bytes
	RDIB_FUNC(Word32) GetDIBSize();
	//Get used by DIB RGB Quads memory allocation size
	RDIB_FUNC(Word32) GetRGBPalleteSize(void);
	// get HANDLE to DIB
	RDIB_FUNC(Bool32) GetDIBHandle(PHandle phDIB);
	// Set Handle for DIB if it not attached
	RDIB_FUNC(Bool32) SetDIBHandle(Handle hDIB);
	// get pointer to DIB
	RDIB_FUNC(Bool32) GetDIBPtr(PPVOID ppDIB);
	////////////////////////////////////////////////////////Pallette Data
	// get pointer to first RGBQuad of RGB Quads ( or Triads)
	RDIB_FUNC(PVOID)  GetPtrToRGB();
	// Get RGBQuad[wQuad]
	RDIB_FUNC(Bool32) GetRGBQuad(Word32 wQuad, PCTDIBRGBQUAD pQuad);
	// set RGBQuad
	RDIB_FUNC(Bool32) SetRGBQuad(Word32 wQuad, CTDIBRGBQUAD Quad);
	////////////////////////////////////////////////////////BitFild Data
	// Get pointer to BitFild data;
	RDIB_FUNC(PVOID)  GetPtrToBitFild(void);
	// get pointer to Line (from 0 to |biHeight|-1)
	RDIB_FUNC(PVOID)  GetPtrToLine(Word32 wLine);
	// get ptr to BitFild memory on pixel(x,y)
	// if PixelSize < 8 - ptr to byte where its pixel
	RDIB_FUNC(PVOID)  GetPtrToPixel(Word32 wPixelX, Word32 wPixelY);
	// Get bit position in byte of image fild for pixel
	RDIB_FUNC(Word32) GetPixelShiftInByte(Word32 dwX);
	// Check Externals Memory functions
	RDIB_FUNC(Bool32) IsExternalsSets(void);
///////////////////////////////////////////////////////////////////////////////
protected:

};

typedef CTDIB    *PCTDIB, **PPCTDIB;

#endif // !defined(_CTDIB_H_)
