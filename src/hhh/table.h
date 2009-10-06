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

/**********  Title  **********/
/*  Author     :  Alexander Mikhailov                                        */
/*  Last Edit  :  02.09.98 (Almi)                                            */
/*  Source     :  'Table.H'                                                  */
/*  Means      :  Macros and Structures                                      */
/*  Aim        :  Description of Tables                                      */

#ifndef __TABLE_H__
#define __TABLE_H__
#pragma pack (push,8)
/*---------------------------------------------------------------------------*/
#include "cttypes.h"
/*---------------------------------------------------------------------------*/
#define    MaxTableUsers       5
typedef enum
{
		TABLE_USER_COMMON = 0,
		TABLE_USER_RLTABLE,
		TABLE_USER_RBLOCK,
		TABLE_USER_CPAGE,
		TABLE_USER_RBTABLE
} NAMES_TABLE_USER;
/*---------------------------------------------------------------------------*/
typedef struct tagTABLE_LINE_DESC
{
        int32_t   Level;
#define TLT_UnKnown       0x00 //- ничего не означает
#define TLT_Rough         0x01 //- правильная линия, не пытаться сократить
#define TLT_UnSure        0x02 //- ничего пока не означает
	char    Type;              // need types will be described
	char    reserv[3];
}TABLE_LINE_DESC;
typedef struct tagTABLE_LEVEL_DESC
{
	char    Type;              // need types will be described
	char    reserv[3];
}TABLE_LEVEL_DESC;
/*---------------------------------------------------------------------------*/
typedef enum
{
		TABLE_CELL_LEF = 0,  // Beg_X
		TABLE_CELL_RIG,      // End_X
		TABLE_CELL_TOP,      // Beg_Y
		TABLE_CELL_BOT       // End_Y
} SIDES_TABLE_CELL;
typedef struct tagTABLE_CELL_DESC
{
	char    TypeOwn;           // need types will be described
	char    TypeBound[4];      // это - избыточная информация
	char    reserv[3];
}TABLE_CELL_DESC;
/*---------------------------------------------------------------------------*/
typedef struct tagTABLE_SPECIAL_PROPERTIES
{//сейчас использует только 'RLTABLE'
//	char    reserv[16]; - было
	char    nSpecVertex;   // кол-во особых узлов
	char    Hori[5];       // номер табличной гориз.линии
	char    Vert[5];       // номер табличной гориз.линии
#define    NON_CELL_VERTEX          0x01
	char    Type[5];       // характер особенности узла
}TABLE_SPECIAL_PROPERTIES;
/*---------------------------------------------------------------------------*/
#define    MaxHorLines        99
#define    MaxVerLines        50
#define    MaxCells          (MaxHorLines-1)*(MaxVerLines-1)
typedef struct tagTABLE_DESC
{
	/*  Как искали таблицу  */
	Rect32  RectFieldSearch;
	int32_t   SkewFieldSearch;   // at what units?
	char    TaskTableSearch;   // need types will be described
	/*  Пользователи таблицы  */
		/*  Key[USER] - ключ пользователя "USER" от хранилища его связанных  */
	    /*  с этой таблицей данных. Если равно 0 - хранилище не заведено.    */
	uint32_t  Key[MaxTableUsers];
	/*  Общие свойства таблицы  */
	Rect32  RectTable;         // это - избыточная информация
	int32_t   SkewTable;         // at what units?
	char    LifeTable;         // need types will be described
	int     nHorLines;
	int     nVerLines;
	int     nCell;             // это - избыточная информация [(nH-1)*(nV-1)]
	/*  Свойства крупных структурных элементов таблицы  */
	TABLE_LINE_DESC    HorLine [MaxHorLines];
	TABLE_LINE_DESC    VerLine [MaxVerLines];
	TABLE_LEVEL_DESC   HorLevel[MaxHorLines-1];
	TABLE_LEVEL_DESC   VerLevel[MaxVerLines-1];
	/*  Свойства мелких структурных элементов таблицы  */
#define    CELL_BOUND_PSEVDO          0x01
	char    TypeHorBound[MaxHorLines*MaxVerLines];
	char    TypeVerBound[MaxHorLines*MaxVerLines];
	TABLE_CELL_DESC       Cell [MaxCells];
	/*  Прочее  */
	TABLE_SPECIAL_PROPERTIES   SpecProp;
	char    iSet; //номер сета, которому
#define TTD_None          0x00 //- никакой, в сете она одна
#define TTD_AsInImage     0x01 //- простая
#define TTD_ByLogic       0x02 //- средняя
#define TTD_Maximal       0x03 //- максимально подробная
        char    TypeTablDetail; // тип детализации
        char    Active; // 0 - пассивная, 1 - активная в сете
	char    reserv[253];
} TABLE_DESC;
/*---------------------------------------------------------------------------*/
#pragma pack (pop)
#endif
