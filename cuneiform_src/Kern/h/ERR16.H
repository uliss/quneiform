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

//#pragma message(__FILE__"(1)  :")
#ifndef __ERR16_H
#define __ERR16_H

   typedef signed short int       Err16 ;
   #ifdef _SETTYPES
      _SETTYPES( Err16  )
   #endif

   // common use error codes
   #define ER_NONE                0   // all right
   #define ER_INTERNAL            -1  // internal error
   #define ER_UNDOCUMENTED        -2  // undocumented error (no message for unknown error code)
   #define ER_NOMEMORY            -3  // lack of memory
   #define ER_NOSUCHFILE          -4  // no such file
   #define ER_DLLFAILED           -5  // cannot load dll
   #define ER_CANTOPEN            -6  // cannot open file
   #define ER_CANTREAD            -7  // cannot read file
   #define ER_CANTWRITE           -8  // cannor write file
   #define ER_DISKFULL            -9  // disk full
   #define ER_UNSUPPORTEDFORMAT   -10 // unsupported format
   #define ER_UNSUPPORTEDVERSION  -11 // unsupported version (modification)
   #define ER_UNSUPPORTEDDEVICE   -12 // unsupperted device
   #define ER_UNSUPPORTEDPLATFORM -13 // unsupported platform
   #define ER_CORRUPTED           -14 // data corrupted (file, internal structure)
   #define ER_EMPTYFEEDER         -15 // scanner feeder is empty
   #define ER_IMPROPERIMAGETYPE   -16 // improper image
   #define ER_NOTIMPLEMENTED      -17 // not implemented routine
   #define ER_CURRENTLYDISABLED   -18 // attempt to do currently disabled action
   #define ER_TOOMANYITEMS        -19 // too many items
   #define ER_BADUSAGE            -20 // wrong arguments, etc...
   #define ER_SKEWTOOBIG          -21 // too big image skew...
   #define ER_CANTCONVERT         -22 // cannot convert from source to dest.
   #define ER_CANTLOCK            -23 // some resource cannot be locked
   #define ER_CANTCREATE          -24 // cannot create some object...
   #define ER_CANTDELETE          -25 // cannot delete some object...
   #define ER_IMPROPERVALUE       -26 // some another value expected!
   #define ER_EMPTY               -27 // some value or object assumed to be not empty
   #define ER_TOOLONG             -28 // some file or string too long
   #define ER_NOFITNESS           -29 // some object does not correspond another
   #define ER_TIMEOUT             -30 // timeout !!!
   #define ER_OVERTURN            -31 // something is overturned - image, bytes,...
   #define ER_PAGETOOBLACK        -32 // f.e. bad binarization result
   #define ER_NOTREADY            -33 // abstract device still not ready
   #define ER_OVERFLOW            -34 // something overflowed
   #define ER_ACCESSDENIED        -35 // something overflowed
   #define ER_BADIMAGE            -36 //
   #define ER_BADIMAGEGEOMETRY    -37 //
   #define ER_BADIMAGEWIDTH       -38 //
   #define ER_BADIMAGEHEIGHT      -39 //
   #define ER_BADIMAGEINTENSITY   -40 //
   #define ER_CANTFIND            -41 // cannot find something
   #define ER_BUSY                -42 // object busy
   #define ER_CANTCLOSE           -43 // cant close file, stream,...
   #define ER_USERBREAK           -44 // user breaks the process...
   #define ER_FORGOTTEN           -45 // error details are forgotten - end of error queue


      // DLL specific
   #define ER_NDX_FIRST           -1024
   //
   //#define ER_USERFIRST           -1024

   #define ER_USERFIRST           -1024
      // user's error coding convention:
      // #define ER_MY_ERROR1        (ER_USERFIRST - 1)
      // #define ER_MY_ERROR2        (ER_USERFIRST - 2)
      // ...

#endif
