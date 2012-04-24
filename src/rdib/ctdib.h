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

#ifndef _CTDIB_H_
#define _CTDIB_H_

#include "globus.h"
#if defined(WIN32) & defined(CTDIB_USE_WIN32_API)
#include <wingdi.h>
typedef BITMAPINFOHEADER CTDIBBITMAPINFOHEADER, *PCTDIBBITMAPINFOHEADER, **PPCTDIBBITMAPINFOHEADER;
typedef BITMAPV4HEADER CTDIBBITMAPV4HEADER, *PCTDIBBITMAPV4HEADER, **PPCTDIBBITMAPV4HEADER;
typedef BITMAPV5HEADER CTDIBBITMAPV5HEADER, *PCTDIBBITMAPV5HEADER, **PPCTDIBBITMAPV5HEADER;
#define CTDIB_BI_JPEG    BI_JPEG
#else
// DIB version 3 header (lenght - 40 bytes)
typedef struct tagCTDIBBITMAPINFOHEADER { // bmih
    uint32_t        biSize;
    int32_t         biWidth;
    int32_t         biHeight;
    uint16_t        biPlanes;
    uint16_t        biBitCount;
    uint32_t        biCompression;
    uint32_t        biSizeImage;
    int32_t         biXPelsPerMeter;
    int32_t         biYPelsPerMeter;
    uint32_t        biClrUsed;
    uint32_t        biClrImportant;
} CTDIBBITMAPINFOHEADER;

typedef int32_t     CTDIBPOINTCOORDINATE;

struct CTDIBCIEXYZ {
    CTDIBPOINTCOORDINATE ciexyzX;
    CTDIBPOINTCOORDINATE ciexyzY;
    CTDIBPOINTCOORDINATE ciexyzZ;
};

struct CTDIBICEXYZTRIPLE {
    CTDIBCIEXYZ  ciexyzRed;
    CTDIBCIEXYZ  ciexyzGreen;
    CTDIBCIEXYZ  ciexyzBlue;
};

// DIB version 4 header (lenght - 108 bytes)
typedef struct tagCTDIBBITMAPV4HEADER {
    uint32_t             bV4Size;
    int32_t              bV4Width;
    int32_t              bV4Height;
    uint16_t             bV4Planes;
    uint16_t             bV4BitCount;
    uint32_t             bV4V4Compression;
    uint32_t             bV4SizeImage;
    int32_t              bV4XPelsPerMeter;
    int32_t              bV4YPelsPerMeter;
    uint32_t             bV4ClrUsed;
    uint32_t             bV4ClrImportant;
    uint32_t             bV4RedMask;
    uint32_t             bV4GreenMask;
    uint32_t             bV4BlueMask;
    uint32_t             bV4AlphaMask;
    uint32_t             bV4CSType;
    CTDIBICEXYZTRIPLE  bV4Endpoints;
    uint32_t             bV4GammaRed;
    uint32_t             bV4GammaGreen;
    uint32_t             bV4GammaBlue;
} CTDIBBITMAPV4HEADER;

// DIB version 5 header (lenght - 124 bytes)
typedef struct tagCTDIBBITMAPV5HEADER {
    uint32_t             bV5Size;
    int32_t              bV5Width;
    int32_t              bV5Height;
    uint16_t             bV5Planes;
    uint16_t             bV5BitCount;
    uint32_t             bV5Compression;
    uint32_t             bV5SizeImage;
    int32_t              bV5XPelsPerMeter;
    int32_t              bV5YPelsPerMeter;
    uint32_t             bV5ClrUsed;
    uint32_t             bV5ClrImportant;
    uint32_t             bV5RedMask;
    uint32_t             bV5GreenMask;
    uint32_t             bV5BlueMask;
    uint32_t             bV5AlphaMask;
    uint32_t             bV5CSType;
    CTDIBICEXYZTRIPLE  bV5Endpoints;
    uint32_t             bV5GammaRed;
    uint32_t             bV5GammaGreen;
    uint32_t             bV5GammaBlue;
    uint32_t             bV5Intent;
    uint32_t             bV5ProfileData;
    uint32_t             bV5ProfileSize;
    uint32_t             bV5Reserved;
} CTDIBBITMAPV5HEADER;

#define CTDIB_BI_JPEG  4L

struct CTDIBRGBQUAD { // rgbq
    CTDIBRGBQUAD() :
        rgbBlue(0),
        rgbGreen(0),
        rgbRed(0),
        rgbReserved(0) {}

    CTDIBRGBQUAD(uchar r, uchar g, uchar b, uchar a) :
        rgbBlue(r),
        rgbGreen(g),
        rgbRed(b),
        rgbReserved(a) {}
public:
    uchar    rgbBlue;
    uchar    rgbGreen;
    uchar    rgbRed;
    uchar    rgbReserved;
};

typedef CTDIBRGBQUAD * PCTDIBRGBQUAD;
#endif //   !defined(WIN32) | !defined(CTDIB_USE_WIN32_API)

typedef Handle (*PCTDIBMemAlloc)(uint32_t);
typedef void   (*PCTDIBMemFree)(Handle);
typedef pvoid  (*PCTDIBMemLock)(Handle);
typedef void   (*PCTDIBMemUnlock)(Handle);

class CLA_EXPO CTDIB
{
    public:
        // macros etc.
        // CTDIBBITMAPINFOHEADER alloced:
        enum CTDIBALLOC {
            noAlloc,
            crtAlloc,
            heapAlloc
        };
        // DIB Version: 3, 4 or 5
        enum CTDIBVersion {
            UnknownVersion = 0,
            WindowsVersion = 3,
            FourthVersion,
            FifhtVersion
        };
        enum CTDIBDirection {
            TopDown = -1,
            UnknownDirection = 0,
            BottomUp
        };
    public:
        // empty constructor
        CTDIB();
        // constructor by Handle
        CTDIB(Handle hDIB);
        // destructor   virtual
        ~CTDIB();

        bool pixelColor(uint x, uint y, CTDIBRGBQUAD * dest) const;
    public:
        // return black pixel RGBQuad index or 00-00-00
        uint32_t GetBlackPixel();
        // return white pixel RGBQuad index or ff-ff-ff
        uint32_t GetWhitePixel();
        // copy resolution from another DIB at once4
        Bool32 CopyDPIFromDIB( CTDIB * pSrcDIB);
        // copy line from another DIB from X at once
        Bool32 SetFuelLineFromDIB(const CTDIB * pSrcDIB, uint32_t nSrcLine, uint32_t nDscLine, uint32_t wSrcX);
        // Copy fuel pallete from another DIB
        Bool32 CopyPalleteFromDIB(CTDIB *pSrcDIB);
        // get DIB vrsion by enum versions
        CTDIB::CTDIBVersion GetVersion(void);
        ////////////////////////////Creating
        // get 4 external function for class:
        // Handle (*PCTDIBMemAlloc)(uint32_t);
        // void   (*PCTDIBMemFree)(Handle);
        // pvoid  (*PCTDIBMemLock)(Handle);
        // void   (*PCTDIBMemUnlock)(Handle);
        Bool32 SetExternals(PCTDIBMemAlloc pfAlloc, PCTDIBMemFree pfFree, PCTDIBMemLock pfLock, PCTDIBMemUnlock pfUnlock);
        // start to create new DIB
        Handle CreateDIBBegin(int32_t Width, int32_t Height, uint32_t BitCount, uint32_t UseColors = 0, CTDIBVersion dVersion = WindowsVersion);
        // end of creating DIB
        Bool32 CreateDIBEnd(void);
        // Free memory if DIB created by this class
        Bool32 DestroyDIB();
        ////////////////////////////////////////////////////////Existst DIB
        // set DIB by global handle
        Bool32 SetDIBbyHandle(Handle hDIB);
        // set DIB by memory ptr
        Bool32 SetDIBbyPtr(pvoid pDIB);
        // remove DIB and set to ready for new once
        Bool32 ResetDIB();
        ///////////////////////////////////////////////////////////////////
        // return TRUE if image attached to class and FALSE otherwise
        Bool32 IsDIBAvailable() const;
        ////////////////////////////////////////////////////////Header Data
        //Get DIB header memory allocation size
        uint32_t GetHeaderSize() const;
        // get pointer to DIB header
        CTDIBBITMAPINFOHEADER * GetPtrToHeader() const;
        // get version of DIB
        CTDIBVersion GetDIBVersion() const;
        // return image width in pixels
        int32_t GetImageWidth() const;
        // return image height in pixels
        int32_t GetImageHeight() const;
        // return image width in pixels
        uint32_t GetLineWidth() const;
        // return image width in bytes forsed to 4
        uint32_t GetLineWidthInBytes() const;
        // return image width in bytes
        uint32_t GetUsedLineWidthInBytes() const;
        // return image height in pixels
        uint32_t GetLinesNumber() const;
        // return image size in pixels
        uint32_t GetImageSize() const;
        // return image size in bytes
        uint32_t GetImageSizeInBytes() const;
        // return bits per pixel
        uint32_t GetPixelSize() const;
        // Get number of used colors
        // if 0 - DIB is JPEG format
        uint32_t GetActualColorNumber() const;
        // get resolution of DIB: x,[y]
        Bool32 GetResolutionDPM(uint32_t * pX_Dpm, uint32_t * pY_Dpm = 0x0);
        // get resolution of DIB: x,[y]
        Bool32 GetResolutionDPI(uint32_t * pX_Dpi, uint32_t * pY_Dpi = 0x0);
        // set resolution of DIB: x,[y]. available if DIB constructed by CTDIB
        Bool32 SetResolutionDPM(uint32_t X_Dpm, uint32_t Y_DPM = 0);
        // set resolution of DIB: x,[y]. available if DIB constructed by CTDIB
        Bool32 SetResolutionDPI(uint32_t X_DPI, uint32_t Y_DPI = 0);
        // Get allocated memory size for DIBHeader, RGBQuads and Image in bytes
        uint32_t GetDIBSize() const;
        //Get used by DIB RGB Quads memory allocation size
        uint32_t GetRGBPalleteSize() const;
        // get Handle to DIB
        Bool32 GetDIBHandle(Handle* phDIB);
        // Set Handle for DIB if it not attached
        Bool32 SetDIBHandle(Handle hDIB);
        // get pointer to DIB
        bool GetDIBPtr(pvoid* ppDIB) const;
        ////////////////////////////////////////////////////////Pallette Data
        // get pointer to first RGBQuad of RGB Quads ( or Triads)
        pvoid  GetPtrToRGB() const;
        // Get RGBQuad[wQuad]
        bool GetRGBQuad(uint32_t idx, CTDIBRGBQUAD *dest) const;
        // set RGBQuad
        Bool32 SetRGBQuad(uint32_t wQuad, CTDIBRGBQUAD Quad);
        ////////////////////////////////////////////////////////BitFild Data
        // Get pointer to BitFild data;
        pvoid  GetPtrToBitFild(void);
        // get pointer to Line (from 0 to |biHeight|-1)
        pvoid  GetPtrToLine(uint32_t wLine) const;
        // get ptr to BitFild memory on pixel(x,y)
        // if PixelSize < 8 - ptr to byte where its pixel
        pvoid  GetPtrToPixel(uint32_t wPixelX, uint32_t wPixelY) const;
        // Get bit position in byte of image fild for pixel
        uint32_t GetPixelShiftInByte(uint32_t dwX) const;
    private:
        // open DIB properties
        Bool32 AttachDIB();
        // close DIB properties
        void DetachDIB();
        // Check Externals Memory functions
        bool isExternalsSets() const;
        // return number of used RGBQUAD structures
        static uint UsedColors(uint bitCount, uint colorUsed);
    private:
        // Hangle of DIB (Global memory)
        Handle              hDIB;
        // pointer to DIB (CRC memory)
        pvoid               pDIB;
        // pointer to DIB header
        CTDIBBITMAPINFOHEADER * pDIBHeader;
        // pointer to first RGBQUAD 32 bit fild
        PCTDIBRGBQUAD       pRGBQuads;
        // pointer to BitFild
        puchar              pBitFild;
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
};

typedef CTDIB * PCTDIB;

#endif // _CTDIB_H_
