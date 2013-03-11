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
#include <boost/current_function.hpp>

#include "svlprocessor.h"
#include "shortverticallinesfilter.h"

#include "common/log.h"
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

static void readLineData(CPDLine data, LineInfo& info) {
    info.A.rx() = static_cast<short>(data->Line.Beg_X);
    info.A.ry() = static_cast<short>(data->Line.Beg_Y);
    info.B.rx() = static_cast<short>(data->Line.End_X);
    info.B.ry() = static_cast<short>(data->Line.End_Y);
    info.Thickness = static_cast<uchar>(data->Line.Wid10 / 10);
    info.Flags = data->Flags;
}

static void readLineData(CPDLine data, LinesTotalInfo * info) {
    // serj: why other direction handled as vertical?
    if(data->Dir == LD_Horiz) {
        if(!info->Hor.Lns)
            return;

        int num = info->Hor.Cnt;
        readLineData(data, info->Hor.Lns[num]);
        (info->Hor.Cnt)++;
    }
    else {
        if(!info->Ver.Lns)
            return;

        int num = info->Ver.Cnt;
        readLineData(data, info->Ver.Lns[num]);
        (info->Ver.Cnt)++;
    }
}

void SVLProcessor::readSVL(LinesTotalInfo * info)
{
    int count = 0;
    CLINE_handle hline = CLINE_GetFirstLine(image_->hCLINE);
    info->Hor.Cnt = 0;
    info->Ver.Cnt = 0;

    while (hline) {
        CPDLine cpdata = CLINE_GetLineData(hline);

        if(!cpdata) {
            hline = CLINE_GetNextLine(hline);
            continue;
        }

        if(count >= MAX_LINES) {
            cfWarning(MODULE_MARKUP) << BOOST_CURRENT_FUNCTION << " Warning: too many SVL lines!";
            break;
        }

        count++;
        readLineData(cpdata, info);

        hline = CLINE_GetNextLine(hline);
    }
}

}



