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

#ifndef __PUMADEF_H__
#define __PUMADEF_H__

#ifndef __NT__
	#define __NT__
#endif

#include "lang_def.h"
#include "dsnap.h"

// constants
#define PUMA_MODULE_CCOM	      1
#define PUMA_MODULE_CPAGE	      2
#define PUMA_MODULE_CFIO	      3
#define PUMA_MODULE_CSTR	      4
#define PUMA_MODULE_CIMAGE	      5
#define PUMA_MODULE_CLINE         6

#define PUMA_MODULE_REXC	    100
#define PUMA_MODULE_RLTABLE	    101
#define PUMA_MODULE_RBLOCK	    102
#define PUMA_MODULE_RSTR	    103
#define PUMA_MODULE_RLINE	    104
#define PUMA_MODULE_RFRMT	    105
#define PUMA_MODULE_RCOLOR	    106
#define PUMA_MODULE_RIMAGE	    107
#define PUMA_MODULE_RPSTR	    108
#define PUMA_MODULE_RPIC	    109
#define PUMA_MODULE_RANALFRM	110
#define PUMA_MODULE_CED		    111
#define PUMA_MODULE_ROUT	    112
#define PUMA_MODULE_RAGR        113
//Allex
#define PUMA_MODULE_RSTUFF      114
#define PUMA_MODULE_RVERLINE    115
//Alik
#define PUMA_MODULE_RSL         116
#define PUMA_MODULE_SLINEDOT    117
//Andrey
#define PUMA_MODULE_RNORM       118
#define PUMA_MODULE_RRECCOM     119
//Tanya
#define PUMA_MODULE_RMSEGMENT   120
#define PUMA_MODULE_SSTREAK     121
//Andrey
#define PUMA_MODULE_RCORRKEGL     122
//allex
/////////////////////////////////////////////////////////////////////////////////////////////////////
// Allex (перенесено из mpuma.h
#define NAME_IMAGE_INPUT		"Исходное изображение"
#define NAME_IMAGE_BINARIZE     "Бинаризованное изображение"
#define NAME_IMAGE_DELLINE		"Изображение со снятыми линиями"
#define NAME_IMAGE_ORTOMOVE     "Изображение после ортосдвига"
/////////////////////////////////////////////////////////////////////////////////////////////////////
// allex
#define PUMA_IMAGE_USER		"ImageFromUser"
#define PUMA_IMAGE_BINARIZE "ImageBinarize"
#define PUMA_IMAGE_DELLINE	"ImageAfterDeleteLine"
#define PUMA_IMAGE_ROTATE	"ImageAfterRotate"
#define PUMA_IMAGE_TURN		"Изображение после разворота"
#define PUMA_IMAGE_ORTOMOVE "After OrtoMove"
#endif
