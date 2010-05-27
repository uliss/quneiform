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

// ============================================================================
// Written by Shahverdiev  Alik
// This file cration date: 10.18.98
//
// Frmt.cpp
//
// ============================================================================

#include <stdlib.h>
#include <stdio.h>
#include "resource.h"
#include "rfrmt.h"
#include "mymem.h"
#include "rtffragrect.h"
#include "dpuma.h"
#include "aldebug.h"
#include "rfrmt_prot.h"
#include "rfrmtoptions.h"
#include <cstring>
#include <cassert>
#include <vector>
#include <climits>

#include "compat/filefunc.h"
#include "font.h"

uint32_t ExFlagMode;
char RtfFileName[PATH_MAX];
char WriteRtfImageName[PATH_MAX];
char lpMyNameSerif[PATH_MAX];
char lpMyNameNonSerif[PATH_MAX];
char lpMyNameMono[PATH_MAX];

extern Bool32 gbBold;
extern Bool32 gbItalic;
extern Bool32 gbSize;
extern uint32_t gnFormat;
extern const char* gpSerifName;
extern const char* gpSansSerifName;
extern const char* gpCourierName;
extern uint32_t CountTable;

// extern functions
void SetReturnCode_rfrmt(uint16_t rc);

extern Handle hDebugMy;
extern Handle hDebugKegl;
extern Handle hDebugFrame;
extern Handle hDebugLineTransfer;
extern Handle hDebugAlign;

bool RFRMT_Formatter(const char* lpInputImageName, CIF::CEDPage** page) {
    FILE *fpInternalFile = create_temp_file();

    if (fpInternalFile == NULL) {
        assert ("Could not create tmpfile\n");
    }

    CIF::RfrmtOptions::setFormatMode(0);
    ExFlagMode = FALSE;
    strcpy((char*) WriteRtfImageName, lpInputImageName);
    SetReturnCode_rfrmt(IDS_ERR_NO);

    if (CreateInternalFileForFormatter(fpInternalFile) == FALSE) {
        fclose(fpInternalFile);
        return FALSE;
    }

    if (!gbBold)
        CIF::RfrmtOptions::setFlag(CIF::NOBOLD);

    if (!gbItalic)
        CIF::RfrmtOptions::setFlag(CIF::NOCURSIV);

    if (!gbSize)
        CIF::RfrmtOptions::setFlag(CIF::NOSIZE);

    if (!LDPUMA_Skip(hDebugFrame))
        CIF::RfrmtOptions::setFlag(CIF::USE_FRAME);
    else if (gnFormat == 1 && ExFlagMode == FALSE)
        CIF::RfrmtOptions::setFlag(CIF::USE_FRAME_AND_COLUMN);
    else
        CIF::RfrmtOptions::setFlag(CIF::USE_NONE);

    strcpy((char*) lpMyNameSerif, gpSerifName);
    strcpy((char*) lpMyNameNonSerif, gpSansSerifName);
    strcpy((char*) lpMyNameMono, gpCourierName);

    if (!LDPUMA_Skip(hDebugLineTransfer))
        CIF::RfrmtOptions::setLineTransfer(true);
    else
        CIF::RfrmtOptions::setLineTransfer(false);

    if (!FullRtf(fpInternalFile, NULL, page)) {
        fclose(fpInternalFile);
        return FALSE;
    }

    if (fclose(fpInternalFile) != 0) {
        char ch[500];
        sprintf(ch, "File %s cannot be closed\n", fpInternalFile);
        assert (ch);
    }

    return TRUE;
}

