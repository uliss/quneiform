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

#ifndef __POLYBLOCK_H__
#define __POLYBLOCK_H__
#pragma pack (push,8)

#include "table.h"

# define  TYPE_TEXT			CPAGE_GetInternalType("TYPE_TEXT")
# define  TYPE_IMAGE		CPAGE_GetInternalType("TYPE_IMAGE")
# define  TYPE_TABLE		CPAGE_GetInternalType("TYPE_TABLE")
# define  TYPE_EMPTY		CPAGE_GetInternalType("TYPE_EMPTY")
# define  TYPE_DESC			CPAGE_GetInternalType("TYPE_DESC")
# define  TYPE_DESK			TYPE_DESC
# define  TYPE_PICTURE      CPAGE_GetInternalType("TYPE_PICTURE")
# define  TYPE_SCROLL       CPAGE_GetInternalType("TYPE_SCROLL")
# define  TYPE_LINE         CPAGE_GetInternalType("TYPE_LINE")
# define  MaxNum			1000

//константы аттрибутов фрагментов (к текстовому фрагменту и к ячейке таблицы) типа: негатив - позитив для представлени
//01.01.01 Логинов
# define TYPE_NEGATIVE 1
# define TYPE_POSITIVE 0

//константы ориентации содержимого фрагментов (к текстовому фрагменту и к ячейке таблицы)
//01.01.01 Логинов
# define TYPE_LEFTRIGHT 0
# define TYPE_UPDOWN    1
# define TYPE_DOWNUP    2


//Значения нижеследующих констант нельзя менять. Они используются для побитового сравнени
# define VISIBLE_LINE      0
# define HIDE_LINE         1
# define GRAYED_LINE       2
# define DOTTED_LINE       4
# define DASHED_LINE       8
# define DOUBLED_LINE      16
# define NORMAL_LINE       0	// дублирует



typedef struct tagStructBool16
{
//////////////////////////////////////////
#define    BIT_0                      0x01
#define    BIT_1                      0x02
#define    BIT_2                      0x04
#define    BIT_3                      0x08
#define    BIT_4                      0x10
#define    BIT_5                      0x20
#define    BIT_6                      0x40
#define    BIT_7                      0x80
//////////////////////////////////////////
	char Type;
	char Visible;
} StructBool16;

typedef struct tagVertex
{
	int32_t x;
	int32_t y;
	int32_t mark;
} VERTEX;

typedef struct tagCommon
{
	Handle type;//Текст, Картинка, Таблица;
	int16_t number;//порядковый номер
	int16_t Color;
	Bool Select;//
	int16_t count;
	Point32 Vertex[MaxNum];
// Almi 18.04.00
#define POS_NEGTABCAP   0x00000001 // Вероятный негативный заголовок таблицы
#define POS_NEGTXTCAP   0x00000002 // Вероятный негативный заголовок текста
#define POS_MATCH       0x00000004 // Используется в программе сравнения фрагментации
	uint32_t Flags;
} COMMON;

typedef struct tagPOLY
{
	COMMON com;

	int32_t mark[MaxNum];

	int32_t alphabet;//Цифры,Цифры и буквы, Буквы
	Bool16 negative;//Негатив = TYPE_NEGATIVE, Позитив = TYPE_POSITIVE;//     01.01.01 Логинов
	int16_t orient;//TYPE_NORD- Сверху вниз (нормальное), TYPE_WEST- лежит на левом боку, TYPE_OST- лежит на правом боку.
} POLY_;

typedef struct tagTABLE
{
	COMMON com;

	int32_t num_colons;//число колонок
	int32_t num_rows;//число строк
	int32_t LineY[MaxHorLines/*MaxHorLines*/-1];//координаты линий (нулевая совпадает с первой линией, верхняя крышка таблицы не участвует)
	int32_t LineX[MaxVerLines-1];//координаты колонок (нулевая совпадает с первой левой колонкой)
	Bool16 Visible[MaxHorLines][MaxVerLines][2];//видима-невидима плюс флаги: сплошная, пунктирная, штрих
	//StructBool16 Visible[MaxHorLines][MaxVerLines][2];//видима-невидима плюс флаги: сплошная, пунктирная, штрих
	Bool16 TypeCell[MaxHorLines][MaxVerLines];//тип ячейки
	int32_t Skew;
	Bool16 Negative[MaxHorLines][MaxVerLines];//Негатив = 1, Позитив = 0;//     01.01.01 Логинов
	int16_t Orient[MaxHorLines][MaxVerLines];//TYPE_UPDOWN- Сверху вниз и т.д. см константы выше

	char    iSet; //номер сета, которому
	char    TypeTablDetail;
	char    Active; // 0 - пассивная, 1 - активная в сете
	char    reserv[3];
	//int16_t SetNum;//Нумерация в сете
	//Bool16 bActive;//для маркировки активной таблицы в сете
	//int16_t GlobNum;//Внутренняя нумерация

} TABLE_;
/*
typedef struct tagLINE
{
	COMMON com;

	int32_t width;//Ширина

	int32_t visi;//Внешний вид: Простая,Невидимая,Двойная,Пунктир.

} LINE_;
*/
#pragma pack (pop)
#endif
