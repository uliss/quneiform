/*
Copyright (c) 1993-2008, Cognitive Technologies
All rights reserved.

Ğàçğåøàåòñÿ ïîâòîğíîå ğàñïğîñòğàíåíèå è èñïîëüçîâàíèå êàê â âèäå èñõîäíîãî êîäà,
òàê è â äâîè÷íîé ôîğìå, ñ èçìåíåíèÿìè èëè áåç, ïğè ñîáëşäåíèè ñëåäóşùèõ óñëîâèé:

      * Ïğè ïîâòîğíîì ğàñïğîñòğàíåíèè èñõîäíîãî êîäà äîëæíû îñòàâàòüñÿ óêàçàííîå
        âûøå óâåäîìëåíèå îá àâòîğñêîì ïğàâå, ıòîò ñïèñîê óñëîâèé è ïîñëåäóşùèé
        îòêàç îò ãàğàíòèé.
      * Ïğè ïîâòîğíîì ğàñïğîñòğàíåíèè äâîè÷íîãî êîäà â äîêóìåíòàöèè è/èëè â
        äğóãèõ ìàòåğèàëàõ, ïîñòàâëÿåìûõ ïğè ğàñïğîñòğàíåíèè, äîëæíû ñîõğàíÿòüñÿ
        óêàçàííàÿ âûøå èíôîğìàöèÿ îá àâòîğñêîì ïğàâå, ıòîò ñïèñîê óñëîâèé è
        ïîñëåäóşùèé îòêàç îò ãàğàíòèé.
      * Íè íàçâàíèå Cognitive Technologies, íè èìåíà åå ñîòğóäíèêîâ íå ìîãóò
        áûòü èñïîëüçîâàíû â êà÷åñòâå ñğåäñòâà ïîääåğæêè è/èëè ïğîäâèæåíèÿ
        ïğîäóêòîâ, îñíîâàííûõ íà ıòîì ÏÎ, áåç ïğåäâàğèòåëüíîãî ïèñüìåííîãî
        ğàçğåøåíèÿ.

İÒÀ ÏĞÎÃĞÀÌÌÀ ÏĞÅÄÎÑÒÀÂËÅÍÀ ÂËÀÄÅËÜÖÀÌÈ ÀÂÒÎĞÑÊÈÕ ÏĞÀÂ È/ÈËÈ ÄĞÓÃÈÌÈ ËÈÖÀÌÈ "ÊÀÊ
ÎÍÀ ÅÑÒÜ" ÁÅÇ ÊÀÊÎÃÎ-ËÈÁÎ ÂÈÄÀ ÃÀĞÀÍÒÈÉ, ÂÛĞÀÆÅÍÍÛÕ ßÂÍÎ ÈËÈ ÏÎÄĞÀÇÓÌÅÂÀÅÌÛÕ,
ÂÊËŞ×Àß ÃÀĞÀÍÒÈÈ ÊÎÌÌÅĞ×ÅÑÊÎÉ ÖÅÍÍÎÑÒÈ È ÏĞÈÃÎÄÍÎÑÒÈ ÄËß ÊÎÍÊĞÅÒÍÎÉ ÖÅËÈ, ÍÎ ÍÅ
ÎÃĞÀÍÈ×ÈÂÀßÑÜ ÈÌÈ. ÍÈ ÂËÀÄÅËÅÖ ÀÂÒÎĞÑÊÈÕ ÏĞÀÂ È ÍÈ ÎÄÍÎ ÄĞÓÃÎÅ ËÈÖÎ, ÊÎÒÎĞÎÅ
ÌÎÆÅÒ ÈÇÌÅÍßÒÜ È/ÈËÈ ÏÎÂÒÎĞÍÎ ĞÀÑÏĞÎÑÒĞÀÍßÒÜ ÏĞÎÃĞÀÌÌÓ, ÍÈ Â ÊÎÅÌ ÑËÓ×ÀÅ ÍÅ
ÍÅÑ¨Ò ÎÒÂÅÒÑÒÂÅÍÍÎÑÒÈ, ÂÊËŞ×Àß ËŞÁÛÅ ÎÁÙÈÅ, ÑËÓ×ÀÉÍÛÅ, ÑÏÅÖÈÀËÜÍÛÅ ÈËÈ
ÏÎÑËÅÄÎÂÀÂØÈÅ ÓÁÛÒÊÈ, ÑÂßÇÀÍÍÛÅ Ñ ÈÑÏÎËÜÇÎÂÀÍÈÅÌ ÈËÈ ÏÎÍÅÑÅÍÍÛÅ ÂÑËÅÄÑÒÂÈÅ
ÍÅÂÎÇÌÎÆÍÎÑÒÈ ÈÑÏÎËÜÇÎÂÀÍÈß ÏĞÎÃĞÀÌÌÛ (ÂÊËŞ×Àß ÏÎÒÅĞÈ ÄÀÍÍÛÕ, ÈËÈ ÄÀÍÍÛÅ,
ÑÒÀÂØÈÅ ÍÅÃÎÄÍÛÌÈ, ÈËÈ ÓÁÛÒÊÈ È/ÈËÈ ÏÎÒÅĞÈ ÄÎÕÎÄÎÂ, ÏÎÍÅÑÅÍÍÛÅ ÈÇ-ÇÀ ÄÅÉÑÒÂÈÉ
ÒĞÅÒÜÈÕ ËÈÖ È/ÈËÈ ÎÒÊÀÇÀ ÏĞÎÃĞÀÌÌÛ ĞÀÁÎÒÀÒÜ ÑÎÂÌÅÑÒÍÎ Ñ ÄĞÓÃÈÌÈ ÏĞÎÃĞÀÌÌÀÌÈ,
ÍÎ ÍÅ ÎÃĞÀÍÈ×ÈÂÀßÑÜ İÒÈÌÈ ÑËÓ×ÀßÌÈ), ÍÎ ÍÅ ÎÃĞÀÍÈ×ÈÂÀßÑÜ ÈÌÈ, ÄÀÆÅ ÅÑËÈ ÒÀÊÎÉ
ÂËÀÄÅËÅÖ ÈËÈ ÄĞÓÃÎÅ ËÈÖÎ ÁÛËÈ ÈÇÂÅÙÅÍÛ Î ÂÎÇÌÎÆÍÎÑÒÈ ÒÀÊÈÕ ÓÁÛÒÊÎÂ È ÏÎÒÅĞÜ.

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

#ifndef __MEMOPS_H
#define __MEMOPS_H
#ifdef __cplusplus

/*#ifndef NO_ASM*/
#if 0
   extern "C"
   {
//#endif
      // realized in 'memops.asm'
      void CopyOR(         void*             dst,
                           void*             src,
                           unsigned short    dword_cnt
                 );
      void CopyAND(         void*             dst,
                           void*             src,
                           unsigned short    dword_cnt
                 );

      #undef memcpy
      void* memcpy( void*          dst,
                    const void*    src,
                    unsigned int   byte_cnt
                  );

      void InvertTo( void*   dst,
                     void*   src,
                     unsigned short dwcnt
                   );

      void InvertSelf(  void*   dst,
                        unsigned short dwcnt
                     );
   #ifdef __cplusplus
   }
   #endif

#else // ============ NO_ASM ============

   #ifndef MEMOPS_INLINE
   #define MEMOPS_INLINE inline
      // you can use here 'static' instead
      // if you have some problems with 'inline'
   #endif

MEMOPS_INLINE
void CopyOR(         void*             dst,
                     void*             src,
							unsigned short    dword_cnt
           )
   {
      register long* ldst = (long*)dst;
      register long* lsrc = (long*)src;
      int cnt = dword_cnt >> 3;
      while ( cnt-- )
      {
         *ldst++ |= *lsrc++;  *ldst++ |= *lsrc++;
         *ldst++ |= *lsrc++;  *ldst++ |= *lsrc++;
         *ldst++ |= *lsrc++;  *ldst++ |= *lsrc++;
         *ldst++ |= *lsrc++;  *ldst++ |= *lsrc++;
      };
      cnt = dword_cnt & 7;
      while ( cnt-- )
      {
         *ldst++ |= *lsrc++;
      };
   };

MEMOPS_INLINE
void CopyAND(         void*             dst,
                     void*             src,
							unsigned short    dword_cnt
           )
   {
      register long* ldst = (long*)dst;
      register long* lsrc = (long*)src;
      int cnt = dword_cnt >> 3;
      while ( cnt-- )
      {
         *ldst++ &= *lsrc++;  *ldst++ &= *lsrc++;
         *ldst++ &= *lsrc++;  *ldst++ &= *lsrc++;
         *ldst++ &= *lsrc++;  *ldst++ &= *lsrc++;
         *ldst++ &= *lsrc++;  *ldst++ &= *lsrc++;
      };
      cnt = dword_cnt & 7;
      while ( cnt-- )
      {
         *ldst++ &= *lsrc++;
      };
   };

MEMOPS_INLINE
void __CopyAND(      Word8*            dst,
                     Word8*            src,
							int               bytes_cnt
           )
   {
      register long* ldst = (long*)dst;
      register long* lsrc = (long*)src;
      int cnt = bytes_cnt >> 5;
      while ( cnt-- )
      {
         *ldst++ &= *lsrc++;  *ldst++ &= *lsrc++;
         *ldst++ &= *lsrc++;  *ldst++ &= *lsrc++;
         *ldst++ &= *lsrc++;  *ldst++ &= *lsrc++;
         *ldst++ &= *lsrc++;  *ldst++ &= *lsrc++;
      };
      dst += bytes_cnt & ~31;
      src += bytes_cnt & ~31;
      cnt = bytes_cnt & 31;
      while ( cnt-- )
      {
         *dst++ &= *src++;
      };
   };

MEMOPS_INLINE
void __CopyOR(       Word8*            dst,
                     Word8*            src,
							int               bytes_cnt
             )
   {
      register long* ldst = (long*)dst;
      register long* lsrc = (long*)src;
      int cnt = bytes_cnt >> 5;
      while ( cnt-- )
      {
         *ldst++ |= *lsrc++;  *ldst++ |= *lsrc++;
         *ldst++ |= *lsrc++;  *ldst++ |= *lsrc++;
         *ldst++ |= *lsrc++;  *ldst++ |= *lsrc++;
         *ldst++ |= *lsrc++;  *ldst++ |= *lsrc++;
      };
      dst += bytes_cnt & ~31;
      src += bytes_cnt & ~31;
      cnt = bytes_cnt & 31;
      while ( cnt-- )
      {
         *dst++ |= *src++;
      };
   };


   /* if NO_ASM - use standart memcpy()
   #undef memcpy
   void* memcpy( void*          dst,
                 const void*    src,
                 unsigned int   byte_cnt
               );
   */

MEMOPS_INLINE
void InvertTo( void*   dst,
					void*   src,
					unsigned short dword_cnt
             )
   {
      register long* ldst = (long*)dst;
      register long* lsrc = (long*)src;
      int cnt = dword_cnt >> 3;
      while ( cnt-- )
      {
         *ldst++ = ~*lsrc++;  *ldst++ = ~*lsrc++;
         *ldst++ = ~*lsrc++;  *ldst++ = ~*lsrc++;
         *ldst++ = ~*lsrc++;  *ldst++ = ~*lsrc++;
         *ldst++ = ~*lsrc++;  *ldst++ = ~*lsrc++;
      };
      cnt = dword_cnt & 7;
      while ( cnt-- )
      {
         *ldst++ = ~*lsrc++;
      };
   };

MEMOPS_INLINE
void InvertSelf(  void*   dst,
						unsigned short dword_cnt
               )
   {
      register long* ldst = (long*)dst;
      int cnt = dword_cnt >> 3;
      while ( cnt-- )
      {
         *ldst++ = ~*ldst;  *ldst++ = ~*ldst;
         *ldst++ = ~*ldst;  *ldst++ = ~*ldst;
         *ldst++ = ~*ldst;  *ldst++ = ~*ldst;
         *ldst++ = ~*ldst;  *ldst++ = ~*ldst;
      };
      cnt = dword_cnt & 7;
      while ( cnt-- )
      {
         *ldst++ = ~*ldst;
      };
   };

MEMOPS_INLINE
void __InvertSelf(   Word8*   dst,
						   int bytes_cnt
                 )
   {
      register long* ldst = (long*)dst;
      int cnt = bytes_cnt >> 5;
      while ( cnt-- )
      {
         *ldst++ = ~*ldst;  *ldst++ = ~*ldst;
         *ldst++ = ~*ldst;  *ldst++ = ~*ldst;
         *ldst++ = ~*ldst;  *ldst++ = ~*ldst;
         *ldst++ = ~*ldst;  *ldst++ = ~*ldst;
      };
      dst += bytes_cnt & ~31;
      cnt = bytes_cnt & 31;
      while ( cnt-- )
      {
         *dst++ = ~*dst;
      };
   };

#endif
#endif
#endif
