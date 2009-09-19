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

#ifndef __EXCSTRUC_H
   #define __EXCSTRUC_H

#ifndef __CTTYPES_H
#include "cttypes.h"
#endif
/*
#ifndef __EVNDEFS_H
#include "evndefs.h"
#endif
*/
#pragma pack(push,1) // Byte align

#define REXC_VERSION_CODE   1
// c_comp in H\struct.h OLD TIGER
//-------------------- component in kit --------------------------
typedef struct tagExtComponent
{
 uint16_t size;             // size of component in bytes (head + body)
                          // NOTE: in tiger.dll - in paragraphs >=3
                          // can be used to step to next comp
 /////////////////////////////////// in image coords, not ideal
 int16_t upper;             // upper boundary of component
 int16_t left;              // left boundary of component
 int16_t h;                 // height of component
 int16_t w;                 // width of component
 ///////////////////////////////////////////////////////////////
 uchar rw;               // raster width in bytes  == (w+7)/8
 uchar type;             // recognition type
#define ch_perfect    1     // perfect type defined
#define ch_letter     2     // letter type
#define ch_dust       4     // dust type
#define ch_punct      8     // punctuation
#define ch_stick     16     // vertical bar component
#define ch_great     32     // great component
#define ch_merge     64     // merged components
#define ch_notltr   128     // not letter or punctuation
 uchar cs;              // recognition case (see bellow)        *
 uchar pidx;            // proportional index (ref.)            *
 int16_t nvers;           // number of alternative versions, 0 - none !!
 int16_t records;         // recognition records offset, 0 - none     !!
 int16_t lines;           // ptr to line representation, 0 - never
                        // p_body == (char*)p_comp + lines;
 int16_t nl;               // number of lines
 uchar begs;             // number of free begins
 uchar ends;             // number of free ends
 uchar reasno;           // proportional criteria messages
 uchar large;            // large type
#define REXC_ch_underlined   0x01       // component was underlined
#define REXC_ch_taken        0x02       // taken to line at dust_ini
// Almi added //
    /*  for 'RCM'  */
#define ch_spec_put_comp  0x04     // nonstandard comp for string
#define ch_del_top_comp   0x08     // 2 comp. was vert.united; need to delete top's part
#define ch_del_bot_comp   0x10     // 2 comp. was vert.united; need to delete bot's part
#define ch_cut_left       0x80
#define ch_cut_right      0x40
    /*  for 'FRH' internal work  */
//#define el_vert_big      32     // more than anyone letter
//#define el_hori_big      64     // extremely long
////////////////
 uchar scale;                    // scale of the component
 uchar begends;
 int32_t dens;
} ExtComponent;

///////////////////////////////////////////////



//////////////////////////////////////////////




// dust_comp in H\struct.h OLD TIGER
typedef struct tagExtCompDust // currently not used
{
  uint16_t size;            // =1
  int16_t  upper;
  int16_t  left;
  uchar h;               // >0
  uchar w;               // >0
  uchar raster[8];
} ExtCompDust;


// lnhead in H\struct.h OLD TIGER
//-------------------- line representation ----------------------
//      At the beginning of line representation - word of total length -
//      not use it, simply skip
//      At end of each line zero byte as mark of line end
//      After last line zero word

//      line header
typedef struct tagExtLnHead
{
 int16_t lth;       // length of one line representation,
                  // ==head+intervals size in bytes
 int16_t h;         // height of line == count of intervals
 int16_t row;       // relative row of line start
                  // (offset in pixels from top bound of comp, 0 based)
 uint16_t flg;       // flags of free beg and free end
#define l_fbeg          0x20
#define l_fend          0x80
#define l_cbeg          0x02
#define l_cend          0x08
} ExtLnHead;


// interval in H\struct.h OLD TIGER
//      one interval
typedef struct tagExtInterval
{
 uchar l;        // length of interval
 uchar e;        // end of interval from left bound of comp in pixels
                 // so, start == e-l, relatively to left bound of comp
         //beg = inter->e - inter->l;
         //end = inter->e - 1;

} ExtInterval;


////////////////////////////////////////////////////////////////
// pool:
// 1. if    Ex_ExtraComp pool has following structure:
//      <ExtComponent>
//      <unused uint16_t>
//      <ExtLnHead>, <interval[]>
//      <ExtLnHead>, <interval[]>
//      ......................
//      <ExtLnHead>   // <= ln_head.lth == 0 ( terminator )
// where:
//       <c_comp>, <lnhead>, <interval> are structures from "struct.h"
// count of  <lnhead>, <interval[]> pairs == c_comp->nl (int16_t)
// count of intervals == lnhead->h (int16_t)
//
// 2. else  array of <gcomp> structures from "struct.h"
// size: pool size in bytes
// <count> == size/sizeof(gcomp) // if not Ex_ExtraComp
//


//  Pass parameters to extrcomp

   #define EXC_DUMP_FILE "hardcopy.cmp" // TODO: "tmp\\hardcopy.cmp"

typedef struct tagExcControl
{
        uint32_t Control;
         // Control flag (bitonal)
         #define Ex_ExtraComp            0x00000001
            // 1 - result -> ExtComponent+...
            // 0 - just frames, not intervals -> ExcBox[]
         #define Ex_DumpFile             0x00000002
            // additionally, writes to EXC_DUMP_FILE
         #define Ex_EvnRecog             0x00000004
            // additionally, recognizing all comps without evn
         #define Ex_Picture              0x00000008
                        // enable picture recognizing
         #define Ex_Orient               0x00000010
                        // enable page orientation
         #define Ex_PictureLarge         0x00000020
            // enable storing large pictures with long intervals
         #define Ex_DisableCut           0x00000040
            // enable cutting long big comps to boxes for recog
         #define Ex_Invert               0x00000080
            // use inverting image
         #define Ex_Background           0x00000100
            // stady background
         #define Ex_NetRecog             0x00000200
            // additionally, recognizing all comps without neural network
         #define Ex_NoContainer          0x00000400
            // not used container CCOM

   uint16_t MaxCompWid;  // if comp width > MaxCompWid => ignored; 0 - not init
   uint16_t MaxCompHei;  // =""= by height
   uint16_t MinCompWid;  // if comp width <= MinCompWid => ignored; 0 - not init
   uint16_t MinCompHei;  // =""= by width
   uint16_t MaxScale;    // if scaling > => use long intervals
} ExcControl;

typedef struct tagExcBox
{
 int16_t   row;       // real row of comp
 int16_t   col;       // real column of  comp
 int16_t   h;         // height of  comp
 int16_t   w;         // width of  comp
 uint16_t  flag;     // some logical info
 int32_t   user;     // working var for user
} ExcBox;


typedef struct tagREXC_DIB
      {
      uint32_t       biSize;
      int32_t        biWidth;
      int32_t        biHeight;
      uint16_t        biPlanes;
      uint16_t        biBitCount;
      uint32_t       biCompression;
      uint32_t       biSizeImage;
      int32_t        biXPelsPerMeter;
      int32_t        biYPelsPerMeter;
      uint32_t       biClrUsed;
      uint32_t       biClrImportant;
      }
   REXC_DIB;
typedef struct tagREXC_RGBQUAD { // rgbq
    uchar    rgbBlue;
    uchar    rgbGreen;
    uchar    rgbRed;
    uchar    rgbReserved;
} REXC_RGBQUAD;

#define MAX_NUM_CUTPN 2048
//Andrey: это потом надо будет убрать, аналог есть в tigerh\h\struct.h
#define RASTER_MAX_HEIGHT	63
#define RASTER_MAX_WIDTH	128
/*
struct big_merge_struct
{
 int16_t vh[2*RASTER_MAX_HEIGHT+2];
 char eh[MAX_NUM_CUTPN];
 char sh[MAX_NUM_CUTPN];
 uint16_t np;
 int16_t min_est;
 CP cp[MAX_NUM_CUTPN];
};

typedef struct big_merge_struct BM;
*/
      #pragma pack(pop)

#endif // __EXCSTRUC_H
