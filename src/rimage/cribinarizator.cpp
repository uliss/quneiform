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

#include <cstdlib>
#include <cmath>

#include "resource.h"
#include "cribinarizator.h"
#include "crimemory.h"
#include "rprogressor.h"
#include "gdata.h"
#include "rimage_debug.h"
#include "common/ctdib.h"

namespace cf {

CRIBinarizator::CRIBinarizator()
{
    init();
}

CRIBinarizator::CRIBinarizator(CRProgressor * pProgressIndicator)
{
    init();
    mpProgressor = pProgressIndicator;
}

CRIBinarizator::~CRIBinarizator()
{
}

bool CRIBinarizator::setRasters(CTDIB * src, CTDIB * dest)
{
    if (src && dest) {
        mpIncomeDIB = src;
        mpOutcomeDIB = dest;

        if ((mwSrcBitCount = mpIncomeDIB->bpp()) < 4) {
            RIMAGE_ERROR << " source image already binarized\n";
            return false;
        }

        if (mpOutcomeDIB->bpp() != 1) {
            RIMAGE_ERROR << " destination image should be 1-bit depth\n";
            return false;
        }

        if ((mwLineLenght = mpIncomeDIB->lineWidth())
                != mpOutcomeDIB->lineWidth()) {
            SetReturnCode_rimage(IDS_RIMAGE_OUTCOME_DIB_NOT_LINK_TO_INCOME);
            return false;
        }

        if (!supportedIndexColorImage(mpIncomeDIB))
            RIMAGE_ERROR << " unsupported indexed format in source image\n";

        return true;
    }

    return false;
}

void CRIBinarizator::init()
{
    meBinType = CTBIN_UNKNOWN;
    mpIncomeDIB = NULL;
    mpOutcomeDIB = NULL;
    mpszGreyBuffer = NULL;
    mpDezaBinarizator = NULL;
    mhszGreyBuffer = NULL;
    mpszGreyBuffer = NULL;
    mpProgressor = NULL;
    mbIndexColor = false;
    mfBlueK = ((float) (mwBlueK = 70)) / 255;
    mfGreenK = ((float) (mwGreenK = 130)) / 255;
    mfRedK = ((float) (mwRedK = 220)) / 255;
}

Bool32 CRIBinarizator::Binarize(CTBinarize eBinType, uint32_t wFlag)
{
    Bool32 bRet = FALSE;

    if (!mpIncomeDIB || !mpOutcomeDIB)
        return FALSE;

    meBinType = eBinType;

    try {
        if (!OpenBinarizator(wFlag))
            return FALSE;

        if (!OnBinarizator()) {
            CloseBinarizator();
            return FALSE;
        }

        CloseBinarizator();
        bRet = TRUE;
    }

    catch (uint32_t wExit) {
        if (wExit == IDS_RIMAGE_EXIT_BY_USER) {
            SetReturnCode_rimage(IDS_RIMAGE_EXIT_BY_USER);
        }
        else {
            SetReturnCode_rimage(IDS_RIMAGE_UNKNOWN_ERROR);
        }

        bRet = FALSE;
    }

    return bRet;
}

Bool32 CRIBinarizator::OpenBinarizator(uint32_t wFlag)
{
    Bool32 bRet = FALSE;

    switch (meBinType) {
            //open Deza
        case CTBIN_DEZA:

            if (!mpDezaBinarizator) {
                mpDezaBinarizator = new CDezaBinarizator;
            }

            bRet = DezaOpenBin(wFlag);
            break;
            //Open Kronrod
        case CTBIN_KRONROD:
            /*
             if ( !mpKronrodBinarizator )
             {
             mpKronrodBinarizator = new CKronrodBinarizator;
             }
             */
            mwGreyBufferSize = mpIncomeDIB->lineWidth() + 8;

            if (!(mhszGreyBuffer = RIMAGEDAlloc(mwGreyBufferSize,
                                                "Binarizator - grey buffer")) || !(mpszGreyBuffer
                                                                                   = (puchar) RIMAGELock(mhszGreyBuffer)))
                return FALSE;

            bRet = KronrodOpenBin(mpIncomeDIB->linesNumber(),
                                  mpIncomeDIB->lineWidth());
            break;
        default:
            return FALSE;
    }

    return bRet;
}

Bool32 CRIBinarizator::CloseBinarizator()
{
    switch (meBinType) {
            //close Deza////////////////////////////////
        case CTBIN_DEZA:
            DezaCloseBin();

            if (mpDezaBinarizator) {
                delete mpDezaBinarizator;
                mpDezaBinarizator = NULL;
            }

            break;
            //close Kronrod/////////////////////////////
        case CTBIN_KRONROD:
            KronrodCloseGray();

            if (mpszGreyBuffer) {
                RIMAGEUnlock(mhszGreyBuffer);
                mpszGreyBuffer = NULL;
            }

            if (mhszGreyBuffer)
                RIMAGEFree(mhszGreyBuffer);

            mhszGreyBuffer = NULL;
            /*
             if ( mpKronrodBinarizator)
             {
             delete mpKronrodBinarizator;
             mpKronrodBinarizator = NULL;
             }
             */
            break;
            ////////////////////////////////////////////
        default:
            return FALSE;
    }

    return TRUE;
}

Bool32 CRIBinarizator::OnBinarizator()
{
    Bool32 bRet = FALSE;

    switch (meBinType) {
        case CTBIN_DEZA:
        case CTBIN_KRONROD:
            bRet = OnBinarizeLoop();
            break;
        default:
            return FALSE;
    }

    return bRet;
}

Bool32 CRIBinarizator::OnBinarizeLoop()
{
    Bool32 bRet = FALSE;
    int32_t i;
    puchar pLALine;
    int32_t NumberBWLines = 0;
    int32_t CurGreyLine = 0;
    uint32_t nLines = mpOutcomeDIB->linesNumber();

    /////////////////////////////////////////////////////////////////////////
    //   см. G2BW.cpp, дшту 67 - Binarize
    //   особое внимание для Kronrod-а
    switch (meBinType) {
            // DEZA line binarize//////////////////////////////////////////
        case CTBIN_DEZA:
            i = 0;

            do {
                pLALine = (puchar) mpOutcomeDIB->lineAt(i++);

                if (!pLALine)
                    break;
            }
            while (mpDezaBinarizator->GetBinarized(pLALine,
                                                   (uint16_t) (mpOutcomeDIB->lineRealWidthInBytes()))); // was Outcome

            bRet = (i == ((int32_t) nLines + 1));
            break;
            // Kranrod line binarize///////////////////////////////////////
        case CTBIN_KRONROD:
            i = 0;
            CurGreyLine = 0;

            if(mpProgressor)
                mpProgressor->Start();

            while (i < (int32_t) nLines) {
                pLALine = (puchar) mpOutcomeDIB->lineAt(i++);

                if (!NumberBWLines) {
                    do {
                        if (!KronrodImageRead(mpszGreyBuffer,
                                              (int16_t) CurGreyLine++, (int16_t) 1))
                            return 0;

                        if (mpProgressor && mpProgressor->SetStep(((CurGreyLine / nLines) * 50))) {
                            mpProgressor->Finish();
                            throw IDS_RIMAGE_EXIT_BY_USER;
                        }
                    }
                    while ((NumberBWLines = KronrodGreyTo(mpszGreyBuffer), !NumberBWLines));
                }

                KronrodGreyFrom(pLALine);
                NumberBWLines--;

                if (mpProgressor && mpProgressor->SetStep(((i / nLines) * 50) + 50)) {
                    throw IDS_RIMAGE_EXIT_BY_USER;
                }
            }

            if(mpProgressor)
                mpProgressor->Finish();
            bRet = TRUE;
            break;
        default:
            return FALSE;
    }

    return bRet;
}

Bool32 CRIBinarizator::KronrodOpenBin(uint32_t wHeight, uint32_t wWeidth)
{
    grey_open((uint16_t) wHeight, (uint16_t) wWeidth);
    mbKronrodInvert = TRUE;
    return TRUE;
}

int16_t CRIBinarizator::KronrodImageRead(puchar lpImage, int16_t fstLine,
                                         int16_t nLines)
{
    return (int16_t) KronrodImageRead(lpImage, (int32_t) fstLine,
                                      (int32_t) nLines);
}

int32_t CRIBinarizator::KronrodImageRead(puchar lpImage, int32_t fstLine,
                                         int32_t nLines)
{
    uint32_t i;
    uint32_t j;
    uint32_t ReadedPixelsperLine = 0;
    uint32_t ReadedLines = 0;
    uint32_t wFirstLine = fstLine;
    uint32_t wLines = nLines;
    uchar gray;
    uchar halfgray;
    puchar pIn;

    if (!lpImage)
        return 0;

    if ((wFirstLine + wLines) > mpIncomeDIB->linesNumber())
        return 0;

    switch (mwSrcBitCount) {
        case 8:

            for (i = 0; i < wLines; i++) {
                ReadedPixelsperLine = 0;

                for (j = 0; j < mwLineLenght; j++) {
                    if (!(pIn
                            = (puchar) mpIncomeDIB->pixelAt(j, fstLine + i /*- 1*/)))
                        break;

                    gray = mbIndexColor ? wIndex8ToGray[pIn[0]] : pIn[0];
                    *(lpImage + mwLineLenght * i + j) = gray;
                    ReadedPixelsperLine++;
                }

                ReadedLines += (ReadedPixelsperLine / mwLineLenght);
            }

            break;
        case 4:

            for (i = 0; i < wLines; i++) {
                ReadedPixelsperLine = 0;

                for (j = 0; j < mwLineLenght; j += 2) {
                    if (!(pIn = (puchar) mpIncomeDIB->pixelAt(j, fstLine + i)))
                        break;

                    gray = *pIn;
                    halfgray = mbIndexColor ? wIndex4ToGray[((gray & 0xF0) >> 4)]
                               : ((gray & 0xF0) >> 4);
                    *(lpImage + mwLineLenght * i + j) = halfgray;
                    halfgray = mbIndexColor ? wIndex4ToGray[(gray & 0x0F)] : (gray
                                                                              & 0x0F);
                    *(lpImage + mwLineLenght * i + j + 1) = halfgray;
                    ReadedPixelsperLine += 2;
                }

                ReadedLines += (ReadedPixelsperLine / mwLineLenght);
            }

            break;
        case 24:

            for (i = 0; i < wLines; i++) {
                ReadedPixelsperLine = 0;

                for (j = 0; j < mwLineLenght; j++) {
                    if (!(pIn = (puchar) mpIncomeDIB->pixelAt(j, fstLine + i)))
                        break;

                    gray = ((pIn[0] + pIn[1] + pIn[2]) / 3);
                    *(lpImage + mwLineLenght * i + j) = gray;
                    ReadedPixelsperLine++;
                }

                ReadedLines += (ReadedPixelsperLine / mwLineLenght);
            }

            break;
        default:
            return 0;
    }

    return ReadedLines;//nLines;
}

Bool32 CRIBinarizator::KronrodGreyTo(puchar pGTo)
{
    uint32_t Size = mwGreyBufferSize;
    uint32_t i;

    if (mbKronrodInvert) {
        for (i = 0; i < Size; i++)
            *(pGTo + i) = ~(*(pGTo + i));
    }

    return grey_to(pGTo);
}

Bool32 CRIBinarizator::KronrodGreyFrom(puchar pGFrom)
{
    uint32_t Size = mpOutcomeDIB->lineWidthInBytes();
    uint32_t i;
    grey_from(pGFrom);

    if (mbKronrodInvert) {
        for (i = 0; i < Size; i++)
            *(pGFrom + i) = ~(*(pGFrom + i));
    }

    return TRUE;
}

Bool32 CRIBinarizator::KronrodCloseGray()
{
    //mpKronrodBinarizator->grey_close();
    grey_close();
    return TRUE;
}
Bool32 CRIBinarizator::DezaOpenBin(uint32_t wDezaFlag)
{
    uint wDpiX, wDpiY;
    pvoid fGetGreyBlock;
    fGetGreyBlock = (pvoid) this;
#ifdef DEZA_TIG_IMAGEINFO
    TIG_IMAGEINFO DezaImageInfo;
    DezaImageInfo.wImageHeight = (uint16_t)mpIncomeDIB->GetLinesNumber();
    DezaImageInfo.wImageWidth = (uint16_t)mpIncomeDIB->GetLineWidth();
    DezaImageInfo.wImageByteWidth = (uint16_t)mpIncomeDIB->GetUsedLineWidthInBytes();
    DezaImageInfo.wImageDisplacement = 0;
    mpIncomeDIB->GetResolutionDPI(&wDpiX, &wDpiY);
    DezaImageInfo.wResolutionX = (uint16_t)wDpiX;
    DezaImageInfo.wResolutionY = (uint16_t)wDpiY;
    DezaImageInfo.bFotoMetrics = 1;
    DezaImageInfo.wAddX = 0;
    DezaImageInfo.wAddY = 0;
    return OpenTrackBin(&DezaImageInfo, fGetGreyBlock, (uint16_t)wDezaFlag);
#else
    mDezaImageInfo.wImageHeight = (uint16_t) mpIncomeDIB->linesNumber();
    mDezaImageInfo.wImageWidth = (uint16_t) mpIncomeDIB->lineWidth();
    mDezaImageInfo.wImageByteWidth
    = (uint16_t) mpIncomeDIB->lineRealWidthInBytes();
    mDezaImageInfo.wImageDisplacement = 0;
    mpIncomeDIB->resolutionDotsPerInch(&wDpiX, &wDpiY);
    mDezaImageInfo.wResolutionX = (uint16_t) wDpiX;
    mDezaImageInfo.wResolutionY = (uint16_t) wDpiY;
    mDezaImageInfo.bFotoMetrics = 1;
    mDezaImageInfo.wAddX = 0;
    mDezaImageInfo.wAddY = 0;
    return mpDezaBinarizator->OpenTrackBin(&mDezaImageInfo, this, wDezaFlag);
#endif
}

Bool32 CRIBinarizator::DezaCloseBin()
{
    mpDezaBinarizator->CloseTrackBin();
    return TRUE;
}

bool CRIBinarizator::supportedIndexColorImage(CTDIB * dib)
{
    uint colors = dib->palleteUsedColorsNum();
    uint pallette_size = dib->palleteSize();

    if (pallette_size == 0 || colors == 2 || colors > 256)
        return true;

    RGBQuad Q, prQ;
    uint i;
    mbIndexColor = false;

    for (i = 1; i < colors; i++) {
        if (!dib->palleteColor(i, &Q) || !dib->palleteColor(i - 1, &prQ))
            return false;

        if ((Q.rgbBlue < prQ.rgbBlue) ||
                (Q.rgbGreen < prQ.rgbGreen)
                || (Q.rgbRed < prQ.rgbRed))
            break;
    }

    if (i == colors)
        return true;

    PrepareIndexTable(dib);
    return true;
}

Bool32 CRIBinarizator::PrepareIndexTable(CTDIB * dib)
{
    puchar pTable = NULL;

    switch (dib->bpp()) {
        case 4:
            pTable = wIndex4ToGray;
            break;
        case 8:
            pTable = wIndex8ToGray;
            break;
        default:
            return false;
    }

    uint Colors = (dib->palleteSize()) / 4;
    RGBQuad Quad;

    for (uint i = 0; i < Colors; i++) {
        if (!dib->palleteColor(i, &Quad))
            return false;

        pTable[i] = IndexPalleteToGray(&Quad);
    }

    mbIndexColor = true;
    return true;
}

uchar CRIBinarizator::IndexPalleteToGray(RGBQuad * pQuad)
{
    float b = ((float) (pQuad->rgbBlue) * 70) / 255;
    float g = ((float) (pQuad->rgbGreen) * 220) / 255;
    float r = ((float) (pQuad->rgbRed) * 130) / 255;
    return (uchar) sqrt((((b * b) + (g * g) + (r * r)) / 70200) * 65025);
}

}
