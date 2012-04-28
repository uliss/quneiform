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
#include <cstring>
#include <iostream>
#include <boost/current_function.hpp>

#include "resource.h"
#include "cricontrol.h"
#include "crimemory.h"
#include "oldbinarizator.h"
#include "cimage/cticontrol.h"
#include "rimage_debug.h"
#include "binarizatorfactory.h"

namespace cf {

CRIControl::CRIControl()
{
    init();
}

CRIControl::~CRIControl()
{
    clear();
}

void CRIControl::clear()
{
    if (binarizator_)
        delete binarizator_;

    if (turner_)
        delete turner_;

    if (invertor_)
        delete invertor_;

    if (rotator_)
        delete rotator_;

    if (dest_dib_)
        delete dest_dib_;
}

void CRIControl::reset()
{
    clear();
    init();
}

bool CRIControl::binarise(const std::string& src, const std::string& dest,
                          binarizator_t binType,
                          int param)
{
    CTDIB * src_dib = CImage::instance().imageDib(src);

    if(!src_dib) {
        RIMAGE_ERROR << " can\'t get source image: \"" << src << "\"\n";
        return false;
    }

    BinarizatorPtr bin = BinarizatorFactory::instance().make(binType, param);
    bin->setSource(src_dib);

    CTDIB * dest_dib = bin->binarize(binType);

    // бинаризуем
    if (!dest_dib) {
        RIMAGE_ERROR << " binarization error: " << src << "\n";
        delete src_dib;
        return false;
    }

    CImage::instance().addImageCopy(dest, dest_dib);
    delete dest_dib;
    delete src_dib;

    return true;
}

bool CRIControl::rotate(const std::string& src, const std::string& dest, int high, int low)
{
    Bool32 Ret = TRUE;
    Bool32 NoDest = FALSE;

    // открываем исходный
    if (!openSourceDIB(src)) {
        RIMAGE_ERROR << "OpenSourceDIB failed\n";
        return false;
    }

    if (dest_dib_) {
        SetReturnCode_rimage(IDS_RIMAGE_INTERNAL_MODULE_ERROR);
        return false;
    }

    dest_dib_ = new CTDIB;

    //открываем вертелку
    if (!rotator_)
        rotator_ = new CRRotator(&mcProgress);

    // забываем старое имя
    mcLastDIBName[0] = 0x0;

    if (!rotator_->Rotate(src_dib_, dest_dib_, high, low)) {
        uint16_t wRet = GetReturnCode_rimage();
        // !!! Art Изменил - теперь она заносит не хендлы, а указатели, а то память утекала
        //почему-то...
        //      Handle hDIBtoSet;
        BitmapHandle handle;

        if ((wRet == IDS_RIMAGE_ZERO_NUMERATOR_OR_DENUMERATOR ||
             wRet == IDS_RIMAGE_ANGLE_LEAST_MINIMUM) &&
                src_dib_->GetDIBPtr((void**) &handle))
        {
            saveCopy(dest, handle);
            SetReturnCode_rimage(IDS_RIMAGE_ERR_NO);
            NoDest = Ret = TRUE;
        }
        else {
            SetReturnCode_rimage(IDS_RIMAGE_CANNOT_ROTATE_IMAGE);
            Ret = FALSE;
        }
    }
    else {
        WriteDIBtoBMP("Allex.DIBBeforeDeskew.bmp", src_dib_);
        WriteDIBtoBMP("Allex.DIBAfterDeskew.bmp", dest_dib_);
    }

    //отписваем новый в контейнер и освобождаем
    if (!closeDestinationDIB(dest)) {
        if (NoDest == FALSE) {
            SetReturnCode_rimage(IDS_RIMAGE_CANNT_SAVE_OUTCOMING_DIB);
            Ret = FALSE;
        }
    }

    strcpy(mcLastDIBName, dest.c_str());

    //закрываем исходный
    if (!closeSourceDIB()) {
        SetReturnCode_rimage(IDS_RIMAGE_UNDER_CONSTRUCTION);
        Ret = FALSE;
    }

    return Ret;
}

bool CRIControl::turn(const std::string &src, const std::string& dest, rimage_turn_angle_t angle)
{
    if (!openSourceDIB(src))
        return false;

    int new_width = (angle == RIMAGE_TURN_180) ? src_dib_->GetImageWidth()
                                               : src_dib_->GetImageHeight();
    int new_height = (angle == RIMAGE_TURN_180) ? src_dib_->GetImageHeight()
                                                : src_dib_->GetImageWidth();

    //открываем вертелку
    if (!turner_)
        turner_ = new CRTurner;

    // генерим новенький
    if (dest_dib_) {
        closeSourceDIB();
        RIMAGE_ERROR << " previous dest dib not removed\n";
        return false;
    }

    dest_dib_ = new CTDIB;
    dest_dib_->SetExternals(RIMAGEAlloc, RIMAGEFree, RIMAGELock, RIMAGEUnlock);

    bool bRet = false;

    if (dest_dib_->CreateDIBBegin(new_width, new_height, src_dib_->GetPixelSize()) &&
            dest_dib_->CopyPalleteFromDIB(src_dib_) &&
            dest_dib_->CopyDPIFromDIB(src_dib_) &&
            dest_dib_->CreateDIBEnd())
    {
        bRet = turner_->TurnDIB(src_dib_, dest_dib_, angle);
    }
    else {
        RIMAGE_ERROR << " can't create destination dib\n";
        closeSourceDIB();
        delete dest_dib_;
        return false;
    }

    //отписваем новый в контейнер и освобождаем
    if (!closeDestinationDIB(dest)) {
        SetReturnCode_rimage(IDS_RIMAGE_CANNT_SAVE_OUTCOMING_DIB);
        bRet = false;
    }

    //закрываем исходный
    if (!closeSourceDIB()) {
        SetReturnCode_rimage(IDS_RIMAGE_UNDER_CONSTRUCTION);
        bRet = false;
    }

    return bRet;
}

bool CRIControl::inverse(const std::string& src, const std::string& dest)
{
    bool bErrors = true;

    // копируем из исходного DIB в обрабатываемый
    // получаем заполненный mpDescinationDIB и пустой mpSourceDIB
    if (!openDestinationDIBfromSource(src))
        return false;

    //открываем инвертор
    if (!invertor_)
        invertor_ = new CRInvertor;

    // Инвертируем
    if (!invertor_->Inverse(dest_dib_)) {
        RIMAGE_ERROR << " image inverse error: " << src << "\n";
        bErrors = false;
    }

    //отписваем новый в контейнер и освобождаем
    if (!SetDestinationDIBtoStorage(dest)) {
        SetReturnCode_rimage(IDS_RIMAGE_UNABLE_WRITE_DIB);
        bErrors = false;
    }

    return bErrors;
}

bool CRIControl::saveCopy(const std::string& name, BitmapHandle handle)
{
    return CImage::instance().addImageCopy(name, handle);
}

bool CRIControl::readDIBCopy(const std::string& name, BitmapHandle * dest)
{
    if (CIMAGE_ReadDIBCopy(name,  dest))
        return true;

    RIMAGE_ERROR << " copy error: " << name << "\n";
    return false;
}

bool CRIControl::closeSourceDIB()
{
    if (src_dib_ == NULL) {
        RIMAGE_ERROR << " source not opened\n";
        return false;
    }

    BitmapHandle dib = NULL;
    if (src_dib_->GetDIBPtr((Handle*) &dib))
        CImage::instance().free(dib);

    delete src_dib_;
    return true;
}

bool CRIControl::openSourceDIB(const std::string& name)
{
    BitmapHandle handle = CImage::instance().imageCopy(name);

    if (!handle) {
        RIMAGE_ERROR << " image not found: " << name << "\n";
        return false;
    }

    src_dib_ = new CTDIB;

    if (!src_dib_->SetDIBbyPtr(handle)) {
        delete src_dib_;
        src_dib_ = NULL;
        RIMAGE_ERROR << " invalid image: " << name << "\n";
        return false;
    }

    return true;
}

//Создаем временный DIB куда отпишем, что получили
bool CRIControl::closeDestinationDIB(const std::string& name)
{
    if (DIBOpeningType == TRUE)
        return SetDestinationDIBtoStorage(name);

    if (!dest_dib_)
        return false;

    BitmapHandle dib = NULL;
    if (!dest_dib_->GetDIBPtr((void**) &dib)) {
        delete dest_dib_;
        dest_dib_ = NULL;
        return false;
    }

    if (!saveCopy(name, dib)) {
        RIMAGE_ERROR << " image saving failed: " << name << "\n";
        return false;
    }

    delete dest_dib_;
    dest_dib_ = NULL;
    return true;
}

bool CRIControl::createDestinatonDIB()
{
    if (src_dib_ == NULL) {
        RIMAGE_ERROR << " source image is not opened\n";
        return false;
    }

    dest_dib_ = new CTDIB;

    if (!dest_dib_->SetExternals(RIMAGEAlloc, RIMAGEFree, RIMAGELock, RIMAGEUnlock)) {
        RIMAGE_ERROR << " set external error\n";
        delete dest_dib_;
        dest_dib_ = NULL;
        return false;
    }

    uint32_t wNewHeight;
    uint32_t wNewWidth;
    uint32_t wXResolution;
    uint32_t wYResolution;

    wNewHeight = src_dib_->GetLinesNumber();
    wNewWidth = src_dib_->GetLineWidth();
    src_dib_->GetResolutionDPM(&wXResolution, &wYResolution);

    uint32_t BitCount = 1;
    if (!dest_dib_->CreateDIBBegin(wNewWidth, wNewHeight, BitCount))
        return false;

    if (!dest_dib_->SetResolutionDPM(wXResolution, wYResolution)) {
        //return FALSE;
    }

    CTDIBRGBQUAD BWQuads[2] = {
        CTDIBRGBQUAD(0, 0, 0, 0),
        CTDIBRGBQUAD(0xff, 0xff, 0xff, 0)
    };

    if (!dest_dib_->SetRGBQuad(0, BWQuads[0]))
        return false;

    if (!dest_dib_->SetRGBQuad(1, BWQuads[1]))
        return false;

    if (!dest_dib_->CreateDIBEnd())
        return false;

    return true;
}

bool CRIControl::openDestinationDIBfromSource(const std::string& name)
{
    BitmapHandle hDIBIn;
    pvoid pDIB;

    if (src_dib_ != NULL)
        return false;

    if (!readDIBCopy(name, &hDIBIn)) {
        SetReturnCode_rimage(IDS_RIMAGE_NO_IMAGE_FOUND);
        return false;
    }

    if (!(pDIB = RIMAGELock(hDIBIn))) {
        SetReturnCode_rimage(IDS_RIMAGE_INTERNAL_MODULE_ERROR);
        return false;
    }

    dest_dib_ = new CTDIB(hDIBIn);

    if (!dest_dib_->SetDIBbyPtr(pDIB)) {
        delete src_dib_;
        dest_dib_ = NULL;
        SetReturnCode_rimage(IDS_RIMAGE_DIB_NOT_ATTACHED);
        return false;
    }

    DIBOpeningType = TRUE;
    return true;
}

Bool32 CRIControl::SetDestinationDIBtoStorage(const std::string& name)
{
    BitmapHandle hSDIB;
    Bool32 bErrors = TRUE;

    if (DIBOpeningType == FALSE) {
        return closeDestinationDIB(name);
    }

    // записывваем в блок выделеный в CIMAGE при открытии
    if (dest_dib_ == NULL)
        return FALSE;

    if (!dest_dib_->GetDIBHandle((void**) &hSDIB))
        return FALSE;

    if (!CIMAGE_AddImage(name, hSDIB)) {
        SetReturnCode_rimage(IDS_RIMAGE_NO_IMAGE_FOUND);
        return FALSE;
    }

    if (!CIMAGE_FreeCopiedDIB(hSDIB)) {
        SetReturnCode_rimage(IDS_RIMAGE_CIMAGE_MEMORY_ERROR);
        bErrors = FALSE;
    }

    delete dest_dib_;
    dest_dib_ = NULL;
    DIBOpeningType = FALSE;
    return bErrors;
}

Bool32 CRIControl::Roll(char* cDIBIn, char* cDIBOut, int32_t Num,
                        int32_t Denum, uint32_t bUseMargins)
{
    Bool32 Ret = TRUE;

    // открываем исходный
    if (!openSourceDIB(cDIBIn)) {
        return FALSE;
    }

    if (dest_dib_) {
        SetReturnCode_rimage(IDS_RIMAGE_INTERNAL_MODULE_ERROR);
        return FALSE;
    }

    dest_dib_ = new CTDIB;

    //открываем вертелку
    if (!rotator_) {
        rotator_ = new CRRotator;
    }

    if (!rotator_->Roll(src_dib_, dest_dib_, Num, Denum)) {
        SetReturnCode_rimage(IDS_RIMAGE_CANNOT_ROTATE_IMAGE);
        Ret = FALSE;
    }

    //отписваем новый в контейнер и освобождаем
    if (!closeDestinationDIB(cDIBOut)) {
        SetReturnCode_rimage(IDS_RIMAGE_CANNT_SAVE_OUTCOMING_DIB);
        Ret = FALSE;
    }

    //закрываем исходный
    if (!closeSourceDIB()) {
        SetReturnCode_rimage(IDS_RIMAGE_UNDER_CONSTRUCTION);
        Ret = FALSE;
    }

    return Ret;
}

Bool32 CRIControl::SetProgressCallBacks(PRIMAGECBPRogressStart pcbStart,
                                        PRIMAGECBPRogressStep pcbStep, PRIMAGECBPRogressFinish pcbFinish)
{
    return mcProgress.SetExternals((CRPStart) pcbStart, (CRPFinish) pcbFinish,
                                   (CRPStep) pcbStep);
}

Bool32 CRIControl::StartProgress()
{
    return mcProgress.Start();
}

bool CRIControl::rotatePoint(const std::string& name, int32_t iX, int32_t iY,
                               int32_t * prX, int32_t * prY)
{
    bool bRet = false;

    if (rotator_ && name == mcLastDIBName) {
        bRet = rotator_->RotatePoint(iX, iY, prX, prY);
    }

    return bRet;
}

Bool32 CRIControl::WriteDIBtoBMP(const char *cName, PCTDIB pDIB)
{
#ifdef RIMAGE_DUMP_ENABLE
    uint32_t wBMPSize = pDIB->GetDIBSize() + 14;
    PumaMemoryToFileDumper BMPDump(cName);
    BMPDump.AddDump("BM", 2);
    BMPDump.AddDump((char *)&wBMPSize, 4);
    wBMPSize = 0;
    BMPDump.AddDump((char *)&wBMPSize, 4);
    wBMPSize = 14 + pDIB->GetHeaderSize() + pDIB->GetRGBPalleteSize();
    BMPDump.AddDump((char *)&wBMPSize, 4);
    BMPDump.AddDump(pDIB->GetPtrToHeader(), pDIB->GetDIBSize());
    BMPDump.EndDump();
#endif
    return TRUE;
}

void CRIControl::init()
{
    src_dib_ = NULL;
    dest_dib_ = NULL;
    binarizator_ = NULL;
    turner_ = NULL;
    invertor_ = NULL;
    rotator_ = NULL;
    DIBOpeningType = FALSE;
    mcLastDIBName[0] = 0x0;
}

}
