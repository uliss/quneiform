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

#ifndef __XYDIM_H
#	define __XYDIM_H

   #ifndef __LNSLANG_H
   #  include "lnslang.h"
	#endif
	class TXDim // : public TFarHeap
   {
		public:
			int left;
			int right;

         TXDim( void ){};
			TXDim( int x1, int x2 ): left(x1), right(x2) {};
			TXDim( TXDim* seg ): left(seg->left), right(seg->right) {};
			int width(void) {
				return (right-left+1);
			};
			int length(void) {
				return (right-left+1);
			};
         Bool  isXIntersected( TXDim* brother ){
				return 	( 	(	this->left 	<= brother->right	)
							&&	( 	brother->left 	<= this->right	)
							);
			};
         Bool  isXEmbedded( TXDim* father ){
				return (		(	this->left	>= father->left	)
							&&	( 	this->right	<= father->right 	)
					);
			};
         void expandX( int l, int r ){
				left = min( left, l );
				right = max( right, r);
			};

	};

	class TYDim  // : public TFarHeap
   {
		public:
			int bottom;
			int top;

         TYDim( void ){};
			TYDim( int y1, int y2 ): bottom(y1), top(y2) {};
			TYDim( TYDim* seg ): bottom(seg->bottom), top(seg->top) {};
			int height(void) {
				return (top-bottom+1);
			};
         Bool  isYIntersected( TYDim* brother ){
         	return ( 	(	this->bottom		<= brother->top	)
							&& ( 	brother->bottom	<= this->top 		)
				);
			};
         Bool  isYEmbedded( TYDim* father ){
         	return ( 	(	this->bottom	>= father->bottom	)
							&& ( 	this->top		<= father->top 	)
				);
			};
         void expandY( int b, int t ){
				bottom = min( bottom, b );
				top = max( top, t );
			};

	};

	class TXYDim:
		public TXDim,
		public TYDim
	{
   	public:

         TXYDim( void ){};
			TXYDim( int x1, int x2, int y1, int y2 ) :
				TXDim( x1, x2),
				TYDim( y1, y2) {};
			TXYDim( TXYDim* box ) :
				TXDim( (TXDim*)box ),
				TYDim( (TYDim*)box ) {};
         Bool isXYIntersected( TXYDim* brother_box ){
				return (		isXIntersected(brother_box)
							&&	isYIntersected(brother_box)
						 );
         };
         Bool isXYEmbedded( TXYDim* father_box ){
				return (		isXEmbedded(father_box)
							&&	isYEmbedded(father_box)
						 );
			};
         void restrictXY( int x1, int x2, int y1, int y2 ){
				left 		= 	max( left, x1 );
				right		=  min( right, x2 );
				bottom	= 	max(bottom, y1);
				top    	=  min(top, y2);
			};
         void expandXY( TXYDim* brother ){
				expandX( brother->left, brother->right );
				expandY( brother->bottom, brother->top );
			};
         void expandXY( int l, int r, int b, int t ){
				expandX( l, r );
				expandY( b, t );
			};
	};

#endif
