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


// Rout_DLL.cpp

//********************************************************************
// Rout_DLL.CPP - стандартные точки входа в ROUT.DLL,
//				  межмодульные связи
//				  и отладочные функции
//
// This file creation date: 18.05.99
// By Eugene Pliskin pliskin@cs.isa.ac.ru
//********************************************************************
#include <stdafx.h>

#define __ROUT_DLL__	// Только в этом модуле, перед rout_own.h

#include "rout_own.h"

#include "resource.h"

#ifdef _MSC_VER
#include <crtdbg.h>
#include <io.h>
#else
#include <unistd.h>
#endif

#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <assert.h>
#include <stdio.h>
#include <stdarg.h>
#include "compat_defs.h"

//*********************************************************************
Bool APIENTRY DllMain( HANDLE hModule,
uint32_t ul_reason_for_call,
                        pvoid lpReserved )
{
    switch( ul_reason_for_call )
		{
		case DLL_PROCESS_ATTACH:
			ghInst_rout = hModule;
			break;
		case DLL_THREAD_ATTACH:
			break;
		case DLL_THREAD_DETACH:
			break;
		case DLL_PROCESS_DETACH:
			break;
		}

    return TRUE;
}
//********************************************************************
ROUT_FUNC(Bool32) ROUT_Init(uint16_t wHighCode,HANDLE hStorage)
{
//	DEBUG_PRINT("ROUT_Init(%d,%d)",wHighCode,hStorage);

	gwHighRC_rout = wHighCode;
	ghStorage_rout = hStorage;
	gwLowRC_rout = 0;

	// Собственный кусок памяти на одну страницу
	gOwnMemory = MyAlloc(gOwnMemorySize,0);
	if ( !gOwnMemory )
		NO_MEMORY;

	return ROUT_GetReturnCode()==0? TRUE:FALSE;
}
//********************************************************************
ROUT_FUNC(Bool32) ROUT_Done()
{
//	DEBUG_PRINT("ROUT_Done");
	ROUT_UnloadEd();

	if (gOwnMemory)
		MyFree(gOwnMemory);
	gOwnMemory  = NULL;

	return TRUE;
}
//********************************************************************
ROUT_FUNC(uint32_t) ROUT_GetReturnCode()
{
// Возвращает 0 если нет ошибки
// Добавляет в старшие 2 байта мой код модуля из gwHighRC_rout
uint32_t rc = 0;
	if(gwLowRC_rout)
		rc = (uint32_t)(gwHighRC_rout<<16)|(gwLowRC_rout - IDS_ERR_NO);

return rc;
}
//********************************************************************
ROUT_FUNC(char *) ROUT_GetReturnString(uint32_t dwError)
{
	uint16_t rc = (uint16_t)(dwError & 0xFFFF) + IDS_ERR_NO;
	static char szBuffer[512];

	if( dwError >> 16 != gwHighRC_rout)
		gwLowRC_rout = IDS_ERR_NOTIMPLEMENT;

	if( rc >= IDS_ERR_NO )
		LoadString((HINSTANCE)ghInst_rout,rc,
				(char *)szBuffer,sizeof(szBuffer));
	else
		return NULL;

	return szBuffer;
}
//********************************************************************
ROUT_FUNC(Bool32) ROUT_GetExportData(uint32_t dwType, void * pData)
{
// Экспорт моих функций
	Bool32 rc = TRUE;

	gwLowRC_rout = 0;

//#define CASE_FUNCTION(a) case ROUT_FN##a: *(FN##a *)pData = a; break

	switch(dwType)
	{

	case ROUT_LONG_TableTextOptions:
		*(long*) pData = gTableTextOptions;
		break;

	case ROUT_HANDLE_PageHandle:
		*(Handle*) pData = gPageHandle;
		break;

	default:
		gwLowRC_rout = IDS_ERR_NOTIMPLEMENT;
		rc = FALSE;
	}

//#undef CASE_FUNCTION

	return rc;
}
//********************************************************************
ROUT_FUNC(Bool32) ROUT_SetImportData(uint32_t dwType, void * pData)
{
// Импорт моих опций

#define CASE_FUNCTION(a) case ROUT_FN##a: a=(FN##a)pData; break
	Bool rc = TRUE;

	switch(dwType)
		{
		// Страница в контейнере CED
		case ROUT_HANDLE_PageHandle:
			gPageHandle = (Handle)pData;
			break;

		// Язык
		case ROUT_LONG_Language:
			SetLanguage ((long)pData);
			break;

		// Формат
		case ROUT_LONG_Format:
			SetFormat((long)pData);
			break;

		// Выходная кодировка
		case ROUT_LONG_Code:
			SetActiveCode ((long)pData);
			break;

		// Сохранение концов строк
		case ROUT_BOOL_PreserveLineBreaks:
			gPreserveLineBreaks = (pData!=0);
			break;

		// Нераспознанный символ
		case ROUT_PCHAR_BAD_CHAR:
			gBadChar = *(char*) pData;
			break;

		// Количество подстановок из REC6.DAT
		case ROUT_LONG_CountTigerToUserCharSet:
			gCountTigerToUserCharSet = (long) pData;
			break;

		// Массив подстановок [3][128] (Tiger/Windows/DOS)
		case ROUT_PPBYTE_TigerToUserCharSet:
			gTigerToUserCharSet = (uchar**) pData;
			break;

		// Максимальное количество строк текста в одной таблице
		case ROUT_LONG_MaxTextLinesInOneTable:
			gMaxTextLinesInOneTable	= (long) pData;
			break;

		// Интервал между ячейками таблицы по вертикали
		case ROUT_ULONG_TableTextIntervalBetweenCellsYY:
			{
                        ulong ul = (ulong) pData;
			if (ul > 100)
				{
				WRONG_ARGUMENT;
				break;
				}
			else
                                gTableTextIntervalBetweenCellsYY = (ulong) pData;
			}
			break;

		// Интервал между ячейками таблицы по горизонтали
		case ROUT_ULONG_TableTextIntervalBetweenCellsXX:
			{
                        ulong ul = (ulong) pData;
			if (ul > 100)
				{
				WRONG_ARGUMENT;
				break;
				}
			else
                                gTableTextIntervalBetweenCellsXX = (ulong) pData;
			}
			break;

		// Смещение таблицы от начала строки
		case ROUT_ULONG_TableTextLeftIndent:
			{
                        ulong ul = (ulong) pData;
			if (ul > 100)
				{
				WRONG_ARGUMENT;
				break;
				}
			else
				gTableTextLeftIndent = ul;
			}
			break;

	 	// Имя страницы без расширения .tif или .fed
		case ROUT_PCHAR_PageName:
			memset(gPageName,0,sizeof(gPageName));
			if (pData)
				{
				if ( strlen((char*)pData)
						+ 20 // Запас для суффиксов и расширения
						>= sizeof(gPageName)
					)
					WRONG_ARGUMENT;
				else
					strcpy (gPageName, (char*)pData);
				}

			break;

		// Список разделителей:
		case ROUT_PCHAR_TableTextSeparators:
			SetTableTextSeparators((char*)pData);
			break;

		// Опции табличного текста (экспорт-импорт выполняются
		// по одинаковому номеру, см. enum ROUT_EXPORT_ENTRIES)
		case ROUT_LONG_TableTextOptions:
			gTableTextOptions = (long) pData;
			break;

		default:
			gwLowRC_rout = IDS_ERR_NOTIMPLEMENT;
			rc = FALSE;
		}
#undef CASE_FUNCTION
return rc;
}
//********************************************************************
void SetReturnCode_rout(uint16_t rc)
{
	gwLowRC_rout = rc;
}
//********************************************************************
uint16_t GetReturnCode_rout()
{
	return gwLowRC_rout;
}
//********************************************************************
// Далее идут мои переходники для CFIO.
//
Handle MyAlloc(uint32_t dwSize, uint32_t dwFlag)
{
	return CFIO_Alloc?
		CFIO_Alloc(dwSize, dwFlag):
		GlobalAlloc(dwFlag, dwSize);
}
//********************************************************************
Handle MyReAlloc(Handle hMem,
				uint32_t dwSize,
				uint32_t dwFlag)
{
	return CFIO_ReAlloc?
		CFIO_ReAlloc(hMem, dwSize, dwFlag):
		GlobalReAlloc(hMem, dwSize, dwFlag);
}
//********************************************************************
Handle MyLock(Handle hMem)
{
	return CFIO_Lock?
		CFIO_Lock(hMem):
		GlobalLock(hMem);
}
//********************************************************************
Bool32 MyUnlock(Handle hMem)
{
	return CFIO_Unlock?
		CFIO_Unlock(hMem):
		GlobalUnlock(hMem);
}
//********************************************************************
Bool32 MyFree(Handle hMem)
{
	return CFIO_Free?
		CFIO_Free(hMem):
	((GlobalFree(hMem)==NULL)?TRUE:FALSE);
}

//*********************************************************************/
void MyDebugPrint(const char *format,...)
{
// Отладочная печать. См. макру DEBUG_PRINT.

#ifdef _DEBUG

	char buf[4096] = {0};
	va_list marker;

	va_start( marker, format ); // Initialize variable arguments.
	vsprintf(buf,format,marker);
	va_end( marker );	// Reset variable arguments

	_CrtDbgReport(	_CRT_WARN,
					gFile,
					gLine,
					"ROUT.DLL",
					"\n%s\n",
					buf);

#endif
}
//********************************************************************
void ClearError()
{
	gwLowRC_rout = 0;
}
//********************************************************************
void NotImplemented(const char *file, long line)
{
	gFile = file;
	gLine = line;
	MyDebugPrint("NotImplemented");

	gwLowRC_rout = IDS_ERR_NOTIMPLEMENT;
}
//********************************************************************
void WrongArgument(const char *file, long line)
{
	gFile = file;
	gLine = line;
	MyDebugPrint("WrongArgument");

	gwLowRC_rout = IDS_ERR_WRONG_ARGUMENT;
}
//********************************************************************
void NoMemory(const char *file, long line)
{
	gFile = file;
	gLine = line;
	MyDebugPrint("NoMemory");

	gwLowRC_rout = IDS_ERR_NO_MEMORY;
}
//********************************************************************
void ErrOpenFile(const char *file, long line)
{
	gFile = file;
	gLine = line;
	MyDebugPrint("ErrOpenFile");

	gwLowRC_rout = IDS_ERR_OPEN_FILE;
}
//********************************************************************
void ErrWritingToFile(const char *file, long line)
{
	gFile = file;
	gLine = line;
	MyDebugPrint("ErrWritingToFile");

	gwLowRC_rout = IDS_ERR_WRITING_TO_FILE;
}
//********************************************************************
void ErrCloseFile(const char *file, long line)
{
	gFile = file;
	gLine = line;
	MyDebugPrint("ErrCloseFile");

	gwLowRC_rout = IDS_ERR_CLOSE_FILE;
}
//********************************************************************
void ErrCreateDirectory(char *file, long line)
{
	gFile = file;
	gLine = line;
	MyDebugPrint("ErrCreateDirectory");

	gwLowRC_rout = IDS_ERR_CREATE_DIRECTORY;
}
//********************************************************************
void ErrPageNotLoaded(const char *file, long line)
{
	gFile = file;
	gLine = line;
	MyDebugPrint("ErrPageNotLoaded");

	gwLowRC_rout = IDS_ERR_PAGE_NOT_LOADED;
}
//********************************************************************
void ErrObjectNotFound(const char *file, long line)
{
	gFile = file;
	gLine = line;
	MyDebugPrint("ErrObjectNotFound");

	gwLowRC_rout = IDS_ERR_OBJECT_NOT_FOUND;
}
//********************************************************************
void ErrPossibleLossOfData(const char *file, long line)
{
	gFile = file;
	gLine = line;
	MyDebugPrint("ErrPossibleLossOfData");

	gwLowRC_rout = IDS_ERR_POSSIBLE_LOSS_OF_DATA;
}
//********************************************************************
void ErrPictureData(const char *file, long line)
{
	gFile = file;
	gLine = line;
	MyDebugPrint("ErrPictureData");

	gwLowRC_rout = IDS_ERR_PICTURE_DATA;
}
//********************************************************************
void ErrLoadAlphabet(const char *file, long line)
{
	gFile = file;
	gLine = line;
	MyDebugPrint("ErrLoadAlphabet");

	gwLowRC_rout = IDS_ERR_LOAD_ALPHABET;
}
//********************************************************************
void ErrLoadRec6List(const char *file, long line)
{
	gFile = file;
	gLine = line;
	MyDebugPrint("ErrLoadRec6List");

	gwLowRC_rout = IDS_ERR_LOAD_REC6LIST;
}
//********************************************************************
void ErrUpdateActiveAlphabet(const char *file, long line)
{
	gFile = file;
	gLine = line;
	MyDebugPrint("ErrUpdateActiveAlphabet");

	gwLowRC_rout = IDS_ERR_UPDATE_ACTIVE_ALPHABET;
}
//********************************************************************
Bool InitMemory(Byte *memStart,
				  long sizeMem)
{
// Отвести страховочные бамперы в начале
// и в конце отведенной области памяти
//
// Если передана нулевая память, то это значит, то
// нужно повторно использовать ту же память.

	if ( !memStart )
		{
		// Reuse memory
		memStart = gMemStart;
		sizeMem = gMemEnd - gMemStart + gBumperSize;
		}

	else if(sizeMem < gBumperSize)
		{
		NO_MEMORY;
		return FALSE;
		}

	// Всю память обнулить, это важно.
	memset(memStart, 0, sizeMem);

	// Отвести область данных
	gMemStart = memStart;
	gMemEnd = gMemStart + sizeMem - gBumperSize;
	gMemCur = gMemStart;

	return TRUE;
}
//********************************************************************
Bool SetTableTextSeparators(char* s)
{
// Список разделителей табличного текста:

	if (!s || strlen(s) >= sizeof(gTableTextSeparators))
		{
		WRONG_ARGUMENT;
		return FALSE;
		}

	memset(gTableTextSeparators,0,sizeof(gTableTextSeparators));
	strcpy(gTableTextSeparators,s);

	// Заменить пробелы на нули, а t на табуляцию
	for(long n = 0; n < sizeof(gTableTextSeparators)-1; n++)
		{
		char *p = &gTableTextSeparators[n];
		if (*p == ' ')
			*p = 0;
		else if (*p == 't')
			*p = '\t';
		}

	return TRUE;
}
//********************************************************************
