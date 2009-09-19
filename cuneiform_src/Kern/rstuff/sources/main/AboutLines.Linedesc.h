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

/**********  Заголовок  **********/
/*  Author     :  Alexander Mikhailov                                        */
/*  Last Edit  :  09.12.98                                                   */
/*  Source     :  'LineDesc.H'                                               */
/*  Содержание :  Структуры и макросы разделителей.                          */
/*  Назначение :  Описание главных (для алгоритмов библиотеки) типов данных. */
/*---------------------------------------------------------------------------*/
#include "cttypes.h"
/*---------------------------------------------------------------------------*/
typedef enum
{// четыре сектора направления линий
	RLT_LDIR_Horiz = 0,
	RLT_LDIR_Verti,
	RLT_LDIR_Kosa1,
	RLT_LDIR_Kosa2
}RLT_LINEDIR_TYPE;

typedef struct tagRLT_LINEATDIR
{// в данном направлении - по индексу "RLT_LINEDIR_TYPE"
	int   nLine;           // всего линий
	int   nLongLine;       // всего длинных линий
	int   nLongNoAtPool;   // всего длинных линий, противоречащих пулу
	int   nShortContrPool; // всего коротких линий, противоречащих пулу
	int   SkewPool;        // вычисленный угол пула
	int   WeightSkew;      // голоса за такое решение по углу (в пикселях)
	int   LightSkew;       // убедительность голосов (в pro2miles)
}RLT_LINEATDIR;

typedef struct tagRLT_LINE
{
	/*  primary values  */
	Point16  Beg;
	Point16  End;
	int      Wid;
	int16_t    nSeg;
	uchar    Plot;
	/*  derivative values  */
	int      Len;
	int      Skew;
	int      Skew2Delta;  // ширина сектора возможных колебаний угла
	/*  decision about line  */
#define    RLT_LT_Initial      0x00
#define    RLT_LT_Vertic       0x01
#define    RLT_LT_Kosaya       0x02
	/***   V & !K : вертикальная    :  90' - 22.5' < Skew <  90' + 22.5'  ***/
	/***  !V & !K : горизонтальная  :   0' - 22.5' < Skew <   0' + 22.5'  ***/
	/***   V &  K : косая-1квадрант :  45' - 22.5' < Skew <  45' + 22.5'  ***/
	/***  !V &  K : косая-2квадрант : 135' - 22.5' < Skew < 135' + 22.5'  ***/
#define    RLT_LT_SkewConflict 0x04
	/***   противоречит по углу пулу "родной" ориентации  ***/
#define    RLT_LT_Long         0x08
	/***   длинная линия  ***/
#define    RLT_LT_Line         0x10
	/***   линия  ***/
#define    RLT_LT_Ssel         0x20
	/***   белая полоса  ***/
#define    RLT_LT_Pointed      0x40
	/***   отточия  ***/
#define    RLT_LT_Doubt        0x80
	/***   линия только отчасти правильная  ***/
	char   Type;
}RLT_LINE;

typedef struct tagRLT_LINEPOOL
{
	RLT_LINEATDIR  Pool[4];
	int      nLine;
	RLT_LINE  *pLine;
}RLT_LINEPOOL;
/*---------------------------------------------------------------------------*/
