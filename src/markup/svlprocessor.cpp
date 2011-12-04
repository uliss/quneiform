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

    if(!ReadSVLFromPageContainer(LineInfoA, image_))
        Debug() << "ReadSVLFromPageContainer failed.\n";
}

void SVLProcessor::countSVLStep2()
{
    assert(LineInfoB);

    LineInfoB->Hor.Lns = NULL;

    if (VLinefBufferB == NULL)
        VLinefBufferB = LineInfoB->Ver.Lns = (LineInfo *) calloc(sizeof(LineInfo), MAX_LINES);

    if(!ReadSVLFromPageContainer(LineInfoB, image_))
        Debug() << "ReadSVLFromPageContainer failed.\n";
}

}


