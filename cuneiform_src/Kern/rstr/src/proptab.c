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

#include <string.h>

#include "cttypes.h"
#include "lang.h"
#include "func.h"
#include "tuner.h"
#include "ligas.h"
/*
;*****  h>w ==> p= (64*w)/h         ******
;*****  h=w ==> p=64                ******
;*****  h<w ==> p=128 - (64*h)/w    ******
*/

static uchar letters_pidx_table_lat[512] = {
10,100,		//  0
10,100,		//  1
10,100,		//  2
10,100,		//  3
10,100,		//  4
10,100,		//  5
10,100,		//  6
10,100,		//  7
10,100,		//  8
10,100,		//  9
10,100,		//  a
10,100,		//  b
10,100,		//  c
10,100,		//  d
10,100,		//  e
10,100,		//  f

10,100,		// 10
10,100,		// 11
10,100,		// 12
10,100,		// 13
10,100,		// 14
10,100,		// 15
10,100,		// 16
10,100,		// 17
10,100,		// 18
10,100,		// 19
10,100,		// 1a
10,100,		// 1b
10,100,		// 1c
10,100,		// 1d
10,100,		// 1e
10,100,		// 1f

10,100,		// 20
2 ,32 ,		// 21		!
10,100,		// 22		"
36,72 ,		// 23		#
24,48 ,     // 24		$
36,80 ,		// 25		%
36,84 ,		// 26		&
20,56 ,		// 27		'
7 ,32 ,		// 28		(
7 ,32 ,		// 29		)
52,72 ,		// 2a		*
52,76 ,		// 2b		+
20,56 ,		// 2c		,
80,120,		// 2d		-
32,84 ,		// 2e		.
16,70 ,     // 2f   /   16,50
30,69 ,		// 30		0
9 ,53 ,		// 31		1
29,64 ,		// 32		2
22,59 ,		// 33		3
30,69 ,		// 34		4
27,60 ,		// 35		5
30,67 ,		// 36		6
26,58 ,		// 37		7
30,64 ,		// 38		8
29,63 ,		// 39		9
10,100,		// 3a
10,100,		// 3b
19,76 ,		// 3c		<
10,100,		// 3d		=
19,76 ,		// 3e		>
27,74 ,		// 3f		?

44,72 ,		// 40		@
32,80 ,		// 41		A
35,71 ,		// 42		B
32,72 ,		// 43		C
36,78 ,		// 44		D
28,72 ,		// 45		E
26,69 ,		// 46		F
36,72 ,		// 47		G
32,80 ,		// 48		H
4 ,52 ,		// 49		I
12,60 ,		// 4a		J
32,80 ,		// 4b		K
20,72 ,		// 4c		L
41,92 ,		// 4d		M
32,80 ,		// 4e		N
32,76 ,		// 4f		O

27,73 ,		// 50		P
37,72 ,		// 51		Q
32,76 ,		// 52		R
30,69 ,		// 53		S
28,76 ,		// 54		T
34,76 ,		// 55		U
36,76 ,		// 56		V
38,88 ,		// 57		W
37,76 ,		// 58		X
32,72 ,		// 59		Y
36,72 ,		// 5a		Z
8 ,26 ,		// 5b		[
16,50 ,		// 5c		backslash
8 ,26 ,		// 5d		]
10,100,		// 5e		^
112,124,	// 5f		_

20,56 ,		// 60		`
36,80 ,		// 61		a
28,62 ,		// 62		b
32,76 ,		// 63		c
30,64 ,		// 64		d
36,76 ,		// 65		e
13,50 ,		// 66		f
28,66 ,		// 67		g
24,64 ,		// 68		h
4 ,68 ,		// 69		i
8 ,47,		// 6a		j
24,64 ,		// 6b		k
2 ,48 ,		// 6c		l
48,96 ,		// 6d		m
40,88 ,		// 6e		n
36,80 ,		// 6f		o

28,64 ,		// 70		p
28,56 ,		// 71		q
22,72 ,		// 72		r
30,68 ,		// 73		s
8 ,55 ,		// 74		t
36,84 ,		// 75		u
36,80 ,		// 76		v
46,96 ,		// 77		w
36,80 ,		// 78		x
28,64 ,		// 79		y
36,73 ,		// 7a		z
8 ,24 ,		// 7b		{
2 ,20 ,		// 7c		|
8 ,24 ,		// 7d		}
10,100,		// 7e
10,100,		// 7f

10,100,		// 80
10,100,		// 81
10,100,		// 82
10,100,		// 83
10,100,		// 84
10,100,		// 85
10,100,		// 86
10,100,		// 87
10,100,		// 88
10,100,		// 89
10,100,		// 8a
10,100,		// 8b
10,100,		// 8c
10,100,		// 8d
10,100,		// 8e
10,100,		// 8f

10,100,		// 90
10,100,		// 91
10,100,		// 92
10,100,		// 93
10,100,		// 94
10,100,		// 95
10,100,		// 96
10,100,		// 97
10,100,		// 98
10,100,		// 99
10,100,		// 9a
10,100,		// 9b
10,100,		// 9c
10,100,		// 9d
10,100,		// 9e
10,100,		// 9f

27,74, 		// a0           invers qm
 2,32, 		// a1           invers exm
10,100,		// a2
10,100,		// a3
10,100,		// a4
10,100,		// a5
10,100,		// a6
10,100,		// a7
10,100,		// a8
10,100,		// a9
38,88, 		// aa		OE_cap_deaf_sound
10,100,		// ab
46,96, 		// ac		oe_deaf_sound
10,100,		// ad
10,100,		// ae
10,100,		// af

10,100,		// b0		Bad character
24,59 ,		// b1		liga_fi
32,68 ,		// b2		liga_fl
32,68 ,		// b3		liga_ff
51,80 ,		// b4		liga_ffi
44,80 ,		// b5		liga_rt
40,80 ,		// b6		liga_ri
36,80 ,		// b7		liga_uperc
36,80 ,		// b8		liga_lperc
38,80 ,		// b9		liga_ffl
2 ,48 ,		// ba		liga_i
8 ,40 ,		// bb		liga_j
2 ,26 ,		// bc		liga_exm
25,72 ,		// bd		liga_qm
2 ,26 ,		// be       liga_inv_exm
25,72,		// bf       liga_inv_qm

10,100,		// c0
10,100,		// c1
10,100,		// c2
10,100,		// c3
10,100,		// c4
10,100,		// c5
38,88, 		// c6        AE_cap_deaf_sound
37,72, 		// c7        CC_bot accent
10,100,		// c8
10,100,		// c9
10,100,		// ca
10,100,		// cb
10,100,		// cc
10,100,		// cd
10,100,		// ce
10,100,		// cf

10,100,		// d0
10,100,		// d1
10,100,		// d2
10,100,		// d3
10,100,		// d4
10,100,		// d5
10,100,		// d6
10,100,		// d7
10,100,		// d8
10,100,		// d9
10,100,		// da
10,100,		// db
10,100,		// dc
10,100,		// dd
10,100,		// de
32,66, 		// df           ss (germ szet)

10,100,		// e0
10,100,		// e1
10,100,		// e2
10,100,		// e3
10,100,		// e4
10,100,		// e5
46,96, 		// e6          ae_deaf_sound
28,56,  	// e7          c_bot_accent
10,100,		// e8
10,100,		// e9
10,100,		// ea
10,100,		// eb
10,100,		// ec
10,100,		// ed
10,100,		// ee
10,100,		// ef

10,100,		// f0
10,100,		// f1
10,100,		// f2
10,100,		// f3
10,100,		// f4
10,100,		// f5
10,100,		// f6
10,100,		// f7
10,100,		// f8
10,100,		// f9
10,100,		// fa
10,100,		// fb
10,100,		// fc
10,100,		// fd
10,100,		// fe
10,100		// ff
};

static uchar letters_pidx_table_rus[512]={
  1,127,  1,127,  1,127,  1,127,  1,127,  1,127,  1,127,  1,127,  1,127,  1,127,  1,127,  1,127,  1,127,  1,127,  1,127,  1,127, // 0x00
  1,127,  1,127,  1,127,  1,127,  1,127,  1,127,  1,127,  1,127,  1,127,  1,127,  1,127,  1,127,  1,127,  1,127,  1,127,  1,127, // 0x01
  1,127,
        2, 34,// ! some italic
        1,127,// "
       36, 72,// #
       24, 48,// $
       36, 80,// %
       36, 84,// &
        1,127,// '
        7, 32,// (
        7, 32,// }
       52, 72,// *
       52, 76,// +
       20, 56,// ,
       80,120,// -
       32, 84,// .
       16,70 ,   // 2f
       30, 62,// 0
        9, 52,// 1
       30, 61,// 2 pape0 cursiv
       22, 56,// 3
       30, 58,// 4
       27, 60,// 5
       30, 56,// 6
       30, 56,// 7
       30, 56,// 8
       30, 63,// 9
        1,127,// :
        1,127,// ;
       19, 76,// <
        1,127,// =
       19, 76,// >
       27, 72,// ?       0x03
       44, 68,// @
       32, 80,// A
       36, 68,// B
       32, 72,// C
       36, 78,// D
       28, 72,// E
       26, 69,// F
       36, 72,// G
       32, 80,// H
        4, 52,// I
       12, 60,// J
       32, 80,// K
       20, 72,// L
       41, 88,// M
       32, 80,// N
       32, 74,// O      0x04
       32, 67,// P
       37, 72,// Q
       32, 76,// R
       30, 64,// S
       28, 76,// T
       34, 76,// U
       36, 76,// V
       38, 88,// W
       37, 76,// X
       32, 72,// Y
       36, 72,// Z
        8, 26,// [
        1,127,// back clash
        8, 26,// ]
        1,127,// ^
      112,124,// _      0x05
        1,127,// `
       36, 80,// a
       28, 62,// b
       32, 76,// c
       30, 64,// d
       36, 76,// e
       16, 50,// f
       28, 61,// g
       24, 64,// h
        4, 68,// i
        8, 42,// j
       24, 64,// k
        2, 48,// l
       48, 96,// m
       40, 88,// n
       36, 80,// o      0x06
       28, 64,// p
       28, 56,// q
       22, 72,// r
       30, 68,// s
        8, 55,// t
       36, 84,// u
       36, 80,// v
       46, 96,// w
       36, 80,// x
       28, 64,// y
       36, 72,// z
        8, 24,// {
        4, 52,// | stick from bI
        8, 24,// }
        1,127,// ~
        1,127,//        0x07f
       37, 75,// Ђ
       34, 74,// Ѓ
       33, 71,// ‚ jurnal 7 kegl
       28, 74,// ѓ futuris 10 kegl
       33, 69,// „
       35, 71,// …
       40, 90,// †
       33, 70,// ‡
       34, 82,// €
       35, 68,// ‰
       35, 75,// Љ
       37, 80,// ‹
       33, 87,// Њ
       33, 82,// Ќ 81 kudryash 14 kegl cursiv
       35, 78,// Ћ
       33, 80,// Џ
       33, 72,// ђ bodoni halfbold 12 kegl
       35, 70,// ‘
       37, 70,// ’
       35, 69,// “ lower bound corr from UFA stend
       32, 78,// ” UFA : Ўл«® 39,78
       37, 81,// •
       32, 67,// –
       32, 69,// —
       37, 108,//� changed
       32, 89,// ™
       38, 76,// љ  changed
       37, 86,// ›
       35, 69,// њ
       34, 69,// ќ
       37, 88,// ћ
       35, 79,// џ
       42, 79,//  
       32, 60,// Ў
       37, 80,// ў
       33, 84,// Ј
       42, 79,// ¤
       38, 82,// Ґ  7 kegl
       39, 98,// ¦
       37, 77,// § changed
       33, 85,// Ё
       35, 70,// ©
       41, 82,// Є changed 45,82 26-12-1994 for RUS_ENG
       45, 87,// «
       55, 92,// ¬
       33, 85,// ­
       41, 77,// ®
       33, 85,// Ї
        1,127,// °
       24, 58,// ±
       32, 68,// І
       32, 68,// і
       51, 80,// ґ
       44, 80,// µ
       40, 80,// ¶
       36, 80,// ·
       36, 80,// ё
       38, 80,// №
        2, 48,// є
        8, 40,// »
        2, 26,// ј
       27, 72,// Ѕ
        1,127,// ѕ
        1,127,// ї
        1,127,// А
        1,127,// Б
       51,100,// В serbian - n
        1,127,// Г
       51,110,// Д serbian - l
        1,127,// Е
       38, 88,// Ж
       37, 72,// З
        1,127,// И
        1,127,// Й
        1,127,// К
       51,100,// Л serbian - N
        1,127,// М
       51,110,// Н serbian - L
        1,127,// О
        1,127,// П
        1,127,// Р
        1,127,// С
        1,127,// Т
        1,127,// У
        1,127,// Ф
        1,127,// Х
        1,127,// Ц
        1,127,// Ч
        1,127,// Ш
        1,127,// Щ
        1,127,// Ъ
        1,127,// Ы
        1,127,// Ь
        1,127,// Э
        1,127,// Ю
       32, 66,// Я
       30, 72,// а
       33, 76,// б , 37
       34, 92,// в
       28, 79,// г  lazurski small kegl
       32, 68,// д  changed cune17/18 & UKRAINIAN PAPERS
       42, 86,// е
       36, 69,// ж
       40, 82,// з
       48, 97,// и
       45, 88,// й    jurnal 7 kegl
       52, 82,// к
       51, 94,// л
       33, 80,// м lower bound up     & ERECT
       42, 80,// н
       34, 93,// о lower bound up
       40, 84,// п
       32, 54,// р cursiv d
       36, 57,// с cursiv d tail down & ERECT
        1,127,// т
        1,127,// у
        1,127,// ф
       77, 94,// cursiv m
        1,127,// ц
       50, 82,// ч cursiv u
       28, 56,// ш cursiv ee
        1,127,// щ
        1,127,// ъ
        1,127,// ы
        1,127,// ь
       41, 76,// э cursiv a
        1,127,// ю
        1,127 //
              };

static uchar work_table[512]={0};        /* 23.09.1997 E.Pliskin */
uchar *letters_pidx_table=work_table;

#define SET_LET(a,b,c) \
{work_table[2*(uchar)(a)]=(b);\
work_table[2*(uchar)(a)+1]=(c);}

// 05.09.2000 E.P.
#define COPY_LET(a,b) \
{work_table[2*(uchar)(a)]=work_table[2*(uchar)(b)];\
work_table[2*(uchar)(a)+1]=work_table[2*(uchar)(b)+1];}

void correct_letters_pidx_table(void)
{
  switch(language)
     {
     case LANG_RUSSIAN:
        memcpy(work_table, letters_pidx_table_rus, sizeof (work_table));
        break;

     default:
        memcpy(work_table, letters_pidx_table_lat, sizeof (work_table));
        break;
     }

// Центральноевропейские языки.
if(is_cen_language(language))
	{
	// Код лиги определен через макру в ligas.h 05.09.2000 E.P.
	COPY_LET(liga_exm,		liga_exm_usual);
	COPY_LET(right_quocket,	right_quocket_usual);
	COPY_LET(liga_CC,		liga_CC_usual);
	COPY_LET(liga_CR,		liga_CR_usual);

    SET_LET(AA_right_accent,37,80);		/* Min as C_bottom_accent, Max as 'A' */
    SET_LET(a_right_accent,28,80);		/* Min as c_bottom_accent, Max as 'a' */

    SET_LET(AA_bottom_accent_latin,10,100);
    SET_LET(a_bottom_accent_latin,10,100);

    SET_LET(AA_semicircle,10,100);
    SET_LET(a_semicircle,10,100);

    SET_LET(AA_roof_accent,10,100);
    SET_LET(a_roof_accent,10,100);

    SET_LET(CC_right_accent,37,72);
    SET_LET(c_right_accent,25,56);

    SET_LET(CC_inv_roof,37,72);
    SET_LET(c_inv_roof,25,56);

    SET_LET(CROAT_D,36,78);		// Перечеркнутое D
    SET_LET(CROAT_d,30,64);

	SET_LET(DD_inv_roof,10,100);
	SET_LET(d_inv_roof,10,100);

    SET_LET(EE_right_accent,10,100);
    SET_LET(e_right_accent,10,100);

    SET_LET(EE_inv_roof,10,100);
    SET_LET(e_inv_roof,10,100);

    SET_LET(EE_bottom_accent_latin,27,72);	/* Max as 'E' */
    SET_LET(e_bottom_accent_latin,28,76);	/* Min as c_bottom_accent, Max as 'e' */

    SET_LET(POLISH_LL,20,80);			/* Min as 'L', Max as 'K' */
    SET_LET(POLISH_l,2,64);				/* Min as 'l', Max as 'k' */

    SET_LET(NN_inv_roof,10,100);
    SET_LET(n_inv_roof,10,100);

    SET_LET(OO_double_right,10,100);
    SET_LET(o_double_right,10,100);

    SET_LET(RR_inv_roof,10,100);
    SET_LET(r_inv_roof,10,100);

	SET_LET(SS_inv_roof_latin,37,72);
    SET_LET(s_inv_roof_latin,25,56);

    SET_LET(SS_bottom_accent_latin,10,100);
    SET_LET(s_bottom_accent_latin,10,100);

    SET_LET(TT_inv_roof,10,100);
    SET_LET(t_inv_roof,10,100);

    SET_LET(TT_bottom_accent,10,100);
    SET_LET(t_bottom_accent,10,100);

    SET_LET(UU_double_right,10,100);
    SET_LET(u_double_right,10,100);

    SET_LET(UU_circle_accent,10,100);
    SET_LET(u_circle_accent,10,100);

    SET_LET(ZZ_inv_roof_latin,37,72);
    SET_LET(z_inv_roof_latin,25,56);

    SET_LET(ZZ_dot_accent,32,72);		/* Max as 'Z' */
    SET_LET(z_dot_accent, 32,73);		/* Max as 'z' */

    }

// Балтийские языки. 10.07.2001 E.P.
if(is_baltic_language(language))
	{
	// Лиги определены через макросы в ligas.h
	COPY_LET(low_quotes		,low_quotes_usual);
	COPY_LET(liga_exm		,liga_exm_usual);
	COPY_LET(liga_i			,liga_i_usual);
	COPY_LET(right_quocket	,right_quocket_usual);
	COPY_LET(liga_TM		,liga_TM_usual);
	COPY_LET(liga_CC		,liga_CC_usual);
	COPY_LET(liga_CR		,liga_CR_usual);
	COPY_LET(liga_bull		,liga_bull_usual);

    SET_LET(AA_2dot_accent,10,100);
    SET_LET(a_2dot_accent, 10,100);

    SET_LET(AA_macron,10,100);
    SET_LET(a_macron, 10,100);

    SET_LET(AA_bottom_accent,10,100);
    SET_LET(a_bottom_accent, 10,100);

    SET_LET(CC_inv_roof,37,72);
    SET_LET(c_inv_roof, 25,56);

    SET_LET(EE_macron,24,72);			/* Max as 'E' */
    SET_LET(e_macron, 32,76);			/* Max as 'e' */

    SET_LET(EE_bottom_accent,27,72);	/* Max as 'E' */
    SET_LET(e_bottom_accent, 28,76);	/* Min as c_bottom_accent, Max as 'e' */

    SET_LET(EE_dot_accent,24,72);		/* Max as 'E' */
    SET_LET(e_dot_accent, 32,76);		/* Max as 'e' */

    SET_LET(GG_bottom_accent,28,72);	/* Max as 'G' */
    SET_LET(LATVIAN_g,24,66);			/* Max as 'g' */

    SET_LET(II_macron,20,76);			/* */
    SET_LET(i_macron, 20,76);			/* */

    SET_LET(II_bottom_accent,12,52);	/* Max as 'I' */
    SET_LET(i_bottom_accent, 12,68);	/* Max as 'i' */

    SET_LET(KK_bottom_accent,28,80);	/* Max as 'K' */
    SET_LET(k_bottom_accent, 20,64);	/* Max as 'k' */

    SET_LET(LL_bottom_accent,20,72);	/* Max as 'L' */
    SET_LET(l_bottom_accent, 12,68);	/* */

    SET_LET(NN_bottom_accent,28,80);	/* Max as 'N' */
    SET_LET(n_bottom_accent, 36,88);	/* Max as 'n' */

	SET_LET(OO_tild_accent,10,100);
    SET_LET(o_tild_accent, 10,100);

    SET_LET(OO_2dot_accent,10,100);
    SET_LET(o_2dot_accent, 10,100);

	SET_LET(SS_inv_roof,37,72);
    SET_LET(s_inv_roof, 25,56);

    SET_LET(UU_2dot_accent,10,100);
    SET_LET(u_2dot_accent, 10,100);

    SET_LET(UU_bottom_accent,10,100);
    SET_LET(u_bottom_accent, 10,100);

    SET_LET(UU_macron,10,100);
    SET_LET(u_macron, 10,100);

    SET_LET(ZZ_inv_roof,37,72);
    SET_LET(z_inv_roof, 25,56);

    }

// Турецкий язык. 21.05.2002 E.P.
if(is_turkish_language(language))
	{
	// Лиги определены через макросы в ligas.h
	COPY_LET(liga_TM		,liga_TM_usual);
	COPY_LET(liga_CC		,liga_CC_usual);
	COPY_LET(liga_CR		,liga_CR_usual);
	COPY_LET(liga_bull		,liga_bull_usual);

    SET_LET(GG_semicircle,28,72);	// Max as 'G'
    SET_LET(g_semicircle, 24,66);	// Max as 'g'

    COPY_LET(II_dot_accent,'I');
    COPY_LET(i_sans_accent,'i');

    SET_LET(SS_bottom_accent_turkish,10,100);
    SET_LET(s_bottom_accent_turkish,10,100);
	}

}
