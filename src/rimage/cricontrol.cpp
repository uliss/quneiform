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

#include <stdio.h>
#include <string.h>
#include <iostream>
#include <boost/current_function.hpp>

#include "resource.h"
#include "cricontrol.h"
#include "crimemory.h"
#include "common/debug.h"
#include "cimage/cticontrol.h"

#define RIMAGE_ERROR cf::Debug() << "[RIMAGE] ERROR " << BOOST_CURRENT_FUNCTION

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

bool CRIControl::binarise(const std::string& src, const std::string& dest, binarizator_t binType)
{
    bool Ret = true;

    // открываем исходный
    if (!openSourceDIB(src))
        return false;

    // создаем новый
    if (!createDestinatonDIB()) {
        closeSourceDIB();
        RIMAGE_ERROR << " can't create new image\n";
        return false;
    }

    //открываем бинаризатор
    if (!binarizator_)
        binarizator_ = new CRIBinarizator(&mcProgress);

    // закидываем туда картинки
    if (!binarizator_->SetRasters(src_dib_, dest_dib_)) {
        SetReturnCode_rimage(IDS_RIMAGE_CANNOT_SET_DIB);
        Ret = false;
    }

    CTBinarize bType = CTBIN_UNKNOWN;
    switch(binType) {
    case BINARIZATOR_DEZA:
        bType = CTBIN_DEZA;
        break;
    case BINARIZATOR_KRONROD:
        bType = CTBIN_KRONROD;
        break;
    default:
        break;
    }

    // бинаризуем
    if (!binarizator_->Binarize(bType, binType)) {
        RIMAGE_ERROR << " binarization error\n";
        Ret = false;
    }

    //отписваем новый в контейнер и освобождаем
    if (!CloseDestinationDIB(dest)) {
        SetReturnCode_rimage(IDS_RIMAGE_UNDER_CONSTRUCTION);
        Ret = false;
    }

    //закрываем исходный
    if (!closeSourceDIB()) {
        SetReturnCode_rimage(IDS_RIMAGE_UNDER_CONSTRUCTION);
        Ret = false;
    }

    return Ret;
}

Bool32 CRIControl::Rotate(char* cDIBIn, char* cDIBOut, int32_t High,
                          int32_t Low, uint32_t UseMargins)
{
    Bool32 Ret = TRUE;
    Bool32 NoDest = FALSE;

    // открываем исходный
    if (!openSourceDIB(cDIBIn)) {
        fprintf(stderr, "OpenSourceDIB failed\n");
        return FALSE;
    }

    if (dest_dib_) {
        SetReturnCode_rimage(IDS_RIMAGE_INTERNAL_MODULE_ERROR);
        return FALSE;
    }

    dest_dib_ = new CTDIB;

    //открываем вертелку
    if (!rotator_) {
        rotator_ = new CRRotator(&mcProgress);
    }

    // забываем старое имя
    mcLastDIBName[0] = 0x0;

    if (!rotator_->Rotate(src_dib_, dest_dib_, High, Low)) {
        uint16_t wRet = GetReturnCode_rimage();
        // !!! Art Изменил - теперь она заносит не хендлы, а указатели, а то память утекала
        //почему-то...
        //      Handle hDIBtoSet;
        pvoid pDIBtoSet;

        if ((wRet == IDS_RIMAGE_ZERO_NUMERATOR_OR_DENUMERATOR || wRet
                == IDS_RIMAGE_ANGLE_LEAST_MINIMUM) && src_dib_->GetDIBPtr(
                    &pDIBtoSet)
                /*mpSourceDIB->GetDIBHandle(&hDIBtoSet)*/) {
            //          SetDIB(cDIBOut, hDIBtoSet);
            WriteDIB(cDIBOut, pDIBtoSet);
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
    if (!CloseDestinationDIB(cDIBOut)) {
        if (NoDest == FALSE) {
            SetReturnCode_rimage(IDS_RIMAGE_CANNT_SAVE_OUTCOMING_DIB);
            Ret = FALSE;
        }
    }

    strcpy(mcLastDIBName, cDIBOut);

    //закрываем исходный
    if (!closeSourceDIB()) {
        SetReturnCode_rimage(IDS_RIMAGE_UNDER_CONSTRUCTION);
        Ret = FALSE;
    }

    return Ret;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////
//
Bool32 CRIControl::Turn(const char* cDIBIn, const char* cDIBOut, uint32_t wFlag,
                        uint32_t UseMargins)
{
    int32_t NewWidth;
    int32_t NewHeight;
    Bool32 bRet = FALSE;
    //void *   pSDIB;

    if (wFlag != RIMAGE_TURN_90 && wFlag != RIMAGE_TURN_270 && wFlag
            != RIMAGE_TURN_180) {
        RIMAGE_ERROR << " invalid angle: " << wFlag << "\n";
        return FALSE;
    }

    if (mp_TurnedDIB != NULL) {
        SetReturnCode_rimage(IDS_RIMAGE_INTERNAL_MODULE_ERROR);
        return FALSE;
    }

    // открываем исходный
    if (!openSourceDIB(cDIBIn)) {
        return FALSE;
    }

    NewWidth = (wFlag == RIMAGE_TURN_180 ? src_dib_->GetImageWidth()
                : src_dib_->GetImageHeight());
    NewHeight = (wFlag == RIMAGE_TURN_180 ? src_dib_->GetImageHeight()
                 : src_dib_->GetImageWidth());

    /*
     if ( !mpSourceDIB->GetDIBPtr( &pSDIB ) )
     {
     SetReturnCode_rimage(IDS_RIMAGE_INVALID_EXTERNAL_DIB);
     return FALSE;
     }
     */

    //открываем вертелку
    if (!turner_) {
        turner_ = new CRTurner;
    }

    // генерим новенький
    if (dest_dib_) {
        SetReturnCode_rimage(IDS_RIMAGE_INTERNAL_MODULE_ERROR);
        return FALSE;
    }

    dest_dib_ = new CTDIB;
    RIMAGEComment("Turn - temporary destination DIB");
    dest_dib_->SetExternals(RIMAGEAlloc, RIMAGEFree, RIMAGELock,
                                   RIMAGEUnlock);

    if (dest_dib_->CreateDIBBegin(NewWidth, NewHeight,
                                         src_dib_->GetPixelSize())
            && dest_dib_->CopyPalleteFromDIB(src_dib_)
            && dest_dib_->CopyDPIFromDIB(src_dib_)
            && dest_dib_->CreateDIBEnd()) {
        bRet = turner_->TurnDIB(src_dib_, dest_dib_, wFlag);
    }

    // вертим
    /*
     if ( !(mp_TurnedDIB = mpTurner->TurnDIB(pSDIB, wFlag)) )
     {
     SetReturnCode_rimage(IDS_RIMAGE_CANT_TURN_DIB);
     return FALSE;
     }
     */

    /*
     //отписваем новый в контейнер и освобождаем
     if ( !WriteDIB(cDIBOutt, mp_TurnedDIB) )
     {
     SetReturnCode_rimage(IDS_RIMAGE_UNABLE_WRITE_DIB);
     return FALSE;
     }

     if ( !mpTurner->FreeDIB(mp_TurnedDIB) )
     {
     SetReturnCode_rimage(IDS_RIMAGE_INVALID_EXTERNAL_DIB);
     return FALSE;
     }

     mp_TurnedDIB = NULL;
     */
    //отписваем новый в контейнер и освобождаем
    if (!CloseDestinationDIB(cDIBOut)) {
        SetReturnCode_rimage(IDS_RIMAGE_CANNT_SAVE_OUTCOMING_DIB);
        bRet = FALSE;
    }

    //закрываем исходный
    if (!closeSourceDIB()) {
        SetReturnCode_rimage(IDS_RIMAGE_UNDER_CONSTRUCTION);
        bRet = FALSE;
    }

    return bRet;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////
//
Bool32 CRIControl::Inverse(char* cDIBIn, char* cDIBOut, uint32_t UseMargins)
{
    Bool32 bErrors = TRUE;

    // копируем из исходного DIB в обрабатываемый
    // получаем заполненный mpDescinationDIB и пустой mpSourceDIB
    if (!OpenDestinationDIBfromSource(cDIBIn)) {
        return FALSE;
    }

    //открываем инвертор
    if (!invertor_) {
        invertor_ = new CRInvertor;
    }

    // Инвертируем
    if (!invertor_->Inverse(dest_dib_)) {
        SetReturnCode_rimage(IDS_RIMAGE_CANNOT_INVERT_IMAGE);
        bErrors = FALSE;
    }

    //отписваем новый в контейнер и освобождаем
    if (!SetDestinationDIBtoStorage(cDIBOut)) {
        SetReturnCode_rimage(IDS_RIMAGE_UNABLE_WRITE_DIB);
        bErrors = FALSE;
    }

    return bErrors;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////
// взять без копировыания
Bool32 CRIControl::GetDIB(const char* cDIB, BitmapHandle *phDIB)
{
    // берем с копированием, что б маска была!
    if (CIMAGE_ReadDIB(cDIB, phDIB))
        return TRUE;

    SetReturnCode_rimage(IDS_RIMAGE_NO_IMAGE_FOUND);
    return FALSE;
}

// положить без копирования
Bool32 CRIControl::SetDIB(const std::string& name, Handle hDIB)
{
    if (CIMAGE_AddImage(name, (BitmapHandle) hDIB))
        return TRUE;

    SetReturnCode_rimage(IDS_RIMAGE_UNABLE_WRITE_DIB);
    return FALSE;
}

// положить c копированием
Bool32 CRIControl::WriteDIB(const std::string& name, Handle hDIB)
{
    if (CIMAGE_AddImageCopy(name, (BitmapHandle) hDIB))
        return TRUE;

    SetReturnCode_rimage(IDS_RIMAGE_NO_IMAGE_FOUND);
    return FALSE;
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
    if (src_dib_->GetDIBHandle((Handle*) &dib)) {
        RIMAGEUnlock(dib);
    }

    delete src_dib_;
    CImage::instance().free(dib);
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
Bool32 CRIControl::CloseDestinationDIB(const std::string& name)
{
    Handle hDIB = NULL;
    pvoid pDIB = NULL;

    if (DIBOpeningType == TRUE) {
        return SetDestinationDIBtoStorage(name);
    }

    if (!dest_dib_)
        return FALSE;

    if (!dest_dib_->GetDIBHandle(&hDIB) || !dest_dib_->GetDIBPtr(
                &pDIB)) {
        delete dest_dib_;
        dest_dib_ = NULL;
        return FALSE;
    }

    if (!WriteDIB(name, pDIB)) {
        SetReturnCode_rimage(IDS_RIMAGE_NO_IMAGE_FOUND);
        return FALSE;
    }

    delete dest_dib_;
    dest_dib_ = NULL;
    return TRUE;
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
        { 0x00, 0x00, 0x00, 0x00 },
        { 0xff, 0xff, 0xff, 0x00 }
    };

    if (!dest_dib_->SetRGBQuad(0, BWQuads[0]))
        return false;

    if (!dest_dib_->SetRGBQuad(1, BWQuads[1]))
        return false;

    if (!dest_dib_->CreateDIBEnd())
        return false;

    return true;
}

Bool32 CRIControl::OpenDestinationDIBfromSource(const char* cDIBName)
{
    BitmapHandle hDIBIn;
    pvoid pDIB;

    if (src_dib_ != NULL)
        return FALSE;

    if (!readDIBCopy(cDIBName, &hDIBIn)) {
        SetReturnCode_rimage(IDS_RIMAGE_NO_IMAGE_FOUND);
        return FALSE;
    }

    if (!(pDIB = RIMAGELock(hDIBIn))) {
        SetReturnCode_rimage(IDS_RIMAGE_INTERNAL_MODULE_ERROR);
        return FALSE;
    }

    dest_dib_ = new CTDIB(hDIBIn);

    if (!dest_dib_->SetDIBbyPtr(pDIB)) {
        delete src_dib_;
        dest_dib_ = NULL;
        SetReturnCode_rimage(IDS_RIMAGE_DIB_NOT_ATTACHED);
        return FALSE;
    }

    DIBOpeningType = TRUE;
    return TRUE;
}

Bool32 CRIControl::SetDestinationDIBtoStorage(const std::string& name)
{
    BitmapHandle hSDIB;
    Bool32 bErrors = TRUE;

    if (DIBOpeningType == FALSE) {
        return CloseDestinationDIB(name);
    }

    // записывваем в блок выделеный в CIMAGE при открытии
    if (dest_dib_ == NULL)
        return FALSE;

    if (!dest_dib_->GetDIBHandle((void**) &hSDIB))
        return FALSE;

    if (!SetDIB(name, hSDIB)) {
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
    if (!CloseDestinationDIB(cDIBOut)) {
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

Bool32 CRIControl::RotatePoint(char* cDIB, int32_t iX, int32_t iY,
                               int32_t * prX, int32_t * prY)
{
    Bool32 bRet = FALSE;

    if (rotator_ && strcmp(cDIB, mcLastDIBName) == 0) {
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
    mp_TurnedDIB = NULL;
    DIBOpeningType = FALSE;
    mcLastDIBName[0] = 0x0;
}

}
