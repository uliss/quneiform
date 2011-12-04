/***************************************************************************
 *   Copyright (C) 2011 by Serge Poltavski                                 *
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

#include <cstdlib>
#include <cassert>

#include "svlprocessor.h"
#include "shortverticallinesfilter.h"

#include "common/debug.h"
#include "lns/lnsdefs.h"
#include "cline/cline.h"

namespace cf {

SVLProcessor::SVLProcessor(RMPreProcessImage * image) :
    image_(image),
    LineInfoA(NULL),
    LineInfoB(NULL),
    VLinefBufferA(NULL),
    VLinefBufferB(NULL)
{
    LineInfoA = (LinesTotalInfo*) calloc(1, sizeof(LinesTotalInfo));
    LineInfoB = (LinesTotalInfo*) calloc(1, sizeof(LinesTotalInfo));
}

SVLProcessor::~SVLProcessor() {
    free(VLinefBufferA);
    free(VLinefBufferB);
    free(LineInfoA);
    free(LineInfoB);
}

void SVLProcessor::filter() {
    SVLFilter(LineInfoA, LineInfoB, image_);
}

void SVLProcessor::countSVLStep1()
{
    assert(LineInfoA);

    LineInfoA->Hor.Lns = NULL;

    if (VLinefBufferA == NULL)
        VLinefBufferA = LineInfoA->Ver.Lns = (LineInfo *) calloc(sizeof(LineInfo), MAX_LINES);

    readSVL(LineInfoA);
}

void SVLProcessor::countSVLStep2()
{
    assert(LineInfoB);

    LineInfoB->Hor.Lns = NULL;

    if (VLinefBufferB == NULL)
        VLinefBufferB = LineInfoB->Ver.Lns = (LineInfo *) calloc(sizeof(LineInfo), MAX_LINES);

    readSVL(LineInfoB);
}

void readLineData(CPDLine data, LineInfo& info) {
    info.A.rx() = data->Line.Beg_X;
    info.A.ry() = data->Line.Beg_Y;
    info.B.rx() = data->Line.End_X;
    info.B.ry() = data->Line.End_Y;
    info.Thickness = data->Line.Wid10 / 10;
    info.Flags = data->Flags;
}

void SVLProcessor::readSVL(LinesTotalInfo * info)
{
    bool fl_break = false;
    int num = 0;
    int count = 0;
    CLINE_handle hline = CLINE_GetFirstLine(image_->hCLINE);
    info->Hor.Cnt = 0;
    info->Ver.Cnt = 0;

    while (hline) {
        fl_break = false;
        CPDLine cpdata = CLINE_GetLineData(hline);

        if(!cpdata) {
            hline = CLINE_GetNextLine(hline);
        }
        else {
            if(count >= MAX_LINES) {
                fl_break = true;
            }
            else {
                count++;

                if (cpdata->Dir == LD_Horiz) {
                    if (info->Hor.Lns) {
                        num = info->Hor.Cnt;
                        readLineData(cpdata, info->Hor.Lns[num]);
                        (info->Hor.Cnt)++;
                    }
                }
                else {
                    if (info->Ver.Lns) {
                        num = info->Ver.Cnt;
                        readLineData(cpdata, info->Ver.Lns[num]);
                        (info->Ver.Cnt)++;
                    }
                }
            }

            hline = CLINE_GetNextLine(hline);
        }

        if (fl_break)
            break;
    }
}

}



