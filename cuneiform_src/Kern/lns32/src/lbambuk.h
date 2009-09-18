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

   #ifndef __LBAMBUK_H
   #define __LBAMBUK_H

   #ifndef __LNSLANG_H
   #  include "lnslang.h"
	#endif

	#ifndef __BAMBUK_H
	#	include "Bambuk.h"
	#endif
	#ifndef __FARARRAY_H
	#	include "FarArray.h"
	#endif
	#ifndef __RBAMBUK_H
	#	include "RBambuk.h"
	#endif
	#ifndef __LNSDEFS_H
	#	include "lnsdefs.h"
	#endif

#ifndef __HLINER_H
#include "hliner.h"
#endif

/*
* TLinesBambuk*******************************************

	Bambuk of Lines to it Fragments  +  LinesInfoArray corresponding to
	bambuk's entries

*/

struct Line {
	Point32 start;
	Point32 end;
   int16_t   width10;
   int16_t   flags;          // LF_,,, in LnsFrag from lnsdefs.h
#define LF_DONTLINK 0x0001 // don't try to link
#define LF_HLINER   0x0002 // extracted by HLiner

// Bool  isStartJoined;    // 10-11-93 09:27pm, Basil
// Bool  isEndJoined;      // ...
};

typedef 	Line * 	PLine;

struct TLineInfo {
	Line		lineAsIs;
	BEntry	linesBambukEntry;
	int 		level;                // line intersection with 0 axis
};

typedef TArray< TLineInfo > TLineInfoArray;

struct TLineFragment {
	Line		fragmentAsIs;
	BEntry 	rasterBambukEntry;    // pointer to proper raster
    int		level;
};
typedef TLineFragment * PTLineFragment;

class TLinesBambuk :
	public TBambuk< TLineFragment >,
	public TXYDim
{
public:

TLineInfoArray  linesRoot;   // some addition to bambuk entries (||)
TLinesBambuk( 	TRasterBambuk* rb, TSegBambuk* sb,
					BEntry max_lines,
               Bool isVertical
				 ):
TBambuk< TLineFragment >( rb->totalRasterCount()
                           + (isVertical ? 0 : HLiner_GetCount()),
                           max_lines
                        ),
	linesRoot(max_lines),
	fragmentsCount(0),
	linesCount(0),
	ok( FALSE )
	{
		if (linesRoot.isOk() && TBambuk< TLineFragment >::isOk()){
         ok = makeIt( rb, sb, isVertical );      // false if too many lines or rasters
   	};
	};

~TLinesBambuk( void ) { destroy(); };

int32_t fragmentsCount;
int32_t linesCount;     // result count of lines
double averagePhi;

Bool isOk( void ) { return (TBambuk< TLineFragment >::isOk() && ok); };
int compareEntry( const void *entry1, const void *entry2 );

void destroy( void )
{
	linesRoot.flash();
	destroyBambuk();
}

Bool linkHFragments();
Bool linkVFragments();

	private:
      Bool ok;
      Bool makeIt( TRasterBambuk* rb, TSegBambuk* sb, Bool isVertical );
      void computeHRasters( TRasterBambuk* rb, TSegBambuk* sb );
      void computeVRasters( TRasterBambuk* rb, TSegBambuk* sb );

      void sortByLevel();
};

	#endif
