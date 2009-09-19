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

#include "linedefs.h"

#define CLINE_HorizantalLine   1
#define CLINE_VerticalLine     0

#define CLINE_HalfOtstup       4
#define CLINE_Otstup           8

#define CLINE_MaxVerticsNumber        4

typedef void* CLINE_handle;

typedef union {
	struct {
		NR_PoinLine data;
		int32_t res[2];
	} point_line;
	struct {
		NR_DefiLine data;
	} defi_line;
} SPECLINE;

typedef struct SL_POLOY_struct {
	int16_t count;
	Point16 Vertex[CLINE_MaxVerticsNumber];
} CLINE_SL_POLY;

typedef struct Interval_Data_struct {
	int32_t Pos;
	int32_t Lent;
} DInterval;

typedef const DInterval* CPDInterval;

typedef struct Event_Data_struct {
	Bool32 Hori; //гориз.(1) или вертик.(0)
	Bool32 Increase; //наращивать или уменьшать уровень
	int32_t Lev_0; //уровень первого интервала
	int Width;
	int32_t EventLength;
} DEvent;

typedef const DEvent* CPDEvent;

#define DirectedUpwards       0  //defines for Direction
#define DirecteDownwards      1
#define DirectedForBothSides  2

typedef struct CutPoint_Data_struct {
	Bool32 Direction;
	int32_t Level; //уровень полосы
	uchar reserv;
} DCutPoint;

typedef const DCutPoint* CPDCutPoint;

#define  CLINE_InitialPointHasCrossPoint  1  //defines for Component
#define  CLINE_MiddleHasCrossPoint        2
#define  CLINE_EndPointHasCrossPoint      4

typedef struct Component_Data_struct {
	Bool32 CrossPointFlag;
	int32_t BeginPoint;
	int32_t EndPoint;
} DComponent;

typedef const DComponent* CPDComponent;

typedef struct Lns_Data_struct {
	Point16 Anew;
	Point16 Bnew;
} DLns;

typedef const DLns* CPDLns;

typedef struct Line_Data_struct {
	uint32_t Flags; // common use info
	uint32_t Tail; // остаток линии за таблицой
	Rect32 rect; // прямоугольник линии
	Bool32 ProcessingType; // тип обработки линии(гор. или верт.)
	int32_t LineEventsLength; // сумма черных точек линии
	double Degree;
	Handle RelationshipIndex; // индекс коробки компонент
	Bool32 FlagCalculatedWidth;
	Bool32 FlagExtensible;
	Bool32 FlagDot; // устанавливается когда линия точечная
	Bool32 FlagBad;
	Bool32 FlagLongStrip;

	NR_SimpLine Line; // простейшее описание линии
	int32_t Status; // история линии в программе
	NORM_DATATYPES Type; // *Unknown, *Simple, *Defis, *Pointed
	LINE_DIR Dir;
	char Qual; // quality (or estimation) at percents
	char Dens; // density at percents
	SPECLINE Specline; // (NR_DefiLine), (NR_PoinLine)
	CLINE_SL_POLY poly;
	int32_t n_event; //число ивентов
	DLns lns_data;
} DLine;

typedef const DLine* CPDLine;

