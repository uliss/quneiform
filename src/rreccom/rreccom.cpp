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

#include <cstring>
#include "rreccom.h"
#include "compat_defs.h"
#include "evn32/evn.h"
#include "excdefs.h"
#include "ccom/ccom.h"
#include "alphabets/alphabetfactory.h"
#include "alphabets/digits.h"

namespace CIF {

const char *tabevn1[LANG_TOTAL] = { "rec1.dat", // LANG_ENGLISH      0
    "rec1.dat", // LANG_GERMAN      1
    "rec1.dat", // LANG_FRENCH      2
    "rec1rus.dat", // LANG_RUSSIAN      3
    "rec1.dat", // LANG_SWEDISH     4
    "rec1.dat", // LANG_SPANISH     5
    "rec1.dat", // LANG_ITALIAN     6
    "rec1r&e.dat", // LANG_RUSENG       7
    "rec1rus.dat", // LANG_UKRAINIAN    8
    "rec1rus.dat", // LANG_SERBIAN      9
    "rec1cen.dat", // LANG_CROATIAN 10
    "rec1cen.dat", // LANG_POLISH       11
    "rec1n.dat", // LANG_DANISH     12
    "rec1n.dat", // LANG_PORTUGUESE 13
    "rec1n.dat", // LANG_DUTCH      14
    "rec1.dat", // LANG_DIG         15
    "rec1uzb.dat", // LANG_UZBEK        16  // 01.09.2000 E.P.
    "rec1kaz.dat", // LANG_KAZ          17
    "rec1kaz.dat", // LANG_KAZ_ENG      18
    "rec1cen.dat", // LANG_CZECH        19
    "rec1cen.dat", // LANG_ROMAN        20
    "rec1cen.dat", // LANG_HUNGAR       21
    "rec1rus.dat", // LANG_BULGAR       22
    "rec1cen.dat", // LANG_SLOVENIAN    23
    "rec1blt.dat", // LANG_LATVIAN      24
    "rec1blt.dat", // LANG_LITHUANIAN  25
    "rec1blt.dat", // LANG_ESTONIAN 26
    "rec1tur.dat" // LANG_TURKISH       27
        };

const char *tabevn2[LANG_TOTAL] = { "rec2.dat", // LANG_ENGLISH      0
    "rec2.dat", // LANG_GERMAN      1
    "rec2.dat", // LANG_FRENCH      2
    "rec2rus.dat", // LANG_RUSSIAN      3
    "rec2.dat", // LANG_SWEDISH     4
    "rec2.dat", // LANG_SPANISH     5
    "rec2.dat", // LANG_ITALIAN     6
    "rec2r&e.dat", // LANG_RUSENG       7
    "rec2rus.dat", // LANG_UKRAINIAN    8
    "rec2rus.dat", // LANG_SERBIAN      9
    "rec2cen.dat", // LANG_CROATIAN 10
    "rec2cen.dat", // LANG_POLISH       11
    "rec2n.dat", // LANG_DANISH     12
    "rec2n.dat", // LANG_PORTUGUESE 13
    "rec2n.dat", // LANG_DUTCH      14
    "rec2.dat", // LANG_DIG         15
    "rec2uzb.dat", // LANG_UZBEK        16 // 01.09.2000 E.P.
    "rec2kaz.dat", // LANG_KAZ          17
    "rec2kaz.dat", // LANG_KAZ_ENG      18
    "rec2cen.dat", // LANG_CZECH        19
    "rec2cen.dat", // LANG_ROMAN        20
    "rec2cen.dat", // LANG_HUNGAR       21
    "rec2rus.dat", // LANG_BULGAR       22
    "rec2cen.dat", // LANG_SLOVENIAN    23
    "rec2blt.dat", // LANG_LATVIAN      24
    "rec2blt.dat", // LANG_LITHUANIAN  25
    "rec2blt.dat", // LANG_ESTONIAN 26
    "rec2tur.dat" // LANG_TURKISH       27
        };

std::string RReccom::ocr_path_(".");
char RReccom::alphabet_tbl_[256] = { };

RReccom::RReccom() :
    language_(LANG_RUSENG) {
    max_component_width_ = RASTER_MAX_WIDTH;
    max_component_height_ = RASTER_MAX_HEIGHT;
    min_component_width_ = 0;
    min_component_height_ = 0;
    //  for cuneiform pitures process
    max_scale_ = 5;
}

RReccom::~RReccom() {
}

void RReccom::eventRecognition(CCOM_comp * pcomp) {
    CCOM_comp comp = *pcomp;

    comp.scale = pcomp->scale;
    comp.w = pcomp->w;
    comp.rw = pcomp->rw;
    comp.h = pcomp->h;

    int nvers = 0;
    unsigned char evn_res[17] = "";
    if (comp.scale < 3 && (comp.w >> comp.scale) < max_component_width_ && (comp.h >> comp.scale)
            < max_component_height_) {

        if (comp.scale) {
            comp.w >>= comp.scale;
            comp.h >>= comp.scale;
            comp.rw = (comp.w + 7) / 8;
        }

        CCOM_comp ec;
        memset(&ec, 0, sizeof(CCOM_comp));
        ec.h = pcomp->h;
        ec.w = pcomp->w;
        ec.rw = pcomp->rw;
        ec.nl = pcomp->nl;
        ec.begs = pcomp->begs;
        ec.ends = pcomp->ends;
        ec.scale = pcomp->scale;

        nvers = EVNRecog_lp(&ec, comp.linerep + sizeof(int16_t), comp.size_linerep
                - sizeof(int16_t), evn_res);
        pcomp->type = ec.type;
        pcomp->cs = ec.cs;
    }

    if (nvers) {
        if (!pcomp->vers) {
            pcomp->vers = static_cast<RecVersions*> (calloc(1, sizeof(RecVersions)));
        }

        if (pcomp->cs == 255)
            nvers >>= 1;

        int32_t vers_beg = pcomp->vers->lnAltCnt;

        if (nvers + pcomp->vers->lnAltCnt > REC_MAX_VERS)
            nvers = REC_MAX_VERS - pcomp->vers->lnAltCnt;

        pcomp->vers->lnAltCnt += nvers;

        if (pcomp->cs == 255)
            for (int i = 0; i < nvers; i++) {
                pcomp->vers->Alt[vers_beg + i].Code = evn_res[2 * i];
                pcomp->vers->Alt[vers_beg + i].Prob = evn_res[2 * i + 1];
                pcomp->vers->Alt[vers_beg + i].Method = 13;
            } // network collection
        else
            for (int i = 0; i < nvers; i++) {
                pcomp->vers->Alt[vers_beg + i].Code = evn_res[i];
                pcomp->vers->Alt[vers_beg + i].Prob = 255;
                pcomp->vers->Alt[vers_beg + i].Method = 5;
            } // event collection
    }
}

void RReccom::initAlphabet() {
    if (language_ >= LANG_TOTAL)
        throw RReccomException("Wrong language code: ", language_);
}

void RReccom::initData() {
    if (!isLanguage(language_))
        throw RReccomException("Language not exists", language_);

    if (chdir(ocr_path_.c_str()) != 0)
        throw RReccomException("Can not chdir to: " + ocr_path_);

    Alphabet * alph = AlphabetFactory::instance().make(language_);
    alph->exportToTable(alphabet_tbl_);
    delete alph;

    EVNSetAlphabet(alphabet_tbl_);
    loadDataTables();
}

bool RReccom::isLanguage(language_t language) {
    if (chdir(ocr_path_.c_str()) != 0)
        throw RReccomException("Can not chdir to: " + ocr_path_);

    if (language < LANG_ENGLISH || language >= LANG_TOTAL)
        return false;
    if (data_file_exists(tabevn1[language]) == -1)
        return false;
    if (data_file_exists(tabevn2[language]) == -1)
        return false;
    return true;
}

void RReccom::loadDataTables() {
    if (!EVNInitLanguage(tabevn1[language_], tabevn2[language_], language_))
        throw RReccomException("EVNInitLanguage failed");
}

void RReccom::recognize(Handle ccom, language_t language) {
    language_ = language;
    initData();

    CCOM_comp* pcomp = CCOM_GetFirst(ccom, NULL);
    while (pcomp) {
        eventRecognition(pcomp);
        pcomp = CCOM_GetNext(pcomp, NULL);
    }
}

void RReccom::setOcrPath(const std::string& path) {
    ocr_path_ = path;
}

}
