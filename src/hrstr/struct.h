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

#ifndef H_struct_h
#define H_struct_h

#include "tuner.h"
#include "cttypes.h"
#include "version.h"
#include "comp.h"
#include "common/point.h"

#ifdef  HUGE_IMAGE
const int WORLD_MAX_HEIGHT = 10000;
const int WORLD_MAX_WIDTH = 10000; //7000
const int WORLD_MAX_RESOLUTION = 800;
const int WORLD_MIN_RESOLUTION = 50;
#else
const int WORLD_MAX_HEIGHT = 4096; // ordinary image
const int WORLD_MAX_WIDTH = 4096;
const int WORLD_MAX_RESOLUTION = 800;
const int WORLD_MIN_RESOLUTION = 50;
#endif

const int LPOOL_SIZE = 8000;
const int RASTER_MAX_HEIGHT = 63;
const int RASTER_MAX_WIDTH = 128;
const int SMALL_SIZE = 8;

// AK! Atention!!!!//////////////////////////////////////////////////////
//  имеет смысл при необходимости отловить глюки так как падает при попытке
//  принять dust за letter
#define err_pnlet(c)       ((c))->nextl=((c))->prevl=(cell*)(0xffff0000)//
#define err_pnnextlet(c)   ((c))->nextl=             (cell*)(0xffff0000)//
#define err_pnprevlet(c)   ((c))->prevl=             (cell*)(0xffff0000)//
#define err_pncell(c)      ((c))->next =((c))->prev =(cell*)(0xffff0000)//
#define err_pnnextcell(c)  ((c))->next =             (cell*)(0xffff0000)//
#define err_pnprevcell(c)  ((c))->prev =             (cell*)(0xffff0000)//
struct dust_comp_struc
{
        uint16_t size; // =1
        int16_t upper;
        int16_t left;
        uchar h; // >0
        uchar w; // >0
        uchar raster[8];
};
typedef struct dust_comp_struc dust_comp;

struct file_comp_struct
{
        uint16_t size; // =1
        int16_t upper;
        int16_t left;
        uchar h; // =0
        uchar w; // =0
        uint32_t offset;
        uint16_t lth;
        uchar scale;
        uchar reserv;
};
typedef struct file_comp_struct file_comp;

typedef struct
{ // == ExcBox from ExcDefs.h
        int16_t row; // real row of comp
        int16_t col; // real column of  comp
        int16_t h; // height of  comp
        int16_t w; // width of  comp
        uint16_t flag; // some logical info
        int32_t user; // working var for user
} gcomp;

struct frame_struct
{
        cf::Point16 topleft, topright, botleft, botright;
};
typedef struct frame_struct FRAME;

struct rule_struct
{
        cf::Point16 beg, end;
        uchar width;
        uchar type;
#define VERT_LN  0
#define HOR_LN   1
#define UNDRLN   2
#define FRM_LN   4
};
typedef struct rule_struct STRLN;

#include "cutstr.h"
#include "embbox.h"
#endif
