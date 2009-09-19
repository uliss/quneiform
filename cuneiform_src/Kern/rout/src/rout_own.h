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


// ROUT_OWN.H

#pragma once

#include "stdafx.h"
#include "rout.h"
#include "cfio.h"

#include "ced.h"
//#include "ced_struct.h"
#include "resource.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <limits.h>

#include "compat_defs.h"

//*****************************************************************

// Отладочные макры
// Осторожно, макра DEBUG_PRINT состоит из нескольких предложений
// и не должна использоваться после if без фигурных скобок...
// Правильно: if (...) { DEBUG_PRINT(...,...,...); }
#ifdef _DEBUG
	#define DEBUG_PRINT \
			gFile = __FILE__;gLine = __LINE__;\
			MyDebugPrint
#else
	#define DEBUG_PRINT if(0) MyDebugPrint
#endif

#define NOT_IMPLEMENTED NotImplemented(__FILE__,__LINE__)
#define WRONG_ARGUMENT  WrongArgument(__FILE__,__LINE__)
#define NO_MEMORY NoMemory(__FILE__,__LINE__)
#define ERR_WRITING_TO_FILE ErrWritingToFile(__FILE__,__LINE__)
#define ERR_OPEN_FILE ErrOpenFile(__FILE__,__LINE__)
#define ERR_CLOSE_FILE ErrCloseFile(__FILE__,__LINE__)
#define ERR_CREATE_DIRECTORY ErrCreateDirectory(__FILE__,__LINE__)
#define ERR_PICTURE_DATA ErrPictureData(__FILE__,__LINE__)
#define ERR_LOAD_ALPHABET ErrLoadAlphabet(__FILE__,__LINE__)
#define ERR_LOAD_REC6LIST ErrLoadRec6List(__FILE__,__LINE__)
#define ERR_UPDATE_ACTIVE_ALPHABET ErrUpdateActiveAlphabet(__FILE__,__LINE__)

#define PAGE_NOT_LOADED ErrPageNotLoaded(__FILE__,__LINE__)
#define OBJECT_NOT_FOUND ErrObjectNotFound(__FILE__,__LINE__)
#define POSSIBLE_LOSS_OF_DATA ErrPossibleLossOfData(__FILE__,__LINE__)
#define ASSERT assert

#define NEW_LINE {if(!NewLine()) return FALSE;}
#define ONE_CHAR(a) {if(!OneChar(a)) return FALSE;}
#define PUT_STRING(a) {if(!PutString((char*)a)) return FALSE;}
#define PUT_SPACES(a) {if(!PutSpaces((long)a)) return FALSE;}

#define WORDS_CONTROL(a) {if(!WordsControl(a)) return FALSE;}
#define IS_LETTER(c) IsLetter((Byte)c)
#define INIT_MEMORY(a,b) {if(!InitMemory(a,b)) return FALSE;}
#define CHECK_MEMORY(a) {if(gMemCur+(a)>gMemEnd)\
						{NO_MEMORY; return FALSE;}}

//*****************************************************************
// Глобальные данные.
#ifdef __ROUT_DLL__
	#define EXTERN
	#define VAL(a) = a
	#define VAL2(a,b) = {a,b}
#else
	#define EXTERN extern
	#define VAL(a)
	#define VAL2(a,b)
#endif

//*****************************************************************
// Функции из CFIO
EXTERN FNCFIOAllocMemory		CFIO_Alloc		VAL(0);
EXTERN FNCFIOReAllocMemory		CFIO_ReAlloc	VAL(0);
EXTERN FNCFIOLockMemory			CFIO_Lock		VAL(0);
EXTERN FNCFIOUnlockMemory		CFIO_Unlock		VAL(0);
EXTERN FNCFIOFreeMemory			CFIO_Free		VAL(0);
EXTERN FNCFIOOpenFreeFile		CFIO_Open		VAL(0);
EXTERN FNCFIOCloseFreeFile		CFIO_Close		VAL(0);
EXTERN FNCFIOWriteToFile		CFIO_Write		VAL(0);
EXTERN FNCFIOSeekFilePointer	CFIO_Seek		VAL(0);

//*****************************************************************
// Для отладочной печати
EXTERN const char *gFile	VAL(0);	// Имя файла CPP из __FILE__
EXTERN long gLine	VAL(0);	// Номер строки из __LINE

// Отладочная опция для проверки межмодульных связей
EXTERN long gROUT_Option1	VAL(0);

// Параметры инициализации и мой код ошибки
EXTERN uint16_t	gwHighRC_rout	VAL(0);
EXTERN uint16_t   gwLowRC_rout    VAL(0);
EXTERN HANDLE   ghStorage_rout	VAL(NULL);
EXTERN HANDLE   ghInst_rout	VAL(NULL);

// Язык распознавания, см. lang_def.h
EXTERN long gLanguage		VAL(-1);

// Для Французского Windows
EXTERN Bool gWinFrench		VAL(FALSE);

// Формат, см. enum ROUT_FMT
EXTERN long gFormat			VAL(0);

// Текущая кодировка, см. enum ROUT_CODE
EXTERN long gActiveCode		VAL(ROUT_CODE_ANSI);

// Активная таблица перекодировки
EXTERN Byte *gActiveCodeTable	VAL(NULL);

// Сохранять ли концы строк в текстовых форматах
EXTERN Bool gPreserveLineBreaks VAL(FALSE);

// Нераспознанный символ
EXTERN char gBadChar		VAL('~');

// Собственный кусок памяти, выделяется при инициации
EXTERN Handle gOwnMemory		VAL(NULL);
EXTERN long gOwnMemorySize	VAL(16384);

// Рабочая память: внешняя или своя.
// В начале памяти и в конце отводятся бамперы для страховки.
EXTERN long gBumperSize	VAL(16);	// Длина бампера
EXTERN Byte *gMemStart	VAL(0);		// Начало данных
EXTERN Byte *gMemEnd	VAL(0);		// Конец области данных
EXTERN Byte *gMemCur	VAL(0);		// Текущий конец данных

// Настройка перекодировки, загружаемая из REC6.DAT
// для Узбекского и Казахского.
EXTERN long gCountTigerToUserCharSet	VAL(0);
EXTERN uchar **gTigerToUserCharSet VAL({0});	// [3][128]; // SPELABC.C

// Для удобства
EXTERN Bool langUzbek	VAL(FALSE);
EXTERN Bool langKaz		VAL(FALSE);

// Конец строки
#if defined(_WIN32) || defined(__CYGWIN__)
EXTERN char gEOL[4]		VAL2(0x0d,0x0a);
#else
EXTERN char gEOL[4]             VAL2(0x0a, 0x00);
#endif

// Имя страницы без расширения .tif или .fed;
// может включать или не включать путь
EXTERN char gPageName[_MAX_PATH] VAL("PageName");

// Загруженная страница
EXTERN Handle gPageHandle		VAL(0);

// Страница была загружена мной
EXTERN Bool gPageLoadedByMe		VAL(FALSE);

// Состояние обхода страницы.
// Модуль Browse.cpp.

// Раздел
EXTERN Handle gSectionHandle	VAL(0);
EXTERN long gHitSections		VAL(0);

// Колонка
EXTERN Handle gColumnHandle		VAL(0);
EXTERN long gHitColumns			VAL(0);

// Фрейм
EXTERN Handle gFrameHandle		VAL(0);
EXTERN long gHitFrames			VAL(0);

// Таблица
EXTERN Handle gTableHandle		VAL(0);
EXTERN long gHitTables			VAL(0);

// Формат таблицы
EXTERN long gTableRows			VAL(0);
EXTERN long gTableCols			VAL(0);
EXTERN long gTableCells			VAL(0);
EXTERN long *gLogicalCells		VAL(0);

// Строка таблицы
EXTERN long gIndexTableRow		VAL(0);
EXTERN Handle gRowHandle		VAL(0);

// Клетка
EXTERN Handle gCellHandle		VAL(0);
EXTERN long gHitCells			VAL(0);
EXTERN long gIndexTableCol		VAL(0);
EXTERN long gIndexTableCell		VAL(0);

// Абзац
EXTERN Handle gParagraphHandle	VAL(0);
EXTERN long gHitParagraphs		VAL(0);

// Строка
EXTERN Handle gLineHandle		VAL(0);
EXTERN long gHitLines			VAL(0);

// Конец строки "жесткий" (TRUE) или "скрытый" (FALSE)
EXTERN Bool gEdLineHardBreak	VAL(FALSE);

// Символ
EXTERN Handle gCharHandle		VAL(0);

// Скрытый символ
EXTERN Bool gEdCharHidden		VAL(FALSE);

// Куда был записан символ
EXTERN Byte *gCharBack			VAL(NULL);

// Исходный код символа
EXTERN ulong gCharCode			VAL(0);

// Набор альтернатив
EXTERN struct letterEx *gAlt	VAL(NULL);

// Символы и картинки
EXTERN long gHitChars			VAL(0);
EXTERN long gHitPictures		VAL(0);

// Поиск объекта на странице
EXTERN long gTargetObjectIndex	VAL(0);
EXTERN Handle gTargetObjectHandle VAL(0);

// Размер алфавита
EXTERN long gSizeAlphabet		VAL(0);

// Прописные (строка "АБВ...")
EXTERN Byte gUpper[256]			VAL({0});

// Строчные  (строка "абв...")
EXTERN Byte gLower[256]			VAL({0});

// Гласные (строка "^...")
EXTERN Byte gVowels[256]		VAL({0});

// Склейка переносов и строк (Words.cpp):
// Все буквы (позиционная таблица)
#define CASE_UPPER	1	// Прописная буква
#define CASE_LOWER	2	// Строчная буква
#define CASE_VOWEL	4	// Гласная буква
#define CASE_DIGIT	8	// Цифра
EXTERN Byte gAlphabetTable[256]	VAL({0});

// Последний конец строки в текущем абзаце
EXTERN Byte *gLastEOL			VAL(NULL);

// Начало слова
EXTERN Byte *gBegWord			VAL(NULL);

// Последний дефис в слове
EXTERN Byte *gDefis				VAL(NULL);

// Структуры для создания построчного представления
// табличного текста. Модуль TableText.cpp.

// Представление одной строки текста
typedef struct {
	long ltext;			// Длина строки текста
	Byte *text;			// Адрес строки текста
} LINE_TEXT;

// Для упрощения распределения памяти для массива
// дескрипторов строк текста устанавливается
// максимальное количество строк текста в одной таблице:
EXTERN long gMaxTextLinesInOneTable	VAL(5000);

// Массив дескрипторов строк текста:
EXTERN LINE_TEXT *gBegLineText	VAL(NULL);
EXTERN LINE_TEXT *gCurLineText	VAL(NULL);
EXTERN LINE_TEXT *gEndLineText	VAL(NULL);

// Построчное представление текста в одной ячейке таблицы:
typedef struct {
	long lines;			// Количество строк текста в ячейке
	long width;			// Ширина ячейки ( = максимальная длина
						// строк текста в ячейке )
	LINE_TEXT *line;	// Информация о строках текста в ячейке
	long row;			// В какой строке таблицы начинается ячейка
	long col;			// В какой колонке таблицы начинается ячейка
} CELL_TEXT;

// Табличный текст, разложенный построчно по ячейкам:
EXTERN CELL_TEXT *gTableText	VAL(NULL);

// Дескриптор текущей ячейки
EXTERN CELL_TEXT *gCellText		VAL(NULL);

// Массив верхних координат строк таблицы
// (измеряется в текстовых строках от верха таблицы)
EXTERN long *gTableRowTops		VAL(NULL);

// Массив нижних координат строк таблицы
// (измеряется в текстовых строках от верха таблицы)
EXTERN long *gTableRowBottoms	VAL(NULL);

// Общая высота таблицы (низ нижней строки)
EXTERN long gTableTextHeight	VAL(0);

// Общая ширина таблицы (по правой колонке)
EXTERN long gTableTextWidth		VAL(0);

// Массив ширин колонок таблицы. Определяется для каждой
// колонки как максимум ширины ячеек, НАЧИНАЮЩИХСЯ
// в данной колонке таблицы.
EXTERN long *gTableColWidths	VAL(NULL);

// Массив левых координат колонок таблицы
// (измеряется в текстовых символах от левого края таблицы)
EXTERN long *gTableColLefts		VAL(NULL);

// Массив правых координат колонок таблицы
// (измеряется в текстовых символах от левого края таблицы)
EXTERN long *gTableColRights	VAL(NULL);

// Интервалы между ячейками таблицы
// по вертикали и по горизонтали
EXTERN long gTableTextIntervalBetweenCellsYY	VAL(0);
EXTERN long gTableTextIntervalBetweenCellsXX	VAL(4);

// Смещение таблицы от начала строки
EXTERN long gTableTextLeftIndent	VAL(0);

// Текушие текстовые координаты
EXTERN long gXpos				VAL(0);
EXTERN long gYpos				VAL(0);

// Список разделителей:
// 0 - перед первой колонкой таблицы
// 1 - между колонками таблицы
// 2 - после последней колонки таблицы
EXTERN char gTableTextSeparators[8]	VAL("");

// Опции табличного текста
EXTERN long gTableTextOptions		VAL(
				ROUT_TABLE_TEXT_INCLUDED |
				ROUT_TABLE_TEXT_ALIGN_COLUMNS |
				0);

// Картинка
EXTERN long	gPictureNumber	VAL(0);
EXTERN Byte	*gPictureData	VAL(0);	// Адрес DIB включая заголовок
EXTERN long	gPictureLength	VAL(0);	// Длина DIB включая заголовок

// Размер картинки в TIFF-файле в пикселах
EXTERN EDSIZE gPictureSize	VAL({0});

// Размер картинки на экране в twips
EXTERN EDSIZE gPictureGoal	VAL({0});

// Имя подпапки для картинок "<page>_files". Путь не включается.
EXTERN char gPageFilesFolder[256]	VAL("");

// Таблица алфавитов
#define kMaxSizeAlphabet 63
EXTERN long gAT_sizeAlphabet[LANG_TOTAL] VAL({0});
EXTERN char gAT_upper[LANG_TOTAL][kMaxSizeAlphabet+1]	VAL({0});
EXTERN char gAT_lower[LANG_TOTAL][kMaxSizeAlphabet+1]	VAL({0});
EXTERN char gAT_vowels[LANG_TOTAL][kMaxSizeAlphabet+1] VAL({0});

//*****************************************************************
// Конец глобальных данных
#undef EXTERN
#undef VAL
#undef VAL2
//*****************************************************************
// Rout_DLL.cpp

Handle MyAlloc(uint32_t dwSize, uint32_t dwFlag);
Handle MyReAlloc(Handle hMem,
				uint32_t dwSize,
				uint32_t dwFlag);
Handle MyLock(Handle hMem);
Bool32 MyUnlock(Handle hMem);
Bool32 MyFree(Handle hMem);

void MyDebugPrint(const char *format,...);
void ClearError();
void NotImplemented(const char *file, long line);
void WrongArgument(const char *file, long line);
void NoMemory(const char *file, long line);

void ErrOpenFile(const char *file, long line);
void ErrWritingToFile(const char *file, long line);
void ErrCloseFile(const char *file, long line);
void ErrPageNotLoaded(const char *file, long line);
void ErrObjectNotFound(const char *file, long line);
void ErrPossibleLossOfData(const char *file, long line);
void ErrPictureData(const char *file, long line);
void ErrLoadAlphabet(const char *file, long line);
void ErrLoadRec6List(const char *file, long line);
void ErrUpdateActiveAlphabet(const char *file, long line);

Bool InitMemory(Byte *memStart, long sizeMem);
Bool SetTableTextSeparators(char* s);

//*****************************************************************
// CodeTables.cpp
void ResetCodeTables();
Bool UpdateActiveCodeTable();
long GetCodePage();
const char * getUTF8Str(const unsigned char in, const int codepage);

//*****************************************************************
// Rout.cpp
Bool SetLanguage(long language);
Bool SetFormat(long format);
Bool SetActiveCode(long code);
long BrowseObjects(ulong targetIndex);
long BrowseTables(ulong targetIndex);

//*****************************************************************
// Browse.cpp

// Функция, вызываемая при обходе страницы для каждого объекта
typedef Bool (*FNROUT_BrowseFunction)(
			Handle hObject,
			long reason	// См. enum BROWSE_REASON
			);

// Обход страницы
Bool BrowsePage(FNROUT_BrowseFunction BrowseFunction,
				Bool wantSkipTableCells,
				Bool wantSkipParagraphs
				);

// Обход ячеек таблицы
Bool BrowseCells(FNROUT_BrowseFunction BrowseFunction);

// Обход таблицы, включает обход ячеек, а также
// начальное и конечное сообщение
Bool BrowseTable(FNROUT_BrowseFunction BrowseFunction);

// Обход абзацев, таблиц и фреймов
// в родительском объекте (колонке, фрейме, ячейке)
Bool BrowseParagraphs(Handle hParentObject,
			FNROUT_BrowseFunction BrowseFunction,
			Bool wantSkipTableCells,
			Bool wantSkipParagraphs
			);

typedef enum {
	BROWSE_PAGE_START = 1,
	BROWSE_PAGE_END,		// 2

	BROWSE_SECTION_START,	// 3
	BROWSE_SECTION_END,		// 4

	BROWSE_COLUMN_START,	// 5
	BROWSE_COLUMN_END,		// 6

	BROWSE_FRAME_START,		// 7
	BROWSE_FRAME_END,		// 8

	BROWSE_TABLE_START,		// 9
	BROWSE_TABLE_END,		// 10

	BROWSE_ROW_START,		// 11
	BROWSE_ROW_END,			// 12

	BROWSE_CELL_START,		// 13
	BROWSE_CELL_END,		// 14

	BROWSE_PARAGRAPH_START,	// 15
	BROWSE_PARAGRAPH_END,	// 16

	BROWSE_LINE_START,		// 17
	BROWSE_LINE_END,		// 18

	BROWSE_CHAR,			// 19

	BROWSE_PICTURE			// 20

} BROWSE_REASON;

//*****************************************************************
// Text.cpp
Bool MakeText();
Bool NewLine();
Bool OneChar(Handle charHandle);
Bool PutString(char *s);
Bool PutSpaces(long lth);

//*****************************************************************
// Words.cpp
Bool IsLetter(Byte c);
Bool IsEOL(Byte c);
Bool WordsControl(long reason);
Bool WordEnd();

//*****************************************************************
// HTML.cpp
Bool MakeHTML();
Bool MakeHOCR();
//*****************************************************************
// То чего не хватает в CED.H (из EDP.H и EDFILE.H)
#define FONT_DBLUNDERLINED 1 // подчеркнутый двойной линией
#define FONT_HIDDEN		   2 // скрытый
#define FONT_BOLD          4 // жирный
#define FONT_LIGHT         8
#define FONT_ITALIC       16 // курсивный
#define FONT_PROTECT      32 // защищенный
#define FONT_UNDERLINE    64 // подчеркнутый
#define FONT_STRIKE		 128 // перечеркнутый
#define FONT_SUPER		 256 // верхний индекс
#define FONT_SUB		 512 // нижний индекс
#define FONT_FOOTNOTE	1024 // сноска

#define ALIGN_LEFT		2
#define ALIGN_RIGHT		4
#define ALIGN_CENTER	8
#define ALIGN_MASK		(ALIGN_LEFT | ALIGN_RIGHT | ALIGN_CENTER)

//*****************************************************************
// TableText.cpp
Bool MakeTableText();
Bool TableText_Prepare();
Bool TableText_Delete();

// Обход построчного представления таблицы:
typedef enum {
	BROWSE_TABLE_TEXT_TABLE_START = 1,
	BROWSE_TABLE_TEXT_TABLE_END,

	BROWSE_TABLE_TEXT_RECORD_START,
	BROWSE_TABLE_TEXT_RECORD_END,

	BROWSE_TABLE_TEXT_STRING

} BROWSE_TABLE_TEXT_REASON;

// Функция, вызываемая при обходе построчного
// представления таблицы
typedef Bool (*FNROUT_BrowseTableTextFunction)(
				long reason // См. enum BROWSE_TABLE_TEXT_REASON
				);

Bool BrowseTableText(FNROUT_BrowseTableTextFunction
						BrowseTableTextFunction);

//*****************************************************************
// TableDBF.cpp
Bool MakeTableDBF();

//*****************************************************************
// Pictures.cpp

// Получить кортинку из спецсимвола
Bool PictureFromChar(Handle charHandle);

// Извлечь номер картинки из спецсимвола
long GetPictureNumber(Handle charHandle);

// Записать картинку в BMP-файл
Bool WritePictureToBMP_File(
				Byte *pDIB,	   // Адрес DIB включая заголовок
				long lenDIB,   // Длина DIB включая заголовок
				char *filename // Имя файла
				);

//*****************************************************************
