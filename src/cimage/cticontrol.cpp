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

#include "bitmask.h"
#include "ctiimage.h"
#include "cticontrol.h"
#include "ctimemory.h"
#include "ctiimageheader.h"
#include "ctimask.h"
#include "compat_defs.h"
#include "cimage_debug.h"
#include "common/ctdib.h"
#include "common/bmp.h"

namespace cf
{

static inline Size dibImageSize(const BitmapPtr dib)
{
    return Size(dib->biWidth, dib->biHeight);
}

static size_t dibSize(const BitmapPtr dib)
{
    CTDIB ctdib;

    if (!ctdib.setBitmap(dib))
        return 0;

    return ctdib.dibSize();
}

static inline bool isBitSet(puchar data, size_t n)
{
    const int BITS = 8;
    return data[(n)/BITS] & (0x80 >> ((n) % BITS));
}

bool CTIControl::writeDIBtoBMP(const std::string& name, CTDIB * pDIB)
{
    if(!pDIB) {
        CIMAGE_ERROR_FUNC << "NULL ctdib";
        return false;
    }

    BitmapPtr dib = NULL;
    pDIB->bitmap(&dib);

    if(!dib)
        return false;

    if(!pDIB->dibSize())
        return false;

    CIMAGE_DEBUG_FUNC << "dump to \"" << name << "\"";

    BitmapFileHeader bf;
    bf.bfSize = BMP_FILE_HEADER_SIZE + pDIB->dibSize();
    // fileheader + infoheader + palette
    bf.bfOffBits = BMP_FILE_HEADER_SIZE + pDIB->headerSize() + pDIB->palleteSize();

    std::ofstream of(name.c_str());
    of.write((char*) &bf, sizeof(bf));
    of.write((char*) dib, pDIB->dibSize());

    return !of.fail();
}

bool CTIControl::writeDIBtoBMP(const std::string& name, BitmapPtr h)
{
    CTDIB dib;
    dib.setBitmap(h);
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
    wCBStep = 0;
    cb_src_dib_ = NULL;
    cb_dest_dib_ = NULL;
    cb_write_src_dib_ = NULL;
    cb_write_dest_dib_ = NULL;
    last_dib_ = NULL;
    last_raw_data_ = NULL;
    memory_errors_ = 0;
}

CTIControl::~CTIControl() {
    clear();
}

bool CTIControl::addRectToReadMask(const std::string& name, const Rect &r)
{
    CTIImageHeader * img = images_.image(name);

    if(!img) {
        CIMAGE_ERROR_FUNC << "image not found:" << name;
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
        CIMAGE_ERROR_FUNC << "image not found:" << name;
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
        CIMAGE_ERROR_FUNC << "image not found:" << name;
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
        CIMAGE_ERROR_FUNC << "image not found:" << name;
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

bool CTIControl::applyReadMask(const std::string& name)
{
    CTIImageHeader * img = images_.image(name);

    if(!img) {
        CIMAGE_ERROR_FUNC << "image not found:" << name;
        return false;
    }

    if(!img->imageHandle()) {
        CIMAGE_ERROR_FUNC << "invalid image handle:" << name;
        return false;
    }

    if(img->isReadMaskEnabled() && img->readMask()) {
        CIMAGE_DEBUG_FUNC << "mask applied:" << name;
        applyMaskToHandle(img->imageHandle(), img->readMask(), 0, 0);
    }

    return true;
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
    images_.clear();
    freeBuffers();
    delete cb_src_dib_;
    delete cb_write_src_dib_;
    delete cb_dest_dib_;
    delete cb_write_dest_dib_;
}

void CTIControl::reset() {
    clear();
    init();
}

bool CTIControl::writeImageCallbacks(const std::string& name, CIMAGEIMAGECALLBACK cbk)
{
    // Создаем новый DIB
    if (cb_write_dest_dib_ != NULL || cb_write_src_dib_ != NULL)
        return false;

    // этот в качестве буфера для закидывания по одной строке
    cb_write_src_dib_ = new CTDIB;
    // а этот будет основным
    cb_write_dest_dib_ = new CTDIB;

    CIMAGE_ImageInfo image_info;
    // закачиваем информацию о создаваемом
    bool ret = cbk.CIMAGE_ImageOpen(&image_info);

    if (!ret) {
        CIMAGE_ERROR_FUNC << "unable to use import callback";
        return false;
    }

    if (image_info.wImageByteWidth == 0 || image_info.wImageHeight == 0) {
        CIMAGE_ERROR_FUNC << "unable to use import callback";
        return false;
    }

    bool bInvert = false;

    if (cb_write_dest_dib_->createBegin(image_info.wImageWidth, image_info.wImageHeight, 1)
            && cb_write_dest_dib_->setResolutionDotsPerInch(image_info.wResolutionX, image_info.wResolutionY)
            && cb_write_dest_dib_->createEnd())
    {
        if (cb_write_src_dib_->createBegin(image_info.wImageWidth, 1, 1)
                && cb_write_src_dib_->createEnd()) {
            if (image_info.bFotoMetrics == 0)
                bInvert = true;

            cb_write_dest_dib_->makeBlackAndWhitePallete();
            cb_write_src_dib_->makeBlackAndWhitePallete();

            // заполняем структурку  для замещения строк
            CIMAGE_InfoDataInReplace frame_to_replace;
            frame_to_replace.byBit = (uint16_t) cb_write_src_dib_->bpp();
            frame_to_replace.dwX = 0;
            frame_to_replace.dwY = 0;
            frame_to_replace.dwWidth = cb_write_src_dib_->lineWidth();
            frame_to_replace.dwHeight = cb_write_src_dib_->linesNumber();
            frame_to_replace.wByteWidth = (uint16_t) cb_write_src_dib_->lineWidthInBytes();
            frame_to_replace.lpData = (puchar) cb_write_src_dib_->lineAt(0);

            // Заполняем его
            for (uint i = 0; i < image_info.wImageHeight; i++) {
                // вызываем второй калбэк
                uint Readed = cbk.CIMAGE_ImageRead((pchar) cb_write_src_dib_->lineAt(0),
                                                   (uint16_t) cb_write_src_dib_->lineWidthInBytes());

                //инвертируем битовое поле, ежели надо
                if (bInvert) {
                    pchar bits = (pchar) cb_write_src_dib_->lineAt(0);

                    for (uint j = 0; j < cb_write_src_dib_->lineWidthInBytes(); j++) {
                        *bits = ~(*bits);
                        bits++;
                    }
                }

                // номер строчки
                frame_to_replace.dwY = i;

                // записваем строчку в новый из буфера
                if (Readed) {
                    setFrame(cb_write_src_dib_, cb_write_dest_dib_, &frame_to_replace);
                }
            }

            // Закрываем
            // вызываем третий калбэк
            ret = cbk.CIMAGE_ImageClose();
            // отписываем в контейнер
            BitmapPtr new_dib;
            cb_write_dest_dib_->bitmap(&new_dib);
            // Пишем картинку, и не каких масок!!!
            disableReadMask(name);
            ret = addImageCopy(name, new_dib);
            enableReadMask(name);

            if (!ret)
                return false;
        }
    }

    if (cb_write_src_dib_) {
        delete cb_write_src_dib_;
        cb_write_src_dib_ = NULL;
    }

    if (cb_write_dest_dib_) {
        delete cb_write_dest_dib_;
        cb_write_dest_dib_ = NULL;
    }

    return ret;
}

bool CTIControl::getImageCallbacks(const std::string& name, CIMAGEIMAGECALLBACK * cbk)
{
    if (!cbk) {
        CIMAGE_ERROR_FUNC << "NULL callback given";
        return false;
    }

    BitmapPtr hImage = image(name);
    if (!hImage)
        return false;

    cbk->CIMAGE_ImageOpen = CIMAGE_Callback_ImageOpen;
    cbk->CIMAGE_ImageRead = CIMAGE_Callback_ImageRead;
    cbk->CIMAGE_ImageClose = CIMAGE_Callback_ImageClose;

    wCBStep = 0;
    hCBImage = hImage;

    return true;
}

bool CTIControl::addImage(const std::string& name, BitmapPtr handle)
{
    return images_.addImage(name, handle, true);
}

bool CTIControl::addImageCopy(const std::string& name, BitmapPtr handle) {
    BitmapPtr image = NULL;

    if (!copyDIB(handle, &image)) {
        CIMAGE_ERROR_FUNC << "unable copy DIB:" << name;
        return false;
    }

    return images_.addImage(name, image, false);
}

bool CTIControl::addImageCopy(const std::string& name, const CTDIB * dib)
{
    if(!dib)
        return false;

    BitmapPtr handle = NULL;

    if(!dib->bitmap(&handle))
        return false;

    return addImageCopy(name, handle);
}

bool CTIControl::dumpImage(const std::string& name, const std::string& fileName)
{
    CTIImageHeader * img = images_.image(name);
    if(!img) {
        CIMAGE_ERROR_FUNC << "image not found:" << name;
        return false;
    }

    CTDIB dib;
    if(!dib.setBitmap(img->imageHandle()))
        return false;

    return writeDIBtoBMP(fileName, &dib);
}

BitmapPtr CTIControl::image(const std::string& name)
{
    CTIImageHeader * img = images_.image(name);

    if(!img) {
        CIMAGE_ERROR_FUNC << "image not found:" << name;
        return NULL;
    }

    return img->imageHandle();
}

CTDIB * CTIControl::imageDib(const std::string& name)
{
    BitmapPtr handle = image(name);

    if(!handle) {
        CIMAGE_ERROR_FUNC << "image not found:" << name;
        return NULL;
    }

    CTDIB * res = new CTDIB;

    if (!res->setBitmap(handle)) {
        CIMAGE_ERROR_FUNC << "invalid image:" << name;
        delete res;
        return NULL;
    }

    return res;

}

BitmapPtr CTIControl::imageCopy(const std::string& name)
{
    CTIImageHeader * img = images_.image(name);

    if(!img) {
        CIMAGE_ERROR_FUNC << "image not found:" << name;
        return NULL;
    }

    BitmapPtr src_handle = img->imageHandle();

    if(!src_handle)
        return NULL;

    BitmapPtr dest_handle = NULL;

    if(!copyDIB(src_handle, &dest_handle)) {
        CIMAGE_ERROR_FUNC << "unable copy dib:" << name;
        return NULL;
    }

    if(img->isReadMaskEnabled() && img->readMask()) {
        if(!applyMaskToHandle(dest_handle, img->readMask(), 0, 0))
            return NULL;
    }

    return dest_handle;
}

std::list<std::string> CTIControl::images() const
{
    return images_.imageNames();
}

bool CTIControl::getImageRawData(const std::string& name,
                            CIMAGE_InfoDataInGet * in,
                            CIMAGE_InfoDataOutGet * out)
{
    BitmapPtr dib;

    if (!getDIBFromImage(name, Rect(in->dwX, in->dwY, in->dwWidth, in->dwHeight), NULL, &dib)) {
        CIMAGE_ERROR_FUNC << "can't get image";
        out->lpData = NULL;
        return false;
    }

    CTDIB dest;

    if (!dest.setBitmap(dib)) {
        CIMAGE_ERROR_FUNC << "invalid image data";
        out->lpData = NULL;
        return false;
    }

    if (in->wByteWidth >= dest.lineRealWidthInBytes() &&
            in->dwWidth == dest.lineWidth() &&
            in->dwHeight == dest.linesNumber()) {

        out->byBit = (uint16_t) dest.bpp();
        out->dwHeight = dest.linesNumber();
        out->dwWidth = dest.lineWidth();
        out->wByteWidth = (uint16_t) dest.lineRealWidthInBytes();
        out->wBlackBit = dest.blackPixel();

        last_raw_data_ = new uchar[in->wByteWidth * in->dwHeight];
        out->lpData = last_raw_data_;
        puchar out_line = last_raw_data_;

        for (size_t i = 0; i < out->dwHeight; i++) {
            memcpy(out_line, dest.lineAt(i), out->wByteWidth);
            out_line += out->wByteWidth;
        }

        return true;
    }

    CIMAGE_ERROR_FUNC << "invalid input params";
    out->lpData = NULL;
    return false;
}

bool CTIControl::removeImage(const std::string& name)
{
    return images_.removeImage(name);
}

bool CTIControl::copyDIB(const BitmapPtr src, BitmapPtr * dest)
{
    const size_t dib_size = dibSize(src);

    BitmapPtr new_image = (BitmapPtr) malloc(dib_size);

    if (!new_image) {
        CIMAGE_ERROR_FUNC << "malloc failed";
        return false;
    }

    memcpy(new_image, src, dib_size);

    *dest = new_image;
    return true;
}

bool CTIControl::applyMask(const std::string& name, int x, int y)
{
    CTIImageHeader * img = images_.image(name);

    if(!img) {
        CIMAGE_ERROR_FUNC << "image not found" << name;
        return false;
    }

    if(img->isReadMaskEnabled() && img->readMask()) {
        if(!applyMaskToDIB(last_dib_, img->readMask(), x, y))
            return false;
    }

    return true;
}

bool CTIControl::getFrame(const CTDIB * src, CTDIB * dest, const Rect& frame)
{
    if (src == NULL || dest == NULL) {
        CIMAGE_ERROR_FUNC << "empty image containers";
        return false;
    }

    if ((src->lineWidth() < dest->lineWidth() + frame.left())
            || (src->linesNumber() < dest->linesNumber() + frame.top())) {
        return false;
    }

    //копируем из одного диба в другой
    if (!copyToDestinationFrame(src, dest, frame.left(), frame.top(), frame.height())) {
        CIMAGE_ERROR_FUNC << "copy failed";
        return false;
    }

    return true;
}

bool CTIControl::copyToDestinationFrame(const CTDIB * src, CTDIB * dest, uint xOffset, uint yOffset, uint lineCount)
{
    if (src->bpp() != dest->bpp()) {
        CIMAGE_ERROR_FUNC << "images depth is not equal";
        return false;
    }

    uint dest_line = 0;

    for (uint i = 0; i < lineCount; i++) {
        if (!dest->copyLineFromDIB(src, yOffset + i, dest_line++, xOffset)) {
            CIMAGE_ERROR_FUNC << "invalid frame size";
            return false;
        }
    }

    return true;
}

bool CTIControl::copyFromSourceFrame(const CTDIB * src, CTDIB * dest, uint destYOffset, uint lineCount)
{
    if (src->bpp() != dest->bpp()) {
        CIMAGE_ERROR_FUNC << "image depth is not equal";
        return false;
    }

    if (dest->lineWidth() != src->lineWidth()) {
        CIMAGE_ERROR_FUNC << "line width not equal";
        return false;
    }

    uint dest_start_y = destYOffset;
    uint end_y = dest_start_y + lineCount;
    uint src_line = 0;

    for (uint dest_line = dest_start_y; dest_line < end_y; dest_line++) {
        if (!dest->copyLineFromDIB(src, src_line++, dest_line, 0)) {
            CIMAGE_ERROR_FUNC << "invalid frame size";
            return false;
        }
    }

    return true;
}

bool CTIControl::setFrame(const CTDIB * src, CTDIB * dest, CIMAGE_InfoDataInReplace * pIn)
{
    if (src == NULL || dest == NULL || !pIn) {
        CIMAGE_ERROR_FUNC << "invalid image containers";
        return false;
    }

    // проверяем на ненулевость
    uint frame_size = pIn->dwHeight * pIn->dwWidth;

    if (frame_size == 0) {
        CIMAGE_ERROR_FUNC << "invalid frame size";
        return false;
    }

    // Проверяем на соответствие размеров
    if ((dest->lineWidth() < src->lineWidth() + pIn->dwX)
            || (dest->linesNumber() < src->linesNumber() + pIn->dwY)) {
        CIMAGE_ERROR_FUNC << "invalid frame size";
        return false;
    }

    if (!copyFromSourceFrame(src, dest, pIn->dwY, pIn->dwHeight)) {
        CIMAGE_ERROR_FUNC << "invalid images container";
        return false;
    }

    return true;
}

Bool32 CTIControl::CBImageOpen(CIMAGE_ImageInfo * info)
{
    uint wFrgb;
    uint wSrgb;
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

    pCBImage = hCBImage;

    if (cb_dest_dib_ != NULL || cb_src_dib_ != NULL) {
        return FALSE;
    }

    cb_src_dib_ = new CTDIB;
    cb_dest_dib_ = new CTDIB;

    if (cb_src_dib_->setBitmap(pCBImage)) {
        wCBBufferSize = cb_src_dib_->lineRealWidthInBytes();
        int width = cb_src_dib_->lineWidth();
        wCBStep++;

        Handle hDescDIB;

        if ((hDescDIB = cb_dest_dib_->createBegin(width, 1, cb_src_dib_->bpp()))
                && cb_dest_dib_->createEnd())
        {
            info->wImageWidth = (uint16_t) cb_src_dib_->lineWidth();
            info->wImageHeight = (uint16_t) cb_src_dib_->linesNumber();
            info->wImageByteWidth = (uint16_t) wCBBufferSize;
            info->wImageDisplacement = 0;
            cb_src_dib_->resolutionDotsPerInch(&wFrgb, &wSrgb);
            info->wResolutionX = (uint16_t) wFrgb;
            info->wResolutionY = (uint16_t) wSrgb;
            info->bFotoMetrics = (uchar) cb_src_dib_->whitePixel();
            info->wAddX = 0;
            info->wAddY = 0;
            wCBStep++;
        }
    }

    wCBLine = 0;
    return TRUE;
}

uint32_t CTIControl::CBImageRead(char * buff, uint32_t maxSize)
{
    char * pNextLineInBuffer = buff;
#ifndef CIMAGE_CBR_ONE_LINE
    const uint lines_at_once = maxSize / wCBBufferSize;
#else
    const uint lines_at_once = 1;
#endif

    if (!pCBImage)
        return 0;

    if (!(maxSize < wCBBufferSize)) {
        Rect frame(0, 0, cb_dest_dib_->lineWidth(), cb_dest_dib_->linesNumber());
        uint CopiedBytes = 0;

        for (uint out_line = 0; out_line < lines_at_once; out_line++) {
            frame.moveYTo(wCBLine);

            if (cb_dest_dib_->lineAt(0) != NULL) {
                if (getFrame(cb_src_dib_, cb_dest_dib_, frame)) {
                    wCBLine++;
                    wCBStep += 10;
                    memcpy(pNextLineInBuffer, cb_dest_dib_->lineAt(0),
                           wCBBufferSize);
                    CopiedBytes += wCBBufferSize;
                    pNextLineInBuffer += wCBBufferSize;
                }
            }
        }

        return CopiedBytes;
    }

    return 0;
}

Bool32 CTIControl::CBImageClose()
{
    if (pCBBuffer) {
        delete[] pCBBuffer;
        pCBBuffer = NULL;
    }

    if (cb_src_dib_) {
        delete cb_src_dib_;
        cb_src_dib_ = NULL;
    }

    if (cb_dest_dib_) {
        delete cb_dest_dib_;
        cb_dest_dib_ = NULL;
    }

    hCBImage = NULL;
    pCBBuffer = NULL;
    wCBBufferSize = 0;
    wCBLine = 0;
    wCBStep = 0;
    return TRUE;
}

bool CTIControl::free(BitmapPtr handle)
{
    if (!handle) {
        CIMAGE_ERROR_FUNC << "NULL handle";
        return false;
    }

    if (images_.hasHandle(handle)) {
        CIMAGE_ERROR_FUNC << "invalid handle";
        return false;
    }

    // if image was stored in last_dib_
    BitmapPtr saved_handle = NULL;
    if(last_dib_ && last_dib_->bitmap(&saved_handle)) {
        if(saved_handle == handle)
            last_dib_->reset();
    }

    ::free(handle);

    return true;
}

bool CTIControl::getDIBFromImage(const std::string& name, const Rect& r, BitMask * bitMask,
                                   BitmapPtr * dest)
{
    // в случае повторного вызова предыдущий диб стирается
    freeBuffers();

    BitmapPtr dib = image(name);

    if (!dib) {
        CIMAGE_ERROR_FUNC << "image not found:" << name;
        return false;
    }

    //инициализируем ctdib-чики
    CTDIB src;
    last_dib_ = new CTDIB;

    uint res_x = 0;
    uint res_y = 0;

    if (!src.setBitmap(dib) && src.resolutionDotsPerMeter(&res_x, &res_y)) {
        CIMAGE_ERROR_FUNC << "invalid image:" << name;
        delete last_dib_;
        last_dib_ = NULL;
        return false;
    }

    // проверяем размеры картинки
    Rect frame = checkRect(src, r);

    if (last_dib_->createBegin(frame.width(), frame.height(), src.bpp())
            && last_dib_->copyPalleteFromDIB(&src)
            && last_dib_->setResolutionDotsPerMeter(res_x, res_y)
            && last_dib_->createEnd())
    {
        if(!getFrame(&src, last_dib_, frame)) {
            delete last_dib_;
            last_dib_ = NULL;
            CIMAGE_ERROR_FUNC << "getFrame failed";
            return false;
        }

        if(bitMask) {
            if(!bitMask->apply(last_dib_)) {
                CIMAGE_ERROR_FUNC << "bit mask apply failed";
//                return false;
            }
        }

        if(!applyMask(name, frame.left(), frame.top())) {
            delete last_dib_;
            last_dib_ = NULL;
            CIMAGE_ERROR_FUNC << "read mask apply failed";
            return false;
        }
    } else {
        CIMAGE_ERROR_FUNC << "unable create image";
        delete last_dib_;
        last_dib_ = NULL;
        return false;
    }

    if (last_dib_->bitmap(dest)) {
        return true;
    }
    else {
        *dest = NULL;
        return false;
    }
}

void CTIControl::freeBuffers()
{
    if (last_raw_data_) {
        delete[] last_raw_data_;
        last_raw_data_ = NULL;
    }

    if (last_dib_) {
        delete last_dib_;
        last_dib_ = NULL;
    }
}

Rect CTIControl::checkRect(const CTDIB& dib, const Rect& r)
{
    Rect frame(0, 0, dib.lineWidth(), dib.linesNumber());

    if(frame.intersects(r))
        return frame.intersected(r);

    return frame;
}

bool CTIControl::applyMaskToDIB(CTDIB * dib, CTIMask * mask, int at_x, int at_y)
{
    if (!dib) {
        CIMAGE_ERROR_FUNC << "NULL dib";
        return false;
    }

    if(!mask) {
        CIMAGE_ERROR_FUNC << "NULL mask";
        return false;
    }

    int x_begin = at_x;
    int y_begin = at_y;
    int x_end = x_begin + dib->lineWidth();
    int y_end = y_begin + dib->linesNumber();

    Rect dib_area(Point(x_begin, y_begin), Point(x_end, y_end));

    if (!mask->isRectOnMask(dib_area)) {
        CIMAGE_ERROR_FUNC << "mask not on dib:" << dib_area;
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
                    CIMAGE_ERROR_FUNC << "unable apply mask";
                    return false;
                }
            }
            else
                break;
        }
    }

    return true;
}

bool CTIControl::applyMaskToHandle(BitmapPtr handle, CTIMask * mask, int at_x, int at_y)
{
    CTDIB ctdib;

    if(!ctdib.setBitmap(handle))
        return false;

    return applyMaskToDIB(&ctdib, mask, at_x, at_y);
}

static bool fillLineWhiteDIB_color(CTDIB * dib, int x_start, int x_end, int y)
{
    if(!dib)
        return false;

    puchar pixel_begin = static_cast<puchar>(dib->pixelAt(x_start, y));
    puchar pixel_end = static_cast<puchar>(dib->pixelAt(x_end, y));

    if(!pixel_begin || !pixel_end) {
        CIMAGE_ERROR_FUNC << "invalid pixels";
        return false;
    }

    const size_t segm_length = ((x_end - x_start) * dib->bpp()) / 8;
    memset(pixel_begin, dib->whitePixel(), segm_length);
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

    puchar pixel_begin = static_cast<puchar>(dib->pixelAt(x_start, y));
    puchar pixel_end = static_cast<puchar>(dib->pixelAt(x_end, y));

    if(!pixel_begin || !pixel_end) {
        CIMAGE_ERROR_FUNC << "invalid pixels";
        return false;
    }

    uint pixel_begin_shift = dib->pixelShiftInByte(x_start);
    uint pixel_end_shift = dib->pixelShiftInByte(x_end);
    uchar white_pixel = (uchar) dib->whitePixel();

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
    const uint bit_count = dib->bpp();

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
        CIMAGE_ERROR_FUNC << "unsupported bit count" << bit_count;
        return false;
    }
}

bool CTIControl::applyMaskToDIBLine(CTDIB * dib, CTIMaskLineSegment * segm, int line, int at_x, int at_y)
{
    if ((segm->start() > int(dib->lineWidth() + at_x)) ||
            (segm->end() > int(dib->lineWidth() + at_x)) ||
            (line >= int(dib->linesNumber() + at_y)))
        return false;

    const int x_begin = segm->start() - at_x;
    int x_end = segm->end() - at_x;
    const int y = line - at_y;

    // проверяем на соответствие с картинкой
    if (x_begin >= (int) dib->lineWidth())
        return true;

    if (x_end >= (int) dib->lineWidth())
        x_end = dib->lineWidth() - 1;

    return fillLineWhite(dib, x_begin, x_end, y);
}

}
