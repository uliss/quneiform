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

#include "lnslang.h"

extern const uchar bit_byte[8] = {
    0x01,   0x02,   0x04,   0x08,
    0x10,   0x20,   0x40,   0x80
};

extern const uchar lbit_byte[8] = {
    0x80, 0x40, 0x20,   0x10,
    0x08, 0x04, 0x02,   0x01
};

extern const uint16_t bit_word[16] = {
    0x0001, 0x0002, 0x0004, 0x0008,
    0x0010, 0x0020, 0x0040, 0x0080,
    0x0100, 0x0200, 0x0400, 0x0800,
    0x1000, 0x2000, 0x4000, 0x8000
};

extern const uint16_t lbit_word[16] = {
    0x0080, 0x0040, 0x0020, 0x0010,
    0x0008, 0x0004, 0x0002, 0x0001,
    0x8000, 0x4000, 0x2000, 0x1000,
    0x0800, 0x0400, 0x0200, 0x0100
};

extern const uint16_t lbit_word0[16] = {
    0xFF7F,     0xFFBF, 0xFFDF, 0xFFEF,
    0xFFF7, 0xFFFB, 0xFFFD, 0xFFFE,
    0x7FFF,     0xBFFF, 0xDFFF, 0xEFFF,
    0xF7FF,     0xFBFF,  0xFDFF,    0xFEFF
};

extern const uchar left_0[256] = {
    0x00, 0x01, 0x03, 0x03, 0x07, 0x07, 0x07, 0x07, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F,
    0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F,
    0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F,
    0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F,
    0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F,
    0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F,
    0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F,
    0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
};

extern const uchar right_0[256] = {
    0x00, 0xFF, 0xFE, 0xFF, 0xFC, 0xFF, 0xFE, 0xFF, 0xF8, 0xFF, 0xFE, 0xFF, 0xFC, 0xFF, 0xFE, 0xFF,
    0xF0, 0xFF, 0xFE, 0xFF, 0xFC, 0xFF, 0xFE, 0xFF, 0xF8, 0xFF, 0xFE, 0xFF, 0xFC, 0xFF, 0xFE, 0xFF,
    0xE0, 0xFF, 0xFE, 0xFF, 0xFC, 0xFF, 0xFE, 0xFF, 0xF8, 0xFF, 0xFE, 0xFF, 0xFC, 0xFF, 0xFE, 0xFF,
    0xF0, 0xFF, 0xFE, 0xFF, 0xFC, 0xFF, 0xFE, 0xFF, 0xF8, 0xFF, 0xFE, 0xFF, 0xFC, 0xFF, 0xFE, 0xFF,
    0xC0, 0xFF, 0xFE, 0xFF, 0xFC, 0xFF, 0xFE, 0xFF, 0xF8, 0xFF, 0xFE, 0xFF, 0xFC, 0xFF, 0xFE, 0xFF,
    0xF0, 0xFF, 0xFE, 0xFF, 0xFC, 0xFF, 0xFE, 0xFF, 0xF8, 0xFF, 0xFE, 0xFF, 0xFC, 0xFF, 0xFE, 0xFF,
    0xE0, 0xFF, 0xFE, 0xFF, 0xFC, 0xFF, 0xFE, 0xFF, 0xF8, 0xFF, 0xFE, 0xFF, 0xFC, 0xFF, 0xFE, 0xFF,
    0xF0, 0xFF, 0xFE, 0xFF, 0xFC, 0xFF, 0xFE, 0xFF, 0xF8, 0xFF, 0xFE, 0xFF, 0xFC, 0xFF, 0xFE, 0xFF,
    0x80, 0xFF, 0xFE, 0xFF, 0xFC, 0xFF, 0xFE, 0xFF, 0xF8, 0xFF, 0xFE, 0xFF, 0xFC, 0xFF, 0xFE, 0xFF,
    0xF0, 0xFF, 0xFE, 0xFF, 0xFC, 0xFF, 0xFE, 0xFF, 0xF8, 0xFF, 0xFE, 0xFF, 0xFC, 0xFF, 0xFE, 0xFF,
    0xE0, 0xFF, 0xFE, 0xFF, 0xFC, 0xFF, 0xFE, 0xFF, 0xF8, 0xFF, 0xFE, 0xFF, 0xFC, 0xFF, 0xFE, 0xFF,
    0xF0, 0xFF, 0xFE, 0xFF, 0xFC, 0xFF, 0xFE, 0xFF, 0xF8, 0xFF, 0xFE, 0xFF, 0xFC, 0xFF, 0xFE, 0xFF,
    0xC0, 0xFF, 0xFE, 0xFF, 0xFC, 0xFF, 0xFE, 0xFF, 0xF8, 0xFF, 0xFE, 0xFF, 0xFC, 0xFF, 0xFE, 0xFF,
    0xF0, 0xFF, 0xFE, 0xFF, 0xFC, 0xFF, 0xFE, 0xFF, 0xF8, 0xFF, 0xFE, 0xFF, 0xFC, 0xFF, 0xFE, 0xFF,
    0xE0, 0xFF, 0xFE, 0xFF, 0xFC, 0xFF, 0xFE, 0xFF, 0xF8, 0xFF, 0xFE, 0xFF, 0xFC, 0xFF, 0xFE, 0xFF,
    0xF0, 0xFF, 0xFE, 0xFF, 0xFC, 0xFF, 0xFE, 0xFF, 0xF8, 0xFF, 0xFE, 0xFF, 0xFC, 0xFF, 0xFE, 0xFF
};

/********************************************************************/
extern const uchar bit1_cnt[256] = { 0, 1, 1, 2, 1, 2, 2, 3,
                                     1, 2, 2, 3, 2, 3, 3, 4,
                                     1, 2, 2, 3, 2, 3, 3, 4,
                                     2, 3, 3, 4, 3, 4, 4, 5,
                                     1, 2, 2, 3, 2, 3, 3, 4,
                                     2, 3, 3, 4, 3, 4, 4, 5,
                                     2, 3, 3, 4, 3, 4, 4, 5,
                                     3, 4, 4, 5, 4, 5, 5, 6,
                                     1, 2, 2, 3, 2, 3, 3, 4,
                                     2, 3, 3, 4, 3, 4, 4, 5,
                                     2, 3, 3, 4, 3, 4, 4, 5,
                                     3, 4, 4, 5, 4, 5, 5, 6,
                                     2, 3, 3, 4, 3, 4, 4, 5,
                                     3, 4, 4, 5, 4, 5, 5, 6,
                                     3, 4, 4, 5, 4, 5, 5, 6,
                                     4, 5, 5, 6, 5, 6, 6, 7,
                                     1, 2, 2, 3, 2, 3, 3, 4,
                                     2, 3, 3, 4, 3, 4, 4, 5,
                                     2, 3, 3, 4, 3, 4, 4, 5,
                                     3, 4, 4, 5, 4, 5, 5, 6,
                                     2, 3, 3, 4, 3, 4, 4, 5,
                                     3, 4, 4, 5, 4, 5, 5, 6,
                                     3, 4, 4, 5, 4, 5, 5, 6,
                                     4, 5, 5, 6, 5, 6, 6, 7,
                                     2, 3, 3, 4, 3, 4, 4, 5,
                                     3, 4, 4, 5, 4, 5, 5, 6,
                                     3, 4, 4, 5, 4, 5, 5, 6,
                                     4, 5, 5, 6, 5, 6, 6, 7,
                                     3, 4, 4, 5, 4, 5, 5, 6,
                                     4, 5, 5, 6, 5, 6, 6, 7,
                                     4, 5, 5, 6, 5, 6, 6, 7,
                                     5, 6, 6, 7, 6, 7, 7, 8
                                   };

extern const uchar bit0_cnt[256] = { 8, 7, 7, 6, 7, 6, 6, 5,
                                     7, 6, 6, 5, 6, 5, 5, 4,
                                     7, 6, 6, 5, 6, 5, 5, 4,
                                     6, 5, 5, 4, 5, 4, 4, 3,
                                     7, 6, 6, 5, 6, 5, 5, 4,
                                     6, 5, 5, 4, 5, 4, 4, 3,
                                     6, 5, 5, 4, 5, 4, 4, 3,
                                     5, 4, 4, 3, 4, 3, 3, 2,
                                     7, 6, 6, 5, 6, 5, 5, 4,
                                     6, 5, 5, 4, 5, 4, 4, 3,
                                     6, 5, 5, 4, 5, 4, 4, 3,
                                     5, 4, 4, 3, 4, 3, 3, 2,
                                     6, 5, 5, 4, 5, 4, 4, 3,
                                     5, 4, 4, 3, 4, 3, 3, 2,
                                     5, 4, 4, 3, 4, 3, 3, 2,
                                     4, 3, 3, 2, 3, 2, 2, 1,
                                     7, 6, 6, 5, 6, 5, 5, 4,
                                     6, 5, 5, 4, 5, 4, 4, 3,
                                     6, 5, 5, 4, 5, 4, 4, 3,
                                     5, 4, 4, 3, 4, 3, 3, 2,
                                     6, 5, 5, 4, 5, 4, 4, 3,
                                     5, 4, 4, 3, 4, 3, 3, 2,
                                     5, 4, 4, 3, 4, 3, 3, 2,
                                     4, 3, 3, 2, 3, 2, 2, 1,
                                     6, 5, 5, 4, 5, 4, 4, 3,
                                     5, 4, 4, 3, 4, 3, 3, 2,
                                     5, 4, 4, 3, 4, 3, 3, 2,
                                     4, 3, 3, 2, 3, 2, 2, 1,
                                     5, 4, 4, 3, 4, 3, 3, 2,
                                     4, 3, 3, 2, 3, 2, 2, 1,
                                     4, 3, 3, 2, 3, 2, 2, 1,
                                     3, 2, 2, 1, 2, 1, 1, 0
                                   };

extern const uchar first_bit1_pos[256] = {
    /*       0 1 2 3 4 5 6 7 8 9 a b c d e f */
    /*0*/    8, 7, 6, 6, 5, 5, 5, 5, 4, 4, 4, 4, 4, 4, 4, 4,
    /*1*/    3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
    /*2*/    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    /*3*/    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    /*4*/    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    /*5*/    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    /*6*/    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    /*7*/    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    /*8*/    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    /*9*/    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    /*a*/    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    /*b*/    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    /*c*/    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    /*d*/    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    /*e*/    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    /*f*/    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};
extern const uchar first_bit0_pos[256] = {
    /*       0 1 2 3 4 5 6 7 8 9 a b c d e f */
    /*0*/    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    /*1*/    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    /*2*/    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    /*3*/    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    /*4*/    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    /*5*/    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    /*6*/    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    /*7*/    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    /*8*/    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    /*9*/    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    /*a*/    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    /*b*/    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    /*c*/    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    /*d*/    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    /*e*/    3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
    /*f*/    4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 6, 6, 7, 8
};

extern const uchar nFirstByte[] = {
    /*       0 1 2 3 4 5 6 7 8 9 a b c d e f */
    /*0*/    0, 1, 1, 2, 1, 1, 2, 3, 1, 1, 1, 1, 2, 2, 3, 4,
    /*1*/    1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 4, 5,
    /*2*/    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    /*3*/    2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 4, 4, 4, 5,
    /*4*/    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    /*5*/    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    /*6*/    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    /*7*/    3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 5, 5, 6, 7,
    /*8*/    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    /*9*/    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    /*a*/    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    /*b*/    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    /*c*/    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    /*d*/    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    /*e*/    3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
    /*f*/    4, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 6, 6, 7, 8
};
