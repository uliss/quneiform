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

#include "lnslang.h"

	#ifndef __BLCKWHTE_H
	#	include "blckwhte.h"
	#endif

	#ifndef __SBAMBUK_H
	#  include "sbambuk.h"
	#endif

	void SetupHConverter( int right,
						  int left,
						  int filter_len,
                    TSegBambuk* bambuk,
                    Bool include_borders
						);

   void ConvertBWLine( void* image_line_ptr );


#define Word16_BIT_SIZE   16

#ifndef NDEBUG
	int32_t tot_seg_extracted = 0;
#endif

	static uint16_t left_pixel_ = 1;           // left border in pixels [1..)
	static uint16_t right_pixel_ = 0;          // right border (included) in pixels [16..)
	static uint16_t left_word_offset_ = 0;     // offset from start of line in word (0..);
	static uint16_t left_pixel_offset_ = 0;    // pixel in word from wich starts line
                                          // [0..Word16_BIT_SIZE-1]
	static uint16_t right_pixel_offset_ = 0;
   static Bool include_border_segments_ = TRUE; // if FALSE - segments touching border are ignored
   static TSegBambuk* bambuk_ptr_ = NULL;       // all line segments
	static int filter_len_ = 0;            // segments shorter (or equal) filter_len
														// are ignored

	static inline void new_segment( int left, int right );

#ifndef NDEBUG
//	extern word seg_len_histo[34];
#endif


void SetupHConverter( int 			left,
					  int 			right,
					  int 			filter_len,
                 TSegBambuk*  bambuk,
                 Bool         include_borders
							)
 {
	assert( right > left );
	assert( filter_len >= 0 );
	assert( bambuk != NULL );
	assert( bambuk->isOk() );

	left_pixel_ = left;
	right_pixel_ = right;
   left_word_offset_ = left / Word16_BIT_SIZE;
   left_pixel_offset_ = left % Word16_BIT_SIZE;
   right_pixel_offset_ = right % Word16_BIT_SIZE;
	include_border_segments_ = include_borders;

	bambuk_ptr_ = bambuk;
	filter_len_ = filter_len;
 }

#define NO_DOTMATRIX
static int prev_left=0;
static int prev_right=0;

static inline void line_start()
{
   prev_right = 0;
   prev_left  = 0;
}

static inline void line_finish()
{
#ifndef NO_DOTMATRIX  // old style processing
   if (prev_right)
      if ((prev_right-prev_left) >= filter_len_)
		   bambuk_ptr_->addHorisontal( prev_left, prev_right );
#endif
}

static inline void new_segment( int left, int right )
 {

#ifdef NO_DOTMATRIX  // old style processing

	if ((right-left) >= filter_len_)
		bambuk_ptr_->addHorisontal( left, right );

#else  // DOTMATRIX

   if (prev_right) // not first segment
   {
      if (left - prev_right < 4) // small gap?
         prev_right = right;     // expand right
      else  // big gap - register prev segment and save new one
      {
      	if ((prev_right-prev_left) >= filter_len_)
		      bambuk_ptr_->addHorisontal( prev_left, prev_right );
         prev_left = left;
         prev_right=right;
      }
   }
   else
   {  // first segment
      prev_left = left;
      prev_right=right;
   }

#endif // NO_DOTMATRIX

 }

inline uint16_t GetWord16( uint16_t* pw )
#ifndef BIG_ENDIAN
{  return *pw; }
#else
{  return (*pw << 8 ) | (*pw >> 8); }
#endif


const int trail_zeros[256] = // byte=>count of trailed zeros
{
/*       0 1 2 3 4 5 6 7 8 9 a b c d e f */
/*0*/    8,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,
/*1*/    4,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,
/*2*/    5,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,
/*3*/    4,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,
/*4*/    6,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,
/*5*/    4,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,
/*6*/    5,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,
/*7*/    4,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,
/*8*/    7,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,
/*9*/    4,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,
/*a*/    5,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,
/*b*/    4,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,
/*c*/    6,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,
/*d*/    4,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,
/*e*/    5,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,
/*f*/    4,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0
};


#include "fillgap3.h"
void ConvertBWLine(  void*      image_line_ptr
						)
{
#ifndef BLACK_IS_ZERO
#error Wow, !BLACK_IS_ZERO, Correct, me please!
#endif

   image_line_ptr = FillGap3_SubstLine( (uchar*)image_line_ptr );

	register uchar* pb = (uchar*)image_line_ptr + left_pixel_/8;
   uchar* pfirst = (uchar*)image_line_ptr;
   uchar* plast  = (uchar*)image_line_ptr + right_pixel_/8;
	uchar last_byte = *plast;
   int left, right;
   line_start();
#ifdef NO_DOTMATRIX
	for (;;)
	{
	   // loop on white area
	   *plast = 0x00;
	   while (*++pb){;}        // skip for entire black segment
	   if (!(plast-pb))	// stopped on last byte?
		   break;
      // started black area...
      left = ((pb-pfirst)<<3) - trail_zeros[*(pb-1)];
      *plast = 0xff;
	   while (!*++pb){;}
	   if (!(plast-pb))	// stopped on last byte?
      {
         right = ((pb-pfirst)<<3) + first_bit1_pos[ last_byte ];
         new_segment(left, right);
         break;
      };
      right = ((pb-pfirst)<<3) + first_bit1_pos[*pb];
      new_segment(left, right);
	};
#else    // DOTMATRIX
   while (plast!=pb) // not the last byte
   {
      if (white)
      {
   	   *plast = 0x00;
	      while (*++pb==0xff){;}  // skip clean white bytes
   	   if (!(plast-pb))	// stopped on last byte?
	   	   break;

         AlgStatus& as= stat_tbl[*pb];
         if (as.next)
         {  *pb++;
            continue;
         }
         else
         {
            right = as.right;
            *pb = as.


         white


         left = first_bit0_pos[*pb]+((pb-pfirst)<<3);
         pb +=

      }
      else
      {
      }
   }


	for (;;)
	{
	   // loop on white area
	   *plast = 0x00;
	   while (*++pb==0xff){;}  // skip clean white bytes
	   if (!(plast-pb))	// stopped on last byte?
		   break;
      // started black area...
      left = ((pb-pfirst)<<3) - trail_zeros[*(pb-1)];
      *plast = 0xff;
	   while (!*++pb){;}
	   if (!(plast-pb))	// stopped on last byte?
      {
         right = ((pb-pfirst)<<3) + first_bit1_pos[ last_byte ];
         new_segment(left, right);
         break;
      };
      right = ((pb-pfirst)<<3) + first_bit1_pos[*pb];
      new_segment(left, right);
	};
#endif

   line_finish();
}
