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

#include <string.h> // for memcpy
#include "sectorinfo.h"
#include "rfrmtoptions.h"
#include "frmtpict.h"
#include "puma/pumadef.h"
#include "cpage/cpage.h"
#include "rdib/ctdib.h"
#include "cimage/ctiimage.h"
#include "cimage/bitmask.h"
#include "rimage/criimage.h"
#include "ced/ced.h"
#include "ced/cedchar.h"
#include "ced/cedcolumn.h"
#include "ced/cedframe.h"
#include "ced/cedline.h"
#include "ced/cedpicture.h"
#include "ced/cedparagraph.h"
#include "ced/cedsection.h"
#include "compat/filefunc.h"
#include "common/debug.h"

#include "minmax.h"

extern cf::Point TemplateOffset;

namespace cf
{

size_t GetPictCount() {
    size_t res = 0;
    uint32_t NumberPage = CPAGE_GetCurrentPage();
    Handle h_Page = CPAGE_GetHandlePage(NumberPage);
    Handle h_Pict = CPAGE_PictureGetFirst(h_Page);

    while (h_Pict) {
        res++;
        h_Pict = CPAGE_PictureGetNext(h_Page, h_Pict);
    }

    return res;
}

Rect GetPictRect(uint NumberPict, uint32_t * UserNumber) {
    uint PictCount = 0;

    uint NumberPage = CPAGE_GetCurrentPage();
    Handle h_Page = CPAGE_GetHandlePage(NumberPage);
    Handle h_Pict = CPAGE_PictureGetFirst(h_Page);

    while (h_Pict && PictCount < NumberPict) {
        PictCount++;
        h_Pict = CPAGE_PictureGetNext(h_Page, h_Pict);
    }

    if (!h_Pict)
        throw std::runtime_error("[GetPictRect] can't get picture");

    *UserNumber = (uint32_t) CPAGE_GetBlockUserNum(h_Page, h_Pict);

    Point Lr, Wh;
    if (CPAGE_PictureGetPlace(h_Page, h_Pict, 0, &Lr, &Wh))
        return Rect(Lr - TemplateOffset, Wh.x(), Wh.y());
    else
        throw std::runtime_error("[GetPictRect] CPAGE_PictureGetPlace failed");
}

bool WritePict(uint32_t IndexPict, SectorInfo * SectorInfo, Bool OutPutTypeFrame) {
    uint32_t PictNumber = 0;
    Point RtfLt;
    CPAGE_PICTURE pict;
    uint32_t NumberPage = CPAGE_GetCurrentPage();
    Handle h_Page = CPAGE_GetHandlePage(NumberPage);
    Handle h_Pict = CPAGE_PictureGetFirst(h_Page);

    while (h_Pict) {
        if (++PictNumber > IndexPict)
            break;

        h_Pict = CPAGE_PictureGetNext(h_Page, h_Pict);
    }

    if (!h_Pict)
        return false;

    PAGEINFO pinfo;

    if (!GetPageInfo(h_Page, &pinfo))
        return false;

    CIMAGE_InfoDataInGet in = { 0 };
    BitmapInfoHeader image_info;
    Point Lr;
    Point Wh;
    Point PLr;
    Point LrN;
    Point WhN;

    if (CIMAGE_GetImageInfo(pinfo.szImageName, &image_info) == FALSE)
        return false;

    CPAGE_PictureGetPlace(h_Page, h_Pict, 0, &Lr, &Wh);
    CPAGE_PictureGetPlace(h_Page, h_Pict, -pinfo.Incline2048, &LrN, &WhN);
    Lr.rx() -= TemplateOffset.x();
    Lr.ry() -= TemplateOffset.y();
    int FrameOffset = abs(WhN.x() - Wh.x());

    if (Lr.x() < 0)
        FrameOffset += abs(Lr.x());

    // Получим картинку из исходного изображения задав ее контур
    //определяем размер маски

    if (!CPAGE_PictureGetPlace(h_Page, h_Pict, -pinfo.Incline2048, &Lr, &Wh))
        return false;

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
    BitmapHandle pOutDIB = NULL;

    if (!CIMAGE_GetDIBData(PUMA_IMAGE_USER, Rect(in.dwX, in.dwY, in.dwWidth, in.dwHeight), NULL, &pOutDIB)) {
        Debug() << "[WritePict] CIMAGE_GetDIBData failed: " << PUMA_IMAGE_USER << "\n";
        return false;

    }

    // Соберем изображение
    const char * szTurnName = "RFRMT:TurnPicture";
    const char * szPictName = "RFRMT:Picture";
    const char * szRotateName = "RFRMT:RotatePicture";
    const char * lpName = szPictName;

    if (CIMAGE_AddImage(szPictName, (BitmapHandle) pOutDIB)) {
        switch (pinfo.Angle) {
        case 90:
            rc = RIMAGE_Turn(szPictName, szTurnName, RIMAGE_TURN_90, FALSE);
            CIMAGE_RemoveImage(lpName);
            lpName = szTurnName;
            break;
        case 180:
            rc = RIMAGE_Turn(szPictName, szTurnName, RIMAGE_TURN_180, FALSE);
            CIMAGE_RemoveImage(lpName);
            lpName = szTurnName;
            break;
        case 270:
            rc = RIMAGE_Turn(szPictName, szTurnName, RIMAGE_TURN_270, FALSE);
            CIMAGE_RemoveImage(lpName);
            lpName = szTurnName;
            break;
        }

        if (!rc) {
            Debug() << "[WritePict] RIMAGE_Turn failed";
            rc = FALSE;
        }
    } else {
        Debug() << "[WritePict] CIMAGE_WriteDIB failed: " << szPictName << "\n";
    }

    // Довернем изображение на малый угол.
    if (!RIMAGE_Rotate((puchar) lpName, (puchar) szRotateName, pinfo.Incline2048, 2048, 0)) {
        Debug() << "[WritePict] RIMAGE_Rotate failed\n";
        rc = FALSE;
    } else {
        CIMAGE_RemoveImage(lpName);
        lpName = szRotateName;
    }

    // Маскируем полученное изображение
    Point ptLt, ptWh;

    if (rc && CPAGE_PictureGetPlace(h_Page, h_Pict, 0, &ptLt, &ptWh)) {
        if (pinfo.Incline2048 >= 0) {
            in.dwX = ptWh.y() * pinfo.Incline2048 / 2048;
            in.dwY = 0;
        } else {
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
        // Получим размер маски
        uint32_t nSize = 0;

        if (CPAGE_PictureGetMask(h_Page, h_Pict, 0, NULL, &nSize)) {
            char * lpMask = (char*) malloc(sizeof(in) + nSize);

            if (lpMask) {// Получаем маску
                *(CIMAGE_InfoDataInGet*) lpMask = in;

                if (CPAGE_PictureGetMask(h_Page, h_Pict, 0, lpMask + sizeof(in), &nSize)) {
                     cf::BitMask bit_mask(0, 0, (uchar*) lpMask + sizeof(in));

                    if (!CIMAGE_GetDIBData(lpName, Rect(in.dwX, in.dwY, in.dwWidth, in.dwHeight), &bit_mask, &pOutDIB)) {
                        Debug() << "CIMAGE_GetDIBData failed\n";
                        rc = FALSE;
                    }
                } else {
                    Debug() << "PAGE_PictureGetMask failed\n";
                    rc = FALSE;
                }

                free(lpMask);
            }
        } else {
            Debug() << "PAGE_PictureGetMask() failed\n";
            rc = FALSE;
        }
    }

    BlockElement * hPrevObject;

    if (rc) {
        Size pictSize(Wh.x(), Wh.y());

        PCTDIB pTmpDIB = new CTDIB;
        pTmpDIB->SetDIBbyPtr(pOutDIB);
        cf::Size pictGoal;
        pictGoal.rwidth() = pTmpDIB->GetLineWidth();
        pictGoal.rheight() = pTmpDIB->GetLinesNumber();
        int32_t iDIBSize = pTmpDIB->GetDIBSize();
        delete pTmpDIB;
        Rect indent;
        Rect playout;
        Lr.rx() = MAX(0, Lr.x());
        Lr.ry() = MAX(0, Lr.y());

        Rect slayout;
        slayout.rleft() = Lr.x();
        slayout.rright() = Lr.x() + Wh.x();
        slayout.rtop() = Lr.y();
        slayout.rbottom() = Lr.y() + Wh.y();
        hPrevObject = SectorInfo->hObject;

        if (SectorInfo->FlagInColumn || (OutPutTypeFrame && SectorInfo->FlagFictiveParagraph)) {
            CEDParagraph * par = SectorInfo->hEDSector->createParagraph(SectorInfo->hColumn,
                    ALIGN_NONE, indent, SectorInfo->userNum, -1, playout);
            par->addLine(new CEDLine(NULL, false, 6));
            SectorInfo->FlagFictiveParagraph = FALSE;
        }

        if (RfrmtOptions::useNone() || SectorInfo->CountFragments == 1)
            SectorInfo->hObject = SectorInfo->hColumn;
        else {
            if (SectorInfo->FlagInColumn == TRUE) {
                Rect EdFragmRect;
                EdFragmRect.setLeft(MAX(0, SectorInfo->OffsetFromColumn.x()));
                EdFragmRect.setTop(MAX(0, SectorInfo->OffsetFromColumn.y()));
                EdFragmRect.setWidth(MAX(0, Wh.x() - FrameOffset));
                EdFragmRect.setHeight(Wh.y());

                CEDFrame * frame = new CEDFrame(NULL, CEDFrame::HPOS_COLUMN,
                        CEDFrame::VPOS_PARAGRAPH);
                frame->setBoundingRect(EdFragmRect);
                SectorInfo->hColumn->addElement(frame);
                SectorInfo->hObject = frame;
            } else {
                Rect EdFragmRect;
                EdFragmRect.setLeft(Lr.x() - SectorInfo->Offset.x());
                EdFragmRect.setTop(Lr.y() - SectorInfo->Offset.y());
                EdFragmRect.setWidth(MAX(0, Wh.x() - FrameOffset));
                EdFragmRect.setHeight(Wh.y());
                CEDFrame * frame = new CEDFrame(NULL, CEDFrame::HPOS_COLUMN,
                        CEDFrame::VPOS_PARAGRAPH);
                frame->setBoundingRect(EdFragmRect);
                frame->dxfrtextx = 0;
                frame->dxfrtexty = 0;
                SectorInfo->hColumn->addElement(frame);
                SectorInfo->hObject = frame;
            }
        }

        CEDParagraph * ced_par = SectorInfo->hEDSector->createParagraph(SectorInfo->hObject,
                ALIGN_NONE, indent, SectorInfo->userNum, -1, playout);
        CEDLine * ced_line = new CEDLine;
        // TODO hard coded font value
        ced_line->setDefaultFontHeight(6);

        CEDPicture * ced_pict = new CEDPicture();
        ced_pict->setBoundingRect(slayout);
        ced_pict->setPictureNumber(IndexPict);
        ced_pict->setAlign(ED_ALIGN_MIDDLE);
        uchar * img_data = new uchar[iDIBSize];
        memcpy(img_data, pOutDIB, iDIBSize);
        ced_pict->setImage(new Image(img_data, iDIBSize, Image::AllocatorNew));
        ced_pict->image()->setSize(slayout.size());

        ced_line->addElement(ced_pict);
        ced_par->addLine(ced_line);
    }

    // piter
    // освобождает память переданную по pOutDIB
    CIMAGE_RemoveImage(lpName);
    CIMAGE_FreeCopiedDIB(pOutDIB);
    // end piter
    SectorInfo->hObject = hPrevObject;
    return TRUE;
}

}

