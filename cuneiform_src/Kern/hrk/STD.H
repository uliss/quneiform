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

#ifdef __DEBUG_HDRS
#pragma message(__FILE__"(1)  :")
#endif

#ifndef __STD_H
   #define __STD_H

   #ifndef __STDIO_H
      #include <stdio.h>
      #define __STDIO_H
   #endif

   #ifndef __GLOBUS_H
      #include "globus.h"
   #endif

   #ifndef STD_FUNC
      #ifdef __STD__
         #define STD_FUNC  FUN_EXPO
      #else
         #define STD_FUNC  FUN_IMPO
      #endif
   #endif

   #ifdef __cplusplus
   extern "C" {
   #define DEFAULT_ZERO =0
   #define DEFAULT_NULL =NULL
   #else
   #define DEFAULT_ZERO
   #define DEFAULT_NULL
   #endif

/////////////////////////////////
// INITIALIZATION
	STD_FUNC( Bool32 ) StdLoad(void);
	STD_FUNC( void  ) StdUnload();

//////////////////////////////////
// CONSOLE()
   STD_FUNC( int  )  stdConsole( const char* str, ... );  // с переводом строки
   STD_FUNC( int )   stdConsole_( const char* str, ... ); // без перевода строки
   typedef int (*FTConsole)( const char* str, ... );
   STD_FUNC( void )  stdSetConsoleHandler( FTConsole handler );
   STD_FUNC( void )  stdSetConsoleFile( char * file );
   STD_FUNC( void )  stdSetOutputFile( FILE* hfile );

   #if (defined( PPS_WIN ) || defined( WIN32 ))
      #ifndef CONSOLE_MESSAGE
      #define CONSOLE_MESSAGE     "Windows Console Message"
      #endif
      #ifndef CONSOLE_WNDCLASS
      #define CONSOLE_WNDCLASS    "ConsoleWindow"
      #endif
   #endif

////////////////////////////////////
// assert() redefinition
#ifndef STD_NO_ASSERT_REDEFINITION
   STD_FUNC( void ) stdAssert(const char * cond, const char *__file,int __line);
#endif

//////////////////////////////////
// malloc() -free()
   STD_FUNC( void* ) stdMalloc( Int32 size );
   STD_FUNC( void* ) stdRealloc( void* old_ptr, Int32 new_size, Int32 old_size );
   STD_FUNC( void )  stdFree( void* ptr );
   STD_FUNC( void )  stdFreeX( void** pptr );
      // smart version of free: checks *ptr,
      // if it not NULL calls stdFree, then set *pptr to = NULL

typedef struct tagStdMemInfo
{
   Int32 nMallocCnt;
   Int32 nFreeCnt;
   Int32 nReallocCnt;
   char  reserved[244];
}  StdMemInfo;

   STD_FUNC( void )  stdGetMemInfo( StdMemInfo* pMemInf );
      // debug purpose function - if _DEBUG not defined, returned values are 0

   STD_FUNC( void* ) stdMemcpy( void* dest , void* src , Int32 copy_count );
      // in 32-bit - falls to memcpy,
      // in 16-bit - takes care about 64k segments bounds

   STD_FUNC( Int32 ) stdOpen( const char *filename, Int32 oflag, Int32 pmode DEFAULT_ZERO );
   STD_FUNC( Int32 ) stdTell( Int32 handle );
   STD_FUNC( Int32 ) stdSeek( Int32 handle, Int32 offset, Int32 origin  );
   STD_FUNC( Int32 ) stdFileLength( Int32 hnd );
   STD_FUNC( Int32 ) stdClose( Int32 handle ); // ret: 0 -success, -1 -failed
   STD_FUNC( Int32 ) stdRead( Int32 handle, void *buffer, Int32 count );
   STD_FUNC( Int32 ) stdWrite( Int32 handle, void *buffer, Int32 count );
   STD_FUNC( Int32 ) stdAccess( const char *path, Int32 mode);

#ifdef __cplusplus
   inline bool stdFileExists(const char* path)
      {  return (stdAccess(path, 00) != -1); };
   inline bool stdFileReadable(const char* path)
      {  return (stdAccess(path, 04) != -1); };
#endif //__cplusplus

   #define stdUnlink unlink


   STD_FUNC( Bool32 ) stdCheckDirectory( const char* name, Bool32 bCreateIfNone );
      // checks directory for existance;
      // if bCreateIfNone==TRUE - creates if not found
      // returns TRUE if directory exists or successfully created

  STD_FUNC( Bool32 ) stdDeleteDirectory(
      const char * lpDirName);

   STD_FUNC( Bool32 ) stdMoveDirectory(
      const char * lpDirNameDst,const char * lpDirNameSrc);

   STD_FUNC( Bool32 ) stdCopyDirectory(
      const char * lpDirNameDst,const char * lpDirNameSrc);

  STD_FUNC( Word32 ) stdGetCurrentDirectory(
      Word32 nBufferLength,   // size, in characters, of directory buffer
      char * lpBuffer   // address of buffer for current directory
      ); // Win32 ::GetCurrentDirectory() analog

   STD_FUNC( Bool32 ) stdSetCurrentDirectory(
      const char * lpBuffer   // address of buffer for current directory
      ); // Win32 ::SetCurrentDirectory() analog

   STD_FUNC( Int32 ) stdGetDirectoryInfo(
      const char * lpBuffer, // current directory name buffer
      Int32 nFlags,
#define STD_DIRINFO_FL_DRVTYPE                  0x00000001
      void* pExtParm DEFAULT_NULL
      ); //return special info
#define STD_DIRINFO_DRV_LOCAL                    0x00000001
#define STD_DIRINFO_DRV_NET                         0x00000002

   STD_FUNC( Int32 ) stdCmpFileTime(
      const char * lpFileName1,
      const char * lpFileName2,
#define STD_CMPFILETIME_FL_LASTCHANGE 0x00000001                    -1
      Int32 nFlags
#ifdef __cplusplus
                     =STD_CMPFILETIME_FL_LASTCHANGE
#endif
      );

   STD_FUNC( Bool32 ) stdNetPathFromLocal(
       char* pszNetPath,
       Int32 nNetPathSize,
       const char* pszLocalPath
       ); //return network path from local path for mapped drives

	STD_FUNC(const char*) stdGetComputerName();


#define STD_CMPFILETIME_LESS                      -1
#define STD_CMPFILETIME_EQUAL                    0
#define STD_CMPFILETIME_MORE                      1
#define STD_CMPFILETIME_ERR                        -2

/* On Windows wchar_t is 16 bit. On Unix it is 32 bit. */
#ifndef wchar_t
#define wchar_t Int16
#endif

STD_FUNC( Bool32 ) stdCheckFile(const char * lpFileName );

STD_FUNC( Bool32 ) stdMoveFile(
      const char * lpFileNameDst,
      const char * lpFileNameSrc,
#define STD_MOVEFILE_FL_OVERWRITE      0x00000001                    -1
#define STD_MOVEFILE_FL_DEFAULT        STD_MOVEFILE_FL_OVERWRITE                    -1
      Int32 nFlags
#ifdef __cplusplus
      =STD_MOVEFILE_FL_OVERWRITE
#endif
                              );

  STD_FUNC( Bool32 ) stdCopyFile(
      const char * lpFileNameDst,
      const char * lpFileNameSrc );

  STD_FUNC( Bool32 ) stdDeleteFile(
      const char * lpFileName);

   STD_FUNC( const char* ) stdGetHomeDirectory(void);
      // gets pointer to working directory string (no terminating slash)

   STD_FUNC( void ) stdGoToHomeDirectory(void);
      // sets home directory to be current

   //Analog of Windows MessageBox function
   STD_FUNC( Int32 ) stdMessageBox(
      const char * szMessageText,
      const char * szMessageTitle,
	  Int32 nFlags  DEFAULT_ZERO //defined in windows.h or wmb.h
      );	//return value: defined in windows.h or wmb.h

   //MessageBox function with no repeate checkbox
   //all params as in stdMessageBox function except key
   STD_FUNC( Int32 ) stdRptMessageBox(
      const char * szMessageText,
      const char * szMessageTitle,
	  Int32 nFlags DEFAULT_ZERO, //defined in windows.h or wmb.h
      const char * szKey DEFAULT_NULL //if ommited then key is szMessageText
      );	//return value: defined in windows.h or wmb.h

    STD_FUNC( Word8 ) stdAnsiToAscii( Word8 Code); // WIN => DOS
    STD_FUNC( Word8 ) stdAsciiToAnsi( Word8 Code); // DOS => WIN
    STD_FUNC(Word16) stdAnsiToUnicode( Word8 Code);
    STD_FUNC(void) stdStrUnicodeToAnsi(char* pAnsi,const wchar_t* pUnicode);

    STD_FUNC( Word8 ) stdUpperAscii ( Word8 Code);
    STD_FUNC( Word8 ) stdLowerAscii ( Word8 Code);
	//String WIN<->DOS converters
	STD_FUNC(void) stdStrAsciiToAnsi( char* Str);
	STD_FUNC(void) stdStrAnsiToAscii( char* Str);
    STD_FUNC(void) stdStrAnsiToUnicode(wchar_t* pUnicode,const char* pAnsi);
	STD_FUNC(void) stdStrNAsciiToAnsi( char* Str,Int32 nLen);
	STD_FUNC(void) stdStrNAnsiToAscii( char* Str,Int32 nLen);
    STD_FUNC(void) stdStrNAnsiToUnicode(wchar_t* pUnicode,const char* pAnsi,Int32 nLen);


    STD_FUNC( Bool32) stdLeoCompareChar(Word8 c1, Word8 c2);
      // ('A'rus, 'A'lat) => TRUE; ('A', 'B') => FALSE
    STD_FUNC( Word8 ) stdLeoTypefaceChar(Word8 c);
      // 'A'lat => 'A'rus (conversion to dominant letter)

#define     STD_LEO_CASE_NONE   0
#define     STD_LEO_CASE_LOWER  1
#define     STD_LEO_CASE_UPPER  2
    STD_FUNC( Int32 ) stdLeoIsCase(Word8 Code);
      // upper, lower or nondefined
    STD_FUNC( Word8)  stdLeoSetUpperCase(Word8 Code);
      // 'A'lat => 'A'lat, 'o'rus => 'O'rus, 'e'rus => 'e'rus
    STD_FUNC( Word8)  stdLeoSetLowerCase(Word8 Code);
      // 'C'lat => 'C'lat, 'O'rus => 'o'rus, 'A'rus => 'A'rus
    STD_FUNC(Word8 *) stdLeoGetTypeface(Word8 c);

    STD_FUNC(void) stdQsort(void *pMas,Int32 Num, Int32 width,
			  Int32 (*compare)(const void *elm1,const void *elm2) );
/*
//////////////////////////////////
// open(), read(), _write(), close(), filelength(), seek(), tell()
   stdOpen()
   stdRead()
   stdWrite()
   stdClose()
   stdFilelength()
   stdSeek()
   stdTell()
*/

// INI-file Read-Write Functions
#define STD_SETPROF_DIR_PROJECT                     0x00000000
#define STD_SETPROF_DIR_WINDOWS                   0x00000001
#define STD_SETPROF_DIR_VERSION						0x00000002
STD_FUNC( Bool32 ) stdGetProfileString(char* szString,Int32* nStrLen,const char* szIniFileName,const char* szSection,const char* szKey,Int32 nFlags
#ifdef __cplusplus
                                       =STD_SETPROF_DIR_PROJECT
#endif
                                        , const char* pszStrDefault
#ifdef __cplusplus
                                       =""
#endif
                                       );
STD_FUNC( Bool32 ) stdSetProfileString(const char* szString,const char* szIniFileName,const char* szSection,const char* szKey,Int32 nFlags
#ifdef __cplusplus
                                       =STD_SETPROF_DIR_PROJECT
#endif
                                       );
STD_FUNC( Int32 ) stdGetProfileInt(const char* szIniFileName,const char* szSection,const char* szKey,Int32 nFlags
#ifdef __cplusplus
                                       =STD_SETPROF_DIR_PROJECT
#endif
                                        ,  Int32 nValDefault
#ifdef __cplusplus
                                       =0
#endif
                                       );
STD_FUNC( Bool32 ) stdSetProfileInt(Int32 nValue,const char* szIniFileName,const char* szSection,const char* szKey,Int32 nFlags
#ifdef __cplusplus
                                       =STD_SETPROF_DIR_PROJECT
#endif
                                       );

// property bag DLL can use prop bags for info exchange
STD_FUNC(Bool32) stdSetProperty(const char* pKey,const char* pValue);
STD_FUNC(const char*) stdGetProperty(const char* pKey);
//standard key prefixes :
	//SK-System Key
	//AK-Application Key
	//EK-sEssion Key
//look for standard keys in "StdPropK.h

#ifndef NO_STDBITS

	 STD_FUNC(int) stdBits2Ints(
		Word8*   pBits,
		int      nByteWidth,
		Int32*   pIntervals  // pointer to intervals buffer,
									// should have length (in bytes) == (nByteWidth*4)*4
                          );
#endif


   /////////////////////
   // Error handling
   STD_FUNC( Int32 ) stdGetErrorCount(void);
   STD_FUNC( Int32 ) stdSetErrorCount( Int32 _nErrCount );
   typedef struct tagStdError
   {
      Int32 nErrorCode;
      char  szCondition[128];
      char  szFile[128];
      Int32 nLine;
      Int32 lData;
   } StdError;

   STD_FUNC( Int32 ) stdSetError( Int32 nErrorCode, char* cond, char* file,
      Int32 nline, Int32 lData
#ifdef __cplusplus
	  = 0
#endif
	  );
   // возвращает номер под которым зарегистрирована ошибка

   STD_FUNC( Bool32 ) stdGetError( Int32 nError
#ifdef __cplusplus
	  = 0
#endif
, StdError* pse
#ifdef __cplusplus
	  = NULL
#endif
);
   // true - есть ошибка с таким номером (диапазон [0, stdGetErrorCount()-1] ):
   // простейший вариант использования:
   //
   // stdSetErrorCount(0)
   // ...
   // if (stdGetError())   // были зарегистрированы ошибки
   //    MyErrorProc();

   STD_FUNC( Bool32 ) stdGetLastError( StdError* pse
#ifdef __cplusplus
	  = NULL
#endif
);
   // false - количество ошибок == 0


   #ifdef __cplusplus
            }
   #endif

#endif
