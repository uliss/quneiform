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

#include <setjmp.h>
#include <string.h>
#include <math.h>
#include <time.h>

#include "rselstr.h"

#include "new_c.h"
#include "cline/cline.h"
#include "lns/lns.h"
#include "rline/rline.h"

#include "minmax.h"
#include "rootlist.h"

using namespace cf;

#define max_po_li 1000

////////////// KERNEL //////////////////

jmp_buf fatal_error_exit;

int16_t nStrings;
extern int nIncline;
Handle HCCOM;
Handle HCPAGE;

struct PL;
struct PL
{
        int Beg_X;
        int Beg_Y;
        int End_X;
        int End_Y;
        Bool fl_use;
        PL* next;
        PL* prev;
};

/////////////////////////////////

# define __RGB__(r,g,b)          ((uint32_t)(((uchar)(r)|((uint16_t)((uchar)(g))<<8))|(((uint32_t)(uchar)(b))<<16)))

/////////////////////////////////

extern uint16_t run_options;
extern Handle MainWindowD;
extern Handle hShowString;
extern Handle hShowCells;
extern Handle ShowVertCells;
extern Handle VertCells;
extern Handle NegaCells;
extern Handle ShowNegaCells;
extern Handle hStrPointedD;
extern Handle RselstrTime;
extern Handle hExit;
extern Handle hTime;
extern Handle hDrawComp;
extern BLOCK** pBlockPointer;

//////////////////////////////////
static void LayoutFromCPAGE(Handle hCPAGE, CCOM_handle hCCOM);
int IsInPoly(const cf::Point16& a, POLY_* pPoly);
Bool dphShowString;

void RotatePageToIdeal(void);
void RotatePageToReal(void);
void LayoutPart2(void);
void LayoutPart1(void);
void BlocksExtract(void);
Bool32 ReadRoots(CCOM_handle hCCOM);
void EnableDebug(void);
void progress_finish(void);
void DrawRect(Handle wnd, uint32_t OperCode, uint32_t color, int top, int bottom, int left,
        int right);
Bool InitHstrMas(CHSTR_Objects** ppRc, int len);
void DelHstrMas(CHSTR_Objects *masp);
Bool AddLenHstrMas(CHSTR_Objects** ppRc, int& len, int add);
void PointedProc(void);
Bool LineInCell(Rect32* CellRect, PL* po_li);
void SomeDraw(void);
void DrawComps(Handle hCCOM);

RSELSTR_FUNC(Bool32) RSELSTR_ExtractTextStrings(CCOM_handle hCCOM, Handle hCPAGE)
{
    LDPUMA_Skip(RselstrTime);
    int t;
    MainWindowD = NULL;
    MainWindowD = LDPUMA_GetWindowHandle("Изображение после разворота");
    if (!MainWindowD)
        MainWindowD = LDPUMA_GetWindowHandle("Main");

    PAGEINFO info;
    if (GetPageInfo(hCPAGE,&info))
        nIncline = info.Incline2048;
    else
        nIncline = 0;

    HCCOM = hCCOM;
    HCPAGE = hCPAGE;

    dphShowString = !LDPUMA_Skip(hShowString);

    SetReturnCode_rselstr(IDS_ERR_NO);
    Open_Res_Log();

    if (setjmp (fatal_error_exit)) {
        progress_finish();
        return FALSE;
    }

    if (!LDPUMA_Skip(hDrawComp))
        DrawComps(hCCOM);

    EnableDebug();
    PageLayoutStrings(hCCOM, hCPAGE);

    if (!LDPUMA_Skip(hTime))
        t = clock();
    PointedProc();
    if (!LDPUMA_Skip(hTime))
        LDPUMA_ConsoleN("Точечность = %d", clock() - t);

    SomeDraw();
    Close_Res_Log();

    LDPUMA_Skip(hExit);
    return TRUE;
}

void PageLayoutStrings(CCOM_handle hCCOM, Handle hCPAGE)
{
    if (ReadRoots(hCCOM)) {
        run_options = FORCE_ONE_COLUMN;

        LayoutPart1();

        nStrings = 0;

        LayoutFromCPAGE(hCPAGE, hCCOM);

        nStrings = CSTR_GetMaxNumber();

        LayoutPart2();
        if (!LDPUMA_Skip(hShowString)) {
            LDPUMA_Console("Нажмите любую клавишу...");
            LDPUMA_WaitUserInput(NULL, NULL);
            LDPUMA_DeleteRects(NULL, PUMA_MODULE_RBLOCK);
        }
    }
}

void LayoutFromCPAGE(Handle hCPAGE, CCOM_handle hCCOM)
{
    Handle h = NULL;
    POLY_ block;
    int nBlocks = FIRST_REGULAR_BLOCK_NUMBER;
    Point16 pLeftTop, pRightTop, pLeftBottom, pRightBottom;
    ROOT * pRoot = NULL;
    uint32_t BlockNumber;
    Bool WasVertCells = FALSE;
    Bool WasNegaCells = FALSE;
    Bool dpShowVertCells = !LDPUMA_Skip(ShowVertCells);
    Bool dpShowNegaCells = !LDPUMA_Skip(ShowNegaCells);
    Bool dphShowCells = !LDPUMA_Skip(hShowCells);

    RotatePageToReal();

    PAGEINFO PInfo;
    CPAGE_GetPageData(hCPAGE, PT_PAGEINFO, (void*) &PInfo, sizeof(PInfo));

    RotatePageToIdeal();
    // piter
    // remove ALL
    for (pRoot = cf::Roots::first(); pRoot < pAfterRoots; pRoot++) {
        pRoot -> nBlock = REMOVED_BLOCK_NUMBER;
    }

    BlockNumber = 1;

    for (h = CPAGE_GetBlockFirst(hCPAGE, TYPE_TEXT); h != NULL; h = CPAGE_GetBlockNext(hCPAGE, h,
            TYPE_TEXT)) {
        uint32_t f = CPAGE_GetBlockFlags(hCPAGE, h);
        if (CPAGE_GetBlockData(hCPAGE, h, TYPE_TEXT, &block, sizeof(block)) != sizeof(block)) {
            SetReturnCode_rselstr(CPAGE_GetReturnCode());
            longjmp(fatal_error_exit, -1);
        }
        if (block.negative == TYPE_NEGATIVE ||
                block.orient == TYPE_DOWNUP ||
                block.orient == TYPE_UPDOWN)
            continue;

        cf::Roots::add(ROOT());

        for (pRoot = cf::Roots::first(); pRoot < pAfterRoots; pRoot++) {
            pLeftTop.rx() = pRoot->xColumn + 1;
            pLeftTop.ry() = pRoot->yRow + 1;
            pRightTop.rx() = pRoot->xColumn + pRoot->nWidth - 1;
            pRightTop.ry() = pRoot->yRow + 1;
            pLeftBottom.rx() = pRoot->xColumn + 1;
            pLeftBottom.ry() = pRoot->yRow + pRoot->nHeight - 1;
            pRightBottom.rx() = pRoot->xColumn + pRoot->nWidth - 1;
            pRightBottom.ry() = pRoot->yRow + pRoot->nHeight - 1;

            if (IsInPoly(pLeftTop, &block) ||
                    IsInPoly(pRightTop, &block) ||
                    IsInPoly(pLeftBottom, &block) ||
                    IsInPoly(pRightBottom, &block)) {
                pRoot->nBlock = BlockNumber + FIRST_REGULAR_BLOCK_NUMBER;
                pRoot->nUserNum = BlockNumber;
            }
        }
        CPAGE_SetBlockInterNum(hCPAGE, h, BlockNumber);
        BlockNumber++;
    }

    //**********************************************

    if (CPAGE_GetReturnCode()) {
        SetReturnCode_rselstr(CPAGE_GetReturnCode());
        longjmp(fatal_error_exit, -1);
    }

    BlocksExtract();

    Bool type_vert;
    Bool type_neg;
    uchar* ImageTurn = (uchar*) PUMA_IMAGE_TURN;
    uchar* ImageUser = (uchar*) PUMA_IMAGE_USER;
    uchar* ImageOrto = (uchar*) PUMA_IMAGE_ORTOMOVE;
    uchar* ImageBin = (uchar*) PUMA_IMAGE_BINARIZE;
    CHSTR_Objects* Hstr = NULL;
    int nObjects = 0;
    int len_Objects = 50;
    int add_len_Objects = 10;
    int j;
    if (!InitHstrMas(&Hstr, len_Objects))
        return;

    //негативы и вертикальные фрагменты как блоки текста

    ::Rect16 Rc;
    for (h = CPAGE_GetBlockFirst(hCPAGE, TYPE_TEXT); h != NULL; h = CPAGE_GetBlockNext(hCPAGE, h,
            TYPE_TEXT)) {
        CPAGE_GetBlockData(hCPAGE, h, TYPE_TEXT, &block, sizeof(POLY_));
        if (block.negative == TYPE_NEGATIVE || block.orient == TYPE_UPDOWN || block.orient
                == TYPE_DOWNUP) {
            Rc.bottom = block.com.Vertex[2].y();
            Rc.top = block.com.Vertex[0].y();
            Rc.left = block.com.Vertex[0].x();
            Rc.right = block.com.Vertex[2].x();
            if (nIncline >= 0) {
                REAL_XY(Rc.left, Rc.top);
                REAL_XY(Rc.right, Rc.bottom);
            }
            else {
                REAL_XY(Rc.right, Rc.top);
                REAL_XY(Rc.left, Rc.bottom);
            }
            type_vert = 0;
            type_neg = 0;
            if (block.orient == TYPE_DOWNUP)
                type_vert = 1;
            if (block.orient == TYPE_UPDOWN)
                type_vert = 2;
            if (block.negative == TYPE_NEGATIVE)
                type_neg = 1;

            if (nObjects >= len_Objects)
                AddLenHstrMas(&Hstr, len_Objects, add_len_Objects);
            if (nObjects < len_Objects) {
                Hstr[nObjects].block_number = BlockNumber;
                Hstr[nObjects].Rc.bottom = Rc.bottom;
                Hstr[nObjects].Rc.top = Rc.top;
                Hstr[nObjects].Rc.left = Rc.left;
                Hstr[nObjects].Rc.right = Rc.right;
                Hstr[nObjects].type_neg = type_neg;
                Hstr[nObjects].type_vert = type_vert;

                if (type_neg) {
                    if (PInfo.Images & IMAGE_ORTOMOVE) {
                        for (j = 0; j < CPAGE_MAXNAME - 1 && ImageOrto[j] != '\0'; j++)
                            Hstr[nObjects].ImageName[j] = ImageOrto[j];
                        Hstr[nObjects].ImageName[j] = '\0';
                    }
                    else {
                        if (PInfo.Images & IMAGE_TURN) {
                            for (j = 0; j < CPAGE_MAXNAME - 1 && ImageTurn[j] != '\0'; j++)
                                Hstr[nObjects].ImageName[j] = ImageTurn[j];
                            Hstr[nObjects].ImageName[j] = '\0';
                        }
                        else if (PInfo.Images & IMAGE_BINARIZE) {
                            for (j = 0; j < CPAGE_MAXNAME - 1 && ImageBin[j] != '\0'; j++)
                                Hstr[nObjects].ImageName[j] = ImageBin[j];
                            Hstr[nObjects].ImageName[j] = '\0';
                        }
                        else {
                            for (j = 0; j < CPAGE_MAXNAME - 1 && ImageUser[j] != '\0'; j++)
                                Hstr[nObjects].ImageName[j] = ImageUser[j];
                            Hstr[nObjects].ImageName[j] = '\0';
                        }
                    }
                }
                else
                    Hstr[nObjects].ImageName[0] = '\0';

                CPAGE_SetBlockInterNum(hCPAGE, h, BlockNumber);
                BlockNumber++;
                nObjects++;
            }
        }
    }

    int t;
    if (!LDPUMA_Skip(hTime))
        t = clock();
    if (nObjects > 0)
        RSELSTR_PutObjects(hCPAGE, hCCOM, Hstr, nObjects);
    if (!LDPUMA_Skip(hTime))
        LDPUMA_ConsoleN("Время формирования необычных строк = %d", clock() - t);

    DelHstrMas(Hstr);
}

void file_string(STRING * s)
{
    CSTR_line lin_in;
    ::Rect16 r = { s->xLeft, s->yTop, s->xRight, s->yBottom };
    Rect32 left, right;
    Bool32 filtr = FALSE;

    nStrings++;

    if (dphShowString)
        LDPUMA_DrawRect(NULL, &r, nIncline / 2, __RGB__(0,127,0), -100, PUMA_MODULE_RBLOCK);

    lin_in = CSTR_NewLine(nStrings, 0);
    if (lin_in == (CSTR_line) NULL) {
        SetReturnCode_rselstr(CSTR_GetReturnCode());
        longjmp(fatal_error_exit, -1);
    }
    else {
        int i = 0;
        ROOT * pRoot = NULL;
        CCOM_comp * com = NULL;

        CSTR_rast_attr attr = { 0 };
        CSTR_rast rst = { 0 };
        CSTR_attr lattr;

        lattr.incline = nIncline; // Incline*2048 = tg
        lattr.fragment = s->nBlock - FIRST_REGULAR_BLOCK_NUMBER;
        lattr.col = s->xLeft;
        lattr.row = s->yTop;
        lattr.wid = s->xRight - s->xLeft + 1;
        lattr.hei = s->yBottom - s->yTop + 1;
        strcpy((char*) lattr.VersionName, "RBLOCKS"); // 64 bytes
        lattr.Flags = CSTR_STR_No;
        if (s->uFlags & CSTR_STR_CapDrop) {
            lattr.Flags |= CSTR_STR_CapDrop;
        }

        CSTR_SetLineAttr(lin_in, &lattr);
        // OLEG : Delete Garbage
        if (s->nDust > s->nLetters * 1) {
            int32_t le, ri, nri, nle;
            for (le = 32000, ri = -16000, i = 0; i < s->nLetters; i++) {
                com = cf::Roots::at(s->pLettersList[i])->pComp;
                if (le > com->left)
                    le = com->left;
                if (ri < com->left + com->w)
                    ri = com->left + com->w;
            }
            // find left & right letters limit
            left.left = right.left = 32000;
            left.top = right.top = 32000;
            left.right = right.right = -16000;
            left.bottom = right.bottom = -16000;
            for (nri = nle = i = 0; i < s->nDust; i++) {
                com = cf::Roots::at(s->pDustList[i])->pComp;
                if(!com)
                    continue;

                if (com->w * com->h < 15) {
                    CCOM_comp com1 = *com;
                    com1.upper = com1.upper - (int16_t) (nIncline * com1.left / 2048);
                    com1.left = com1.left + (int16_t) (nIncline * com1.upper / 2048);
                    if (com->left + com->w <= le) {
                        nle++;
                        if (left.left > com1.left)
                            left.left = com1.left;
                        if (left.right < com1.left + com->w)
                            left.right = com1.left + com1.w;
                        if (left.top > com1.upper)
                            left.top = com1.upper;
                        if (left.bottom < com1.upper + com->h)
                            left.bottom = com1.upper + com1.h;
                    }
                    if (com->left >= ri) {
                        nri++;
                        if (right.left > com1.left)
                            right.left = com1.left;
                        if (right.right < com1.left + com->w)
                            right.right = com1.left + com1.w;
                        if (right.top > com1.upper)
                            right.top = com1.upper;
                        if (right.bottom < com1.upper + com->h)
                            right.bottom = com1.upper + com1.h;
                    }
                } // end of if select sizes
            } // end of dust cycl
            if (nle > 10 && left.bottom - left.top > 10 || nri > 10 && right.bottom - right.top
                    > 10)
                filtr = TRUE;
        } // end of if num dust > num let * ...
        // dust
        for (i = 0; i < s->nDust; i++) {
            pRoot = cf::Roots::at(s->pDustList[i]);
            com = pRoot->pComp;

            if(!com)
                continue;

            if (filtr && com->w * com->h < 15) {
                CCOM_comp com1 = *com;
                com1.upper = com1.upper - (int16_t) (nIncline * com1.left / 2048);
                com1.left = com1.left + (int16_t) (nIncline * com1.upper / 2048);
                if (com1.upper >= left.top && com1.upper + com1.h <= left.bottom && com1.left
                        >= left.left && com1.left + com1.w <= left.right)
                    continue;
                if (com1.upper >= right.top && com1.upper + com1.h <= right.bottom && com1.left
                        >= right.left && com1.left + com1.w <= right.right)
                    continue;
            }
            if ((rst = (CSTR_rast) CSTR_NewRaster(lin_in, com->left, com->upper, com->w))
                    != (CSTR_rast) NULL) {
                attr.row = com->upper;
                attr.col = com->left;
                attr.w = com->w;
                attr.h = com->h;
                attr.r_row = com->upper;
                attr.r_col = com->left;
                attr.flg = CSTR_f_dust;
                CSTR_SetAttr(rst, &attr);
                CSTR_StoreComp(rst, (uchar*) com->linerep, 1, com->scale);
                if (com->vers)
                    CSTR_StoreCollection(rst, com->vers);
            }
        }
        // letters
        for (i = 0; i < s->nLetters; i++) {
            pRoot = cf::Roots::at(s->pLettersList[i]);
            com = pRoot->pComp;

            if(!com)
                continue;

            // begin Oleg 2003.08.29
            if (((com->w + (1 << com->scale) - 1) >> com->scale) > 127 || ((com->h + (1
                    << com->scale) - 1) >> com->scale) > 63)
                continue;
            // end Oleg 2003.08.29

            if ((rst = (CSTR_rast) CSTR_NewRaster(lin_in, com->left, com->upper, com->w))
                    != (CSTR_rast) NULL) {
                attr.row = com->upper;
                attr.col = com->left;
                attr.w = com->w;
                attr.h = com->h;
                attr.r_row = com->upper;
                attr.r_col = com->left;
                attr.flg = CSTR_f_let | CSTR_f_bad;
                CSTR_SetAttr(rst, &attr);
                CSTR_StoreComp(rst, (uchar*) com->linerep, 1, com->scale);
                if (s->uFlags & CSTR_STR_CapDrop) {
                    CCOM_comp *ci = com;
                    CCOM_comp *co = CSTR_GetComp(rst);
                    CCOM_USER_BLOCK ub;
                    int32_t n;

                    ub.code = CCOM_UB_CAPDROPLN;
                    ub.data = (uchar*) &n;
                    if (CCOM_GetUserBlock(ci, &ub))
                        CCOM_SetUserBlock(co, &ub);
                }
                if (com->vers)
                    CSTR_StoreCollection(rst, com->vers);
            }
        }
        CSTR_PackLine(lin_in);
    }

}

int IsInPoly(Point16 a, void * pPoly)
{
    int i, y, n, ind;
    int Count = 0;
    POLY_ *p;
    p = (POLY_*) pPoly;
    n = p->com.count;
    for (i = 0; i < n; i++) {
        int j = (i + 1) % n;
        if (p->com.Vertex[i].y() == p->com.Vertex[j].y())
            continue;
        if (p->com.Vertex[i].y() > a.y() && p->com.Vertex[j].y() > a.y())
            continue;
        if (p->com.Vertex[i].y() < a.y() && p->com.Vertex[j].y() < a.y())
            continue;
        y = p->com.Vertex[i].y();
        ind = i;
        if (p->com.Vertex[j].y() > y) {
            y = p->com.Vertex[j].y();
            ind = j;
        }
        if ((y == a.y()) && (p->com.Vertex[ind].x() >= a.x()))
            Count++;
        else if (MIN(p->com.Vertex[i].y(), p->com.Vertex[j].y()) == a.y())
            continue;
        else {
            double t = ((double) (a.y() - p->com.Vertex[i].y()) / ((double) (p->com.Vertex[j].y()
                    - (double) p->com.Vertex[i].y())));
            if (t > 0 && t < 1 && (double) p->com.Vertex[i].x() + t
                    * ((double) p->com.Vertex[j].x() - (double) p->com.Vertex[i].x())
                    >= (double) a.x())
                Count++;
        }
    }
    return Count & 1;
}

Bool AddLenHstrMas(CHSTR_Objects** ppRc, int& len, int add)
{
    CHSTR_Objects *dop;
    int i;
    if (!(InitHstrMas(&dop, len)))
        return FALSE;
    for (i = 0; i < len; i++) {
        dop[i].block_number = (*ppRc)[i].block_number;
        for (int j = 0; j < CPAGE_MAXNAME; j++)
            dop[i].ImageName[j] = (*ppRc)[i].ImageName[j];
        dop[i].Rc.bottom = (*ppRc)[i].Rc.bottom;
        dop[i].Rc.top = (*ppRc)[i].Rc.top;
        dop[i].Rc.left = (*ppRc)[i].Rc.left;
        dop[i].Rc.right = (*ppRc)[i].Rc.right;
        dop[i].type_neg = (*ppRc)[i].type_neg;
        dop[i].type_vert = (*ppRc)[i].type_vert;
    }

    DelHstrMas(*ppRc);
    if (!(InitHstrMas(ppRc, len + add))) {
        (*ppRc) = dop;
        return FALSE;
    }

    for (i = 0; i < len; i++) {
        (*ppRc)[i].block_number = dop[i].block_number;
        for (int j = 0; j < CPAGE_MAXNAME; j++)
            (*ppRc)[i].ImageName[j] = dop[i].ImageName[j];
        (*ppRc)[i].Rc.bottom = dop[i].Rc.bottom;
        (*ppRc)[i].Rc.top = dop[i].Rc.top;
        (*ppRc)[i].Rc.left = dop[i].Rc.left;
        (*ppRc)[i].Rc.right = dop[i].Rc.right;
        (*ppRc)[i].type_neg = dop[i].type_neg;
        (*ppRc)[i].type_vert = dop[i].type_vert;
    }

    len += add;
    DelHstrMas(dop);
    return TRUE;
}

void DelHstrMas(CHSTR_Objects *masp)
{
    delete[] masp;
}

Bool InitHstrMas(CHSTR_Objects** ppRc, int len)
{
    (*ppRc) = NULL;
    if (!((*ppRc) = new CHSTR_Objects[len]))
        return FALSE;
    return TRUE;
}

void PointedProc()
{

    CSTR_line str;
    CSTR_attr lattr;
    PAGEINFO info;
    GetPageInfo(HCPAGE, &info);
    nIncline = info.Incline2048;

    int Right;
    int Left;
    int Top;
    int Bottom;
    Bool fl_beg;
    Bool fl_end;

    CLINE_handle hCLINE = CLINE_GetMainContainer();
    CLINE_handle hline;
    CPDLine data_line;

    PL po_li[max_po_li] = { 0 };
    int n_po_li = 0;
    PL* now = po_li;
    PL* end;
    now->next = NULL;
    now->prev = NULL;

    //  if(fl_use_cline)
    //  {
    for (hline = CLINE_GetFirstLine(hCLINE); hline; hline = CLINE_GetNextLine(hline)) {
        data_line = CLINE_GetLineData(hline);
        if (!data_line)
            continue;

        if (data_line->Type == NR_DT_LinePointed) {
            if ((data_line->Flags & LI_IsTrue) && (data_line->Dir == LD_Horiz)) {
                if (n_po_li < max_po_li) {
                    now->Beg_X = data_line->Line.Beg_X;
                    now->Beg_Y = data_line->Line.Beg_Y;
                    now->End_X = data_line->Line.End_X;
                    now->End_Y = data_line->Line.End_Y;
                    now->fl_use = FALSE;
                    now->next = NULL;
                    if (!n_po_li)
                        now->prev = NULL;
                    else {
                        now->prev = &po_li[n_po_li - 1];
                        now->prev->next = now;
                    }
                    IDEAL_XY(now->Beg_X, now->Beg_Y);
                    IDEAL_XY(now->End_X, now->End_Y);
                    now++;
                    n_po_li++;
                }
            }
        }
    }
    end = now - 1;
    for (str = CSTR_FirstLine(0); str; str = CSTR_NextLine(str, 0)) {
        if (!CSTR_GetLineAttr(str, &lattr))
            continue;

        Left = lattr.col;
        Right = Left + lattr.wid - 1;
        Bottom = lattr.row + lattr.hei + 1;
        Top = (lattr.row + Bottom) / 2;
        fl_beg = FALSE;
        fl_end = FALSE;

        //Навешивание флага присутствия точечных линий
        now = po_li;
        for (; now; now = now->next) {
            if (now->fl_use)
                continue;

            if (now->Beg_Y < Bottom && now->Beg_Y > Top) {
                fl_beg = TRUE;
            }
            if (now->End_Y < Bottom && now->End_Y > Top) {
                fl_end = TRUE;
            }
            if (fl_beg) {
                fl_beg = FALSE;
                fl_end = FALSE;
            }
            else {
                if (fl_end) {
                    fl_end = FALSE;
                }
            }
        }
        CSTR_SetLineAttr(str, &lattr);
    }

}

Bool LineInCell(Rect32* CellRect, PL* po_li)
{
    int left = po_li->Beg_X;
    int right = po_li->End_X;
    if (CellRect->left < left) {
        if (CellRect->right > right)
            return TRUE;
        else {
            if (CellRect->right - left > ((right - left) >> 1))
                return TRUE;
            return FALSE;
        }
    }
    else {
        if (right - CellRect->left > ((right - left) >> 1))
            return TRUE;
        else
            return FALSE;
    }
}

void SomeDraw()
{
    if (!LDPUMA_Skip(hStrPointedD)) {
        CSTR_line str;
        CSTR_attr lattr;
        ::Rect16 r;
        PAGEINFO info;
        GetPageInfo(HCPAGE, &info);
        nIncline = info.Incline2048;
        Bool fl_exist = FALSE;

        for (str = CSTR_FirstLine(0); str; str = CSTR_NextLine(str, 0)) {
            if (CSTR_GetLineAttr(str, &lattr)) {
                if (lattr.Flags & (CSTR_STR_END_POINTED | CSTR_STR_BEGIN_POINTED)) {
                    r.bottom = lattr.row + lattr.hei - 1;
                    r.top = lattr.row;
                    r.left = lattr.col;
                    r.right = lattr.col + lattr.wid - 1;
                    LDPUMA_DrawRect(MainWindowD, &r, nIncline / 2, __RGB__(255,0,0), -100, 117);
                    fl_exist = TRUE;
                }
            }
        }
        if (fl_exist) {
            LDPUMA_WaitUserInput(hStrPointedD, MainWindowD);
            LDPUMA_DeleteRects(MainWindowD, 117);
        }
    }
}

