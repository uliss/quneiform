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
/**********  Заголовок  *******************************************************/
/*  Автор      : Глеб Корольков                                                             */
/*  комментарии                                                               */
/*  и далнейшая                                                               */
/*  правка     :                                                            */
/*  Редакция   :  16.10.2000                                                  */
/*  Файл       :  'RMARKER.H'                                                 */
/*  Содержание :  Интерфейс библиотеки                                        */
/*  Назначение :                                                              */
/*----------------------------------------------------------------------------*/
#ifndef __RMARKER_H
#define __RMARKER_H

#include <string>
#include "common/lang_def.h"
#include "cttypes.h"
#include "globus.h"

namespace cf
{
class CLA_EXPO RMarker
{
    public:
        RMarker();
        ~RMarker();

        void markupPage();
        void setCCom(Handle ccom);
        void setCLine(Handle cline);
        void setCPage(Handle cpage);
        void setLanguage(language_t language);
        void setPicturesNum(uint num);
        void setFax(bool value);
        void setOneColumnLayout(bool value);
        void setKillVslComponents(bool value);
        void setLayoutFilename(const std::string& fname);
    private:
        Handle cpage_;
        Handle ccom_;
        Handle cline_;
        language_t language_;
        uint pictures_;
        bool fax_;
        bool one_column_;
        bool kill_vsl_components_;
        std::string layout_file_name_;
};

}

struct CCOM_cont;

struct RMPreProcessImage
{
        Bool32 gbFax100;
        Bool32 gbOneColumn;
        Bool32 gKillVSLComponents;
        uint32_t gnLanguage;
        Handle hCPAGE;
        CCOM_cont * hCCOM;
        Handle hCLINE;
        Handle hDebugCancelSearchPictures;
        Handle hDebugLayoutFromFile;
        Handle hDebugCancelExtractBlocks;
        Handle hDebugSVLines;
        Handle hDebugSVLinesStep;
        Handle hDebugSVLinesData;
        const char *szLayoutFileName;
        uint32_t gnPictures;
};

typedef RMPreProcessImage * PRMPreProcessImage;

enum
{
    PUMA_SVL_FIRST_STEP = 0x1,
    PUMA_SVL_SECOND_STEP = 0x2,
    PUMA_SVL_THRID_STEP = 0x3
};

const int PUMAMaxNumLines = 2000;
FUN_EXPO__ Bool32 RMARKER_PageMarkup(PRMPreProcessImage, void*, int, void*, int);

#endif
