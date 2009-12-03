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

#ifndef __PUMADEF_H__
#define __PUMADEF_H__

#include "lang_def.h"
#include <climits>

// Format codes
enum puma_format_t {
    PUMA_TOEDNATIVE = 0,
    PUMA_TOTEXT = 0x02,
    PUMA_TOSMARTTEXT = 0x04,
    PUMA_TORTF = 0x08,
    PUMA_TOTABLETXT = 0x0100,
    PUMA_TOTABLECSV = 0x0200,
    PUMA_TOTABLEDBF = 0x0400,
    PUMA_TOTABLEODBC = 0x0800,
    PUMA_TOTABLEWKS = 0x1000,
    PUMA_TOHTML = 0x2000,
    PUMA_TOHOCR = 0x4000,
    PUMA_DEBUG_TOTEXT = 0x8000
};

//       Codes
enum puma_code_t {
    PUMA_CODE_UNKNOWN = 0x0000,
    PUMA_CODE_ASCII = 0x0001,
    PUMA_CODE_ANSI = 0x0002,
    PUMA_CODE_KOI8 = 0x0004,
    PUMA_CODE_ISO = 0x0008,
    PUMA_CODE_UTF8 = 0x0010
};

enum puma_format_mode_t {
    PUMA_FORMAT_NONE = 0x0040,
    PUMA_FORMAT_ALL = 0x0001,
    PUMA_FORMAT_ONLY_FRAME = 0x0002
};

enum puma_table_t {
    PUMA_TABLE_NONE = 0,
    PUMA_TABLE_DEFAULT = 1,
    PUMA_TABLE_ONLY_LINE = 2,
    PUMA_TABLE_ONLY_TEXT = 3,
    PUMA_TABLE_LINE_TEXT = 4
};

enum puma_picture_t {
    PUMA_PICTURE_NONE = 0, PUMA_PICTURE_ALL = 1
};


// Флаги обновления контейнеров
enum puma_update_flag_t {
    FLG_UPDATE_NO = 0,
    FLG_UPDATE = UINT_MAX,
    FLG_UPDATE_CCOM = 0x1,
    FLG_UPDATE_CPAGE = 0x2,
};

// constants
enum puma_module_t {
    PUMA_MODULE_CCOM = 1,
    PUMA_MODULE_CPAGE = 2,
    PUMA_MODULE_CFIO = 3,
    PUMA_MODULE_CSTR = 4,
    PUMA_MODULE_CIMAGE = 5,
    PUMA_MODULE_CLINE = 6,
    PUMA_MODULE_REXC = 100,
    PUMA_MODULE_RLTABLE = 101,
    PUMA_MODULE_RBLOCK = 102,
    PUMA_MODULE_RSTR = 103,
    PUMA_MODULE_RLINE = 104,
    PUMA_MODULE_RFRMT = 105,
    PUMA_MODULE_RCOLOR = 106,
    PUMA_MODULE_RIMAGE = 107,
    PUMA_MODULE_RPSTR = 108,
    PUMA_MODULE_RPIC = 109,
    PUMA_MODULE_RANALFRM = 110,
    PUMA_MODULE_CED = 111,
    PUMA_MODULE_ROUT = 112,
    PUMA_MODULE_RAGR = 113,
    PUMA_MODULE_RSTUFF = 114,
    PUMA_MODULE_RVERLINE = 115,
    PUMA_MODULE_RSL = 116,
    PUMA_MODULE_SLINEDOT = 117,
    PUMA_MODULE_RNORM = 118,
    PUMA_MODULE_RRECCOM = 119,
    //Tanya
    PUMA_MODULE_RMSEGMENT = 120,
    PUMA_MODULE_SSTREAK = 121,
    PUMA_MODULE_RCORRKEGL = 122
};

#define NAME_IMAGE_INPUT     "Original image"
#define NAME_IMAGE_BINARIZE  "Binarized image"
#define NAME_IMAGE_DELLINE   "Image with deleted lines"
#define NAME_IMAGE_ORTOMOVE  "Image after ortomove"

#define PUMA_IMAGE_USER     "ImageFromUser"
#define PUMA_IMAGE_BINARIZE "ImageBinarize"
#define PUMA_IMAGE_DELLINE  "ImageAfterDeleteLine"
#define PUMA_IMAGE_ROTATE   "ImageAfterRotate"
#define PUMA_IMAGE_TURN     "ImageAfterTurn"
#define PUMA_IMAGE_ORTOMOVE "ImageAfterOrtoMove"
#endif
