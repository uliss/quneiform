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

#ifndef SYS_PROG_DEFINED
#define SYS_PROG_DEFINED

#ifndef WIN32
#define huge
#define far
#endif

#ifdef PPS_MAC
	#include "split.h"
	#define GLOB
	#define FIND_NDX
	#define FIND_DOT
	#define FIND_BOX
	#define __NDX__
	#define __MRK__
	#define __DOT__
	#define PRS_T
#endif

#define CT_SKEW

#if defined (WIN32) || defined (PPS_MAC)
	#define BITS32
#endif

#ifdef PPS_MAC
	#include <unix.h>
	#include <fcntl.h>
	#define wsprintf sprintf
#endif

#ifdef BITS32
  #define huge
  #define _pascal
  #define far
  #define _far
#endif

//=====‘ҐЄжЁп ­ бва®©ЄЁ ­  баҐ¤г=====
#ifndef _MSC_VER
  #ifdef __BORLANDC__
      #define TC_MOD
  #endif
#else
  #define MSC_MOD
#endif

#ifndef _QWIN
  #if defined (_Windows) || defined (_WINDOWS)
    #define WIN_MOD
    #if defined (__DLL__) || defined (_DLL) || defined (_WINDLL)
      #define DLL_MOD
    #endif
  #endif
#else
	#define DOS_MOD
#endif

#if !defined (WIN_MOD) && !defined(PPS_MAC)
	#define QWIN
#endif


#if defined (_DEBUG) || defined (PPS_MAC)
	// Ћв« ¤®з­л© § Є §/®бў®Ў®¦¤Ґ­ЁҐ Ї ¬пвЁ
	#define DEBUG_MEM
#endif


//#if !defined (WIN_MOD) || !defined (DLL_MOD)
//   #error  ......NOT WIN_MOD OR NOT DLL_MOD......
//#endif

#define MAIN5 //Line

#ifdef WIN_MOD
  #define FL_WIN 1
  #ifdef DLL_MOD
   #ifdef NATUR_WIN
     #undef NATUR_WIN
   #endif
  #else
   #ifndef NATUR_WIN
		 #ifndef QWIN
		 	#define NATUR_WIN
		 #endif
   #endif
  #endif
#else
  #define FL_WIN 0
  #ifdef DLL_MOD
    #undef DLL_MOD
  #endif
  #ifdef NATUR_WIN
    #undef NATUR_WIN
  #endif
#endif

#include <stdio.h>
#include <stdlib.h>
#ifndef PPS_MAC
	#include <sys/stat.h>
#endif

#include <string.h>
#include <fcntl.h>

#ifdef MSC_MOD
  #include <memory.h>
  #include <malloc.h>
  #define MAXINT      0x7FFF
  #ifndef WIN32
    #define MAXSHORT    0x7FFF
    #define MAXLONG     0x7FFFFFFFL
  #endif
#else

	#ifdef __BORLANDC__
	  #include <alloc.h>
	  #include <values.h>
	  #include <mem.h>
	#endif

#endif
/*
#ifndef WIN_MOD
  typedef unsigned char BYTE;
  typedef unsigned int WORD;
  typedef signed long LONG;
  typedef unsigned long DWORD;
  typedef int BOOL;
  #ifndef PASCAL
	  #define PASCAL _pascal
  #endif
  #ifndef LPSTR
     typedef char * LPSTR;
  #endif
  #define MAKELONG(a,b) ((DWORD)(((WORD)(a)) | (((DWORD)((WORD)(b))) << 16)))
  #define LOWORD(l) ((WORD)(l))
  #define HIWORD(l) ((WORD)(((DWORD)(l) >> 16) & 0xFFFF))
  typedef int HFILE;
#endif
*/
#ifndef uint
  #ifndef BITS32
    typedef unsigned int uint;
  #else
    typedef unsigned short uint;
  #endif
  //typedef WORD uint;
#endif

#ifndef uchar
  #define uchar unsigned char
#endif

#ifndef ulong
  #define ulong unsigned long
#endif

#ifdef WIN_MOD
  //#define STRICT
  #include <windows.h>
  #include <windowsx.h>

  #ifdef TC_MOD
    //#define CALLBACK FAR PASCAL
    typedef unsigned int UINT;
    typedef UINT WPARAM;
    typedef LONG LPARAM;
  #endif
  #ifndef WIN32
    #ifdef DLL_MOD
      #define PASC FAR PASCAL _export
    #else
      #define PASC FAR PASCAL _export
    #endif
  #else
    #define PASC
  #endif
  int PutMyMessage(HWND,int,char *);
#else
	#ifndef PPS_MAC
  	/*#include <conio.h>*/
  
	#endif
  #define PASC
  #define WINAPI
  #define CALLBACK
  #define _export
  #define pascal
  
  #ifndef FAR
  #define FAR far
  #endif
  
  #define COM
  /*
  typedef struct {int left,top,right,bottom;} RECT;
  typedef struct h_point { int x,y; } POINT ;

	#ifndef PPS_MAC
		int UnionRect(RECT *out,RECT *in1,RECT *in2);
	#endif
  int SetRectEmpty(RECT *r);
  int CopyRect(RECT *out,RECT *in);
  */
#endif

#ifdef DLL_MOD
  #define FL_DLL 1
  #define STATIC static
#else
  #define FL_DLL 0
  #define STATIC
#endif

#ifdef NATUR_WIN
  #define FL_NATUR 1
#else
  #define FL_NATUR 0
#endif

#ifndef _MAX_DRIVE
	#ifdef DOS_MOD
	  #define _MAX_DRIVE  4
	  #define _MAX_DIR    100
	  #define _MAX_FNAME  9
	  #define _MAX_EXT    4
	#else
		#ifdef PPS_MAC

			#define	_MAX_DRIVE		32
			#define	_MAX_DIR		128
			#define	_MAX_FNAME		32
			#define	_MAX_EXT		32

			#define	LPCSTR			char*

		#endif
	#endif
#endif

// --------- ђ Ў®в  б Ї ¬пвмо Ё ®Ўа Ў®вЄ  ®иЁЎ®Є. Large Model ONLY!!! ----------
#if !defined (SYS_INTERFACE)
#define SYS_INTERFACE

#ifdef __cplusplus
extern "C" {
#endif

#ifdef WIN_MOD
     char      *  PASC malloc_m(unsigned long size);
     char huge *  PASC halloc_m(long n, uint size);
     void         PASC free_m(void *ptr);
     void         PASC hfree_m(void huge *ptr);

     #define    strlen_m  lstrlen
     #define    strcpy_m  lstrcpy
     #define    strcmp_m  lstrcmp
     #define    memcpy_m(s1,s2,n)   hmemcopy((void huge *)s1,(const void huge *)s2,(long)n)
     #define    memset_m(s1,n,len)  hmemset((void huge *)s1,n,(long)len)
#else
     char huge * halloc_m(long n, uint size);
     void        hfree_m(void huge *ptr);

  #ifdef DEBUG_MEM
     void        heapstat(char * mess);
     char      * malloc_m(unsigned long size);
     void        free_m(void *ptr);
  #else
     #define malloc_m  malloc
     #define free_m    free
     #ifndef PPS_MAC
     		#define halloc_m  halloc
		#define hfree_m   hfree
     #endif
  #endif

     #define    strlen_m  strlen
     #define    strcpy_m  strcpy
     #define    strcmp_m  strcmp
     #define    memcpy_m  memcpy
     #define    memset_m  memset
#endif
void free_c(void *ptr);

/* ”г­ЄжЁЁ ®ЎйЁҐ ¤«п WINDOWS & DOS */
void    hmemmove(void huge * out, const void huge * in, long size),
        hmemcopy(void huge *, const void huge *, long),
        hmemset(void huge *, uint , long);

void * realloc_m(void *ptr_old, uint size_old,
                      uint size_new);
void huge * hrealloc_m(void *ptr_old, long size_old, long size_new);


//------  Common functions for mrk.dll, dot.dll, ndx.dll --------
#include "undef32.h"
	#include "globus.h"
#include "wind32.h"

void  free_t(void *ptr, Word32 size);
void* malloc_t(Word32 size);
void* malloc_u(Word32 size);
void  free_u(void *ptr, Word32 size);
void  ProjectRect1024(Rect16 *r,Int32 Skew1024);
void ProjectPoint1024(Point16 *r,Int32 Skew1024);
void  ProjectPoint(Point16 *r,float tg_ang);
Bool16 GetScanRes_LenPrs(char *ImageName);

/* ‘®®ЎйҐ­Ёп Ё ЇаҐ¤гЇаҐ¦¤Ґ­Ёп */
extern char NameFuncErr[100],Buff[60]; extern short NumErr;
void PutMess(int num, char *str),
     ClearERR(void),
     WAR(int num, char *str),
     ERRO(int num,char *str);
void ERR(int num,char *str);
int  GetERR(char *NameFunc);

#include "undef32.h"
#endif
// ---------------- ђ Ў®в  б Ї ¬пвмо Ё ®Ўа Ў®вЄ  ®иЁЎ®Є: ЉЋЌ…– --------------

#ifndef WIN_MOD
/*
	int GetPrivateProfileInt(char *section, char *key, int Default, char *name);
	int GetPrivateProfileString(char *section, char *key, char *Default,
			char *result,int maxsize, char *name);
*/
#endif
#ifdef PPS_MAC
	short strcmpi(const char* s1, const char *s2);
#endif

#ifdef __cplusplus
}
#endif

#endif
/**/

