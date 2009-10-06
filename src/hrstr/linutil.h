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

#include "struct.h"

Bool is_liga_ff(uchar c) ;
Bool is_liga_ffl(uchar c);
uchar to_lower(uchar c);
uchar to_upper(uchar c);
int16_t  is_lower(uchar ch);
int16_t  is_upper(uchar ch);
int16_t  is_russian(uchar ch);
int16_t  is_english(uchar ch);
int16_t is_polish_special(uchar ch);
int16_t is_czech_special(uchar ch);	// 05.09.2000 E.P.
int16_t is_roman_special(uchar ch);
int16_t is_hungar_special(uchar ch);
int16_t is_serbian_special(uchar ch);

int16_t is_slovenian_special(uchar ch);	// 25.05.2001 E.P.
int16_t isnot_slovenian(uchar let); // 25.05.2001 E.P.

// 09.07.2001 E.P.
int16_t is_latvian_special(uchar ch);
int16_t isnot_latvian(uchar let);

int16_t is_lithuanian_special(uchar ch);
int16_t isnot_lithuanian(uchar let);

int16_t is_estonian_special(uchar ch);
int16_t isnot_estonian(uchar let);
int16_t is_baltic_palka(uchar c);

int16_t is_cen_bottom_accent(uchar c);		// 12.09.2000 E.P.
int16_t is_baltic_bottom_accent(uchar c);	// 10.07.2001 E.P.
int16_t is_russian_baltic_conflict(uchar c);	// 17.07.2001 E.P.

// Турецкий язык 20.05.2002 E.P.
int16_t is_turkish_special(uchar ch);
int16_t isnot_turkish(uchar let);
int16_t is_turkish_bottom_accent(uchar c);
int16_t is_turkish_palka(uchar c);
int16_t is_russian_turkish_conflict(uchar c);

int16_t  isletter(uchar ch);
Bool is_digit(uchar ch);
uchar get_homot(uchar ch);
int16_t  twin(uchar ch);
//int16_t  setup_let_case(void);
int16_t  setup_let_case(int16_t);
int16_t  count_line_hi(void);
int16_t  draft_cut_hyps(int16_t bs,int16_t fl);
int16_t smart_diff(cell * c);


void tell_for_b3(int16_t hist_array[]);
int16_t flood_peak(int16_t *Hh,int16_t ind);
int16_t GetPsFromHeights(void);
uchar BracketIn(version *v);
int16_t def_upper_side(void);

extern uchar page_stat,ufa,stable_b3;
extern int16_t HIST_STATISTIC;

struct uf_bases { int16_t bb1,bb2; };

struct uf_bases uf_let_case(void);
#define arrnum(a)	(sizeof(a)/sizeof(a[0]))
