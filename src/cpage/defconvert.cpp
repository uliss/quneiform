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

#include "polyblock.h"
#include "convert.h"
#include "picture.h"
#include "internal.h"
#include "cpagetyps.h"
#include "cpage.h"
#include "cpage_debug.h"

static CDataType varTYPE_CPAGE_TABLE = 0;
static CDataType varTYPE_CPAGE_PICTURE = 0;

void DefConvertInit()
{
    varTYPE_CPAGE_TABLE = TYPE_CPAGE_TABLE;
    varTYPE_CPAGE_PICTURE = TYPE_CPAGE_PICTURE;
}
#undef TYPE_CPAGE_TABLE
#undef TYPE_CPAGE_PICTURE

using namespace cf;

uint32_t DefConvertBlock(uint32_t /*context*/,
                         CDataType typeIn, const void * dataIn, uint32_t sizeIn,
                         CDataType typeOut, void * dataOut, uint32_t sizeOut)
{
    if(!dataIn) {
        CPAGE_ERROR_FUNC << "NULL input given";
        return 0;
    }

    if(typeIn == TYPE_DESK && typeOut == varTYPE_CPAGE_TABLE)
        return TYPE_DESK_to_CPAGE_TABLE((TABLE_DESC *)dataIn, sizeIn, (CPAGE_TABLE *)dataOut, sizeOut);

    if(typeIn == varTYPE_CPAGE_TABLE && typeOut == TYPE_DESK)
        return CPAGE_TABLE_to_TYPE_DESK((CPAGE_TABLE *)dataIn, sizeIn, (TABLE_DESC *)dataOut, sizeOut);

    if(typeIn == TYPE_IMAGE && typeOut == varTYPE_CPAGE_PICTURE)
        return cpage::polyBlockToPicture(dataIn, sizeIn, dataOut, sizeOut);

    if(typeIn == varTYPE_CPAGE_PICTURE && typeOut == TYPE_IMAGE)
        return cpage::pictureToPolyBlock(dataIn, sizeIn, dataOut, sizeOut);

    CPAGE_ERROR_FUNC << "unsupported conversion:"
                     << CPAGE_GetNameInternalType(typeIn)
                     << "=>"
                     << CPAGE_GetNameInternalType(typeOut);
    return 0;
}

uint32_t DefConvertPage(uint32_t /*dwContext*/,
                        CDataType /*TypeIn*/, const void * /*lpDataIn*/, uint32_t /*SizeIn*/,
                        CDataType /*TypeOut*/, void * /*LpDataOut*/, uint32_t /*SizeOut*/)
{
    return 0;
}
