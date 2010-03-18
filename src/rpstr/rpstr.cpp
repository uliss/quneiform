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

#include <cstdio>
#include <cstring>
#include <cstdlib>

#include "rpstr.h"
#include "cstr/cstr.h"
#include "ccom/ccom.h"
#include "resource.h"
#include "dpuma.h"
#include "cor_incl.h"
#include "cap_drop.h"
#include "ligas.h"	// 12.06.2002 E.P.
#define  MAX_KEG    40
#define  KEG_RANGE  34
uint16_t wHeightRC = 0;
uint16_t wLowRC = RPSTR_ERR_NO;
Bool32 snap_enable = TRUE;
Bool32 exit_enable = FALSE, skip_line = FALSE;
;
Handle hSnapSpell = 0, hSnapWordSpell = 0, hSnapMatch = 0, hSnapCapDrop = 0;
Handle hSnapEndWord = NULL, hSnapStartWord = NULL;
Handle hVertCompD;
Bool32 gbFax100 = FALSE;
Bool32 gbGarbage = TRUE;

char* sp_err = "no spell errors";

// from COR_SPEL.C
Bool32 correct_line_spell(CSTR_line line, CSTR_rast* re, CSTR_rast* rb, int32_t line_num,
        Bool32 disable_new_dict, Bool32 disable_check_word, int32_t* rf);

extern Bool32 rpstr_correct_spell(CSTR_line ln, CSTR_rast *addbeg, CSTR_rast *addend,
        int32_t *linefrag, int32_t num_ln, Bool32 disable_new_dict, Bool32 disable_check_word);
extern Bool32 rpstr_txt_spell(char * s, uchar lang);

extern int Snap_Console(char *text);

uchar language = 3;

static uchar s_lang4page = -1;

static uchar set_lang4page(const uchar lang) {
    s_lang4page = lang;
    return s_lang4page;
}

static void show_spell(CSTR_rast c) {
    uchar wrd[80] = {0};
    uchar *w = wrd, lang;
    char buf[160];
    Bool32 nonrec = FALSE;
    CSTR_rast_attr attr;
    UniVersions uni;
    CSTR_rast co = c;
    *w = 0;
    do {
        CSTR_GetAttr(c, &attr);
        c = CSTR_GetPrev(c);
    }
    while (c && attr.flg & (CSTR_f_let | CSTR_f_bad));
    if (!c)
        c = CSTR_GetNext(CSTR_GetFirstRaster(CSTR_GetRasterLine(co)));
    else
        c = CSTR_GetNext(CSTR_GetNext(c));
    CSTR_GetAttr(c, &attr);
    for (lang = attr.language; c; c = CSTR_GetNext(c)) {
        CSTR_GetAttr(c, &attr);
        if (!(attr.flg & (CSTR_f_let | CSTR_f_bad)))
            break;
        if (attr.flg & CSTR_f_bad)
            nonrec = TRUE;
        CSTR_GetCollectionUni(c, &uni);
        strcat((char*) w, (char*) uni.Alt[0].Code);
    }

    strcpy(buf, "<");
    strcat(buf, (char*) wrd);
    strcat(buf, "> static : ");
    if (rpstr_txt_spell((char*) wrd, lang))
        strcat(buf, "y");
    else
        strcat(buf, " n");
    Snap_Console(buf);
    return;
}

uint32_t myMonitorProc(Handle wnd, Handle hwnd, uint32_t message, uint32_t wParam, uint32_t lParam) {
    int ret = 0;
    uint32_t pos;
    CSTR_rast r = (CSTR_rast) 0;

    pos = LDPUMA_CSTR_GetPosition((uint32_t *) &r);

    if (!is_turkish_language(language)) // 12.06.2002 E.P.
        language = 3;

    return (uint32_t) (ret);
}

/////////////////////
// common functions
/////////////////////

Bool32 RPSTR_Init(uint16_t wHeightCode, Handle /*hStorage*/) {
    wHeightRC = wHeightCode;
    wLowRC = RPSTR_ERR_NO;
    snap_enable = TRUE;
    exit_enable = FALSE;
    LDPUMA_Init(0, NULL);
    //kegl_snap_init();
    LDPUMA_Registry(&hSnapSpell, "Словарь и постобработка", NULL);
    LDPUMA_RegistryHelp(hSnapSpell, "Словарь", FALSE);
    LDPUMA_Registry(&hSnapWordSpell, "Запретить проверку словами", hSnapSpell);
    LDPUMA_Registry(&hSnapMatch, "Показать проверку словами", hSnapWordSpell);
    LDPUMA_RegistryHelp(hSnapWordSpell, "Разрешить проверку словами", FALSE);
    LDPUMA_Registry(&hSnapCapDrop, "Запретить обработку БУКВИЦ", hSnapSpell);
    LDPUMA_RegistryHelp(hSnapCapDrop, "Запретить обработку БУКВИЦ", FALSE);

    LDPUMA_Registry(&hSnapEndWord, "Отменить проверку окончаний", hSnapSpell);
    LDPUMA_RegistryHelp(hSnapEndWord, "Разрешить или нет любые замены окончаний", FALSE);
    LDPUMA_Registry(&hSnapStartWord, "Отменить осторожность в именах", hSnapSpell);
    LDPUMA_RegistryHelp(hSnapStartWord, "Обращаться или нет одинаково со всеми словами", FALSE);

    LDPUMA_Registry(&hVertCompD, "Прорисовка букв в вертикальных строках!", NULL);
    LDPUMA_RegistryHelp(hVertCompD, "Разрешить прорисовку букв в вертикальных строках", FALSE);
    return TRUE;
}

void RPSTR_Done(void) {
    wLowRC = RPSTR_ERR_NO;
    wHeightRC = 0;
    snap_enable = TRUE;
    exit_enable = FALSE;
    LDPUMA_Done();
    return;
}

uint32_t RPSTR_GetReturnCode(void) {
    if (wLowRC == RPSTR_ERR_NO)
        return 0;
    return (wHeightRC << 16) | (wLowRC - RPSTR_ERR_MIN);
}

char* RPSTR_GetReturnString(uint32_t dwError) {
    uint16_t rc = (uint16_t) (dwError & 0xFFFF + RPSTR_ERR_MIN);
    static char szBuffer[512];

    if (dwError >> 16 != wHeightRC)
        wLowRC = RPSTR_ERR_NOTIMPLEMENT;

    if (rc > 0 && rc <= RPSTR_ERR_MAX - RPSTR_ERR_MIN)
        strcpy((char *) szBuffer, RPSTR_error_name[rc]);
    else
        return NULL;

    return szBuffer;
}

Bool32 RPSTR_NewPage(void) {
    wLowRC = RPSTR_ERR_NO;
    snap_enable = TRUE;
    exit_enable = FALSE;
    return TRUE;
}

Bool32 RPSTR_CorrectIncline(int32_t version) {
    int n = CSTR_GetMaxFragment(version);
    for (int i = 0; i <= n; i++) {
        if (!incl_test_fragment(version, i)) {
            incl_ordering(version, i, incl_calculate(version, i));
        }
    }
    return TRUE;
}

Bool32 correct_line_spell(CSTR_line line, CSTR_rast* re, CSTR_rast* rb, int32_t line_num,
        Bool32 disable_new_dict, Bool32 disable_check_word, int32_t* rf) {
    char snapstr[256];

    exit_enable = FALSE;
    snap_enable = TRUE;
    skip_line = FALSE;

    if (!LDPUMA_SkipEx(hSnapSpell, TRUE, TRUE, 1) && snap_enable) {
        sprintf(snapstr, "before spelling line %d", line_num);
        Snap_Console(snapstr);
        LDPUMA_RasterText("before spelling");
        //		LDPUMA_CSTR_Monitor(hSnapSpell, (uint32_t) line, 0, myMonitorProc);

        if (exit_enable) {
            LDPUMA_DestroyRasterWnd();
            return FALSE;
        }
    }

    if (!rpstr_correct_spell(line, rb, re, rf, line_num, disable_new_dict, disable_check_word)) {
        sprintf(snapstr, "speller error : %s ", sp_err);
        Snap_Console(snapstr);
        LDPUMA_DestroyRasterWnd();
        return FALSE;
    }
    if (!skip_line && !LDPUMA_SkipEx(hSnapSpell, TRUE, TRUE, 1) && snap_enable) {
        sprintf(snapstr, "after spelling line %d", line_num);
        Snap_Console(snapstr);
        LDPUMA_RasterText("after spelling");
        //		LDPUMA_CSTR_Monitor(hSnapSpell, (uint32_t) line, 0, myMonitorProc);
    }
    if (exit_enable) {
        LDPUMA_DestroyRasterWnd();
        return FALSE;
    }

    return TRUE;
}

Bool32 RPSTR_CorrectLineSpell(CSTR_line line, int32_t line_num, CSTR_rast* re, CSTR_rast* rb,
        int32_t* rf) {
    Bool32 disable_new_dict = FALSE, disable_check_word = FALSE;

    exit_enable = FALSE;
    snap_enable = TRUE;
    skip_line = FALSE;

    // 12.06.2002 E.P.
    if (is_turkish_language(language))
        disable_new_dict = TRUE;

    if (!correct_line_spell(line, re, rb, line_num, disable_new_dict, disable_check_word, rf))
        return FALSE;

    if (!LDPUMA_SkipEx(hSnapSpell, TRUE, TRUE, 1))
        LDPUMA_DestroyRasterWnd();

    return TRUE;
}

Bool32 RPSTR_CorrectSpell(int32_t version) {
    int32_t n, i, rf = -1;
    CSTR_line line;
    CSTR_rast rb = (CSTR_rast) 0, re = (CSTR_rast) 0;
    Bool32 disable_new_dict = FALSE, disable_check_word = FALSE;

    exit_enable = FALSE;
    snap_enable = TRUE;
    skip_line = FALSE;
    n = CSTR_GetMaxNumber();

    // 12.06.2002 E.P.
    if (is_turkish_language(language))
        disable_new_dict = TRUE;

    LDPUMA_StartLoop(hSnapSpell, n);
    for (i = 1; i <= n; i++) {
        LDPUMA_LoopNext(hSnapSpell);
        line = CSTR_GetLineHandle(i, version);
        if (!line)
            continue;
        skip_line = FALSE;

        if (!correct_line_spell(line, &re, &rb, i, disable_new_dict, disable_check_word, &rf))
            return FALSE;
    }

    if (re) {
        CSTR_rast_attr attr;
        CSTR_GetAttr(re, &attr);
        attr.flg_spell |= CSTR_fa_spell_nocarrying;
        CSTR_SetAttr(re, &attr);
    }
    if (!LDPUMA_SkipEx(hSnapSpell, TRUE, TRUE, 1))
        LDPUMA_DestroyRasterWnd();
    return TRUE;
}

Bool32 RPSTR_CollectCapDrops(int32_t version) {
    int n = CSTR_GetMaxFragment(version);
    CSTR_line ln_cd, ln_main;
    if (!LDPUMA_SkipEx(hSnapCapDrop, FALSE, FALSE, 0))
        return FALSE;
    for (int i = 0; i <= n; i++) {
        if (capdrop_test_fragment(version, i, &ln_cd, &ln_main)) {
            capdrop_collection(version, i, ln_cd, ln_main);
        }
    }
    return TRUE;
}

Bool32 RPSTR_SetImportData(uint32_t dwType, void * pData) {

    wLowRC = RPSTR_ERR_NO;
#define CASE_DATA(a,b,c)	case a: c = *(b *)pData; break;
#define CASE_PDATA(a,b,c)	case a: c = (b)pData; break;
    switch (dwType) {
    CASE_DATA(RPSTR_Bool32_Fax100,Bool32,gbFax100)
        // 12.06.2002 E.P.
    case RPSTR_FNIMP_LANGUAGE:
        language = *(uchar*) pData;
        set_lang4page(language);
        break;

#undef CASE_DATA
#undef CASE_PDATA

    default:
        wLowRC = RPSTR_ERR_NOTIMPLEMENT;
        return FALSE;
    }
    return TRUE;
}

