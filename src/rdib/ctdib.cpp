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

#include <cassert>
#include <cstdio>
#include <memory.h>
#include <stdlib.h>

#include "ctdib.h"
#include "common/debug.h"

using namespace cf;

static inline size_t DIB_BITS_TO_BYTES(size_t b)
{
    return ((((((b) + 7) / 8) + 3) / 4) * 4);
}

static inline size_t BITS_TO_BYTES(size_t a)
{
    return (((a) + 7) / 8);
}

static bool equal(const CTDIBRGBQUAD& q1, const CTDIBRGBQUAD& q2)
{
    return q1.rgbBlue == q2.rgbBlue &&
            q1.rgbGreen == q2.rgbGreen &&
            q1.rgbRed == q2.rgbRed;
}

bool firstQUADLighterThenSecond(const CTDIBRGBQUAD& q1, const CTDIBRGBQUAD& q2)
{
    return ((q1.rgbBlue * q1.rgbBlue +
             q1.rgbGreen * q1.rgbGreen +
             q1.rgbRed * q1.rgbRed) >
            (q2.rgbBlue * q2.rgbBlue +
             q2.rgbGreen * q2.rgbGreen +
             q2.rgbRed * q2.rgbRed) ? true : false);
}

static const int CTDIB_VERSION_3_HEADER_SIZE = sizeof(CTDIBBITMAPINFOHEADER); //40
static const int CTDIB_VERSION_4_HEADER_SIZE = sizeof(CTDIBBITMAPV4HEADER);   //108
static const int CTDIB_VERSION_5_HEADER_SIZE = sizeof(CTDIBBITMAPV5HEADER);   //124
static const int CTDIB_DEFAULT_PLANES = 1;
static const int CTDIB_DEFAULT_COMPRESSION = 0;
static const int CTDIB_DEFAULT_COLORSUSED = 0;
static const int CTDIB_DEFAULT_COLORSIMPORTANT = 0;
static const int CTDIB_DEFAULT_RESOLUTION = 0;

#define CTDIB_IFNODIB(a)                    if ( !IsDIBAvailable() ) return a;
#define CTDIB_UNDECONST(a)                  if ( !UnderConstruction ) return a;
#define CTDIB_READYTOCREAT                  ( pExternalAlloc && pExternalFree && pExternalLock && pExternalUnlock )
#define CTDIB_DPI_TO_DPM(a)                 (((a) / 2.54) * 100)
#define CTDIB_DPM_TO_DPI(a)                 ((((a) / 100 ) * 2.54) + 1)

CTDIB::CTDIB() {
    hDIB = NULL;
    pDIB = NULL;
    DetachDIB();
    IsAvailable = FALSE;
    UnderConstruction = FALSE;
    wDirect = UnknownDirection;
    pExternalAlloc = NULL;
    pExternalFree = NULL;
    pExternalLock = NULL;
    pExternalUnlock = NULL;
    CreatedByMe = FALSE;
}

CTDIB::~CTDIB() {
    DestroyDIB();
}

CTDIB::CTDIB(Handle hAtDIB) {
    hDIB = NULL;
    pDIB = NULL;
    IsAvailable = FALSE;
    UnderConstruction = FALSE;
    wDirect = UnknownDirection;
    pExternalAlloc = NULL;
    pExternalFree = NULL;
    pExternalLock = NULL;
    pExternalUnlock = NULL;
    CreatedByMe = FALSE;
    DetachDIB();
    hDIB = hAtDIB;
}

uint32_t CTDIB::GetActualColorNumber() {
    CTDIB_IFNODIB(0);
    return UsedColors(pDIBHeader->biBitCount, pDIBHeader->biClrUsed);
}

Bool32 CTDIB::AttachDIB() {
    CTDIBBITMAPINFOHEADER * pSimpleHead;

    if (hDIB == NULL && pDIB == NULL) {
        DetachDIB();
        return false;
    }

    if (isExternalsSets() && hDIB && !UnderConstruction)
        pDIB = pExternalLock(hDIB);

    if (pDIB) { // bitmapinfoheader
        IsAvailable = TRUE;
        pSimpleHead = (CTDIBBITMAPINFOHEADER*) pDIB;
        wDirect = (pSimpleHead->biHeight > 0 ? BottomUp : TopDown);

        //#define CERR(var) std::cerr << #var << " = " << (var) << "\n";

        //        CERR(pSimpleHead->biBitCount);
        //        CERR(pSimpleHead->biWidth);
        //        CERR(pSimpleHead->biHeight);
        //        CERR(pSimpleHead->biSize);
        //        CERR(pSimpleHead->biSizeImage);


        switch (pSimpleHead->biSize) {
        case CTDIB_VERSION_3_HEADER_SIZE:
            wVersion = WindowsVersion;
            break;
        case CTDIB_VERSION_4_HEADER_SIZE:
            wVersion = FourthVersion;
            break;
        case CTDIB_VERSION_5_HEADER_SIZE:
            wVersion = FifhtVersion;
            break;
        default: {
            // uliss
            // TODO seems that memory corruption occured and field biSize filled with junk
            pSimpleHead->biSize = CTDIB_VERSION_3_HEADER_SIZE;
            break;
            // uliss
            Debug() << "CTDIB::AttachDIB: Unknown DIB header size: " << pSimpleHead->biSize << "\n";
            DetachDIB();
            return false;
        }
        }

        pDIBHeader = (CTDIBBITMAPINFOHEADER*) pDIB;
        pRGBQuads = (PCTDIBRGBQUAD) ((puchar) pDIB + pSimpleHead->biSize);
        pBitFild = (puchar) (pRGBQuads + GetActualColorNumber());
        IsAvailable = TRUE;
        wDirect = (pSimpleHead->biHeight > 0 ? BottomUp : TopDown);
        return TRUE;
    }

    return FALSE;
}

void CTDIB::DetachDIB() {
    if (hDIB) {
        hDIB = NULL;
    }

    pDIBHeader = NULL;
    pRGBQuads = NULL;
    pBitFild = NULL;
    pDIB = NULL;
    IsAvailable = FALSE;
    CreatedByMe = FALSE;
    UnderConstruction = FALSE;
    wVersion = UnknownVersion;
    wDirect = UnknownDirection;
}

pvoid CTDIB::GetPtrToHeader() {
    CTDIB_IFNODIB(NULL);
    return pDIBHeader;
}

pvoid CTDIB::GetPtrToRGB() {
    CTDIB_IFNODIB(NULL);
    return (pvoid) pRGBQuads;
}

uint32_t CTDIB::GetDIBVersion() {
    CTDIB_IFNODIB(0);
    return wVersion;
}

Bool32 CTDIB::SetDIBbyPtr(pvoid ptr) {
    if ((hDIB && pDIB) || !ptr)
        return FALSE;

    pDIB = ptr;
    return AttachDIB();
}

Bool32 CTDIB::SetDIBbyHandle(Handle hnd) {
    if (hDIB || pDIB || !hnd)
        return FALSE;

    hDIB = hnd;
    return AttachDIB();
}

Bool32 CTDIB::IsDIBAvailable() const {
    return IsAvailable;
}

int32_t CTDIB::GetImageHeight() const {
    CTDIB_IFNODIB(0);
    return pDIBHeader->biHeight;
}

uint32_t CTDIB::GetLinesNumber() const {
    CTDIB_IFNODIB(0);
    return abs(GetImageHeight());
}

int32_t CTDIB::GetImageWidth() const {
    CTDIB_IFNODIB(0);
    return pDIBHeader->biWidth;
}

uint32_t CTDIB::GetLineWidth() const {
    CTDIB_IFNODIB(0);
    return abs(GetImageWidth());
}

uint32_t CTDIB::GetLineWidthInBytes() const {
    CTDIB_IFNODIB(0);
    return DIB_BITS_TO_BYTES(GetLineWidth() * GetPixelSize());
}

uint32_t CTDIB::GetUsedLineWidthInBytes() const {
    CTDIB_IFNODIB(0);
    return BITS_TO_BYTES(GetLineWidth() * GetPixelSize());
}

uint32_t CTDIB::GetImageSize() {
    CTDIB_IFNODIB(0);
    return GetLinesNumber() * GetLineWidth();
}

uint32_t CTDIB::GetImageSizeInBytes() {
    CTDIB_IFNODIB(0);
    return GetLineWidthInBytes() * GetLinesNumber();
}

uint32_t CTDIB::GetPixelSize() const {
    CTDIB_IFNODIB(0);
    return (uint32_t) pDIBHeader->biBitCount;
}

Handle CTDIB::CreateDIBBegin(int32_t Width, int32_t Height, uint32_t BitCount, uint32_t UseColors,
        CTDIBVersion dVersion) {
#define DELETE_PHEADER(a)  delete a;
    CTDIBBITMAPINFOHEADER * pV3Header = NULL;
    CTDIBBITMAPV4HEADER * pV4Header = NULL;
    CTDIBBITMAPV5HEADER * pV5Header = NULL;
    pvoid pHeader = NULL;
    uint32_t HeaderSize;
    uint32_t FuelSize = 0;

    if (!CTDIB_READYTOCREAT)
        return NULL;

    if (IsDIBAvailable())
        return NULL;

    switch (dVersion) {
    case WindowsVersion:
        HeaderSize = CTDIB_VERSION_3_HEADER_SIZE;
        pV3Header = new CTDIBBITMAPINFOHEADER;
        pV3Header->biSize = CTDIB_VERSION_3_HEADER_SIZE;
        pV3Header->biWidth = Width;
        pV3Header->biHeight = Height;
        pV3Header->biPlanes = CTDIB_DEFAULT_PLANES;
        pV3Header->biBitCount = (uint16_t) BitCount;
        pV3Header->biCompression = CTDIB_DEFAULT_COMPRESSION;
        pV3Header->biSizeImage = DIB_BITS_TO_BYTES(Width * BitCount) * abs(Height);
        pV3Header->biXPelsPerMeter = CTDIB_DEFAULT_RESOLUTION;
        pV3Header->biYPelsPerMeter = CTDIB_DEFAULT_RESOLUTION;
        pV3Header->biClrUsed = (uint16_t) UseColors;
        pV3Header->biClrImportant = CTDIB_DEFAULT_COLORSIMPORTANT;
        pHeader = (pvoid) pV3Header;
        break;
    case FourthVersion:
        HeaderSize = CTDIB_VERSION_4_HEADER_SIZE;
        pV4Header = new CTDIBBITMAPV4HEADER;
        pV4Header->bV4Size = CTDIB_VERSION_4_HEADER_SIZE;
        pV4Header->bV4Height = Height;
        pV4Header->bV4Width = Width;
        pV4Header->bV4Planes = CTDIB_DEFAULT_PLANES;
        pV4Header->bV4BitCount = (uint16_t) BitCount;
        pV4Header->bV4V4Compression = CTDIB_DEFAULT_COMPRESSION;
        pV4Header->bV4SizeImage = DIB_BITS_TO_BYTES(Width * BitCount) * abs(Height);
        pV4Header->bV4XPelsPerMeter = CTDIB_DEFAULT_RESOLUTION;
        pV4Header->bV4YPelsPerMeter = CTDIB_DEFAULT_RESOLUTION;
        pV4Header->bV4ClrUsed = (uint16_t) UseColors;
        pV4Header->bV4ClrImportant = CTDIB_DEFAULT_COLORSIMPORTANT;
        pHeader = (pvoid) pV4Header;
        break;
    case FifhtVersion:
        HeaderSize = CTDIB_VERSION_5_HEADER_SIZE;
        pV5Header = new CTDIBBITMAPV5HEADER;
        pV5Header->bV5Size = CTDIB_VERSION_5_HEADER_SIZE;
        pV5Header->bV5Height = Height;
        pV5Header->bV5Width = Width;
        pV5Header->bV5Planes = CTDIB_DEFAULT_PLANES;
        pV5Header->bV5BitCount = (uint16_t) BitCount;
        pV5Header->bV5Compression = CTDIB_DEFAULT_COMPRESSION;
        pV5Header->bV5SizeImage = DIB_BITS_TO_BYTES(Width * BitCount) * abs(Height);
        pV5Header->bV5XPelsPerMeter = CTDIB_DEFAULT_RESOLUTION;
        pV5Header->bV5YPelsPerMeter = CTDIB_DEFAULT_RESOLUTION;
        pV5Header->bV5ClrUsed = (uint16_t) UseColors;
        pV5Header->bV5ClrImportant = CTDIB_DEFAULT_COLORSIMPORTANT;
        pHeader = (pvoid) pV4Header;
        break;
    default:
        return NULL;
    }

    FuelSize += HeaderSize;
    FuelSize += UsedColors(BitCount, UseColors) * sizeof(CTDIBRGBQUAD);
    FuelSize += DIB_BITS_TO_BYTES(Width * BitCount) * abs(Height);
    hDIB = malloc(FuelSize);
    assert(hDIB);
    pDIB = hDIB;
    //    TODO uliss: check!!!
    //    if (!(pDIB = pExternalLock(hDIB))) {
    //        free(hDIB);
    //        return FALSE;
    //    }
    CreatedByMe = TRUE;
    memset(pDIB, 0x00, FuelSize);
    memcpy(pDIB, pHeader, HeaderSize);
    pDIBHeader = (CTDIBBITMAPINFOHEADER*) pDIB;
    DELETE_PHEADER( pV3Header );
    DELETE_PHEADER( pV4Header );
    DELETE_PHEADER( pV5Header );
    IsAvailable = TRUE;
    UnderConstruction = TRUE;
    AttachDIB();
    return hDIB;
}

Bool32 CTDIB::SetExternals(PCTDIBMemAlloc pfAlloc, PCTDIBMemFree pfFree, PCTDIBMemLock pfLock,
        PCTDIBMemUnlock pfUnlock) {
    if (!pfAlloc && !pfFree && !pfLock && !pfUnlock)
        return FALSE;

    if ((void *) pfAlloc == (void *) pfFree || (void *) pfAlloc == (void *) pfLock
            || (void *) pfAlloc == (void *) pfUnlock || (void *) pfFree == (void *) pfLock
            || (void *) pfFree == (void *) pfUnlock) {
        return FALSE;
    }

    if (CTDIB_READYTOCREAT)
        return FALSE;

    if (isExternalsSets())
        return false;

    if (IsDIBAvailable())
        return FALSE;

    pExternalAlloc = pfAlloc;
    pExternalFree = pfFree;
    pExternalLock = pfLock;
    pExternalUnlock = pfUnlock;
    return TRUE;
}

uint CTDIB::UsedColors(uint bitCount, uint colorUsed)
{
    if(colorUsed != 0)
        return colorUsed;

    switch (bitCount) {
    case 0:
        return 0;
    case 1:
        return 2;
    case 4:
        return 16;
    case 8:
        return 256;
    case 16:
    case 24:
    case 32:
    default:
        return 0;
    }
}

Bool32 CTDIB::SetResolutionDPI(uint32_t X_Dpi, uint32_t Y_Dpi) {
    CTDIB_UNDECONST(FALSE);
    return SetResolutionDPM((uint32_t) CTDIB_DPI_TO_DPM(X_Dpi), (uint32_t) CTDIB_DPI_TO_DPM(Y_Dpi));
}

Bool32 CTDIB::SetResolutionDPM(uint32_t X_Dpm, uint32_t Y_Dpm) {
    CTDIBBITMAPINFOHEADER * pH;
    CTDIB_UNDECONST(FALSE);
    pH = pDIBHeader;
    pH->biXPelsPerMeter = X_Dpm;

    if (Y_Dpm == 0)
        pH->biYPelsPerMeter = pH->biXPelsPerMeter;

    else
        pH->biYPelsPerMeter = Y_Dpm;

    return TRUE;
}

Bool32 CTDIB::GetResolutionDPI(uint32_t * pX_Dpi, uint32_t * pY_Dpi) {
    uint32_t x_dpi;
    uint32_t y_dpi;
    CTDIB_IFNODIB(FALSE);

    if (pX_Dpi == NULL || !GetResolutionDPM(&x_dpi, &y_dpi))
        return FALSE;

    *pX_Dpi = (uint32_t) CTDIB_DPM_TO_DPI(x_dpi);

    if (pY_Dpi != NULL)
        *pY_Dpi = (uint32_t) CTDIB_DPM_TO_DPI(y_dpi);

    return TRUE;
}

Bool32 CTDIB::GetResolutionDPM(uint32_t * pX_Dpm, uint32_t * pY_Dpm) {
    CTDIBBITMAPINFOHEADER * pH;
    CTDIB_IFNODIB(FALSE);

    if (pX_Dpm == NULL)
        return FALSE;

    pH = pDIBHeader;
    *pX_Dpm = pH->biXPelsPerMeter;

    if (pY_Dpm != NULL)
        *pY_Dpm = pH->biYPelsPerMeter;

    return TRUE;
}

Bool32 CTDIB::CreateDIBEnd() {
    return !(UnderConstruction = FALSE);
}

pvoid CTDIB::GetPtrToLine(uint32_t wLine) const {
    puchar pLine = NULL;
    CTDIB_IFNODIB(NULL);

    if (wLine >= GetLinesNumber())
        return NULL;

    pLine = pBitFild;

    if (wDirect == BottomUp) {
        pLine += ((GetLinesNumber() - (wLine + 1)) * GetLineWidthInBytes());
        return (pvoid) pLine;
    }

    if (wDirect == TopDown) {
        pLine += (wLine * GetLineWidthInBytes());
        return (pvoid) pLine;
    }

    return NULL;
}

pvoid CTDIB::GetPtrToPixel(uint32_t wPixelX, uint32_t wPixelY) const {
    puchar pLine = NULL;
    CTDIB_IFNODIB(NULL);

    if (wPixelX >= GetLineWidth())
        return NULL;

    pLine = (puchar) GetPtrToLine(wPixelY);

    if (!pLine)
        return NULL;

    pLine += ((wPixelX * GetPixelSize()) / 8);
    return pLine;
}

Bool32 CTDIB::ResetDIB() {
    CTDIB_IFNODIB(FALSE);
    DetachDIB();
    return true;
}

Bool32 CTDIB::GetDIBHandle(Handle* phDIB) {
    CTDIB_IFNODIB(FALSE);
    *phDIB = hDIB;
    return TRUE;
}

Bool32 CTDIB::GetDIBPtr(pvoid* ppDIB) {
    CTDIB_IFNODIB(NULL);
    *ppDIB = pDIB;
    return TRUE;
}

Bool32 CTDIB::SetRGBQuad(uint32_t wQuad, CTDIBRGBQUAD Quad) {
    PCTDIBRGBQUAD pCurrentQuad;

    if (pRGBQuads == NULL)
        return FALSE;

    //CTDIB_UNDECONST(FALSE);

    if (wQuad > GetActualColorNumber()) {
        return FALSE;
    }

    pCurrentQuad = (PCTDIBRGBQUAD) GetPtrToRGB();
    pCurrentQuad += wQuad;
    *pCurrentQuad = Quad;
    return TRUE;
}

Bool32 CTDIB::DestroyDIB() {
    Handle hMem;
    Bool32 FreeMem = FALSE;
    Bool32 bRet = TRUE;
    CTDIB_IFNODIB(FALSE);
    hMem = hDIB;
    FreeMem = CreatedByMe && isExternalsSets();

    if (CreatedByMe != isExternalsSets())
        bRet = FALSE;

    if (FreeMem) {
        free(hMem);
    }

    DetachDIB();
    return bRet;
}

Bool32 CTDIB::SetDIBHandle(Handle hSetDIB) {
    if (IsDIBAvailable() || hDIB != NULL)
        return FALSE;

    hDIB = hSetDIB;
    return FALSE;
}

uint32_t CTDIB::GetDIBSize() {
    uint32_t FuelSize = 0;
    CTDIB_IFNODIB(0);
    FuelSize += GetHeaderSize();
    FuelSize += GetRGBPalleteSize();
    FuelSize += GetImageSizeInBytes();
    return FuelSize;
}

uint32_t CTDIB::GetHeaderSize() {
    CTDIB_IFNODIB(0);
    return *((uint32_t *) (GetPtrToHeader()));
}

uint32_t CTDIB::GetRGBPalleteSize() {
    CTDIB_IFNODIB(0);
    return (GetActualColorNumber() * sizeof(CTDIBRGBQUAD));
}

uint32_t CTDIB::GetPixelShiftInByte(uint32_t dwX) const {
    CTDIB_IFNODIB(0);

    switch (GetPixelSize()) {
    case 1:
        return (dwX % 8);
        break;
    case 4:

        if (dwX % 2 == 0)
            return 0;

        else
            return 4;

        break;
    default:
        return 0;
    }
}

bool CTDIB::isExternalsSets() const
{
    return pExternalAlloc != NULL &&
            pExternalFree != NULL &&
            pExternalLock != NULL &&
            pExternalUnlock != NULL;
}

Bool32 CTDIB::GetRGBQuad(uint32_t wQuad, PCTDIBRGBQUAD pQuad) {
    PCTDIBRGBQUAD pCurrentQuad;

    if (pRGBQuads == NULL)
        return FALSE;

    if (wQuad > GetActualColorNumber()) {
        return FALSE;
    }

    pCurrentQuad = (PCTDIBRGBQUAD) GetPtrToRGB();
    pCurrentQuad += wQuad;
    *pQuad = *pCurrentQuad;
    return TRUE;
}

void * CTDIB::GetPtrToBitFild() {
    CTDIB_IFNODIB(NULL);
    return pBitFild;
}

CTDIB::CTDIBVersion CTDIB::GetVersion(void) {
    CTDIB_IFNODIB(UnknownVersion);
    return wVersion;
}
///////////////////////////////////////////////////////////////////////////////////////////////
//
Bool32 CTDIB::SetFuelLineFromDIB(const CTDIB * pSrcDIB, uint32_t nSrcLine, uint32_t nDscLine,
        uint32_t wSrcX) {
    puchar pSrcStart = (puchar) pSrcDIB->GetPtrToPixel(wSrcX, nSrcLine);
    puchar pDscStart = (puchar) this->GetPtrToLine(nDscLine);
    puchar pBuffer;
    uint32_t wShift;

    if (pSrcDIB == NULL)
        return FALSE;

    if (pSrcStart == NULL || pDscStart == NULL)
        return FALSE;

    if ((pSrcDIB->GetLineWidth() - wSrcX) < this->GetLineWidth() )
        return FALSE;

    if (pSrcDIB->GetPixelSize() != this->GetPixelSize())
        return FALSE;

    uint32_t t = (pSrcDIB->GetUsedLineWidthInBytes() > this->GetUsedLineWidthInBytes() + ((wSrcX
            * GetPixelSize()) / 8)) ? 1 : 0;

    switch (this->GetPixelSize()) {
    case 1:
    case 4:
        //      pBuffer = new uchar[pSrcDIB->GetUsedLineWidthInBytes() + 1];
        pBuffer = new uchar[pSrcDIB->GetUsedLineWidthInBytes()];

        if (pBuffer == NULL)
            return FALSE;

        memset(pBuffer, 0, pSrcDIB->GetUsedLineWidthInBytes());
        //      memcpy ( pBuffer, pSrcStart, this->GetUsedLineWidthInBytes() + 1 );
        memcpy(pBuffer, pSrcStart, this->GetUsedLineWidthInBytes() + t);
        wShift = pSrcDIB->GetPixelShiftInByte(wSrcX);

        if (wShift) {
            //          uint32_t wByte  = this->GetUsedLineWidthInBytes() + 1;
            uint32_t wByte = this->GetUsedLineWidthInBytes() + t;
            puchar pwByte = pBuffer;
            uchar wShiftic = 0;

            //          while ( wByte-- )
            while (wByte - t > 0) {
                /*  Commemted by Andrey 11.04.2002
                 wShiftic = *(pwByte + 1);
                 *pwByte = *pwByte << wShift;
                 wShiftic = wShiftic >> (8 - wShift);
                 *pwByte |= wShiftic;
                 pwByte++;
                 */
                wByte--;

                if (wByte)
                    wShiftic = *(pwByte + 1);

                *pwByte = *pwByte << wShift;

                if (wByte) {
                    wShiftic = wShiftic >> (8 - wShift);
                    *pwByte |= wShiftic;
                    pwByte++;
                }
            }
        }

        memcpy(pDscStart, pBuffer, this->GetUsedLineWidthInBytes());
        delete[] pBuffer;
        break;
    case 8:
    case 16:
    case 24:
    case 32:
        memcpy(pDscStart, pSrcStart, this->GetUsedLineWidthInBytes());
        break;
    default:
        return FALSE;
    }

    return TRUE;
}

Bool32 CTDIB::CopyPalleteFromDIB(CTDIB *pSrcDIB) {
    CTDIBRGBQUAD Quad;
    uint32_t wQuad;
    uint32_t nColors = this->GetActualColorNumber();
    CTDIB_IFNODIB(FALSE);

    if (pSrcDIB->GetActualColorNumber() != nColors) {
        return FALSE;
    }

    for (wQuad = 0; wQuad < nColors; wQuad++) {
        if (!(pSrcDIB->GetRGBQuad(wQuad, &Quad)) || !(this->SetRGBQuad(wQuad, Quad)))
            return FALSE;
    }

    return TRUE;
}

Bool32 CTDIB::CopyDPIFromDIB(CTDIB *pSrcDIB) {
    uint32_t wX, wY;
    CTDIB_UNDECONST(FALSE);

    if (!pSrcDIB)
        return FALSE;

    if (pSrcDIB->GetResolutionDPM(&wX, &wY) && this->SetResolutionDPM(wX, wY))
        return TRUE;

    return FALSE;
}

uint32_t CTDIB::GetWhitePixel() {
    CTDIBRGBQUAD fQ;
    CTDIBRGBQUAD sQ;
    uint32_t i;
    uint32_t Color = 0;
    CTDIB_IFNODIB(0);

    switch (GetPixelSize()) {
    case 1:

        if (GetRGBQuad(0, &fQ) && GetRGBQuad(1, &sQ)) {
            if (equal(fQ, sQ))
                return 1;

            return firstQUADLighterThenSecond(sQ, fQ);
        }

        return 0;
        break;
    case 4:
    case 8:

        if (GetRGBQuad(0, &fQ)) {
            for (i = 1; i < GetActualColorNumber(); i++) {
                if (GetRGBQuad(i, &sQ))
                    if (firstQUADLighterThenSecond(sQ, fQ)) {
                        Color = i;
                        GetRGBQuad(i, &fQ);
                    }
            }
        }

        return Color;
        break;
    case 16:
        return 0x00007fff;
        break;
    case 24:
        return 0x00ffffff;
        break;
    case 32:
        return 0xffffffff;
        break;
    default:
        return 0;
    }
}

uint32_t CTDIB::GetBlackPixel() {
    CTDIBRGBQUAD fQ;
    CTDIBRGBQUAD sQ;
    uint32_t i;
    uint32_t Color = 0;
    CTDIB_IFNODIB(0);

    switch (GetPixelSize()) {
    case 1:

        if (GetRGBQuad(0, &fQ) && GetRGBQuad(1, &sQ))
            return firstQUADLighterThenSecond(fQ, sQ);

        return 0;
        break;
    case 4:
    case 8:

        if (GetRGBQuad(0, &fQ)) {
            for (i = 1; i < GetActualColorNumber(); i++) {
                if (GetRGBQuad(i, &sQ))
                    if (firstQUADLighterThenSecond(fQ, sQ)) {
                        Color = i;
                        GetRGBQuad(i, &fQ);
                    }
            }
        }

        return Color;
        break;
    case 16:
    case 24:
    case 32:
        return 0;
        break;
    default:
        return 0;
    }
}
