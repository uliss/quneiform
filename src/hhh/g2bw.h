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

#ifndef __G2BW_H
#define __G2BW_H

#include "globus.h"
#include "imgaccs_gbw.h"

#ifndef GBW_FUNC
        #define GBW_FUNC FUN_IMPO
#endif

enum TBinEnum {VIA_DEZA,VIA_KRONROD};


#ifdef __cplusplus
 extern "C"
  {
#endif
/*
	ЗАМЕЧАНИЯ.
1. Работа трио базируется на информации получаемой из структуры ImageInfo.
2. Размер серого изображения (в байтах) равен ImageInfo->wImageWidth.
3. Размер получаемого чб изображения вычисляется по формуле (ImageInfo->wImageWidth+7)/8 (для целых!!!).
4. Учитывается флаг инверсности ImageInfo->bFotoMetrics
5. fGetGreyBlock имеет три параметра: адрес куда записывать серое, начальная строка, число запрашиваемых строк
*/

GBW_FUNC( Bool ) OpenBinarizator  (enum TBinEnum benum,
								   LPTIGER_IMAGEINFO ImageInfo,
									TImageGetBlock fGetGreyBlock
//#ifdef gbwMANUBRI      // version with manual Image Intensity flag
		 ,
		  short int ImageInty   // 0 - "weak", 1- "normal", 2 -"dirty"
//#endif
					   );
GBW_FUNC( uint16_t ) Binarize  ( puchar lpBuff, uint16_t Lth);
/*
	lpBuff - адрес, куда писать черно-белое
	lth    - число байт
Возвращает 0, если изображение кончилось.
*/
GBW_FUNC( void )   CloseBinarizator (void);

#ifdef WIN32
	#include "windows.h"
/*
	Преобразование серого 8 битного DIB изображения в ч.б. DIB.
	параметры:
		benum - вид бинаризации (см выше)
		ImageInty - качество изображения (см выше)
		hInMemory - handle глобальной памяти на серый DIB
	Возврашает NULL в случае ошибки или handle глобальной памяти на ч.б. DIB.
	Для освобождения памяти надо вызвать GlobalFree(<ч.б. DIB>).


*/
//  hInMemory - 8 pixel-gray image DIB
//  Return B/W image DIB or NULL if error.
GBW_FUNC( HGLOBAL ) GreyToMono(enum TBinEnum benum,short int ImageInty,HGLOBAL hInMemory);
GBW_FUNC( void    ) DestroyMono(HGLOBAL hInMemory);

GBW_FUNC(uint16_t)	Grey_open(enum TBinEnum benum,uint16_t Width, uint16_t Height);
GBW_FUNC(uint16_t)	Grey_to(puchar pTo);
GBW_FUNC(void)		Grey_from(puchar pFrom);
GBW_FUNC(void)		Grey_close(void);

#ifndef DPITODPM
	#define DPITODPM(a) ((long)a * 254L / 10000L)
	#define DPMTODPI(a) ((long)a * 10000L / 254L)
#endif

#endif // WIN32

#ifdef __cplusplus
};
#endif

#endif   // __G2BW_H
