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
#ifndef __STD_H
#define __STD_H
#include <stdio.h>
#include "globus.h"
#ifndef STD_FUNC
#ifdef __STD__
#define STD_FUNC  FUN_EXPO
#else
#define STD_FUNC  FUN_IMPO
#endif
#endif
#define DEFAULT_ZERO 0
#define DEFAULT_NULL NULL
/////////////////////////////////
// INITIALIZATION
STD_FUNC( Bool32 ) StdLoad(void);
STD_FUNC( void ) StdUnload();

// CONSOLE()
STD_FUNC( int ) stdConsole(const char* str, ...); // с переводом строки
typedef int (*FTConsole)(const char* str, ...);

STD_FUNC( int32_t ) stdOpen(const char *filename, int32_t oflag, int32_t pmode = DEFAULT_ZERO);
STD_FUNC( int32_t ) stdTell(int32_t handle);
STD_FUNC( int32_t ) stdSeek(int32_t handle, int32_t offset, int32_t origin);
STD_FUNC( int32_t ) stdFileLength(int32_t hnd);
STD_FUNC( int32_t ) stdClose(int32_t handle); // ret: 0 -success, -1 -failed
STD_FUNC( int32_t ) stdRead(int32_t handle, void *buffer, int32_t count);
STD_FUNC( int32_t ) stdWrite(int32_t handle, void *buffer, int32_t count);
STD_FUNC( int32_t ) stdAccess(const char *path, int32_t mode);
STD_FUNC( Bool32 ) stdCheckDirectory(const char* name, Bool32 bCreateIfNone);
// checks directory for existance;
// if bCreateIfNone==TRUE - creates if not found
// returns TRUE if directory exists or successfully created
STD_FUNC( uint32_t ) stdGetCurrentDirectory(uint32_t nBufferLength, // size, in characters, of directory buffer
        char * lpBuffer // address of buffer for current directory
        ); // Win32 ::GetCurrentDirectory() analog

STD_FUNC( Bool32 ) stdSetCurrentDirectory(const char * lpBuffer // address of buffer for current directory
        ); // Win32 ::SetCurrentDirectory() analog

STD_FUNC( int32_t ) stdGetDirectoryInfo(const char * lpBuffer, // current directory name buffer
        int32_t nFlags,
#define STD_DIRINFO_FL_DRVTYPE                  0x00000001
        void* pExtParmDEFAULT_NULL); //return special info
#define STD_DIRINFO_DRV_LOCAL                    0x00000001
#define STD_DIRINFO_DRV_NET                         0x00000002
STD_FUNC( int32_t ) stdCmpFileTime(const char * lpFileName1, const char * lpFileName2,
#define STD_CMPFILETIME_FL_LASTCHANGE 0x00000001                    -1
        int32_t nFlags
        = STD_CMPFILETIME_FL_LASTCHANGE
        );

#define STD_CMPFILETIME_LESS                      -1
#define STD_CMPFILETIME_EQUAL                    0
#define STD_CMPFILETIME_MORE                      1
#define STD_CMPFILETIME_ERR                        -2

STD_FUNC( const char* ) stdGetHomeDirectory(void);
// gets pointer to working directory string (no terminating slash)

STD_FUNC( void ) stdGoToHomeDirectory(void);
// sets home directory to be current
//Analog of Windows MessageBox function
STD_FUNC( int32_t ) stdMessageBox(const char * szMessageText, const char * szMessageTitle, int32_t nFlags =
        DEFAULT_ZERO //defined in windows.h or wmb.h
        ); //return value: defined in windows.h or wmb.h

STD_FUNC( uchar ) stdAnsiToAscii(uchar Code); // WIN => DOS
STD_FUNC( uchar ) stdAsciiToAnsi(uchar Code); // DOS => WIN
STD_FUNC(uint16_t) stdAnsiToUnicode(uchar Code);
STD_FUNC(void) stdStrUnicodeToAnsi(char* pAnsi, const wchar_t* pUnicode);
STD_FUNC( uchar ) stdUpperAscii(uchar Code);
STD_FUNC( uchar ) stdLowerAscii(uchar Code);
//String WIN<->DOS converters
STD_FUNC(void) stdStrAsciiToAnsi(char* Str);
STD_FUNC(void) stdStrAnsiToAscii(char* Str);
STD_FUNC(void) stdStrAnsiToUnicode(wchar_t* pUnicode, const char* pAnsi);
STD_FUNC(void) stdStrNAsciiToAnsi(char* Str, int32_t nLen);
STD_FUNC(void) stdStrNAnsiToAscii(char* Str, int32_t nLen);
STD_FUNC(void) stdStrNAnsiToUnicode(wchar_t* pUnicode, const char* pAnsi, int32_t nLen);
STD_FUNC( Bool32) stdLeoCompareChar(uchar c1, uchar c2);
// ('A'rus, 'A'lat) => TRUE; ('A', 'B') => FALSE
STD_FUNC( uchar ) stdLeoTypefaceChar(uchar c);
// 'A'lat => 'A'rus (conversion to dominant letter)
#define     STD_LEO_CASE_NONE   0
#define     STD_LEO_CASE_LOWER  1
#define     STD_LEO_CASE_UPPER  2
STD_FUNC( int32_t ) stdLeoIsCase(uchar Code);
// upper, lower or nondefined
STD_FUNC( uchar) stdLeoSetUpperCase(uchar Code);
// 'A'lat => 'A'lat, 'o'rus => 'O'rus, 'e'rus => 'e'rus
STD_FUNC( uchar) stdLeoSetLowerCase(uchar Code);
// 'C'lat => 'C'lat, 'O'rus => 'o'rus, 'A'rus => 'A'rus
STD_FUNC(uchar *) stdLeoGetTypeface(uchar c);

STD_FUNC(void) stdQsort(void *pMas, int32_t Num, int32_t width, int32_t(*compare)(const void *elm1, const void *elm2));
// INI-file Read-Write Functions
#define STD_SETPROF_DIR_PROJECT                     0x00000000
#define STD_SETPROF_DIR_WINDOWS                   0x00000001
#define STD_SETPROF_DIR_VERSION                     0x00000002
STD_FUNC( Bool32 ) stdGetProfileString(char* szString, int32_t* nStrLen, const char* szIniFileName,
        const char* szSection, const char* szKey, int32_t nFlags = STD_SETPROF_DIR_PROJECT, const char * pszStrDefault =
                "");
STD_FUNC( Bool32 ) stdSetProfileString(const char* szString, const char* szIniFileName, const char* szSection,
        const char* szKey, int32_t nFlags = STD_SETPROF_DIR_PROJECT);
STD_FUNC( int32_t ) stdGetProfileInt(const char* szIniFileName, const char* szSection, const char* szKey,
        int32_t nFlags = STD_SETPROF_DIR_PROJECT, int32_t nValDefault = 0);
STD_FUNC( Bool32 ) stdSetProfileInt(int32_t nValue, const char* szIniFileName, const char* szSection,
        const char* szKey, int32_t nFlags = STD_SETPROF_DIR_PROJECT);

#ifndef NO_STDBITS
STD_FUNC(int) stdBits2Ints(uchar* pBits, int nByteWidth, int32_t* pIntervals // pointer to intervals buffer,
        // should have length (in bytes) == (nByteWidth*4)*4
        );
#endif
/////////////////////
// Error handling
STD_FUNC( int32_t ) stdGetErrorCount(void);
STD_FUNC( int32_t ) stdSetErrorCount(int32_t _nErrCount);
typedef struct tagStdError
{
        int32_t nErrorCode;
        char szCondition[128];
        char szFile[128];
        int32_t nLine;
        int32_t lData;
} StdError;
STD_FUNC( int32_t ) stdSetError(int32_t nErrorCode, const char* cond, const char* file, int32_t nline, int32_t lData
#ifdef __cplusplus
        = 0
#endif
        );
// возвращает номер под которым зарегистрирована ошибка
STD_FUNC( Bool32 ) stdGetError(int32_t nError = 0, StdError * pse = NULL);
// true - есть ошибка с таким номером (диапазон [0, stdGetErrorCount()-1] ):
// простейший вариант использования:
//
// stdSetErrorCount(0)
// ...
// if (stdGetError())   // были зарегистрированы ошибки
//    MyErrorProc();
STD_FUNC( Bool32 ) stdGetLastError(StdError* pse = NULL);
// false - количество ошибок == 0


#endif
