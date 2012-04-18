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

#ifndef __CTI_CONTROL_H_
#define __CTI_CONTROL_H_

#include "ctiimagelist.h"
#include "common/rect.h"
#include "globus.h"

class CTDIB;

namespace cf
{

class CTIMaskLineSegment;
class BitMask;

class CLA_EXPO CTIControl
{
    public:
        CTIControl();
        ~CTIControl();

        /**
          * Adds rectangle to image read mask
          * @param name - image name
          * @param r - rectangle
          * @see addRectToWriteMask(), removeRectFromReadMask()
          */
        bool addRectToReadMask(const std::string& name, const Rect& r);

        /**
          * Adds rectangle to image write mask
          * @param name - image name
          * @param r - rectangle
          * @see addRectToReadMask(), removeRectFromReadMask()
          */
        bool addRectToWriteMask(const std::string& name, const Rect& r);

        /**
          * Substructs rectangle from image read mask
          * @param name - image name
          * @param r - rectangle
          * @see addRectToReadMask()
          */
        bool removeRectFromReadMask(const std::string& name, const Rect& r);

        /**
          * Substructs rectangle from image write mask
          * @param name - image name
          * @param r - rectangle
          * @see addRectToWriteMask()
          */
        bool removeRectFromWriteMask(const std::string& name, const Rect& r);

        bool disableReadMask(const std::string& imageName);
        bool disableWriteMask(const std::string& imageName);
        bool enableReadMask(const std::string& imageName);
        bool enableWriteMask(const std::string& imageName);

        /**
          * Adds image handle into container
          * @param name - image name
          * @param handle - image handle
          * @return true on success
          * @see addImageCopy()
          */
        bool addImage(const std::string& name, BitmapHandle handle);

        /**
          * Adds image copy into container
          * @param name - image name
          * @param handle - image handle
          * @return true on success
          * @see addImage()
          */
        bool addImageCopy(const std::string& name, BitmapHandle handle);

        /**
          * Dumps image to file
          * @param name - image name
          * @param fileName - destination file name
          * @return true on success
          */
        bool dumpImage(const std::string& name, const std::string& fileName);

        /**
          * Returns image callbacks
          * @param name - image name
          * @param cbk - pointer to callbacks
          * @return true on success
          */
        bool getImageCallbacks(const std::string& name, CIMAGEIMAGECALLBACK * cbk);

        /**
          * Returns image handle
          * @param name - image name
          * @return image handle on success or NULL on error
          * @see imageCopy()
          */
        BitmapHandle image(const std::string& name);

        /**
          * Returns image copy. Caller should free result with free()
          * if mask exists and enabled for image - it appleid to result image.
          * @param name - image name
          * @return image handle on success or NULL on error
          * @see image()
          */
        BitmapHandle imageCopy(const std::string& name);

        /**
          * Frees image memory return by imageCopy()
          * @param handle - bitmap handle to free
          * @return true on success
          * @see image(), imageCopy(), getDIBFromImage()
          */
        bool free(BitmapHandle handle);

        /**
          * Removes image from container
          * @param name - image name
          * @return true on success
          */
        bool removeImage(const std::string& name);

        /**
          * Resets image container and frees memory
          */
        void reset();

        bool writeImageCallbacks(const std::string& name, CIMAGEIMAGECALLBACK cbk);

        /**
          * Returns copy of image area by given name
          * @param name - image name
          * @param r - copy area
          * @param bitMask - if not NUL, bit mask applied
          * @param dest - result dib
          * @note you should free return dib pointer by free(BitmapHandle handle)
          * if image have active masks - they applied
          * @see free(), imageCopy()
          */
        bool getDIBFromImage(const std::string& name, const Rect &r, BitMask * bitMask, BitmapHandle * dest);

        /**
          * Returns copy of raw image data
          * @param name - image name
          * @param in - input param
          * @param out - output
          */
        bool getImageRawData(const std::string &name, CIMAGE_InfoDataInGet * in, CIMAGE_InfoDataOutGet * out);

        Bool32 CBImageOpen(CIMAGE_ImageInfo * lpImageInfo);
        Bool32 CBImageClose(void);
        uint32_t CBImageRead(char * buffer, uint32_t wMaxSize);
    public:
        static bool applyMaskToDIBLine(CTDIB * dib, CTIMaskLineSegment * segm, int line, int at_x, int at_y);
        static bool applyMaskToDIB(CTDIB * dib, CTIMask * mask, int at_x = 0, int at_y = 0);
        static bool applyMaskToHandle(BitmapHandle handle, CTIMask * mask, int at_x, int at_y);

        /**
          * Copies line from source to destination image
          * Image widths should be equal.
          * @param src - source image
          * @param dest - destination image
          * @param destYOffset - destination line offset
          * @param lineCount - number of lines to copy
          * @return true on success
          */
        static bool copyFromSourceFrame(const CTDIB * src, CTDIB * dest, uint destYOffset, uint lineCount);

        /**
          * Copies lines from equal or bigger source image to equal or smaller destination image.
          * @param src - source image
          * @param dest - destination image
          * @param xOffset - source line x offset.
          * @param yOffset - start source line
          * @param lineCount - line count to copy
          * @note (source line width - x offset) should be greater or equal to destination width
          * @return true on success
          */
        static bool copyToDestinationFrame(const CTDIB * src, CTDIB * dest, uint xOffset, uint yOffset, uint lineCount);

        /**
          * Writes given image fo BMP file
          * @param name - file name
          * @param pDib - given image
          */
        static bool writeDIBtoBMP(const std::string& name, CTDIB * pDIB);
        static bool writeDIBtoBMP(const std::string& name, BitmapHandle h);
    private:
        static bool copyDIB(const BitmapHandle src, BitmapHandle * dest);
    private:
        bool applyMask(const std::string& name, int x, int y);
        void clear();
        void freeBuffers();
        void init();

        bool getFrame(const CTDIB * src, CTDIB * dest, const Rect& frame);
        bool setFrame(const CTDIB * src, CTDIB * dest, CIMAGE_InfoDataInReplace * pIn);
        Rect checkRect(const CTDIB& dib, const Rect &r);
    protected:
        CTIImageList images_;
        Handle hCBImage;
        void * pCBImage;
        char * pCBBuffer;
        uint32_t wCBBufferSize;
        uint32_t wCBLine;
        uint32_t wCBWidth;
        uint32_t wCBLines;
        uint32_t wCBStep;
        puchar image_raw_data_;
        CTDIB * image_dib_;
        uint32_t mwMemoryErrors;
        CTDIB * mCBDestianationDIB;
        CTDIB * mCBSourceDIB;
        CTDIB * mCBWDestianationDIB;
        CTDIB * mCBWSourceDIB;
        Bool32 mCBWInProcess;
};

}

#endif

