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


// ROUT.H

//********************************************************************
// Copyright (c) 1999 Cognitive Technologies Ltd.
//
// ROUT.H - API модуля ROUT
//
// This file creation date: 18.05.99
// By Eugene Pliskin pliskin@cs.isa.ac.ru
//********************************************************************

#ifndef __ROUT_H
#define __ROUT_H

#include "puma.h"
#include "pumadef.h"

#ifdef __ROUT__
  #define ROUT_FUNC  FUN_EXPO
#else
  #define ROUT_FUNC  FUN_IMPO
#endif

#ifdef __cplusplus
	extern "C" {
#endif

#pragma pack (push,8)

#ifndef Byte
	typedef unsigned char Byte;
#endif

//*****************************************************************
// Элемент списка форматов или кодировок
// См. функции ROUT_ListFormats и ROUT_ListCodes
typedef struct {
	long code;
	char name[64];
} ROUT_ITEM;
//*****************************************************************
// Список форматов
typedef enum {
ROUT_FMT_Text  = PUMA_TOTEXT,// 2 - Plain text
ROUT_FMT_SmartText = PUMA_TOSMARTTEXT, // 4 - Formatted text
ROUT_FMT_TableText  = PUMA_TOTABLETXT, // 0x100 - Table text
ROUT_FMT_CSV = PUMA_TOTABLECSV, // 0x200 - Table CSV (comma separated)
ROUT_FMT_DBF = PUMA_TOTABLEDBF,	// 0x400 - Table DBF
ROUT_FMT_WKS = PUMA_TOTABLEWKS,	// 0x1000 - Table WKS (Lotus)
ROUT_FMT_HTML = PUMA_TOHTML,	// 0x2000 - HTML
ROUT_FMT_HOCR = PUMA_TOHOCR,

ROUT_FMT_COUNT  = 7,	// Количество форматов
ROUT_FMT_MAX  = 0x2000	// Максимальный формат
	} ROUT_FMT;

//*****************************************************************
// Список кодировок
typedef enum {
// 1 - ASCII (text, formatted text, dbf)
ROUT_CODE_ASCII = PUMA_CODE_ASCII,

// 2 - Windows (all formats)
ROUT_CODE_ANSI = PUMA_CODE_ANSI,

// 4 - KOI8-R (text, formatted text, html)
ROUT_CODE_KOI8R = PUMA_CODE_KOI8,

// 8 - ISO (text, formatted text, html)
ROUT_CODE_ISO = PUMA_CODE_ISO,

// 10 - UTF8 (text, formatted text, html)
ROUT_CODE_UTF8 = PUMA_CODE_UTF8,

	ROUT_CODE_COUNT  = 5,	// Количество кодировок
	ROUT_CODE_MAX  = 16		// Максимальная кодировка
	} ROUT_CODE;
//*****************************************************************
// Опции табличного текста
typedef enum {
// 1 - включить таблицы в текст страницы в формате ROUT_FMT_Text
ROUT_TABLE_TEXT_INCLUDED = 1,

// 2 - выравнивание колонок
ROUT_TABLE_TEXT_ALIGN_COLUMNS = 2

	} ROUT_TABLE_TEXT_OPTIONS;
//*****************************************************************
// Точки входа в DLL имени Петра Хлебутина.
ROUT_FUNC(Bool32) ROUT_Init(uint16_t wHeightCode,Handle hStorage);
ROUT_FUNC(Bool32) ROUT_Done();
ROUT_FUNC(uint32_t) ROUT_GetReturnCode();
ROUT_FUNC(char *) ROUT_GetReturnString(uint32_t dwError);
ROUT_FUNC(Bool32) ROUT_GetExportData(uint32_t dwType, void * pData);
ROUT_FUNC(Bool32) ROUT_SetImportData(uint32_t dwType, void * pData);
//*****************************************************************
// Экспорт
typedef enum
{
	// Опции табличного текста (экспорт-импорт выполняются
	// по одинаковому номеру; потребуется два действия для
	// установки одного отдельного флага не меняя остальные)
	ROUT_LONG_TableTextOptions = 100

} ROUT_EXPORT_ENTRIES;

/*
#ifdef __ROUT__
	#define DEC_FUN(a,b,c) typedef a (*FN##b)c; ROUT_FUNC(a) b c
#else
	#define DEC_FUN(a,b,c) typedef a (*FN##b)c;
//#endif
*/
#define DEC_FUN(a,b,c) ROUT_FUNC(a) b c

// Импорт алфавита, загруженного из REC6.DAT в SPELABC.C.
// Гласные буквы отмечаются знаком "^", согласные любым отличным символом
// Например для английского vowels = "^bcd^fgh^^klmn^pqrst^v^x^z"
DEC_FUN(Bool32, ROUT_SetAlphabet,(
		uint32_t sizeAlphabet,// Количество букв
		char *upper,		// Прописные буквы ( ровно sizeAlphabet )
		char *lower,		// Строчные буквы  ( ровно sizeAlphabet )
		char *vowels		// Гласные буквы   ( ровно sizeAlphabet )
		));

// Функция для загрузки списка таблиц из файла rec6all.dat
DEC_FUN(Bool32, ROUT_LoadRec6List,(
		const char *rec6AllFilename
		));

// Загрузка ED-файла
DEC_FUN(Bool32, ROUT_LoadEd,
		// Параметры как в CED_ReadFormattedEd:
		(char *lpEdFile,	// Имя файла или адрес в памяти
		Bool32 readFromFile,	// TRUE, если задано имя файла
		uint32_t bufLen));	// Длина (только при readFromFile=FALSE)

// Выгрузка ED-файла
DEC_FUN(Bool32, ROUT_UnloadEd,(void));

// Получение списка поддерживаемых форматов
// Возвращает количество форматов или (-1) при ошибке
DEC_FUN(long, ROUT_ListFormats,
	(puchar buf,	  // Адрес буфера для списка ROUT_ITEM
	uint32_t sizeBuf // Длина буфера
	));

// Получение списка возможных форматов сохранения
// для текущей загруженной страницы.
// Возвращает количество форматов или (-1) при ошибке
DEC_FUN(long, ROUT_ListAvailableFormats,
	(puchar buf,	// Адрес буфера для списка ROUT_ITEM
	uint32_t sizeBuf	// Длина буфера
	));

// Получение списка кодировок для данного формата
// Возвращает количество кодировок или -1 при ошибке
DEC_FUN(long, ROUT_ListCodes,
	(puchar buf,	// Адрес буфера для списка ROUT_ITEM
	uint32_t sizeBuf	// Длина буфера
	));

// Перекодировать один байт по кодовой таблице
DEC_FUN(Byte, ROUT_Byte,(Byte c));

// Перекодировать блок памяти по кодовой таблице
DEC_FUN(Bool32, ROUT_Block,(
		Byte *lpMem,	// Адрес блока памяти
		long sizeMem	// Длина блока памяти
		));

// Сосчитать количество объектов на странице.
// Предварительно рекомендуется загрузить страницу (ROUT_LoadEd)
// и установить формат выдачи (ROUT_LONG_Format).
//
// Для табличных форматов выдает количество таблиц на странице,
// а для не-табличных форматов выдает всегда 1.
//
// Выдает -1, если страница не загружена или при другой ошибке.
//
DEC_FUN(long, ROUT_CountObjects,());

// Конвертирование в один формат на заданной памяти
DEC_FUN(Bool32, ROUT_GetObject,
		(
		uint32_t objIndex,	// Индекс объекта начиная от 1
		Byte *lpMem,	// Адрес блока памяти ( 0 - старая память)
		long *sizeMem	// Длина блока памяти ( 0 - старая память)
		));

// Конвертирование в один формат и запись в файл
DEC_FUN(Bool32, ROUT_SaveObject,
		(
		uint32_t objIndex,	// Индекс объекта начиная от 1
		char *path,			// Путь до выходного файла
		Bool32 append		// Дополнение в конец файла
		));

//	Сформировать имя выходного файла из имени страницы,
//	для установленной комбинации формат-кодировка и для
//	заданного индекса объекта. Например:
//		PageName_w.txt - текст в кодировке ANSI
//		PageName_a.txt - текст в кодировке ASCII
//		PageName_k.txt - текст в кодировке KOI8-R
//		PageName_i.txt - текст в кодировке ISO
//		PageName_b_w.txt - табличный текст в кодировке ANSI
//		PageName_b_w2.txt - вторая таблица на странице
//		и т.д.
DEC_FUN(char *, ROUT_GetDefaultObjectName,
		(
		uint32_t objIndex	// Индекс объекта начиная от 1
		));

// Гадкая функция для определения длины объекта
DEC_FUN(uint32_t, ROUT_GetObjectSize,(
		uint32_t objIndex	// Индекс объекта начиная от 1
		));

#undef DEC_FUN

//*****************************************************************

// Импорт
typedef enum
{
 	// Имя страницы без расширения .tif или .fed;
	// может включать или не включать путь
	ROUT_PCHAR_PageName = 1,

	// Страница в контейнере CED
	ROUT_HANDLE_PageHandle = 2,

	// Язык распознавания
	ROUT_LONG_Language = 3,

	// Формат
	ROUT_LONG_Format = 4,

	// Выходная кодировка
	ROUT_LONG_Code = 5,

	// Сохранение концов строк
	ROUT_BOOL_PreserveLineBreaks = 6,

	// Нераспознанный символ
	ROUT_PCHAR_BAD_CHAR = 7,

	// Количество подстановок из REC6.DAT
	ROUT_LONG_CountTigerToUserCharSet = 8,

	// Массив подстановок [3][128] (Tiger/Windows/DOS)
	ROUT_PPBYTE_TigerToUserCharSet = 9,

	// Максимальное количество строк текста в одной таблице
	ROUT_LONG_MaxTextLinesInOneTable = 10,

	// Интервал между ячейками таблицы по вертикали
	ROUT_ULONG_TableTextIntervalBetweenCellsYY = 11,

	// Интервал между ячейками таблицы по горизонтали
	ROUT_ULONG_TableTextIntervalBetweenCellsXX = 12,

	// Смещение таблицы от начала строки
	ROUT_ULONG_TableTextLeftIndent = 13,

	// Список разделителей (пробел - нет, t - табуляция):
	// символ 0 - перед первой колонкой таблицы
	// символ 1 - между колонками таблицы
	// символ 2 - после последней колонки таблицы
	ROUT_PCHAR_TableTextSeparators = 14

	// Опции табличного текста (экспорт-импорт выполняются
	// по одинаковому номеру; потребуется два действия для
	// установки одного отдельного флага не меняя остальные.
	// См. enum ROUT_EXPORT_ENTRIES)
	// ROUT_LONG_TableTextOptions = 100

} ROUT_IMPORT_ENTRIES;

//#define DEC_FUN(a,b,c) typedef a (*FN##b)c
// Колбэки
//#undef DEC_FUN

//*****************************************************************
// Коды возврата
typedef enum {
	ROUT_RETURN_OK = 0,
	ROUT_RETURN_NOTIMPLEMENT = 1,
	ROUT_RETURN_NO_MEMORY = 2,
	ROUT_RETURN_WRONG_ARGUMENT = 3,
	ROUT_RETURN_WRITING_TO_FILE = 4,
	ROUT_RETURN_OPEN_FILE = 5,
	ROUT_RETURN_CLOSE_FILE = 6,
	ROUT_RETURN_PAGE_NOT_LOADED = 7,
	ROUT_RETURN_OBJECT_NOT_FOUND = 8,
	ROUT_RETURN_POSSIBLE_LOSS_OF_DATA = 9
	} ROUT_RETURN_CODES;
//*****************************************************************

#pragma pack (pop)

#ifdef __cplusplus
	}
#endif

#endif	// __ROUT_H
