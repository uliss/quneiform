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
void __CopyOR(uchar* dst, const uchar* src, const int bytes_cnt) {
	for(int i=0; i<bytes_cnt; i++) {
		*dst++ |= *src++;
	}
}

MEMOPS_INLINE
void CopyOR(void* dst, const void* src, const unsigned short dword_cnt) {
	__CopyOR((uchar*)dst, (uchar*)src, 4*(int)dword_cnt);
}

MEMOPS_INLINE
void __CopyAND(uchar* dst, const uchar* src, const int bytes_cnt) {
	for(int i=0; i<bytes_cnt; i++) {
		*dst++ &= *src++;
	}
}

MEMOPS_INLINE
void CopyAND(void* dst, const void* src, const unsigned short dword_cnt) {
	__CopyAND((uchar*)dst, (uchar*)src, 4*(int)dword_cnt);
}

MEMOPS_INLINE
void __InvertSelf(uchar* dst, const int bytes_cnt) {
	for(int i=0; i<bytes_cnt; i++) {
		*dst++ = ~*dst;
	}
}

MEMOPS_INLINE
void InvertSelf(void* dst, const unsigned short dword_cnt) {
	__InvertSelf((uchar*)dst, 4*(int)dword_cnt);
}

MEMOPS_INLINE
void InvertTo(void* dst, const void* src, const unsigned short dword_cnt) {
	unsigned char *ldst = (unsigned char*)dst;
	unsigned char *lsrc = (unsigned char*)src;
	for(int i=0; i<4*(int)dword_cnt; i++) {
		*ldst++ = ~*lsrc++;
	}
}



#endif
#endif
#endif
