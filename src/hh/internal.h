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

#ifndef __INTERNAL_H
   #define __INTERNAL_H

   #if (defined( _Windows ) || defined( _WINDOWS ) || defined( WIN32 ) )
      #ifdef WIN32
         #ifndef PPS_WIN32
            #define PPS_WIN32
         #endif
      #else
         #ifndef PPS_WIN
            #define PPS_WIN
            #define PPS_WIN16 // 16-bit Windows
	    #define WIN16
         #endif
      #endif
   #endif

   #if !defined(__STDDEF_H) && !defined(_INC_STDDEF)
      #include <stddef.h>
   #endif

   #if !defined(__STDIO_H) && !defined(_INC_STDIO)
      #include <stdio.h>
   #endif

   #if !defined(__STDLIB_H) && !defined(_INC_STDLIB)
      #include <stdlib.h>
   #endif

   #if !defined(__MATH_H) && !defined(_INC_MATH)
      #include <math.h>
   #endif

   #if !defined(__STRING_H) && !defined(_STRING_)
      #include <string.h>
   #endif

   #ifdef __BORLANDC__
      #define BITS_16
   #else
      #define BITS_32
   #endif

   #if defined( PPS_WIN ) || defined( PPS_WIN32 )
      #ifdef __BORLANDC__
      #  ifndef __WINDOWS_H
      #        include <windows.h>     // 16 bit, borland assumed
      #  endif
      #else //  assumed 32 bit && VC
      #  ifndef _WINDOWS_
      #     define WIN32_EXTRA_LEAN
      #     define WIN32_LEAN_AND_MEAN

      #     define NOSERVICE // no service control manager, include "winsvc.h" directly if need
      #     define NOIME     // no input method manager, include "ime.h" directly if need
      #     define NOMCX     // no modem extensions, include "mcx.h" directly if need

//      #     include <windows.h>
      #     include "win_mini.h"
      #  endif
      #endif
   #endif

   #ifndef __GLOBUS_H
      #include "globus.h"
   #endif

   //== to be precompiled
   #ifndef __STD_H
      #include "std.h"
   #endif

   #define CONSOLE   stdConsole
   #define CONSOLE_is_stdConsole
   #define CONSOLE_IF( statm )   if(statm) stdConsole

   #if defined(assert)
   #undef assert
   #endif
   #ifdef NDEBUG
      #define assert(p) ((void)0)
   #else
      #define assert(p) \
         ((p)) ? (void)0 : (void)stdAssert( #p, __FILE__, __LINE__)
   #endif
   #define assert_is_stdAssert

   #define ifret( cond ) \
      {  if (!(cond)) { stdSetError( ER_INTERNAL, #cond, __FILE__, __LINE__); return; } }

   #define ifret0( cond ) \
      {  if (!(cond)) { stdSetError( ER_INTERNAL, #cond, __FILE__, __LINE__); return 0; } }

   #ifndef __XMACROS_H
   #include "xmacros.h"
   #endif

   #ifndef __SAFESTR_H
   #include "safestr.h"
   #endif

   #ifndef __WMB_H
   #include "wmb.h"
   #endif

#endif  // INTERNAL
