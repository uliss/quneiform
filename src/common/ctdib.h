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

#include <iosfwd>
#include <string>

#include "globus.h"
#include "dib.h"

namespace cf {

class CLA_EXPO CTDIB
{
public:
    // DIB Version: 3, 4 or 5
    enum version_t {
        VERSION_UNKNOWN = 0,
        VERSION_WINDOWS = 3,
        VERSION_4,
        VERSION_5
    };
public:
    // empty constructor
    CTDIB();
    ~CTDIB();

    /**
      * Returns pixel color by given coordinate
      * @param x - x pixel coordinate
      * @param y - y pixel coordinate
      * @param dest - pointer to result
      * @return true on success
      * @see blackPixel(), whitePixel()
      */
    bool pixelColor(uint x, uint y, RGBQuad * dest) const;

    /**
     * Returns pixel color index
     * @return true on success
     */
    bool pixelColorIndex(uint x, uint y, uint * dest) const;

    /**
     * Sets pixel color
     * @see pixelColor(), fill()
     */
    bool setPixelColor(uint x, uint y, const RGBQuad& c);

    /**
     * Sets pixel color index
     */
    bool setPixelColorIndex(uint x, uint y, uint colorIdx);

    /**
     * Fills all image with given color
     */
    bool fill(const RGBQuad& c);

    /**
      * Returns black pixel RGBQuad index or 00-00-00
      * @see whitePixel()
      */
    uint32_t blackPixel() const;

    /**
     * Returns bits per pixel
     */
    size_t bpp() const;

    /**
      * Returns white pixel RGBQuad index or ff-ff-ff
      * @see blackPixel()
      */
    uint32_t whitePixel() const;

    /**
     * Copies resolution from another DIB at once
     */
    bool copyDPIFromDIB(const CTDIB * src);

    /**
     * Copies line from another DIB
     * @param src - pointer to source dib
     * @param srcLine - source line number
     * @param destLine - destination line number
     * @param srcX - source line pixel from which copy starts
     */
    bool copyLineFromDIB(const CTDIB * src, uint srcLine, uint destLine, uint srcX);

    /**
     * Copies fuel pallete from another DIB
     */
    bool copyPalleteFromDIB(const CTDIB * src);

    /**
     * Creates black and white pallete
     */
    bool makeBlackAndWhitePallete();

    /**
      * Returns DIB version by enum versions
      */
    version_t version() const;

    /**
     * Starts to create new DIB
     * @param width - image width in pixels
     * @param height - image height in pixels
     * @param bitCount - image depth in bits per pixel, valid values: 1, 4, 8, 16, 24, 32
     * @param version - DIB version
     * @see createEnd()
     */
    BitmapPtr createBegin(int width, int height, uint bitCount, version_t version = VERSION_WINDOWS);

    /**
     * Ends of DIB creation
     * @see createBegin()
     */
    bool createEnd();

    /**
     * Removes DIB and set to ready for new once
     */
    bool reset();

    /**
     * Returns true if image not attached to class
     * @see isValid()
     */
    bool isNull() const;

    /**
     * Returns true if image attached to class and false otherwise
     * @see isNull()
     */
    bool isValid() const;

    /**
     * Returns true if image in indexed color
     */
    bool isIndexed() const;

    /**
     * Returns allocated memory size for DIBHeader, RGBQuads and Image in bytes
     * @see headerSize(), palleteSize()
     */
    size_t dibSize() const;

    /**
     * Returns DIB header memory allocation size in bytes
     * @see header(), dibSize(), palleteSize()
     */
    size_t headerSize() const;

    /**
     * Returns image data size in bytes
     * To get full dib size use dibSize()
     * @see dibSize(), headerSize(), palleteSize()
     */
    size_t imageSizeInBytes() const;

    /**
     * Returns used by DIB RGBQuads memory allocation size in bytes
     * @see headerSize(), dibSize()
     */
    size_t palleteSize() const;

    /**
     * Returns true if pallete has color
     */
    bool palleteHasColor(const RGBQuad& c) const;

    /**
     * Returns color index in pallete or -1, if pallete has no such color
     */
    int palleteColorIndex(const RGBQuad& c) const;

    /**
     * Adds color to pallete
     * @return color index or -1 if failed
     */
    int addPalleteColor(const RGBQuad& c);

    /**
     * Returns number of used colors in pallete
     */
    uint palleteUsedColorsNum() const;

    /**
     * Returns pointer to DIB header
     * @see headerSize()
     */
    BitmapInfoHeader * header() const;

    /**
     * Returns image width in pixels
     * @see height()
     */
    int width() const;

    /**
     * Return image height in pixels
     * @note can be negative
     * @see width()
     */
    int height() const;

    /**
     * Return image height in pixels
     * @see height()
     */
    size_t linesNumber() const;

    /**
     * Returns image width in pixels
     * @see width(), height()
     */
    size_t lineWidth() const;

    /**
     * Returns image width in bytes forced to 4 (stride)
     * @see lineWidth(), lineRealWidthInBytes()
     */
    size_t lineWidthInBytes() const;

    /**
     * Returns image real line width in bytes without padding
     * @see lineWidthInBytes()
     */
    size_t lineRealWidthInBytes() const;

    /**
     * Returns bitmap resolution in dots per meter
     * @see resolutionDotsPerInch()
     */
    bool resolutionDotsPerMeter(uint * x, uint * y = 0) const;

    /**
     * Returns bitmap resolution in dots per inch
     * @see resolutionDotsPerMeter()
     */
    bool resolutionDotsPerInch(uint * x, uint * y = 0) const;

    /**
     * Sets resolution of DIB: x,[y].
     * @seesetResolutionDotsPerInch()
     * @note available if DIB constructed by CTDIB
     */
    bool setResolutionDotsPerMeter(uint x, uint y = 0);

    /**
     * Sets resolution of DIB: x,[y].
     * @see setResolutionDotsPerMeter()
     * @note available if DIB constructed by CTDIB
     */
    bool setResolutionDotsPerInch(uint x, uint y = 0);

    /**
     * Get pointer to bitmap
     * @see setBitmap()
     */
    bool bitmap(BitmapPtr * dib) const;

    /**
     * Sets DIB by memory ptr
     * @see bitmap
     */
    bool setBitmap(void * dib_);

    /**
     * Gets pallete color
     * @return false on error
     * @see setPalleteColor()
     */
    bool palleteColor(size_t idx, RGBQuad * dest) const;

    /**
     * Sets pallete color
     * @see palleteColor(), pallete(), palleteSize()
     */
    bool setPalleteColor(uint32_t idx, const RGBQuad& color);

    /**
     * Returns pointer to pallete - first RGBQuad of RGB Quads (or Triads)
     */
    void * pallete() const;

    /**
     * Returns pointer to raw image data
     * @see lineAt(), pixelAt()
     */
    void * imageData();

    /**
     * Returns pointer to Line (from 0 to height-1)
     * @see pixelAt(), imageData()
     * @return valid pointer on success or NULL on error
     */
    void * lineAt(uint y) const;

    /**
     * Returns pointer to BitField memory on pixel(x,y)
     * if bpp() < 8 - pointer to byte where its pixel
     * @see bpp(), lineAt()
     * @return valid pointer on success, NULL pointer on error
     */
    void * pixelAt(uint x, uint y) const;

    /**
     * Get bit position in byte of image field for pixel
     * @param xPos - x pixel coordinate
     */
    uint pixelShiftInByte(uint xPos) const;

    /**
     * Saves DIB to BMP format
     */
    bool saveToBMP(const std::string& fileName) const;
    bool saveToBMP(std::ostream& os) const;
public:
    static bool saveToBMP(const std::string& fileName, BitmapPtr bitmap);
    static bool saveToBMP(std::ostream& os, BitmapPtr bitmap);
private:
    enum direction_t {
        DIRECTION_TOP_DOWN = -1,
        DIRECTION_UNKNOWN = 0,
        DIRECTION_BOTTOM_UP
    };
private:
    // open DIB properties
    bool attachDIB();
    // close DIB properties
    void detachDIB();
    // free memory if DIB created by this class
    bool destroyDIB();
    // return number of used RGBQUAD structures
    static uint usedColors(uint bitCount, uint colorUsed);
private:
    void * dib_;
    BitmapInfoHeader * info_header_;
    // pointer to first RGBQUAD 32 bit fild
    RGBQuad * rgb_quads_;
    // pointer to BitFild
    uchar * pBitFild;
    version_t version_;
    direction_t direction_;
    // true if DIB attached
    bool valid_;
    // true if DIB created by this class
    bool under_construction_;
    // flag for DIB created by this module
    bool created_by_me_;
};

}

#endif // _CTDIB_H_
