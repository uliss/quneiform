/*
Copyright (c) 1993-2008, Cognitive Technologies
All rights reserved.

Ðàçðåøàåòñÿ ïîâòîðíîå ðàñïðîñòðàíåíèå è èñïîëüçîâàíèå êàê â âèäå èñõîäíîãî êîäà,
òàê è â äâîè÷íîé ôîðìå, ñ èçìåíåíèÿìè èëè áåç, ïðè ñîáëþäåíèè ñëåäóþùèõ óñëîâèé:

      * Ïðè ïîâòîðíîì ðàñïðîñòðàíåíèè èñõîäíîãî êîäà äîëæíû îñòàâàòüñÿ óêàçàííîå
        âûøå óâåäîìëåíèå îá àâòîðñêîì ïðàâå, ýòîò ñïèñîê óñëîâèé è ïîñëåäóþùèé
        îòêàç îò ãàðàíòèé.
      * Ïðè ïîâòîðíîì ðàñïðîñòðàíåíèè äâîè÷íîãî êîäà â äîêóìåíòàöèè è/èëè â
        äðóãèõ ìàòåðèàëàõ, ïîñòàâëÿåìûõ ïðè ðàñïðîñòðàíåíèè, äîëæíû ñîõðàíÿòüñÿ
        óêàçàííàÿ âûøå èíôîðìàöèÿ îá àâòîðñêîì ïðàâå, ýòîò ñïèñîê óñëîâèé è
        ïîñëåäóþùèé îòêàç îò ãàðàíòèé.
      * Íè íàçâàíèå Cognitive Technologies, íè èìåíà åå ñîòðóäíèêîâ íå ìîãóò
        áûòü èñïîëüçîâàíû â êà÷åñòâå ñðåäñòâà ïîääåðæêè è/èëè ïðîäâèæåíèÿ
        ïðîäóêòîâ, îñíîâàííûõ íà ýòîì ÏÎ, áåç ïðåäâàðèòåëüíîãî ïèñüìåííîãî
        ðàçðåøåíèÿ.

ÝÒÀ ÏÐÎÃÐÀÌÌÀ ÏÐÅÄÎÑÒÀÂËÅÍÀ ÂËÀÄÅËÜÖÀÌÈ ÀÂÒÎÐÑÊÈÕ ÏÐÀÂ È/ÈËÈ ÄÐÓÃÈÌÈ ËÈÖÀÌÈ "ÊÀÊ
ÎÍÀ ÅÑÒÜ" ÁÅÇ ÊÀÊÎÃÎ-ËÈÁÎ ÂÈÄÀ ÃÀÐÀÍÒÈÉ, ÂÛÐÀÆÅÍÍÛÕ ßÂÍÎ ÈËÈ ÏÎÄÐÀÇÓÌÅÂÀÅÌÛÕ,
ÂÊËÞ×Àß ÃÀÐÀÍÒÈÈ ÊÎÌÌÅÐ×ÅÑÊÎÉ ÖÅÍÍÎÑÒÈ È ÏÐÈÃÎÄÍÎÑÒÈ ÄËß ÊÎÍÊÐÅÒÍÎÉ ÖÅËÈ, ÍÎ ÍÅ
ÎÃÐÀÍÈ×ÈÂÀßÑÜ ÈÌÈ. ÍÈ ÂËÀÄÅËÅÖ ÀÂÒÎÐÑÊÈÕ ÏÐÀÂ È ÍÈ ÎÄÍÎ ÄÐÓÃÎÅ ËÈÖÎ, ÊÎÒÎÐÎÅ
ÌÎÆÅÒ ÈÇÌÅÍßÒÜ È/ÈËÈ ÏÎÂÒÎÐÍÎ ÐÀÑÏÐÎÑÒÐÀÍßÒÜ ÏÐÎÃÐÀÌÌÓ, ÍÈ Â ÊÎÅÌ ÑËÓ×ÀÅ ÍÅ
ÍÅÑ¨Ò ÎÒÂÅÒÑÒÂÅÍÍÎÑÒÈ, ÂÊËÞ×Àß ËÞÁÛÅ ÎÁÙÈÅ, ÑËÓ×ÀÉÍÛÅ, ÑÏÅÖÈÀËÜÍÛÅ ÈËÈ
ÏÎÑËÅÄÎÂÀÂØÈÅ ÓÁÛÒÊÈ, ÑÂßÇÀÍÍÛÅ Ñ ÈÑÏÎËÜÇÎÂÀÍÈÅÌ ÈËÈ ÏÎÍÅÑÅÍÍÛÅ ÂÑËÅÄÑÒÂÈÅ
ÍÅÂÎÇÌÎÆÍÎÑÒÈ ÈÑÏÎËÜÇÎÂÀÍÈß ÏÐÎÃÐÀÌÌÛ (ÂÊËÞ×Àß ÏÎÒÅÐÈ ÄÀÍÍÛÕ, ÈËÈ ÄÀÍÍÛÅ,
ÑÒÀÂØÈÅ ÍÅÃÎÄÍÛÌÈ, ÈËÈ ÓÁÛÒÊÈ È/ÈËÈ ÏÎÒÅÐÈ ÄÎÕÎÄÎÂ, ÏÎÍÅÑÅÍÍÛÅ ÈÇ-ÇÀ ÄÅÉÑÒÂÈÉ
ÒÐÅÒÜÈÕ ËÈÖ È/ÈËÈ ÎÒÊÀÇÀ ÏÐÎÃÐÀÌÌÛ ÐÀÁÎÒÀÒÜ ÑÎÂÌÅÑÒÍÎ Ñ ÄÐÓÃÈÌÈ ÏÐÎÃÐÀÌÌÀÌÈ,
ÍÎ ÍÅ ÎÃÐÀÍÈ×ÈÂÀßÑÜ ÝÒÈÌÈ ÑËÓ×ÀßÌÈ), ÍÎ ÍÅ ÎÃÐÀÍÈ×ÈÂÀßÑÜ ÈÌÈ, ÄÀÆÅ ÅÑËÈ ÒÀÊÎÉ
ÂËÀÄÅËÅÖ ÈËÈ ÄÐÓÃÎÅ ËÈÖÎ ÁÛËÈ ÈÇÂÅÙÅÍÛ Î ÂÎÇÌÎÆÍÎÑÒÈ ÒÀÊÈÕ ÓÁÛÒÊÎÂ È ÏÎÒÅÐÜ.

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

char NameFuncErr[100],Buff[60]; short NumErr;

void heapstat(char *mess)
{
 #ifdef _MSC_VER
  #ifndef __cplusplus
    int   status = _heapchk(), n;
  #else
    int   status = _fheapchk(), n;
  #endif
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

    default:            // ¥à á¯®§­ ­­ ï ®è¨¡ª 
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
char huge * PASC  halloc_m(long n, uint size)
{ char *err="halloc_m";

  #ifdef WIN_MOD
    #ifndef WIN32
      HANDLE h;
      if (!(h=GlobalAlloc(GHND, n * size))) return NULL;
      return GlobalLock(h);
    #else
      #ifdef DEBUG_MEM
        char huge* p;
        if(!size)ERR(1,err);
        heapstat("halloc_m bef");
        p=(char*)malloc(n*size);
        heapstat("halloc_m aft");
        return p;
      #else
       return (char*) malloc((Int32)n * size);
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
void   PASC hfree_m(void huge *ptr)
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

//®á¢®¡®¦¤¥­¨¥ á ¯à¥¤¢ à¨â. ¯à®¢¥àª®© ­  NULL
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
    /* ’ ª âà¥¡ã¥â áâ ­¤ àâ realloc'  */
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
//void huge * hrealloc_m(void *ptr_old, long size_old, long size_new)
//{ void huge *ptr_new;
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

//== ‘ŽŽ™…ˆŸ Ž Ž˜ˆŠ€• ==
void ERR(int num, const char  *str)
{ char str1[60];
  WAR(num,str);
  #ifdef WIN_MOD
    wsprintf(str1,"\nERR=%d %s",num,str);
    //MessageBox((HWND) 0,str1,(LPSTR) "ERROR",MB_ICONHAND|MB_OK);
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
    MessageBox((HWND) 0,str1,(LPSTR) "ERROR",MB_ICONHAND|MB_OK);
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
    MessageBox((HWND) 0,str1,(LPSTR) "MESS",MB_ICONHAND|MB_OK);
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
{ Int16 xa,ya;
  float fi=(float)atan(tg_ang);
  float si=(float)sin(fi),co=(float)cos(fi);
  xa=r->x; ya=r->y; r->x=(Int16)(xa*co+ya*si); r->y=(Int16)(-xa*si+ya*co);
}
*/
//#undef CT_SKEW

#ifdef CT_SKEW
	#include "skew1024.h"
#endif

//==
void ProjectRect1024(Rect16 *r,Int32 Skew1024)
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
	    xc=xa + (int)(((Int32)ya*Skew1024)/1024);
	    yc=ya - (int)(((Int32)xa*Skew1024)/1024);
	    dx=xc-xa; dy=yc-ya;
		#else
			Point16 pt;
			pt.x=xa; pt.y=ya;
			Deskew(pt,-Skew1024);
	    dx=pt.x-xa; dy=pt.y-ya;
		#endif
  	r->left+=(Int16)dx; r->right+=(Int16)dx; r->bottom+=(Int16)dy; r->top+=(Int16)dy;
  }
}

//==
void ProjectPoint1024(Point16 *r,Int32 Skew1024)
//==
{
	#ifndef CT_SKEW
		Int16 xa,ya;
	  xa=r->x; ya=r->y;
	  r->x=xa + (Int16)(((Int32)ya*Skew1024)/1024);
	  r->y=ya - (Int16)(((Int32)xa*Skew1024)/1024);
	#else
		Deskew(*r,-Skew1024);
	#endif
}

#if defined (FIND_NDX) || defined (FIND_BOX) || defined (FIND_DOT)
	extern MemFunc mem;
	void* malloc_t(Word32 size)         { return (*mem.alloc)(size); }
	void free_t(void *ptr, Word32 size) { (*mem.free)(ptr,size); }
	void* malloc_u(Word32 size)         {return malloc_m((WORD)size);}
	void free_u(void *ptr, Word32 size) {free_m(ptr);}
#endif

#ifndef WIN_MOD

/* // !!! Art - óñòàðåëî
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
		BYTE buf03[255];
		Int16 ret=0;

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
*/ // !!! Art - óñòàðåëî
#endif /*WIN_MOD*/



