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

#ifndef __BIT_FUNC_H
#	define __BIT_FUNC_H

#ifndef __LNSLANG_H
#  include "lnslang.h"
#endif

extern const uchar bit_byte[8];
extern const uchar lbit_byte[8];
extern const uint16_t bit_word[16];
extern const uint16_t lbit_word[16];
extern const uchar right_0[256];
extern const uchar left_0[256];
extern const uchar bit0_cnt[256];
extern const uchar bit1_cnt[256];
extern const uchar first_bit0_pos[256];
extern const uchar first_bit1_pos[256];
/**
#ifndef BIG_ENDIAN
#define hi_byte( some_word )     ( *( ((uchar*)&some_word) +1 )    )
#define low_byte( some_word )    (    (uchar)some_word             )
#else
#define hi_byte( some_word )     (    (uchar)some_word             )
#define low_byte( some_word )    ( *( ((uchar*)&some_word) +1 )    )
#endif
**/
#define hi_byte( some_word )     (    ((uint16_t)some_word)>>8       )
#define low_byte( some_word )    (    (uchar)some_word             )

#define left_byte( some_word )      ( low_byte( some_word )       )
#define right_byte( some_word )     ( hi_byte( some_word ) 			)

#define lr_word( le_byte, ri_byte )   ( (ri_byte << 8) + le_byte )
#define hl_word( hi_byte, lo_byte )   ( (hi_byte << 8) + lo_byte )

#define FIRST_BIT_IN_BYTE( bb )     (	first_bit1_pos[ bb ] )     // 0..7; 8 if none

#define set_bit_to_1( to_word, bit_num )	( (to_word) |= bit_word[ bit_num  ] )
#define set_bit_to_0( to_word, bit_num )	( (to_word) &= ~bit_word[ bit_num ] )
#define is_bit( in_word, bit_num )  		( (in_word) & bit_word[ bit_num   ] )

// for reversed data images (TIFF 'II')
#define set_bit_to_1_inv( to_word, bit_num )	( (to_word) |= lbit_word[ bit_num  ] )
#define set_bit_to_0_inv( to_word, bit_num )	( (to_word) &= ~lbit_word[ bit_num ] )
#define is_bit_inv( in_word, bit_num )  		( (in_word) & lbit_word[ bit_num   ] )
		// returns 0x0000 if bit "i" is 0 in "in_word"

#define bit0_count( in_word )  \
	( 	bit0_cnt[ hi_byte(in_word) ] + bit0_cnt[ low_byte(in_word) ] )
#define bit1_count( in_word )  \
	( 	bit1_cnt[ hi_byte(in_word) ] + bit1_cnt[ low_byte(in_word) ] )

#define left_part_0( of_word )      											  		\
	(  left_byte( of_word ) ?             										  		\
			lr_word( left_0[ left_byte(of_word) ],	0xFF ) 							\
		: 	lr_word( 0x00,	left_0[ right_byte(of_word) ] )                    \
	)

#define right_part_0( of_word )    														\
	(  right_byte( of_word ) ?            												\
			lr_word( 0xFF, right_0[ right_byte(of_word) ])     					\
		:	lr_word( right_0[ left_byte(of_word) ], 0x00 )							\
	)

#endif  // __BIT_FUNC_H
