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

	#ifndef __RBAMBUK_H
	#define __RBAMBUK_H

   #ifndef __LNSLANG_H
   #  include "lnslang.h"
	#endif

//_TYPEDEFFAR( TRasterBambuk )

	#ifndef __FARARRAY_H
	#	include "fararray.h"
	#endif

	#ifndef __SBAMBUK_H
	#	include "sbambuk.h"
	#endif


/*
* TRasterBambuk*******************************************

	Bambuk of Rasters to Dashes

*/


struct TDash {
	BHandle firstSegHandle;    // points to first segment (handle of segbambuk)
	BEntry  firstSegEntry;     // first segment column/row (entry of segbambuk)
	BEntry  rasterEntry;       // own entry in raster bambuk
};

class TRasterBambuk : public TBambuk< TDash > {
	public:
TRasterBambuk( TSegBambuk* sb, BHandle max_dashes, BEntry max_rasters ):
	TBambuk< TDash >(max_dashes, max_rasters),
	count(0),
	nextRasterEntry(0),
	ok( FALSE )
	{
		if (TBambuk< TDash >::isOk())
			ok = makeIt( sb );      // false if too many dashes or rasters
	};

Bool isOk( void ) { return (TBambuk< TDash >::isOk() && ok); };
int32_t totalRasterCount( void ) { return (count); };
	private:
      int32_t count;     // result count of rasters
      int32_t nextRasterEntry;
      Bool ok;
      Bool makeIt( TSegBambuk* sb );

      void completeDash( BHandle dash_handle ){;};
		BHandle startDash( 	BHandle segment_handle,
									BEntry segment_entry,
									BEntry raster_entry = NULLBEntry
								){
			BHandle dash_handle;
			if (raster_entry==NULLBEntry){ // this is first line in component
				raster_entry = nextRasterEntry++;
            if (raster_entry > lastEntry())
               return NULLBHandle;
				count++;
			};
			if ((dash_handle = addMember( raster_entry )) != NULLBHandle){
				(*this)[ dash_handle ].firstSegHandle = segment_handle;
				(*this)[ dash_handle ].firstSegEntry  = segment_entry;
				(*this)[ dash_handle ].rasterEntry	  = raster_entry;
			};
			return (dash_handle);
		};
      void joinRasters( BEntry first, BEntry second );

		BEntry rasterEntry( TBlackSeg* seg ){
			assert( seg->dashHandle != NULLBHandle );
			return ((*this)[seg->dashHandle].rasterEntry);
		};

};

	#endif // __RBAMBUK_H
