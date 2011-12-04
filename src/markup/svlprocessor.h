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

#ifndef SVLPROCESSOR_H
#define SVLPROCESSOR_H

typedef struct tagLinesTotalInfo LinesTotalInfo;
struct RMPreProcessImage;

namespace cf {

class SVLProcessor
{
public:
    SVLProcessor(RMPreProcessImage * image);
    ~SVLProcessor();

    void filter();
    void countSVLStep1();
    void countSVLStep2();
public:
    static const int MAX_LINES = 2000;
private:
    RMPreProcessImage * image_;
    LinesTotalInfo * LineInfoA;
    LinesTotalInfo * LineInfoB;
    void * VLinefBufferA;
    void * VLinefBufferB;
};

}

#endif // SVLPROCESSOR_H
