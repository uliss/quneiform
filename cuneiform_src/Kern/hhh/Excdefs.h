/*
Copyright (c) 1993-2008, Cognitive Technologies
All rights reserved.

–‡ÁÂ¯‡ÂÚÒˇ ÔÓ‚ÚÓÌÓÂ ‡ÒÔÓÒÚ‡ÌÂÌËÂ Ë ËÒÔÓÎ¸ÁÓ‚‡ÌËÂ Í‡Í ‚ ‚Ë‰Â ËÒıÓ‰ÌÓ„Ó ÍÓ‰‡,
Ú‡Í Ë ‚ ‰‚ÓË˜ÌÓÈ ÙÓÏÂ, Ò ËÁÏÂÌÂÌËˇÏË ËÎË ·ÂÁ, ÔË ÒÓ·Î˛‰ÂÌËË ÒÎÂ‰Û˛˘Ëı ÛÒÎÓ‚ËÈ:

      * œË ÔÓ‚ÚÓÌÓÏ ‡ÒÔÓÒÚ‡ÌÂÌËË ËÒıÓ‰ÌÓ„Ó ÍÓ‰‡ ‰ÓÎÊÌ˚ ÓÒÚ‡‚‡Ú¸Òˇ ÛÍ‡Á‡ÌÌÓÂ
        ‚˚¯Â Û‚Â‰ÓÏÎÂÌËÂ Ó· ‡‚ÚÓÒÍÓÏ Ô‡‚Â, ˝ÚÓÚ ÒÔËÒÓÍ ÛÒÎÓ‚ËÈ Ë ÔÓÒÎÂ‰Û˛˘ËÈ
        ÓÚÍ‡Á ÓÚ „‡‡ÌÚËÈ.
      * œË ÔÓ‚ÚÓÌÓÏ ‡ÒÔÓÒÚ‡ÌÂÌËË ‰‚ÓË˜ÌÓ„Ó ÍÓ‰‡ ‚ ‰ÓÍÛÏÂÌÚ‡ˆËË Ë/ËÎË ‚
        ‰Û„Ëı Ï‡ÚÂË‡Î‡ı, ÔÓÒÚ‡‚ÎˇÂÏ˚ı ÔË ‡ÒÔÓÒÚ‡ÌÂÌËË, ‰ÓÎÊÌ˚ ÒÓı‡ÌˇÚ¸Òˇ
        ÛÍ‡Á‡ÌÌ‡ˇ ‚˚¯Â ËÌÙÓÏ‡ˆËˇ Ó· ‡‚ÚÓÒÍÓÏ Ô‡‚Â, ˝ÚÓÚ ÒÔËÒÓÍ ÛÒÎÓ‚ËÈ Ë
        ÔÓÒÎÂ‰Û˛˘ËÈ ÓÚÍ‡Á ÓÚ „‡‡ÌÚËÈ.
      * ÕË Ì‡Á‚‡ÌËÂ Cognitive Technologies, ÌË ËÏÂÌ‡ ÂÂ ÒÓÚÛ‰ÌËÍÓ‚ ÌÂ ÏÓ„ÛÚ
        ·˚Ú¸ ËÒÔÓÎ¸ÁÓ‚‡Ì˚ ‚ Í‡˜ÂÒÚ‚Â ÒÂ‰ÒÚ‚‡ ÔÓ‰‰ÂÊÍË Ë/ËÎË ÔÓ‰‚ËÊÂÌËˇ
        ÔÓ‰ÛÍÚÓ‚, ÓÒÌÓ‚‡ÌÌ˚ı Ì‡ ˝ÚÓÏ œŒ, ·ÂÁ ÔÂ‰‚‡ËÚÂÎ¸ÌÓ„Ó ÔËÒ¸ÏÂÌÌÓ„Ó
        ‡ÁÂ¯ÂÌËˇ.

›“¿ œ–Œ√–¿ÃÃ¿ œ–≈ƒŒ—“¿¬À≈Õ¿ ¬À¿ƒ≈À‹÷¿Ã» ¿¬“Œ–— »’ œ–¿¬ »/»À» ƒ–”√»Ã» À»÷¿Ã» " ¿ 
ŒÕ¿ ≈—“‹" ¡≈«  ¿ Œ√Œ-À»¡Œ ¬»ƒ¿ √¿–¿Õ“»…, ¬€–¿∆≈ÕÕ€’ ﬂ¬ÕŒ »À» œŒƒ–¿«”Ã≈¬¿≈Ã€’,
¬ Àﬁ◊¿ﬂ √¿–¿Õ“»»  ŒÃÃ≈–◊≈— Œ… ÷≈ÕÕŒ—“» » œ–»√ŒƒÕŒ—“» ƒÀﬂ  ŒÕ –≈“ÕŒ… ÷≈À», ÕŒ Õ≈
Œ√–¿Õ»◊»¬¿ﬂ—‹ »Ã». Õ» ¬À¿ƒ≈À≈÷ ¿¬“Œ–— »’ œ–¿¬ » Õ» ŒƒÕŒ ƒ–”√Œ≈ À»÷Œ,  Œ“Œ–Œ≈
ÃŒ∆≈“ »«Ã≈Õﬂ“‹ »/»À» œŒ¬“Œ–ÕŒ –¿—œ–Œ—“–¿Õﬂ“‹ œ–Œ√–¿ÃÃ”, Õ» ¬  Œ≈Ã —À”◊¿≈ Õ≈
Õ≈—®“ Œ“¬≈“—“¬≈ÕÕŒ—“», ¬ Àﬁ◊¿ﬂ Àﬁ¡€≈ Œ¡Ÿ»≈, —À”◊¿…Õ€≈, —œ≈÷»¿À‹Õ€≈ »À»
œŒ—À≈ƒŒ¬¿¬ÿ»≈ ”¡€“ », —¬ﬂ«¿ÕÕ€≈ — »—œŒÀ‹«Œ¬¿Õ»≈Ã »À» œŒÕ≈—≈ÕÕ€≈ ¬—À≈ƒ—“¬»≈
Õ≈¬Œ«ÃŒ∆ÕŒ—“» »—œŒÀ‹«Œ¬¿Õ»ﬂ œ–Œ√–¿ÃÃ€ (¬ Àﬁ◊¿ﬂ œŒ“≈–» ƒ¿ÕÕ€’, »À» ƒ¿ÕÕ€≈,
—“¿¬ÿ»≈ Õ≈√ŒƒÕ€Ã», »À» ”¡€“ » »/»À» œŒ“≈–» ƒŒ’ŒƒŒ¬, œŒÕ≈—≈ÕÕ€≈ »«-«¿ ƒ≈…—“¬»…
“–≈“‹»’ À»÷ »/»À» Œ“ ¿«¿ œ–Œ√–¿ÃÃ€ –¿¡Œ“¿“‹ —Œ¬Ã≈—“ÕŒ — ƒ–”√»Ã» œ–Œ√–¿ÃÃ¿Ã»,
ÕŒ Õ≈ Œ√–¿Õ»◊»¬¿ﬂ—‹ ›“»Ã» —À”◊¿ﬂÃ»), ÕŒ Õ≈ Œ√–¿Õ»◊»¬¿ﬂ—‹ »Ã», ƒ¿∆≈ ≈—À» “¿ Œ…
¬À¿ƒ≈À≈÷ »À» ƒ–”√Œ≈ À»÷Œ ¡€À» »«¬≈Ÿ≈Õ€ Œ ¬Œ«ÃŒ∆ÕŒ—“» “¿ »’ ”¡€“ Œ¬ » œŒ“≈–‹.

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
 Word16 size;             // size of component in bytes (head + body)
                          // NOTE: in tiger.dll - in paragraphs >=3
                          // can be used to step to next comp
 /////////////////////////////////// in image coords, not ideal
 Int16 upper;             // upper boundary of component
 Int16 left;              // left boundary of component
 Int16 h;                 // height of component
 Int16 w;                 // width of component
 ///////////////////////////////////////////////////////////////
 Word8 rw;               // raster width in bytes  == (w+7)/8
 Word8 type;             // recognition type
#define ch_perfect    1     // perfect type defined
#define ch_letter     2     // letter type
#define ch_dust       4     // dust type
#define ch_punct      8     // punctuation
#define ch_stick     16     // vertical bar component
#define ch_great     32     // great component
#define ch_merge     64     // merged components
#define ch_notltr   128     // not letter or punctuation
 Word8 cs;              // recognition case (see bellow)        *
 Word8 pidx;            // proportional index (ref.)            *
 Int16 nvers;           // number of alternative versions, 0 - none !!
 Int16 records;         // recognition records offset, 0 - none     !!
 Int16 lines;           // ptr to line representation, 0 - never
                        // p_body == (char*)p_comp + lines;
 Int16 nl;               // number of lines
 Word8 begs;             // number of free begins
 Word8 ends;             // number of free ends
 Word8 reasno;           // proportional criteria messages
 Word8 large;            // large type
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
 Word8 scale;                    // scale of the component
 Word8 begends;
 Int32 dens;
} ExtComponent;

///////////////////////////////////////////////



//////////////////////////////////////////////




// dust_comp in H\struct.h OLD TIGER
typedef struct tagExtCompDust // currently not used
{
  Word16 size;            // =1
  Int16  upper;
  Int16  left;
  Word8 h;               // >0
  Word8 w;               // >0
  Word8 raster[8];
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
 Int16 lth;       // length of one line representation,
                  // ==head+intervals size in bytes
 Int16 h;         // height of line == count of intervals
 Int16 row;       // relative row of line start
                  // (offset in pixels from top bound of comp, 0 based)
 Word16 flg;       // flags of free beg and free end
#define l_fbeg          0x20
#define l_fend          0x80
#define l_cbeg          0x02
#define l_cend          0x08
} ExtLnHead;


// interval in H\struct.h OLD TIGER
//      one interval
typedef struct tagExtInterval
{
 Word8 l;        // length of interval
 Word8 e;        // end of interval from left bound of comp in pixels
                 // so, start == e-l, relatively to left bound of comp
         //beg = inter->e - inter->l;
         //end = inter->e - 1;

} ExtInterval;


////////////////////////////////////////////////////////////////
// pool:
// 1. if    Ex_ExtraComp pool has following structure:
//      <ExtComponent>
//      <unused Word16>
//      <ExtLnHead>, <interval[]>
//      <ExtLnHead>, <interval[]>
//      ......................
//      <ExtLnHead>   // <= ln_head.lth == 0 ( terminator )
// where:
//       <c_comp>, <lnhead>, <interval> are structures from "struct.h"
// count of  <lnhead>, <interval[]> pairs == c_comp->nl (Int16)
// count of intervals == lnhead->h (Int16)
//
// 2. else  array of <gcomp> structures from "struct.h"
// size: pool size in bytes
// <count> == size/sizeof(gcomp) // if not Ex_ExtraComp
//


//  Pass parameters to extrcomp

   #define EXC_DUMP_FILE "hardcopy.cmp" // TODO: "tmp\\hardcopy.cmp"

typedef struct tagExcControl
{
        Word32 Control;
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

   Word16 MaxCompWid;  // if comp width > MaxCompWid => ignored; 0 - not init
   Word16 MaxCompHei;  // =""= by height
   Word16 MinCompWid;  // if comp width <= MinCompWid => ignored; 0 - not init
   Word16 MinCompHei;  // =""= by width
   Word16 MaxScale;    // if scaling > => use long intervals
} ExcControl;

typedef struct tagExcBox
{
 Int16   row;       // real row of comp
 Int16   col;       // real column of  comp
 Int16   h;         // height of  comp
 Int16   w;         // width of  comp
 Word16  flag;     // some logical info
 Int32   user;     // working var for user
} ExcBox;


typedef struct tagREXC_DIB
      {
      Word32       biSize;
      Int32        biWidth;
      Int32        biHeight;
      Word16        biPlanes;
      Word16        biBitCount;
      Word32       biCompression;
      Word32       biSizeImage;
      Int32        biXPelsPerMeter;
      Int32        biYPelsPerMeter;
      Word32       biClrUsed;
      Word32       biClrImportant;
      }
   REXC_DIB;
typedef struct tagREXC_RGBQUAD { // rgbq
    Word8    rgbBlue;
    Word8    rgbGreen;
    Word8    rgbRed;
    Word8    rgbReserved;
} REXC_RGBQUAD;

#define MAX_NUM_CUTPN 2048
//Andrey: ˝ÚÓ ÔÓÚÓÏ Ì‡‰Ó ·Û‰ÂÚ Û·‡Ú¸, ‡Ì‡ÎÓ„ ÂÒÚ¸ ‚ tigerh\h\struct.h
#define RASTER_MAX_HEIGHT	63
#define RASTER_MAX_WIDTH	128
/*
struct big_merge_struct
{
 Int16 vh[2*RASTER_MAX_HEIGHT+2];
 char eh[MAX_NUM_CUTPN];
 char sh[MAX_NUM_CUTPN];
 Word16 np;
 Int16 min_est;
 CP cp[MAX_NUM_CUTPN];
};

typedef struct big_merge_struct BM;
*/
      #pragma pack(pop)

#endif // __EXCSTRUC_H
