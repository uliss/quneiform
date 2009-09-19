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

#ifndef __DLLTYPES__
#define __DLLTYPES__

#ifdef WIN32
  #ifndef __NT__
    #define __NT__
  #endif
#endif

#ifdef __NT__
  // for Windows NT

  #undef PASCAL
  #undef EXPORT
  #if defined( _MSC_VER ) && _MSC_VER > 800
    #define  dCDECL(typ)     typ __cdecl
    #define  dEXPORT(typ)    __declspec( dllexport ) typ
    #define  dIMPORT(typ)    __declspec( dllimport ) typ
   #elif defined(__WATCOMC__) && __WATCOMC__ > 1000
    #define  dCDECL(typ)     typ __cdecl
    #define  dEXPORT(typ)    typ __export
    #define  dIMPORT(typ)    typ __export
   #elif
    #error "Unknow function types for this compiler."
  #endif

  #define TIGERFAR
  #define TIGERFUN(typ)  dCDECL(typ)

  #if defined( FILE_TIGERAPI )
    #define TIGERAPI(typ)      TIGERFUN(dEXPORT(typ))
    #define TIGERCALLBACK(typ) TIGERFUN(dIMPORT(typ))
   #else
    #define TIGERAPI(typ)      TIGERFUN(dIMPORT(typ))
    #define TIGERCALLBACK(typ) TIGERFUN(dEXPORT(typ))
  #endif

 #else
// for Windows 3.x
  #ifndef EXPORT
    #define EXPORT _export
  #endif

  #define TIGERFAR            
  #define TIGERFUN(typ)       typ PASCAL
  #define TIGERAPI(typ)       TIGERFUN(typ) EXPORT
  #define TIGERCALLBACK(typ)  TIGERAPI(typ)
#endif

typedef short int INT16,   *PINT16, TIGERFAR *LPINT16;
typedef short int BOOL16,  *PBOOL16, TIGERFAR *LPBOOL16;
//AK
//typedef short int INT32,   *PINT32, TIGERFAR *LPINT32;
typedef short int BOOL32,  *PBOOL32, TIGERFAR *LPBOOL32;


// for exported functions
#define TGBOOL16  TIGERAPI(BOOL16)
#define TGBOOL  TIGERAPI(Bool)
#define TGWORD  TIGERAPI(uint16_t)
#define TGLPSTR TIGERAPI(char*)
#define TGDWORD TIGERAPI(DWORD)
#define TGVOID  TIGERAPI(void)
// for internal functions and connect description
#define FNBOOL16  TIGERFUN(BOOL16)
#define FNBOOL  TIGERFUN(Bool)
#define FNWORD  TIGERFUN(uint16_t)
#define FNLPSTR TIGERFUN(char*)
#define FNDWORD TIGERFUN(DWORD)
#define FNVOID  TIGERFUN(void)
// for callback functions
#define CBBOOL  TIGERCALLBACK(Bool)
#define CBWORD  TIGERCALLBACK(uint16_t)
#define CBLPSTR TIGERCALLBACK(char*)
#define CBDWORD TIGERCALLBACK(DWORD)
#define CBVOID  TIGERCALLBACK(void)

#endif
