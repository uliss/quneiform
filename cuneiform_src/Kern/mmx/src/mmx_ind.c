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

#include"mmx.h"

extern   uint32_t bit_cnt_mmx[];

//
//       uint32_t   MMX_anding (int32 * images, int32 * etalon, int32 limit)
//
MMX_FUNC(Bool32) MMX_anding(int32_t *image,uint16_t *etalons,int32_t L)
{
#ifdef _MSC_VER
_asm    {

        mov     edx,    etalons
		xor     eax,    eax			; accounter of bits

;CYKL 0
        movq    mm0,    [edx]       ;   mm0 = etalon

        movq    mm1,    mm0
;IF     ARG EQ 0
        pand    mm0,    mm2
        pandn   mm1,    mm3
;ENDIF

;IF     ARG EQ 1
;        pand    mm0,    mm4
;        pandn   mm1,    mm5
;ENDIF

;IF     ARG EQ 2
;        pand    mm0,    mm6
;        pandn   mm1,    mm7
;ENDIF

;IF     ARG EQ 3
;		mov     edx,    image
;        pand    mm0,    [edx+24]
;        pandn   mm1,    [edx+8*4+24]
;ENDIF
;   mm0 = tmp & (image   = iobraz)
;   mm1 =~tmp & (image+8 = iobraz2)
;IF     ARG EQ 0
		sub		eax,	L
;ENDIF
        movd   esi,    mm0
		mov		edi,	esi
		and		esi,	0ffffh
        movd   ebx,    mm1
		shr		edi,	16
		mov		ecx,	ebx
		and		ebx,	0ffffh
		shr		ecx,	16
        add     eax,    dword ptr bit_cnt_mmx[esi*4]  ;    0-15

		add     eax,    dword ptr bit_cnt_mmx[ebx*4]  ;    0-15
		jge			retur

        psrlq   mm0,    32
		add     eax,    dword ptr bit_cnt_mmx[edi*4]  ;    16-31
        psrlq   mm1,    32
        add     eax,    dword ptr bit_cnt_mmx[ecx*4]  ;    16-31
		jge			retur

        movd   esi,    mm0
		mov		edi,	esi
		and		esi,	0ffffh
        movd   ebx,    mm1
		shr		edi,	16
		mov		ecx,	ebx
		and		ebx,	0ffffh
		shr		ecx,	16
        add     eax,    dword ptr bit_cnt_mmx[esi*4]  ;    32-47

		add     eax,    dword ptr bit_cnt_mmx[ebx*4]  ;    32-47
		jge			retur

		add     eax,    dword ptr bit_cnt_mmx[edi*4]  ;    48-63
		add     edx,    8
        add     eax,    dword ptr bit_cnt_mmx[ecx*4]  ;    48-63
;IF     ARG NE 3
		jge			retur
;ENDIF

;        CYKL 1
        movq    mm0,    [edx]       ;   mm0 = etalon

        movq    mm1,    mm0
;IF     ARG EQ 0
;        pand    mm0,    mm2
;        pandn   mm1,    mm3
;ENDIF

;IF     ARG EQ 1
        pand    mm0,    mm4
        pandn   mm1,    mm5
;ENDIF

;IF     ARG EQ 2
;        pand    mm0,    mm6
;        pandn   mm1,    mm7
;ENDIF

;IF     ARG EQ 3
;		mov     edx,    image
;        pand    mm0,    [edx+24]
;        pandn   mm1,    [edx+8*4+24]
;ENDIF
;   mm0 = tmp & (image   = iobraz)
;   mm1 =~tmp & (image+8 = iobraz2)
;IF     ARG EQ 0
;		sub		eax,	L
;ENDIF
        movd   esi,    mm0
		mov		edi,	esi
		and		esi,	0ffffh
        movd   ebx,    mm1
		shr		edi,	16
		mov		ecx,	ebx
		and		ebx,	0ffffh
		shr		ecx,	16
        add     eax,    dword ptr bit_cnt_mmx[esi*4]  ;    0-15

		add     eax,    dword ptr bit_cnt_mmx[ebx*4]  ;    0-15
		jge			retur

        psrlq   mm0,    32
		add     eax,    dword ptr bit_cnt_mmx[edi*4]  ;    16-31
        psrlq   mm1,    32
        add     eax,    dword ptr bit_cnt_mmx[ecx*4]  ;    16-31
		jge			retur

        movd   esi,    mm0
		mov		edi,	esi
		and		esi,	0ffffh
        movd   ebx,    mm1
		shr		edi,	16
		mov		ecx,	ebx
		and		ebx,	0ffffh
		shr		ecx,	16
        add     eax,    dword ptr bit_cnt_mmx[esi*4]  ;    32-47

		add     eax,    dword ptr bit_cnt_mmx[ebx*4]  ;    32-47
		jge			retur

		add     eax,    dword ptr bit_cnt_mmx[edi*4]  ;    48-63
		add     edx,    8
        add     eax,    dword ptr bit_cnt_mmx[ecx*4]  ;    48-63
;IF     ARG NE 3
		jge			retur
;ENDIF

;       CYKL 2
        movq    mm0,    [edx]       ;   mm0 = etalon

        movq    mm1,    mm0
;IF     ARG EQ 0
;        pand    mm0,    mm2
;        pandn   mm1,    mm3
;ENDIF

;IF     ARG EQ 1
;        pand    mm0,    mm4
;        pandn   mm1,    mm5
;ENDIF

;IF     ARG EQ 2
        pand    mm0,    mm6
        pandn   mm1,    mm7
;ENDIF

;IF     ARG EQ 3
;		mov     edx,    image
;        pand    mm0,    [edx+24]
;        pandn   mm1,    [edx+8*4+24]
;ENDIF
;   mm0 = tmp & (image   = iobraz)
;   mm1 =~tmp & (image+8 = iobraz2)
;IF     ARG EQ 0
;		sub		eax,	L
;ENDIF
        movd   esi,    mm0
		mov		edi,	esi
		and		esi,	0ffffh
        movd   ebx,    mm1
		shr		edi,	16
		mov		ecx,	ebx
		and		ebx,	0ffffh
		shr		ecx,	16
        add     eax,    dword ptr bit_cnt_mmx[esi*4]  ;    0-15

		add     eax,    dword ptr bit_cnt_mmx[ebx*4]  ;    0-15
		jge			retur

        psrlq   mm0,    32
		add     eax,    dword ptr bit_cnt_mmx[edi*4]  ;    16-31
        psrlq   mm1,    32
        add     eax,    dword ptr bit_cnt_mmx[ecx*4]  ;    16-31
		jge			retur

        movd   esi,    mm0
		mov		edi,	esi
		and		esi,	0ffffh
        movd   ebx,    mm1
		shr		edi,	16
		mov		ecx,	ebx
		and		ebx,	0ffffh
		shr		ecx,	16
        add     eax,    dword ptr bit_cnt_mmx[esi*4]  ;    32-47

		add     eax,    dword ptr bit_cnt_mmx[ebx*4]  ;    32-47
		jge			retur

		add     eax,    dword ptr bit_cnt_mmx[edi*4]  ;    48-63
		add     edx,    8
        add     eax,    dword ptr bit_cnt_mmx[ecx*4]  ;    48-63
;IF     ARG NE 3
		jge			retur
;ENDIF

;       CYKL 3
        movq    mm0,    [edx]       ;   mm0 = etalon

        movq    mm1,    mm0
;IF     ARG EQ 0
;        pand    mm0,    mm2
;        pandn   mm1,    mm3
;ENDIF

;IF     ARG EQ 1
;        pand    mm0,    mm4
;        pandn   mm1,    mm5
;ENDIF

;IF     ARG EQ 2
;        pand    mm0,    mm6
;        pandn   mm1,    mm7
;ENDIF

;IF     ARG EQ 3
		mov     edx,    image
        pand    mm0,    [edx+24]
        pandn   mm1,    [edx+8*4+24]
;ENDIF
;   mm0 = tmp & (image   = iobraz)
;   mm1 =~tmp & (image+8 = iobraz2)
;IF     ARG EQ 0
;		sub		eax,	L
;ENDIF
        movd   esi,    mm0
		mov		edi,	esi
		and		esi,	0ffffh
        movd   ebx,    mm1
		shr		edi,	16
		mov		ecx,	ebx
		and		ebx,	0ffffh
		shr		ecx,	16
        add     eax,    dword ptr bit_cnt_mmx[esi*4]  ;    0-15

		add     eax,    dword ptr bit_cnt_mmx[ebx*4]  ;    0-15
		jge			retur

        psrlq   mm0,    32
		add     eax,    dword ptr bit_cnt_mmx[edi*4]  ;    16-31
        psrlq   mm1,    32
        add     eax,    dword ptr bit_cnt_mmx[ecx*4]  ;    16-31
		jge			retur

        movd   esi,    mm0
		mov		edi,	esi
		and		esi,	0ffffh
        movd   ebx,    mm1
		shr		edi,	16
		mov		ecx,	ebx
		and		ebx,	0ffffh
		shr		ecx,	16
        add     eax,    dword ptr bit_cnt_mmx[esi*4]  ;    32-47

		add     eax,    dword ptr bit_cnt_mmx[ebx*4]  ;    32-47
		jge			retur

		add     eax,    dword ptr bit_cnt_mmx[edi*4]  ;    48-63
		add     edx,    8
        add     eax,    dword ptr bit_cnt_mmx[ecx*4]  ;    48-63
;IF     ARG NE 3
;		jge			retur
;ENDIF
retur:
		add		eax,	L
        }

#else
    return 0;
#endif
}


MMX_FUNC(void) MMX_open( int32_t *image)
{
#ifdef _MSC_VER
_asm    {
		mov     edx,    image
		movq	mm2,	[edx]	     ;   MM2 = (image   = iobraz)
		movq	mm3,	[edx+8*4]    ;   MM3 = (image+8 = iobraz2)
		movq	mm4,	[edx+8]	     ;   MM4 = (image   = iobraz)
		movq	mm5,	[edx+8*4+8]  ;   MM5 = (image+8 = iobraz2)
		movq	mm6,	[edx+16]	 ;   MM6 = (image   = iobraz)
    	movq	mm7,	[edx+8*4+16] ;   MM7 = (image+8 = iobraz2)
        }
#endif
}

MMX_FUNC(void) MMX_close(void)
{
#ifdef _MSC_VER
_asm		EMMS
#endif
}
