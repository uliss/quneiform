/***************************************************************************
 *   Copyright (C) 2010 by Serge Poltavsky                                 *
 *   serge.poltavski@gmail.com                                             *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program. If not, see <http://www.gnu.org/licenses/>   *
 ***************************************************************************/

#include <cstring>
#include "crtffunc.h"
#include "rfrmtoptions.h"

#include "common/font.h"
#include "cline/cline.h"
#include "cpage/cpage.h"

#include "minmax.h"

void RtfUnionRect_CRect_CRect(RECT *s1, RECT *s2) {
    s1->left = MIN(s1->left, s2->left);
    s1->right = MAX(s1->right, s2->right);
    s1->top = MIN(s1->top, s2->top);
    s1->bottom = MAX(s1->bottom, s2->bottom);
}

void RtfAssignRect_CRect_CRect(RECT *s1, RECT *s2) {
    s1->left = s2->left;
    s1->right = s2->right;
    s1->top = s2->top;
    s1->bottom = s2->bottom;
}

void RtfAssignRect_CRect_Rect16(RECT *s1, Rect16 *s2) {
    s1->left = s2->left;
    s1->right = s2->right;
    s1->top = s2->top;
    s1->bottom = s2->bottom;
}

int16_t GetRealSize(const char* str, int16_t len, int16_t FontSize, int16_t FontNumber,
        int16_t* strHeight) {
    cf::Size size;
    int n_Weight = 600;
    uchar bItalic;

    if (cf::RfrmtOptions::useBold() && ((char) FontNumber & cf::FORMAT_FONT_BOLD))
        n_Weight = 800;

    if (cf::RfrmtOptions::useSize())
        FontSize = cf::DefFontSize / 2;

    if (cf::RfrmtOptions::useItalic() && ((char) FontNumber & cf::FORMAT_FONT_ITALIC))
        bItalic = TRUE;
    else
        bItalic = FALSE;

    //  TODO
    *strHeight = size.height();
    return size.width(); //in twips
}

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

    if (cf::RfrmtOptions::useBold() && ((char) FontNumber & cf::FORMAT_FONT_BOLD))
        koef = float(4. / 5.);

    sz = new char[len + 1];
    //  if( str->GetLength() < MAX_BUFFER_SIZE )
    strcpy(sz, str);
    //  else
    //      return FALSE;
    //Заполняем хвост строки пробелами
    memset(sz + strlen(str), ' ', len - strlen(str));
    sz[len] = 0;

    if (FontPointSize > cf::ChangedKeglSize) {
        int Count = FontPointSize - cf::ChangedKeglSize;

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

Bool CheckLines(RECT* Rect, Bool FlagVer, cf::SectorInfo *SectorInfo) {
    const int LMin = 500;

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

