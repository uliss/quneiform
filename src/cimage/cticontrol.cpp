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

#include <cstdio>
#include <cassert>
#include <cstring>
#include <fstream>
#include <boost/current_function.hpp>

#include "resource.h"
#include "bitmask.h"
#include "ctiimage.h"
#include "cticontrol.h"
#include "ctimemory.h"
#include "ctiimageheader.h"
#include "ctimask.h"
#include "dpuma.h"
#include "common/debug.h"
#include "common/tostring.h"
#include "rdib/ctdib.h"
#include "compat_defs.h"

#define CIMAGE_ERROR Debug() << "[CIMAGE] ERROR " << BOOST_CURRENT_FUNCTION
#define CIMAGE_DEBUG Debug() << "[CIMAGE] " << BOOST_CURRENT_FUNCTION

namespace cf
{

static inline Size dibImageSize(const BitmapHandle dib)
{
    return Size(dib->biWidth, dib->biHeight);
}

static size_t dibSize(const BitmapHandle dib)
{
    CTDIB ctdib;

    if (!ctdib.SetDIBbyPtr(dib))
        return 0;

    return ctdib.GetDIBSize();
}

static inline bool isBitSet(puchar data, size_t n)
{
    const int BITS = 8;
    return data[(n)/BITS] & (0x80 >> ((n) % BITS));
}

bool CTIControl::writeDIBtoBMP(const std::string& name, CTDIB * pDIB)
{
    if(!pDIB) {
        CIMAGE_ERROR << " NULL ctdib\n";
        return false;
    }

    Handle dib = NULL;
    pDIB->GetDIBPtr(&dib);

    if(!dib) {
        return false;
    }

    if(!pDIB->GetDIBSize())
        return false;

    CIMAGE_DEBUG << " dump to \"" << name << "\"\n";

    BITMAPFILEHEADER bf;
    memset(&bf, 0, sizeof(BITMAPFILEHEADER));
    bf.bfType = 0x4d42; // 'BM'
    bf.bfSize = sizeof(BITMAPFILEHEADER) + pDIB->GetDIBSize();
    // fileheader + infoheader + palette
    bf.bfOffBits = sizeof(BITMAPFILEHEADER) + pDIB->GetHeaderSize() + pDIB->GetRGBPalleteSize();

    std::ofstream of(name.c_str());
    of.write((char*) &bf, sizeof(bf));
    of.write((char*) dib, pDIB->GetDIBSize());

    return !of.fail();
}

bool CTIControl::writeDIBtoBMP(const std::string& name, BitmapHandle h)
{
    CTDIB dib;
    dib.SetDIBbyPtr(h);
    return writeDIBtoBMP(name, &dib);
}

CTIControl::CTIControl() {
    init();
}

void CTIControl::init() {
    hCBImage = NULL;
    pCBImage = NULL;
    pCBBuffer = NULL;
    wCBBufferSize = 0;
    wCBLine = 0;
    wCBLines = 0;
    wCBWidth = 0;
    wCBStep = 0;
    mCBSourceDIB = NULL;
    mCBDestianationDIB = NULL;
    mCBWSourceDIB = NULL;
    mCBWDestianationDIB = NULL;
    image_dib_ = NULL;
    image_raw_data_ = NULL;
    mwMemoryErrors = 0;
}

CTIControl::~CTIControl() {
    clear();
}

bool CTIControl::addRectToReadMask(const std::string& name, const Rect &r)
{
    CTIImageHeader * img = images_.image(name);

    if(!img) {
        CIMAGE_ERROR << " image not found: " << name << "\n";
        return false;
    }

    // already have mask
    if(img->readMask()) {
        return img->readMask()->addRectangle(r);
    }
    else {
        Size img_size = dibImageSize(img->imageHandle());
        CTIMask * mask = new CTIMask(img_size.width(), img_size.height());
        img->setReadMask(mask);
        return mask->addRectangle(r);
    }
}

bool CTIControl::addRectToWriteMask(const std::string& name, const Rect &r)
{
    CTIImageHeader * img = images_.image(name);

    if(!img) {
        CIMAGE_ERROR << " image not found: " << name << "\n";
        return false;
    }

    // already have mask
    if(img->writeMask()) {
        return img->writeMask()->addRectangle(r);
    }
    else {
        Size img_size = dibImageSize(img->imageHandle());
        CTIMask * mask = new CTIMask(img_size.width(), img_size.height());
        img->setWriteMask(mask);
        return mask->addRectangle(r);
    }
}

bool CTIControl::removeRectFromReadMask(const std::string& name, const Rect& r)
{
    CTIImageHeader * img = images_.image(name);

    if(!img) {
        CIMAGE_ERROR << " image not found: " << name << "\n";
        return false;
    }

    // already have mask
    if(img->readMask()) {
        return img->readMask()->removeRectangle(r);
    }
    else {
        Size img_size = dibImageSize(img->imageHandle());
        CTIMask * mask = new CTIMask(img_size.width(), img_size.height());
        img->setReadMask(mask);
        return mask->removeRectangle(r);
    }
}

bool CTIControl::removeRectFromWriteMask(const std::string& name, const Rect& r)
{
    CTIImageHeader * img = images_.image(name);

    if(!img) {
        CIMAGE_ERROR << " image not found: " << name << "\n";
        return false;
    }

    // already have mask
    if(img->writeMask()) {
        return img->writeMask()->removeRectangle(r);
    }
    else {
        Size img_size = dibImageSize(img->imageHandle());
        CTIMask * mask = new CTIMask(img_size.width(), img_size.height());
        img->setWriteMask(mask);
        return mask->removeRectangle(r);
    }
}

bool CTIControl::disableReadMask(const std::string& imageName)
{
    return images_.disableReadMask(imageName);
}

bool CTIControl::disableWriteMask(const std::string& imageName)
{
    return images_.disableWriteMask(imageName);
}

bool CTIControl::enableReadMask(const std::string& imageName)
{
    return images_.enableReadMask(imageName);
}

bool CTIControl::enableWriteMask(const std::string& imageName)
{
    return images_.enableWriteMask(imageName);
}

void CTIControl::clear() {
    freeBuffers();
    delete mCBSourceDIB;
    delete mCBWSourceDIB;
    delete mCBDestianationDIB;
    delete mCBWDestianationDIB;
}

void CTIControl::reset() {
    clear();
    init();
}

bool CTIControl::writeImageCallbacks(const std::string& name, CIMAGEIMAGECALLBACK cbk)
{
    Handle hNewDIB;
    Bool32 Ret;
    Bool32 bInvert = false;
    CIMAGE_InfoDataInReplace FrameToReplace;
    CIMAGE_ImageInfo ImageInfo;

    ImageInfo.wImageWidth = 0;
    ImageInfo.wImageByteWidth = 0;
    ImageInfo.wImageHeight = 0;

    //////////////////////////////////////////////////////////////////////////////////
    // Создаем новый DIB
    if (mCBWDestianationDIB != NULL || mCBWSourceDIB != NULL) {
        return FALSE;
    }

    // этот в качестве буфера для закидывания по одной строке
    mCBWSourceDIB = new CTDIB;
    // а этот будет основным
    mCBWDestianationDIB = new CTDIB;
    ///////////////////////////////////////////////////////////////////////////////////
    // закачиваем информацию о создаваемом
    Ret = cbk.CIMAGE_ImageOpen(&ImageInfo);

    if (Ret == FALSE) {
        SetReturnCode_cimage(IDS_CIMAGE_UNABLE_USE_IMPORT_CALLBACK);
        return FALSE;
    }

    if (ImageInfo.wImageByteWidth == 0 || ImageInfo.wImageHeight == 0) {
        SetReturnCode_cimage(IDS_CIMAGE_UNABLE_USE_IMPORT_CALLBACK);
        return FALSE;
    }

    // выделение памяти под новый DIB и под буферный
    if (mCBWDestianationDIB->SetExternals(CIMAGEAlloc, CIMAGEFree, CIMAGELock, CIMAGEUnlock)
            && mCBWSourceDIB->SetExternals(CIMAGEAlloc, CIMAGEFree, CIMAGELock, CIMAGEUnlock)) {
        CIMAGEComment(name.c_str());

        if (mCBWDestianationDIB->CreateDIBBegin(ImageInfo.wImageWidth, ImageInfo.wImageHeight, 1)
                && mCBWDestianationDIB->SetResolutionDPI(ImageInfo.wResolutionX,
                        ImageInfo.wResolutionY) && mCBWDestianationDIB->CreateDIBEnd()) {
            CIMAGEComment("Temporary DIB - WriteCBImage");

            if (mCBWSourceDIB->CreateDIBBegin(ImageInfo.wImageWidth, 1, 1)
                    && mCBWSourceDIB->CreateDIBEnd()) {
                CTDIBRGBQUAD cdFQuad;
                CTDIBRGBQUAD cdSQuad;
                cdFQuad.rgbBlue = cdFQuad.rgbGreen = cdFQuad.rgbRed = 0x00;
                cdSQuad.rgbBlue = cdSQuad.rgbGreen = cdSQuad.rgbRed = 0xff;

                if (ImageInfo.bFotoMetrics == 0) {
                    bInvert = true;
                }

                mCBWDestianationDIB->SetRGBQuad(0, cdFQuad);
                mCBWSourceDIB->SetRGBQuad(0, cdFQuad);
                mCBWDestianationDIB->SetRGBQuad(1, cdSQuad);
                mCBWSourceDIB->SetRGBQuad(1, cdSQuad);
                // заполняем структурку  для замещения строк
                FrameToReplace.byBit = (uint16_t) mCBWSourceDIB->GetPixelSize();
                FrameToReplace.dwX = 0;
                FrameToReplace.dwY = 0;
                FrameToReplace.dwWidth = (uint16_t) mCBWSourceDIB->GetLineWidth();
                FrameToReplace.dwHeight = (uint16_t) mCBWSourceDIB->GetLinesNumber();
                FrameToReplace.wByteWidth = (uint16_t) mCBWSourceDIB->GetLineWidthInBytes();
                FrameToReplace.lpData = (puchar) mCBWSourceDIB->GetPtrToLine(0);
                FrameToReplace.MaskFlag = 0;

                // Заполняем его
                for (uint i = 0; i < ImageInfo.wImageHeight; i++) {
                    // вызываем второй калбэк
                    uint Readed = cbk.CIMAGE_ImageRead((pchar) mCBWSourceDIB->GetPtrToLine(0),
                            (uint16_t) mCBWSourceDIB->GetLineWidthInBytes());

                    //инвертируем битовое поле, ежели надо
                    if (bInvert) {
                        pchar pBits = (pchar) mCBWSourceDIB->GetPtrToLine(0);
                        int32_t Ii;

                        for (Ii = 0; Ii < (uint16_t) mCBWSourceDIB->GetLineWidthInBytes(); Ii++) {
                            *pBits++ = ~(*pBits);
                        }
                    }

                    // номер строчки
                    FrameToReplace.dwY = i;

                    // записваем строчку в новый из буфера
                    if (Readed) {
                        setFrame(mCBWSourceDIB, mCBWDestianationDIB, &FrameToReplace);
                    }
                }

                // Закрываем
                // вывзываем третий калбэк
                Ret = cbk.CIMAGE_ImageClose();
                // отписываем в контейнер
                mCBWDestianationDIB->GetDIBHandle(&hNewDIB);
                // Пишем картинку, и не каких масок!!!
                disableReadMask(name);
                Ret = addImageCopy(name, (BitmapHandle) hNewDIB);
                enableReadMask(name);

                if (Ret == FALSE) {
                    mCBWInProcess = FALSE;
                    return FALSE;
                }
            }
        }
    }

    if (mCBWSourceDIB) {
        mCBWSourceDIB->DestroyDIB();
        delete mCBWSourceDIB;
        mCBWSourceDIB = NULL;
    }

#ifdef CIMAGE_DUMP_ENABLE
    //WriteDIBtoBMP("Allex.CBWrited.bmp", mCBWDestianationDIB);
#endif

    if (mCBWDestianationDIB) {
        mCBWDestianationDIB->DestroyDIB();
        delete mCBWDestianationDIB;
        mCBWDestianationDIB = NULL;
    }

    return Ret;
}

bool CTIControl::getImageCallbacks(const std::string& name, CIMAGEIMAGECALLBACK * cbk)
{
    if (!cbk) {
        CIMAGE_ERROR << " NULL callback given\n";
        return false;
    }

    BitmapHandle hImage = image(name);
    if (!hImage)
        return false;

    cbk->CIMAGE_ImageOpen = CIMAGE_Callback_ImageOpen;
    cbk->CIMAGE_ImageRead = CIMAGE_Callback_ImageRead;
    cbk->CIMAGE_ImageClose = CIMAGE_Callback_ImageClose;

    wCBStep = 0;
    hCBImage = hImage;

    return true;
}

bool CTIControl::addImage(const std::string& name, BitmapHandle handle)
{
    return images_.addImage(name, handle, true);
}

bool CTIControl::addImageCopy(const std::string& name, BitmapHandle handle) {
    BitmapHandle image = NULL;

    if (!copyDIB(handle, &image)) {
        CIMAGE_ERROR << " unable copy DIB: " << name << "\n";
        return false;
    }

    return images_.addImage(name, image, false);
}

bool CTIControl::dumpImage(const std::string& name, const std::string& fileName)
{
    CTIImageHeader * img = images_.image(name);
    if(!img)
        return false;

    CTDIB dib;
    if(!dib.SetDIBbyPtr(img->imageHandle()))
        return false;

    return writeDIBtoBMP(fileName, &dib);
}

BitmapHandle CTIControl::image(const std::string& name)
{
    CTIImageHeader * img = images_.image(name);

    if(!img) {
        CIMAGE_ERROR << " image not found: \"" << name << "\"\n";
        return NULL;
    }

    return img->imageHandle();
}

BitmapHandle CTIControl::imageCopy(const std::string& name)
{
    CTIImageHeader * img = images_.image(name);

    if(!img) {
        CIMAGE_ERROR << " image not found: \"" << name << "\"\n";
        return NULL;
    }

    BitmapHandle src_handle = img->imageHandle();

    if(!src_handle)
        return NULL;

    BitmapHandle dest_handle = NULL;

    if(!copyDIB(src_handle, &dest_handle)) {
        CIMAGE_ERROR << " unable copy dib: " << name << "\n";
        return NULL;
    }

    if(img->isReadMaskEnabled() && img->readMask()) {
        if(!applyMaskToHandle(dest_handle, img->readMask(), 0, 0))
            return NULL;
    }

    return dest_handle;
}

bool CTIControl::getImageRawData(const std::string& name,
                            CIMAGE_InfoDataInGet * in,
                            CIMAGE_InfoDataOutGet * out)
{
    BitmapHandle dib;

    if (!getDIBFromImage(name, Rect(in->dwX, in->dwY, in->dwWidth, in->dwHeight), NULL, &dib)) {
        CIMAGE_ERROR << " can't get image\n";
        out->lpData = NULL;
        return false;
    }

    CTDIB dest;

    if (!dest.SetDIBbyPtr(dib)) {
        CIMAGE_ERROR << " invalid image data\n";
        out->lpData = NULL;
        return false;
    }

    if (in->wByteWidth >= dest.GetUsedLineWidthInBytes() &&
            in->dwWidth == dest.GetLineWidth() &&
            in->dwHeight == dest.GetLinesNumber()) {

        out->byBit = (uint16_t) dest.GetPixelSize();
        out->dwHeight = dest.GetLinesNumber();
        out->dwWidth = dest.GetLineWidth();
        out->wByteWidth = (uint16_t) dest.GetUsedLineWidthInBytes();
        out->wBlackBit = dest.GetBlackPixel();

        image_raw_data_ = new uchar[in->wByteWidth * in->dwHeight];
        out->lpData = image_raw_data_;
        puchar out_line = image_raw_data_;

        for (size_t i = 0; i < out->dwHeight; i++) {
            memcpy(out_line, dest.GetPtrToLine(i), out->wByteWidth);
            out_line += out->wByteWidth;
        }

        return true;
    }

    CIMAGE_ERROR << " invalid input params\n";
    out->lpData = NULL;
    return false;
}

bool CTIControl::removeImage(const std::string& name)
{
    return images_.removeImage(name);
}

bool CTIControl::copyDIB(const BitmapHandle src, BitmapHandle * dest)
{
    const size_t dib_size = dibSize(src);

    BitmapHandle new_image = (BitmapHandle) CIMAGEDAlloc(dib_size, "Copyed DIB - CopyDIB");

    if (!new_image) {
        CIMAGE_ERROR << " CIMAGEDAlloc failed.\n";
        return false;
    }

    memcpy(new_image, src, dib_size);

    *dest = new_image;
    return true;
}

bool CTIControl::applyMask(const std::string &name, int x, int y)
{
    CTIImageHeader * img = images_.image(name);

    if(!img) {
        CIMAGE_ERROR << " image not found\n";
        return false;
    }

    if(img->isReadMaskEnabled() && img->readMask()) {
        if(!applyMaskToDIB(image_dib_, img->readMask(), x, y))
            return false;
    }

    return true;
}

bool CTIControl::getFrame(const CTDIB * src, CTDIB * dest, const Rect& frame)
{
    if (src == NULL || dest == NULL) {
        CIMAGE_ERROR << ": empty image containers\n";
        return false;
    }

    if ((src->GetLineWidth() < dest->GetLineWidth() + frame.left())
            || (src->GetLinesNumber() < dest->GetLinesNumber() + frame.top())) {
        return false;
    }

    //копируем из одного диба в другой
    if (!copyToDestinationFrame(src, dest, frame.left(), frame.top(), frame.height())) {
        CIMAGE_ERROR << ": copy failed\n";
        return false;
    }

    return true;
}

bool CTIControl::copyToDestinationFrame(const CTDIB * src, CTDIB * dest, uint xOffset, uint yOffset, uint lineCount)
{
    if (src->GetPixelSize() != dest->GetPixelSize()) {
        CIMAGE_ERROR << " images depth is not equal.\n";
        return false;
    }

    uint dest_line = 0;

    for (uint i = 0; i < lineCount; i++) {
        if (!dest->SetFuelLineFromDIB(src, yOffset + i, dest_line++, xOffset)) {
            CIMAGE_ERROR << " invalid frame size.\n";
            return false;
        }
    }

    return true;
}

bool CTIControl::copyFromSourceFrame(const CTDIB * src, CTDIB * dest, uint destYOffset, uint lineCount)
{
    if (src->GetPixelSize() != dest->GetPixelSize()) {
        CIMAGE_ERROR << " image depth is not equal\n";
        return false;
    }

    if (dest->GetLineWidth() != src->GetLineWidth()) {
        CIMAGE_ERROR << " line width not equal\n";
        return false;
    }

    uint dest_start_y = destYOffset;
    uint end_y = dest_start_y + lineCount;
    uint src_line = 0;

    for (uint dest_line = dest_start_y; dest_line < end_y; dest_line++) {
        if (!dest->SetFuelLineFromDIB(src, src_line++, dest_line, 0)) {
            CIMAGE_ERROR << " invalid frame size\n";
            return false;
        }
    }

    return true;
}

bool CTIControl::setFrame(const CTDIB * src, CTDIB * dest, CIMAGE_InfoDataInReplace * pIn)
{
    if (src == NULL || dest == NULL || !pIn) {
        CIMAGE_ERROR << " invalid image containers\n";
        return false;
    }

    // проверяем на ненулевость
    uint frame_size = pIn->dwHeight * pIn->dwWidth;

    if (frame_size == 0) {
        CIMAGE_ERROR << " invalid frame size\n";
        return false;
    }

    // Проверяем на соответствие размеров
    if ((dest->GetLineWidth() < src->GetLineWidth() + pIn->dwX)
            || (dest->GetLinesNumber() < src->GetLinesNumber() + pIn->dwY)) {
        SetReturnCode_cimage(IDS_CIMAGE_INVALID_FRAME_SIZE);
        return false;
    }

    if (!copyFromSourceFrame(src, dest, pIn->dwY, pIn->dwHeight)) {
        SetReturnCode_cimage(IDS_CIMAGE_INVALID_IMAGES_CONTAINER);
        return false;
    }

    return true;
}

Bool32 CTIControl::CBImageOpen(CIMAGE_ImageInfo * lpImageInfo) {
    uint32_t wFrgb;
    uint32_t wSrgb;
#if defined( _TRASING ) & defined ( _DEBUG )

    if ( wCBStep != 0 ) {
        TRACE("!!!CIMAGE!!! Повторный вызов CBImageOpen для картинки %s без переинициализации CIMAGE_CBImageRead!\n", mCBName);
    }

#endif
    wCBStep++;
#if defined( _TRASING ) & defined ( _DEBUG )

    if ( mCBWInProcess == TRUE ) {
        TRACE("!!!CIMAGE!!! CIMAGE_CBImageRead( %s, ...) вызывается из CIMAGE_WriteCBImage( %s, ...)!\n", mCBName, mCBWName);
    }

#endif

    if (!hCBImage) {
        return FALSE;
    }

    pCBImage = CIMAGELock(hCBImage);

    if (mCBDestianationDIB != NULL || mCBSourceDIB != NULL) {
        return FALSE;
    }

    mCBSourceDIB = new CTDIB;
    mCBDestianationDIB = new CTDIB;

    if (mCBSourceDIB->SetDIBbyPtr(pCBImage)) {
        wCBBufferSize = mCBSourceDIB->GetUsedLineWidthInBytes();
        wCBWidth = mCBSourceDIB->GetLineWidth();
        wCBStep++;
        CIMAGEComment("Temporary DIB - CBImageOpen");

        Handle hDescDIB;

        if (mCBDestianationDIB->SetExternals(CIMAGEAlloc, CIMAGEFree, CIMAGELock, CIMAGEUnlock)
                && (hDescDIB = mCBDestianationDIB->CreateDIBBegin(wCBWidth, 1,
                        mCBSourceDIB->GetPixelSize())) && mCBDestianationDIB->CreateDIBEnd()) {
            wCBLines = mCBSourceDIB->GetLinesNumber();
            lpImageInfo->wImageWidth = (uint16_t) mCBSourceDIB->GetLineWidth();
            lpImageInfo->wImageHeight = (uint16_t) wCBLines;
            lpImageInfo->wImageByteWidth = (uint16_t) wCBBufferSize;
            lpImageInfo->wImageDisplacement = 0;
            mCBSourceDIB->GetResolutionDPI(&wFrgb, &wSrgb);
            lpImageInfo->wResolutionX = (uint16_t) wFrgb;
            lpImageInfo->wResolutionY = (uint16_t) wSrgb;
            lpImageInfo->bFotoMetrics = (uchar) mCBSourceDIB->GetWhitePixel();
            lpImageInfo->wAddX = 0;
            lpImageInfo->wAddY = 0;
            wCBStep++;
        }
    }

    wCBLine = 0;
    return TRUE;
}

uint32_t CTIControl::CBImageRead(char * lpBuff, uint32_t wMaxSize)
{
    uint32_t LinesAtOnce;
    char* pNextLineInBuffer = lpBuff;
#ifndef CIMAGE_CBR_ONE_LINE
    LinesAtOnce = wMaxSize / wCBBufferSize;
#else
    LinesAtOnce = 1;
#endif

    if (pCBImage) {
        if (!(wMaxSize < wCBBufferSize)) {
            Rect frame(0, 0, mCBDestianationDIB->GetLineWidth(), mCBDestianationDIB->GetLinesNumber());
            uint CopiedBytes = 0;

            for (uint nOutLine = 0; nOutLine < LinesAtOnce; nOutLine++) {
                frame.moveYTo(wCBLine);

                if (mCBDestianationDIB->GetPtrToLine(0) != NULL) {
                    if (getFrame(mCBSourceDIB, mCBDestianationDIB, frame)) {
                        wCBLine++;
                        wCBStep += 10;
                        memcpy(pNextLineInBuffer, mCBDestianationDIB->GetPtrToLine(0),
                                wCBBufferSize);
                        CopiedBytes += wCBBufferSize;
                        pNextLineInBuffer += wCBBufferSize;
                    }
                }
            }

            return CopiedBytes;
        }
    }

    return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
Bool32 CTIControl::CBImageClose(void) {
    if (hCBImage)
        CIMAGEUnlock(hCBImage);

    if (pCBBuffer) {
        delete[] pCBBuffer;
        pCBBuffer = NULL;
    }

    if (mCBSourceDIB) {
        delete mCBSourceDIB;
        mCBSourceDIB = NULL;
    }

    if (mCBDestianationDIB) {
        mCBDestianationDIB->DestroyDIB();
        delete mCBDestianationDIB;
        mCBDestianationDIB = NULL;
    }

    hCBImage = NULL;
    pCBBuffer = NULL;
    wCBBufferSize = 0;
    wCBLines = 0;
    wCBLine = 0;
    wCBWidth = 0;
    wCBStep = 0;
    return TRUE;
}

bool CTIControl::free(BitmapHandle handle)
{
    if (!handle) {
        CIMAGE_ERROR << " NULL handle\n";
        return false;
    }

    if (images_.hasHandle(handle)) {
        CIMAGE_ERROR << " invalid handle\n";
        return false;
    }

    CIMAGEFree(handle);

    return true;
}

bool CTIControl::getDIBFromImage(const std::string& name, const Rect& r, BitMask * bitMask,
                                   BitmapHandle * dest)
{
    // в случае повторного вызова предыдущий диб стирается
    freeBuffers();

    BitmapHandle dib = image(name);

    if (!dib) {
        CIMAGE_ERROR << " image not found: \"" << name << "\"\n";
        return false;
    }

    //инициализируем ctdib-чики
    CTDIB src;
    image_dib_ = new CTDIB;

    uint32_t res_x = 0;
    uint32_t res_y = 0;

    if (!src.SetDIBbyPtr(dib) && src.GetResolutionDPM(&res_x, &res_y)) {
        CIMAGE_ERROR << ": invalid image\n";
        return false;
    }

    // проверяем размеры картинки
    Rect frame = checkRect(src, r);

    if (!image_dib_->SetExternals(CIMAGEAlloc, CIMAGEFree, CIMAGELock, CIMAGEUnlock)) {
        CIMAGE_ERROR << ": SetExternals failed\n";
        return false;
    }

    if (image_dib_->CreateDIBBegin(frame.width(), frame.height(),
                                       src.GetPixelSize(), src.GetActualColorNumber())
            && image_dib_->CopyPalleteFromDIB(&src)
            && image_dib_->SetResolutionDPM(res_x, res_y)
            && image_dib_->CreateDIBEnd())
    {
        if(!getFrame(&src, image_dib_, frame)) {
            CIMAGE_ERROR << ": getFrame failed\n";
            return false;
        }

        if(bitMask) {
            if(!bitMask->apply(image_dib_)) {
                CIMAGE_ERROR << " bit mask apply failed\n";
//                return false;
            }
        }

        if(!applyMask(name, frame.left(), frame.top())) {
            CIMAGE_ERROR << ": read mask apply failed\n";
            return false;
        }

    } else {
        CIMAGE_ERROR << " unable create image\n";
        return false;
    }

    if (image_dib_->GetDIBPtr((void**) dest))
        return true;
    else {
        *dest = NULL;
        return false;
    }
}

void CTIControl::freeBuffers()
{
    if (image_raw_data_) {
        delete[] image_raw_data_;
        image_raw_data_ = NULL;
    }

    if (image_dib_) {
        if (image_dib_->DestroyDIB())
            delete image_dib_;
        else
            mwMemoryErrors++;

        image_dib_ = NULL;
    }
}

Rect CTIControl::checkRect(const CTDIB& dib, const Rect& r)
{
    Rect frame(0, 0, dib.GetLineWidth(), dib.GetLinesNumber());

    if(frame.intersects(r))
        return frame.intersected(r);

    return frame;
}

bool CTIControl::applyMaskToDIB(CTDIB * dib, CTIMask * mask, int at_x, int at_y)
{
    if (!dib) {
        CIMAGE_ERROR << " NULL dib\n";
        return false;
    }

    if(!mask) {
        CIMAGE_ERROR << " NULL mask\n";
        return false;
    }

    int x_begin = at_x;
    int y_begin = at_y;
    int x_end = x_begin + dib->GetLineWidth();
    int y_end = y_begin + dib->GetLinesNumber();

    Rect dib_area(Point(x_begin, y_begin), Point(x_end, y_end));

    if (!mask->isRectOnMask(dib_area)) {
        CIMAGE_ERROR << "mask not on dib: " << dib_area << "\n";
        return false;
    }

    for (int y = y_begin; y < y_end; y++) {
        CTIMaskLine * mask_line;
        if (!mask->GetLine(y, &mask_line))
            continue;

        //если удалось открыть маску
        if (!mask_line)
            continue;

        // если есть линия
       int segment_num = mask_line->segmentsNumber();

        if (!segment_num)
            continue;

        // если есть сегменты на этой линии
        int x = x_begin;
        while (x < x_end) {
            CTIMaskLineSegment segm(x, x_end);

            if (mask_line->getLeftIntersection(&segm)) {
                x = segm.end() + 1;

                if (!applyMaskToDIBLine(dib, &segm, y, at_x, at_y)) {
                    CIMAGE_ERROR << " unable apply mask\n";
                    return false;
                }
            }
            else
                break;
        }
    }

    return true;
}

bool CTIControl::applyMaskToHandle(BitmapHandle handle, CTIMask * mask, int at_x, int at_y)
{
    CTDIB ctdib;

    if(!ctdib.SetDIBbyPtr(handle))
        return false;

    return applyMaskToDIB(&ctdib, mask, at_x, at_y);
}

static bool fillLineWhiteDIB_color(CTDIB * dib, int x_start, int x_end, int y)
{
    if(!dib)
        return false;

    puchar pixel_begin = static_cast<puchar>(dib->GetPtrToPixel(x_start, y));
    puchar pixel_end = static_cast<puchar>(dib->GetPtrToPixel(x_end, y));

    if(!pixel_begin || !pixel_end) {
        CIMAGE_ERROR << " invalid pixels\n";
        return false;
    }

    const size_t segm_length = ((x_end - x_start) * dib->GetPixelSize()) / 8;
    memset(pixel_begin, dib->GetWhitePixel(), segm_length);
    return true;
}

static bool fillLineWhiteDIB_1bit(CTDIB * dib, int x_start, int x_end, int y)
{
    static const uchar white_left_mask[8] = { 128, // 10000000
                                              192, // 11000000
                                              224, // 11100000
                                              240, // 11110000
                                              248, // 11111000
                                              252, // 11111100
                                              254, // 11111110
                                              255  // 11111111
                                            };

    static const uchar white_right_mask[8] = { 255, // 11111111
                                              127,  // 01111111
                                              63,   // 00111111
                                              31,   // 00011111
                                              15,   // 00001111
                                              7,    // 00000111
                                              3,    // 00000011
                                              1     // 00000001
                                            };

    static const uchar black_left_mask[8] = { 127, // 01111111
                                              63,  // 00111111
                                              31,  // 00011111
                                              15,  // 00001111
                                              7,   // 00000111
                                              3,   // 00000011
                                              1,   // 00000001
                                              0    // 00000000
                                            };

    static const uchar black_right_mask[8] = { 0,   // 00000000
                                               128, // 10000000
                                               192, // 11000000
                                               224, // 11100000
                                               240, // 11110000
                                               248, // 11111000
                                               252, // 11111100
                                               254  // 11111110
                                            };

    if(!dib)
        return false;

    puchar pixel_begin = static_cast<puchar>(dib->GetPtrToPixel(x_start, y));
    puchar pixel_end = static_cast<puchar>(dib->GetPtrToPixel(x_end, y));

    if(!pixel_begin || !pixel_end) {
        CIMAGE_ERROR << " invalid pixels\n";
        return false;
    }

    uint pixel_begin_shift = dib->GetPixelShiftInByte(x_start);
    uint pixel_end_shift = dib->GetPixelShiftInByte(x_end);
    uchar white_pixel = (uchar) dib->GetWhitePixel();

    int full_bits = pixel_end - pixel_begin - 1;

    if (full_bits > 0) {
        if (white_pixel == 0)
            memset(pixel_begin + 1, 0x00, full_bits);
        else
            memset(pixel_begin + 1, 0xff, full_bits);
    }

    if (white_pixel == 0) {
        *pixel_begin = (*pixel_begin) & black_right_mask[pixel_begin_shift];
        *pixel_end = (*pixel_end) & black_left_mask[pixel_end_shift];
    }
    else {
        *pixel_begin = (*pixel_begin) | white_right_mask[pixel_begin_shift];
        *pixel_end = (*pixel_end) | white_left_mask[pixel_end_shift];
    }

    return true;
}

static bool fillLineWhite(CTDIB * dib, int start, int end, int y)
{
    const uint bit_count = dib->GetPixelSize();

    switch(bit_count) {
    case 1:
        return fillLineWhiteDIB_1bit(dib, start, end, y);
    case 4:
        return false;
    case 8:
    case 16:
    case 24:
    case 32:
        return fillLineWhiteDIB_color(dib, start, end, y);
    default:
        CIMAGE_ERROR << " unsupported bit count: " << bit_count << "\n";
        return false;
    }
}

bool CTIControl::applyMaskToDIBLine(CTDIB * dib, CTIMaskLineSegment * segm, int line, int at_x, int at_y)
{
    if ((segm->start() > int(dib->GetLineWidth() + at_x)) ||
            (segm->end() > int(dib->GetLineWidth() + at_x)) ||
            (line >= int(dib->GetLinesNumber() + at_y)))
        return false;

    const int x_begin = segm->start() - at_x;
    int x_end = segm->end() - at_x;
    const int y = line - at_y;

    // проверяем на соответствие с картинкой
    if (x_begin >= (int) dib->GetLineWidth())
        return true;

    if (x_end >= (int) dib->GetLineWidth())
        x_end = dib->GetLineWidth() - 1;

    return fillLineWhite(dib, x_begin, x_end, y);
}

}
