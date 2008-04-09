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

#include "internal.h"
#pragma hdrstop


#include "std.h"

#ifdef __BORLANDC__
#include "alloc.h"
#include "dos.h"
#endif

#ifdef _DEBUG
   static Int32 _stdMallocCounter=0;
   static Int32 _stdReallocCounter=0;
   static Int32 _stdFreeCounter=0;

   class __TestStdMem
   {  
   public:  
      ~__TestStdMem()
         {
#ifndef _NO_MEM_REPORT
			assert( _stdMallocCounter == _stdFreeCounter );
#endif
	  };
   };
   static __TestStdMem tester;

#endif // _DEBUG

STD_FUNC( void )  stdGetMemInfo( StdMemInfo* pMemInf )
{
   if (pMemInf)
   {
#ifdef _DEBUG
      pMemInf->nMallocCnt = _stdMallocCounter;
      pMemInf->nFreeCnt = _stdFreeCounter;
      pMemInf->nReallocCnt = _stdReallocCounter;
#endif // _DEBUG
   }
}


STD_FUNC( void* ) stdMalloc( Int32 size )
{

   void * res =
               #if defined( WIN32 )
                  malloc(size);
               #endif
               #if defined( PPS_WIN )
                  farmalloc(size);
               #endif
               #ifdef PPS_MAC
                  NewPtr(size);
               #endif

#ifdef _DEBUG
   if (res==NULL)
   {
      CONSOLE("stdMalloc(%d)=>NULL {%d}", size, _stdMallocCounter);
   }
   else
   {
      _stdMallocCounter++; // need free later
   }
#endif

   return res;
};

STD_FUNC( void* )  stdRealloc( void* old_ptr, Int32 new_size, Int32 old_size )
{

#ifdef _DEBUG
   _stdReallocCounter++;
#endif

   ///////////////////////////////////////////////////
   // platforms where we use native realloc ...
   #if defined( WIN32 ) || defined(PPS_WIN)
   {
      old_size=old_size;
      void *   res =
                     #if defined( WIN32 )
                        realloc(old_ptr, new_size);
                     #elif defined(PPS_WIN)
                        farrealloc(old_ptr, new_size);
                     #endif
#ifdef _DEBUG
      if (res==NULL)
      {
         CONSOLE("stdRealloc(%d)=>NULL {%d}", new_size, _stdReallocCounter);
         if (old_ptr != NULL && new_size == 0)   // <=> free
            _stdFreeCounter++;
      }
      else
      {
         if (old_ptr == NULL) // <=> malloc
            _stdMallocCounter++;
      }
#endif

      return res;
   }
   #endif

   ////////////////////////////////////////////////////////////
   // otherwise let's emulate realloc by malloc/memcpy/free:

   #if defined( PPS_MAC )  // || defined(...)
      if (old_ptr)
      {
         if (new_size)
         {
            void* new_ptr = stdMalloc(new_size);
            if (new_ptr==NULL)
               RETNULL;
            if (old_size)
               memcpy(new_ptr, old_ptr, mini(old_size, new_size));
            stdFree(old_ptr);
            return new_ptr;
         }
         else  //
         {
            stdFree(old_ptr);
            return NULL;
         }
      }
      assert(old_ptr==NULL);
      return new_size ? stdMalloc(new_size) : NULL;
   #endif
}

STD_FUNC( void  ) stdFree( void* ptr )
{
   if (ptr==NULL)
      return;
#ifdef _DEBUG
   _stdFreeCounter++;
#endif
   #if   defined( WIN32 )
      free(ptr);
   #elif defined( PPS_WIN )
      farfree(ptr);
   #elif defined( PPS_MAC )
      DisposPtr((char*)ptr);
   #endif
};

STD_FUNC( void  ) stdFreeX( void** pptr )
{
#ifdef _DEBUG
   assert(pptr);
#endif
   if (*pptr)
   {
      stdFree(*pptr);
      *pptr = NULL;
   };
}

STD_FUNC( void* ) stdMemcpy( void* dest , void* src , Int32 copy_count )
{

#ifdef _DEBUG
   assert(dest);
   assert(src);
#endif

#ifdef BITS_32
   memcpy(dest, src, copy_count);
   return dest;

#else // BITS_16
   Word8 HUGE * Src =(Word8 HUGE *)src;
   Word8 HUGE * Dst =(Word8 HUGE *)dest;
   Word16    ToCopy;

   while (copy_count > 0)
   {
      ToCopy = mini(copy_count, mini(0xffff -FP_OFF(Src), 0xffff -FP_OFF(Dst)));
      if(ToCopy)
      {
         memcpy(Dst, Src, ToCopy);
         Src +=ToCopy;
         Dst +=ToCopy;
         copy_count -=ToCopy;
      }
      if(copy_count)
      {
         *Dst++ =*Src++;
         copy_count--;
      }
   }
   return dest;
#endif
}
