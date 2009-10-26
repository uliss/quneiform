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

#ifndef __RRECCOM_H
#define __RRECCOM_H

#include <memory>
#include <string>
#include "cttypes.h"
#include "common/exception.h"
#include "lang_def.h"

enum RRECCOMParametrs
{
    RRECCOM_OcrPath,
};

struct RRecComControl
{
    uint32_t flags;
#define RECOG_EVN 1 //опознание с помошью метода "Event"
#define RECOG_GRA 2 //опознание с помошью нейросети
    uint16_t MaxCompWid; // if comp width > MaxCompWid => ignored; 0 - not init
    uint16_t MaxCompHei; // =""= by height
    uint16_t MinCompWid; // if comp width <= MinCompWid => ignored; 0 - not init
    uint16_t MinCompHei; // =""= by width
    uint16_t MaxScale; // if scaling > => use long intervals
};

Bool32 RRECCOM_SetImportData(uint32_t dwType, const void * pData);
// 1    RRECCOM_FNRECOG   опознать компоненты
Bool32 RRECCOM_Recog(Handle hCCOM, RRecComControl Control, uchar lang);
// 8    RRECCOM_FNREX_ISLANGUAGE существует ли язык
Bool32 RRECCOM_IsLanguage(uchar language);

#define RRECCOM_ERR_MIN                2048
#define RRECCOM_ERR_NO                 2048
#define RRECCOM_ERR_NOTIMPLEMENT       2051
#define RRECCOM_ERR_NOINITEV           2058
#define RRECCOM_ERR_NOLANGUAGE         2062
#define RRECCOM_ERR_NOSETALPHABET      2063
#define RRECCOM_ERR_NOGRA              2064
#define RRECCOM_ERR_MAX                2068

namespace CIF {

class Alphabet;

class RReccom
{
public:
    RReccom();
    ~RReccom();

    void recognize(Handle ccom, language_t language);
public:
    static bool isLanguage(language_t);
private:
    void initData();
    void initAlphabet();
    void loadDataTables();
private:
    language_t language_;
    // if comp width > MaxCompWid => ignored; 0 - not init
    uint max_component_width_;
    // =""= by height
    uint max_component_height_;
    // if comp width <= MinCompWid => ignored; 0 - not init
    uint min_component_width_;
    // =""= by height
    uint min_component_height_;
    // if scaling > => use long intervals
    uint max_scale_;
    //
    std::auto_ptr<Alphabet> alphabet_;
    char alphabet_tbl_[256];
private:
    static std::string ocr_path_;
    static const char * alphabet_to_lang_[LANG_TOTAL];
};

typedef RuntimeExceptionImpl<RReccom> RReccomException;

}

#endif // __RRECCOM_H
