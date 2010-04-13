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

// ============================================================================
// Written by Shahverdiev  Alik
// This file cration date: 05.04.99
//
// FrmtPict.cpp
// Source file for Device-Independent Bitmap (DIB) API.  Provides
// the following functions:
// GetPictCount()      - Return Picture count
// WritePict()         - Write  Picture

// ============================================================================

#include "creatertf.h"
#include <cmath>
#include "cpage/cpage.h"
#include "cpage/cpagetyps.h"
#include "rtfedwrite.h"
#include "frmtpict.h"
#include "dpuma.h"
#include "puma/pumadef.h"
#include "rdib/ctdib.h"
#include "cimage/ctiimage.h"
#include "frmtdibapi.h"
#include "rimage/criimage.h"
#include "sys_prog.h"
#include "ced/ced.h"
#include "ced/cedline.h"
#include "rfrmt_prot.h"
#include "compat/filefunc.h"

#include "minmax.h"

using namespace CIF;

#define  USE_NONE              0x0040   // no formatting
extern uint32_t FlagMode;
extern uint32_t RtfWriteMode;
extern Point16 TemplateOffset;

extern char RtfFileName[PATH_MAX];
/*
 * Dib Header Marker - used in writing DIBs to files
 */
#define DIB_HEADER_MARKER   ((uint16_t) ('M' << 8) | 'B')

//==============   Определение кол-ва картин на странице  ======================
uint32_t GetPictCount(void)
{
    uint32_t PictCount = 0;
    uint32_t NumberPage = CPAGE_GetCurrentPage();
    Handle h_Page = CPAGE_GetHandlePage(NumberPage);
    Handle h_Pict = CPAGE_PictureGetFirst(h_Page);

    while (h_Pict) {
        PictCount++;
        h_Pict = CPAGE_PictureGetNext(h_Page, h_Pict);
    }

    return PictCount;
}

//=====================     Размер картинки     ===================================
uchar GetPictRect(uint32_t NumberPict, ::Rect16* RectPict, uint32_t* UserNumber)
{
    uint32_t PictCount = 0;
    Point Lr, Wh;
    uint32_t NumberPage = CPAGE_GetCurrentPage();
    Handle h_Page = CPAGE_GetHandlePage(NumberPage);
    Handle h_Pict = CPAGE_PictureGetFirst(h_Page);

    while (h_Pict && PictCount < NumberPict) {
        PictCount++;
        h_Pict = CPAGE_PictureGetNext(h_Page, h_Pict);
    }

    if (!h_Pict)
        return FALSE;

    *UserNumber = (uint32_t) CPAGE_GetBlockUserNum(h_Page, h_Pict);

    if (CPAGE_PictureGetPlace(h_Page, h_Pict, 0, &Lr, &Wh)) {
        RectPict->left = (int16_t) (Lr.x() - TemplateOffset.x());
        RectPict->right = (int16_t) (Lr.x() - TemplateOffset.x() + Wh.x());
        RectPict->top = (int16_t) (Lr.y() - TemplateOffset.y());
        RectPict->bottom = (int16_t) (Lr.y() - TemplateOffset.y() + Wh.y());
    }

    return TRUE;
}

//**************************** Запись картин ************************************
Bool WritePict(uint32_t IndexPict, RtfSectorInfo* SectorInfo, Bool OutPutTypeFrame)
{
    uint32_t PictNumber = 0;
    Point RtfLt;
    CPAGE_PICTURE pict;
    LDPUMA_Skip(hTest);
#ifdef EdWrite
    Handle hParagraph = NULL;
    CIF::CEDLine * hString = NULL;
    CIF::Size pictSize;
    EDSIZE pictGoal;
    CIF::Rect indent;
    EDBOX playout;
    CIF::Rect slayout;
    EDSIZE interval;
    EDBOX EdFragmRect;
    Letter Letter;
    Handle hPrevObject;
#endif
    uint32_t NumberPage = CPAGE_GetCurrentPage();
    Handle h_Page = CPAGE_GetHandlePage(NumberPage);
    Handle h_Pict = CPAGE_PictureGetFirst(h_Page);

    while (h_Pict) {
        if (++PictNumber > IndexPict)
            break;

        h_Pict = CPAGE_PictureGetNext(h_Page, h_Pict);
    }

    if (!h_Pict)
        return FALSE;

    PAGEINFO pinfo = { 0 };

    if (!GetPageInfo(h_Page, &pinfo))
        return FALSE;

    CIMAGE_InfoDataInGet in = { 0 };
    BitmapInfoHeader image_info;
    Point Lr;
    Point Wh;
    Point PLr;
    Point LrN;
    Point WhN;
    uint16_t FrameOffset = 0;

    if (CIMAGE_GetImageInfo(pinfo.szImageName, &image_info) == FALSE)
        return FALSE;

    CPAGE_PictureGetPlace(h_Page, h_Pict, 0, &Lr, &Wh);
    CPAGE_PictureGetPlace(h_Page, h_Pict, -pinfo.Incline2048, &LrN, &WhN);
    Lr.rx() -= TemplateOffset.x();
    Lr.ry() -= TemplateOffset.y();
    FrameOffset = abs(WhN.x() - Wh.x());

    if (Lr.x() < 0)
        FrameOffset += abs(Lr.x());

    // Получим картинку из исходного изображения задав ее контур
    //определяем размер маски

    if (!CPAGE_PictureGetPlace(h_Page, h_Pict, -pinfo.Incline2048, &Lr, &Wh))
        return FALSE;

    Bool rc = TRUE;

    //piter : Корректируем координаты из-за повернута страницы.
    switch (pinfo.Angle) {
        case 0:
            in.dwX = Lr.x();
            in.dwY = Lr.y();
            in.dwWidth = Wh.x();
            in.dwHeight = Wh.y();
            break;
        case 270:
            in.dwX = pinfo.Width - (Wh.y() + Lr.y());
            in.dwY = Lr.x();
            in.dwWidth = Wh.y();
            in.dwHeight = Wh.x();
            break;
        case 180:
            in.dwX = pinfo.Width - (Wh.x() + Lr.x());
            in.dwY = pinfo.Height - (Wh.y() + Lr.y());
            in.dwWidth = Wh.x();
            in.dwHeight = Wh.y();
            break;
        case 90:
            in.dwX = Lr.y();
            in.dwY = pinfo.Height - (Wh.x() + Lr.x());
            in.dwWidth = Wh.y();
            in.dwHeight = Wh.x();
            break;
    }

    // end piter
    LDPUMA_Skip(hTestDIBData);
    in.MaskFlag = FALSE;
    void * pOutDIB = NULL;

    if (!CIMAGE_GetDIBData(PUMA_IMAGE_USER, &in, &pOutDIB)) {
        fprintf(stderr, "WritePict error!!! CIMAGE_GetDIBData failed: %s\n", PUMA_IMAGE_USER);
        return FALSE;
    }

    // Соберем изображение
    char szTurnName[] = "RFRMT:TurnPicture";
    char szPictName[] = "RFRMT:Picture";
    char szRotateName[] = "RFRMT:RotatePicture";
    char * lpName = szPictName;
    LDPUMA_Skip(hTestTurn);

    if (CIMAGE_WriteDIB(szPictName, pOutDIB, TRUE)) {
        switch (pinfo.Angle) {
            case 90:
                rc = RIMAGE_Turn((puchar) szPictName, (puchar) szTurnName, RIMAGE_TURN_90, FALSE);
                CIMAGE_DeleteImage(lpName);
                lpName = szTurnName;
                break;
            case 180:
                rc = RIMAGE_Turn((puchar) szPictName, (puchar) szTurnName, RIMAGE_TURN_180, FALSE);
                CIMAGE_DeleteImage(lpName);
                lpName = szTurnName;
                break;
            case 270:
                rc = RIMAGE_Turn((puchar) szPictName, (puchar) szTurnName, RIMAGE_TURN_270, FALSE);
                CIMAGE_DeleteImage(lpName);
                lpName = szTurnName;
                break;
        }

        if (!rc) {
            fprintf(stderr, "RIMAGE_Turn failed\n");
            rc = FALSE;
        }
    }

    else {
        fprintf(stderr, "WritePict error!!! CIMAGE_WriteDIB failed: %s\n", szPictName);
    }

    // Довернем изображение на малый угол.
    LDPUMA_Skip(hTestRotate);

    if (!RIMAGE_Rotate((puchar) lpName, (puchar) szRotateName, pinfo.Incline2048, 2048, 0)) {
        fprintf(stderr, "RIMAGE_Rotate failed\n");
        rc = FALSE;
    }

    else {
        CIMAGE_DeleteImage(lpName);
        lpName = szRotateName;
    }

    // Маскируем полученное изображение
    Point ptLt, ptWh;

    if (rc && CPAGE_PictureGetPlace(h_Page, h_Pict, 0, &ptLt, &ptWh)) {
        if (pinfo.Incline2048 >= 0) {
            in.dwX = ptWh.y() * pinfo.Incline2048 / 2048;
            in.dwY = 0;
        }

        else {
            in.dwX = 0;
            //  Beg of Almi Corr
            //                      in.dwY = ptWh.x*pinfo.Incline2048/2048;
            in.dwY = (-ptWh.x() * pinfo.Incline2048 / 2048);
            //  End of Almi Corr
        }

        if (!RIMAGE_RotatePoint((puchar) lpName, in.dwX, in.dwY, (int32_t *) &in.dwX,
                                (int32_t *) &in.dwY)) {
            in.dwX = 0;
            in.dwY = 0;
        }

        in.dwWidth = ptWh.x();
        in.dwHeight = ptWh.y();
        in.wByteWidth = (unsigned short) ((in.dwWidth + 7) / 8); //?
        in.MaskFlag = TRUE;
        // Получим размер маски
        uint32_t nSize = 0;
        LDPUMA_Skip(hTestGetMaskDIB);

        if (CPAGE_PictureGetMask(h_Page, h_Pict, 0, NULL, &nSize)) {
            char * lpMask = (char*) malloc(sizeof(in) + nSize);

            if (lpMask) {// Получаем маску
                *(CIMAGE_InfoDataInGet*) lpMask = in;

                if (CPAGE_PictureGetMask(h_Page, h_Pict, 0, lpMask + sizeof(in), &nSize)) {
                    if (!CIMAGE_GetDIBData(lpName, (CIMAGE_InfoDataInGet*) lpMask, &pOutDIB)) {
                        fprintf(stderr, "CIMAGE_GetDIBData failed\n");
                        rc = FALSE;
                    }
                }

                else {
                    fprintf(stderr, "PAGE_PictureGetMask failed\n");
                    rc = FALSE;
                }

                free(lpMask);
            }
        }

        else {
            fprintf(stderr, "PAGE_PictureGetMask() failed\n");
            rc = FALSE;
        }
    }

    LDPUMA_Skip(hTestWriteMetafile);

    if (rc) {
        if (!RtfWriteMode) {
#ifdef EdWrite
            LDPUMA_Skip(hTestWriteED);
            PCTDIB pTmpDIB = new CTDIB;
            pTmpDIB->SetDIBbyPtr(pOutDIB);
            pictSize.rwidth() = Wh.x();
            pictSize.rheight() = Wh.y();
            pictGoal.cx = (uint32_t) (Twips * pTmpDIB->GetLineWidth());
            pictGoal.cy = (uint32_t) (Twips * pTmpDIB->GetLinesNumber());
            int32_t iDIBSize = pTmpDIB->GetDIBSize();
            delete pTmpDIB;
            indent = CIF::Rect();
            interval.cx = 0;
            interval.cy = 0;
            playout.x = -1;
            playout.w = -1;
            playout.y = -1;
            playout.h = -1;
            Lr.rx() = MAX(0, Lr.x());
            Lr.ry() = MAX(0, Lr.y());
            slayout.rleft() = Lr.x();
            slayout.rright() = Lr.x() + Wh.x();
            slayout.rtop() = Lr.y();
            slayout.rbottom() = Lr.y() + Wh.y();
            hPrevObject = SectorInfo->hObject;

            if (SectorInfo->FlagInColumn || (OutPutTypeFrame && SectorInfo->FlagFictiveParagraph)) {
                hParagraph = CED_CreateParagraph(SectorInfo->hEDSector, SectorInfo->hColumn, -1,
                                                 indent, SectorInfo->userNum, -1, interval, playout, -1, -1, -1, -1, FALSE);
                hString = (CIF::CEDLine*) CED_CreateLine(hParagraph, 0, 6);
                SectorInfo->FlagFictiveParagraph = FALSE;
            }

            if (FlagMode & USE_NONE || SectorInfo->CountFragments == 1)
                SectorInfo->hObject = SectorInfo->hColumn;

            else {
                if (SectorInfo->FlagInColumn == TRUE) {
                    EdFragmRect.x = MAX(0, SectorInfo->OffsetFromColumn.x());
                    EdFragmRect.y = MAX(0, SectorInfo->OffsetFromColumn.y());
                    EdFragmRect.w = MAX(0, Wh.x() - FrameOffset) * Twips;
                    EdFragmRect.h = Wh.y() * Twips;
                    SectorInfo->hObject = CED_CreateFrame(SectorInfo->hEDSector,
                                                          SectorInfo->hColumn, EdFragmRect, 0x22, -1, -1, -1);
                }

                else {
                    EdFragmRect.x = Lr.x() * Twips - SectorInfo->Offset.x();
                    EdFragmRect.y = Lr.y() * Twips - SectorInfo->Offset.y();
                    EdFragmRect.w = MAX(0, Wh.x() - FrameOffset) * Twips;
                    EdFragmRect.h = Wh.y() * Twips;
                    SectorInfo->hObject = CED_CreateFrame(SectorInfo->hEDSector,
                                                          SectorInfo->hColumn, EdFragmRect, 0x22, -1, 0, 0);
                }
            }

            hParagraph = CED_CreateParagraph(SectorInfo->hEDSector, SectorInfo->hObject, -1,
                                             indent, SectorInfo->userNum, -1, interval, playout, -1, -1, -1, -1, FALSE);
            hString = (CIF::CEDLine*) CED_CreateLine(hParagraph, 0, 6);
            Letter.setChar(' ');
            Letter.setProbability(0);
            CED_CreateChar(hString, slayout, &Letter, 12, ED_PICT_BASE + (int) IndexPict, -1, LANGUAGE_UNKNOWN,
                           -1, -1);

            if (!CED_CreatePicture(SectorInfo->hEDPage, (int) IndexPict, pictSize, pictGoal,
                                   ED_ALIGN_MIDDLE, 1, pOutDIB, (int) iDIBSize)) {
                SectorInfo->hObject = hPrevObject;
                return FALSE;
            }

#endif
            LDPUMA_Skip(hTestDeleteImage);
        }
    }

    // piter
    // освобождает память переданную по pOutDIB
    CIMAGE_DeleteImage(lpName);
    CIMAGE_FreeCopedDIB(pOutDIB);
    // end piter
#ifdef EdWrite

    if (!RtfWriteMode)
        SectorInfo->hObject = hPrevObject;

#endif
    LDPUMA_Skip(hTestEnd);
    return TRUE;
}

// Piter.
// Сохранение изображения в метафайле
static void bufcpy(char ** str, void * mem, unsigned sz)
{
    const char Hex[] = "0123456789ABCDEF";
    unsigned char * c = (unsigned char *) mem;

    for (unsigned i = 0; i < sz; i++, (*str) += 2) {
        (*str)[0] = Hex[c[i] >> 4];
        (*str)[1] = Hex[c[i] & 0x0F];
        (*str)[2] = 0;
    }
}
