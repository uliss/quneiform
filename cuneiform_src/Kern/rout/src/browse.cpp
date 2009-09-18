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


// Browse.cpp

//********************************************************************
//********************************************************************

#include <string.h>
#include "stdafx.h"
#include "rout_own.h"

// Вызов функции обхода
// Если функция=NULL, тогда обход все равно совершается,
// при этом подсчитывается количество объектов gHitxxx
// FALSE означает прекратить обход и вернуть FALSE.
#define BROWSE_FUNCTION(a,b) \
		{if (BrowseFunction && \
			!BrowseFunction(a,b)\
		)\
			return FALSE;}

//********************************************************************
Bool BrowsePage(
	FNROUT_BrowseFunction BrowseFunction, // Вызывать при обходе
	Bool wantSkipTableCells,	// Пропускать клетки таблиц
	Bool wantSkipParagraphs		// Пропускать абзацы
	)
{
/* Обход страницы.

   Для каждого объекта вызывается функция "BrowseFunction"
   Если функция "BrowseFunction" возвращает FALSE, то
   обход страницы прекращается и BrowsePage также возвращает FALSE.

   На обход страницы влияют флаги обхода:
		wantSkipTableCells и
		wantSkipParagraphs
*/
	long sections=0, columns=0, frames=0;
	long indexSection=0, indexColumn=0, indexFrame=0;

	// Текущие объекты на всех уровнях
	gSectionHandle = NULL;
	gColumnHandle = NULL;
	gFrameHandle = NULL;
	gTableHandle = NULL;
	gCellHandle = NULL;
	gParagraphHandle = NULL;
	gLineHandle = NULL;

	// Счетчики найденных объектов
	gHitSections = 0;
	gHitColumns = 0;
	gHitFrames = 0;
	gHitTables = 0;
	gHitCells = 0;
	gHitParagraphs = 0;
	gHitLines = 0;

	gHitChars = 0;
	gHitPictures = 0;

	// Загружена ли страница
	if ( !gPageHandle )
		{
		PAGE_NOT_LOADED;
		return FALSE;
		}

	// Начало страницы
	BROWSE_FUNCTION(gPageHandle,BROWSE_PAGE_START);

	// Разделы
	sections = CED_GetCountSection(gPageHandle);
	for (indexSection = 0; indexSection < sections; indexSection++)
		{
		gSectionHandle = CED_GetSection(gPageHandle,indexSection);

		// Начало раздела
		ASSERT(gSectionHandle);
		gHitSections++;
		BROWSE_FUNCTION(gSectionHandle,BROWSE_SECTION_START);

		// Колонки
		columns = CED_GetCountColumn(gSectionHandle);
		for (indexColumn = 0; indexColumn < columns; indexColumn++)
			{
			gColumnHandle = CED_GetColumn(gSectionHandle,indexColumn);

			// Начало колонки
			ASSERT(gColumnHandle);
			gHitColumns++;
			BROWSE_FUNCTION(gColumnHandle,BROWSE_COLUMN_START);

			// Обход абзацев, таблиц и фреймов внутри колонки
			if (!BrowseParagraphs(gColumnHandle,
							BrowseFunction,
							wantSkipTableCells,
							wantSkipParagraphs
				))
				return FALSE;

			// Конец колонки
			BROWSE_FUNCTION(gColumnHandle,BROWSE_COLUMN_END);
			gColumnHandle = NULL;
			}

		// Фреймы на странице отменены, теперь они принадлежат
		// колонке и обрабатываются в функции BrowseParagraphs()
		// -- 18.10.99 E.P.
/*
		// Фреймы
		frames = CED_GetCountFrame(gSectionHandle);
		for (indexFrame = 0; indexFrame < frames; indexFrame++)
			{
			gFrameHandle = CED_GetFrame(gSectionHandle,indexFrame);

			// Начало фрейма
			ASSERT(gFrameHandle);
			gHitFrames++;
			BROWSE_FUNCTION(gFrameHandle,BROWSE_FRAME_START);

			// Обход абзацев, таблиц и фреймов
			if (!BrowseParagraphs(gFrameHandle,
							BrowseFunction,
							wantSkipTableCells,
							wantSkipParagraphs
				))
				return FALSE;

			// Конец фрейма
			BROWSE_FUNCTION(gFrameHandle,BROWSE_FRAME_END);
			gFrameHandle = NULL;
			}
*/
		// Конец раздела
		BROWSE_FUNCTION(gSectionHandle,BROWSE_SECTION_END);
		gSectionHandle = NULL;
		}

	// Конец страницы
	BROWSE_FUNCTION(gPageHandle,BROWSE_PAGE_END);

	return TRUE;

}
//********************************************************************
Bool BrowseParagraphs(Handle hParentObject,
	FNROUT_BrowseFunction BrowseFunction, // Вызывать при обходе
	Bool wantSkipTableCells,	// Пропускать клетки таблиц
	Bool wantSkipParagraphs		// Пропускать абзацы
	)
{
/*
   Обход абзацев, таблиц и фреймов в заданном
   объекте hParentObject (колонке, фрейме, ячейке)

   Для каждого абзаца, картинки, таблицы вызывается
   функция "BrowseFunction", а если она возвращает FALSE, то
   обход прекращается, при этом BrowseParagraphs также
   возвращает FALSE.

   На обход влияют флаги обхода:
		wantSkipTableCells и
		wantSkipParagraphs
*/
	long objects=0, lines=0, chars=0;
	long indexObject=0, indexLine=0, indexChar=0;
	Handle hObject = NULL;

	gParagraphHandle = NULL;
	gLineHandle = NULL;

	ASSERT (hParentObject);

	// Цикл по всем объектам
	for (hObject = CED_GetFirstObject(hParentObject);
		 hObject;
		 hObject = CED_GetNextObject(hObject)
		 )
		{
		// Определить тип объекта
		if (CED_IsFrame(hObject))
			{
			// Фрейм
			gFrameHandle = hObject;

			// Начало фрейма
			gHitFrames++;
			BROWSE_FUNCTION(gFrameHandle,BROWSE_FRAME_START);

			// Обход абзацев и таблиц внутри фрейма
			if (!BrowseParagraphs(gFrameHandle,
							BrowseFunction,
							wantSkipTableCells,
							wantSkipParagraphs
				))
				return FALSE;

			// Конец фрейма
			BROWSE_FUNCTION(gFrameHandle,BROWSE_FRAME_END);
			gFrameHandle = NULL;

			}

		else if (CED_IsTable(hObject))
			{
			// Таблица
			if(gTableHandle)
				{
				DEBUG_PRINT("Skipping table inside table");
				continue;
				}

			gTableHandle = hObject;

			// Количество строк и столбцов таблицы
			EDSIZE dim = CED_GetSize(gTableHandle);
			gTableRows = dim.cy;
			gTableCols = dim.cx;

			// Количество логических ячеек
			gTableCells = CED_GetCountLogicalCell(gTableHandle);

			// Массив логических номеров ячеек
			gLogicalCells = (long*)CED_GetTableOfCells(gTableHandle);

			// Начало таблицы
			gHitTables++;
			BROWSE_FUNCTION(gTableHandle,BROWSE_TABLE_START);

			// Обход ячеек таблицы
			if (!wantSkipTableCells &&
				!BrowseCells(BrowseFunction)
				)
				return FALSE;

			// Конец таблицы
			BROWSE_FUNCTION(gTableHandle,BROWSE_TABLE_END);
			gTableHandle = NULL;
			gTableRows = 0;
			gTableCols = 0;
			gTableCells = 0;
			gLogicalCells = NULL;
			}

		else if (CED_IsParagraph(hObject))
			{
			// Пропускать абзацы?
			if (wantSkipParagraphs)
				continue;

			// Абзац
			gParagraphHandle = hObject;

			// Начало абзаца
			gHitParagraphs++;
			BROWSE_FUNCTION(gParagraphHandle, BROWSE_PARAGRAPH_START);

			// Обход строк в абзаце
			lines = CED_GetCountLine(gParagraphHandle);
			for (indexLine = 0; indexLine < lines; indexLine++)
				{
				gLineHandle = CED_GetLine(gParagraphHandle,indexLine);

				// Начало строки
				ASSERT(gLineHandle);
				gHitLines++;
			    gEdLineHardBreak = CED_GetLineHardBreak(gLineHandle);
				BROWSE_FUNCTION(gLineHandle,BROWSE_LINE_START);

				// Обход символов в строке
				chars = CED_GetCountChar(gLineHandle);
				for (indexChar = 0; indexChar < chars; indexChar++)
					{
					Handle charHandle = CED_GetChar(gLineHandle,
													indexChar);
					ASSERT(charHandle);

					if (CED_IsPicture(charHandle))
						{
						// Картинка
						// Прочесть описание картинки
						if (!PictureFromChar(charHandle))
							return FALSE;

						gHitPictures++;
						BROWSE_FUNCTION(charHandle, BROWSE_PICTURE);

						gPictureNumber = 0;
						gPictureData = 0;
						gPictureLength = 0;
						}
					else
						{
						// Обычный символ
						gHitChars++;

						// Скрытый ли символ
						gEdCharHidden =
								CED_GetCharFontAttribs(charHandle) &
								ED_HIDDEN;

						BROWSE_FUNCTION(charHandle,BROWSE_CHAR);
						}
					}

				// Конец строки
				BROWSE_FUNCTION(gLineHandle,BROWSE_LINE_END);
				gLineHandle = NULL;
				}

			// Конец абзаца
			BROWSE_FUNCTION(gParagraphHandle, BROWSE_PARAGRAPH_END);
			gParagraphHandle = NULL;
			}
		else
			{
			DEBUG_PRINT("Skipping fictive paragraph in ED");
			}
		}

	return TRUE;

}
//********************************************************************
Bool BrowseCells(FNROUT_BrowseFunction BrowseFunction)
{
/* Обход ячеек таблицы gTableHandle

   Для каждой ячейки вызывается функция "BrowseFunction"
   и кроме того, для обхода абзацев в ячейке
   вызывается функция BrowseParagraphs().

   Если одна из этих функций возвращает FALSE, то
   обход прекращается и BrowseCells также возвращает FALSE.
*/

	// Справочная обхода ячеек
	Byte sprav[10000];
	ASSERT(gTableCells < sizeof(sprav));
	memset(&sprav,0,sizeof(sprav));

	// Цикл по строкам таблицы
	for (gIndexTableRow = 0;
		gIndexTableRow < gTableRows;
		gIndexTableRow++
		)
		{
		// Начало строки таблицы
		gRowHandle = CED_GetTableRow(gTableHandle,gIndexTableRow);
		BROWSE_FUNCTION((Handle)gIndexTableRow, BROWSE_ROW_START);

		// Цикл по столбцам таблицы
		for (gIndexTableCol = 0;
			gIndexTableCol < gTableCols;
			gIndexTableCol++
			)
			{
			// Логический номер ячейки
			gIndexTableCell = gLogicalCells[
								gIndexTableRow * gTableCols +
								gIndexTableCol];

			// Проверить по справочной
			if( gIndexTableCell < 0 ||
				sprav[gIndexTableCell]
			  )
				{
				// В этой логической ячейке уже были
				gCellHandle = NULL;
				continue;
				}

			sprav[gIndexTableCell] = 1;

			// Начало ячейки
			gCellHandle = CED_GetLogicalCell(
							gTableHandle,
							gIndexTableCell);
			ASSERT(gCellHandle);

			gHitCells++;
			BROWSE_FUNCTION(gCellHandle, BROWSE_CELL_START);

			// Обход абзацев в ячейке
			if ( !BrowseParagraphs(gCellHandle,
							BrowseFunction,
							FALSE,	// wantSkipTableCells
							FALSE	// wantSkipParagraphs
				))
				return FALSE;

			// Конец ячейки
			BROWSE_FUNCTION(gCellHandle, BROWSE_CELL_END);
			gCellHandle = NULL;
			}

		// Конец строки таблицы
		BROWSE_FUNCTION((Handle)gIndexTableRow, BROWSE_ROW_END);
		gRowHandle = 0;
		}

	gIndexTableCell = 0;
	gIndexTableRow = 0;
	gIndexTableCol = 0;

	return TRUE;

}
//********************************************************************
Bool BrowseTable(FNROUT_BrowseFunction BrowseFunction)
{
// Обход таблицы.

	// Начало таблицы
	BROWSE_FUNCTION(gTableHandle,BROWSE_TABLE_START);

	// Обход ячеек таблицы
	if ( !BrowseCells(BrowseFunction) )
		return FALSE;

	// Конец таблицы
	BROWSE_FUNCTION(gTableHandle,BROWSE_TABLE_END);

	return TRUE;
}
//********************************************************************
