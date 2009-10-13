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

	#ifndef __SBAMBUK_H
	#	include "sbambuk.h"
	#endif
	#ifndef __RBAMBUK_H
	#	include "rbambuk.h"
	#endif

	#ifndef NDEBUG

    #endif

   TSegBambuk* sb_ = NULL;

Bool        TRasterBambuk::makeIt( TSegBambuk* sb ) //build rasters by segbambuk
	{
		BEntry cur_entry, prev_entry;
   	BHandle cur_seg, prev_seg;
      TBlackSeg* lower, *upper;

		assert(sb);
		sb_ = sb;

		// create first line segments components...
		cur_entry = 0;
		cur_seg = sb_->firstEntryMember( cur_entry );
		while (cur_seg != NULLBHandle){
      	if (( (*sb_)[cur_seg].dashHandle = startDash( 	cur_seg,
																			cur_entry
				 													  )
             ) == NULLBHandle
				) return (WRONG());

			cur_seg = sb_->nextMember( cur_seg );
		};

		for ( cur_entry = 1; cur_entry <= sb_->lastEntry(); cur_entry++ ){
			prev_entry = cur_entry - 1;
			prev_seg = sb_->firstEntryMember( prev_entry );
			cur_seg 	= sb_->firstEntryMember( cur_entry );
			if (prev_seg == NULLBHandle){
				if (cur_seg == NULLBHandle)
					goto BothLinesHasEnded;
				lower = &((*sb_)[cur_seg]);
				goto UpperLineHasEnded;
			};
			upper = &((*sb_)[prev_seg]);
			assert( upper->dashHandle != NULLBHandle );
			if (cur_seg == NULLBHandle)
				goto LowerLineHasEnded;
			lower = &((*sb_)[cur_seg]);

			/***** main "two lines" loop *******/
			while (TRUE){
				assert(prev_seg != NULLBHandle);
				assert(cur_seg != NULLBHandle);

				#ifndef NDEBUG
					//if (lines_processed ++ > 100){
					//	lines_processed = 1;
					//	printf( "." );
					//};
				#endif
				// both segments are not processed yet,
				// but they adresses are getted

				/* Part 1/2 : Not intersected segments */
				if  (upper->right < lower->left){
					completeDash( upper->dashHandle );
					upper->nLower = NULLBHandle;
					if ((prev_seg = sb_->nextMember( prev_seg ))==NULLBHandle)
						goto UpperLineHasEnded;
					upper = &((*sb_)[prev_seg]);
					assert( upper->dashHandle != NULLBHandle );
					continue;
				};

				if  (lower->right < upper->left){
		      	if (( lower->dashHandle = startDash( 	cur_seg,
																		cur_entry
						 										  )
         		    ) == NULLBHandle
						) return (WRONG());

					if ((cur_seg = sb_->nextMember( cur_seg ))==NULLBHandle)
						goto LowerLineHasEnded;
					lower = &((*sb_)[cur_seg]);
					continue;
				};

				/* Part 2/2 : Intersected segments */
				// first intersection - simple link they...
				upper->nLower = cur_seg;
				assert( upper->dashHandle != NULLBHandle );
				lower->dashHandle = upper->dashHandle;
				if 		(upper->right > lower->right){
	ProcOtherLower:
					// current lower has been processed, skip it.
					assert( lower->dashHandle != NULLBHandle );
					if ((cur_seg = sb_->nextMember( cur_seg ))==NULLBHandle){
						// lower has ended - skip upper
						if ((prev_seg = sb_->nextMember( prev_seg ))==NULLBHandle)
							goto BothLinesHasEnded;
						upper = &((*sb_)[prev_seg]);
						assert( upper->dashHandle != NULLBHandle );
						goto LowerLineHasEnded;
					};
					lower = &((*sb_)[ cur_seg ]);
					while (upper->right > lower->right){
						// start lower line
			      	if (( lower->dashHandle = startDash( 	cur_seg,
																			cur_entry,
                                                         rasterEntry( upper )
							 										  )
         			    ) == NULLBHandle
							) return (WRONG());

						// get next lower
						assert( lower->dashHandle != NULLBHandle );
						if ((cur_seg = sb_->nextMember( cur_seg ))==NULLBHandle){
							if ((prev_seg = sb_->nextMember( prev_seg ))==NULLBHandle)
								goto BothLinesHasEnded;
							upper = &((*sb_)[prev_seg]);
							assert( upper->dashHandle != NULLBHandle );
							goto LowerLineHasEnded;
						};
						lower = &((*sb_)[ cur_seg ]);
					};
					if (upper->right < lower->left){
						// skip upper and loop
						if ((prev_seg = sb_->nextMember( prev_seg ))==NULLBHandle)
							goto UpperLineHasEnded;
						upper = &((*sb_)[prev_seg]);
						assert( upper->dashHandle != NULLBHandle );
						continue;
               };
					// upper->left <= lower->right
					// start lower line
              	if (( lower->dashHandle = startDash( 	cur_seg,
																		cur_entry,
                                                      rasterEntry( upper )
						 										  )
         		    ) == NULLBHandle
						) return (WRONG());

					goto ProcOtherUpper;
				};

				if	(upper->right < lower->right){
	ProcOtherUpper:
					// get next upper
					if ((prev_seg = sb_->nextMember( prev_seg ))==NULLBHandle){
						// upper has ended - skip lower
						if ((cur_seg = sb_->nextMember( cur_seg ))==NULLBHandle)
							goto BothLinesHasEnded;
						lower = &((*sb_)[cur_seg]);
						goto UpperLineHasEnded;
					};
					upper = &((*sb_)[ prev_seg ]);
					assert( upper->dashHandle != NULLBHandle );
					while (upper->right < lower->right){
						// complete upper line and may be join rasters
						completeDash( upper->dashHandle );
						upper->nLower = NULLBHandle;

						joinRasters(rasterEntry( upper ), rasterEntry( lower ));
						/*******************************************************/
						// get next upper
						if ((prev_seg = sb_->nextMember( prev_seg ))==NULLBHandle){
							assert( lower->dashHandle != NULLBHandle );
							if ((cur_seg = sb_->nextMember( cur_seg ))==NULLBHandle)
								goto BothLinesHasEnded;
							lower = &((*sb_)[cur_seg]);
							goto UpperLineHasEnded;
						};
						upper = &((*sb_)[ prev_seg ]);
						assert( upper->dashHandle != NULLBHandle );
					};
					if (upper->left > lower->right){
						// skip lower and loop
						assert( lower->dashHandle != NULLBHandle );
						if ((cur_seg = sb_->nextMember( cur_seg ))==NULLBHandle)
							goto LowerLineHasEnded;
						lower = &((*sb_)[cur_seg]);
						continue;
               };
					// upper->left <= lower->right
					// complete upper line and may be join rasters
					completeDash( upper->dashHandle );
					upper->nLower = NULLBHandle;

					joinRasters(rasterEntry( upper ), rasterEntry( lower ));
					/******************************************************/

					goto ProcOtherLower;
				};
				// both segments ended simultaneously
				assert( upper->dashHandle != NULLBHandle );
				assert( lower->dashHandle != NULLBHandle );

				cur_seg = sb_->nextMember( cur_seg );
				prev_seg = sb_->nextMember( prev_seg );
				if (prev_seg == NULLBHandle){
					if (cur_seg == NULLBHandle)
						goto BothLinesHasEnded;
					lower = &((*sb_)[cur_seg]);
					goto UpperLineHasEnded;
				};
				upper = &((*sb_)[prev_seg]);
				assert( upper->dashHandle != NULLBHandle );
				if (cur_seg == NULLBHandle)
					goto LowerLineHasEnded;
				lower = &((*sb_)[cur_seg]);
			}; //...while ((prev_seg != NULLBHandle) && (cur_seg != NULLBHandle)
			/*** end of main "two lines" loop ****/
	UpperLineHasEnded:        // upper ended, lower is not processed yet
			assert( cur_seg != NULLBHandle );
			do	if (((*sb_)[cur_seg].dashHandle = startDash( 	cur_seg,
																				cur_entry
					 													  )
        		    ) == NULLBHandle
					) return (WRONG());
			while (( cur_seg = sb_->nextMember( cur_seg) ) != NULLBHandle );
			goto BothLinesHasEnded;

	LowerLineHasEnded:        // lower ended, upper is not processed yet
			assert( prev_seg != NULLBHandle );
			do {
				completeDash( (*sb_)[prev_seg].dashHandle );
				(*sb_)[prev_seg].nLower = NULLBHandle;
			} while (( prev_seg = sb_->nextMember( prev_seg) ) != NULLBHandle );
			goto BothLinesHasEnded;

	BothLinesHasEnded:   ;     // ...
//	    static int dd = 0; // added to avoid compiler's objections  03-01-96 Akimenkov
		}; // for

		// complete last line segments components...
		prev_seg = sb_->firstEntryMember( sb_->lastEntry() );
		while (prev_seg != NULLBHandle){
			completeDash( (*sb_)[prev_seg].dashHandle );
			(*sb_)[prev_seg].nLower = NULLBHandle;
			prev_seg = sb_->nextMember( prev_seg );
		};

      return TRUE;
	}

void TRasterBambuk::joinRasters( BEntry first, BEntry second ){
			if ( first != second ){
				BHandle cur_dash;
				assert( first 	!= NULLBEntry );
				assert( second != NULLBEntry );
//				if (memberEntryCount(first) < memberEntryCount(second))
//					swap( first, second );
				cur_dash = firstEntryMember( second );
				assert( cur_dash != NULLBHandle );
				do (*this)[ cur_dash ].rasterEntry = first;
				while ((cur_dash = nextMember( cur_dash )) != NULLBHandle );
				appendMemberList( first, second );
				count--;
			};
		}
