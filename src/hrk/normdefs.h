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
/*  Автор      :  Александр Михайлов                                          */
/*  Редакция   :  08.06.00                                                    */
/*  Файл       :  'NormDefs.H'                                                */
/*  Содержание :  Структуры и типы геометрических объектов.                   */
/*  Назначение :  Нормализация изображения.                                   */
/*----------------------------------------------------------------------------*/
#ifndef __NORMDEFS_H
#define __NORMDEFS_H
#include "cttypes.h"

typedef enum
{
	NR_DT_Unknown = 0,
	NR_DT_Rect,
	NR_DT_LineSimple,
	NR_DT_LineDefis,
	NR_DT_LinePointed,
	NR_DT_Frame,
	NR_DT_EvenHori,
	NR_DT_EvenVert,
	NR_DT_PoolInteHori,
	NR_DT_PoolInteVert,
	NR_DT_CompHori,
	NR_DT_CompVert
} NORM_DATATYPES;

typedef struct tagNR_Rect
{
	int32_t  Lef;
	int32_t  Top;
	int32_t  Rig;
	int32_t  Bot;
	int32_t  Skew10; // == 10*1024*Tg(fi)
} NR_Rect;

typedef struct tagNR_SimpLine
{
	int32_t  Beg_X;
	int32_t  Beg_Y;
	int32_t  End_X;
	int32_t  End_Y;
	int32_t  Wid10; // == 10*width
} NR_SimpLine;

typedef struct tagNR_DefiLine
{
	NR_Rect Rc_0;
	int32_t  Step1000; // == 1000*step
	int32_t  nRc;
} NR_DefiLine;

typedef struct tagNR_PoinLine
{
	int32_t  Lef_0;
	int32_t  Top_0;
	int32_t  Size;
	int32_t  Step1000; // == 1000*step
	int32_t  nRc;
} NR_PoinLine;

typedef struct tagNR_Frame
{
	NR_Rect Rc;
	int32_t  StepInto;
} NR_Frame;

typedef struct tagNR_Shift
{
	int32_t  Pos;
	int32_t  Lent;
} NR_Shift;

typedef struct tagNR_Event
{
	Bool   Hori;      //гориз. или вертик.
	Bool   Increase;  //наращивать или уменьшать уровень
	int32_t  Lev_0;     //уровень первого интервала
	int    nInter;
	NR_Shift *pInter; //отступы и протяженности интервалов
} NR_Event;

typedef struct tagNR_Inter
{
	int32_t  Lev;
	int32_t  Pos;
	int32_t  Lent;
} NR_Inter;

typedef struct tagNR_Bag
{
	Bool   Hori;      //гориз. или вертик.
	int    nInter;
	NR_Inter *pInter;
} NR_Bag;

#endif
