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



#include "roots.h"

#define LETTER_OK (ROOT_LETTER+ROOT_DUST+ROOT_RECOGNIZED)
#define RECF_DUST (ROOT_DUST+ROOT_RECOGNIZED)

uchar letters_sf_table[256] = {
RECF_DUST,		//  0
RECF_DUST,		//  1
RECF_DUST,		//  2
RECF_DUST,		//  3
RECF_DUST,		//  4
RECF_DUST,		//  5
RECF_DUST,		//  6
RECF_DUST,		//  7
RECF_DUST,		//  8
RECF_DUST,		//  9
RECF_DUST,		//  a
RECF_DUST,		//  b
RECF_DUST,		//  c
RECF_DUST,		//  d
RECF_DUST,		//  e
RECF_DUST,		//  f

RECF_DUST,		// 10
RECF_DUST,		// 11
RECF_DUST,		// 12
RECF_DUST,		// 13
RECF_DUST,		// 14
RECF_DUST,		// 15
RECF_DUST,		// 16
RECF_DUST,		// 17
RECF_DUST,		// 18
RECF_DUST,		// 19
RECF_DUST,		// 1a
RECF_DUST,		// 1b
RECF_DUST,		// 1c
RECF_DUST,		// 1d
RECF_DUST,		// 1e
RECF_DUST,		// 1f

RECF_DUST,		// 20
RECF_DUST,		// 21		!
RECF_DUST,		// 22		"
LETTER_OK,		// 23		#
LETTER_OK,		// 24		$
RECF_DUST,		// 25		%
LETTER_OK,		// 26		&
RECF_DUST,		// 27		'
RECF_DUST,		// 28		(
RECF_DUST,		// 29		)
RECF_DUST,		// 2a		*
RECF_DUST,		// 2b		+
RECF_DUST,		// 2c		,
RECF_DUST,		// 2d		-
RECF_DUST,		// 2e		.
RECF_DUST,		// 2f		/

LETTER_OK,		// 30		0
RECF_DUST,		// 31		1
LETTER_OK,		// 32		2
LETTER_OK,		// 33		3
LETTER_OK,		// 34		4
LETTER_OK,		// 35		5
LETTER_OK,		// 36		6
LETTER_OK,		// 37		7
LETTER_OK,		// 38		8
LETTER_OK,		// 39		9
RECF_DUST,		// 3a
RECF_DUST,		// 3b
RECF_DUST,		// 3c		<
RECF_DUST,		// 3d		=
RECF_DUST,		// 3e		>
RECF_DUST,		// 3f		?

LETTER_OK,		// 40		@
LETTER_OK,		// 41		A
LETTER_OK,		// 42		B
LETTER_OK,		// 43		C
LETTER_OK,		// 44		D
LETTER_OK,		// 45		E
LETTER_OK,		// 46		F
LETTER_OK,		// 47		G
LETTER_OK,		// 48		H
RECF_DUST,		// 49		I
LETTER_OK,		// 4a		J
LETTER_OK,		// 4b		K
LETTER_OK,		// 4c		L
LETTER_OK,		// 4d		M
LETTER_OK,		// 4e		N
LETTER_OK,		// 4f		O

LETTER_OK,		// 50		P
LETTER_OK,		// 51		Q
LETTER_OK,		// 52		R
LETTER_OK,		// 53		S
LETTER_OK,		// 54		T
LETTER_OK,		// 55		U
LETTER_OK,		// 56		V
LETTER_OK,		// 57		W
LETTER_OK,		// 58		X
LETTER_OK,		// 59		Y
LETTER_OK,		// 5a		Z
RECF_DUST,		// 5b		[
RECF_DUST,		// 5c		\ ...
RECF_DUST,		// 5d		]
RECF_DUST,		// 5e		^
RECF_DUST,		// 5f		_

RECF_DUST,		// 60		`
LETTER_OK,		// 61		a
LETTER_OK,		// 62		b
LETTER_OK,		// 63		c
LETTER_OK,		// 64		d
LETTER_OK,		// 65		e
LETTER_OK,		// 66		f
LETTER_OK,		// 67		g
LETTER_OK,		// 68		h
RECF_DUST,		// 69		i
LETTER_OK,		// 6a		j
LETTER_OK,		// 6b		k
RECF_DUST,		// 6c		l
LETTER_OK,		// 6d		m
LETTER_OK,		// 6e		n
LETTER_OK,		// 6f		o

LETTER_OK,		// 70		p
LETTER_OK,		// 71		q
LETTER_OK,		// 72		r
LETTER_OK,		// 73		s
LETTER_OK,		// 74		t
LETTER_OK,		// 75		u
LETTER_OK,		// 76		v
LETTER_OK,		// 77		w
LETTER_OK,		// 78		x
LETTER_OK,		// 79		y
LETTER_OK,		// 7a		z
RECF_DUST,		// 7b		{
RECF_DUST,		// 7c           |
RECF_DUST,		// 7d		}
RECF_DUST,		// 7e
RECF_DUST,		// 7f

LETTER_OK,    // 80 Ђ
LETTER_OK,    // 81 Ѓ
LETTER_OK,    // 82 ‚
LETTER_OK,    // 83 ѓ
LETTER_OK,    // 84 „
LETTER_OK,    // 85 …
LETTER_OK,    // 86 †
LETTER_OK,    // 87 ‡
LETTER_OK,    // 88 €
LETTER_OK,    // 89 ‰
LETTER_OK,    // 8a Љ
LETTER_OK,    // 8b ‹
LETTER_OK,    // 8c Њ
LETTER_OK,    // 8d Ќ
LETTER_OK,    // 8e Ћ
LETTER_OK,    // 8f Џ

LETTER_OK,    // 90 ђ
LETTER_OK,    // 91 ‘
LETTER_OK,    // 92 ’
LETTER_OK,    // 93 “
LETTER_OK,    // 94 ”
LETTER_OK,    // 95 •
LETTER_OK,    // 96 –
LETTER_OK,    // 97 —
LETTER_OK,    // 98 �
LETTER_OK,    // 99 ™
LETTER_OK,    // 9a љ
LETTER_OK,    // 9b ›
LETTER_OK,    // 9c њ
LETTER_OK,    // 9d ќ
LETTER_OK,    // 9e ћ
LETTER_OK,    // 9f џ

LETTER_OK,    // a0  
LETTER_OK,    // a1 Ў
LETTER_OK,    // a2 ў
LETTER_OK,    // a3 Ј
LETTER_OK,    // a4 ¤
LETTER_OK,    // a5 Ґ
LETTER_OK,    // a6 ¦
LETTER_OK,    // a7 §
LETTER_OK,    // a8 Ё
LETTER_OK,    // a9 ©
LETTER_OK,    // aa Є
LETTER_OK,    // ab «
LETTER_OK,    // ac ¬
LETTER_OK,    // ad ­
LETTER_OK,    // ae ®
LETTER_OK,    // af Ї

RECF_DUST,		// b0		Bad charackter
LETTER_OK,		// b1		liga_fi
LETTER_OK,		// b2		liga_fl
LETTER_OK,		// b3		liga_ff
LETTER_OK,		// b4		liga_ffi
LETTER_OK,		// b5		liga_rt
RECF_DUST,		// b6		liga_ri
LETTER_OK,		// b7		liga_uperc
LETTER_OK,		// b8		liga_lperc
LETTER_OK,		// b9		liga_ffl
RECF_DUST,		// ba		liga_i
RECF_DUST,		// bb		liga_j
RECF_DUST,		// bc		liga_exm
LETTER_OK,		// bd		liga_qm
RECF_DUST,		// be
RECF_DUST,		// bf

RECF_DUST,		// c0
RECF_DUST,		// c1
RECF_DUST,		// c2
RECF_DUST,		// c3
RECF_DUST,		// c4
RECF_DUST,		// c5
RECF_DUST,		// c6
RECF_DUST,		// c7
RECF_DUST,		// c8
RECF_DUST,		// c9
RECF_DUST,		// ca
RECF_DUST,		// cb
RECF_DUST,		// cc
RECF_DUST,		// cd
RECF_DUST,		// ce
RECF_DUST,		// cf

RECF_DUST,		// d0
RECF_DUST,		// d1
RECF_DUST,		// d2
RECF_DUST,		// d3
RECF_DUST,		// d4
RECF_DUST,		// d5
RECF_DUST,		// d6
RECF_DUST,		// d7
RECF_DUST,		// d8
RECF_DUST,		// d9
RECF_DUST,		// da
RECF_DUST,		// db
RECF_DUST,		// dc
RECF_DUST,		// dd
RECF_DUST,		// de
RECF_DUST,		// df

LETTER_OK,    // e0 а
LETTER_OK,    // e1 б
LETTER_OK,    // e2 в
LETTER_OK,    // e3 г
LETTER_OK,    // e4 д
LETTER_OK,    // e5 е
LETTER_OK,    // e6 ж
LETTER_OK,    // e7 з
LETTER_OK,    // e8 и
LETTER_OK,    // e9 й
LETTER_OK,    // ea к
LETTER_OK,    // eb л
LETTER_OK,    // ec м
LETTER_OK,    // ed н
LETTER_OK,    // ee о
LETTER_OK,    // ef п

LETTER_OK,    // f0 ЄгабЁў ¤ ўҐае
LETTER_OK,    // f1 Єга    ¤ ­Ё§
RECF_DUST,		// f2
RECF_DUST,		// f3
RECF_DUST,		// f4
LETTER_OK,    // f5 Єга в
RECF_DUST,		// f6
LETTER_OK,    // f7 Єга Ё
LETTER_OK,    // f8 Єга Ј
RECF_DUST,		// f9
RECF_DUST,		// fa
RECF_DUST,		// fb
RECF_DUST,		// fc
LETTER_OK,    // fd Єга  
RECF_DUST,		// fe
RECF_DUST		// ff
};
