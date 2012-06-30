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

# ifndef __CRI_CONTROL_H_
# define __CRI_CONTROL_H_

#include "cttypes.h"
#include "criimage.h"
#include "cimage/ctiimage.h"
#include "rprogressor.h"    // Added by ClassView
#include "globus.h"

class CRInvertor;
class CRRotator;

namespace cf {

class CTDIB;
class CRTurner;

class CLA_EXPO CRIControl
{
public:
    CRIControl();
    ~CRIControl();

    void clear();
    void init();
    void reset();
public:
    bool rotatePoint(const std::string& name, int32_t iX, int32_t iY, int32_t * prX, int32_t * prY);
    Bool32          StartProgress(void);
    Bool32          SetProgressCallBacks(PRIMAGECBPRogressStart pcbStart, PRIMAGECBPRogressStep pcbStep, PRIMAGECBPRogressFinish pcbFinish);

    /**
      * Binarises source imgage and stores result into CImage container
      * @param src - source image name in CImage storage
      * @param dest - destination image name in CImage container
      * @param binType - type of binarizator
      * @param param - extra binarizator params
      * @return true on success
      */
    bool binarise(const std::string& src, const std::string& dest, binarizator_t binType, int param = 0);

    /**
      * Inverts image colors and stores result in CImage container
      * @param src - source image name in CImage storage
      * @param dest - destination image name in CImage storage
      * @return true on success
      */
    bool inverse(const std::string& src, const std::string& dest);

    /**
      * Turns image angle degrees clockwise and stores result in CImage container.
      * @param src - source image name in CImage container.
      * @param dest - destination image name in CImage container.
      * @param angle - turn angle
      * @return true on success
      */
    bool turn(const std::string& src, const std::string& dest, rimage_turn_angle_t angle);

    bool rotate(const std::string& src, const std::string& dest, int high, int low);
    Bool32          Roll(char* cDIBIn, char* cDIBOut, int32_t Num, int32_t Denum, uint32_t bUseMargins);
private:
    Bool32          WriteDIBtoBMP(const char *cName, CTDIB * pDIB);
    bool closeSourceDIB();
    // Creating new DIB by CTDIB class and 4 RIMAGE functions
    bool createDestinatonDIB();
    Bool32          SetDestinationDIBtoStorage(const std::string&  name);
    bool openDestinationDIBfromSource(const std::string& name);
    bool closeDestinationDIB(const std::string& name);
    bool openSourceDIB(const std::string&   name);
    bool saveCopy(const std::string& name, BitmapPtr handle);
    bool readDIBCopy(const std::string& name, BitmapPtr * dest);
private:
    Bool32          DIBOpeningType;
    CTDIB * src_dib_;
    CTDIB * dest_dib_;
    // Name of last processed image
    char mcLastDIBName[256];
    // pointer to class ProgressShow
    CRProgressor mcProgress;
    // pointer to clas  Invertor
    CRInvertor * invertor_;
    // pointer to class Turn
    CRTurner * turner_;
    // pointer to class Rotate
    CRRotator * rotator_;
};

}

# endif
