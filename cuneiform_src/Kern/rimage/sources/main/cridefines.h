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

# ifndef __CRIMAGE_DEFINES_H_
# define __CRIMAGE_DEFINES_H_
//////////////////////////////////////////////////////////////////////////////////////////////////////////
//

//# define    char               char
//# define    char*              char *
/*
# define    CTIBOOL16          Bool16
# define    CTIWORD            uint16_t
*/
#define     RIMAGE_MAX_IMAGE_NAME           256
//////////////////////////////////////////////////////////////////////////////////////////////////////////
//
#define     RIMAGE_RGBQUDSSIZE(a, b)        a += ( (0x0001 << b) * sizeof(RIMAGERGBQUAD) )
//////////////////////////////////////////////////////////////////////////////////////////////////////////
//
# define    RIMAGE_EXTENDBYTES(a,b)         (((a) + (b) - 1)/(b))*(b)
# define    RIMAGE_EXTENDBITS(a,b)          (((a) + ((b)*8) - 1)/((b)*8))*(b)
# define    RIMAGE_DIB_EXT                  4
//////////////////////////////////////////////////////////////////////////////////////////////////////////
//
# define    RIMAGE_INCH_TO_METER(a)         ( ( (a) / 2.54 ) * 100 )
//////////////////////////////////////////////////////////////////////////////////////////////////////////
//
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
# define    RIMAGE_STRING_LENGHT(a)         strlen(a)
# define    RIMAGE_STRING_COPY(a,b)         strcpy(a,b)
# define    RIMAGE_STRING_N_COPY(a,b,c)     strncpy(a,b,c)
# define    RIMAGE_STRING_CAT(a,b)          strcat(a,b)
# define    RIMAGE_STRING_COMPARE(a,b)      strcmp(a,b)
# define    RIMAGE_ABS(a)                   abs(a)
# define    RIMAGE_MEMSET(a,b,c)            memset(a,b,c)
# define    RIMAGE_MEMCOPY(a,b,c)           memcpy(a,b,c)
//////////////////////////////////////////////////////////////////////////////////////////////////////////
//
#if !defined DLL_PROCESS_ATTACH || !defined DLL_THREAD_ATTACH || !defined DLL_THREAD_DETACH || !defined DLL_PROCESS_DETACH
#define DLL_PROCESS_ATTACH 1
#define DLL_THREAD_ATTACH  2
#define DLL_THREAD_DETACH  3
#define DLL_PROCESS_DETACH 0
#endif
//////////////////////////////////////////////////////////////////////////////////////////////////////////
//
#ifndef APIENTRY
#ifdef WIN32
# define APIENTRY     __stdcall
#endif
#endif
//////////////////////////////////////////////////////////////////////////////////////////////////////////
//
#if !defined Bool
typedef int                 Bool;
#endif
//////////////////////////////////////////////////////////////////////////////////////////////////////////
//
# endif //__CRIMAGE_DEFINES_H_
//////////////////////////////////////////////////////////////////////////////////////////////////////////
// end of file
