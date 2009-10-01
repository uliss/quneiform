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

#ifndef __CPAGETYPS_H__
#define __CPAGETYPS_H__

#include "table.h"
#include "polyblock.h"

#pragma pack (push,8)

// PAGES
#define PT_EMPTY    CPAGE_GetInternalType("__Page__")
#define PT_PAGEINFO CPAGE_GetInternalType("__PageInfo__")

typedef struct {
	char	szImageName[CPAGE_MAXNAME];// имя блока распознаваемого изображения в контейнере RIMAGE
	uint32_t	Page; // номер страницы
	uint32_t	DPIX;
	uint32_t	DPIY;
	uint32_t	BitPerPixel;
	int32_t	X; // смещение относительно глобальных координат
	int32_t	Y;
	uint32_t	Width; // размеры в пиксеалх
	uint32_t	Height;
	int32_t	Incline2048; // наклон страницы в долях от 2048
	uint32_t  Angle;// угол поворота страницы 0, 90, 180, 270
	int32_t   SkewLocVerLin2048;//вертикальное отклонение в долях 2048
#define IMAGE_USER		1
#define IMAGE_BINARIZE  2
#define IMAGE_DELLINE	4
#define IMAGE_ROTATE	8
#define IMAGE_ORTOMOVE  16
#define IMAGE_TURN		32
	uint16_t   Images;//существующие изображения
	uint32_t	status;
#define PINFO_USERTEMPLATE	1;	// темплейт установлен пользователем
#define PINFO_AUTOTEMPLATE	2;	// темплейт установлен автоматически
} PAGEINFO;
typedef PAGEINFO * LPPAGEINFO;

#define CreateEmptyPage() CPAGE_CreatePage(PT_EMPTY,NULL,0)
#define CreatePageWithPageInfo(a) CPAGE_CreatePage(PT_PAGEINFO,&a,sizeof(a))
#define GetPageInfo(a,b) CPAGE_GetPageData(a,PT_PAGEINFO,b,sizeof(*b))
#define SetPageInfo(a,b) CPAGE_SetPageData(a,PT_PAGEINFO,&b,sizeof(b))


// BLOCKS

#define	BT_EMPTY CPAGE_GetInternalType("__Block__")
#define	BT_TABLE CPAGE_GetInternalType("__Table__")

#define CreateEmptyBlock(a) CPAGE_CreateBlock(a,BT_EMPTY,0,0,NULL,0)
#define CreateBlockNumber(a,b) CPAGE_CreateBlock(a,BT_EMPTY,b,0,NULL,0)

#define TYPE_CPAGE_TABLE	CPAGE_GetInternalType("TYPE_CPAGE_TABLE")
#define TYPE_CPAGE_PICTURE	CPAGE_GetInternalType("TYPE_CPAGE_PICTURE")

typedef struct tagCellInfo
{
	int32_t	Number; // Номер ячейки физической таблицы ( начиная с 1 )
	Point32 PhCoord;// Координаты привязки к физической ячейке
	int32_t   Block;  // Номер фрагмента
	int32_t	GeCount;// число геометрических ячеек, входящих в физическую
	uint32_t	wFlags[16];// флажки
	int32_t   reserv[48];

} CellInfo;

typedef struct tagCPAGE_TABLE
{
	TABLE_DESC	prop; // основная информация о таблице
	CellInfo	cell[MaxHorLines-1][MaxVerLines-1];// номер соответсвующего
	uint32_t      PhNumber; // число физических ячеек
	uint32_t		wFlags[16];// флажки


} CPAGE_TABLE;

#define CPAGE_MAXCORNER  1000

typedef struct tagCPAGE_PICTURE
{
	uint16_t  Number;
	Point32 Corner[CPAGE_MAXCORNER];

} CPAGE_PICTURE;

#pragma pack (pop)
#endif
