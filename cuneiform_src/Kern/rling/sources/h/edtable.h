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

/* Length of ED records by codes.
   Length of fixed record is table entry less than 64.
   Length of variable record is one or two bytes in record.
   Bits 128 and 64 in this table mean variable record,
   and lower 4 bits show displacement of length in record.
    64 - record contains one-byte length.
   128 - record contains two-byte length.
*/
#define ED_ONE_BYTE_LENGTH 64
#define ED_TWO_BYTE_LENGTH 128

#define ED_LLP(p)   (ed_table[*(p)])
#define ED_PPP(p)   ( (p) + (ED_LLP(p)&15) )
#define ED_LREC(p) ( (ED_LLP(p)<64)? ED_LLP(p):( (ED_LLP(p)&64)? (*ED_PPP(p)):(*(int16_t*)(ED_PPP(p))) ) )
#define ED_VAR_LREC(p) ((ED_LLP(p)&64)? (*(uchar*)(ED_PPP(p))): (*(int16_t*)(ED_PPP(p))))

static unsigned char ed_table[]=
    {
    10,     /* 0  SS_BITMAP_REF         */
    4,      /* 1  SS_TEXT_REF           */
    4,      /* 2  SS_FONT_KEGL          */
    2,      /* 3  SS_KEGL               */
    4,      /* 4  SS_SHIFT              */
    2,      /* 5  SS_RETR_LEVEL         */
    2,      /* 6  SS_UNDERLINE          */
    2,      /* 7  SS_DENS_PRINT         */
    2,      /* 8  SS_TABUL              */
    0x41,   /* 9  SS_TABL_TABUL         */
    0x84,   /* 0A SS_SHEET_DESCR        */
    4,      /* 0B SS_FRAGMENT  (extended to 14 bytes in ED file header) */
    2,      /* 0C SS_STEP_BACK          */
    4,      /* 0D SS_LINE_BEG           */
    4,      /* 0E SS_POSITION           */
    2,      /* 0F SS_LANGUAGE           */
    18,     /* 10 SS_TABL_CONFORM_SIZES */
    2,      /* 11 SS_GROUP_WORDS        */
    2,      /* 12 SS_GROUP_SYMBOLS      */
    0,      /* 13 Unused code           */
    0,      /* 14 Unused code           */
    2,      /* 15 ASCII symbol ''      */
    8,      /* 16 SS_BORDER             */
    0x82,   /* 17 SS_TABLE_HEADER       */
    0x82,   /* 18 SS_LIST_OF FRAGMENTS  */
    0,      /* 19 Unused code           */
    0,      /* 1A Unused code           */
    0,      /* 1B Unused code           */
    0,      /* 1C Special code. Extention ED. Piter 01.08.96 14:53 */
    2,      /* 1D SS_AKSANT             */
    2,      /* 1E SS_NEG_HALF_SPACE     */
    2       /* 1F SS_POS_HALF_SPACE     */
    };
