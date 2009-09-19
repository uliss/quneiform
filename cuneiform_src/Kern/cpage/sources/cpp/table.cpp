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

#include "globus.h"
#include "cpage.h"
#include "backup.h"
#include "polyblock.h"
#include "resource.h"

// Конверторы преобразования из TYPE_DESC в CPAGE_TABLE
uint32_t TYPE_DESK_to_CPAGE_TABLE(TABLE_DESC * lpDataIn,uint32_t SizeIn,CPAGE_TABLE * lpDataOut,uint32_t SizeOut)
{
	uint32_t rc = 0;
	if(lpDataOut == NULL)
		return sizeof(CPAGE_TABLE);

	if(sizeof(TABLE_DESC) != SizeIn ||
		sizeof(CPAGE_TABLE) != SizeOut ||
		lpDataIn == NULL)
	{
		SetReturnCode_cpage(IDS_ERR_DISCREP);
		return 0;
	}

    memset(lpDataOut,0,SizeOut);

	lpDataOut->prop = *lpDataIn;
	// заполним структуру описания ячеек таблицы
	if(lpDataIn->nHorLines>0 &&
		lpDataIn->nHorLines>0)
	{
		int c_current = 0;
		Point32 p = {0,0};

		for(int c_row = 0; c_row < lpDataIn->nHorLines - 1; c_row++)
			for( int c_col = 0;c_col < lpDataIn->nVerLines - 1; c_col++)
			{
                _ASSERT((c_row*(lpDataIn->nVerLines - 1)+c_col)<MaxCells);
				if(c_row && lpDataIn->Cell[c_row*(lpDataIn->nVerLines - 1)+c_col].TypeBound[TABLE_CELL_TOP] & CELL_BOUND_PSEVDO)
				{// нет горизонтального разделителя сверху
					p = lpDataOut->cell[c_row - 1][c_col].PhCoord;
					c_current = lpDataOut->cell[c_row - 1][c_col].Number;
				}
				else if(!c_col || !(lpDataIn->Cell[c_row*(lpDataIn->nVerLines - 1) + c_col].TypeBound[TABLE_CELL_LEF] & CELL_BOUND_PSEVDO))
				{// есть вертикальный разделитель слева
					c_current = ++lpDataOut->PhNumber;
					p.x = c_col;
					p.y = c_row;
				}
				else
				{// есть гор. разделитель сверху и нет верт. справа
					p = lpDataOut->cell[c_row][c_col - 1].PhCoord;
					c_current = lpDataOut->cell[c_row][c_col - 1].Number;
				}

				lpDataOut->cell[c_row][c_col].Number = c_current;
				lpDataOut->cell[c_row][c_col].PhCoord = p;
				lpDataOut->cell[c_row][c_col].Block = -1;
				lpDataOut->cell[p.y][p.x].GeCount++;
			}
			rc = sizeof(*lpDataOut);
	}
	else
		_ASSERT(FALSE);

	return rc;
}
//##########################################################
uint32_t CPAGE_TABLE_to_TYPE_DESK( CPAGE_TABLE * lpDataIn,uint32_t SizeIn,TABLE_DESC * LpDataOut,uint32_t SizeOut)
{
	uint32_t rc = 0;

	if(LpDataOut == NULL)
			return sizeof(TABLE_DESC);

	if(sizeof(TABLE_DESC) != SizeOut ||
		sizeof(CPAGE_TABLE) != SizeIn ||
		lpDataIn == NULL)
	{
		SetReturnCode_cpage(IDS_ERR_DISCREP);
		return 0;
	}

	*LpDataOut = lpDataIn->prop;
	rc = sizeof(TABLE_DESC);
	return rc;
}

