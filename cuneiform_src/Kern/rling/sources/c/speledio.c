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

/*
  ******************************************************************
  * Module IoEd_Tg - "reading" and "writing"  Ed-file from and to  *
  * Tiger RAM buffer -- functions are used by speller incorporated *
  * in TIGER.                                                      *
  ******************************************************************
*/

//
// 08-13-93 06:48pm, Mike
// Comments:  Updating for multi-lingual version of CuneiForm ( release
//            28-June-93 ). Release has been received from Joe by FEDERAL
//            mail.
//
// Process indicator:
//  1. Preliminary updation. 08-13-93 06:40pm, Mike
//
// --------------------------------------------------------------------
// ---- Revision History
// - 1 SPELEDIO.C 13-Aug-93,18:51:04 Start point.
// - 2 SPELEDIO.C 14-Mar-94,17:22:54,`MIKE'
// -      This version has one serious correction: when read_file() prepare
// -      ED-file for working session it insert special "safeguard" space symbol
// -      after the last symbol of the ED-file. And when save_ed_file() saves
// -      results into output stream it removes this "safeguard" space symbol
// -      from output stream.
// -      The problem we did fight with is infinite loop in the case of removing
// -      last charachter of ED-file. This error is result of design fault of the
// -      input ED-stream handler.
// ---- Revision History
// --------------------------------------------------------------------

  #include "spelmode.h"
  
  #include <stdlib.h>
  #include <stdio.h>
  #include <string.h>
  #include <ctype.h>
  #include <setjmp.h>

//  #include "iolib.h"          // 08-13-93 06:46pm, Mike
  #include "speldefs.h"
  #include "spelfunc.h"
  #include  "tigeremulate.h"

  #include "compat_defs.h"
// --------------------------------------------------------------------
  void	ErrorExit(int Code);
  void ed_out_write(puchar p, uint16_t size);

  //AK kill all stupid macros
  //#define t  SPQ.t
  //#define st SPQ.st
  //#define N  SPQ.N


// --------------------------------------------------------------------

  extern uchar * ED_file_start;
  extern uchar * ED_file_end;
  extern dQ SPQ;            //Q;

// --------------------------------------------------------------------

  static void skip_safeguard( struct segm  *segm_ptr );

// --------------------------------------------------------------------
// --------------------------------------------------------------------
//
// This procedure stores new state of ed_file into output stream.
//
int16_t save_ed_file(uchar batch_run)
{
  struct segm  *segm_ptr;
  int16_t lth;

  batch_run++;
  segm_ptr=SPQ.tab_ptr->tab_sheet_descr[SPQ.cur_sheet].first_segm;
  lth = 0;

  if ( segm_ptr == NULL ) {
    return 0;
  }

  skip_safeguard( segm_ptr );

  while ( segm_ptr ) {
    ed_out_write( segm_ptr->string, segm_ptr->busy_lth );
    segm_ptr = segm_ptr->next_in_sheet;
  }

  return 1;
}

// --------------------------------------------------------------------
//
//  This procedure reads ed file into memory and generates
//  needed structures.
//
int16_t read_file( uchar batch_run )
{
	int16_t  i;
	char ret;
	int32_t shift;
	int16_t  read_cnt;
	LT   safeGuard = { 0x20, 0xF };

	batch_run++;
	read_cnt = 0;

	for ( i=0; i <= read_cnt; i++ )
	{
		SPQ.buff_l = SPQ.file_lth = ( ED_file_end - ED_file_start );
		SPQ.buff_ptr = SPQ.text_buff = ED_file_start;
		SPQ.l=0;

		for ( SPQ.l=0; SPQ.l < SPQ.buff_l; SPQ.buff_ptr++ )
		{
			ret=(char)processEdSymb();

			if (ret == TOO_MANY_FRAGMS)
			{
				return FALSE;
			}
			else
				if ( ret == NO )
				{
					shift=SPQ.l-SPQ.buff_l;
					lseek( SPQ.ed_file,shift,SEEK_CUR);
					break;
				}
				else
					if( ret==-1 )
						ErrorExit(RLING_ERROR_WRONG_ED_FILE);
		}
	}

	puff_last_segm();

    // Insert "safeguard" space symbol to avoid lexical integrity
    // destruction at the last symbol of ED-file.
    // SPQ.st.cur_segm must point to the last segment of ED-file!!!
	insert_symb( SPQ.st.cur_segm, NULL, &safeGuard );
	return TRUE;
}

// --------------------------------------------------------------------
//
//  This procedure removes "safeguard" space symbol inserted by
//  read_file() from the last position of the ED-file.
//
void skip_safeguard( struct segm  *segm_ptr )
{
  while ( segm_ptr->next_in_sheet ) {
    segm_ptr = segm_ptr->next_in_sheet;
  }

  skip_letter_in_line( segm_ptr, LAST_LETTER);
  shift( LEFT, sizeof(LT), segm_ptr, SPQ.ns_symb+sizeof(LT) );

  return;
}

// --------------------------------------------------------------------
