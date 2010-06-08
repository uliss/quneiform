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
// This file cration date: 17.08.99
//
// CreateRtf.cpp
//
// ============================================================================
#include <cstdio>

#include "crtfhorizontalcolumn.h"
#include "cfcompat.h"
#include "creatertf.h"
#include "cpage/cpage.h"
#include "cfio/cfio.h"
#include "ced/ced.h"
#include "ced/cedpage.h"
#include "ced/cedline.h"
#include "ced/cedparagraph.h"
#include "edfile.h"
#include "dpuma.h"
#include "lns/lnsdefs.h"
#include "linedefs.h"
#include "normdefs.h"
#include "cstr/cstrdefs.h"
#include "cline/cline.h"

#include "rfrmtoptions.h"
#include "crtfstruct.h"
#include "crtfchar.h"
#include "crtffragment.h"
#include "crtfpage.h"
#include "crtfsector.h"
#include "crtfstring.h"
#include "crtfverticalcolumn.h"
#include "crtfword.h"
#include "crtffunc.h"
#include "crtfstruct.h"
#include "font.h"

#include "common/size.h"
#include "minmax.h"
#include "common/rect.h"
#include "common/debug.h"
#include "compat/filefunc.h"

namespace CIF
{
class CEDPage;
}

#define CHEREDOVON

int16_t GetRealSizeKegl(const char* str, int16_t width, int16_t FontPointSize, int16_t FontNumber) {
    char* sz;
    uint16_t PenaltyKeglForString = 0;
    int16_t strHeight;
    float koef = 1.;
    int len = strlen(str);

    if (FontPointSize > 30) //~ в больших кеглях большая ошибка в Microsoft function
        //      *str +="     ";
        len += 5;

    else if (FontPointSize > 20)
        len++;

    //      *str +=" ";

    if (CIF::RfrmtOptions::useBold() && ((char) FontNumber & CIF::FORMAT_FONT_BOLD))
        koef = float(4. / 5.);

    sz = new char[len + 1];
    //  if( str->GetLength() < MAX_BUFFER_SIZE )
    strcpy(sz, str);
    //  else
    //      return FALSE;
    //Заполняем хвост строки пробелами
    memset(sz + strlen(str), ' ', len - strlen(str));
    sz[len] = 0;

    if (FontPointSize > CIF::ChangedKeglSize) {
        int Count = FontPointSize - CIF::ChangedKeglSize;

        for (int i = 0; i < Count; i++) {
            int16_t RealSize = GetRealSize(sz, strlen(sz), FontPointSize, FontNumber, &strHeight);

            if (RealSize > (width * koef)) {
                //криретий окончания итераций- чтобы все символы влезли в строку по ширине
                PenaltyKeglForString++;
                FontPointSize--;
            }

            else
                break;
        }
    }

    delete[] sz;
    return FontPointSize;
}

int16_t GetRealSize(const char* str, int16_t len, int16_t FontSize, int16_t FontNumber,
        int16_t* strHeight) {
    CIF::Size size;
    int n_Weight = 600, fn;
    uchar bItalic;

    if (CIF::RfrmtOptions::useBold() && ((char) FontNumber & CIF::FORMAT_FONT_BOLD))
        n_Weight = 800;

    if (CIF::RfrmtOptions::useSize())
        FontSize = CIF::DefFontSize / 2;

    if (CIF::RfrmtOptions::useItalic() && ((char) FontNumber & CIF::FORMAT_FONT_ITALIC))
        bItalic = TRUE;
    else
        bItalic = FALSE;

    fn = CIF::fontName(FontNumber);
    //  TODO
    *strHeight = size.height();
    return size.width(); //in twips
}

const int LMin = 500;

Bool CheckLines(RECT* Rect, Bool FlagVer, CIF::SectorInfo *SectorInfo) {
    if (FlagVer == TRUE && Rect->bottom - Rect->top < LMin / 2)
        return FALSE;

    Handle hPage = CPAGE_GetHandlePage(CPAGE_GetCurrentPage());
    CLINE_handle hCLINE = CLINE_GetMainContainer();
    int VCentre = Rect->top + (Rect->bottom - Rect->top) / 2;
    CLINE_handle hline = CLINE_GetFirstLine(hCLINE);
    if (!hline)
        return FALSE;

    DLine data;

    while (hline) {
        CPDLine cpdata = CLINE_GetLineData(hline);

        if ((!cpdata) || (FlagVer && (cpdata->Dir == LD_Horiz)) || ((!FlagVer) && (cpdata->Dir
                != LD_Horiz)))
            hline = CLINE_GetNextLine(hline);
        else {
            if ((cpdata->Flags & LI_IsTrue) && !(cpdata->Flags & LI_IsAtTable) && !(cpdata->Flags
                    & LI_Used) && !(cpdata->Flags & LI_FRMT_Used)) {
                if (FlagVer) {
                    if (cpdata->Line.Beg_X > Rect->left && cpdata->Line.End_X > Rect->left
                            && cpdata->Line.Beg_X < Rect->right && cpdata->Line.End_X < Rect->right
                            && VCentre > cpdata->Line.Beg_Y && VCentre < cpdata->Line.End_Y && //
                            abs(cpdata->Line.Beg_Y - cpdata->Line.End_Y) > (Rect->bottom
                                    - Rect->top) / 2)
                        return TRUE;
                } else {//Hor
                    if (((cpdata->Line.Beg_Y > Rect->top && cpdata->Line.Beg_Y < Rect->bottom)
                            || (cpdata->Line.End_Y > Rect->top && cpdata->Line.End_Y < Rect->bottom))
                            && abs(cpdata->Line.Beg_X - cpdata->Line.End_X) > SectorInfo->PaperW
                                    / 2) {
                        if (cpdata->Line.Beg_Y > Rect->top && cpdata->Line.Beg_Y < Rect->bottom) {
                            Rect->top = cpdata->Line.Beg_Y;
                            // FIXME why 10?
                            Rect->bottom = cpdata->Line.Beg_Y + 10;
                        } else {
                            Rect->top = cpdata->Line.End_Y;
                            // FIXME why 10?
                            Rect->bottom = cpdata->Line.End_Y + 10;
                        }

                        CLINE_CopyData(&data, cpdata, sizeof(DLine));
                        data.Flags |= LI_FRMT_Used;
                        CLINE_SetLineData(hline, (CPDLine) (&data));
                        return TRUE;
                    }
                }
            }

            hline = CLINE_GetNextLine(hline);
        }
    }

    return FALSE;
}

void Cleaning_LI_FRMT_Used_Flag() {
    Handle hPage = CPAGE_GetHandlePage(CPAGE_GetCurrentPage());
    CLINE_handle hCLINE = CLINE_GetMainContainer();
    CLINE_handle hline = CLINE_GetFirstLine(hCLINE);
    if (!hline)
        return;

    DLine data;

    while (hline) {
        CPDLine cpdata = CLINE_GetLineData(hline);

        if (!cpdata)
            hline = CLINE_GetNextLine(hline);
        else {
            if (cpdata->Flags & LI_FRMT_Used) {
                CLINE_CopyData(&data, cpdata, sizeof(DLine));
                data.Flags &= 0xffff7fff;
                CLINE_SetLineData(hline, (CPDLine) (&data));
            }

            hline = CLINE_GetNextLine(hline);
        }
    }
}
