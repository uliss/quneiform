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

// ACC_TABS.C

#include "string.h"
#include "cttypes.h"
#include "charsets.h"
#include "cstr/cstr.h"
#include "context.h"
#include "status.h"
#include "lang.h"
#include "ligas.h"

// RCM.C
extern uchar alpha_used_mode;

uint16_t accent_tab_lat[256] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0,
		0,
		// 0
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0,
		// 10
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0,
		// 20
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0,
		// 30
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0,
		// 40
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0,
		// 50
		0, 0, 0, 0, 0, 0, 0, 0, 0, 64, 64, 0, 0, 0, 0,
		0,
		// 60                         i   j
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0,
		// 70
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0,
		// 80
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0,
		// 90
		64, 64, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0,
		// a0
		0, 64, 0, 0, 64, 0, 64, 0, 0, 0, 0, 0, 0, 0, 0,
		0,
		// b0  fi         ffi    ri
		0x81, 0x82, 0x84, 0x80, 0x90, 0xa0, 0, 0, 0x81, 0x82, 0x84, 0x90, 0x81,
		0x82,
		0x84,
		0x90, // AA, EE, II
		// A`   A'   A^   A~   A:   Ao         E`   E'   E^   E:   I`   I'   I^   I:
		0, 0x88, 0x81, 0x82, 0x84, 0x88, 0x90, 0, 0, 0x81, 0x82, 0x84, 0x90, 0,
		0, 0,
		//      N~   O`   O'   O^   O~   O:       U`   U'   U^   U:
		1, 2, 4, 8, 16, 32, 0, 0, 1, 2, 4, 16, 1, 2, 4, 16,
		// a` a' a^ a~ a:  ao        e` e' e^ e:  i` i' i^ i:
		0, 8, 1, 2, 4, 8, 16, 0, 0, 1, 2, 4, 16, 2, 0, 16
//    n~ o` o' o^ o~ o:        u` u' u^ u:  y'    y:
		};

uint16_t accent_tab_rus[256] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0,
		0,
		// 0
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0,
		// 10
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0,
		// 20
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0,
		// 30
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0,
		// 40
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0,
		// 50
		0, 0, 0, 0, 0, 0, 0, 0, 0, 64, 64, 0, 0, 0, 0,
		0,
		// 60                         i   j
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0,
		// 70
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0,
		// 80
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0,
		// 90
		64, 64, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0,
		// a0
		0, 64, 0, 0, 64, 0, 64, 0, 0, 0, 0, 0, 0, 0, 0,
		0,
		// b0  fi         ffi    ri
		0x90, 0x90, 0x84, 0x88, 0x90, 0xa0, 0, 0, 0x81, 0x82, 0x84, 0x90, 0x81,
		0x82,
		0x84,
		0x90, // AA, EE, II
		// A`   A'   A^   A~   A:   Ao         E`   E'   E^   E:   I`   I'   I^   I:
		0, 0x88, 0x81, 0x82, 0x84, 0x88, 0x90, 0, 0, 0x81, 0x82, 0x84, 0x90, 0,
		0, 0,
		//      N~   O`   O'   O^   O~   O:       U`   U'   U^   U:
		1, 2, 4, 8, 16, 32, 0, 0, 1, 2, 4, 16, 1, 2, 4, 16,
		// a` a' a^ a~ a:  ao        e` e' e^ e:  i` i' i^ i:
		0, 8, 1, 2, 4, 8, 16, 0, 0, 1, 2, 4, 16, 2, 0, 16
//    n~ o` o' o^ o~ o:        u` u' u^ u:  y'    y:
		};

/////////////////
//   linpos  -- upper and lower check types
/////////////////
//   lindef  -- defines b1, b2, b3, b4
/////////////////
//   linshape -- solid caps, allowes to def bs1, sticky
/////////////////
////
////   here are copies of original "linpon" ans "linban" tables
////

/*************

 tblg -----> linpos for ligatures

 char tblg[]={0x16,0x16,0x16,0x16,0x31,0x21,0x10,0x10,0x16,0x11,0x12,0x11,0x11,0, 0, 0  };
 //   	       b1   b2   b3   b4   b5   b6   b7   b8   b9   ba   bb   bc   bd   be bf c0
 //             fi   fl   ff   ffi  rt   ri   %    %   ffl    i.   j.   !.   ?.

 tblgt ----> capital type if ligas
 char tblgt[]={1,1,1,1,4,2,1,1,1,1,1,4,4,0,0,0};

 tbsl  ----> spec. signs having specific capital type
 tbsn  ----> linpos for these signs

 char tbsl[]={"&$/%()+!#{}"};
 char tbsn[]={0x11,0x16,0x16,0x11,0x46,0x46,0,0x14,0x04,0x35,0x35};
 //              &    $    /    %    (    ) +    !    #    {    }

 tblt ----> linpos for characters from '0' till 'z'
 char tblt[]={0x11,0x11,0x11,0x16,0x10,0x16,0x11,0x16,0x11,0x16,
 //           0    1    2    3    4 ?? 5    6    7    8    9
 //
 00,  00,0x01, 00,0x01,0x14,0x11,
 //	        :    ;    <   =    >    ?    @
 //
 0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x16,0x11,0x11,0x11,0x11,
 //   A    B    C    D    E    F    G    H    I    J    K    L    M    N
 //
 0x11,0x11,0x16,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,
 //   O    P    Q    R    S    T    U    V    W    X    Y    Z
 //
 0x35,0x10,0x35,00,00,00,
 //	        [    \    ]  ^  _  '

 0x21,0x11,0x21,0x11,0x21,0x16,0x52,0x11,0x21,0xa2,0x11,0x11,0x21,0x21,
 //   a    b    c    d    e    f    g    h    i    j    k    l    m    n
 //
 0x21,0x53,0x22,0x21,0x21,0x31,0x21,0x21,0x21,0x21,0xa2,0x21};
 //   o    p    q    r    s    t    u    v    w    x    y    z
 //
 //

 **************/
////////////////
/////
/////    NOW modern (positional) tables
/////
////////////////

/************

 <upper_type> * 16 + <down_type>
 upper_types: 	1: large  -    l
 2: small at bs2
 3: t [ ] { }  ©
 4: ( )
 5: g, p -- bs2 or upper  ä
 2+8: j, y -- at bs2, not a small
 1+8: at bs1, not a large   ¡
 0: free

 down_types :	1: at base-3 lst
 2: sunk      dQЦЩДру
 3: p         dqQ(
 4: !?
 5: []{}	     qQ(
 6: ()f       lower than bbs3 or at bbs3  К,Я,Ж
 7: slightly lower bbs3  ¤,æ,é
 0: free
 ************/

static uchar let_linpos_lat[256] = { 0, 0, 0, 0, 0, 0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		// 0
		0, 0, 0, 0, 0, 0, 0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		// 10
		0, 0x14, 0, 0x04, 0x16, 0x11, 0x11,
		0,
		0x46,
		0x46,
		0,
		0,
		0,
		0,
		0,
		0x16,
		// 20 !    " #    $    %    &    ' (    )    . + , - . /     //
		0x11, 0x11, 0x11, 0x16, 0x10, 0x16, 0x11,
		0x16,
		0x11,
		0x16,
		0x00,
		0x00,
		0x00,
		0x00,
		0x00,
		0x14,
		// 0    1    2    3    4    5    6    7    8    9    :    ;    <    =    >    ?
		0x00, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11,
		0x11,
		0x11,
		0x16,
		0x11,
		0x11,
		0x11,
		0x11,
		0x11,
		// @    A    B    C    D    E    F    G    H    I    J    K    L    M    N    O
		0x11, 0x16, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11,
		0x11,
		0x11,
		0x35,
		0x10,
		0x35,
		0x00,
		0x00,
		// P    Q    R    S    T    U    V    W    X    Y    Z    [    \    ]    ^    _
		0x00, 0x21, 0x11, 0x21, 0x11, 0x21, 0x16, 0x52, 0x11, 0x21,
		0xa2,
		0x11,
		0x11,
		0x21,
		0x21,
		0x21,
		// `    a    b    c    d    e    f    g    h    i    j    k    l    m    n    o
		0x53, 0x22, 0x21, 0x21, 0x31, 0x21, 0x21, 0x21, 0x21, 0xa2, 0x26,
		0x35,
		0x00,
		0x35,
		0x00,
		0x00,
		// p    q    r    s    t    u    v    w    x    y    z    {    |    }    ~
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0,
		0,
		0,
		0,
		// 80                                                  //
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0,
		0,
		0,
		0,
		// 90
		0x02, 0x02, 0, 0, 0, 0, 0, 0, 0, 0, 0x11, 0,
		0x21,
		0,
		0,
		0,
		// a0                                 OE       oe
		0, 0x16, 0x16, 0x16, 0x16, 0x31, 0x21, 0x10,
		0x10,
		0x16,
		0x11,
		0x12,
		0x11,
		0x11,
		0x22,
		0x22,
		//   b1   b2   b3   b4   b5   b6   b7   b8   b9   ba   bb   bc   bd   be   bf
		//   fi   fl   ff   ffi  rt   ri   %    %   ffl    i.   j.   !.   ?.
		0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x16, 0x11, 0x11, 0x11, 0x11,
		0x11, 0x11,
		0x11,
		0x11,
		// A`   A'   A^   A~   A:   Ao   AE   C,   E`   E'   E^   E:   I`   I'   I^   I:
		0, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0, 0, 0x11, 0x11, 0x11, 0x11, 0,
		0,
		0x16,
		//   N~   O`   O'   O^   O~   O:    x  0  U`   U'   U^   U:   Y'
		0x21, 0x21, 0x21, 0x21, 0x21, 0x21, 0x21, 0x22, 0x21, 0x21, 0x21, 0x21,
		0x21, 0x21,
		0x21,
		0x21, // a, e, i
		// a`   a'   a^   a~   a:   ao   ae   c,   e`   e'   e^   e:   i`   i'   i^   i:
		0, 0x21, 0x21, 0x21, 0x21, 0x21, 0x21, 0, 0, 0x21, 0x21, 0x21, 0x21,
		0xa2, 0, 0xa2 //  n, o, u, y^ y:
		//     n~  o`   o'   o^   o~   o:     0 u`   u'   u^   u:   y'     y:
		};

static uchar let_linpos_rus[256] = { 0, 0, 0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		// 0
		0, 0, 0, 0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		// 10
		0, 0x14, 0, 0x04,
		0x16,
		0x11,
		0x11,
		0,
		0x46,
		0x46,
		0,
		0,
		0,
		0,
		0,
		0x16,
		// 20 !    " #    $    %    &    ' (    )    . + , - . /     //
		0x11, 0x11, 0x11, 0x16,
		0x10,
		0x16,
		0x11,
		0x16,
		0x11,
		0x16,
		0x00,
		0x00,
		0x00,
		0x00,
		0x00,
		0x14,
		// 0    1    2    3    4    5    6    7    8    9    :    ;    <    =    >    ?
		0x00, 0x11, 0x11, 0x11, 0x11,
		0x11,
		0x11,
		0x11,
		0x11,
		0x11,
		0x16,
		0x11,
		0x11,
		0x11,
		0x11,
		0x11,
		// @    A    B    C    D    E    F    G    H    I    J    K    L    M    N    O
		0x11, 0x16, 0x11, 0x11, 0x11, 0x11,
		0x11,
		0x11,
		0x11,
		0x11,
		0x11,
		0x35,
		0x10,
		0x35,
		0x00,
		0x00,
		// P    Q    R    S    T    U    V    W    X    Y    Z    [    \    ]    ^    _
		0x00, 0x21, 0x11, 0x21, 0x11, 0x21, 0x16,
		0x52,
		0x11,
		0x21,
		0xa2,
		0x11,
		0x11,
		0x21,
		0x21,
		0x21,
		// `    a    b    c    d    e    f    g    h    i    j    k    l    m    n    o
		0x53, 0x22, 0x21, 0x21, 0x31, 0x21, 0x21, 0x21,
		0x21,
		0xa2,
		0x26,
		0x35,
		0x00,
		0x35,
		0x00,
		0x00,
		// p    q    r    s    t    u    v    w    x    y    z    {    |    }    ~
		0x11, 0x11, 0x11, 0x11, 0x12, 0x11, 0x11, 0x11, 0x11,
		0x11,
		0x16,
		0x11,
		0x11,
		0x11,
		0x11,
		0x11,
		// А    Б    В    Г    Д    Е    Ж    З    И    Й    К    Л    М    Н    О    П
		0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x12, 0x11, 0x11, 0x12,
		0x11,
		0x11,
		0x11,
		0x11,
		0x11,
		0x16,
		// Р    С    Т    У    Ф    Х    Ц    Ч    Ш    Щ    Ъ    Ы    Ь    Э    Ю    Я
		0x21, 0x11, 0x21, 0x21, 0x27, 0x21, 0x21, 0x21, 0x21, 0x31, 0x21,
		0x21,
		0x21,
		0x21,
		0x21,
		0x21,
		// а    б    в    г    д    е    ж    з    и    й    к    л    м    н    о    п      OE       oe
		0, 0x16, 0x16, 0x16, 0x16, 0x31, 0x21, 0x10,
		0x10,
		0x16,
		0x11,
		0x12,
		0x11,
		0x11,
		0,
		0,
		//   b1   b2   b3   b4   b5   b6   b7   b8   b9   ba   bb   bc   bd   be bf c0
		//   fi   fl   ff   ffi  rt   ri   %    %   ffl    i.   j.   !.   ?.
		0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x16, 0x11, 0x11, 0x11, 0x11,
		0x11, 0x11,
		0x11,
		0x11,
		// A`   A'   A^   A~   A:   Ao   AE   C,   E`   E'   E^   E:   I`   I'   I^   I:
		0, 0, 0x11, 0x11, 0x11, 0x11, 0x11, 0, 0, 0x11, 0x11, 0x11, 0x11, 0,
		0,
		0x16,
		//    N~ O`   O'   O^   O~   O:    x  0  U`   U'   U^   U:   Y'
		0x22, 0x21, 0x21, 0xa2, 0x57, 0x21, 0x27, 0x21, 0x21, 0x27, 0x21, 0x21,
		0x21, 0x21,
		0x21,
		0x21, // a, e, i
		// р    с    т    у    ф    х    ц    ч    ш    щ    ъ    ы    ь    э    ю    я
		0x11, 0x22, 0x21, 0x21, 0x21, 0x21, 0x21, 0x21, 0x21, 0x21, 0x21, 0x21,
		0x21, 0x21, 0, 0xa2 //  n, o, u, y^ y:
		// d    g                   m          u   z                        a
		};

/***********************
 static char tblg[]={0x01,0x01,0x01,0x01,0x44,0x06,0x01,0x01,
 //                   b1   b2   b3   b4   b5   b6   b7   b8
 //                   fi   fl   ff  ffi   rt   ri    %    %
 0x01,0x44,0x48,40,40,00,00 };
 //               b9   ba   ba bc bd be bf
 //              ffl    i.   j. !. ?.

 //  tbdf describes baselines defined by a character; 8->bs1,...,1->bs4
 uchar let_tbdf[]={ 0, 0x40,0,0x40,0x40,1,5,0,0x40,0x40,0,0,0,0,0,0x40,
 //                     !    " #    $    % & ' (    )    * + , - . /    30
 0x35,5,5,1,0x40,0x40,0x44,1,5,1,0,0,0x40,0,0x40,0x40,0x40,
 //           0    1 2 3 4    5    6    7 8 9 : ; <    = >    ?    @    40
 5,5,0x35,5,5,5,0x15,5,5,1,5,5,5,5,0x35,
 //           A B C    D E F G    H I J K L M N O
 0x25,1,5,0x35,5,0x35,0x25,0x25,0x25,5,0x25,
 //           P    Q R S    T U    V    W    X    Y Z
 0,0,0,0,0,0,
 //           5b c d e f 60
 0x16,5,0x36,5,0x16,1,0x80,5,6,0x0a,5,5,6,6,0x36,
 //           a    b c    d e    f g    h i j    k l m n o
 0x22,0x0a,6,0x36,4,0x36,0x26,0x26,0x26,0x0a,0x26};
 //           p    q    r s    t u    v    w    x    y    z

 *************************/

// 0x10 (16) - round letter; 0x20 (32) - twin present, 0x40 (64)- agrres to be b1, 0x80 (128) - agrees to b2
// define bases: 1 - bs1, 2 - bs2, 4 - bs3, 8 - bs4
// Nick 25.12.2001 for a,n,o,u accents (0xe0 - 0xe5, f1, f2-f6, f9-fc) change from 0x16 to 0x14

static uchar let_lindef_lat[256] = {

0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0,
		0,
		0,
		0,
		0,
		// 0
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0,
		0,
		0,
		0,
		0,
		// 10
		0, 0x40, 0, 0x40, 0x50, 1, 5, 0, 0x40, 0x40, 0,
		0,
		0,
		0,
		0,
		0x40,
		//    !    " #    $    % & ' (    )    * + , - . /
		0x35, 5, 5, 0x11, 0x40, 0x40, 0x44, 1, 0x15, 1, 0,
		0,
		0x40,
		0,
		0x40,
		0x40,
		// 0    1 2 3    4    5    6    7 8    9 : ; <    = >    ?
		0x40, 5, 5, 0x35, 5, 5, 5, 0x15, 5, 5, 1, 5,
		5,
		5,
		5,
		0x35,
		// @    A B C    D E F G    H I J K L M N O
		0x25, 1, 5, 0x35, 5, 0x35, 0x25, 0x25, 0x25, 5, 0x25, 0,
		0,
		0,
		0,
		0,
		// P    Q R S    T U    V    W    X    Y Z    5b c d e f
		0, 0x16, 5, 0x36, 5, 0x16, 1, 0x80, 5, 4, 0x0a, 5,
		5,
		6,
		6,
		0x36,
		//   a    b c    d e    f g    h i j    k l m n o
		0x22, 0x0a, 6, 0x36, 0x44, 0x36, 0x26, 0x26, 0x26, 0x0a, 0x26, 0,
		0,
		0,
		0,
		0,
		// p    q    r s    t    u    v    w    x    y    z
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0,
		0,
		0,
		// 80                                                  //
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0,
		0,
		0,
		// 90
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0,
		0,
		0,
		// a0
		0x00, 0x01, 0x01, 0x01, 0x01, 0x44, 0x44, 0x01, 0x01, 0x01,
		0x44,
		0x48,
		40,
		40,
		00,
		00,
		// b0   b1   b2   b3   b4   b5   b6   b7   b8   b9   ba   ba   bc bd be bf
		//      fi   fl   ff  ffi   rt   ri    %    %   ffl  i.   j.   !. ?.
		5, 5, 5, 0, 5, 5, 0, 0, 5, 5,
		5,
		5,
		5,
		5,
		5,
		5, // AA, EE, II
		// c0
		0, 0, 0x15, 0x15, 0x15, 0x15, 0x15, 0, 0, 0x15, 0x15, 0x15, 0x15, 5,
		0,
		0,
		// d0       O     O     O     O     O        U     U     U     U     Y
		0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x16, 0x0a, 0x16, 0x16, 0x16, 0x16,
		6, 6,
		6,
		6, // a, e, i
		// a accents  a    a    a    a    ae   c,   e    e    e    e    i i i i
		0, 4, 0x34, 0x34, 0x34, 0x34, 0x34, 0, 0, 0x34, 0x34, 0x34, 0x34, 0x0a,
		0, 0x0a //  n, o, u, y^ y:
		// f0 n o accents                      u accents
		};

// Nick 25/01/2001 для русского маленького '\xee' /* о */ изменено с 0x34 на 0x36
//                 для i с 6 на 4
static uchar let_lindef_rus[256] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0,
		0,
		0,
		0,
		0,
		0,
		// 0
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0,
		0,
		0,
		0,
		0,
		// 10
		0, 0x40, 0, 0x40, 0x50, 1, 5, 0, 0x40, 0x40, 0,
		0,
		0,
		0,
		0,
		0x40,
		//    !    " #    $    % & ' (    )    * + , - . /
		0x35, 5, 5, 21, 5, 5, 5, 5, 21, 21, 0,
		0,
		0x40,
		0,
		0x40,
		0x40,
		// 0    1 2 3  4 5 6 7 8  9  : ; <    = >    ?
		0x40, 5, 5, 0x35, 5, 5, 5, 0x15, 5, 5, 1,
		5,
		5,
		5,
		5,
		0x35,
		// @    A B C    D E F G    H I J K L M N O
		0x25, 1, 5, 0x35, 5, 0x35, 0x25, 0x25, 0x25, 5, 0x25,
		0,
		0,
		0,
		0,
		0,
		// P    Q R S    T U    V    W    X    Y Z    5b c d e f
		0, 0x16, 5, 0x36, 5, 0x16, 1, 0x80, 5, 4, 0x0a,
		5,
		5,
		6,
		6,
		0x36,
		//   a    b c    d e    f g    h i j    k l m n o
		0x22, 0x0a, 6, 0x36, 0x44, 0x36, 0x26, 0x26, 0x26, 0x0a, 0x26,
		0,
		0,
		0,
		0,
		0,
		// p    q    r s    t    u    v    w    x    y    z
		5, 5, 0x25, 0x25, 0x21, 5, 0x25, 0x25,
		0x25,
		0x64,
		0x21,
		0x25,
		0x25,
		0x25,
		0x35,
		0x25,
		// А  Б   В    Г     Д  Е  Ж    З    И    Й    К    Л    М    Н    О     П
		// А  Б  В  Г  Д  Е  Ж  З  И  Й   К  Л  М  Н  О  П
		0x25, 0x35, 0x25, 0x25, 5, 0x25, 0x21, 0x25, 0x25, 0x21,
		0x25,
		0x25,
		0x25,
		0x25,
		0x25,
		0x21,
		// Р     С    Т    У   Ф   Х    Ц    Ч    Ш    Щ    Ъ    Ы    Ь    Э    Ю    Я
		// Р  С  Т  У  Ф  Х  Ц  Ч  Ш  Щ   Ъ  Ы  Ь  Э  Ю  Я
		0x16, 5, 0x26, 0x26, 0x22, 6, 0x26, 0x26, 0x26, 0x64, 0x26, 0x26,
		0x26,
		0x26,
		0x36,
		0x26,
		// а     б   в    г     д  е  ж    з    и    й    к    л    м    н    о     п
		// а  б  в  г  д  е  ж  з  и  й   к  л  м  н  о  п
		0x00, 0x01, 0x01, 0x01, 0x01, 0x44, 0x06, 0x01, 0x01, 0x01, 0x44, 0x48,
		40, 40, 00,
		00,
		// b0   b1   b2   b3   b4   b5   b6   b7   b8   b9   ba   ba   bc bd be bf
		//      fi   fl   ff  ffi   rt   ri    %    %   ffl  i.   j.   !. ?.
		5, 5, 5, 0, 5, 5, 0, 0, 5, 5, 5, 5, 5, 5, 5, 5,
		// c0
		0, 0, 5, 5, 5, 5, 5, 0, 0, 5, 5, 5, 5, 5, 0, 0,
		// d0
		0x2a, 0x36, 0x26, 0x2a, 0xC0, 0x26, 0x22, 0x26, 0x26, 0x22, 0x26, 0x26,
		0x26, 0x26, 0x26, 0x26,
		// р     с    т    у    ф     х    ц    ч    ш    щ    ъ    ы    ь    э    ю    я
		// р     с    т    у    ф     х    ц    ч    ш    щ    ъ    ы    ь    э    ю    я
		5, 0xA, 0, 0, 0, 6, 0, 6, 6, 0, 0, 0, 0, 6, 0, 0
// d  g        m   u z         a
		};

// lindef3 == lindef when b3 made, so twins could be resolved (as pP, yY)
static uchar let_lindef3_lat[256] = {

0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0,
		0,
		0,
		0,
		0,
		// 0
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0,
		0,
		0,
		0,
		0,
		// 10
		0, 0x40, 0, 0x40, 0x50, 1, 5, 0, 0x40, 0x40, 0,
		0,
		0,
		0,
		0,
		0x40,
		//    !    " #    $    % & ' (    )    * + , - . /
		0x35, 5, 5, 0x11, 0x40, 0x40, 0x44, 1, 0x15, 1, 0,
		0,
		0x40,
		0,
		0x40,
		0x40,
		// 0    1 2 3    4    5    6    7 8    9 : ; <    = >    ?
		0x40, 5, 5, 0x35, 5, 5, 5, 0x15, 5, 5, 1, 5,
		5,
		5,
		5,
		0x35,
		// @    A B C    D E F G    H I J K L M N O
		0x05, 1, 5, 0x35, 5, 0x35, 0x25, 0x25, 0x25, 5, 0x25, 0,
		0,
		0,
		0,
		0,
		// P    Q R S    T U    V    W    X    Y Z    5b c d e f
		0, 0x16, 5, 0x36, 5, 0x16, 1, 0x80, 5, 6, 0x0a, 5,
		5,
		6,
		6,
		0x36,
		//   a    b c    d e    f g    h i j    k l m n o
		0x02, 0x0a, 6, 0x36, 0x44, 0x36, 0x26, 0x26, 0x26, 0x0a, 0x26, 0,
		0,
		0,
		0,
		0,
		// p    q    r s    t    u    v    w    x    y    z
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0,
		0,
		0,
		// 80                                                  //
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0,
		0,
		0,
		// 90
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0,
		0,
		0,
		// ?up ?up
		0x00, 0x01, 0x01, 0x01, 0x01, 0x44, 0x44, 0x01, 0x01, 0x01,
		0x44,
		0x48,
		0x40,
		0x40,
		0x00,
		0x00,
		// b0   b1   b2   b3   b4   b5   b6   b7   b8   b9   ba   ba   bc   bd   be   bf
		//      fi   fl   ff  ffi   rt   ri    %    %   ffl  i.   j.   !.   ?.   !up  ?up
		5, 5, 5, 5, 5, 5, 0, 0, 5, 5,
		5,
		5,
		5,
		5,
		5,
		5, // AA, EE, II
		// c0
		0, 0, 0x15, 0x15, 0x15, 0x15, 0x15, 0, 0, 0x15, 0x15, 0x15, 0x15, 5,
		0,
		0,
		// d0       O     O     O     O     O        U     U     U     U     Y
		0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x16, 0x0a, 0x16, 0x16, 0x16, 0x16,
		6, 6,
		6,
		6, // a, e, i
		// a    a    a    a    a    a    ae   c,   e    e    e    e    i i i i
		0, 6, 0x36, 0x36, 0x36, 0x36, 0x36, 0, 0, 0x36, 0x36, 0x36, 0x36, 0x0a,
		0, 0x0a //  n, o, u, y^ y:
		// f0
		};
static uchar let_lindef3_rus[256] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0,
		0,
		0,
		// 0
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0,
		0,
		// 10
		0, 0x40, 0, 0x40, 0x50, 1, 5, 0, 0x40, 0x40, 0, 0, 0, 0,
		0,
		0x40,
		//    !    " #    $    % & ' (    )    * + , - . /
		0x35, 5, 5, 0x11, 0x40, 0x40, 0x44, 1, 0x15, 1, 0, 0, 0x40, 0,
		0x40,
		0x40,
		// 0    1 2 3    4    5    6    7 8    9 : ; <    = >    ?
		0x40, 5, 5, 0x35, 5, 5, 5, 0x15, 5, 5, 1, 5, 5, 5, 5,
		0x35,
		// @    A B C    D E F G    H I J K L M N O
		5, 1, 5, 0x35, 5, 0x35, 0x25, 0x25, 0x25, 5, 0x25, 0, 0, 0, 0,
		0,
		// P    Q R S    T U    V    W    X    Y Z    5b c d e f
		0, 0x16, 5, 0x36, 5, 0x16, 1, 0x80, 5, 6, 0x0a, 5, 5, 6, 6,
		0x36,
		//   a    b c    d e    f g    h i j    k l m n o
		2, 0x0a, 6, 0x36, 0x44, 0x36, 0x26, 0x26, 0x26, 0x0a, 0x26, 0, 0, 0, 0,
		0,
		// p    q    r s    t    u    v    w    x    y    z
		5, 5, 37, 37, 33, 5, 37, 37, 37, 100, 33, 37, 37, 37, 53, 37,
		// А  Б  В  Г  Д  Е  Ж  З  И  Й   К  Л  М  Н  О  П
		5, 53, 37, 5, 5, 37, 33, 37, 37, 33, 37, 37, 37, 37, 37, 33,
		// Р С  Т  У  Ф  Х  Ц  Ч  Ш  Щ   Ъ  Ы  Ь  Э  Ю  Я
		22, 5, 38, 38, 50, 6, 38, 38, 38, 100, 38, 38, 38, 38, 52, 38,
		//    ¡  ¢  £  ¤  ¥  ¦  §  ¨  ©   ª  «  ¬  ­  ®  ¯
		0x00, 0x01, 0x01, 0x01, 0x01, 0x44, 0x06, 0x01, 0x01, 0x01, 0x44, 0x48,
		40, 40, 00, 00,
		// b0   b1   b2   b3   b4   b5   b6   b7   b8   b9   ba   ba   bc bd be bf
		//      fi   fl   ff  ffi   rt   ri    %    %   ffl  i.   j.   !. ?.
		5, 5, 5, 5, 5, 5, 0, 0, 5, 5, 5, 5, 5, 5, 5, 5,
		// c0
		0, 0, 5, 5, 5, 5, 5, 0, 0, 5, 5, 5, 5, 5, 0, 0,
		// d0
		2, 52, 38, 2, 192, 38, 34, 38, 38, 34, 38, 38, 38, 38, 38, 38,
		// à   á   â   ã   ä   å   æ   ç   è   é   ê   ë   ì   í   î   ï
		5, 10, 0, 0, 0, 6, 0, 6, 6, 0, 0, 0, 0, 6, 0, 0
// d g        m   u z         a
		};

// lin_compatible -- which letter agrees to be what size
/********
 //  1 - small; 2 - capital, 4 - low (like 'y'), 8 - other
 static char tbdf[]={    // starts at 'space'
 0,10,8,11,15,3,3,8,15,15,9,11,8,8,8,15,
 //             !  " #  $  % & ' (   ) *  + , - . /           last = 2f
 3,2,2,2,2,2,2,2,2,2,0,0,11,8,11,10,15,
 //           0 1 2 3 4 5 6 7 8 9 : ; <  =  >  ?  @         last = 40
 2,2,3,2,2,2,2,2,2,10,2,2,2,2,3,15,10,2,3,2,2,3,3,3,2,3,
 //           A B C D E F G H I J  K L M N O P  Q  R S T U V W X Y Z
 15,3,15,8,8,8,
 //           [  \  ] ^ _ `                                 last: 60
 1,2,3,2,1,10,15,2,3,4,2,3,1,1,3,15,4,1,3,11,1,3,3,3,4,3};
 //           a b c d e f  g  h i j k l m n o p  q r s t  u v w x y z
 **********/
// Nick 25.12.2001 for a,n,o,u accents (0xe0 - 0xe5, f1, f2-f6, f9-fc) change from 1 to 0
static uchar let_lincomp_lat[256] = {

9, 3, 9, 10, 9, 2, 9, 12, 9, 1, 0, 10, 0, 3, 2,
		8,
		//                                                         last 0f
		4, 3, 4, 15, 4, 7, 4, 14, 4, 9, 5, 4, 4, 9, 5,
		6,
		//                                                         last 1f
		0, 10, 8, 11, 15, 3, 3, 8, 15, 15, 9, 11, 8, 8, 8,
		15,
		//     !  " #  $  % & ' (   ) *  + , - . /           last = 2f
		3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0, 0, 11, 8, 11, 10,
		15,
		//   0 1 2 3 4 5 6 7 8 9 : ; <  =  >  ?  @         last = 40
		2, 2, 3, 2, 2, 2, 2, 2, 2, 10, 2, 2, 2, 2, 3, 15, 10, 2, 3, 2, 2, 3, 3,
		3, 2, 3, 15, 3, 15, 8, 8, 8,
		//   A B C D E F G H I J  K L M N O P  Q  R S T U V W X Y Z [  \  ] ^ _ `                                 last: 60
		1, 2, 3, 2, 1, 10, 15, 2, 3, 4, 2, 3, 1, 1, 3, 15, 4, 1, 3, 11, 1, 3,
		3, 3, 4, 11, 0, 0, 0, 0, 0,
		//   a b c d e f  g  h i j k l m n o p  q r s t  u v w x y z
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		//   80                                                  //
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		//   90
		15, 15, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 1, 0, 0, 0,
		//a0 ?~ !~                          OE    oe
		0x00, 2, 2, 2, 2, 11, 11, 3, 3, 2, 2, 15, 15, 15, 4, 4,
		//   b0   b1   b2   b3   b4   b5   b6   b7   b8   b9   ba   ba   bc bd be bf
		//        fi   fl   ff  ffi   rt   ri    %    %   ffl  i.   j.   !. ?. !~ ?~
		2, 2, 2, 10, 2, 2, 2, 14, 2, 2, 2, 2, 2, 2, 2, 2, // AA, EE, II
		// A  A  A  A  A  A  AE C  E  E  E  E  I  I  I  I
		14, 0, 2, 2, 2, 2, 2, 0, 0, 2, 2, 2, 2, 10, 10, 10, //  OO, UU, YY  'ss'
		//       O  O  O  O  O        U  U  U  U  Y
		0, 0, 0, 0, 0, 0, 1, 14, 1, 1, 1, 1, 1, 1, 1, 1, // a, e, i
		// a  a  a  a  a  a  ae  c  e  e  e  e  i  i  i  i
		1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 4 //  n, o, u, y^ y:
		//    n  o  o  o  o  o        u  u  u  u  y     y
		};

static uchar let_lincomp_rus[256] = { 9, 3, 9, 10, 9, 2, 9, 12, 9, 1, 0, 10, 0,
		3, 2,
		8,
		//                                                         last 0f
		4, 3, 4, 15, 4, 7, 4, 14, 4, 9, 5, 4, 4, 9, 5,
		6,
		//                                                         last 1f
		0, 10, 8, 11, 15, 3, 3, 8, 15, 15, 9, 11, 8, 8, 8,
		15,
		//     !  " #  $  % & ' (   ) *  + , - . /           last = 2f
		3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0, 0, 11, 8, 11, 10,
		15,
		//   0 1 2 3 4 5 6 7 8 9 : ; <  =  >  ?  @         last = 40
		2, 2, 3, 2, 2, 2, 2, 2, 2, 10, 2, 2, 2, 2, 3, 15, 10, 2, 3, 2, 2, 3, 3,
		3, 2, 3, 15, 3, 15, 8, 8, 8,
		//   A B C D E F G H I J  K L M N O P  Q  R S T U V W X Y Z [  \  ] ^ _ `                                 last: 60
		1, 2, 3, 2, 1, 10, 15, 2, 3, 4, 2, 3, 1, 1, 3, 15, 4, 1, 3, 11, 1, 3,
		3, 3, 4, 11, 0, 3, 0, 0, 0,
		//   a b c d e f  g  h i j k l m n o p  q r s t  u v w x y z
		2, 2, 2, 2, 14, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
		//   А Б В Г Д  Е Ж З И Й К Л М Н О П
		2, 2, 2, 2, 2, 2, 14, 2, 2, 14, 2, 2, 2, 2, 2, 10,
		//   Р С Т У Ф Х Ц  Ч Ш Щ  Ъ Ы Ь Э Ю Я
		1, 2, 1, 1, 13, 1, 1, 1, 1, 10, 1, 1, 1, 1, 1, 1,
		//     ¡ ¢ £ ¤  ¥ ¦ § ¨ ©  ª « ¬ ­ ® ¯
		0x00, 2, 2, 2, 2, 11, 11, 3, 3, 2, 2, 15, 15, 15, 00, 00,
		//   b0   b1   b2   b3   b4   b5   b6   b7   b8   b9   ba   ba   bc bd be bf
		//        fi   fl   ff  ffi   rt   ri    %    %   ffl  i.   j.   !. ?.
		2, 2, 2, 2, 2, 2, 2, 14, 2, 2, 2, 2, 2, 2, 2, 2, // AA, EE, II
		// A  A  A  10  A  A  AE C  E  E  E  E  I  I  I  I
		0, 0, 2, 2, 2, 2, 2, 0, 0, 2, 2, 2, 2, 10, 10, 10, //  OO, UU, YY  'ss'
		//       O  O  O  O  O        U  U  U  U  Y
		5, 1, 1, 5, 8, 1, 13, 1, 1, 13, 1, 1, 1, 1, 1, 1, // a, e, i
		// à á â ã ä å æ  ç è é  ê ë ì í î ï
		2, 4, 0, 0, 0, 1, 0, 1, 1, 0, 0, 0, 0, 1, 0, 0 //  n, o, u, y^ y:
		// d g       m   u z         a
		};

static uchar let_linshape_lat[256] = {
// 1 - capital
		// 2 - sticky
		// 4 - stick allows to define bs1
		// 8 - small
		// 16 - may be above b1 ($)
		// 32 - d,b,h (may be above b1)
		/*
		 #define c_bs_b1a    1      // agrees to be at  b1
		 #define c_bs_b2a    2      //                  b2
		 #define c_bs_bt     4      //                  't' level
		 #define c_bs_t      8      // 't' only  (letters)
		 #define c_bs_b1up   16     // may be above b1
		 #define c_bs_round  32
		 */
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		// 0
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		// 10
		0, 0, 0, 0, 16, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		// 20 !  "  #  $   %  &  '  (  )  .  +  ,  -  .  /
		1, 7, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0,
		// 0  1  2  3  4  5  6  7  8  9  :  ;  <  =  >  ?
		0, 1, 1, 1, 1, 1, 1, 1, 1, 7, 1, 1, 1, 1, 1, 1,
		// @  A  B  C  D  E  F  G  H  I  J  K  L  M  N  O
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0,
		// P  Q  R  S  T  U  V  W  X  Y  Z  [  \  ]  ^  _
		0, 8, 0x08, 8, 0x08, 8, 8, 8, 0x08, 0x0e, 8, 8, 0x0e, 8, 8, 8,
		// `  a  b     c  d     e  f  g  h     i     j  k  l     m  n  o
		8, 8, 8, 8, 0x0a, 8, 8, 8, 8, 8, 8, 0, 0, 0, 0, 0,
		// p  q  r  s  t     u  v  w  x  y  z  {  |  }  ~
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		// 80                                                  //
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		// 90
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		// a0
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0,
		//     fi  fl  ff  ffi rt  ri  %   %   ffl i.  j.  !.  ?.
		1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 7, 7, 7, 7,
		// A  A  A  A  A  A        E  E  E  E  I  I  I  I
		0, 0, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 0,
		// d0    O  O  O  O  O        U  U  U  U
		8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 0x0a, 0x0a, 0x0a, 0x0a,
		// a  a  a  a  a  a        e  e  e  e  i     i     i     i
		8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8
// f0
		};

static uchar let_linshape_rus[256] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0,
		// 0
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		// 10
		0, 0, 0, 0, 16, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		// 20 !  "  #  $   %  &  '  (  )  .  +  ,  -  .  /
		1, 7, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0,
		// 0  1  2  3  4  5  6  7  8  9  :  ;  <  =  >  ?
		0, 1, 1, 1, 1, 1, 1, 1, 1, 7, 1, 1, 1, 1, 1, 1,
		// @  A  B  C  D  E  F  G  H  I  J  K  L  M  N  O
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0,
		// P  Q  R  S  T  U  V  W  X  Y  Z  [  \  ]  ^  _
		0, 8, 0x08, 8, 0x08, 8, 8, 8, 0x08, 0x0e, 8, 8, 0x0e, 8, 8, 8,
		// `  a  b     c  d     e  f  g  h     i     j  k  l     m  n  o
		8, 8, 8, 8, 0x0a, 8, 8, 8, 8, 8, 8, 0, 0, 0, 0, 0,
		// p  q  r  s  t     u  v  w  x  y  z  {  |  }  ~
		1, 1, 0, 6, 0, 1, 0, 0, 0, 16, 0, 0, 0, 0, 0, 0,
		// А Б В Г Д Е Ж З И Й  К Л М Н О П
		0, 0, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		// Р С Т У Ф Х Ц Ч Ш Щ Ъ Ы Ь Э Ю Я
		8, 8, 8, 10, 8, 8, 8, 8, 8, 0, 8, 8, 8, 8, 8, 8,
		//   ¡ ¢ £  ¤ ¥ ¦ § ¨ © ª « ¬ ­ ® ¯
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0,
		//     fi  fl  ff  ffi rt  ri  %   %   ffl i.  j.  !.  ?.
		1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 6, 6, 6, 6,
		// A  A  A  A  A  A        E  E  E  E  I  I  I  I
		0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0,
		// d0                         U  U  U  U
		8, 8, 10, 0, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
		// à á â  ã ä å æ ç è é ê ë ì í î ï
		8, 8, 0, 0, 0, 8, 0, 8, 8, 0, 0, 0, 0, 8, 0, 0
// d  g           m     u  z              a
		};

/**************/
static uchar let_sans_acc_lat[257] = { "\0               "
	"                "
	".!\"#$%&'()*+,-./"
	"0123456789:;<=>?"
	"@ABCDEFGHIJKLMNO"
	"PQRSTUVWXYZ[\\]^_"
	"`abcdefghijklmno"
	"pqrstuvwxyz{|}~a"
	"                "
	"                "
	"\xa0\xa1"
	"        "
	"\xaa"
	" "
	"\xac"
	"   "
	" \xb1\xb2\xb3\xb4\xb5\xb6\xb7\xb8\xb9\xba\xbb\xbc\xbd\xbe\xbf"
	"AAAAAA"
	"\xc6"
	"C" // C7 - CC_bottom_accent
		"EEEEIIII"
		" NOOOOO  UUUU\xdd\xde"
		"\xdf"
		"aaaaaa"
		"\xe6"
		"c" // E7 - c_bottom_accent
		"eeeeiiii"
		" nooooo  uuuuy y" };
static uchar let_sans_acc_rus[257] = { "\0               "
	"                "
	"\x2E!\"\x23\x24\x25\x26\x27\x28\x29\x2A\x2B\x2C\x2D\x2E\x2F"
	"0123456789\x3A;\x3C\x3D\x3E\x3F"
	"@ABCDEFGHIJKLMNO"
	"PQRSTUVWXYZ[\\]\x5E\x5F"
	"`abcdefghijklmno"
	"pqrstuvwxyz{|}~a"
	//"АБВГДЕЖЗИЙКЛМНОП"
	"\x80\x81\x82\x83\x84\x85\x86\x87\x88\x89\x8A\x8B\x8C\x8D\x8E\x8F"
	//"РСТУФХЦЧШЩЪЫЬЭЮЯ"
	"\x90\x91\x92\x93\x94\x95\x96\x97\x98\x99\x9A\x9B\x9C\x9D\x9E\x9F"
	//"абвгдежзийклмноп"
	"\xA0\xA1\xA2\xA3\xA4\xA5\xA6\xA7\xA8\xA9\xAA\xAB\xAC\xAD\xAE\xAF"
	" \xb1\xb2\xb3\xb4\xb5\xb6\xb7\xb8\xb9\xba\xbb\xbc\xbd\xbe\xbf"
	//"еЕAAAA  EEEEIIII"
	"\xA5\x85\x41\x41\x41\x41  EEEEIIII"
	" NOOOOO  UUUU\xdd\xde\xdf"
	//"рстуфхцчшщъыьэюя"
	"\xE0\xE1\xE2\xE3\xE4\xE5\xE6\xE7\xE8\xE9\xEA\xEB\xEC\xED\xEE\xEF"
	//"Ёёoooїoў°uuuu¤ y" };
	"\xF0\xF1ooo\xF5o\xF7\xF8uuuu\xFD y" };
/************/
uchar *let_linpos, *let_lindef, *let_lincomp, *let_linshape, *let_sans_acc,
		*let_lindef3;
uint16_t *accent_tab; // 30.08.2000 E.P.

static uchar alph_russian[256] = {

//  0 1 2 3 4 5 6 7 8 9 A B C D E F
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 0
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 1
		0, 0, 0, 1, 1, 1, 1, 0, 1, 1, 0, 0, 0, 1, 1, 1, // 2
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // 3
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 4
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, // 5
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 6
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, // 7
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // 8
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // 9
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // a
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // b
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // c
		0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, // d
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // e
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 // f
		};
static uchar alph_digital[256] = {
//  0 1 2 3 4 5 6 7 8 9 A B C D E F
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 0
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 1
		0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 2
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, // 3
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 4
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 5
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 6
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 7
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 8
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 9
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // a
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // b
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // c
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // d
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // e
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 // f
		};
static uchar alph_english[256] = {
//  0 1 2 3 4 5 6 7 8 9 A B C D E F
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 0
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 1
		0, 0, 0, 1, 1, 1, 1, 0, 1, 1, 0, 0, 0, 1, 1, 1, // 2
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // 3
		0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // 4
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, // 5
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // 6
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 0, // 7
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 8
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 9
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // a
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, // b
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // c
		0, 0, 0, 0, 0, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, // d
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // e
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 // f
		};
static uchar alph_ce[256] = {
//  0 1 2 3 4 5 6 7 8 9 A B C D E F
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 0
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 1
		0, 0, 0, 1, 0, 1, 1, 0, 1, 1, 0, 0, 0, 1, 1, 1, // 2
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // 3
		0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // 4
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, // 5
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // 6
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 0, // 7
		1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 8
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 9
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // a
		1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 0, // b
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // c
		0, 0, 0, 0, 0, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, // d
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // e
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 // f
		};
// [3] - Nick 20.10.2000 - на странице может быть 3 языка одновременно -
// русский, английский и цифровой !!!
uchar fon_alphabet_language[3][256];

uchar *alphabet_language[LANG_TOTAL] = { // 31.08.2000 E.P.
		alph_english, // LANGUAGE_ENGLISH     0
				alph_english, // LANGUAGE_GERMAN      1
				alph_english, // LANGUAGE_FRENCH      2
				alph_russian, // LANGUAGE_RUSSIAN     3
				alph_english, // LANGUAGE_SWEDISH     4
				alph_english, // LANGUAGE_SPANISH     5
				alph_english, // LANGUAGE_ITALIAN     6
				alph_russian, // LANGUAGE_RUS_ENG      7
				alph_russian, // LANGUAGE_UKRAINIAN   8
				alph_russian, // LANGUAGE_SERBIAN     9
				alph_ce, // LANGUAGE_CROATIAN   10
				alph_ce, // LANGUAGE_POLISH     11
				alph_english, // LANGUAGE_DANISH     12
				alph_english, // LANGUAGE_PORTUGUESE 13
				alph_english, // LANGUAGE_DUTCH      14
				alph_digital, // LANGUAGE_DIGITS        15
				alph_russian, // LANGUAGE_UZBEK      16
				alph_russian, // LANGUAGE_KAZAKH        17
				alph_russian, // LANGUAGE_KAZ_ENG    18
				alph_ce, // LANGUAGE_CZECH      19
				alph_ce, // LANGUAGE_ROMANIAN      20
				alph_ce, // LANGUAGE_HUNGARIAN     21
				alph_russian, // LANGUAGE_BULGARIAN     22
				alph_ce, // LANGUAGE_SLOVENIAN  23
				alph_english, // LANGUAGE_LATVIAN	 24
				alph_english, // LANGUAGE_LITHUANIAN 25
				alph_english, // LANGUAGE_ESTONIAN	 26
                               alph_english, // LANGUAGE_TURKISH	 27
                               alph_russian, // LANGUAGE_BELARUSIAN    28
		};
static void list_latin_letters(); // 01.09.2000 E.P.
static void list_baltic_letters(); // 09.07.2001 E.P.
static void list_turkish_letters(); // 20.05.2002 E.P.

void correct_let_tables(void) // used in EMBBOX.C: load_BOX()
{
	extern uchar decode_ASCII_to_[256][4];
	extern uchar decode_rus_ASCII_to_[256][4];
	extern uchar CodePages[];
	extern char alphabet[256];
	int32_t i;
	alpha_used_mode = 0;
	for (i = 128; i < 256; i++) {
		switch (language) {
		case LANGUAGE_RUSSIAN:
			strcpy((char*) decode_ASCII_to_[i], (char*) decode_rus_ASCII_to_[i]);
			break;
		default:
			decode_ASCII_to_[i][0] = '\0';
			break;
		}
	}
	if (alphabet_language[language])
		memcpy(alphabet, alphabet_language[language], 256);

	if (language == LANGUAGE_RUSSIAN) {
		let_linpos = let_linpos_rus;
		accent_tab = accent_tab_rus;
		let_lindef = let_lindef_rus;
		let_lincomp = let_lincomp_rus;
		let_linshape = let_linshape_rus;
		let_sans_acc = let_sans_acc_rus;
		let_lindef3 = let_lindef3_rus;

		alphabet[r_cu_d] = 1;
		alphabet[r_cu_g] = 1;
		alphabet[r_cu_m] = 1;
		alphabet[r_cu_u] = 1;
		alphabet[r_cu_z] = 1;
		alphabet[r_cu_a] = 1;

		alphabet[liga_exm] = 1;
		alphabet[liga_qm] = 1;
		alphabet[liga_uperc] = 1;
		alphabet[liga_lperc] = 1;
		if (multy_language) {
			strcpy((char*) decode_ASCII_to_[(uchar) liga_i], "_i_");
			strcpy((char*) decode_ASCII_to_[(uchar) liga_j], "_j_");
			strcpy((char*) decode_ASCII_to_[(uchar) liga_exm], "_!_");
			strcpy((char*) decode_ASCII_to_[(uchar) liga_qm], "_?_");
			strcpy((char*) decode_ASCII_to_[(uchar) liga_inv_exm], "_\xA2_");
			strcpy((char*) decode_ASCII_to_[(uchar) liga_inv_qm], "_\xBF_");
			strcpy((char*) decode_ASCII_to_[(uchar) liga_fi], "fi");
			strcpy((char*) decode_ASCII_to_[(uchar) liga_fl], "fl");
			strcpy((char*) decode_ASCII_to_[(uchar) liga_ff], "ff");
			strcpy((char*) decode_ASCII_to_[(uchar) liga_ffi], "ffi");
			strcpy((char*) decode_ASCII_to_[(uchar) liga_rt], "rt");
			strcpy((char*) decode_ASCII_to_[(uchar) liga_ri], "ri");
			strcpy((char*) decode_ASCII_to_[(uchar) liga_ffl], "ffl");
		}
		if (!langUkr && !langSer && !langBul) // 31.08.2000 E.P.
		{
			alphabet[r_EE_2dot] = 1;
			alphabet[r_e_2dot] = 1;
			// base lines for diskrim
			let_linpos[r_EE_2dot] = 0x11;
			let_linpos[r_e_2dot] = 0x21; //0x21

			// base lines for its determination
			let_lindef[r_EE_2dot] = 0x15;
			let_lindef[r_e_2dot] = 0x36;

			// detems klasres for 3x5
			let_lincomp[r_EE_2dot] = 0x02;
			let_lincomp[r_e_2dot] = 0x02;

			// shape of letter
			let_linshape[r_EE_2dot] = 0x01;
			let_linshape[r_e_2dot] = 0x08;

			// for accent is used in abris
			let_sans_acc[r_EE_2dot] = (uchar) '\x85' /* Е */;
			let_sans_acc[r_e_2dot] = (uchar) '\xa5' /* е */;

			// ???
			let_lindef3[r_EE_2dot] = 0;
			let_lindef3[r_e_2dot] = 0;
		}

                if(langBy) {
                       alphabet[UKR_I ]=1;
                       alphabet[UKR_i ]=1;
                       alphabet[liga_i]=1;

                       alphabet['I' ]=1;
                       alphabet['i' ]=1;
                       alphabet[u_bel] = 1;
                       alphabet[U_bel] = 1;
                       alphabet[UKR_II]=0;
                       alphabet[UKR_ii]=0;
                       alphabet[UKR_E ]=0;
                       alphabet[UKR_e ]=0;
                       alphabet[UKR_G ]=0;
                       alphabet[UKR_g ]=0;


                       alphabet[SERB_hh]=0;
                       alphabet[SERB_h ]=0;
                       alphabet['J']=0;
                       alphabet['j' ]=0;

                      strcpy((char*) decode_ASCII_to_[UKR_I ] , "\xB2");
                      strcpy((char*) decode_ASCII_to_[UKR_i ] , "\xB3");
                      strcpy((char*) decode_ASCII_to_[u_bel],"\xA2");
                      strcpy((char*) decode_ASCII_to_[u_bel],"\xA3");


                      let_linpos[UKR_I] = 0x11;   // base lines for diskrim
                      let_linpos[UKR_i] = 0x11;
                     //let_linpos['I'] = 0x11;
                     //let_linpos['i'] = 0x11;
                     let_linpos[u_bel] = 0xa2;
                     let_linpos[U_bel] = 0x11;

                     let_lindef[UKR_I] = 0;   // base lines for its determination
                     let_lindef[UKR_i] = 0;
                     let_lindef['I'] = 0;
                     let_lindef['i'] = 0;
                     let_lindef[u_bel]=0x0a;
                     let_lindef[U_bel]=0x05;

                     let_lincomp[UKR_I] = 0;  // detems klasres for 3x5
                     let_lincomp[UKR_i] = 0;
                     let_lincomp['I'] = 0;
                     let_lincomp['i'] = 0;
                     let_lincomp[u_bel]=0x04;
                     let_lincomp[U_bel]=0x0a;

                     let_linshape[UKR_I] = 0;  // shape of letter
                     let_linshape[UKR_i] = 0;
                     let_linshape['I'] = 0;
                     let_linshape['i'] = 0;
                     let_linshape[u_bel] = 0x08;
                     let_linshape[U_bel]= 0;


                     let_sans_acc[UKR_I] = UKR_I; // for accent is used in abris
                     let_sans_acc[UKR_i] = UKR_i;
                     let_sans_acc['I'] = 'I';
                     let_sans_acc['i'] = 'i';
                     let_sans_acc[u_bel]='\x95';
                     let_sans_acc[U_bel]='\xA5';

                     let_lindef3[UKR_I] = 0;  // ???
                     let_lindef3[UKR_i] = 0;
                     let_lindef3['I'] = 0;  // ???
                     let_lindef3['i'] = 0;
                     let_lindef3[u_bel] = 0x0a;
                     let_lindef3[U_bel] = 0x05;
                }

		if (langUkr) {
			alphabet[UKR_I] = 1;
			alphabet[UKR_i] = 1;
			alphabet[liga_i] = 1;

			alphabet['I'] = 1;
			alphabet['i'] = 1;
			alphabet[UKR_II] = 1;
			alphabet[UKR_ii] = 1;
			alphabet[UKR_E] = 1;
			alphabet[UKR_e] = 1;
			alphabet[UKR_G] = 1;
			alphabet[UKR_g] = 1;

			alphabet[SERB_hh] = 0;
			alphabet[SERB_h] = 0;
			alphabet['J'] = 0;
			alphabet['j'] = 0;

			strcpy((char*) decode_ASCII_to_[UKR_I], "\xB2");
			strcpy((char*) decode_ASCII_to_[UKR_i], "\xB3");
			strcpy((char*) decode_ASCII_to_[UKR_II], "\xAF");
			strcpy((char*) decode_ASCII_to_[UKR_ii], "\xBF");
			strcpy((char*) decode_ASCII_to_[UKR_E], "\xAA");
			strcpy((char*) decode_ASCII_to_[UKR_e], "\xBA");
			strcpy((char*) decode_ASCII_to_[UKR_G], "\xA5");
			strcpy((char*) decode_ASCII_to_[UKR_g], "\xB4");

			let_linpos[UKR_I] = 0x11; // base lines for diskrim
			let_linpos[UKR_i] = 0x11;
			//let_linpos['I'] = 0x11;
			//let_linpos['i'] = 0x11;
			let_linpos[UKR_II] = 0x11;
			let_linpos[UKR_ii] = 0x11;
			let_linpos[UKR_E] = 0x11;
			let_linpos[UKR_e] = 0x21;
			let_linpos[UKR_G] = 0x11;
			let_linpos[UKR_g] = 0x31;

			let_lindef[UKR_I] = 0; // base lines for its determination
			let_lindef[UKR_i] = 0;
			let_lindef['I'] = 0;
			let_lindef['i'] = 0;
			let_lindef[UKR_II] = 0;
			let_lindef[UKR_ii] = 0;
			let_lindef[UKR_E] = 38;
			let_lindef[UKR_e] = 37;
			let_lindef[UKR_G] = 0;
			let_lindef[UKR_g] = 0;

			let_lincomp[UKR_I] = 0; // detems klasres for 3x5
			let_lincomp[UKR_i] = 0;
			let_lincomp['I'] = 0;
			let_lincomp['i'] = 0;
			let_lincomp[UKR_II] = 0;
			let_lincomp[UKR_ii] = 0;
			let_lincomp[UKR_E] = 15;
			let_lincomp[UKR_e] = 15;
			let_lincomp[UKR_G] = 14;
			let_lincomp[UKR_g] = 15;

			let_linshape[UKR_I] = 0; // shape of letter
			let_linshape[UKR_i] = 0;
			let_linshape['I'] = 0;
			let_linshape['i'] = 0;
			let_linshape[UKR_II] = 0;
			let_linshape[UKR_ii] = 0;
			let_linshape[UKR_E] = 1;
			let_linshape[UKR_e] = 8;
			let_linshape[UKR_G] = 0;
			let_linshape[UKR_g] = 0;

			let_sans_acc[UKR_I] = UKR_I; // for accent is used in abris
			let_sans_acc[UKR_i] = UKR_i;
			let_sans_acc['I'] = 'I';
			let_sans_acc['i'] = 'i';
			let_sans_acc[UKR_II] = UKR_II;
			let_sans_acc[UKR_ii] = UKR_ii;
			let_sans_acc[UKR_E] = UKR_E;
			let_sans_acc[UKR_e] = UKR_e;
			let_sans_acc[UKR_G] = UKR_G;
			let_sans_acc[UKR_g] = UKR_g;

			let_lindef3[UKR_I] = 0; // ???
			let_lindef3[UKR_i] = 0;
			let_lindef3['I'] = 0; // ???
			let_lindef3['i'] = 0;
			let_lindef3[UKR_II] = 0;
			let_lindef3[UKR_ii] = 0;
			let_lindef3[UKR_E] = 0;
			let_lindef3[UKR_e] = 0;
			let_lindef3[UKR_G] = 0;
			let_lindef3[UKR_g] = 0;
		}
		if (langSer) {
			alphabet[SERB_HH] = 1;
			alphabet[SERB_hh] = 1;
			alphabet[SERB_L] = 1;
			alphabet[SERB_l] = 1;
			alphabet[SERB_N] = 1;
			alphabet[SERB_n] = 1;
			alphabet[SERB_H] = 1;
			alphabet[SERB_h] = 1;
			alphabet[SERB_U] = 1;
			alphabet[SERB_u] = 1;
			alphabet[SERB_J] = 1;
			alphabet[SERB_j] = 1;
			alphabet['J'] = 1;
			alphabet['j'] = 1;
			alphabet[liga_j] = 1;

			strcpy((char*) decode_ASCII_to_[SERB_HH], "\x80");
			strcpy((char*) decode_ASCII_to_[SERB_hh], "\x90");
			strcpy((char*) decode_ASCII_to_[SERB_L], "\x8A");
			strcpy((char*) decode_ASCII_to_[SERB_l], "\x9A");
			strcpy((char*) decode_ASCII_to_[SERB_N], "\x8C");
			strcpy((char*) decode_ASCII_to_[SERB_n], "\x9C");
			strcpy((char*) decode_ASCII_to_[SERB_H], "\x8E");
			strcpy((char*) decode_ASCII_to_[SERB_h], "\x9E");
			strcpy((char*) decode_ASCII_to_[SERB_U], "\x8F");
			strcpy((char*) decode_ASCII_to_[SERB_u], "\x9F");
			strcpy((char*) decode_ASCII_to_[SERB_J], "\xA3");
			strcpy((char*) decode_ASCII_to_[SERB_j], "\xBC");

			let_linpos[SERB_HH] = 0x11; // base lines for diskrim
			let_linpos[SERB_hh] = 0x17;
			let_linpos[SERB_L] = 0x11;
			let_linpos[SERB_l] = 0x21;
			let_linpos[SERB_N] = 0x11;
			let_linpos[SERB_n] = 0x21;
			let_linpos[SERB_H] = 0x11;
			let_linpos[SERB_h] = 0x11;
			let_linpos[SERB_U] = 0x12;
			let_linpos[SERB_u] = 0x27;
			let_linpos[SERB_J] = 0x16;
			let_linpos[SERB_j] = 0xa2;
			let_linpos['J'] = 0x16;
			let_linpos['j'] = 0xa2;

			let_lindef[SERB_HH] = 5; // base lines for its determination
			let_lindef[SERB_hh] = 0;
			let_lindef[SERB_L] = 37;
			let_lindef[SERB_l] = 38;
			let_lindef[SERB_N] = 37;
			let_lindef[SERB_n] = 38;
			let_lindef[SERB_H] = 5;
			let_lindef[SERB_h] = 5;
			let_lindef[SERB_U] = 33;
			let_lindef[SERB_u] = 34;
			let_lindef[SERB_J] = 0;
			let_lindef[SERB_j] = 0;
			let_lindef['J'] = 0;
			let_lindef['j'] = 0;

			let_lincomp[SERB_HH] = 2; // detems klasres for 3x5
			let_lincomp[SERB_hh] = 14;
			let_lincomp[SERB_L] = 2;
			let_lincomp[SERB_l] = 1;
			let_lincomp[SERB_N] = 2;
			let_lincomp[SERB_n] = 1;
			let_lincomp[SERB_H] = 2;
			let_lincomp[SERB_h] = 2;
			let_lincomp[SERB_U] = 14;
			let_lincomp[SERB_u] = 13;
			let_lincomp[SERB_J] = 10;
			let_lincomp[SERB_j] = 4;
			let_lincomp['J'] = 10;
			let_lincomp['j'] = 4;

			let_linshape[SERB_HH] = 0; // shape of letter
			let_linshape[SERB_hh] = 8;
			let_linshape[SERB_L] = 0;
			let_linshape[SERB_l] = 8;
			let_linshape[SERB_N] = 0;
			let_linshape[SERB_n] = 8;
			let_linshape[SERB_H] = 0;
			let_linshape[SERB_h] = 8;
			let_linshape[SERB_U] = 0;
			let_linshape[SERB_u] = 8;
			let_linshape[SERB_J] = 1;
			let_linshape[SERB_j] = 8;
			let_linshape['J'] = 1;
			let_linshape['j'] = 8;

			let_sans_acc[SERB_HH] = SERB_HH; // for accent is used in abris
			let_sans_acc[SERB_hh] = SERB_hh;
			let_sans_acc[SERB_L] = SERB_L;
			let_sans_acc[SERB_l] = SERB_l;
			let_sans_acc[SERB_N] = SERB_N;
			let_sans_acc[SERB_n] = SERB_n;
			let_sans_acc[SERB_H] = SERB_H;
			let_sans_acc[SERB_h] = SERB_h;
			let_sans_acc[SERB_U] = SERB_U;
			let_sans_acc[SERB_u] = SERB_u;
			let_sans_acc[SERB_J] = SERB_J;
			let_sans_acc[SERB_j] = SERB_j;
			let_sans_acc['J'] = 'J';
			let_sans_acc['j'] = 'j';

			let_lindef3[SERB_HH] = 0; // ???
			let_lindef3[SERB_hh] = 0;
			let_lindef3[SERB_L] = 0;
			let_lindef3[SERB_l] = 0;
			let_lindef3[SERB_N] = 0;
			let_lindef3[SERB_n] = 0;
			let_lindef3[SERB_H] = 0;
			let_lindef3[SERB_h] = 0;
			let_lindef3[SERB_U] = 0;
			let_lindef3[SERB_u] = 0;
			let_lindef3[SERB_J] = 0;
			let_lindef3[SERB_j] = 0;
			let_lindef3['J'] = 0;
			let_lindef3['j'] = 0;
		}
		if (langBul) { // В Болгарском нет трех русских букв. 31.08.2000 E.P.
			// Ы=155, ы=235, Э= 157, э=237
			// BULGAR
			alphabet[r_EE_2dot] = 0;
			alphabet[r_e_2dot] = 0;
			alphabet[155] = 0;
			alphabet[235] = 0;
			alphabet[157] = 0;
			alphabet[237] = 0;

			// BULGAR
			let_linpos[r_EE_2dot] = 0;
			let_linpos[r_e_2dot] = 0;
			let_linpos[155] = 0;
			let_linpos[235] = 0;
			let_linpos[157] = 0;
			let_linpos[237] = 0;

			// BULGAR
			let_lindef[r_EE_2dot] = 0;
			let_lindef[r_e_2dot] = 0;
			let_lindef[155] = 0;
			let_lindef[235] = 0;
			let_lindef[157] = 0;
			let_lindef[237] = 0;

			// BULGAR
			let_lincomp[r_EE_2dot] = 0;
			let_lincomp[r_e_2dot] = 0;
			let_lincomp[155] = 0;
			let_lincomp[235] = 0;
			let_lincomp[157] = 0;
			let_lincomp[237] = 0;

			// BULGAR
			let_linshape[r_EE_2dot] = 0;
			let_linshape[r_e_2dot] = 0;
			let_linshape[155] = 0;
			let_linshape[235] = 0;
			let_linshape[157] = 0;
			let_linshape[237] = 0;

			// BULGAR
			let_sans_acc[r_EE_2dot] = 0;
			let_sans_acc[r_e_2dot] = 0;
			let_sans_acc[155] = 0;
			let_sans_acc[235] = 0;
			let_sans_acc[157] = 0;
			let_sans_acc[237] = 0;

			// BULGAR
			let_lindef3[r_EE_2dot] = 0;
			let_lindef3[r_e_2dot] = 0;
			let_lindef3[155] = 0;
			let_lindef3[235] = 0;
			let_lindef3[157] = 0;
			let_lindef3[237] = 0;

			// BULGAR
			accent_tab[r_EE_2dot] = 0;
			accent_tab[r_e_2dot] = 0;
			accent_tab[155] = 0;
			accent_tab[235] = 0;
			accent_tab[157] = 0;
			accent_tab[237] = 0;

		}
	} // LANGUAGE_RUSSIAN
	else {
		let_linpos = let_linpos_lat;
		accent_tab = accent_tab_lat;
		let_lindef = let_lindef_lat;
		let_lincomp = let_lincomp_lat;
		let_linshape = let_linshape_lat;
		let_sans_acc = let_sans_acc_lat;
		let_lindef3 = let_lindef3_lat;

		// В связи с новыми кодами лиг для cp1250 01.09.2000 E.P.
		if (is_cen_language(language)) {
			alphabet[liga_i] = 1;
			alphabet[liga_exm] = 1;
			alphabet[liga_inv_exm] = 1;
			alphabet[right_quocket] = 1;
			alphabet[liga_CC] = 1;
			alphabet[liga_CR] = 1;
			alphabet[liga_bull] = 1;

			accent_tab[liga_i] = accent_tab[liga_i_usual];
			accent_tab[liga_exm] = accent_tab[liga_exm_usual];
			accent_tab[liga_inv_exm] = accent_tab[liga_inv_exm_usual];
			accent_tab[right_quocket] = accent_tab[right_quocket_usual];
			accent_tab[liga_CC] = accent_tab[liga_CC_usual];
			accent_tab[liga_CR] = accent_tab[liga_CR_usual];
			accent_tab[liga_bull] = accent_tab[liga_bull_usual];

			let_linpos[liga_i] = let_linpos[liga_i_usual];
			let_linpos[liga_exm] = let_linpos[liga_exm_usual];
			let_linpos[liga_inv_exm] = let_linpos[liga_inv_exm_usual];
			let_linpos[right_quocket] = let_linpos[right_quocket_usual];
			let_linpos[liga_CC] = let_linpos[liga_CC_usual];
			let_linpos[liga_CR] = let_linpos[liga_CR_usual];
			let_linpos[liga_bull] = let_linpos[liga_bull_usual];

			let_lindef[liga_i] = let_lindef[liga_i_usual];
			let_lindef[liga_exm] = let_lindef[liga_exm_usual];
			let_lindef[liga_inv_exm] = let_lindef[liga_inv_exm_usual];
			let_lindef[right_quocket] = let_lindef[right_quocket_usual];
			let_lindef[liga_CC] = let_lindef[liga_CC_usual];
			let_lindef[liga_CR] = let_lindef[liga_CR_usual];
			let_lindef[liga_bull] = let_lindef[liga_bull_usual];

			let_lincomp[liga_i] = let_lincomp[liga_i_usual];
			let_lincomp[liga_exm] = let_lincomp[liga_exm_usual];
			let_lincomp[liga_inv_exm] = let_lincomp[liga_inv_exm_usual];
			let_lincomp[right_quocket] = let_lincomp[right_quocket_usual];
			let_lincomp[liga_CC] = let_lincomp[liga_CC_usual];
			let_lincomp[liga_CR] = let_lincomp[liga_CR_usual];
			let_lincomp[liga_bull] = let_lincomp[liga_bull_usual];

			let_linshape[liga_i] = let_linshape[liga_i_usual];
			let_linshape[liga_exm] = let_linshape[liga_exm_usual];
			let_linshape[liga_inv_exm] = let_linshape[liga_inv_exm_usual];
			let_linshape[right_quocket] = let_linshape[right_quocket_usual];
			let_linshape[liga_CC] = let_linshape[liga_CC_usual];
			let_linshape[liga_CR] = let_linshape[liga_CR_usual];
			let_linshape[liga_bull] = let_linshape[liga_bull_usual];

			let_lindef3[liga_i] = let_lindef3[liga_i_usual];
			let_lindef3[liga_exm] = let_lindef3[liga_exm_usual];
			let_lindef3[liga_inv_exm] = let_lindef3[liga_inv_exm_usual];
			let_lindef3[right_quocket] = let_lindef3[right_quocket_usual];
			let_lindef3[liga_CC] = let_lindef3[liga_CC_usual];
			let_lindef3[liga_CR] = let_lindef3[liga_CR_usual];
			let_lindef3[liga_bull] = let_lindef3[liga_bull_usual];

			strcpy((char*) decode_ASCII_to_[(uchar) liga_i], "_i_");
			strcpy((char*) decode_ASCII_to_[(uchar) liga_exm], "_!_");
			strcpy((char*) decode_ASCII_to_[(uchar) liga_inv_exm], "_!!_");
			strcpy((char*) decode_ASCII_to_[(uchar) right_quocket], "\xbb");
			strcpy((char*) decode_ASCII_to_[(uchar) liga_CC], "\xa9");
			strcpy((char*) decode_ASCII_to_[(uchar) liga_CR], "\xae");
			strcpy((char*) decode_ASCII_to_[(uchar) liga_bull], "\xB0");

		}

		// Балтийские лиги для cp1257 06.07.2001 E.P.
		if (is_baltic_language(language)) {
			alphabet[low_quotes] = 1;
			alphabet[liga_exm] = 1;
			alphabet[liga_i] = 1;
			alphabet[right_quocket] = 1;
			alphabet[liga_TM] = 1;
			alphabet[liga_CC] = 1;
			alphabet[liga_CR] = 1;
			alphabet[liga_bull] = 1;

			accent_tab[low_quotes] = accent_tab[low_quotes_usual];
			accent_tab[liga_exm] = accent_tab[liga_exm_usual];
			accent_tab[liga_i] = accent_tab[liga_i_usual];
			accent_tab[right_quocket] = accent_tab[right_quocket_usual];
			accent_tab[liga_TM] = accent_tab[liga_TM_usual];
			accent_tab[liga_CC] = accent_tab[liga_CC_usual];
			accent_tab[liga_CR] = accent_tab[liga_CR_usual];
			accent_tab[liga_bull] = accent_tab[liga_bull_usual];

			let_linpos[low_quotes] = let_linpos[low_quotes_usual];
			let_linpos[liga_exm] = let_linpos[liga_exm_usual];
			let_linpos[liga_i] = let_linpos[liga_i_usual];
			let_linpos[right_quocket] = let_linpos[right_quocket_usual];
			let_linpos[liga_TM] = let_linpos[liga_TM_usual];
			let_linpos[liga_CC] = let_linpos[liga_CC_usual];
			let_linpos[liga_CR] = let_linpos[liga_CR_usual];
			let_linpos[liga_bull] = let_linpos[liga_bull_usual];

			let_lindef[low_quotes] = let_lindef[low_quotes_usual];
			let_lindef[liga_exm] = let_lindef[liga_exm_usual];
			let_lindef[liga_i] = let_lindef[liga_i_usual];
			let_lindef[right_quocket] = let_lindef[right_quocket_usual];
			let_lindef[liga_TM] = let_lindef[liga_TM_usual];
			let_lindef[liga_CC] = let_lindef[liga_CC_usual];
			let_lindef[liga_CR] = let_lindef[liga_CR_usual];
			let_lindef[liga_bull] = let_lindef[liga_bull_usual];

			let_lincomp[low_quotes] = let_lincomp[low_quotes_usual];
			let_lincomp[liga_exm] = let_lincomp[liga_exm_usual];
			let_lincomp[liga_i] = let_lincomp[liga_i_usual];
			let_lincomp[right_quocket] = let_lincomp[right_quocket_usual];
			let_lincomp[liga_TM] = let_lincomp[liga_TM_usual];
			let_lincomp[liga_CC] = let_lincomp[liga_CC_usual];
			let_lincomp[liga_CR] = let_lincomp[liga_CR_usual];
			let_lincomp[liga_bull] = let_lincomp[liga_bull_usual];

			let_linshape[low_quotes] = let_linshape[low_quotes_usual];
			let_linshape[liga_exm] = let_linshape[liga_exm_usual];
			let_linshape[liga_i] = let_linshape[liga_i_usual];
			let_linshape[right_quocket] = let_linshape[right_quocket_usual];
			let_linshape[liga_TM] = let_linshape[liga_TM_usual];
			let_linshape[liga_CC] = let_linshape[liga_CC_usual];
			let_linshape[liga_CR] = let_linshape[liga_CR_usual];
			let_linshape[liga_bull] = let_linshape[liga_bull_usual];

			let_lindef3[low_quotes] = let_lindef3[low_quotes_usual];
			let_lindef3[liga_exm] = let_lindef3[liga_exm_usual];
			let_lindef3[liga_i] = let_lindef3[liga_i_usual];
			let_lindef3[right_quocket] = let_lindef3[right_quocket_usual];
			let_lindef3[liga_TM] = let_lindef3[liga_TM_usual];
			let_lindef3[liga_CC] = let_lindef3[liga_CC_usual];
			let_lindef3[liga_CR] = let_lindef3[liga_CR_usual];
			let_lindef3[liga_bull] = let_lindef3[liga_bull_usual];

		}

		if (language == LANGUAGE_CROATIAN) {
			alphabet[CROAT_D] = 1;
			alphabet[CROAT_d] = 1;
			alphabet[CROAT_SR] = 1;
			alphabet[CROAT_sr] = 1;
			alphabet[CROAT_CA] = 1;
			alphabet[CROAT_ca] = 1;
			alphabet[CROAT_ZR] = 1;
			alphabet[CROAT_zr] = 1;
			alphabet[CROAT_CR] = 1;
			alphabet[CROAT_cr] = 1;

			let_linpos[CROAT_D] = 0x11; // base lines for diskrim
			let_linpos[CROAT_d] = 0x11;
			let_linpos[CROAT_SR] = 0x11;
			let_linpos[CROAT_sr] = 0x11; //0x21
			let_linpos[CROAT_CA] = 0x11;
			let_linpos[CROAT_ca] = 0x11; //0x21
			let_linpos[CROAT_ZR] = 0x11;
			let_linpos[CROAT_zr] = 0x11; //0x21
			let_linpos[CROAT_CR] = 0x11;
			let_linpos[CROAT_cr] = 0x11; //0x21

			let_lindef[CROAT_D] = 0x05; // base lines for its determination
			let_lindef[CROAT_d] = 0x05;
			let_lindef[CROAT_SR] = 0x15;
			let_lindef[CROAT_sr] = 0x36;
			let_lindef[CROAT_CA] = 0x15;
			let_lindef[CROAT_ca] = 0x36;
			let_lindef[CROAT_ZR] = 0x15;
			let_lindef[CROAT_zr] = 0x36;
			let_lindef[CROAT_CR] = 0x15;
			let_lindef[CROAT_cr] = 0x36;

			let_lincomp[CROAT_D] = 0x02; // detems klasres for 3x5
			let_lincomp[CROAT_d] = 0x02;
			let_lincomp[CROAT_SR] = 0x02;
			let_lincomp[CROAT_sr] = 0x02; // 0x01
			let_lincomp[CROAT_CA] = 0x02;
			let_lincomp[CROAT_ca] = 0x02; // 0x01
			let_lincomp[CROAT_ZR] = 0x02;
			let_lincomp[CROAT_zr] = 0x02; // 0x01
			let_lincomp[CROAT_CR] = 0x02;
			let_lincomp[CROAT_cr] = 0x02; // 0x01

			let_linshape[CROAT_D] = 0x01; // shape of letter
			let_linshape[CROAT_d] = 0x08;
			let_linshape[CROAT_SR] = 0x01;
			let_linshape[CROAT_sr] = 0x08;
			let_linshape[CROAT_CA] = 0x01;
			let_linshape[CROAT_ca] = 0x08;
			let_linshape[CROAT_ZR] = 0x01;
			let_linshape[CROAT_zr] = 0x08;
			let_linshape[CROAT_CR] = 0x01;
			let_linshape[CROAT_cr] = 0x08;

			let_sans_acc[CROAT_D] = CROAT_D; // for accent is used in abris
			let_sans_acc[CROAT_d] = CROAT_d;
			let_sans_acc[CROAT_SR] = 'S';
			let_sans_acc[CROAT_sr] = 's';
			let_sans_acc[CROAT_CA] = 'C';
			let_sans_acc[CROAT_ca] = 'c';
			let_sans_acc[CROAT_ZR] = 'Z';
			let_sans_acc[CROAT_zr] = 'z';
			let_sans_acc[CROAT_CR] = 'C';
			let_sans_acc[CROAT_cr] = 'c';

			let_lindef3[CROAT_D] = 0; // ???
			let_lindef3[CROAT_d] = 0;
			let_lindef3[CROAT_SR] = 0;
			let_lindef3[CROAT_sr] = 0;
			let_lindef3[CROAT_CA] = 0;
			let_lindef3[CROAT_ca] = 0;
			let_lindef3[CROAT_ZR] = 0;
			let_lindef3[CROAT_zr] = 0;
			let_lindef3[CROAT_CR] = 0;
			let_lindef3[CROAT_cr] = 0;

			accent_tab[CROAT_SR] = ACC_ROOF_INV | ACC_SUPERUP; // 31.08.2000 E.P.
			accent_tab[CROAT_sr] = ACC_ROOF_INV;
			accent_tab[CROAT_CA] = ACC_RIGHT | ACC_SUPERUP;
			accent_tab[CROAT_ca] = ACC_RIGHT;
			accent_tab[CROAT_ZR] = ACC_ROOF_INV | ACC_SUPERUP;
			accent_tab[CROAT_zr] = ACC_ROOF_INV;
			accent_tab[CROAT_CR] = ACC_ROOF_INV | ACC_SUPERUP;
			accent_tab[CROAT_cr] = ACC_ROOF_INV;
		} else if (language == LANGUAGE_POLISH) {
			alphabet[POLISH_SS] = 1;
			alphabet[POLISH_s] = 1;
			alphabet[POLISH_ZZR] = 1;
			alphabet[POLISH_zr] = 1;
			alphabet[POLISH_ZZD] = 1;
			alphabet[POLISH_zd] = 1;
			alphabet[POLISH_CC] = 1;
			alphabet[POLISH_c] = 1;
			alphabet[POLISH_EE] = 1;
			alphabet[POLISH_e] = 1;
			alphabet[POLISH_NN] = 1;
			alphabet[POLISH_n] = 1;
			alphabet[POLISH_OO] = 1;
			alphabet[POLISH_o] = 1;
			alphabet[POLISH_LL] = 1;
			alphabet[POLISH_l] = 1;
			alphabet[POLISH_AA] = 1;
			alphabet[POLISH_a] = 1;
			// top accents: s, z(acute), z(dot), c, n, o.
			accent_tab[POLISH_SS] = 0x82;
			accent_tab[POLISH_s] = 2;
			accent_tab['Z'] = 0xC2; // don't discrim upper dust, like ij+right accent. See discrid() in DM1.C
			accent_tab['z'] = 0x42; // don't discrim upper dust, like ij+right accent. See discrid() in DM1.C
			accent_tab[POLISH_ZZR] = 0x82;
			accent_tab[POLISH_zr] = 2;
			accent_tab[POLISH_ZZD] = 0xC0; // dot accent
			accent_tab[POLISH_zd] = 2;
			accent_tab[POLISH_CC] = 0x82;
			accent_tab[POLISH_c] = 2;
			accent_tab[POLISH_EE] = 0x82;
			accent_tab[POLISH_e] = 2;
			accent_tab[POLISH_NN] = 0x82; // вместо тильды 0x88
			accent_tab[POLISH_n] = 2; // вместо тильды 8
			accent_tab[POLISH_OO] = 0x82;
			accent_tab[POLISH_o] = 2;

			let_linpos[POLISH_SS] = 0x11; // base lines for diskrim
			let_linpos[POLISH_s] = 0x11;
			let_linpos[POLISH_ZZR] = 0x11;
			let_linpos[POLISH_zr] = 0x11;
			let_linpos[POLISH_LL] = 0x11;
			let_linpos[POLISH_l] = 0x11;
			let_linpos[POLISH_AA] = 0x16;
			let_linpos[POLISH_a] = 0x22;
			let_linpos[POLISH_ZZD] = 0x11;
			let_linpos[POLISH_zd] = 0x31; // Like russian "й". 23.10.97
			let_linpos[POLISH_CC] = 0x11;
			let_linpos[POLISH_c] = 0x11;
			let_linpos[POLISH_EE] = 0x16;
			let_linpos[POLISH_e] = 0x22;
			let_linpos[POLISH_NN] = 0x11;
			let_linpos[POLISH_n] = 0x11;
			let_linpos[POLISH_OO] = 0x11;
			let_linpos[POLISH_o] = 0x11;

			// base lines for its determination
			let_lindef[POLISH_SS] = 0x15;
			let_lindef[POLISH_s] = 0x36;
			let_lindef[POLISH_ZZR] = 0x25; // not round, as Z
			let_lindef[POLISH_zr] = 0x26; // not round, as z
			let_lindef[POLISH_LL] = 5; // as L
			let_lindef[POLISH_l] = 5; // as l
			let_lindef[POLISH_AA] = 0x19; // bottom accent
			let_lindef[POLISH_a] = 0x1a; // bottom accent
			let_lindef[POLISH_ZZD] = 0x25; // Как Z
			let_lindef[POLISH_zd] = 0x26; // Как z
			let_lindef[POLISH_CC] = 0x35; // Как С
			let_lindef[POLISH_c] = 0x36;
			let_lindef[POLISH_EE] = 0x19; // bottom accent
			let_lindef[POLISH_e] = 0x1a; // bottom accent
			let_lindef[POLISH_NN] = 0x15;
			let_lindef[POLISH_n] = 0x16;
			let_lindef[POLISH_OO] = 0x15;
			let_lindef[POLISH_o] = 0x36;

			// clasters for 3x5
			let_lincomp[POLISH_SS] = 2; // 2 - capital
			let_lincomp[POLISH_s] = 2;
			let_lincomp[POLISH_ZZR] = 2;
			let_lincomp[POLISH_zr] = 2;
			let_lincomp[POLISH_LL] = 2;
			let_lincomp[POLISH_l] = 2;
			let_lincomp[POLISH_AA] = 0x0E; // 2 - capital, 4 - low, 8 - other
			let_lincomp[POLISH_a] = 0x0D; // 1 - small,   4 - low, 8 - other
			let_lincomp[POLISH_ZZD] = 2;
			let_lincomp[POLISH_zd] = 2;
			let_lincomp[POLISH_CC] = 2;
			let_lincomp[POLISH_c] = 2;
			let_lincomp[POLISH_EE] = 0x0E; // 2 - capital, 4 - low, 8 - other
			let_lincomp[POLISH_e] = 0x0D; // 1 - small,   4 - low, 8 - other
			let_lincomp[POLISH_NN] = 2;
			let_lincomp[POLISH_n] = 2;
			let_lincomp[POLISH_OO] = 2;
			let_lincomp[POLISH_o] = 2;

			// solid caps, allows to define bs1, sticky
			let_linshape[POLISH_SS] = 1;
			let_linshape[POLISH_s] = 8;
			let_linshape[POLISH_ZZR] = 1;
			let_linshape[POLISH_zr] = 8;
			let_linshape[POLISH_LL] = 1;
			let_linshape[POLISH_l] = 0x0e; // sticky, as l
			let_linshape[POLISH_AA] = 1;
			let_linshape[POLISH_a] = 8;
			let_linshape[POLISH_ZZD] = 1;
			let_linshape[POLISH_zd] = 8;
			let_linshape[POLISH_CC] = 1;
			let_linshape[POLISH_c] = 8;
			let_linshape[POLISH_EE] = 1;
			let_linshape[POLISH_e] = 8;
			let_linshape[POLISH_NN] = 1;
			let_linshape[POLISH_n] = 8;
			let_linshape[POLISH_OO] = 1;
			let_linshape[POLISH_o] = 8;

			let_sans_acc[POLISH_SS] = 'S';
			let_sans_acc[POLISH_s] = 's';
			let_sans_acc[POLISH_ZZR] = 'Z';
			let_sans_acc[POLISH_zr] = 'z';
			let_sans_acc[POLISH_LL] = POLISH_LL; // stroked
			let_sans_acc[POLISH_l] = POLISH_l; // stroked
			let_sans_acc[POLISH_AA] = 'A'; // bottom accent
			let_sans_acc[POLISH_a] = 'a'; // bottom accent
			let_sans_acc[POLISH_ZZD] = 'Z';
			let_sans_acc[POLISH_zd] = 'z';
			let_sans_acc[POLISH_CC] = 'C';
			let_sans_acc[POLISH_c] = 'c';
			let_sans_acc[POLISH_EE] = 'E'; // bottom accent
			let_sans_acc[POLISH_e] = 'e'; // bottom accent
			let_sans_acc[POLISH_NN] = 'N';
			let_sans_acc[POLISH_n] = 'n';
			let_sans_acc[POLISH_OO] = 'O';
			let_sans_acc[POLISH_o] = 'o';

			let_lindef3[POLISH_SS] = 0; // ?
			let_lindef3[POLISH_s] = 0;
			let_lindef3[POLISH_ZZR] = 0;
			let_lindef3[POLISH_zr] = 0;
			let_lindef3[POLISH_LL] = 0;
			let_lindef3[POLISH_l] = 0;
			let_lindef3[POLISH_AA] = 0;
			let_lindef3[POLISH_a] = 0;
			let_lindef3[POLISH_ZZD] = 0;
			let_lindef3[POLISH_zd] = 0;
			let_lindef3[POLISH_CC] = 0;
			let_lindef3[POLISH_c] = 0;
			let_lindef3[POLISH_EE] = 0;
			let_lindef3[POLISH_e] = 0;
			let_lindef3[POLISH_NN] = 0;
			let_lindef3[POLISH_n] = 0;
			let_lindef3[POLISH_OO] = 0;
			let_lindef3[POLISH_o] = 0;

		} // POLISH

		if (language == LANGUAGE_CZECH) {

			// CZECH 31.08.2000 E.P.
			alphabet[AA_right_accent] = alphabet[a_right_accent] = 1;
			alphabet[CC_inv_roof] = alphabet[c_inv_roof] = 1;
			alphabet[DD_inv_roof] = alphabet[d_inv_roof] = 1;
			alphabet[EE_right_accent] = alphabet[e_right_accent] = 1;
			alphabet[EE_inv_roof] = alphabet[e_inv_roof] = 1;
			alphabet[II_right_accent] = alphabet[i_right_accent] = 1;
			alphabet[NN_inv_roof] = alphabet[n_inv_roof] = 1;
			alphabet[OO_right_accent] = alphabet[o_right_accent] = 1;
			alphabet[RR_inv_roof] = alphabet[r_inv_roof] = 1;
			alphabet[SS_inv_roof_latin] = alphabet[s_inv_roof_latin] = 1;
			alphabet[TT_inv_roof] = alphabet[t_inv_roof] = 1;
			alphabet[UU_right_accent] = alphabet[u_right_accent] = 1;
			alphabet[UU_circle_accent] = alphabet[u_circle_accent] = 1;
			alphabet[YY_right_accent] = alphabet[y_right_accent] = 1;
			alphabet[ZZ_inv_roof_latin] = alphabet[z_inv_roof_latin] = 1;

			// base lines for diskrim (CZECH)
			let_linpos[AA_right_accent] = 1;
			let_linpos[a_right_accent] = 0x11;
			let_linpos[CC_inv_roof] = 1;
			let_linpos[c_inv_roof] = 0x11;
			let_linpos[DD_inv_roof] = 1;
			let_linpos[d_inv_roof] = 0x11;
			let_linpos[EE_right_accent] = 1;
			let_linpos[e_right_accent] = 0x11;
			let_linpos[EE_inv_roof] = 1;
			let_linpos[e_inv_roof] = 0x11;
			let_linpos[II_right_accent] = 1;
			let_linpos[i_right_accent] = 0x11;
			let_linpos[NN_inv_roof] = 1;
			let_linpos[n_inv_roof] = 0x11;
			let_linpos[OO_right_accent] = 1;
			let_linpos[o_right_accent] = 0x11;
			let_linpos[RR_inv_roof] = 1;
			let_linpos[r_inv_roof] = 0x11;
			let_linpos[SS_inv_roof_latin] = 1;
			let_linpos[s_inv_roof_latin] = 0x11;
			let_linpos[TT_inv_roof] = 1;
			let_linpos[t_inv_roof] = 0x31; // Êàê t
			let_linpos[UU_right_accent] = 1;
			let_linpos[u_right_accent] = 0x11;
			let_linpos[UU_circle_accent] = 1;
			let_linpos[u_circle_accent] = 0x11;
			let_linpos[YY_right_accent] = 1;
			let_linpos[y_right_accent] = 0x11;
			let_linpos[ZZ_inv_roof_latin] = 1;
			let_linpos[z_inv_roof_latin] = 0x11;

			// base lines for its determination (CZECH)
			let_lindef[AA_right_accent] = 4; // Только bs3
			let_lindef[a_right_accent] = 4;
			let_lindef[CC_inv_roof] = 0x14; // 10-круглая, 4 - bs3
			let_lindef[c_inv_roof] = 0x15;
			let_lindef[DD_inv_roof] = 4;
			let_lindef[d_inv_roof] = 5;
			let_lindef[EE_right_accent] = 4;
			let_lindef[e_right_accent] = 4;
			let_lindef[EE_inv_roof] = 4;
			let_lindef[e_inv_roof] = 5;
			let_lindef[II_right_accent] = 4;
			let_lindef[i_right_accent] = 6; // 2 - bs2, 3 - bs3 (like i)
			let_lindef[NN_inv_roof] = 4;
			let_lindef[n_inv_roof] = 5;
			let_lindef[OO_right_accent] = 0x14; // 10-круглая, 4 - bs3
			let_lindef[o_right_accent] = 0x14;
			let_lindef[RR_inv_roof] = 4;
			let_lindef[r_inv_roof] = 5;
			let_lindef[SS_inv_roof_latin] = 0x14; // 10-круглая, 4 - bs3
			let_lindef[s_inv_roof_latin] = 0x15;
			let_lindef[TT_inv_roof] = 4;
			let_lindef[t_inv_roof] = 4;
			let_lindef[UU_right_accent] = 0x14; // 10-круглая, 4 - bs3
			let_lindef[u_right_accent] = 0x14;
			let_lindef[UU_circle_accent] = 0x14;
			let_lindef[u_circle_accent] = 0x14;
			let_lindef[YY_right_accent] = 4;
			let_lindef[y_right_accent] = 4;
			let_lindef[ZZ_inv_roof_latin] = 4;
			let_lindef[z_inv_roof_latin] = 5;

			// detems klasres for 3x5 (CZECH)
			let_lincomp[AA_right_accent] = let_lincomp[a_right_accent] = 2;
			let_lincomp[CC_inv_roof] = let_lincomp[c_inv_roof] = 2;
			let_lincomp[DD_inv_roof] = let_lincomp[d_inv_roof] = 2;
			let_lincomp[EE_right_accent] = let_lincomp[e_right_accent] = 2;
			let_lincomp[EE_inv_roof] = let_lincomp[e_inv_roof] = 2;
			let_lincomp[II_right_accent] = let_lincomp[i_right_accent] = 2;
			let_lincomp[NN_inv_roof] = let_lincomp[n_inv_roof] = 2;
			let_lincomp[OO_right_accent] = let_lincomp[o_right_accent] = 2;
			let_lincomp[RR_inv_roof] = let_lincomp[r_inv_roof] = 2;
			let_lincomp[SS_inv_roof_latin] = let_lincomp[s_inv_roof_latin] = 2;
			let_lincomp[TT_inv_roof] = let_lincomp[t_inv_roof] = 2;
			let_lincomp[UU_right_accent] = let_lincomp[u_right_accent] = 2;
			let_lincomp[UU_circle_accent] = let_lincomp[u_circle_accent] = 2;
			let_lincomp[YY_right_accent] = let_lincomp[y_right_accent] = 2;
			let_lincomp[ZZ_inv_roof_latin] = let_lincomp[z_inv_roof_latin] = 2;

			// shape of letter (CZECH)
			let_linshape[AA_right_accent] = 1;
			let_linshape[a_right_accent] = 8;
			let_linshape[CC_inv_roof] = 1;
			let_linshape[c_inv_roof] = 8;
			let_linshape[DD_inv_roof] = 1;
			let_linshape[d_inv_roof] = 8;
			let_linshape[EE_right_accent] = 1;
			let_linshape[e_right_accent] = 8;
			let_linshape[EE_inv_roof] = 1;
			let_linshape[e_inv_roof] = 8;
			let_linshape[II_right_accent] = 1;
			let_linshape[i_right_accent] = 0x0e; // as i
			let_linshape[NN_inv_roof] = 1;
			let_linshape[n_inv_roof] = 8;
			let_linshape[OO_right_accent] = 1;
			let_linshape[o_right_accent] = 8;
			let_linshape[RR_inv_roof] = 1;
			let_linshape[r_inv_roof] = 8;
			let_linshape[SS_inv_roof_latin] = 1;
			let_linshape[s_inv_roof_latin] = 8;
			let_linshape[TT_inv_roof] = 1;
			let_linshape[t_inv_roof] = 8;
			let_linshape[UU_right_accent] = 1;
			let_linshape[u_right_accent] = 8;
			let_linshape[UU_circle_accent] = 1;
			let_linshape[u_circle_accent] = 8;
			let_linshape[YY_right_accent] = 1;
			let_linshape[y_right_accent] = 8;
			let_linshape[ZZ_inv_roof_latin] = 1;
			let_linshape[z_inv_roof_latin] = 8;

			// for accent is used in abris (CZECH)
			let_sans_acc[AA_right_accent] = 'A';
			let_sans_acc[a_right_accent] = 'a';
			let_sans_acc[CC_inv_roof] = 'C';
			let_sans_acc[c_inv_roof] = 'c';
			let_sans_acc[DD_inv_roof] = 'D';
			let_sans_acc[d_inv_roof] = 'd';
			let_sans_acc[EE_right_accent] = 'E';
			let_sans_acc[e_right_accent] = 'e';
			let_sans_acc[EE_inv_roof] = 'E';
			let_sans_acc[e_inv_roof] = 'e';
			let_sans_acc[II_right_accent] = 'I';
			let_sans_acc[i_right_accent] = 'i';
			let_sans_acc[NN_inv_roof] = 'N';
			let_sans_acc[n_inv_roof] = 'n';
			let_sans_acc[OO_right_accent] = 'O';
			let_sans_acc[o_right_accent] = 'o';
			let_sans_acc[RR_inv_roof] = 'R';
			let_sans_acc[r_inv_roof] = 'r';
			let_sans_acc[SS_inv_roof_latin] = 'S';
			let_sans_acc[s_inv_roof_latin] = 's';
			let_sans_acc[TT_inv_roof] = 'T';
			let_sans_acc[t_inv_roof] = 't';
			let_sans_acc[UU_right_accent] = 'U';
			let_sans_acc[u_right_accent] = 'u';
			let_sans_acc[UU_circle_accent] = 'U';
			let_sans_acc[u_circle_accent] = 'u';
			let_sans_acc[YY_right_accent] = 'Y';
			let_sans_acc[y_right_accent] = 'y';
			let_sans_acc[ZZ_inv_roof_latin] = 'Z';
			let_sans_acc[z_inv_roof_latin] = 'z';

			// (CZECH)
			let_lindef3[AA_right_accent] = let_lindef3['A'];
			let_lindef3[a_right_accent] = let_lindef3['a'];
			let_lindef3[CC_inv_roof] = let_lindef3['C'];
			let_lindef3[c_inv_roof] = let_lindef3['c'];
			let_lindef3[DD_inv_roof] = let_lindef3['D'];
			let_lindef3[d_inv_roof] = let_lindef3['d'];
			let_lindef3[EE_right_accent] = let_lindef3['E'];
			let_lindef3[e_right_accent] = let_lindef3['e'];
			let_lindef3[EE_inv_roof] = let_lindef3['E'];
			let_lindef3[e_inv_roof] = let_lindef3['e'];
			let_lindef3[II_right_accent] = let_lindef3['I'];
			let_lindef3[i_right_accent] = let_lindef3['i'];
			let_lindef3[NN_inv_roof] = let_lindef3['N'];
			let_lindef3[n_inv_roof] = let_lindef3['n'];
			let_lindef3[OO_right_accent] = let_lindef3['O'];
			let_lindef3[o_right_accent] = let_lindef3['o'];
			let_lindef3[RR_inv_roof] = let_lindef3['R'];
			let_lindef3[r_inv_roof] = let_lindef3['r'];
			let_lindef3[SS_inv_roof_latin] = let_lindef3['S'];
			let_lindef3[s_inv_roof_latin] = let_lindef3['s'];
			let_lindef3[TT_inv_roof] = let_lindef3['T'];
			let_lindef3[t_inv_roof] = let_lindef3['t'];
			let_lindef3[UU_right_accent] = let_lindef3['U'];
			let_lindef3[u_right_accent] = let_lindef3['u'];
			let_lindef3[UU_circle_accent] = let_lindef3['U'];
			let_lindef3[u_circle_accent] = let_lindef3['u'];
			let_lindef3[YY_right_accent] = let_lindef3['Y'];
			let_lindef3[y_right_accent] = let_lindef3['y'];
			let_lindef3[ZZ_inv_roof_latin] = let_lindef3['Z'];
			let_lindef3[z_inv_roof_latin] = let_lindef3['z'];

			// (CZECH)
			accent_tab[AA_right_accent] = ACC_RIGHT | ACC_SUPERUP;
			accent_tab[a_right_accent] = ACC_RIGHT;
			accent_tab[CC_inv_roof] = ACC_ROOF_INV | ACC_SUPERUP;
			accent_tab[c_inv_roof] = ACC_ROOF_INV;
			accent_tab[DD_inv_roof] = ACC_ROOF_INV | ACC_SUPERUP;
			accent_tab[d_inv_roof] = 0; // Не пытаться отрезать ACC_ROOF_INV;
			accent_tab[EE_right_accent] = ACC_RIGHT | ACC_SUPERUP;
			accent_tab[e_right_accent] = ACC_RIGHT;
			accent_tab[EE_inv_roof] = ACC_ROOF_INV | ACC_SUPERUP;
			accent_tab[e_inv_roof] = ACC_ROOF_INV;
			accent_tab[II_right_accent] = ACC_RIGHT | ACC_SUPERUP;
			accent_tab[i_right_accent] = ACC_RIGHT;
			accent_tab[NN_inv_roof] = ACC_ROOF_INV | ACC_SUPERUP;
			accent_tab[n_inv_roof] = ACC_ROOF_INV;
			accent_tab[OO_right_accent] = ACC_RIGHT | ACC_SUPERUP;
			accent_tab[o_right_accent] = ACC_RIGHT;
			accent_tab[RR_inv_roof] = ACC_ROOF_INV | ACC_SUPERUP;
			accent_tab[r_inv_roof] = ACC_ROOF_INV;
			accent_tab[SS_inv_roof_latin] = ACC_ROOF_INV | ACC_SUPERUP;
			accent_tab[s_inv_roof_latin] = ACC_ROOF_INV;
			accent_tab[TT_inv_roof] = ACC_ROOF_INV | ACC_SUPERUP;
			accent_tab[t_inv_roof] = ACC_ROOF_INV;
			accent_tab[UU_right_accent] = ACC_RIGHT | ACC_SUPERUP;
			accent_tab[u_right_accent] = ACC_RIGHT;
			accent_tab[UU_circle_accent] = ACC_CIR | ACC_SUPERUP;
			accent_tab[u_circle_accent] = ACC_CIR;
			accent_tab[YY_right_accent] = ACC_RIGHT | ACC_SUPERUP;
			accent_tab[y_right_accent] = ACC_RIGHT;
			accent_tab[ZZ_inv_roof_latin] = ACC_ROOF_INV | ACC_SUPERUP;
			accent_tab[z_inv_roof_latin] = ACC_ROOF_INV;

		} // CZECH

		if (language == LANGUAGE_ROMANIAN) {

			// ROMAN 31.08.2000 E.P.
			alphabet[AA_semicircle] = alphabet[a_semicircle] = 1;
			alphabet[AA_roof_accent] = alphabet[a_roof_accent] = 1;
			alphabet[II_roof_accent] = alphabet[i_roof_accent] = 1;
			alphabet[SS_bottom_accent_latin] = alphabet[s_bottom_accent_latin]
					= 1;
			alphabet[TT_bottom_accent] = alphabet[t_bottom_accent] = 1;

			// base lines for diskrim (ROMAN)
			let_linpos[AA_semicircle] = 1;
			let_linpos[a_semicircle] = 0x11;
			let_linpos[AA_roof_accent] = 1;
			let_linpos[a_roof_accent] = 0x11;
			let_linpos[II_roof_accent] = 1;
			let_linpos[i_roof_accent] = 0x11;
			let_linpos[SS_bottom_accent_latin] = 0x16;
			let_linpos[s_bottom_accent_latin] = 0x22;
			let_linpos[TT_bottom_accent] = 0x16;
			let_linpos[t_bottom_accent] = 0x32; // 22->32 05.07.2002 E.P.

			// base lines for its determination (ROMAN)
			let_lindef[AA_semicircle] = 4; // 4 - bs3
			let_lindef[a_semicircle] = 0x84; // 40-agrees to be b1, 4 - bs3
			let_lindef[AA_roof_accent] = 4;
			let_lindef[a_roof_accent] = 0x84;
			let_lindef[II_roof_accent] = 4;
			let_lindef[i_roof_accent] = 0x84;
			let_lindef[SS_bottom_accent_latin] = 0x31; // 20 - есть близнец, 10 - круглая, 1 - bs1
			let_lindef[s_bottom_accent_latin] = 0x32; // 20 - есть близнец, 10 - круглая, 2 - bs2
			let_lindef[TT_bottom_accent] = 0;
			let_lindef[t_bottom_accent] = 0x40; // 40-agrees to be b1, как t

			// detems klasres for 3x5 (ROMAN)
			let_lincomp[AA_semicircle] = let_lincomp[a_semicircle] = 2;
			let_lincomp[AA_roof_accent] = let_lincomp[a_roof_accent] = 2;
			let_lincomp[II_roof_accent] = let_lincomp[i_roof_accent] = 2;
			let_lincomp[SS_bottom_accent_latin] = 0x0e; // 2 - capital, 4 - low, 8 - other
			let_lincomp[s_bottom_accent_latin] = 0x0d; // 1 - small,   4 - low, 8 - other
			let_lincomp[TT_bottom_accent] = 0x0e;
			let_lincomp[t_bottom_accent] = 0x0d;

			// shape of letter (ROMAN)
			let_linshape[AA_semicircle] = 1;
			let_linshape[a_semicircle] = 8;
			let_linshape[AA_roof_accent] = 1;
			let_linshape[a_roof_accent] = 8;
			let_linshape[II_roof_accent] = 1;
			let_linshape[i_roof_accent] = 8;
			let_linshape[SS_bottom_accent_latin] = 1;
			let_linshape[s_bottom_accent_latin] = 8;
			let_linshape[TT_bottom_accent] = 1;
			let_linshape[t_bottom_accent] = 8;

			// for accent is used in abris (ROMAN)
			let_sans_acc[AA_semicircle] = 'A';
			let_sans_acc[a_semicircle] = 'a';
			let_sans_acc[AA_roof_accent] = 'A';
			let_sans_acc[a_roof_accent] = 'a';
			let_sans_acc[II_roof_accent] = 'I';
			let_sans_acc[i_roof_accent] = 'i';
			let_sans_acc[SS_bottom_accent_latin] = 'S';
			let_sans_acc[s_bottom_accent_latin] = 's';
			let_sans_acc[TT_bottom_accent] = 'T';
			let_sans_acc[t_bottom_accent] = 't';

			//  (ROMAN)
			let_lindef3[AA_semicircle] = let_lindef3[a_semicircle] = 0;
			let_lindef3[AA_roof_accent] = let_lindef3[a_roof_accent] = 0;
			let_lindef3[II_roof_accent] = let_lindef3[i_roof_accent] = 0;
			let_lindef3[SS_bottom_accent_latin]
					= let_lindef3[s_bottom_accent_latin] = 0;
			let_lindef3[TT_bottom_accent] = let_lindef3[t_bottom_accent] = 0;

			//  (ROMAN)
			accent_tab[AA_semicircle] = ACC_ROOF_INV | ACC_SUPERUP;
			accent_tab[a_semicircle] = ACC_ROOF_INV;
			accent_tab[AA_roof_accent] = ACC_ROOF | ACC_SUPERUP;
			accent_tab[a_roof_accent] = ACC_ROOF;
			accent_tab[II_roof_accent] = ACC_ROOF | ACC_SUPERUP;
			accent_tab[i_roof_accent] = ACC_ROOF;
			accent_tab[SS_bottom_accent_latin] = 0;
			accent_tab[s_bottom_accent_latin] = 0;
			accent_tab[TT_bottom_accent] = 0;
			accent_tab[t_bottom_accent] = 0;

		} // ROMAN

		if (language == LANGUAGE_HUNGARIAN) { // 31.08.2000 E.P.
			alphabet[AA_right_accent] = alphabet[a_right_accent] = 1;
			alphabet[EE_right_accent] = alphabet[e_right_accent] = 1;
			alphabet[II_right_accent] = alphabet[i_right_accent] = 1;
			alphabet[OO_right_accent] = alphabet[o_right_accent] = 1;
			alphabet[OO_2dot_accent] = alphabet[o_2dot_accent] = 1;
			alphabet[OO_double_right] = alphabet[o_double_right] = 1;
			alphabet[UU_right_accent] = alphabet[u_right_accent] = 1;
			alphabet[UU_2dot_accent] = alphabet[u_2dot_accent] = 1;
			alphabet[UU_double_right] = alphabet[u_double_right] = 1;

			// base lines for diskrim (HUNGAR)
			let_linpos[AA_right_accent] = 1;
			let_linpos[a_right_accent] = 0x11;
			let_linpos[EE_right_accent] = 1;
			let_linpos[e_right_accent] = 0x11;
			let_linpos[II_right_accent] = 1;
			let_linpos[i_right_accent] = 0x11;
			let_linpos[OO_right_accent] = 1;
			let_linpos[o_right_accent] = 0x11;
			let_linpos[OO_2dot_accent] = 1;
			let_linpos[o_2dot_accent] = 0x11;
			let_linpos[OO_double_right] = 1;
			let_linpos[o_double_right] = 0x11;
			let_linpos[UU_right_accent] = 1;
			let_linpos[u_right_accent] = 0x11;
			let_linpos[UU_2dot_accent] = 1;
			let_linpos[u_2dot_accent] = 0x11;
			let_linpos[UU_double_right] = 1;
			let_linpos[u_double_right] = 0x11;

			// base lines for its determination (HUNGAR)
			let_lindef[AA_right_accent] = 4; // Òîëüêî bs3
			let_lindef[a_right_accent] = 4;
			let_lindef[EE_right_accent] = 4;
			let_lindef[e_right_accent] = 4;
			let_lindef[II_right_accent] = 4;
			let_lindef[i_right_accent] = 6; // 2 - bs2, 3 - bs3 (like i)
			let_lindef[OO_right_accent] = 0x14; // 10-круглая, 4 - bs3
			let_lindef[o_right_accent] = 0x14;
			let_lindef[OO_2dot_accent] = 0x14;
			let_lindef[o_2dot_accent] = 0x14;
			let_lindef[OO_double_right] = 0x14;
			let_lindef[o_double_right] = 0x14;
			let_lindef[UU_right_accent] = 0x14;
			let_lindef[u_right_accent] = 0x14;
			let_lindef[UU_2dot_accent] = 0x14;
			let_lindef[u_2dot_accent] = 0x14;
			let_lindef[UU_double_right] = 0x14;
			let_lindef[u_double_right] = 0x14;

			// detems klasres for 3x5 (HUNGAR)
			let_lincomp[AA_semicircle] = let_lincomp[a_semicircle] = 2;
			let_lincomp[AA_roof_accent] = let_lincomp[a_roof_accent] = 2;
			let_lincomp[II_roof_accent] = let_lincomp[i_roof_accent] = 2;
			let_lincomp[SS_bottom_accent_latin] = 0x0e; // 2 - capital, 4 - low, 8 - other
			let_lincomp[s_bottom_accent_latin] = 0x0d; // 1 - small,   4 - low, 8 - other
			let_lincomp[TT_bottom_accent] = 0x0e;
			let_lincomp[t_bottom_accent] = 0x0d;
			let_lincomp[OO_right_accent] = let_lincomp[o_right_accent] = 2;
			let_lincomp[OO_2dot_accent] = let_lincomp[o_2dot_accent] = 2;
			let_lincomp[OO_double_right] = let_lincomp[o_double_right] = 2;
			let_lincomp[UU_right_accent] = let_lincomp[u_right_accent] = 2;
			let_lincomp[UU_2dot_accent] = let_lincomp[u_2dot_accent] = 2;
			let_lincomp[UU_double_right] = let_lincomp[u_double_right] = 2;

			// shape of letter (HUNGAR)
			let_linshape[AA_semicircle] = 1;
			let_linshape[a_semicircle] = 8;
			let_linshape[AA_roof_accent] = 1;
			let_linshape[a_roof_accent] = 8;
			let_linshape[II_roof_accent] = 1;
			let_linshape[i_roof_accent] = 8;
			let_linshape[SS_bottom_accent_latin] = 1;
			let_linshape[s_bottom_accent_latin] = 8;
			let_linshape[TT_bottom_accent] = 1;
			let_linshape[t_bottom_accent] = 0x0a; // sticky as t
			let_linshape[OO_right_accent] = 1;
			let_linshape[o_right_accent] = 8;
			let_linshape[OO_2dot_accent] = 1;
			let_linshape[o_2dot_accent] = 8;
			let_linshape[OO_double_right] = 1;
			let_linshape[o_double_right] = 8;
			let_linshape[UU_right_accent] = 1;
			let_linshape[u_right_accent] = 8;
			let_linshape[UU_2dot_accent] = 1;
			let_linshape[u_2dot_accent] = 8;
			let_linshape[UU_double_right] = 1;
			let_linshape[u_double_right] = 8;

			// for accent is used in abris (HUNGAR)
			let_sans_acc[AA_semicircle] = 'A';
			let_sans_acc[a_semicircle] = 'a';
			let_sans_acc[AA_roof_accent] = 'A';
			let_sans_acc[a_roof_accent] = 'a';
			let_sans_acc[II_roof_accent] = 'I';
			let_sans_acc[i_roof_accent] = 'i';
			let_sans_acc[SS_bottom_accent_latin] = 'S';
			let_sans_acc[s_bottom_accent_latin] = 's';
			let_sans_acc[TT_bottom_accent] = 'S';
			let_sans_acc[t_bottom_accent] = 't';
			let_sans_acc[OO_right_accent] = 'O';
			let_sans_acc[o_right_accent] = 'o';
			let_sans_acc[OO_2dot_accent] = 'O';
			let_sans_acc[o_2dot_accent] = 'o';
			let_sans_acc[OO_double_right] = 'O';
			let_sans_acc[o_double_right] = 'o';
			let_sans_acc[UU_right_accent] = 'U';
			let_sans_acc[u_right_accent] = 'u';
			let_sans_acc[UU_2dot_accent] = 'U';
			let_sans_acc[u_2dot_accent] = 'u';
			let_sans_acc[UU_double_right] = 'U';
			let_sans_acc[u_double_right] = 'u';

			// (HUNGAR)
			let_lindef3[AA_semicircle] = let_lindef3[a_semicircle] = 0;
			let_lindef3[AA_roof_accent] = let_lindef3[a_roof_accent] = 0;
			let_lindef3[II_roof_accent] = let_lindef3[i_roof_accent] = 0;
			let_lindef3[SS_bottom_accent_latin]
					= let_lindef3[s_bottom_accent_latin] = 0;
			let_lindef3[TT_bottom_accent] = let_lindef3[t_bottom_accent] = 0;
			let_lindef3[OO_right_accent] = let_lindef3[o_right_accent] = 0;
			let_lindef3[OO_2dot_accent] = let_lindef3[o_2dot_accent] = 0;
			let_lindef3[OO_double_right] = let_lindef3[o_double_right] = 0;
			let_lindef3[UU_right_accent] = let_lindef3[u_right_accent] = 0;
			let_lindef3[UU_2dot_accent] = let_lindef3[u_2dot_accent] = 0;
			let_lindef3[UU_double_right] = let_lindef3[u_double_right] = 0;

			// (HUNGAR)
			accent_tab[AA_semicircle] = ACC_ROOF_INV | ACC_SUPERUP;
			accent_tab[a_semicircle] = ACC_ROOF_INV;
			accent_tab[AA_roof_accent] = ACC_ROOF | ACC_SUPERUP;
			accent_tab[a_roof_accent] = ACC_ROOF;
			accent_tab[II_roof_accent] = ACC_ROOF | ACC_SUPERUP;
			accent_tab[i_roof_accent] = ACC_ROOF;
			accent_tab[SS_bottom_accent_latin] = 0;
			accent_tab[s_bottom_accent_latin] = 0;
			accent_tab[TT_bottom_accent] = 0;
			accent_tab[t_bottom_accent] = 0;
			accent_tab[OO_right_accent] = ACC_RIGHT | ACC_SUPERUP;
			accent_tab[o_right_accent] = ACC_RIGHT;
			accent_tab[OO_2dot_accent] = ACC_2DOT | ACC_SUPERUP;
			accent_tab[o_2dot_accent] = ACC_2DOT;
			accent_tab[OO_double_right] = ACC_DOUBLE_RIGHT | ACC_SUPERUP;
			accent_tab[o_double_right] = ACC_DOUBLE_RIGHT;
			accent_tab[UU_right_accent] = ACC_RIGHT | ACC_SUPERUP;
			accent_tab[u_right_accent] = ACC_RIGHT;
			accent_tab[UU_2dot_accent] = ACC_2DOT | ACC_SUPERUP;
			accent_tab[u_2dot_accent] = ACC_2DOT;
			accent_tab[UU_double_right] = ACC_DOUBLE_RIGHT | ACC_SUPERUP;
			accent_tab[u_double_right] = ACC_DOUBLE_RIGHT;

		} // HUNGAR

		if (language == LANGUAGE_SLOVENIAN) {

			// SLOVENIAN 25.05.2001 E.P.
			alphabet[CC_inv_roof] = alphabet[c_inv_roof] = 1;
			alphabet[SS_inv_roof_latin] = alphabet[s_inv_roof_latin] = 1;
			alphabet[ZZ_inv_roof_latin] = alphabet[z_inv_roof_latin] = 1;

			alphabet[(uchar) 'Q'] = alphabet[(uchar) 'q'] = 0;
			alphabet[(uchar) 'W'] = alphabet[(uchar) 'w'] = 0;
			alphabet[(uchar) 'X'] = alphabet[(uchar) 'x'] = 0;
			alphabet[(uchar) 'Y'] = alphabet[(uchar) 'y'] = 0;

			// base lines for diskrim (SLOVENIAN)
			let_linpos[CC_inv_roof] = 1;
			let_linpos[c_inv_roof] = 0x11;
			let_linpos[SS_inv_roof_latin] = 1;
			let_linpos[s_inv_roof_latin] = 0x11;
			let_linpos[ZZ_inv_roof_latin] = 1;
			let_linpos[z_inv_roof_latin] = 0x11;

			// base lines for its determination (SLOVENIAN)
			let_lindef[CC_inv_roof] = 0x14; // 10-круглая, 4 - bs3
			let_lindef[c_inv_roof] = 0x15;
			let_lindef[SS_inv_roof_latin] = 0x14; // 10-круглая, 4 - bs3
			let_lindef[s_inv_roof_latin] = 0x15;
			let_lindef[ZZ_inv_roof_latin] = 4;
			let_lindef[z_inv_roof_latin] = 5;

			// detems klasres for 3x5 (SLOVENIAN)
			let_lincomp[CC_inv_roof] = let_lincomp[c_inv_roof] = 2;
			let_lincomp[SS_inv_roof_latin] = let_lincomp[s_inv_roof_latin] = 2;
			let_lincomp[ZZ_inv_roof_latin] = let_lincomp[z_inv_roof_latin] = 2;

			// shape of letter (SLOVENIAN)
			let_linshape[CC_inv_roof] = 1;
			let_linshape[c_inv_roof] = 8;
			let_linshape[SS_inv_roof_latin] = 1;
			let_linshape[s_inv_roof_latin] = 8;
			let_linshape[ZZ_inv_roof_latin] = 1;
			let_linshape[z_inv_roof_latin] = 8;

			// for accent is used in abris (SLOVENIAN)
			let_sans_acc[CC_inv_roof] = 'C';
			let_sans_acc[c_inv_roof] = 'c';
			let_sans_acc[SS_inv_roof_latin] = 'S';
			let_sans_acc[s_inv_roof_latin] = 's';
			let_sans_acc[ZZ_inv_roof_latin] = 'Z';
			let_sans_acc[z_inv_roof_latin] = 'z';

			// (SLOVENIAN)
			let_lindef3[CC_inv_roof] = let_lindef3['C'];
			let_lindef3[c_inv_roof] = let_lindef3['c'];
			let_lindef3[SS_inv_roof_latin] = let_lindef3['S'];
			let_lindef3[s_inv_roof_latin] = let_lindef3['s'];
			let_lindef3[ZZ_inv_roof_latin] = let_lindef3['Z'];
			let_lindef3[z_inv_roof_latin] = let_lindef3['z'];

			// (SLOVENIAN)
			accent_tab[CC_inv_roof] = ACC_ROOF_INV | ACC_SUPERUP;
			accent_tab[c_inv_roof] = ACC_ROOF_INV;
			accent_tab[SS_inv_roof_latin] = ACC_ROOF_INV | ACC_SUPERUP;
			accent_tab[s_inv_roof_latin] = ACC_ROOF_INV;
			accent_tab[ZZ_inv_roof_latin] = ACC_ROOF_INV | ACC_SUPERUP;
			accent_tab[z_inv_roof_latin] = ACC_ROOF_INV;

		} // SLOVENIAN

		//  Конец фигурной скобки RUSSIAN перенес ниже  01.09.2000 E.P.
		if (language == LANGUAGE_GERMAN) {
			alphabet[AA_2dot_accent] = 1;
			alphabet[a_2dot_accent] = 1;
			alphabet[UU_2dot_accent] = 1;
			alphabet[u_2dot_accent] = 1;
			alphabet[OO_2dot_accent] = 1;
			alphabet[o_2dot_accent] = 1;
			alphabet[0xDF] = 1; // ??? small S-cet
		}
		if (language == LANGUAGE_FRENCH) {
			alphabet[AA_left_accent] = 1;
			alphabet[AA_roof_accent] = 1;
			alphabet[CC_bottom_accent] = 1;
			alphabet[EE_left_accent] = 1;
			alphabet[EE_right_accent] = 1;
			alphabet[EE_roof_accent] = 1;
			alphabet[EE_2dot_accent] = 1;
			alphabet[II_roof_accent] = 1;
			alphabet[II_2dot_accent] = 1;
			alphabet[OO_roof_accent] = 1;
			alphabet[OE_cap_deaf_sound] = 1;
			alphabet[AE_cap_deaf_sound] = 1;
			alphabet[UU_left_accent] = 1;
			alphabet[UU_roof_accent] = 1;
			alphabet[UU_2dot_accent] = 1;

			alphabet[a_left_accent] = 1;
			alphabet[a_roof_accent] = 1;
			alphabet[c_bottom_accent] = 1;
			alphabet[e_left_accent] = 1;
			alphabet[e_right_accent] = 1;
			alphabet[e_roof_accent] = 1;
			alphabet[e_2dot_accent] = 1;
			alphabet[i_roof_accent] = 1;
			alphabet[i_2dot_accent] = 1;
			alphabet[o_roof_accent] = 1;
			alphabet[oe_deaf_sound] = 1;
			alphabet[ae_deaf_sound] = 1;
			alphabet[u_left_accent] = 1;
			alphabet[u_roof_accent] = 1;
			alphabet[u_2dot_accent] = 1;
		}
		if (language == LANGUAGE_SWEDISH) {
			alphabet[AA_2dot_accent] = 1;
			alphabet[OO_2dot_accent] = 1;
			alphabet[AA_circle_accent] = 1;

			alphabet[a_2dot_accent] = 1;
			alphabet[o_2dot_accent] = 1;
			alphabet[a_circle_accent] = 1;
		}
		if (language == LANGUAGE_SPANISH) {
			alphabet[AA_right_accent] = 1;
			alphabet[EE_right_accent] = 1;
			alphabet[II_right_accent] = 1;
			alphabet[OO_right_accent] = 1;
			alphabet[UU_right_accent] = 1;
			alphabet[UU_2dot_accent] = 1;
			alphabet[NN_tild_accent] = 1;

			alphabet[a_right_accent] = 1;
			alphabet[e_right_accent] = 1;
			alphabet[i_right_accent] = 1;
			alphabet[o_right_accent] = 1;
			alphabet[u_right_accent] = 1;
			alphabet[u_2dot_accent] = 1;
			alphabet[n_tild_accent] = 1;
		}
		if (language == LANGUAGE_ITALIAN) {
			alphabet[AA_left_accent] = 1;
			alphabet[EE_left_accent] = 1;
			alphabet[EE_right_accent] = 1;
			alphabet[II_left_accent] = 1;
			alphabet[II_right_accent] = 1;
			alphabet[II_roof_accent] = 1;
			alphabet[OO_left_accent] = 1;
			alphabet[OO_right_accent] = 1;
			alphabet[UU_left_accent] = 1;
			alphabet[UU_right_accent] = 1;

			alphabet[a_left_accent] = 1;
			alphabet[e_left_accent] = 1;
			alphabet[e_right_accent] = 1;
			alphabet[i_left_accent] = 1;
			alphabet[i_right_accent] = 1;
			alphabet[i_roof_accent] = 1;
			alphabet[o_left_accent] = 1;
			alphabet[o_right_accent] = 1;
			alphabet[u_left_accent] = 1;
			alphabet[u_right_accent] = 1;
		}
		if (language == LANGUAGE_DANISH) {
			alphabet[AE_cap_deaf_sound] = 1;
			alphabet[AA_circle_accent] = 1;
			alphabet[OO_crossed] = 1;

			alphabet[ae_deaf_sound] = 1;
			alphabet[a_circle_accent] = 1;
			alphabet[o_crossed] = 1;
		}
		if (language == LANGUAGE_PORTUGUESE) {
			alphabet[AA_left_accent] = 1;
			alphabet[AA_right_accent] = 1;
			alphabet[AA_tild_accent] = 1;
			alphabet[AA_roof_accent] = 1;
			alphabet[CC_bottom_accent] = 1;
			alphabet[EE_left_accent] = 1;
			alphabet[EE_right_accent] = 1;
			alphabet[EE_2dot_accent] = 1;
			alphabet[EE_roof_accent] = 1;
			alphabet[OO_left_accent] = 1;
			alphabet[OO_right_accent] = 1;
			alphabet[OO_tild_accent] = 1;
			alphabet[OO_roof_accent] = 1;
			alphabet[II_right_accent] = 1;
			alphabet[II_2dot_accent] = 1;
			alphabet[UU_roof_accent] = 1;

			alphabet[a_left_accent] = 1;
			alphabet[a_right_accent] = 1;
			alphabet[a_tild_accent] = 1;
			alphabet[a_roof_accent] = 1;
			alphabet[c_bottom_accent] = 1;
			alphabet[e_left_accent] = 1;
			alphabet[e_right_accent] = 1;
			alphabet[e_2dot_accent] = 1;
			alphabet[e_roof_accent] = 1;
			alphabet[o_left_accent] = 1;
			alphabet[o_right_accent] = 1;
			alphabet[o_tild_accent] = 1;
			alphabet[o_roof_accent] = 1;
			alphabet[i_right_accent] = 1;
			alphabet[i_2dot_accent] = 1;
			alphabet[u_roof_accent] = 1;
		}
		if (language == LANGUAGE_DUTCH) {
			alphabet[AA_left_accent] = 1;
			alphabet[AA_2dot_accent] = 1;
			alphabet[AA_roof_accent] = 1;
			alphabet[CC_bottom_accent] = 1;
			alphabet[EE_left_accent] = 1;
			alphabet[EE_right_accent] = 1;
			alphabet[EE_2dot_accent] = 1;
			alphabet[EE_roof_accent] = 1;
			alphabet[OO_2dot_accent] = 1;
			alphabet[NN_tild_accent] = 1;
			alphabet[II_roof_accent] = 1;
			alphabet[II_2dot_accent] = 1;
			alphabet[UU_roof_accent] = 1;
			alphabet[UU_2dot_accent] = 1;

			alphabet[a_left_accent] = 1;
			alphabet[a_2dot_accent] = 1;
			alphabet[a_roof_accent] = 1;
			alphabet[c_bottom_accent] = 1;
			alphabet[e_left_accent] = 1;
			alphabet[e_right_accent] = 1;
			alphabet[e_2dot_accent] = 1;
			alphabet[e_roof_accent] = 1;
			alphabet[o_2dot_accent] = 1;
			alphabet[n_tild_accent] = 1;
			alphabet[i_roof_accent] = 1;
			alphabet[i_2dot_accent] = 1;
			alphabet[u_roof_accent] = 1;
			alphabet[u_2dot_accent] = 1;
		}

		// Прибалтийские языки. 09.07.2001 E.P.
		if (language == LANGUAGE_LATVIAN) {
			// LATVIAN 09.07.2001 E.P.
			alphabet[AA_macron] = alphabet[a_macron] = 1;
			alphabet[CC_inv_roof] = alphabet[c_inv_roof] = 1;
			alphabet[EE_macron] = alphabet[e_macron] = 1;
			alphabet[GG_bottom_accent] = alphabet[LATVIAN_g] = 1;
			alphabet[II_macron] = alphabet[i_macron] = 1;
			alphabet[KK_bottom_accent] = alphabet[k_bottom_accent] = 1;
			alphabet[LL_bottom_accent] = alphabet[l_bottom_accent] = 1;
			alphabet[NN_bottom_accent] = alphabet[n_bottom_accent] = 1;
			alphabet[SS_inv_roof_baltic] = alphabet[s_inv_roof_baltic] = 1;
			alphabet[UU_macron] = alphabet[u_macron] = 1;
			alphabet[ZZ_inv_roof_baltic] = alphabet[z_inv_roof_baltic] = 1;

			alphabet[(uchar) 'Q'] = alphabet[(uchar) 'q'] = 0;
			alphabet[(uchar) 'W'] = alphabet[(uchar) 'w'] = 0;
			alphabet[(uchar) 'X'] = alphabet[(uchar) 'x'] = 0;
			alphabet[(uchar) 'Y'] = alphabet[(uchar) 'y'] = 0;

			// base lines for diskrim (LATVIAN)
			let_linpos[AA_macron] = 1;
			let_linpos[a_macron] = 0x11;
			let_linpos[CC_inv_roof] = 1;
			let_linpos[c_inv_roof] = 0x11;
			let_linpos[EE_macron] = 1;
			let_linpos[e_macron] = 0x11;
			let_linpos[GG_bottom_accent] = 0x16;
			let_linpos[LATVIAN_g] = 0x52;
			let_linpos[II_macron] = 1;
			let_linpos[i_macron] = 0x11;
			let_linpos[KK_bottom_accent] = 0x16;
			let_linpos[k_bottom_accent] = 0x12;
			let_linpos[LL_bottom_accent] = 0x16;
			let_linpos[l_bottom_accent] = 0x12;
			let_linpos[NN_bottom_accent] = 0x16;
			let_linpos[n_bottom_accent] = 0x22;
			let_linpos[SS_inv_roof_baltic] = 1;
			let_linpos[s_inv_roof_baltic] = 0x11;
			let_linpos[UU_bottom_accent] = 0x16;
			let_linpos[u_bottom_accent] = 0x22;
			let_linpos[ZZ_inv_roof_baltic] = 1;
			let_linpos[z_inv_roof_baltic] = 0x11;
			/*
			 base lines for its determination (LATVIAN)

			 Старшая цифра: 20 - has twin, 10-круглая
			 Младшая цифра: определение базовых линий 1-bs1, 2-bs2, 4-bs3, 8-bs4

			 */
			let_lindef[AA_macron] = 4;
			let_lindef[a_macron] = 4;
			let_lindef[CC_inv_roof] = 0x34;
			let_lindef[c_inv_roof] = 0x35;
			let_lindef[EE_macron] = 4;
			let_lindef[e_macron] = 0x14;
			let_lindef[GG_bottom_accent] = 0x11;
			let_lindef[LATVIAN_g] = 0;
			let_lindef[II_macron] = 4;
			let_lindef[i_macron] = 4;
			let_lindef[KK_bottom_accent] = 1;
			let_lindef[k_bottom_accent] = 1;
			let_lindef[LL_bottom_accent] = 1;
			let_lindef[l_bottom_accent] = 1;
			let_lindef[NN_bottom_accent] = 1;
			let_lindef[n_bottom_accent] = 2;
			let_lindef[SS_inv_roof_baltic] = 0x24;
			let_lindef[s_inv_roof_baltic] = 0x25;
			let_lindef[UU_bottom_accent] = 0x31;
			let_lindef[u_bottom_accent] = 0x32;
			let_lindef[ZZ_inv_roof_baltic] = 0x24;
			let_lindef[z_inv_roof_baltic] = 0x25;

			// detems klasres for 3x5 (LATVIAN)
			// 1 - small, 2 - capital, 4 - low, 8 - other
			let_lincomp[AA_macron] = 2;
			let_lincomp[a_macron] = 2;
			let_lincomp[CC_inv_roof] = 2;
			let_lincomp[c_inv_roof] = 2;
			let_lincomp[EE_macron] = 2;
			let_lincomp[e_macron] = 2;
			let_lincomp[GG_bottom_accent] = 0x0e;
			let_lincomp[LATVIAN_g] = 0x0e;
			let_lincomp[II_macron] = 2;
			let_lincomp[i_macron] = 2;
			let_lincomp[KK_bottom_accent] = 0x0e;
			let_lincomp[k_bottom_accent] = 0x0e;
			let_lincomp[LL_bottom_accent] = 0x0e;
			let_lincomp[l_bottom_accent] = 0x0e;
			let_lincomp[NN_bottom_accent] = 0x0e;
			let_lincomp[n_bottom_accent] = 0x0d;
			let_lincomp[SS_inv_roof_baltic] = 2;
			let_lincomp[s_inv_roof_baltic] = 2;
			let_lincomp[UU_bottom_accent] = 0x0e;
			let_lincomp[u_bottom_accent] = 0x0d;
			let_lincomp[ZZ_inv_roof_baltic] = 2;
			let_lincomp[z_inv_roof_baltic] = 2;
			/*
			 shape of letter (LATVIAN)

			 1 - capital
			 2 - sticky
			 4 - stick allows to define bs1
			 8 - small
			 0x10 - may be above b1 ($)
			 0x20 - d,b,h (may be above b1)
			 */
			let_linshape[AA_macron] = 0x11;
			let_linshape[a_macron] = 1;
			let_linshape[CC_inv_roof] = 0x11;
			let_linshape[c_inv_roof] = 1;
			let_linshape[EE_macron] = 0x11;
			let_linshape[e_macron] = 1;
			let_linshape[GG_bottom_accent] = 1;
			let_linshape[LATVIAN_g] = 0x11;
			let_linshape[II_macron] = 0x17;
			let_linshape[i_macron] = 0x0e;
			let_linshape[KK_bottom_accent] = 1;
			let_linshape[k_bottom_accent] = 8;
			let_linshape[LL_bottom_accent] = 1;
			let_linshape[l_bottom_accent] = 0x0e;
			let_linshape[NN_bottom_accent] = 1;
			let_linshape[n_bottom_accent] = 8;
			let_linshape[SS_inv_roof_baltic] = 0x11;
			let_linshape[s_inv_roof_baltic] = 1;
			let_linshape[UU_bottom_accent] = 1;
			let_linshape[u_bottom_accent] = 8;
			let_linshape[ZZ_inv_roof_baltic] = 0x11;
			let_linshape[z_inv_roof_baltic] = 1;

			// for accent is used in abris (LATVIAN)
			let_sans_acc[AA_macron] = 'A';
			let_sans_acc[a_macron] = 'a';
			let_sans_acc[CC_inv_roof] = 'C';
			let_sans_acc[c_inv_roof] = 'c';
			let_sans_acc[EE_macron] = 'E';
			let_sans_acc[e_macron] = 'e';
			let_sans_acc[GG_bottom_accent] = 'G';
			let_sans_acc[LATVIAN_g] = 'g';
			let_sans_acc[II_macron] = 'I';
			let_sans_acc[i_macron] = 'i';
			let_sans_acc[KK_bottom_accent] = 'K';
			let_sans_acc[k_bottom_accent] = 'k';
			let_sans_acc[LL_bottom_accent] = 'L';
			let_sans_acc[l_bottom_accent] = 'l';
			let_sans_acc[NN_bottom_accent] = 'N';
			let_sans_acc[n_bottom_accent] = 'n';
			let_sans_acc[SS_inv_roof_baltic] = 'S';
			let_sans_acc[s_inv_roof_baltic] = 's';
			let_sans_acc[UU_bottom_accent] = 'U';
			let_sans_acc[u_bottom_accent] = 'u';
			let_sans_acc[ZZ_inv_roof_baltic] = 'Z';
			let_sans_acc[z_inv_roof_baltic] = 'z';

			// LATVIAN
			// lindef3 == lindef when b3 made, so twins could be resolved (as pP, yY)

			let_lindef3[AA_macron] = let_lindef[AA_macron];
			let_lindef3[a_macron] = let_lindef[a_macron];
			let_lindef3[CC_inv_roof] = let_lindef[CC_inv_roof];
			let_lindef3[c_inv_roof] = let_lindef[c_inv_roof];
			let_lindef3[EE_macron] = let_lindef[EE_macron];
			let_lindef3[e_macron] = let_lindef[e_macron];
			let_lindef3[GG_bottom_accent] = let_lindef[GG_bottom_accent];
			let_lindef3[LATVIAN_g] = let_lindef[LATVIAN_g];
			let_lindef3[II_macron] = let_lindef[II_macron];
			let_lindef3[i_macron] = let_lindef[i_macron];
			let_lindef3[KK_bottom_accent] = let_lindef[KK_bottom_accent];
			let_lindef3[k_bottom_accent] = let_lindef[k_bottom_accent];
			let_lindef3[LL_bottom_accent] = let_lindef[LL_bottom_accent];
			let_lindef3[l_bottom_accent] = let_lindef[l_bottom_accent];
			let_lindef3[NN_bottom_accent] = let_lindef[NN_bottom_accent];
			let_lindef3[n_bottom_accent] = let_lindef[n_bottom_accent];
			let_lindef3[SS_inv_roof_baltic] = let_lindef[SS_inv_roof_baltic];
			let_lindef3[s_inv_roof_baltic] = let_lindef[s_inv_roof_baltic];
			let_lindef3[UU_bottom_accent] = let_lindef[UU_bottom_accent];
			let_lindef3[u_bottom_accent] = let_lindef[u_bottom_accent];
			let_lindef3[ZZ_inv_roof_baltic] = let_lindef[ZZ_inv_roof_baltic];
			let_lindef3[z_inv_roof_baltic] = let_lindef[z_inv_roof_baltic];

			// LATVIAN
			accent_tab[AA_macron] = ACC_MACRON | ACC_SUPERUP;
			accent_tab[a_macron] = ACC_MACRON;
			accent_tab[CC_inv_roof] = ACC_ROOF_INV | ACC_SUPERUP;
			accent_tab[c_inv_roof] = ACC_ROOF_INV;
			accent_tab[EE_macron] = ACC_MACRON | ACC_SUPERUP;
			accent_tab[e_macron] = ACC_MACRON;
			accent_tab[GG_bottom_accent] = 0;
			accent_tab[LATVIAN_g] = ACC_DOT | ACC_LEFT | ACC_RIGHT;
			accent_tab[II_macron] = ACC_MACRON | ACC_SUPERUP;
			accent_tab[i_macron] = ACC_MACRON;
			accent_tab[KK_bottom_accent] = 0;
			accent_tab[k_bottom_accent] = 0;
			accent_tab[LL_bottom_accent] = 0;
			accent_tab[l_bottom_accent] = 0;
			accent_tab[NN_bottom_accent] = 0;
			accent_tab[n_bottom_accent] = 0;
			accent_tab[SS_inv_roof_baltic] = ACC_ROOF_INV | ACC_SUPERUP;
			accent_tab[s_inv_roof_baltic] = ACC_ROOF_INV;
			accent_tab[UU_bottom_accent] = 0;
			accent_tab[u_bottom_accent] = 0;
			accent_tab[ZZ_inv_roof_baltic] = ACC_ROOF_INV | ACC_SUPERUP;
			accent_tab[z_inv_roof_baltic] = ACC_ROOF_INV;

		} // LATVIAN

		if (language == LANGUAGE_LITHUANIAN) {

			// LITHUANIAN 09.07.2001 E.P.
			alphabet[AA_bottom_accent] = 1;
			alphabet[a_bottom_accent] = 1;
			alphabet[CC_inv_roof] = 1;
			alphabet[c_inv_roof] = 1;
			alphabet[EE_bottom_accent] = 1;
			alphabet[e_bottom_accent] = 1;
			alphabet[EE_dot_accent] = 1;
			alphabet[e_dot_accent] = 1;
			alphabet[II_bottom_accent] = 1;
			alphabet[i_bottom_accent] = 1;
			alphabet[SS_inv_roof_baltic] = 1;
			alphabet[s_inv_roof_baltic] = 1;
			alphabet[UU_bottom_accent] = 1;
			alphabet[u_bottom_accent] = 1;
			alphabet[UU_macron] = 1;
			alphabet[u_macron] = 1;
			alphabet[ZZ_inv_roof_baltic] = 1;
			alphabet[z_inv_roof_baltic] = 1;

			alphabet[(uchar) 'Q'] = alphabet[(uchar) 'q'] = 0;
			alphabet[(uchar) 'W'] = alphabet[(uchar) 'w'] = 0;
			alphabet[(uchar) 'X'] = alphabet[(uchar) 'x'] = 0;

			// base lines for diskrim (LITHUANIAN)
			let_linpos[AA_bottom_accent] = 0x16;
			let_linpos[a_bottom_accent] = 0x22;
			let_linpos[CC_inv_roof] = 1;
			let_linpos[c_inv_roof] = 0x11;
			let_linpos[EE_bottom_accent] = 0x16;
			let_linpos[e_bottom_accent] = 0x22;
			let_linpos[EE_dot_accent] = 1;
			let_linpos[e_dot_accent] = 0x11;
			let_linpos[II_bottom_accent] = 0x16;
			let_linpos[i_bottom_accent] = 0x22;
			let_linpos[SS_inv_roof_baltic] = 1;
			let_linpos[s_inv_roof_baltic] = 0x11;
			let_linpos[UU_bottom_accent] = 0x16;
			let_linpos[u_bottom_accent] = 0x22;
			let_linpos[UU_macron] = 1;
			let_linpos[u_macron] = 0x11;
			let_linpos[ZZ_inv_roof_baltic] = 1;
			let_linpos[z_inv_roof_baltic] = 0x11;

			/*
			 base lines for its determination (LITHUANIAN)

			 Старшая цифра: 20 - has twin, 10-круглая
			 Младшая цифра: определение базовых линий 1-bs1, 2-bs2, 4-bs3, 8-bs4

			 Макрон или точка над малой буквой дает bs1.
			 */
			let_lindef[AA_bottom_accent] = 1;
			let_lindef[a_bottom_accent] = 0x12;
			let_lindef[CC_inv_roof] = 0x34;
			let_lindef[c_inv_roof] = 0x35;
			let_lindef[EE_bottom_accent] = 1;
			let_lindef[e_bottom_accent] = 0x12;
			let_lindef[EE_dot_accent] = 4;
			let_lindef[e_dot_accent] = 0x14;
			let_lindef[II_bottom_accent] = 1;
			let_lindef[i_bottom_accent] = 1;
			let_lindef[SS_inv_roof_baltic] = 0x34;
			let_lindef[s_inv_roof_baltic] = 0x35;
			let_lindef[UU_bottom_accent] = 0x31;
			let_lindef[u_bottom_accent] = 0x32;
			let_lindef[UU_macron] = 0x34;
			let_lindef[u_macron] = 0x34;
			let_lindef[ZZ_inv_roof_baltic] = 0x24;
			let_lindef[z_inv_roof_baltic] = 0x25;

			// detems klasres for 3x5 (LITHUANIAN)
			// 1 - small, 2 - capital, 4 - low, 8 - other
			let_lincomp[AA_bottom_accent] = 0x0e;
			let_lincomp[a_bottom_accent] = 0x0d;
			let_lincomp[CC_inv_roof] = 2;
			let_lincomp[c_inv_roof] = 2;
			let_lincomp[EE_bottom_accent] = 0x0e;
			let_lincomp[e_bottom_accent] = 0x0d;
			let_lincomp[EE_dot_accent] = 2;
			let_lincomp[e_dot_accent] = 2;
			let_lincomp[II_bottom_accent] = 0x0e;
			let_lincomp[i_bottom_accent] = 0x0e;
			let_lincomp[SS_inv_roof_baltic] = 2;
			let_lincomp[s_inv_roof_baltic] = 2;
			let_lincomp[UU_bottom_accent] = 0x0e;
			let_lincomp[u_bottom_accent] = 0x0d;
			let_lincomp[UU_macron] = 2;
			let_lincomp[u_macron] = 2;
			let_lincomp[ZZ_inv_roof_baltic] = 2;
			let_lincomp[z_inv_roof_baltic] = 2;

			/*
			 shape of letter (LITHUANIAN)

			 1 - capital
			 2 - sticky
			 4 - stick allows to define bs1
			 8 - small
			 0x10 - may be above b1 ($)
			 0x20 - d,b,h (may be above b1)
			 */
			let_linshape[AA_bottom_accent] = 1;
			let_linshape[a_bottom_accent] = 8;
			let_linshape[CC_inv_roof] = 0x11;
			let_linshape[c_inv_roof] = 1;
			let_linshape[EE_bottom_accent] = 1;
			let_linshape[e_bottom_accent] = 8;
			let_linshape[EE_dot_accent] = 0x11;
			let_linshape[e_dot_accent] = 1;
			let_linshape[II_bottom_accent] = 7;
			let_linshape[i_bottom_accent] = 0x0e;
			let_linshape[SS_inv_roof_baltic] = 0x11;
			let_linshape[s_inv_roof_baltic] = 1;
			let_linshape[UU_bottom_accent] = 1;
			let_linshape[u_bottom_accent] = 8;
			let_linshape[UU_macron] = 0x11;
			let_linshape[u_macron] = 1;
			let_linshape[ZZ_inv_roof_baltic] = 0x11;
			let_linshape[z_inv_roof_baltic] = 1;

			// for accent is used in abris (LITHUANIAN)
			let_sans_acc[AA_bottom_accent] = 'A';
			let_sans_acc[a_bottom_accent] = 'a';
			let_sans_acc[CC_inv_roof] = 'C';
			let_sans_acc[c_inv_roof] = 'c';
			let_sans_acc[EE_bottom_accent] = 'E';
			let_sans_acc[e_bottom_accent] = 'e';
			let_sans_acc[EE_dot_accent] = 'E';
			let_sans_acc[e_dot_accent] = 'e';
			let_sans_acc[II_bottom_accent] = 'I';
			let_sans_acc[i_bottom_accent] = 'i';
			let_sans_acc[SS_inv_roof_baltic] = 'S';
			let_sans_acc[s_inv_roof_baltic] = 's';
			let_sans_acc[UU_bottom_accent] = 'U';
			let_sans_acc[u_bottom_accent] = 'u';
			let_sans_acc[UU_macron] = 'U';
			let_sans_acc[u_macron] = 'u';
			let_sans_acc[ZZ_inv_roof_baltic] = 'Z';
			let_sans_acc[z_inv_roof_baltic] = 'z';

			// LITHUANIAN
			// lindef3 == lindef when b3 made, so twins could be resolved (as pP, yY)
			let_lindef3[AA_bottom_accent] = let_lindef[AA_bottom_accent];
			let_lindef3[a_bottom_accent] = let_lindef[a_bottom_accent];
			let_lindef3[CC_inv_roof] = let_lindef[CC_inv_roof];
			let_lindef3[c_inv_roof] = let_lindef[c_inv_roof];
			let_lindef3[EE_bottom_accent] = let_lindef[EE_bottom_accent];
			let_lindef3[e_bottom_accent] = let_lindef[e_bottom_accent];
			let_lindef3[EE_dot_accent] = let_lindef[EE_dot_accent];
			let_lindef3[e_dot_accent] = let_lindef[e_dot_accent];
			let_lindef3[II_bottom_accent] = let_lindef[II_bottom_accent];
			let_lindef3[i_bottom_accent] = let_lindef[i_bottom_accent];
			let_lindef3[SS_inv_roof_baltic] = let_lindef[SS_inv_roof_baltic];
			let_lindef3[s_inv_roof_baltic] = let_lindef[s_inv_roof_baltic];
			let_lindef3[UU_bottom_accent] = let_lindef[UU_bottom_accent];
			let_lindef3[u_bottom_accent] = let_lindef[u_bottom_accent];
			let_lindef3[UU_macron] = let_lindef[UU_macron];
			let_lindef3[u_macron] = let_lindef[u_macron];
			let_lindef3[ZZ_inv_roof_baltic] = let_lindef[ZZ_inv_roof_baltic];
			let_lindef3[z_inv_roof_baltic] = let_lindef[z_inv_roof_baltic];

			// LITHUANIAN
			accent_tab[AA_bottom_accent] = 0;
			accent_tab[a_bottom_accent] = 0;
			accent_tab[CC_inv_roof] = ACC_ROOF_INV | ACC_SUPERUP;
			accent_tab[c_inv_roof] = ACC_ROOF_INV;
			accent_tab['E'] = ACC_DOT | ACC_SUPERUP; // don't discrim upper dust, like ij. See discrid() in DM1.C
			accent_tab['e'] = ACC_DOT; // don't discrim upper dust, like ij. See discrid() in DM1.C
			accent_tab[EE_bottom_accent] = 0;
			accent_tab[e_bottom_accent] = 0;
			accent_tab[EE_dot_accent] = ACC_DOT | ACC_SUPERUP;
			accent_tab[e_dot_accent] = ACC_DOT;
			accent_tab[II_bottom_accent] = 0;
			accent_tab[i_bottom_accent] = 0;
			accent_tab[SS_inv_roof_baltic] = ACC_ROOF_INV | ACC_SUPERUP;
			accent_tab[s_inv_roof_baltic] = ACC_ROOF_INV;
			accent_tab[UU_bottom_accent] = 0;
			accent_tab[u_bottom_accent] = 0;
			accent_tab[UU_macron] = ACC_MACRON | ACC_SUPERUP;
			accent_tab[u_macron] = ACC_MACRON;
			accent_tab[ZZ_inv_roof_baltic] = ACC_ROOF_INV | ACC_SUPERUP;
			accent_tab[z_inv_roof_baltic] = ACC_ROOF_INV;

		} // LITHUANIAN

		if (language == LANGUAGE_ESTONIAN) {

			// ESTONIAN 09.07.2001 E.P.
			alphabet[AA_2dot_accent] = 1;
			alphabet[a_2dot_accent] = 1;
			alphabet[OO_tild_accent] = 1;
			alphabet[o_tild_accent] = 1;
			alphabet[OO_2dot_accent] = 1;
			alphabet[o_2dot_accent] = 1;
			alphabet[SS_inv_roof_baltic] = 1;
			alphabet[s_inv_roof_baltic] = 1;
			alphabet[UU_2dot_accent] = 1;
			alphabet[u_2dot_accent] = 1;
			alphabet[ZZ_inv_roof_baltic] = 1;
			alphabet[z_inv_roof_baltic] = 1;

			alphabet[(uchar) 'C'] = alphabet[(uchar) 'c'] = 0;
			alphabet[(uchar) 'Q'] = alphabet[(uchar) 'q'] = 0;
			alphabet[(uchar) 'W'] = alphabet[(uchar) 'w'] = 0;
			alphabet[(uchar) 'X'] = alphabet[(uchar) 'x'] = 0;
			alphabet[(uchar) 'Y'] = alphabet[(uchar) 'y'] = 0;

			// base lines for diskrim (ESTONIAN)
			let_linpos[AA_2dot_accent] = 1;
			let_linpos[a_2dot_accent] = 0x11;
			let_linpos[OO_tild_accent] = 1;
			let_linpos[o_tild_accent] = 0x11;
			let_linpos[OO_2dot_accent] = 1;
			let_linpos[o_2dot_accent] = 0x11;
			let_linpos[SS_inv_roof_baltic] = 1;
			let_linpos[s_inv_roof_baltic] = 0x11;
			let_linpos[UU_2dot_accent] = 1;
			let_linpos[u_2dot_accent] = 0x11;
			let_linpos[ZZ_inv_roof_baltic] = 1;
			let_linpos[z_inv_roof_baltic] = 0x11;

			/*
			 base lines for its determination (ESTONIAN)

			 Старшая цифра: 20 - has twin, 10-круглая
			 Младшая цифра: определение базовых линий 1-bs1, 2-bs2, 4-bs3, 8-bs4

			 Макрон или точка над малой буквой дает bs1.
			 */
			let_lindef[AA_2dot_accent] = 4;
			let_lindef[a_2dot_accent] = 0x14;
			let_lindef[OO_tild_accent] = 0x34;
			let_lindef[o_tild_accent] = 0x34;
			let_lindef[OO_2dot_accent] = 0x34;
			let_lindef[o_2dot_accent] = 0x34;
			let_lindef[SS_inv_roof_baltic] = 0x34;
			let_lindef[s_inv_roof_baltic] = 0x35;
			let_lindef[UU_2dot_accent] = 0x34;
			let_lindef[u_2dot_accent] = 0x34;
			let_lindef[ZZ_inv_roof_baltic] = 0x24;
			let_lindef[z_inv_roof_baltic] = 0x25;

			// detems klasres for 3x5 (ESTONIAN)
			// 1 - small, 2 - capital, 4 - low, 8 - other
			let_lincomp[AA_2dot_accent] = 2;
			let_lincomp[a_2dot_accent] = 2;
			let_lincomp[OO_tild_accent] = 2;
			let_lincomp[o_tild_accent] = 2;
			let_lincomp[OO_2dot_accent] = 2;
			let_lincomp[o_2dot_accent] = 2;
			let_lincomp[SS_inv_roof_baltic] = 2;
			let_lincomp[s_inv_roof_baltic] = 2;
			let_lincomp[UU_2dot_accent] = 2;
			let_lincomp[u_2dot_accent] = 2;
			let_lincomp[ZZ_inv_roof_baltic] = 2;
			let_lincomp[z_inv_roof_baltic] = 2;
			/*
			 shape of letter (ESTONIAN)

			 1 - capital
			 2 - sticky
			 4 - stick allows to define bs1
			 8 - small
			 0x10 - may be above b1 ($)
			 0x20 - d,b,h (may be above b1)
			 */
			let_linshape[AA_2dot_accent] = 0x11;
			let_linshape[a_2dot_accent] = 1;
			let_linshape[OO_tild_accent] = 0x11;
			let_linshape[o_tild_accent] = 1;
			let_linshape[OO_2dot_accent] = 0x11;
			let_linshape[o_2dot_accent] = 1;
			let_linshape[SS_inv_roof_baltic] = 0x11;
			let_linshape[s_inv_roof_baltic] = 1;
			let_linshape[UU_2dot_accent] = 0x11;
			let_linshape[u_2dot_accent] = 1;
			let_linshape[ZZ_inv_roof_baltic] = 0x11;
			let_linshape[z_inv_roof_baltic] = 1;

			// for accent is used in abris (ESTONIAN)
			let_sans_acc[AA_2dot_accent] = 'A';
			let_sans_acc[a_2dot_accent] = 'a';
			let_sans_acc[OO_tild_accent] = 'O';
			let_sans_acc[o_tild_accent] = 'o';
			let_sans_acc[OO_2dot_accent] = 'O';
			let_sans_acc[o_2dot_accent] = 'o';
			let_sans_acc[SS_inv_roof_baltic] = 'S';
			let_sans_acc[s_inv_roof_baltic] = 's';
			let_sans_acc[UU_2dot_accent] = 'U';
			let_sans_acc[u_2dot_accent] = 'u';
			let_sans_acc[ZZ_inv_roof_baltic] = 'Z';
			let_sans_acc[z_inv_roof_baltic] = 'z';

			// ESTONIAN
			// lindef3 == lindef when b3 made, so twins could be resolved (as pP, yY)
			let_lindef3[AA_2dot_accent] = let_lindef[AA_2dot_accent];
			let_lindef3[a_2dot_accent] = let_lindef[a_2dot_accent];
			let_lindef3[OO_tild_accent] = let_lindef[OO_tild_accent];
			let_lindef3[o_tild_accent] = let_lindef[o_tild_accent];
			let_lindef3[OO_2dot_accent] = let_lindef[OO_2dot_accent];
			let_lindef3[o_2dot_accent] = let_lindef[o_2dot_accent];
			let_lindef3[SS_inv_roof_baltic] = let_lindef[SS_inv_roof_baltic];
			let_lindef3[s_inv_roof_baltic] = let_lindef[s_inv_roof_baltic];
			let_lindef3[UU_2dot_accent] = let_lindef[UU_2dot_accent];
			let_lindef3[u_2dot_accent] = let_lindef[u_2dot_accent];
			let_lindef3[ZZ_inv_roof_baltic] = let_lindef[ZZ_inv_roof_baltic];
			let_lindef3[z_inv_roof_baltic] = let_lindef[z_inv_roof_baltic];

			// ESTONIAN
			accent_tab[AA_2dot_accent] = ACC_2DOT | ACC_SUPERUP;
			accent_tab[a_2dot_accent] = ACC_2DOT;
			accent_tab[OO_tild_accent] = ACC_TILD | ACC_SUPERUP;
			accent_tab[o_tild_accent] = ACC_TILD;
			accent_tab[OO_2dot_accent] = ACC_2DOT | ACC_SUPERUP;
			accent_tab[o_2dot_accent] = ACC_2DOT;
			accent_tab[SS_inv_roof_baltic] = ACC_ROOF_INV | ACC_SUPERUP;
			accent_tab[s_inv_roof_baltic] = ACC_ROOF_INV;
			accent_tab[UU_2dot_accent] = ACC_2DOT | ACC_SUPERUP;
			accent_tab[u_2dot_accent] = ACC_2DOT;
			accent_tab[ZZ_inv_roof_baltic] = ACC_ROOF_INV | ACC_SUPERUP;
			accent_tab[z_inv_roof_baltic] = ACC_ROOF_INV;

		} // ESTONIAN

		// Турецкий язык. 20.05.2002 E.P.
		if (language == LANGUAGE_TURKISH) {
			// Турецкие лиги, отличные от западноевропейских

			alphabet[liga_TM] = 1;
			alphabet[liga_CC] = 1;
			alphabet[liga_CR] = 1;
			alphabet[liga_bull] = 1;

			accent_tab[liga_TM] = accent_tab[liga_TM_usual];
			accent_tab[liga_CC] = accent_tab[liga_CC_usual];
			accent_tab[liga_CR] = accent_tab[liga_CR_usual];
			accent_tab[liga_bull] = accent_tab[liga_bull_usual];

			let_linpos[liga_TM] = let_linpos[liga_TM_usual];
			let_linpos[liga_CC] = let_linpos[liga_CC_usual];
			let_linpos[liga_CR] = let_linpos[liga_CR_usual];
			let_linpos[liga_bull] = let_linpos[liga_bull_usual];

			let_lindef[liga_TM] = let_lindef[liga_TM_usual];
			let_lindef[liga_CC] = let_lindef[liga_CC_usual];
			let_lindef[liga_CR] = let_lindef[liga_CR_usual];
			let_lindef[liga_bull] = let_lindef[liga_bull_usual];

			let_lincomp[liga_TM] = let_lincomp[liga_TM_usual];
			let_lincomp[liga_CC] = let_lincomp[liga_CC_usual];
			let_lincomp[liga_CR] = let_lincomp[liga_CR_usual];
			let_lincomp[liga_bull] = let_lincomp[liga_bull_usual];

			let_linshape[liga_TM] = let_linshape[liga_TM_usual];
			let_linshape[liga_CC] = let_linshape[liga_CC_usual];
			let_linshape[liga_CR] = let_linshape[liga_CR_usual];
			let_linshape[liga_bull] = let_linshape[liga_bull_usual];

			let_lindef3[liga_TM] = let_lindef3[liga_TM_usual];
			let_lindef3[liga_CC] = let_lindef3[liga_CC_usual];
			let_lindef3[liga_CR] = let_lindef3[liga_CR_usual];
			let_lindef3[liga_bull] = let_lindef3[liga_bull_usual];

			// Турецкие буквы, отличные от западноевропейских
			alphabet[GG_semicircle] = 1;
			alphabet[g_semicircle] = 1;
			alphabet[II_dot_accent] = 1;
			alphabet[i_sans_accent] = 1;
			alphabet[SS_bottom_accent_turkish] = 1;
			alphabet[s_bottom_accent_turkish] = 1;
			alphabet[(uchar) 'Q'] = alphabet[(uchar) 'q'] = 0;
			alphabet[(uchar) 'W'] = alphabet[(uchar) 'w'] = 0;
			alphabet[(uchar) 'X'] = alphabet[(uchar) 'x'] = 0;

			// Для линейного критерия (TURKISH)
			let_linpos[GG_semicircle] = 1;
			let_linpos[g_semicircle] = 0x52; // 27.06.2002 E.P.
			let_linpos[II_dot_accent] = 0x11; // 27.06.2002 E.P.
			let_linpos[i_sans_accent] = 0x21;
			let_linpos[SS_bottom_accent_turkish] = 0x16;
			let_linpos[s_bottom_accent_turkish] = 0x22;

			// Для определения базовых линий (TURKISH)
			// Старшая цифра: 20 - есть близнец, 10-круглая
			// Младшая цифра: определение базовых линий
			// 1-bs1, 2-bs2, 4-bs3, 8-bs4
			let_lindef[GG_semicircle] = 0x14; // 10 - круглая, 4 - bs3
			let_lindef[g_semicircle] = 0; //
			let_lindef[II_dot_accent] = 0x24; // 20 - есть близнец 'i', 4 - bs3 // 27.06.2002 E.P.
			let_lindef[i_sans_accent] = 0x26; // 20 - есть близнец 'I', 2 - bs2, 4 - bs3
			let_lindef['I'] = 0x25; // 20 - есть близнец i_sans_accent, 1 - bs1, 4 - bs3
			let_lindef['i'] = 0x24; // 20 - есть близнец II_dot_accent, 4 - bs3
			let_lindef[SS_bottom_accent_turkish] = 0x31; // 20 - есть близнец, 10 - круглая, 1 - bs1
			let_lindef[s_bottom_accent_turkish] = 0x32; // 20 - есть близнец, 10 - круглая, 2 - bs2

			// Допустимый размер буквы, определяет кластер для 3x5 (TURKISH)
			// 1 - small; 2 - capital, 4 - low (like 'y'), 8 - other
			let_lincomp[GG_semicircle] = 2;
			let_lincomp[g_semicircle] = 0x0e; // 2 - capital, 4 - low, 8 - other
			let_lincomp[II_dot_accent] = 2;
			let_lincomp[i_sans_accent] = 1;
			let_lincomp[SS_bottom_accent_turkish] = 0x0e; // 2 - capital, 4 - low, 8 - other
			let_lincomp[s_bottom_accent_turkish] = 0x0d; // 1 - small,   4 - low, 8 - other

			/*
			 Форма буквы (TURKISH):
			 1 - прописная
			 2 - палка
			 4 - палка позволяет определить bs1
			 8 - строчная
			 0x10 - может быть выше b1 ($)
			 0x20 - d,b,h (может быть выше b1)
			 */

			let_linshape[GG_semicircle] = 0x11; // 10 - может быть выше b1, 1 - прописная
			let_linshape[g_semicircle] = 8; // 8 - строчная

			// 10 - может быть выше b1, 1 - прописная, 2 - палка,
			// 4 - палка позволяет определить bs1
			let_linshape[II_dot_accent] = 0x17;

			// 8 - строчная, 2 - палка, 4 - палка позволяет определить bs1
			let_linshape[i_sans_accent] = 0x0e;

			let_linshape[SS_bottom_accent_turkish] = 1; // 1 - прописная
			let_linshape[s_bottom_accent_turkish] = 8; // 8 - строчная

			// Буква без акцента (TURKISH)
			let_sans_acc[GG_semicircle] = 'G';
			let_sans_acc[g_semicircle] = 'g';
			let_sans_acc[II_dot_accent] = 'I';
			let_sans_acc[i_sans_accent] = i_sans_accent;
			let_sans_acc['i'] = i_sans_accent;
			let_sans_acc[SS_bottom_accent_turkish] = 'S';
			let_sans_acc[s_bottom_accent_turkish] = 's';

			// Неизвестно что (TURKISH)
			let_lindef3[GG_semicircle] = 0;
			let_lindef3[g_semicircle] = 0;
			let_lindef3[II_dot_accent] = 0;
			let_lindef3[i_sans_accent] = 0;
			let_lindef3[SS_bottom_accent_turkish] = 0;
			let_lindef3[s_bottom_accent_turkish] = 0;

			// Тип верхнего акцента (TURKISH)
			accent_tab[GG_semicircle] = ACC_ROOF_INV | ACC_SUPERUP;
			accent_tab[g_semicircle] = ACC_ROOF_INV;
			accent_tab[II_dot_accent] = ACC_DOT;
			accent_tab[i_sans_accent] = 0;
			accent_tab[SS_bottom_accent_turkish] = 0;
			accent_tab[s_bottom_accent_turkish] = 0;
		}

	} // !LANGUAGE_RUSSIAN

	// for all code pages :
	strcpy((char*) decode_ASCII_to_[(uchar) '!'], "!");
	strcpy((char*) decode_ASCII_to_[(uchar) '?'], "?");
	//strcpy((char*) decode_ASCII_to_[(uchar) liga_TM], "Щ");
	strcpy((char*) decode_ASCII_to_[(uchar) liga_TM], "\x99");
	//strcpy((char*) decode_ASCII_to_[(uchar) liga_CC], "й");
	strcpy((char*) decode_ASCII_to_[(uchar) liga_CC], "\xA9");
	//strcpy((char*) decode_ASCII_to_[(uchar) liga_CR], "о");
	strcpy((char*) decode_ASCII_to_[(uchar) liga_CR], "\xAE");
	strcpy((char*) decode_ASCII_to_[(uchar) SS_NEG_HALF_SPACE], "");
	strcpy((char*) decode_ASCII_to_[(uchar) SS_POS_HALF_SPACE], " ");

	if (CodePages[language] == CHARSET_ANSI) {
		strcpy((char*) decode_ASCII_to_[(uchar) liga_fi], "fi");
		strcpy((char*) decode_ASCII_to_[(uchar) liga_fl], "fl");
		strcpy((char*) decode_ASCII_to_[(uchar) liga_ff], "ff");
		strcpy((char*) decode_ASCII_to_[(uchar) liga_ffi], "ffi");
		strcpy((char*) decode_ASCII_to_[(uchar) liga_rt], "rt");
		strcpy((char*) decode_ASCII_to_[(uchar) liga_ri], "ri");
		strcpy((char*) decode_ASCII_to_[(uchar) liga_uperc], "%%u");
		strcpy((char*) decode_ASCII_to_[(uchar) liga_lperc], "%%d");
		strcpy((char*) decode_ASCII_to_[(uchar) liga_ffl], "ffl");
		strcpy((char*) decode_ASCII_to_[(uchar) liga_i], "_i_");
		strcpy((char*) decode_ASCII_to_[(uchar) liga_j], "_j_");
		strcpy((char*) decode_ASCII_to_[(uchar) liga_exm], "_!_");
		strcpy((char*) decode_ASCII_to_[(uchar) liga_qm], "_?_");
		strcpy((char*) decode_ASCII_to_[(uchar) liga_inv_exm], "_\xA2_");
		strcpy((char*) decode_ASCII_to_[(uchar) liga_inv_qm], "_\xBF_");
		strcpy((char*) decode_ASCII_to_[(uchar) liga_bull], "\xB0");

#ifndef SPEC_PROJECT1
		//strcpy((char*) decode_ASCII_to_[(uchar) left_quocket], "«");
		strcpy((char*) decode_ASCII_to_[(uchar) left_quocket], "\xAB");
#else
		strcpy((char*) decode_ASCII_to_[left_quocket],"<<");
#endif
		//strcpy((char*) decode_ASCII_to_[(uchar) right_quocket], "»");
		strcpy((char*) decode_ASCII_to_[(uchar) right_quocket], "\xBB");
		//strcpy((char*) decode_ASCII_to_[(uchar) low_quotes], "„");
		strcpy((char*) decode_ASCII_to_[(uchar) low_quotes], "\x84");
		strcpy((char*) decode_ASCII_to_[(uchar) liga_rt], "rt");
		strcpy((char*) decode_ASCII_to_[(uchar) AA_left_accent], "\xC0");
		strcpy((char*) decode_ASCII_to_[(uchar) a_left_accent], "\xE0");
		strcpy((char*) decode_ASCII_to_[(uchar) AA_right_accent], "\xC1");
		strcpy((char*) decode_ASCII_to_[(uchar) a_right_accent], "\xE1");
		strcpy((char*) decode_ASCII_to_[(uchar) AA_roof_accent], "\xC2");
		strcpy((char*) decode_ASCII_to_[(uchar) a_roof_accent], "\xE2");
		strcpy((char*) decode_ASCII_to_[(uchar) AA_tild_accent], "\xC3");
		strcpy((char*) decode_ASCII_to_[(uchar) a_tild_accent], "\xE3");
		strcpy((char*) decode_ASCII_to_[(uchar) AA_2dot_accent], "\xC4");
		strcpy((char*) decode_ASCII_to_[(uchar) a_2dot_accent], "\xE4");
		strcpy((char*) decode_ASCII_to_[(uchar) AA_circle_accent], "\xC5");
		strcpy((char*) decode_ASCII_to_[(uchar) a_circle_accent], "\xE5");
		strcpy((char*) decode_ASCII_to_[(uchar) EE_left_accent], "\xC8");
		strcpy((char*) decode_ASCII_to_[(uchar) e_left_accent], "\xE8");
		strcpy((char*) decode_ASCII_to_[(uchar) EE_right_accent], "\xC9");
		strcpy((char*) decode_ASCII_to_[(uchar) e_right_accent], "\xE9");
		strcpy((char*) decode_ASCII_to_[(uchar) EE_roof_accent], "\xCA");
		strcpy((char*) decode_ASCII_to_[(uchar) e_roof_accent], "\xEA");
		strcpy((char*) decode_ASCII_to_[(uchar) EE_2dot_accent], "\xCB");
		strcpy((char*) decode_ASCII_to_[(uchar) e_2dot_accent], "\xEB");
		strcpy((char*) decode_ASCII_to_[(uchar) II_left_accent], "\xCC");
		strcpy((char*) decode_ASCII_to_[(uchar) i_left_accent], "\xEC");
		strcpy((char*) decode_ASCII_to_[(uchar) II_right_accent], "\xCD");
		strcpy((char*) decode_ASCII_to_[(uchar) i_right_accent], "\xED");
		strcpy((char*) decode_ASCII_to_[(uchar) II_roof_accent], "\xCE");
		strcpy((char*) decode_ASCII_to_[(uchar) i_roof_accent], "\xEE");
		strcpy((char*) decode_ASCII_to_[(uchar) II_2dot_accent], "\xCF");
		strcpy((char*) decode_ASCII_to_[(uchar) i_2dot_accent], "\xEF");
		strcpy((char*) decode_ASCII_to_[(uchar) NN_tild_accent], "\xD1");
		strcpy((char*) decode_ASCII_to_[(uchar) n_tild_accent], "\xF1");
		strcpy((char*) decode_ASCII_to_[(uchar) OO_left_accent], "\xD2");
		strcpy((char*) decode_ASCII_to_[(uchar) o_left_accent], "\xF2");
		strcpy((char*) decode_ASCII_to_[(uchar) OO_right_accent], "\xD3");
		strcpy((char*) decode_ASCII_to_[(uchar) o_right_accent], "\xF3");
		strcpy((char*) decode_ASCII_to_[(uchar) OO_roof_accent], "\xD4");
		strcpy((char*) decode_ASCII_to_[(uchar) o_roof_accent], "\xF4");
		strcpy((char*) decode_ASCII_to_[(uchar) OO_tild_accent], "\xD5");
		strcpy((char*) decode_ASCII_to_[(uchar) o_tild_accent], "\xF5");
		strcpy((char*) decode_ASCII_to_[(uchar) OO_2dot_accent], "\xD6");
		strcpy((char*) decode_ASCII_to_[(uchar) o_2dot_accent], "\xF6");
		strcpy((char*) decode_ASCII_to_[(uchar) UU_left_accent], "\xD9");
		strcpy((char*) decode_ASCII_to_[(uchar) u_left_accent], "\xF9");
		strcpy((char*) decode_ASCII_to_[(uchar) UU_right_accent], "\xDA");
		strcpy((char*) decode_ASCII_to_[(uchar) u_right_accent], "\xFA");
		strcpy((char*) decode_ASCII_to_[(uchar) UU_roof_accent], "\xDB");
		strcpy((char*) decode_ASCII_to_[(uchar) u_roof_accent], "\xFB");
		strcpy((char*) decode_ASCII_to_[(uchar) UU_2dot_accent], "\xDC");
		strcpy((char*) decode_ASCII_to_[(uchar) u_2dot_accent], "\xFC");
		strcpy((char*) decode_ASCII_to_[(uchar) AE_cap_deaf_sound], "AE");
		strcpy((char*) decode_ASCII_to_[(uchar) ae_deaf_sound], "ae");
		strcpy((char*) decode_ASCII_to_[(uchar) OE_cap_deaf_sound], "OE");
		strcpy((char*) decode_ASCII_to_[(uchar) oe_deaf_sound], "oe");
		strcpy((char*) decode_ASCII_to_[(uchar) ss_deaf_sound], "\xDF");
		strcpy((char*) decode_ASCII_to_[(uchar) CC_bottom_accent], "\xC7");
		strcpy((char*) decode_ASCII_to_[(uchar) c_bottom_accent], "\xE7");
		strcpy((char*) decode_ASCII_to_[(uchar) invers_exm], "\xA1");
		strcpy((char*) decode_ASCII_to_[(uchar) invers_qm], "\xA0");

		if (language == LANGUAGE_DANISH) {
			strcpy((char*) decode_ASCII_to_[(uchar) OO_crossed], "\xD8");
			strcpy((char*) decode_ASCII_to_[(uchar) o_crossed], "\xF8");
		}

	}

	if (CodePages[language] == CHARSET_RUSSIAN) {
		//strcpy((char*) decode_ASCII_to_[(uchar) r_cu_d], "д");
		strcpy((char*) decode_ASCII_to_[(uchar) r_cu_d], "\xE4");
		//strcpy((char*) decode_ASCII_to_[(uchar) r_cu_g], "д");
		strcpy((char*) decode_ASCII_to_[(uchar) r_cu_g], "\xE4");
		//strcpy((char*) decode_ASCII_to_[(uchar) r_cu_m], "т");
		strcpy((char*) decode_ASCII_to_[(uchar) r_cu_m], "\xF2");
		//strcpy((char*) decode_ASCII_to_[(uchar) r_cu_u], "и");
		strcpy((char*) decode_ASCII_to_[(uchar) r_cu_u], "\xE8");
		//strcpy((char*) decode_ASCII_to_[(uchar) r_cu_z], "г");
		strcpy((char*) decode_ASCII_to_[(uchar) r_cu_z], "\xE3");
		//strcpy((char*) decode_ASCII_to_[(uchar) r_cu_a], "а");
		strcpy((char*) decode_ASCII_to_[(uchar) r_cu_a], "\xE0");
		//strcpy((char*) decode_ASCII_to_[(uchar) r_EE_2dot], "Ё");
		strcpy((char*) decode_ASCII_to_[(uchar) r_EE_2dot], "\xA8");
		//strcpy((char*) decode_ASCII_to_[(uchar) r_e_2dot], "ё");
		strcpy((char*) decode_ASCII_to_[(uchar) r_e_2dot], "\xB8");
		strcpy((char*) decode_ASCII_to_[(uchar) 0xC3], "_#_");
		//strcpy((char*) decode_ASCII_to_[(uchar) low_quotes_rus], "„");
		strcpy((char*) decode_ASCII_to_[(uchar) low_quotes_rus], "\x84");
	}

	if (CodePages[language] == CHARSET_EASTEUROPE) {
		strcpy((char*) decode_ASCII_to_[(uchar) liga_fi], "fi");
		strcpy((char*) decode_ASCII_to_[(uchar) liga_fl], "fl");
		strcpy((char*) decode_ASCII_to_[(uchar) liga_ff], "ff");
		strcpy((char*) decode_ASCII_to_[(uchar) liga_ffi], "ffi");
		strcpy((char*) decode_ASCII_to_[(uchar) liga_rt], "rt");
		strcpy((char*) decode_ASCII_to_[(uchar) liga_ri], "ri");
		strcpy((char*) decode_ASCII_to_[(uchar) liga_uperc], "%%u");
		strcpy((char*) decode_ASCII_to_[(uchar) liga_lperc], "%%d");
		strcpy((char*) decode_ASCII_to_[(uchar) liga_ffl], "ffl");
		strcpy((char*) decode_ASCII_to_[(uchar) liga_i], "_i_");
		strcpy((char*) decode_ASCII_to_[(uchar) liga_j], "_j_");
		strcpy((char*) decode_ASCII_to_[(uchar) liga_exm], "_!_");
		strcpy((char*) decode_ASCII_to_[(uchar) liga_qm], "_?_");
		strcpy((char*) decode_ASCII_to_[(uchar) liga_inv_exm], "_!!_");
		strcpy((char*) decode_ASCII_to_[(uchar) liga_inv_qm], "_??_");
		strcpy((char*) decode_ASCII_to_[(uchar) liga_bull], "\xB0");
		//strcpy((char*) decode_ASCII_to_[(uchar) left_quocket], "«");
		strcpy((char*) decode_ASCII_to_[(uchar) left_quocket], "\xAB");
		//strcpy((char*) decode_ASCII_to_[(uchar) right_quocket], "»");
		strcpy((char*) decode_ASCII_to_[(uchar) right_quocket], "\xBB");
		//strcpy((char*) decode_ASCII_to_[(uchar) low_quotes], "„");
		strcpy((char*) decode_ASCII_to_[(uchar) low_quotes], "\x84");
		strcpy((char*) decode_ASCII_to_[(uchar) liga_rt], "rt");

		// Список букв cp1250
		list_latin_letters();

	}

	if (CodePages[language] == CHARSET_BALTIC) {

		strcpy((char*) decode_ASCII_to_[(uchar) liga_TM], "\x99");
		strcpy((char*) decode_ASCII_to_[(uchar) liga_CC], "\xa9");
		strcpy((char*) decode_ASCII_to_[(uchar) liga_CR], "\xae");

		strcpy((char*) decode_ASCII_to_[(uchar) liga_fi], "fi");
		strcpy((char*) decode_ASCII_to_[(uchar) liga_fl], "fl");
		strcpy((char*) decode_ASCII_to_[(uchar) liga_ff], "ff");
		strcpy((char*) decode_ASCII_to_[(uchar) liga_ffi], "ffi");
		strcpy((char*) decode_ASCII_to_[(uchar) liga_rt], "rt");
		strcpy((char*) decode_ASCII_to_[(uchar) liga_ri], "ri");
		strcpy((char*) decode_ASCII_to_[(uchar) liga_uperc], "%%u");
		strcpy((char*) decode_ASCII_to_[(uchar) liga_lperc], "%%d");
		strcpy((char*) decode_ASCII_to_[(uchar) liga_ffl], "ffl");
		strcpy((char*) decode_ASCII_to_[(uchar) liga_i], "_i_");
		strcpy((char*) decode_ASCII_to_[(uchar) liga_j], "_j_");
		strcpy((char*) decode_ASCII_to_[(uchar) liga_exm], "_!_");
		strcpy((char*) decode_ASCII_to_[(uchar) liga_qm], "_?_");
		strcpy((char*) decode_ASCII_to_[(uchar) liga_bull], "\x95");
		//strcpy((char*) decode_ASCII_to_[(uchar) left_quocket], "«");
		strcpy((char*) decode_ASCII_to_[(uchar) left_quocket], "\xAB");
		//strcpy((char*) decode_ASCII_to_[(uchar) right_quocket], "»");
		strcpy((char*) decode_ASCII_to_[(uchar) right_quocket], "\xBB");
		//strcpy((char*) decode_ASCII_to_[(uchar) low_quotes], "„");
		strcpy((char*) decode_ASCII_to_[(uchar) low_quotes], "\x84");
		strcpy((char*) decode_ASCII_to_[(uchar) liga_rt], "rt");

		// Список букв cp1257
		list_baltic_letters();

	}

	if (CodePages[language] == CHARSET_TURKISH) {

		strcpy((char*) decode_ASCII_to_[(uchar) liga_TM], "\x99");
		strcpy((char*) decode_ASCII_to_[(uchar) liga_CC], "\xa9");
		strcpy((char*) decode_ASCII_to_[(uchar) liga_CR], "\xae");
		strcpy((char*) decode_ASCII_to_[(uchar) liga_bull], "\x95");

		strcpy((char*) decode_ASCII_to_[(uchar) liga_fi], "fi");
		strcpy((char*) decode_ASCII_to_[(uchar) liga_fl], "fl");
		strcpy((char*) decode_ASCII_to_[(uchar) liga_ff], "ff");
		strcpy((char*) decode_ASCII_to_[(uchar) liga_ffi], "ffi");
		strcpy((char*) decode_ASCII_to_[(uchar) liga_rt], "rt");
		strcpy((char*) decode_ASCII_to_[(uchar) liga_ri], "ri");
		strcpy((char*) decode_ASCII_to_[(uchar) liga_uperc], "%%u");
		strcpy((char*) decode_ASCII_to_[(uchar) liga_lperc], "%%d");
		strcpy((char*) decode_ASCII_to_[(uchar) liga_ffl], "ffl");
		strcpy((char*) decode_ASCII_to_[(uchar) liga_i], "_i_");
		strcpy((char*) decode_ASCII_to_[(uchar) liga_j], "_j_");
		strcpy((char*) decode_ASCII_to_[(uchar) liga_exm], "_!_");
		strcpy((char*) decode_ASCII_to_[(uchar) liga_qm], "_?_");
		//strcpy((char*) decode_ASCII_to_[(uchar) left_quocket], "«");
		strcpy((char*) decode_ASCII_to_[(uchar) left_quocket], "\xAB");
		//strcpy((char*) decode_ASCII_to_[(uchar) right_quocket], "»");
		strcpy((char*) decode_ASCII_to_[(uchar) right_quocket], "\xBB");
		//strcpy((char*) decode_ASCII_to_[(uchar) low_quotes], "„");
		strcpy((char*) decode_ASCII_to_[(uchar) low_quotes], "\x84");
		strcpy((char*) decode_ASCII_to_[(uchar) liga_rt], "rt");

		// Список букв cp1254
		list_turkish_letters();

	}

	switch (language) {
	case LANGUAGE_ENGLISH:
	case LANGUAGE_GERMAN:
	case LANGUAGE_FRENCH:
	case LANGUAGE_DUTCH:
	case LANGUAGE_DANISH:
	case LANGUAGE_PORTUGUESE:
	case LANGUAGE_CROATIAN:
	case LANGUAGE_POLISH:
	case LANGUAGE_CZECH: // 01.09.2000 E.P.
	case LANGUAGE_ROMANIAN:
	case LANGUAGE_HUNGARIAN:
	case LANGUAGE_SPANISH:
	case LANGUAGE_ITALIAN:
	case LANGUAGE_SWEDISH:
	case LANGUAGE_SLOVENIAN:
	case LANGUAGE_LATVIAN:
	case LANGUAGE_LITHUANIAN:
	case LANGUAGE_ESTONIAN:
	case LANGUAGE_TURKISH:
		memcpy(fon_alphabet_language[0], alphabet, 256);
		fon_alphabet_language[0][0x21] = 1;
		break;
	case LANGUAGE_RUSSIAN:
	case LANGUAGE_UKRAINIAN:
	case LANGUAGE_SERBIAN:
	case LANGUAGE_UZBEK:
	case LANGUAGE_BULGARIAN: // 01.09.2000 E.P.
		memcpy(fon_alphabet_language[1], alphabet, 256);
		break;
	case LANGUAGE_DIGITS: // 20.10.2000 Nick
		memcpy(fon_alphabet_language[2], alphabet, 256);
		break;
	default:
		break;
	}
}

Bool is_russian_language(uchar lang) {
        return (lang == LANGUAGE_RUSSIAN ||
                lang == LANGUAGE_UKRAINIAN ||
                lang == LANGUAGE_SERBIAN ||
                lang == LANGUAGE_UZBEK ||
                lang == LANGUAGE_BULGARIAN ||
                lang == LANGUAGE_BELARUSIAN // 01.09.2000 E.P.
	);
}
//************************************************************
void list_latin_letters() // 05.09.2000 E.P.
{
	// Список букв cp1250

	uchar tab[] = {

	CROAT_D, CROAT_d, POLISH_LL, POLISH_l,

	AA_right_accent, a_right_accent, AA_2dot_accent, a_2dot_accent,
			AA_bottom_accent_latin, a_bottom_accent_latin, AA_semicircle,
			a_semicircle, AA_roof_accent, a_roof_accent, CC_right_accent,
			c_right_accent, CC_inv_roof, c_inv_roof, DD_inv_roof, d_inv_roof,
			EE_right_accent, e_right_accent, EE_bottom_accent_latin,
			e_bottom_accent_latin, EE_inv_roof, e_inv_roof, II_right_accent,
			i_right_accent, II_roof_accent, i_roof_accent, NN_right_accent,
			n_right_accent, NN_inv_roof, n_inv_roof, OO_right_accent,
			o_right_accent, OO_roof_accent, o_roof_accent, OO_2dot_accent,
			o_2dot_accent, OO_double_right, o_double_right, RR_inv_roof,
			r_inv_roof, SS_right_accent, s_right_accent, SS_inv_roof_latin,
			s_inv_roof_latin, SS_bottom_accent_latin, s_bottom_accent_latin,
			TT_inv_roof, t_inv_roof, TT_bottom_accent, t_bottom_accent,
			UU_right_accent, u_right_accent, UU_2dot_accent, u_2dot_accent,
			UU_double_right, u_double_right, UU_circle_accent, u_circle_accent,
			YY_right_accent, y_right_accent, ZZ_right_accent, z_right_accent,
			ZZ_dot_accent, z_dot_accent, ZZ_inv_roof_latin, z_inv_roof_latin,

			0 };

	uchar *p;
	extern uchar decode_ASCII_to_[256][4];

	// Здесь нужно оставить код без изменения,
	// а вот в ROUT/CodeTables.cpp функция fromcp1250_to_cp852()
	// перекодирует из cp1250 в cp852. См. ROUT/CodeTables.cpp.
	for (p = tab; *p; p++) {
		uchar c = *p;
		decode_ASCII_to_[c][0] = c;
		decode_ASCII_to_[c][1] = 0;
	}

}
//************************************************************
void list_baltic_letters() // 09.07.2001 E.P.
{
	// Список букв cp1257

	uchar tab[] = { AA_2dot_accent, a_2dot_accent, AA_macron, a_macron,
			AA_bottom_accent_baltic, a_bottom_accent_baltic, CC_inv_roof,
			c_inv_roof, EE_macron, e_macron, EE_bottom_accent_baltic,
			e_bottom_accent_baltic, EE_dot_accent, e_dot_accent,
			GG_bottom_accent, LATVIAN_g, II_macron, i_macron, II_bottom_accent,
			i_bottom_accent, KK_bottom_accent, k_bottom_accent,
			LL_bottom_accent, l_bottom_accent, NN_bottom_accent,
			n_bottom_accent, OO_tild_accent, o_tild_accent, OO_2dot_accent,
			o_2dot_accent, SS_inv_roof_baltic, s_inv_roof_baltic,
			UU_2dot_accent, u_2dot_accent, UU_bottom_accent, u_bottom_accent,
			UU_macron, u_macron, ZZ_inv_roof_baltic, z_inv_roof_baltic, 0 };

	uchar *p;
	extern uchar decode_ASCII_to_[256][4];

	for (p = tab; *p; p++) {
		uchar c = *p;
		decode_ASCII_to_[c][0] = c;
		decode_ASCII_to_[c][1] = 0;
	}

}
//************************************************************
void list_turkish_letters() // 20.05.2002 E.P.
{
	// Список букв cp1254

	uchar tab[] = { OO_2dot_accent, o_2dot_accent, UU_2dot_accent,
			u_2dot_accent, AA_roof_accent, a_roof_accent, CC_bottom_accent,
			c_bottom_accent, II_roof_accent, i_roof_accent, GG_semicircle,
			g_semicircle, II_dot_accent, i_sans_accent,
			SS_bottom_accent_turkish, s_bottom_accent_turkish,

			0 };

	uchar *p;
	extern uchar decode_ASCII_to_[256][4];

	for (p = tab; *p; p++) {
		uchar c = *p;
		decode_ASCII_to_[c][0] = c;
		decode_ASCII_to_[c][1] = 0;
	}

}
//************************************************************
