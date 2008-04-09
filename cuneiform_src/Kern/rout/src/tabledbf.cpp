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


// TableDBF.cpp

//********************************************************************
//********************************************************************

#include "stdafx.h"
#include "rout_own.h"
#include "dbf.h"
#include <time.h>

static BOOL TableDBF_Finish(long reason);
static BOOL MakeDbfHeader();
static Byte DbfLanguage();

static DBF_HEADER *dbf_header;	// Адрес заголовка
static DBF_FIELD *dbf_fields;	// Адрес описателя поля
static long rec_num;	// Номер текущей записи DBF-файла
static Byte *rec_ptr;	// Адрес текущей записи

//********************************************************************
BOOL MakeTableDBF()
{
/* 
	Формат TableDBF.

	Обрабатывается одна таблица.
	Концы строк сохраняются, если gPreserveLineBreaks = TRUE.

    Сначала формируется построчное представление таблицы на правой 
	половине памяти при помощи функции TableText_Prepare().

	Для финальной сборки DBF используется левая половина памяти.

*/

	// Выложить таблицу построчно
	if ( !TableText_Prepare() )
		return FALSE;

	// Сохранить конец памяти
	Byte *saveMemEnd = gMemEnd;

	// Конец оставшейся свободной памяти,
	// предусмотреть бампер на конце
	gMemEnd = (Byte*)gBegLineText - gBumperSize;

	// Текущий указатель в памяти
	gMemCur = gMemStart;

	// Финальная сборка DBF
	BOOL good = BrowseTableText(TableDBF_Finish);

	// Восстановить конец памяти
	gMemEnd = saveMemEnd;

	// Удалить построчное представление
	TableText_Delete();

	return good;
}
//********************************************************************
static BOOL TableDBF_Finish(
			long reason // См. enum BROWSE_TABLE_TEXT_REASON
			)
{
/* 
	Формирование DBF-файла на памяти в процессе обхода построчного
	представления таблицы с помощью функции BrowseTableText().

	На входе:

		gCurLineText - дескриптор строки текста,
		gCellText - дескриптор ячейки,
		gIndexTableCell - номер ячейки,
		gIndexTableRow - номер строки таблицы, 
		gIndexTableCol - номер столбца таблицы

*/

	switch(reason)
		{
		case BROWSE_TABLE_TEXT_TABLE_START:
			// Начало таблицы

			// Создать заголовок DBF-файла
			// включая дескрипторы полей
			if (!MakeDbfHeader())
				return FALSE;

			// Адрес первой записи
			rec_num = 0;
			rec_ptr = gMemCur;
			break;

		case BROWSE_TABLE_TEXT_TABLE_END:
			// Конец таблицы

			// Доработать заголовок DBF-файла
			// Количество записе в DBF-файле
			dbf_header->records = rec_num;

			// Байт "конец файла" 
			*gMemCur++ = DBF_TERM_FILE;	// Конец DBF-файла
			break;

		case BROWSE_TABLE_TEXT_RECORD_START:
			// Начало записи

			// Опробелить всю запись
			PUT_SPACES(dbf_header->record_size);
			break;
			
		case BROWSE_TABLE_TEXT_RECORD_END:
			// Конец записи
			rec_ptr += dbf_header->record_size;
			rec_num++;
			gMemCur = rec_ptr;
			break;

		case BROWSE_TABLE_TEXT_STRING:
			// Значение поля

			{
			// Дескриптор поля
			DBF_FIELD *f = &dbf_fields[gIndexTableCol];

			// Адрес куда положить
			gMemCur = rec_ptr + f->offset;

			// Не более ширины поля, без нулевого байта
			long w = min(f->length,gCurLineText->ltext);

			// Скопировать текст
			memcpy(gMemCur,gCurLineText->text,w);
			}
			break;
		}

	return TRUE;
}
//********************************************************************
static BOOL MakeDbfHeader()
{
// Создать заголовок DBF-файла включая дескрипторы полей
	CHECK_MEMORY(sizeof(DBF_HEADER) +
				gTableCols * sizeof(DBF_FIELD) + 10);

	dbf_header = (DBF_HEADER*) gMemCur;
	gMemCur += sizeof(DBF_HEADER);

	// Приготовить дату и время
	struct tm *today = 0;
	time_t aclock={0};
	time( &aclock );               // Get time in seconds
	today = localtime( &aclock );  // Convert time to struct

	// Заполнение заголовка
	dbf_header->tag = 3;	// Версия файла
	dbf_header->year  = today->tm_year;// Год минус 1900 (до 255)
	dbf_header->month = today->tm_mon+1;
	dbf_header->day   = today->tm_mday;
	dbf_header->header_size = sizeof(DBF_HEADER) + 
							gTableCols * sizeof(DBF_FIELD)
							+ 1;
	dbf_header->record_size = 1;	// Пробел в начале записи

	// Язык
	dbf_header->language = DbfLanguage();

	// Создать дескрипторы полей
	dbf_fields = (DBF_FIELD*) gMemCur;

	for(gIndexTableCol = 0;
		gIndexTableCol < gTableCols;
		gIndexTableCol++, 	
		gMemCur += sizeof(DBF_FIELD)
		)
		{
		// Дескриптор поля
		DBF_FIELD *f = &dbf_fields[gIndexTableCol];

		// Имя поля "col_n"
		sprintf(f->name,"col_%d",gIndexTableCol+1);

		// Тип поля всегда текстовый
		f->type = 'C';

		// Ширина колонки
		long w = gTableColWidths[gIndexTableCol];
		if (w > 254)
			POSSIBLE_LOSS_OF_DATA;

		// Ширина поля
		f->length = min(w,254);

		// Смещение поля от начала записи
		f->offset = dbf_header->record_size;
		dbf_header->record_size += f->length;

		}

	// Признак конца заголовка
	*gMemCur++ = DBF_TERM_BYTE;
	return TRUE;
}
//********************************************************************
static Byte DbfLanguage()
{
// Код языка в заголовке DBF-файла

	long cp = GetCodePage();
	switch(cp)
		{
		case 437:	return 0x01;	// DOS USA
		case 850:	return 0x02;	// DOS Multilingual
		case 1251:	return 0x03;	// Windows ANSI
		case 1250:	return 0xC8;	// Windows EE
		case 852:	return 0x64;	// DOS USA
		case 866:	return 0x66;	// Russian DOS
		case 865:	return 0x65;	// Nordic DOS
		}

	return 0;
}
//********************************************************************
