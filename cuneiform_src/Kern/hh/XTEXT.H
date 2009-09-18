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

#ifndef __XTEXT_H
#define __XTEXT_H

#include <stdarg.h>

#ifndef __CTTYPES_H
#include "cttypes.h"
#endif

template <int nSize>
class XText
{
public:
   char buf[nSize];
   int  dummy;
   XText(void){ buf[0]=0; dummy = 0; };
   XText(const char* str){ if (str) strcpy( buf, str ); dummy = 0; };
   XText& sprintf(const char* format, ...)
   {  va_list list;
      va_start(list, format);
      vsprintf( buf, format, list );
      va_end( list );
      return *this;
   }

   ~XText()
   {
#ifdef _DEBUG
//      if (dummy != 0 || buf[nSize-1] != 0)
//         asm int 3;
#endif
   };

   operator char* (void) const { return (char*)buf; };
   Bool32 operator !(void) const { return buf[0] == 0; };

   XText& operator = ( const char* str ) { strcpy(buf, str);    return *this; };
   XText& operator += ( const char* str ) { strcat(buf, str);    return *this; };
   XText  operator + ( const char* str ) { return XText(*this) += str; };

   Bool32 operator == ( const char* str ) const { return strcmp(buf, str)==0; };
   Bool32 operator == (       char* str ) const { return strcmp(buf, str)==0; };

   Bool32 operator != ( const char* str ) const { return strcmp(buf, str)!=0; };
   Bool32 operator != (       char* str ) const { return strcmp(buf, str)!=0; };

   Bool32 operator > ( const char* str ) const { return strcmp(buf, str) > 0; };
   Bool32 operator > (       char* str ) const { return strcmp(buf, str) > 0; };

   Bool32 operator < ( const char* str ) const { return strcmp(buf, str) < 0; };
   Bool32 operator < (       char* str ) const { return strcmp(buf, str) < 0; };

   int    GetBufSize(void){ return nSize; };

///////////////
// ??
//   operator int(void) const { return atoi(buf); };
//   int    strlen(void){ return ::strlen(buf); };
//   XText& operator = ( XText& xt ) { strcpy(buf, xt.buf); return *this; };
};

class XPChar
{
public:
   char * ptr;
   XPChar(void){ ptr=NULL; };
   XPChar(char* p){ ptr=p; };
   operator char * (void) const { return ptr; };
   Bool32 NotEmpty(void) { return ptr!=NULL && ptr[0] != 0; };

/* comfortable, but may be not safe in use...
   XPChar& operator = ( char* str ) { strcpy(ptr, str);    return *this; };
   XPChar& operator += ( char* str ) { strcat(ptr, str);    return *this; };
   Bool32 operator == ( char* str ) { return strcmp(ptr, str)==0; };
   Bool32 operator != ( char* str ) { return strcmp(ptr, str)!=0; };
   Bool32 operator > ( char* str ) { return strcmp(ptr, str) > 0; };
   Bool32 operator < ( char* str ) { return strcmp(ptr, str) < 0; };
*/

};

#endif //__XTEXT_H
