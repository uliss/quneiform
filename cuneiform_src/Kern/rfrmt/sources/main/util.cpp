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

#include "sys_prog.h"

#include "wind32.h"

#ifdef _MSC_VER
#include<malloc.h>
#endif

char NameFuncErr[100],Buff[60]; short NumErr;

void heapstat(char *mess)
{
 #ifdef _MSC_VER
    int   status = _heapchk(), n;
  switch (status)
  {
    case _HEAPOK:       // heap is fine
    case _HEAPEMPTY:    // empty heap
    case _HEAPEND:      // end of heap
        return;
  }

  fprintf(stderr, "\n%s",mess);
  switch (status)
  {
    case _HEAPBADPTR:   // bad pointer to heap
         n = 1; break;

    case _HEAPBADBEGIN: // bad start of heap
         n = 2; break;

    case _HEAPBADNODE:  // bad node in heap
         n = 3; break;

    default:            // ЌҐа бЇ®§­ ­­ п ®иЁЎЄ 
         n = -1;
  }

  ERR(n, "Heapstat");
 #endif
}

//===
#if defined (WIN_MOD) || defined (DEBUG_MEM)
char * PASC malloc_m(unsigned long size)
{

#ifdef WIN_MOD
	#ifndef WIN32

		HANDLE h;
		#ifdef DEBUG_MEM
			char *p,*err="malloc_m";
			if (!(h=GlobalAlloc(GMEM_MOVEABLE, (long)size)))
				return NULL;
			if((p=GlobalLock(h)) == NULL)
				ERR(2,err);
			return p;
		#else
			if (!(h=GlobalAlloc(GHND, (long)size)))
				return NULL;
			return GlobalLock(h);
		#endif

	#else
	#ifdef DEBUG_MEM

		char *p,*err="malloc_m";
		if(!size)
       		ERR(1,err);
		heapstat("malloc_m bef");
		if((p=(char*)malloc(size)) == NULL)
			ERR(2,err);
		heapstat("malloc_m aft");
		return p;

	#else
		return (char*)malloc(size);
	#endif
	#endif
#else
#ifdef DEBUG_MEM

	char *p,*err="malloc_m";
	if(!size)
		ERR(1,err);
	if(size > 0xFFFF)
		ERR(3,err);
	heapstat("malloc_m bef");
	if((p=(char*)malloc((size_t)size)) == NULL)
		ERR(2,err);
	heapstat("malloc_m aft");
		return p;

#else
	return malloc((size_t)size);
#endif
#endif
}

//=================
char * PASC  halloc_m(long n, uint size)
{ char *err="halloc_m";

  #ifdef WIN_MOD
    #ifndef WIN32
      HANDLE h;
      if (!(h=GlobalAlloc(GHND, n * size))) return NULL;
      return GlobalLock(h);
    #else
      #ifdef DEBUG_MEM
        char * p;
        if(!size)ERR(1,err);
        heapstat("halloc_m bef");
        p=(char*)malloc(n*size);
        heapstat("halloc_m aft");
        return p;
      #else
       return (char*) malloc((int32_t)n * size);
      #endif
    #endif
  #else
    	return  (char*)malloc(n*size);
  #endif
}
//=================
void PASC free_m(void *ptr)
{


  #ifdef WIN_MOD
  if(ptr)
  {
    #ifndef WIN32
      HANDLE h,h1;
      #ifdef DEBUG_MEM
       while( GlobalUnlock( h=LOWORD(GlobalHandle(SELECTOROF(ptr))) ) );
      #else
       GlobalUnlock( h=LOWORD(GlobalHandle(SELECTOROF(ptr))) );
      #endif
      if( h1 = (HANDLE) GlobalFree(h) )
				ERR(1,"free_m");
    #else
      #ifdef DEBUG_MEM
       if(!ptr)
					ERR(2,"free_m");
       heapstat("free_m bef");
       free(ptr);
       heapstat("free_m aft");
      #else
       free(ptr);
      #endif
    #endif
  }
  #else
   #ifdef DEBUG_MEM
    if(!ptr)
			ERR(2,"free_m");
    heapstat("free_m bef");
    free(ptr);
    heapstat("free_m aft");
   #else
    free(ptr);
   #endif
  #endif
}
//=================
void   PASC hfree_m(void *ptr)
{

  #ifdef WIN_MOD
    #ifndef WIN32
      if(ptr)
      { HANDLE h,h1;
        #ifdef DEBUG_MEM
         while( GlobalUnlock( h=LOWORD(GlobalHandle(SELECTOROF(ptr))) ) );
        #else
         GlobalUnlock( h=LOWORD(GlobalHandle(SELECTOROF(ptr))) );
        #endif
        if (h1 = (HANDLE) GlobalFree(h))
					ERR(1,"hfree_m");
      }
    #else
      #ifdef DEBUG_MEM
       if(ptr == NULL)
					ERR(2,"hfree_m");
       heapstat("hfree_m bef");
       free(ptr);
       heapstat("hfree_m aft");
      #else
       free(ptr);
      #endif
    #endif
  #else //WIN_MOD
     free(ptr);

  #endif //WIN_MOD
}
#endif /* defined (WIN_MOD) || defined (DEBUG_MEM) */

//®бў®Ў®¦¤Ґ­ЁҐ б ЇаҐ¤ў аЁв. Їа®ўҐаЄ®© ­  NULL
void free_c(void *ptr)
{
  if(ptr)
  #ifdef WIN_MOD
    #ifndef WIN32
      { HANDLE h,h1;
        #ifdef DEBUG_MEM
         while( GlobalUnlock( h=LOWORD(GlobalHandle(SELECTOROF(ptr))) ) );
        #else
         GlobalUnlock( h=LOWORD(GlobalHandle(SELECTOROF(ptr))) );
        #endif
        if( h1 = (HANDLE) GlobalFree(h) )
					ERR(1,"free_c");
      }
    #else
      {free(ptr);}
    #endif
  #else
    	{free(ptr);}
    #endif
}

//===
void * realloc_m(void *ptr_old, uint size_old, uint size_new)
{ void *ptr_new;
  if(!size_old)
  #ifdef DEBUG_MEM
    ERR(1,"realloc_m");
  #else
    /* ’ Є ваҐЎгҐв бв ­¤ ав realloc'  */
    return malloc(size_new);
  #endif

  if (size_new <= size_old) return ptr_old;
  if ( ptr_new = malloc(size_new) )
  {
    //memcpy_m(ptr_new,ptr_old,size_old);
    memmove(ptr_new,ptr_old,size_old);
    free(ptr_old);
  }
  return ptr_new;
}

//=================
//void * hrealloc_m(void *ptr_old, long size_old, long size_new)
//{ void *ptr_new;
//  if(!size_old)
//  #ifdef DEBUG_MEM
//    ERR(1,"hrealloc_m");
//  #else
//    return halloc_m(size_new, 1);
//  #endif
//
//  if ( ptr_new = halloc_m(size_new, 1) )
//  {
//    hmemcopy(ptr_new, ptr_old, size_new < size_old ? size_new : size_old);
//    hfree_m(ptr_old);
//  }
//  return ptr_new;
//}

//== ‘ЋЋЃ™…Ќ€џ ЋЃ Ћ�€ЃЉЂ• ==
void ERR(int num, const char  *str)
{ char str1[60];
  WAR(num,str);
  #ifdef WIN_MOD
    wsprintf(str1,"\nERR=%d %s",num,str);
    //MessageBox((HWND) 0,str1,(char*) "ERROR",MB_ICONHAND|MB_OK);
    #ifndef DLL_MOD
      FatalExit(num);
    #endif
  #endif
}

//=================
void ERRO(int num,char *str)
{ char str1[60];
  WAR(num,str);
  #ifdef WIN_MOD
    wsprintf(str1,"\nERR=%d %s",num,str);
    MessageBox((HWND) 0,str1,(char*) "ERROR",MB_ICONHAND|MB_OK);
    #ifndef DLL_MOD
      FatalExit(num);
    #endif
  #else
        fprintf(stderr,"\nERR=%d %s",num,str);
    	exit(num);
  #endif
}

extern char NameFuncErr[100]; extern  short NumErr;

void WAR(int num, const char *str)
{
  strcpy_m(NameFuncErr,str); NumErr=num;
}

//===
void PutMess(int num, char *str)
{
  #ifndef WIN_MOD

	    fprintf(stderr, "\n%s %d",str,num);

  #else
    char str1[160];
    wsprintf(str1,"\n%s %d",str,num);
    MessageBox((HWND) 0,str1,(char*) "MESS",MB_ICONHAND|MB_OK);
  #endif
}

//------  Common functions for mrk.dll, dot.dll, ndx.dll --------

#include "undef32.h"
	#include "globus.h"
//	#include "math.h"
	#include "memfunc.h"
#include "wind32.h"
/*
//==
void ProjectPoint(Point16 *r,float tg_ang)
//==
{ int16_t xa,ya;
  float fi=(float)atan(tg_ang);
  float si=(float)sin(fi),co=(float)cos(fi);
  xa=r->x; ya=r->y; r->x=(int16_t)(xa*co+ya*si); r->y=(int16_t)(-xa*si+ya*co);
}
*/
//#undef CT_SKEW

#ifdef CT_SKEW
	#include "skew1024.h"
#endif

//==
void ProjectRect1024(Rect16 *r,int32_t Skew1024)
//==
{ int xa,ya,
  #ifndef CT_SKEW
   xc,yc,
  #endif
	 dx,dy;
  if(Skew1024)
  {
		xa=(r->right + r->left)>>1; ya=(r->top + r->bottom)>>1;
		#ifndef CT_SKEW
	    xc=xa + (int)(((int32_t)ya*Skew1024)/1024);
	    yc=ya - (int)(((int32_t)xa*Skew1024)/1024);
	    dx=xc-xa; dy=yc-ya;
		#else
			Point16 pt;
			pt.x=xa; pt.y=ya;
			Deskew(pt,-Skew1024);
	    dx=pt.x-xa; dy=pt.y-ya;
		#endif
  	r->left+=(int16_t)dx; r->right+=(int16_t)dx; r->bottom+=(int16_t)dy; r->top+=(int16_t)dy;
  }
}

//==
void ProjectPoint1024(Point16 *r,int32_t Skew1024)
//==
{
	#ifndef CT_SKEW
		int16_t xa,ya;
	  xa=r->x; ya=r->y;
	  r->x=xa + (int16_t)(((int32_t)ya*Skew1024)/1024);
	  r->y=ya - (int16_t)(((int32_t)xa*Skew1024)/1024);
	#else
		Deskew(*r,-Skew1024);
	#endif
}

#if defined (FIND_NDX) || defined (FIND_BOX) || defined (FIND_DOT)
	extern MemFunc mem;
	void* malloc_t(uint32_t size)         { return (*mem.alloc)(size); }
	void free_t(void *ptr, uint32_t size) { (*mem.free)(ptr,size); }
	void* malloc_u(uint32_t size)         {return malloc_m((uint16_t)size);}
	void free_u(void *ptr, uint32_t size) {free_m(ptr);}
#endif

#ifndef WIN_MOD

/* // !!! Art - устарело
	#include <stdio.h>
	#include <string.h>

	//GetPrivateProfileInt & GetPrivateProfileString functions emulation
	static char *fnexts_m(FILE *File, char *buf)
	{
		char *Str=fgets(buf,255,File);
		if(Str) {
		 	Str[strlen(Str)-1]=0;
		}
		return Str;
	}
	static int findname(char *sect, char *key, char *res, char *name)
	{
		FILE *File;
		uchar buf03[255];
		int16_t ret=0;

		if((File=fopen(name,"rt")) == NULL) {
			return 0;
		}

	  while (fnexts_m(File, (char*)buf03))
	  { if (*buf03 == ';' || *buf03 == 0) {
				continue;
			}
	    else if (*buf03 == '[' && buf03[strlen((char*)buf03)-1] == ']')
	    {
				buf03[strlen((char*)buf03)-1] = 0;
	      if (strcmpi((char*)(buf03+1), sect) == 0)
	      {
					while (fnexts_m(File, (char*)buf03))
	        {
						if (*buf03 == ';' || *buf03 == 0) {
							continue;
						}
	          else if (*buf03 == '[') {
							break;
						}
	          else if (buf03[strlen(key)] == '=')
	          { buf03[strlen(key)] = 0;
	            if (strcmpi((char*)buf03, key) == 0) {
								strcpy(res, (char*)buf03+strlen(key)+1);
								ret=1;
								goto EXIT;
							}
	          }
	        }
	        ret=0;
					goto EXIT;
	      }
	    }
	  }
	EXIT:
		fclose(File);
	  return ret;
	}
	#include "undef32.h"
	int GetPrivateProfileInt(char *section, char *key, int Default, char *name)
	{
		int ret = Default;
	  char buf[80];

		return findname(section, key, buf, name) ? atoi(buf) : ret;
	}
	int GetPrivateProfileString(char *section, char *key, char *Default,
			char *result,int maxsize, char *name)
	{
		strcpy(result, Default);
	  return findname(section, key, result, name) ? 1:0;
	}
*/ // !!! Art - устарело
#endif /*WIN_MOD*/



