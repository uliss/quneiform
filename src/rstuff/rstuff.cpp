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

#include <algorithm>

#include "rstuff.h"
#include "rsfunc.h"
#include "rsglobaldata.h"
#include "rverline.h"
#include "smetric.h"
#include "rline.h"
#include "cpage/cpage.h"
#include "ccom/ccom.h"
#include "ctdib.h"
#include "cline.h"
#include "compat_defs.h"
#include "un_buff.h"
#include "line_vp_util.h"
#include "line_vp_2_am.h"
#include "markdataoper.h"
#include "un_err.h"

#include "common/debug.h"
#include "cifconfig.h"

extern Bool32 gbRSLT;

namespace CIF {

int move;

FixedBuffer<unsigned char, RStuff::MainBufferSize> RStuff::main_buffer_;
FixedBuffer<unsigned char, RStuff::WorkBufferSize> RStuff::work_buffer_;

RStuff::RStuff() {
    gLTInfo = new LinesTotalInfo;

    if (!SMetric_Init(0xFFFF, 0))
        throw RStuffException("SMetric_Init failed");
    if (!RLINE_Init(0, 0))
        throw RStuffException("RLINE_Init failed");
}

RStuff::~RStuff() {
    delete gLTInfo;
    gLTInfo = NULL;

    if (!SMetric_Done())
        return;

    if (!RLINE_Done())
        return;
}

void RStuff::binarize() {

}

void RStuff::cleanImage(uchar* pmasp, int bytewide, int num_str, int wide) {
    int realbytewide = ((wide + 7) >> 3);
    int stop = num_str * bytewide;
    int nowbyte = bytewide - 1;
    int nowbyte1 = bytewide - 2;
    int nowbyte2 = bytewide - 3;
    switch (bytewide - realbytewide) {
    case 0:
        break;
    case 1:
        while (nowbyte < stop) {
            pmasp[nowbyte] = 255;
            nowbyte += bytewide;
        }
        break;
    case 2:
        while (nowbyte1 < stop) {
            pmasp[nowbyte] = 255;
            pmasp[nowbyte1] = 255;
            nowbyte += bytewide;
            nowbyte1 += bytewide;
        }
        break;
    case 3:
        while (nowbyte2 < stop) {
            pmasp[nowbyte] = 255;
            nowbyte += bytewide;
            pmasp[nowbyte1] = 255;
            nowbyte1 += bytewide;
            pmasp[nowbyte2] = 255;
            nowbyte2 += bytewide;
        }
    }
    int move = (realbytewide << 3) - wide;
    if (!move)
        return;
    move = 7 - move;
    nowbyte = realbytewide - 1;
    while (nowbyte < stop) {
        pmasp[nowbyte] |= 255 >> move;
        nowbyte += bytewide;
    }
}

void RStuff::calcIncline() {
    char Str[256];
    Bool ret, ManyComp;
    uint16_t Code;
    int32_t SkewReg, Skew, SkewLocVerLin;
    Rect16 RcReg;
    PAGEINFO info;
    UN_BUFF MainBuff = { 0 };
    Handle hCPage = image_->hCPAGE;
    CLINE_handle hCLINE = *((CLINE_handle*) image_->phCLINE);

    GetPageInfo(hCPage, &info);

    /*  2. Инициализация.  */
    /***  переменные  ***/
    bool WasLine = false;
    /***  организация памяти  ***/
    MainBuff.vBuff = main_buffer_.begin();
    MainBuff.SizeBuff = MainBufferSize;
    MainBuff.vCurr = MainBuff.vBuff;
    MainBuff.SizeCurr = MainBuff.SizeBuff;
    /***  линии  ***/
    ret = LoadLinesVP_rv(hCLINE, UN_LD_LinesVP2, &MainBuff, Str, &Code);
    if ((ret != RV_TRUE) && (ret != RV_EMPTY))
        throw RStuffException("LoadLinesVP_rv failed");

    WasLine = (ret == RV_TRUE);
    /***  компоненты  ***/
    ret = LoadComps_rv(*(image_->phCCOM), (void *) (&MainBuff), Str, 0); //t-e-d
    if (ret == RV_DOUBT) {
        SetReturnCode_rstuff(Code);
        CleanLastDataPart((void *) (&MainBuff));
    }
    ManyComp = (ret == RV_TRUE) && (MainBuff.nPartUnits[MainBuff.nPart - 1] > 10000);
    if (ManyComp) {
        CleanLastDataPart((void *) (&MainBuff));
    }

    if (ret == RV_DOUBT || ManyComp) {
        ret = LoadComps_rv(*(image_->phCCOM), (void *) (&MainBuff), Str, 3); //t-e-d
        if (ret == RV_DOUBT) {
            SetReturnCode_rstuff(Code);
            CleanLastDataPart((void *) (&MainBuff));
        }
    }

    if (ret != RV_TRUE)
        throw RStuffException("RStuff::calcIncline()  failed", ret);

    RcReg.rleft() = 0;
    RcReg.rright() = (int16_t) info.Width;
    RcReg.rtop() = 0;
    RcReg.rbottom() = (int16_t) info.Height;
    SkewReg = 0;
    Bool ContWarn = 0;
    SkewLocVerLin = 0;

    //  5.1 Определяем угол наклона страницы (ее заполнения в целом).
    ret = ConvertLinesToAM_rv(UN_LD_LinesVP2, UN_LD_LinesAM, (void *) (&MainBuff),
            (int *) work_buffer_.begin(), WorkBufferSize / sizeof(int), &RcReg, SkewReg, Str,
            ContWarn);
    if (ret == RV_DOUBT)
        SetReturnCode_rstuff(Code);
    if (ret != RV_TRUE)
        throw RStuffException("RStuff::calcIncline()  failed", ret);

    SMetric_SetImportData(SMetric_ResolX, (void *) info.DPIX);
    SMetric_SetImportData(SMetric_ResolY, (void *) info.DPIY);
    ret = SMetric_FindMainSkew((void *) (&MainBuff), (char*) work_buffer_.begin(), WorkBufferSize,
            &Skew, &SkewLocVerLin, &RcReg, SkewReg, Str, FALSE, FALSE);
    if (ret != RV_TRUE)
        throw RStuffException("RStuff::calcIncline()  failed", ret);
    info.Incline2048 = Skew * 2;
    info.SkewLocVerLin2048 = SkewLocVerLin * 2;
    if (!CPAGE_SetPageData(image_->hCPAGE, PT_PAGEINFO, &info, sizeof(PAGEINFO)))
        throw RStuffException("CPAGE_SetPageData failed");
}

void RStuff::checkResolution() {
    PAGEINFO page_info;
    const unsigned int min_res = 99;
    CCOM_comp* pcomp = NULL;

    if (!GetPageInfo(image_->hCPAGE, &page_info))
        return;

    if (page_info.DPIX > min_res && page_info.DPIY > min_res)
        return;

    const int SIZE = 100;
    unsigned int Masy[SIZE], Masx[SIZE];
    std::fill(Masx, Masx + SIZE, 0);
    std::fill(Masy, Masy + SIZE, 0);

    pcomp = CCOM_GetFirst(*image_->phCCOM, NULL);

    while (pcomp) {
        if (pcomp->h > 9 && pcomp->h < 100)
            Masy[pcomp->h]++;

        if (pcomp->w > 9 && pcomp->w < 100)
            Masx[pcomp->w]++;

        pcomp = CCOM_GetNext(pcomp, NULL);
    }

    unsigned int Jy_m = 0, My_m = 0, Jx_m = 0, Mx_m = 0;

    for (int i = 11; i < 99; i++) {
        uint M_t = Masy[i - 1] + Masy[i] + Masy[i + 1];

        if (M_t > My_m) {
            Jy_m = i;
            My_m = M_t;
        }

        uint M_t2 = Masx[i - 1] + Masx[i] + Masx[i + 1];

        if (M_t2 > Mx_m) {
            Jx_m = i;
            Mx_m = M_t2;
        }
    }

    bool flag_set = false;

    if (Jy_m > 10 && My_m > 100 && !(page_info.DPIY * 22 < 2 * 300 * Jy_m && 2 * page_info .DPIY
            * 22 > 300 * Jy_m)) {
        page_info.DPIY = (300 * Jy_m + 11) / 22;
        flag_set = true;
    }

    if (Jx_m > 10 && Mx_m > 100 && !(page_info.DPIX * 22 < 2 * 300 * Jx_m && 2 * page_info.DPIX
            * 22 > 300 * Jx_m)) {
        page_info.DPIX = (300 * Jx_m + 11) / 22;
        flag_set = true;
    }

    if (flag_set) {
        SetPageInfo(image_->hCPAGE, page_info);

        if (Config::instance().debug())
            Debug() << "New resolution: DPIX=" << page_info.DPIX << ", DPIY=" << page_info.DPIY
                    << "\n";
    }
}

struct BIG_IMAGE
{
    CCOM_handle hCCOM;
    uchar ImageName[CPAGE_MAXNAME];
};

void RStuff::createContainerBigComp() {
    const int MIN_BIG_H = 30;
    const int MIN_BIG_W = 30;

    CCOM_handle hCCOM_old = (CCOM_handle) (*(image_->phCCOM));
    Handle hCPage = image_->hCPAGE;
    CCOM_handle hCCOM_new = 0;
    BIG_IMAGE big_Image;
    PAGEINFO info;
    GetPageInfo(hCPage, &info);

    for (int i = 0; i < CPAGE_MAXNAME; i++)
        big_Image.ImageName[i] = info.szImageName[i];

    hCCOM_new = CCOM_CreateContainer();
    if (!hCCOM_new) {
        big_Image.hCCOM = NULL;
        return;
    }

    CCOM_comp* comp = NULL;
    CCOM_comp* new_comp;
    comp = CCOM_GetFirst(hCCOM_old, FALSE);

    while (comp) {
        if ((comp->h >= MIN_BIG_H) && (comp->w >= MIN_BIG_W)) {
            new_comp = CCOM_New(hCCOM_new, comp->upper, comp->left, comp->w, comp->h);
            if (new_comp) {
                if (comp->size_linerep < 0)
                    ;
                else if (!CCOM_Copy(new_comp, comp))
                    CCOM_Delete(hCCOM_new, comp);
            }
        }
        comp = CCOM_GetNext(comp, FALSE);
    }

    big_Image.hCCOM = hCCOM_new;
    CPAGE_CreateBlock(hCPage, CPAGE_GetInternalType("TYPE_BIG_COMP"), 0, 0, &big_Image,
            sizeof(BIG_IMAGE));
}

void RStuff::copyMove(uchar* newpmasp, uchar* oldpmasp, int newbytewide, int oldbytewide,
        int num_str, int move) {
    int max_move = abs((num_str * move) / 2048);
    int spusk = (num_str + max_move - 1) / max_move;
    int nowmove = 0;
    int realspusk = spusk >> 1;
    int bytemove = 0;
    int newnowbyte;
    int oldnowbyte;
    int stopbyte;
    int newstopbyte;

    if (move > 0) {
        stopbyte = num_str * oldbytewide - 1;
        newstopbyte = num_str * newbytewide - 1;
        oldnowbyte = stopbyte - oldbytewide + 1;
        newnowbyte = newstopbyte - newbytewide + 1;
        for (int i = num_str - 1; i >= 0; i--) {
            for (int k = 0; k < bytemove; k++) {
                newpmasp[newnowbyte] = 255;
                newnowbyte++;
            }

            newpmasp[newnowbyte] = ~(255 >> nowmove);

            while ((oldnowbyte < stopbyte) && (newnowbyte < newstopbyte - 1)) {
                newpmasp[newnowbyte] |= (oldpmasp[oldnowbyte] >> nowmove);
                newnowbyte++;
                newpmasp[newnowbyte] = (oldpmasp[oldnowbyte] << (7 - nowmove));
                oldnowbyte++;
            }
            newpmasp[newnowbyte] |= (255 >> nowmove);
            newnowbyte++;
            while (newnowbyte < newstopbyte) {
                newpmasp[newnowbyte] = 255;
                newnowbyte++;
            }

            stopbyte -= oldbytewide;
            newstopbyte -= newbytewide;
            oldnowbyte = stopbyte - oldbytewide + 1;
            newnowbyte = newstopbyte - newbytewide + 1;
            realspusk--;
            if (!realspusk) {
                realspusk = spusk;
                nowmove++;
                if (nowmove == 8) {
                    nowmove = 0;
                    bytemove++;
                }
            }
        }
    }
    else {
        stopbyte = oldbytewide - 1;
        newstopbyte = newbytewide - 1;
        oldnowbyte = 0;
        newnowbyte = 0;
        for (int i = 0; i < num_str; i++) {
            for (int k = 0; k < bytemove; k++) {
                newpmasp[newnowbyte] = 255;
                newnowbyte++;
            }

            newpmasp[newnowbyte] = ~(255 >> nowmove);

            while ((oldnowbyte < stopbyte) && (newnowbyte < newstopbyte - 1)) {
                newpmasp[newnowbyte] |= (oldpmasp[oldnowbyte] >> nowmove);
                newnowbyte++;
                newpmasp[newnowbyte] = (oldpmasp[oldnowbyte] << (7 - nowmove));
                oldnowbyte++;
            }
            newpmasp[newnowbyte] |= (255 >> nowmove);
            newnowbyte++;
            while (newnowbyte < newstopbyte) {
                newpmasp[newnowbyte] = 255;
                newnowbyte++;
            }

            stopbyte += oldbytewide;
            newstopbyte += newbytewide;
            oldnowbyte = stopbyte - oldbytewide + 1;
            newnowbyte = newstopbyte - newbytewide + 1;
            realspusk--;
            if (!realspusk) {
                realspusk = spusk;
                nowmove++;
                if (nowmove == 8) {
                    nowmove = 0;
                    bytemove++;
                }
            }
        }
    }
}

void RStuff::killLines() {
    if (*image_->pgrc_line && *image_->pgneed_clean_line) {
        puchar pDIB = NULL;
        if (!RemoveLines(image_, &pDIB)) {
            *image_->pgpRecogDIB = pDIB;
            throw RStuffException("RStuff::killLines() failed");
        }
    }
}

void RStuff::layout() {
}

void RStuff::normalize() {
    preProcessImage();
    searchLines();
    calcIncline();
    ortoMove();
    createContainerBigComp();
    searchNewLines();
    killLines();
    // убиваем остатки линии после сняти
    // uliss:LineKiller(image_);
}

void RStuff::ortoMove() {
    Handle hCPage = image_->hCPAGE;
    PAGEINFO info;
    char OldImage[CPAGE_MAXNAME];
    GetPageInfo(hCPage, &info);

    move = info.SkewLocVerLin2048;
    if (!move)
        return;

    for (int i = 0; i < CPAGE_MAXNAME; i++)
        OldImage[i] = info.szImageName[i];

    const char* ImageName = PUMA_IMAGE_ORTOMOVE;

    Handle lpDIB;
    if (!CIMAGE_ReadDIB(OldImage, &lpDIB, 1))
        throw RStuffException("CIMAGE_ReadDIB failed");

    std::auto_ptr<CTDIB> olddib(new CTDIB);
    if (!(olddib->SetDIBbyPtr(lpDIB)))
        throw RStuffException("SetDIBbyPtr failed");

    int oldbytewide = olddib->GetLineWidthInBytes();
    int num_str = olddib->GetLinesNumber();
    uchar* pmasp = (uchar*) (olddib->GetPtrToBitFild());

    int oldwide = (int) (olddib->GetLineWidth());

    int max_move = abs((num_str * move) / 2048);
    if (!max_move) {
        olddib->ResetDIB();
        return;
    }

    std::auto_ptr<CTDIB> newdib(new CTDIB);
    int newwide = oldwide + max_move;
    lpDIB = newdib->CreateDIBBegin(newwide, num_str, info.BitPerPixel);
    if (!lpDIB) {
        olddib->ResetDIB();
        return;
    }
    uint32_t X_DPM = 0;
    uint32_t Y_DPM = 0;
    olddib->GetResolutionDPM(&X_DPM, &Y_DPM);
    newdib->SetResolutionDPI(info.DPIX, info.DPIY);
    newdib->SetResolutionDPM(X_DPM, Y_DPM);
    if (!(newdib->CreateDIBEnd())) {
        olddib->ResetDIB();
        return;
    }
    int newbytewide = newdib->GetLineWidthInBytes();
    uchar* newpmasp = (uchar*) (newdib->GetPtrToBitFild());
    if (newwide > (int) (newdib->GetLineWidth())) {
        olddib->ResetDIB();
        newdib->DestroyDIB();
        return;
    }
    cleanImage(pmasp, oldbytewide, num_str, (int) (olddib->GetLineWidth()));
    copyMove(newpmasp, pmasp, newbytewide, oldbytewide, num_str, move);

    if (CIMAGE_WriteDIB(ImageName, lpDIB, 0)) {
        BITMAPINFOHEADER * lp = NULL;
        if (CIMAGE_ReadDIB(ImageName, (Handle*) &lp, TRUE)) {
            info.Images |= IMAGE_ORTOMOVE;
            strcpy((char*) info.szImageName, PUMA_IMAGE_ORTOMOVE);
            SetPageInfo(hCPage, info);
        }
        else {
            olddib->ResetDIB();
            newdib->DestroyDIB();
            throw RStuffException("CIMAGE_ReadDIB failed");
        }

        olddib->ResetDIB();
        newdib->DestroyDIB();

        //снова выделим компоненты
        if (!ExtractComponents(FALSE, NULL, PUMA_IMAGE_ORTOMOVE, image_))
            throw RStuffException("");
        //выделим линии
        CLINE_Reset();
        searchLines();
        //найдём угол наклона
        calcIncline();
    }
    else {
        olddib->ResetDIB();
        newdib->DestroyDIB();
        throw RStuffException("CIMAGE_WriteDIB failed");
    }
}

void RStuff::preProcessImage() {
    const char * glpRecogName = image_->pglpRecogName;
    BitmapInfoHeader * info = (BitmapInfoHeader*) image_->pinfo;

    // Andrey 12.11.01
    // Проинициализируем контейнер CPAGE
    PAGEINFO PInfo;// = { 0 };
    GetPageInfo(image_->hCPAGE, &PInfo);
    strcpy((char*) PInfo.szImageName, glpRecogName);
    PInfo.BitPerPixel = info->biBitCount;
    PInfo.DPIX = info->biXPelsPerMeter * 254L / 10000;
    //      PInfo.DPIX = PInfo.DPIX < 200 ? 200 : PInfo.DPIX;
    PInfo.DPIY = info->biYPelsPerMeter * 254L / 10000;
    //      PInfo.DPIY = PInfo.DPIY < 200 ? 200 : PInfo.DPIY;
    PInfo.Height = info->biHeight;
    PInfo.Width = info->biWidth;
    //      PInfo.X = 0; Уже установлено
    //      PInfo.Y = 0;
    PInfo.Incline2048 = 0;
    PInfo.Page = 1;
    PInfo.Angle = 0;
    SetPageInfo(image_->hCPAGE, PInfo);

    // Выделим компоненты
    ExtractComponents(image_->gbAutoRotate, NULL, glpRecogName, image_);
    //проверим наличие разрешения и попытаемся определить по компонентам, если его нет
    checkResolution();

    // Переинициализируем контейнер CPAGE
    {
        PAGEINFO PInfo;// = { 0 };
        GetPageInfo(image_->hCPAGE, &PInfo);
        strcpy((char*) PInfo.szImageName, glpRecogName);
        PInfo.BitPerPixel = info->biBitCount;
        //      PInfo.DPIX = info->biXPelsPerMeter*254L/10000;
        PInfo.DPIX = PInfo.DPIX < 200 ? 200 : PInfo.DPIX;
        //      PInfo.DPIY = info->biYPelsPerMeter*254L/10000;
        PInfo.DPIY = PInfo.DPIY < 200 ? 200 : PInfo.DPIY;
        PInfo.Height = info->biHeight;
        PInfo.Width = info->biWidth;
        //      PInfo.X = 0; Уже установлено
        //      PInfo.Y = 0;
        PInfo.Incline2048 = 0;
        PInfo.Page = 1;
        PInfo.Angle = 0;

        SetPageInfo(image_->hCPAGE, PInfo);
    }
}

void RStuff::removeLines() {
    if (!(*image_->pgrc_line) || !(*image_->pgneed_clean_line))
        return;
    puchar pDIB = NULL;
    bool rc = RemoveLines(image_, &pDIB);
    if (rc)
        *image_->pgpRecogDIB = pDIB;
}

void RStuff::searchLines() {
    Bool32 b32 = !image_->gbDotMatrix;
    RLINE_SetImportData(RLINE_Bool32_NOFILLGAP3, &b32);
    b32 = TRUE;
    RLINE_SetImportData(RLINE_Bool32_NOHBORDER, &b32);
    RLINE_SetImportData(RLINE_Bool32_NOVBORDER, &b32);

    if (!RLINE_SearchLines(image_->hCPAGE, image_->phCLINE)) {
        *image_->pgrc_line = FALSE;
        Debug() << "Warning: RLINE_SearchLines failed\n";
    }
}

void RStuff::searchNewLines() {
    Handle hSaveImage = CPAGE_CreateBlock(image_->hCPAGE, CPAGE_GetInternalType("RVL_VERIFY"), 0,
            0, image_, sizeof(RSPreProcessImage));

    if (!RLINE_LinesPass1(image_->hCPAGE, *(image_->phCCOM), image_->phCLINE,
            image_->pgneed_clean_line, true, (uchar) image_->gnLanguage))
        throw RStuffException("RLINE_LinesPass1() failed");

    if (!gbRSLT && !RLINE_LinesPass2(*(image_->phCCOM), image_->phCLINE, image_->hCPAGE))
        throw RStuffException("RLINE_LinesPass2() failed");

    CPAGE_DeleteBlock(image_->hCPAGE, hSaveImage);
}

void RStuff::setImageData(RSPreProcessImage& data) {
    image_ = &data;
}

}
