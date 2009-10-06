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


// TableText.cpp

//********************************************************************
//********************************************************************

#include <string.h>
#include "stdafx.h"
#include "rout_own.h"

static Bool Static_TableText_Prepare(Handle hObject, long reason);
static Bool LineEnd();
static Bool TableText_Columns();
static Bool TableText_CellWidths();
static long BottomRow();
static long RightmostCol();
static Bool SearchTable(long fromCol);
static Bool CellRight();
static Bool FirstCell();
static Bool NextLevel();
static Bool TableText_Finish(long reason);
static Bool EmptyColumns(long fromCol,long toCol);
static Bool AfterColumn(long col);

//********************************************************************
Bool MakeTableText()
{
/*
	Формат TableText.

	Обрабатывается одна таблица.
	Концы строк сохраняются, если gPreserveLineBreaks = TRUE.

    Сначала формируется построчное представление таблицы на правой
	половине памяти при помощи функции TableText_Prepare().

	Затем дополнительно рассчитываются координаты колонок текста
	при помощи функции TableText_Columns().

	Для финальной сборки табличного текста используется
	левая половина памяти.

*/

	// Выложить таблицу построчно
	if ( !TableText_Prepare() )
		return FALSE;

	// Рассчитать координаты колонок таблицы
	// с учетом интервалов и разделителей
	if ( !TableText_Columns() )
		return FALSE;

	// Рассчитать ширины ячеек с учетом охваченных колонок
	if ( !TableText_CellWidths() )
		return FALSE;

	// Сохранить конец памяти
	Byte *saveMemEnd = gMemEnd;

	// Конец оставшейся свободной памяти,
	// предусмотреть бампер на конце
	gMemEnd = (Byte*)gBegLineText - gBumperSize;

	// Текущий указатель в памяти
	gMemCur = gMemStart;

	// Финальная сборка табличного текста
	Bool good = BrowseTableText(TableText_Finish);

	// Восстановить конец памяти
	gMemEnd = saveMemEnd;

	// Удалить построчное представление
	TableText_Delete();

	return good;
}
//********************************************************************
Bool TableText_Prepare()
{
/*
	Выложить таблицу построчно.
	Применяется для всех табличных форматов.

	Обрабатывается одна таблица.
	Концы строк сохраняются, если gPreserveLineBreaks = TRUE.

	Массивы размещаются на правой половине памяти.
	Первым размещается массив дескрипторов строк текста
	gBegLineText, он и служит концом доступной памяти
	для последуюших действий.

	В процессе выкладывания текстового представления таблицы
	рассчитываются некоторые характеристики строк и колонок
	таблицы.

	Для строк таблицы вычисляются:
		gTableRowTops - массив верхних координат строк таблицы
		gTableRowBottoms - массив нижних координат строк таблицы

	Оба массива измеряются количеством текстовых строк от верха
	таблицы.

		Замечание 1: предполагается, что "лишних" строк и колонок
		в таблице нет, то есть:

			1) каждая строка таблицы является верхней строкой
			   для некоторой логической ячейки и нижней строкой для
			   некоторой (быть может отличной) логической ячейки.

			2) каждая колонка таблицы является левой колонкой
			   для некоторой логической ячейки и правой колонкой для
			   некоторой (быть может отличной) логической ячейки.

		Замечание 2: в процессе обхода таблицы нижняя координата
		строки может определиться прежде, чем верхняя координата.
		Это объясняется тем, что:

			1)обход совершается в порядке перечисления
			  логических ячеек таблицы и

			2)одна ячейка может простираться сверху вниз
			  на несколько строк таблицы.

		Замечание 3: массивы
			gTableColLefts - массив левых координат колонок таблицы
		  и gTableColRights - массив правых координат колонок таблицы

		 - только аллоцируются, но не заполняются.
		 Это вызвано тем, что:

			1) Для расчета колонок порядок обхода таблицы должен
			   отличаться (не по строкам таблицы, а по колонкам);

			2) Координаты колонок нужны только для формата TableText;

			3) Для расчета координат колонок имеется специальная
			   функция TableText_Columns().

	Для колонок таблицы вычисляется:
		gTableColWidths - массив "ширин колонок" таблицы.

	Ширина колонки таблицы вычисляется как максимум ширины ячеек,
	начинающихся в данной колонке таблицы.

*/

#define USE_MEMORY(a)\
			{lth = (a);\
			CHECK_MEMORY(lth);\
			memset(gMemCur,0,lth);\
			gMemCur+=lth;}

	long lth = 0, row = 0, col = 0, cell = 0;

	// Середина свободной памяти, округленная до 8
        gMemCur = (Byte*)((((ulong)gMemStart + (ulong)gMemEnd) >>4) <<3);

	// Массив дескрипторов строк текста
	gBegLineText = (LINE_TEXT*)gMemCur;
	USE_MEMORY(gMaxTextLinesInOneTable*sizeof(gBegLineText[0]));
	gEndLineText = (LINE_TEXT*)gMemCur;
	gCurLineText = gBegLineText;

	// Массив верхних координат строк таблицы
	gTableRowTops = (long*)gMemCur;
	USE_MEMORY(gTableRows * sizeof(gTableRowTops[0]));

	// Массив нижних координат строк таблицы
	gTableRowBottoms = (long*)gMemCur;
	USE_MEMORY(gTableRows * sizeof(gTableRowBottoms[0]));

	// Массив "ширин" колонок таблицы
	gTableColWidths = (long*)gMemCur;
	USE_MEMORY(gTableCols * sizeof(gTableColWidths[0]));

	// Массив левых координат колонок таблицы
	gTableColLefts = (long*)gMemCur;
	USE_MEMORY(gTableCols * sizeof(gTableColLefts[0]));

	// Массив правых координат колонок таблицы
	gTableColRights = (long*)gMemCur;
	USE_MEMORY(gTableCols * sizeof(gTableColRights[0]));

	// Массив дескрипторов ячеек
	gTableText = (CELL_TEXT*)gMemCur;
	USE_MEMORY(gTableCells * sizeof(gTableText[0]));

	// Текущие объекты на всех уровнях
	gCellHandle = NULL;
	gParagraphHandle = NULL;
	gLineHandle = NULL;

	// Счетчики найденных объектов
	gHitCells = 0;
	gHitParagraphs = 0;
	gHitLines = 0;

	gHitChars = 0;
	gHitPictures = 0;

	// Дескриптор ячейки
	gCellText = NULL;

	// Высота таблицы
	gTableTextHeight = 0;

	// Ширина таблицы
	gTableTextWidth = 0;

	// Просмотр ячеек таблицы
	if ( !BrowseTable(Static_TableText_Prepare) )
		return FALSE;

	return TRUE;

}
//********************************************************************
Bool TableText_Delete()
{
// Удалить построчное представление

// Удалить временные массивы, созданные
// в функции TableText_Prepare().
	gBegLineText = NULL;
	gEndLineText = NULL;
	gCurLineText = NULL;

	gTableRowTops = NULL;
	gTableRowBottoms = NULL;
	gTableColWidths = NULL;
	gTableColLefts = NULL;
	gTableColRights = NULL;
	gTableText = NULL;

	gCellText = NULL;

	return TRUE;
}
//********************************************************************
Bool Static_TableText_Prepare(
			Handle hObject,
			long reason	// См. enum BROWSE_REASON
			)
{
// В конце вызывается WordControl

	long *pl = 0, lth = 0;

	switch(reason)
		{
		case BROWSE_CHAR:
			// Символ
			// Установить язык
			{
			long lang = CED_GetCharFontLang(hObject);
			if (lang != gLanguage)
				SetLanguage(lang);
			}

			// Записать символ
			ONE_CHAR(hObject);
			break;

		case BROWSE_PARAGRAPH_START:
			// Начало абзаца

			if (!gPreserveLineBreaks
				&& gCellText->lines	// 21.03.2000
			   )
				{
				// Занести адрес строки в дескриптор строки
				gCurLineText->text = gMemCur;

				// Количество строк в ячейке
				gCellText->lines++;
				}

			break;

		case BROWSE_LINE_START:
			// Начало строки текста

			if ( (gPreserveLineBreaks || gEdLineHardBreak ||
				 !gCellText->lines			// 21.03.2000
				 )
				 && !gCurLineText->text		// 21.03.2000
			   )
				{
				// Занести адрес строки в дескриптор строки
				gCurLineText->text = gMemCur;

				// Количество строк в ячейке
				gCellText->lines++;
				}

			break;

		case BROWSE_PARAGRAPH_END:
			// Конец абзаца
			if (gCurLineText->text)	// 21.02.2000
				LineEnd();
			break;

		case BROWSE_LINE_END:
			// Конец строки текста
			if (gPreserveLineBreaks || gEdLineHardBreak)
				LineEnd();
			break;

		case BROWSE_CELL_START:
			// Начало ячейки таблицы

			// Дескриптор ячейки
			gCellText = &gTableText[gIndexTableCell];

			// Занести адрес свободного дескриптора строки
			gCellText->line = gCurLineText;

			// Занести строку и колонку таблицы
			gCellText->row = gIndexTableRow;
			gCellText->col = gIndexTableCol;
			break;

		case BROWSE_CELL_END:
			// Конец ячейки таблицы

			// Ширина колонки таблицы
			pl = &gTableColWidths[gIndexTableCol];
			*pl = MAX(*pl,gCellText->width);

			// Координата низа строки таблицы для строки таблицы,
			// в которой заканчивается текущая ячейка
			pl = &gTableRowBottoms[BottomRow()];
			*pl = MAX(*pl,
					gTableRowTops[gIndexTableRow] +
						gCellText->lines);

			gCellText = NULL;
			break;

		case BROWSE_ROW_START:
			// Начало строки таблицы

			if ( gIndexTableRow > 0 )
				{
				// Координата верха строки таблицы
				// по предыдущей строке таблицы
				gTableRowTops[gIndexTableRow] =
					gTableRowBottoms[gIndexTableRow - 1] +
						gTableTextIntervalBetweenCellsYY;
				}
			break;

		case BROWSE_TABLE_END:
			// Конец таблицы

			// Высота таблицы по последней строке
			gTableTextHeight = gTableRowBottoms[gTableRows - 1];

			break;
		}

	// Слежение за словами и строками
	WORDS_CONTROL(reason);

	return TRUE;	// Продолжить просмотр
}
//********************************************************************
static Bool LineEnd()
{
// Конец строки текста

	ASSERT(gCurLineText->text);

	// Длина строки текста
	long lth = gMemCur - gCurLineText->text;

	gCurLineText->ltext = lth;

	// Ширина ячейки
	long *pl = &gCellText->width;
	*pl = MAX(*pl,lth);

	// Дописать ноль на конце строки текста
	*gMemCur++ = 0;

	// Свободный элемент в массиве дескрипторов строк
	gCurLineText++;

	if (gCurLineText >= gEndLineText)
		{
		NO_MEMORY;
		return FALSE;
		}

	return TRUE;
}
//********************************************************************
static Bool TableText_Columns()
{
/*
	Рассчитать координаты колонок таблицы:
		gTableColLefts - массив левых координат колонок таблицы
		gTableColRights - массив правых координат колонок таблицы

	Для расчета колонок совершается обход дескрипторов ячеек
	таблицы по колонкам и по строкам таблицы.

	Дескрипторы ячеек должны быть созданы предварительно
	с помощью функции TableText_Prepare().
*/

	// Справочная обхода ячеек
	Byte sprav[10000];
	ASSERT(gTableCells < sizeof(sprav));
	memset(&sprav,0,sizeof(sprav));

	// Цикл по колонкам таблицы
	for (gIndexTableCol = 0;
		gIndexTableCol < gTableCols;
		gIndexTableCol++
		)
		{
		// Начало колонки таблицы
		if ( gIndexTableCol > 0 )
			{
			// Интервал между колонками
			gTableColLefts[gIndexTableCol] =
				gTableColRights[gIndexTableCol-1] +
					gTableTextIntervalBetweenCellsXX;

			// Разделитель между колонками
			if (gTableTextSeparators[1])
				gTableColLefts[gIndexTableCol]++;
			}
		else
			{
			// Смещение первой колонки от начала строки
			gTableColLefts[0] = gTableTextLeftIndent;

			// Разделитель перед первой колонкой
			if (gTableTextSeparators[0])
				gTableColLefts[0]++;
			}

		// Цикл по строкам таблицы
		for (gIndexTableRow = 0;
			gIndexTableRow < gTableRows;
			gIndexTableRow++
			)
			{
			// Логический номер ячейки
			gIndexTableCell = gLogicalCells[
								gIndexTableRow * gTableCols +
								gIndexTableCol];

			// Проверить по справочной
			if(sprav[gIndexTableCell])
				continue;	// В этой логической ячейке уже были

			sprav[gIndexTableCell] = 1;

			// Дескриптор ячейки
			gCellText = &gTableText[gIndexTableCell];

			// Координата правой границы колонки таблицы
			// для самой правой колонки таблицы,
			// в которой заканчивается текущая ячейка
			long *pl = &gTableColRights[RightmostCol()];
			*pl = MAX(*pl,
					gTableColLefts[gIndexTableCol] +
						gCellText->width);

			} // Конец строки таблицы
		} // Конец колонки таблицы

	// Ширина таблицы по последней колонке
	gTableTextWidth = gTableColRights[gTableCols - 1];

	gCellText = NULL;
	gIndexTableCell = 0;
	gIndexTableRow = 0;
	gIndexTableCol = 0;

	return TRUE;
}
//********************************************************************
static Bool TableText_CellWidths()
{
// Рассчитать ширины ячеек с учетом охваченных колонок

	// Цикл по ячейкам таблицы
	for (gIndexTableCell = 0;
		gIndexTableCell < gTableCells;
		gIndexTableCell++
		)
		{
		gCellText = &gTableText[gIndexTableCell];
		gIndexTableRow = gCellText->row;
		gIndexTableCol = gCellText->col;

		// Правая охваченная колонка
		long col2 = RightmostCol();

		// Ширина ячейки с учетом охваченных колонок
		long w = gTableColRights[col2] -
				 gTableColLefts[gIndexTableCol];


		ASSERT(gCellText->width <=w);
		gCellText->width = w;
		}


	return TRUE;
}
//********************************************************************
Bool BrowseTableText(
			FNROUT_BrowseTableTextFunction
				BrowseTableTextFunction
			)
{
/*
	Обход построчного представления таблицы,
	сформированного функцией TableText_Prepare.

	Для каждой строки текста вызывается заданная функция
	BrowseTableTextFunction(), при этом

		gCurLineText - дескриптор строки текста,
		gCellText - дескриптор ячейки,
		gIndexTableCell - номер ячейки,
		gIndexTableRow - номер строки таблицы,
		gIndexTableCol - номер столбца таблицы,
		gYpos - текущая Y-координата (расчетное количество
			    строк текста от верха таблицы)
		gXpos - левая координата текста колонки (расчетное
			    смещение от левого края таблицы)

	Порядок обхода - по уровням текста сверху вниз. На каждом
	уровне делается срез таблицы слева направо.

*/
#define BROWSE_TABLE_TEXT_FUNCTION(a)\
	{if (!BrowseTableTextFunction(a)) return FALSE;}

	Bool empty_line = FALSE;

	gCurLineText = NULL;
	gCellText = NULL;
	gIndexTableCell = 0;
	gIndexTableRow = 0;
	gIndexTableCol = 0;
	gYpos = 0;
	gXpos = 0;

	// Начало таблицы
	BROWSE_TABLE_TEXT_FUNCTION(BROWSE_TABLE_TEXT_TABLE_START);

	// Первая ячейка, в которой есть текст
	if ( FirstCell() )
		{
	// Цикл по уровням текста сверху вниз.
		do
			{
			// Начало уровня
			BROWSE_TABLE_TEXT_FUNCTION(BROWSE_TABLE_TEXT_RECORD_START);

			// Просмотр уровня слева направо
			if (!empty_line) do
				{
				// Выбрать в ячейке строку текста нужного уровня
				long line = gYpos - gTableRowTops[gIndexTableRow];

				if ( line >= 0 && line < gCellText->lines)
					{
					// Дескриптор строки текста
					gCurLineText = &gCellText->line[line];

					// Обработка строки текста
					BROWSE_TABLE_TEXT_FUNCTION(BROWSE_TABLE_TEXT_STRING);
					}
				else
					{
//					ASSERT ( line >= 0 && line < gCellText->lines);
					}

				gCurLineText = NULL;

				// Переход к самой левой ячейке справа от
				// текущей ячейки на том же уровне текста
				} while ( CellRight() );

			// Конец уровня
			BROWSE_TABLE_TEXT_FUNCTION(BROWSE_TABLE_TEXT_RECORD_END);

			// Переход к самой левой ячейке
			// на следующем уровне текста
			empty_line = !NextLevel();

			} while ( gYpos < gTableTextHeight );

		}

	// Конец таблицы
	BROWSE_TABLE_TEXT_FUNCTION(BROWSE_TABLE_TEXT_TABLE_END);

	return TRUE;
}
//********************************************************************
static long BottomRow()
{
// Определить номер самой нижней строки таблицы,
// которую занимает текущая ячейка

	long row = gIndexTableRow,
		 col = gIndexTableCol;

	while( row < gTableRows &&
			gIndexTableCell == gLogicalCells[
							gTableCols * row + col]
		 )
		row++;

	return row-1;
}
//********************************************************************
static long RightmostCol()
{
// Определить номер самой правой колонки таблицы,
// которую занимает текущая ячейка

	long row = gIndexTableRow,
		 col = gIndexTableCol;

	while( col < gTableCols &&
			gIndexTableCell == gLogicalCells[
							gTableCols * row + col]
		 )
		col++;

	return col-1;
}
//********************************************************************
static Bool SearchTable(
			long fromCol	// Левая колонка области поиска
			)
{
/*
	Поиск самой левой ячейки начиная от заданной колонки,
	содержащей текст на текущем уровне текста gYpos.

	Применяется для обхода построчного представления таблицы
	при помощи функции BrowseTableText().

	На входе и на выходе:
		gYpos - текущий уровень текста (не меняется),
		gXpos - устанавливается,
		gIndexTableCell - номер ячейки,
		gIndexTableRow - номер строки таблицы,
		gIndexTableCol - номер столбца таблицы,
		gCellText - дескриптор ячейки.

   		Замечание: между ячейками могут быть рассчитаны интервалы,
		а некоторые ячейки могут не содержать текста.

  	Использую монотонность возрастания левых координат
	колонок таблицы и верхних координат строк таблицы.

	Возвращает FALSE, если ячейка не найдена, при этом
	текущая ячейка не меняется.

*/

	long bestCol = LONG_MAX, bestCell = -1;

	// Просмотреть все строки таблицы от верха таблицы
	// до строки, начинающейся ниже заданного уровня
	for (long row = 0; row < gTableRows; row++ )
		{
		// Стоит ли искать в этой строке
		if (gYpos > gTableRowBottoms[row])
			 continue;

		// Стоит ли искать дальше вниз
		if (gYpos < gTableRowTops[row])
			 break;

		// Искать справа от начальной колонки
		// до первого кандидата
		for (long col = fromCol; col < gTableCols; col++)
			{
			// Ячейка-кандидат
			long cell = gLogicalCells[gTableCols * row + col];

			// Диапазон уровней текста в ячейке
			if (gYpos >= gTableRowTops[row] &&
				gYpos < gTableRowTops[row] + gTableText[cell].lines
				)
				{
				// Сравнить с лучшим кандидатом
				if ( bestCol > col )
					{
					bestCol = col;
					bestCell = cell;
					}

				break;	// Перейти к следующей строке таблицы
				}
			}
		}

	// Ни одного кандидата нет
	if ( bestCell == -1)
		return FALSE;

	// Выдать ответ
	gIndexTableCell = bestCell;
	gCellText = &gTableText[bestCell];
	gIndexTableRow = gCellText->row;
	gIndexTableCol =  gCellText->col;
	gXpos = gTableColLefts[gIndexTableCol];
	return TRUE;

}
//********************************************************************
static Bool CellRight()
{
/*
	Переход к ячейке справа от текущей ячейки
	на том же уровне текста.

	Применяется для обхода построчного представления таблицы
	при помощи функции BrowseTableText().

	Возвращает FALSE, если ячейка не найдена, при этом
	текущая ячейка не меняется.

*/

	// Колонка справа от исходной ячейки
	long fromCol = RightmostCol() + 1;
	if ( fromCol >= gTableCols )
		return FALSE;

	// Искать справа на том же уровне текста
	return SearchTable(fromCol);

}
//********************************************************************
static Bool FirstCell()
{
/*
	Найти первую ячейку таблицы, в которой есть текст
	на текущем уровне.

	Применяется для обхода построчного представления таблицы
	при помощи функции BrowseTableText().

	Возвращает FALSE, если ячейка не найдена.

*/

	// Искать от левого края таблицы на новом уровне текста
	return SearchTable(0);

}
//********************************************************************
static Bool NextLevel()
{
/*
	Переход к самой левой ячейке на следующем уровне текста.

	Применяется для обхода построчного представления таблицы
	при помощи функции BrowseTableText().

	Возвращает FALSE, если ячейка не найдена, при этом
	текущая ячейка не меняется.

*/

	// Нарастить уровень текста
	gYpos++;
	if ( gYpos >= gTableTextHeight )
		return FALSE;

	// Искать от левого края таблицы на новом уровне текста
	return SearchTable(0);

}
//********************************************************************
static Bool TableText_Finish(
			long reason // См. enum BROWSE_TABLE_TEXT_REASON
			)
{
/*
	Выкладывание строк текста в процессе обхода построчного
	представления таблицы с помощью функции BrowseTableText().

	На входе:

		gCurLineText - дескриптор строки текста,
		gCellText - дескриптор ячейки,
		gIndexTableCell - номер ячейки,
		gIndexTableRow - номер строки таблицы,
		gIndexTableCol - номер столбца таблицы,
		gYpos - текущая Y-координата (расчетное количество
			    строк текста от верха таблицы)
		gXpos - левая координата текста колонки (расчетное
			    смещение от левого края таблицы)


*/

	// Номер строки текста и колонки таблицы (static)
	static long line, col;

	switch(reason)
		{
		case BROWSE_TABLE_TEXT_TABLE_START:
			// Начало таблицы
			line = 0;
			break;

		case BROWSE_TABLE_TEXT_TABLE_END:
			// Конец таблицы
			break;

		case BROWSE_TABLE_TEXT_RECORD_START:
			// Начало уровня текста
			col = -1;

			// Вставить пустые строки до заданного уровня
			while ( line < gYpos )
				{
				NEW_LINE;
				line++;
				}

			// Вставить пробелы и разделитель
			// перед первой колонкой
			PUT_SPACES(gTableTextLeftIndent);

			if (gTableTextSeparators[0])
				*gMemCur++ = gTableTextSeparators[0];

			break;


		case BROWSE_TABLE_TEXT_RECORD_END:
			// Конец уровня текста

			// Вставить пустые колонки
			if (!EmptyColumns(col+1,gTableCols-1))
				return FALSE;

			NEW_LINE;
			line++;
			break;

		case BROWSE_TABLE_TEXT_STRING:
			// Строка текста

			// Вставить пустые колонки
			if (!EmptyColumns(col+1,gIndexTableCol-1))
				return FALSE;

			// Правая охваченная колонка
			col = RightmostCol();

			// Выложить строку текста
			PUT_STRING(gCurLineText->text);

			// Добавить пробелов до ширины ячейки,
			// с учетом всех охваченных ячейкой колонок
			// и интервалов между ними
			if ( gTableTextOptions & ROUT_TABLE_TEXT_ALIGN_COLUMNS )
				PUT_SPACES( gCellText->width -
							gCurLineText->ltext +
							gTableTextIntervalBetweenCellsXX
							);

			// Разделитель после колонки
			if(!AfterColumn(col))
				return FALSE;

			break;
		}

	return TRUE;
}
//********************************************************************
static Bool EmptyColumns(
		long fromCol,	// Первая пустая колонка
		long toCol		// Последняя пустая колонка
		)
{
// Вставить пустые колонки
for(long col = fromCol; col <= toCol; col++)
	{
	// Заполнение пустой колонки пробелами
	// и интервал после колонки
	if ( gTableTextOptions & ROUT_TABLE_TEXT_ALIGN_COLUMNS )
		PUT_SPACES(gTableColRights[col] - gTableColLefts[col] +
				   gTableTextIntervalBetweenCellsXX);

	// Разделитель после колонки
	if(!AfterColumn(col))
		return FALSE;

	}

	return TRUE;
}
//********************************************************************
static Bool AfterColumn(long col)
{
// Разделитель после колонки
	char sep = 0;

	if(col < gTableCols-1)
		// Разделитель между колонками
		sep = gTableTextSeparators[1];
	else
		// Разделитель после последней колонки
		sep = gTableTextSeparators[2];

	if (sep)
		*gMemCur++ = sep;

	return TRUE;
}
//********************************************************************
