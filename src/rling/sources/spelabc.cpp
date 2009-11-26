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

/* ------------------------------------------------------------------ **
 History:
 -------------------------------------------------------------------
 01-15-94 10:11pm, Mike
 1. Marked data are not used anywhere in program. I comments them
 and function that load ones.

 // ---- Revision History
 // - 1 SPELABC.C 15-Jan-94,22:26:58 Start point.
 // - 2 SPELABC.C 31-Jan-94,17:49:48 Spanish overturned ! and ? are added.
 // - 3 SPELABC.C 31-Jan-94,20:20:40 Symbols << and >> have been described
 as delimiters.
 // ---- Revision History
 */

#include "spelmode.h"
#include <stdlib.h>

#include "speldefs.h"
#include "spelfunc.h"
#include "lang.h"
#include "tigeremulate.h"

#include "compat_defs.h"

extern int16_t vocs_NOK;

void std_init(void);
void initcode(void);
void initstdn(void);

int16_t ABCSize;
uchar alphabet[3][ABCSIZE];
KEYTYPE codetable[256];
KEYTYPE codepermit[256];

uchar all_lt[256];
uchar tabstdn[256]; /* nmbs of STD symb in order      */

STD std_;

uchar _2low(uchar c)
{
    if (all_lt[c] != E_CP) {
        return c;
    }

    else {
        return alphabet[1][codetable[c]];
    }
}

/* ------------------------------------------------------------------ */

uchar _2cap(uchar c)
{
    if (all_lt[c] != E_OD) {
        return c;
    }

    else {
        return alphabet[0][codetable[c]];
    }
}

/* ------------------------------------------------------------------ */

Bool IsVowel(uchar c)
{
    return alphabet[2][codetable[c]] == (uchar) '^' ? TRUE : FALSE;
}

/* ------------------------------------------------------------------ */

extern void init_tab_alpha(uchar *s1, uchar *s2, int16_t num);
Bool InitializeAlphabet(int16_t CountryCode)
{
    uchar w[ABCSIZE + 1];
    int16_t h;

    if ((h = TBOPEN( 6, language, BO_READ_TEXT, S_IREAD )) < 0) {
        vocs_NOK |= 1 << 6;
        return FALSE;
    }

    if (TGGETS( h, (char*)w, sizeof(w)) == (char*) w) {
        ABCSize = atoi((char*) w);
    }

    if (TGGETS( h, (char*)memset( w, 0, sizeof(w)), sizeof(w)) == (char*) w) {
        memcpy(alphabet[0], w, ABCSIZE);
    }

    if (TGGETS( h, (char*)memset( w, 0, sizeof(w)), sizeof(w)) == (char*) w) {
        memcpy(alphabet[1], w, ABCSIZE);
    }

    if (TGGETS( h, (char*)memset( w, 0, sizeof(w)), sizeof(w)) == (char*) w) {
        memcpy(alphabet[2], w, ABCSIZE);
    }

    else {
        TGCLOSE(h);
        return FALSE;
    }

    /* 01-15-94 10:10pm, Mike     */
    /*  if ( !setUpAcsGrps(h) ) { */
    /*    TGCLOSE(h);             */
    /*    return FALSE;           */
    /*  }                         */
    TGCLOSE(h);

    if ((strlen((char*) alphabet[0]) != (uint16_t) ABCSize) || (strlen(
                                                                    (char*) alphabet[1]) != (uint16_t) ABCSize)) {
        return FALSE;
    }

    dectable_init();
    std_init();
    initcode();
    initstdn();
    init_tab_alpha(alphabet[0], alphabet[1], ABCSize);
    CountryCode = 0;
    return TRUE;
}
void dectable_init(void)
{
    int16_t i, j;
    uint16_t index;
    memset(codetable, 0, sizeof(codetable));
    memset(codepermit, 0, sizeof(codepermit));

    for (i = 0; i < 2; i++) {
        for (j = 0; j < ABCSize; j++) {
            index = (uchar) alphabet[i][j];
            codetable[index] = (KEYTYPE) j;
            codepermit[index] = (KEYTYPE) 1;
        }
    }

    return;
}

void std_init(void)
{
    int i;
    memset(&std_, 0, sizeof(std_));

    for (i = 0; i < ABCSize; i++) {
        std_.stdlt[i].code = alphabet[0][i];
    }

    std_.stdlt[ABCSize - 1].attr = 1;
}

void initcode(void)
{
    register int16_t i;
    memset(all_lt, 0, sizeof(all_lt));

    for (i = 0; i < ABCSize; i++) {
        all_lt[alphabet[0][i]] = E_CP;
        all_lt[alphabet[1][i]] = E_OD;
    }

    all_lt[(uchar) BLANK] = _BLK;
    all_lt[(uchar) SPACE] = _SPC;
    all_lt[(uchar) TRADEMARK] = E_DL;
    all_lt[(uchar) WRDDIV] = _WDV; // 11-23-93 Mike : DASH problem...
    /* i=(uchar)WRDDIV; all_lt[i]=E_DL; */
    all_lt[0x1e] = _SP1;
    all_lt[0x1f] = _SP2;
    /*       !            .           ?                              */
    all_lt[0x21] = all_lt[0x2e] = all_lt[0x3f] = _TRM;
    /* !(flipped)    ?(flipped)                                      */
#ifndef WIN_FRENCH
    all_lt[0xA1] = _TRM;
#endif
    all_lt[0xA0] = _TRM;
    /*       <<      >>                                              */
    all_lt[0xAB] = all_lt[0xAD] = _TRM;
    /*       '                                                       */
    all_lt[0x27] = _APF;
    /*       0            1             2            3            4  */
    all_lt[0x30] = all_lt[0x31] = all_lt[0x32] = all_lt[0x33] = all_lt[0x34] =
                                                                    /*       5            6             7            8            9  */
                                                                    all_lt[0x35] = all_lt[0x36] = all_lt[0x37] = all_lt[0x38] = all_lt[0x39] =
                                                                                                                                    /*       $                                                       */
                                                                                                                                    all_lt[0x24] = _DIG;
    /*      "            #             %            &           (    */
    all_lt[0x22] = all_lt[0x23] = all_lt[0x25] = all_lt[0x26] = all_lt[0x28] =
                                                                    /*       )           *            +             ,            /   */
                                                                    all_lt[0x29] = all_lt[0x2a] = all_lt[0x2b] = all_lt[0x2c] = all_lt[0x2f] =
                                                                                                                                    /*       :            ;            <            =           >    */
                                                                                                                                    all_lt[0x3a] = all_lt[0x3b] = all_lt[0x3c] = all_lt[0x3d] = all_lt[0x3e] =
                                                                                                                                                                                                    /*      @            [             \             ]           {   */
                                                                                                                                                                                                    all_lt[0x40] = all_lt[0x5b] = all_lt[0x5c] = all_lt[0x5d] = all_lt[0x7b] =
                                                                                                                                                                                                                                                                    /*       |           }                                           */
                                                                                                                                                                                                                                                                    all_lt[0x7c] = all_lt[0x7d] = all_lt[0xab] = all_lt[0xbb] = E_DL;
    /*                            <<          >>                     */
    return;
}

int16_t symcode(char *ed)
{
    return all_lt[(uchar) * ed];
}

/**********************************************************************/
/* This procedure sets standard symb numbers in SDT-list (tabstdn)    */
/**********************************************************************/
void initstdn(void)
{
    register int16_t i, j;
    memset(tabstdn, 0xFF, sizeof(tabstdn));

    for (i = 0; i < sizeof(STD) / sizeof(LT); i++) {
        j = std_.stdlt[i].code; /* symb ASCII mnb */
        tabstdn[j] = (uchar) i;
    }

    return;
}

/**********************************************************************/
/* This procedure converts STD symbol code to it number in STD-list   */
/**********************************************************************/
int16_t getstdn(char *ed)
{
    return tabstdn[(uchar) * ed];
}
