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

#include "mmx.h"

MMX_FUNC(uint32_t) MMX_scalar(uint32_t _EAX, uint32_t _EDX)
{
#ifdef _MSC_VER
_asm{
    mov eax, _EAX
    mov edx, _EDX

	MOVQ    MM0,    [EAX]     ;; mm0 = (a4, a3, a2, a1)           4 words
	MOVQ    MM1,    [EDX]     ;; mm1 = (b4, b3, b2, b1)           4 words

	MOVQ    MM2,    [EAX+8]   ;; mm2 = (a8, a7, a6, a5)           4 words
	PMADDWD MM0,    MM1       ;; mm0 = (a4*b4+a3*b3, a2*b2+a1*b1) 2 dwords
	MOVQ    MM3,    [EDX+8]   ;; mm3 = (b8, b7, b6, b5)           4 words

	MOVQ    MM4,    [EAX+16]  ;; mm4 = (a12, a11, a10, a9)        4 words
	PMADDWD MM2,    MM3       ;; mm2 = (a8*b8+a7*b7, a6*b6+a5*b5) 2 dwords
	MOVQ    MM5,    [EDX+16]  ;; mm5 = (b12, b11, b10, b9)        4 words

	MOVQ    MM6,    [EAX+24]  ;; mm6 = (..., a15, a14, a13)       3 words + fict
	PMADDWD MM4,    MM5       ;; mm4 = (a12*b12+a11*b11, a10*b10+a9*b9) 2 dwords
	MOVQ    MM7,    [EDX+24]  ;; mm7 = (..., b15, b14, b13)       3 words +fict
	PSLLQ 	MM6, 	16        ;; mm6 = (a15, a14, a13, 0)
	PADDD   MM0,    MM2
	PSLLQ	MM7, 	16        ;; mm7 = (b15, b14, b13, 0)
	PADDD   MM0,    MM4
	PMADDWD MM6,    MM7       ;; mm6 = (a15*b15+a14*b14, a13*b13) 2 dwords
	PADDD   MM0,    MM6       ;; mm0 = ( s1, s2 )                 2 dwords

	MOVQ    MM2,    MM0       ;; mm2 = ( s1, s2 )
	PSRLQ   MM2,    32        ;; mm2 = ( 0,  s1 )
	PADDD   MM0,    MM2       ;; mm0 = ( s1, s1+s2) = (..., sum )
	MOVD    EAX,    MM0  	  ;; eax = sum

	EMMS

    //RET
	}
#else
    return 0;
#endif
}




MMX_FUNC(uint32_t) MMX_scalar16_(uint32_t  _EAX, uint32_t _EDX)
{
#ifdef _MSC_VER
_asm{
    mov edx, _EDX

    MOVQ    MM0,    mm1       ;; mm0 = (a4, a3, a2, a1)           4 words
    MOVQ    MM2,    mm3       ;; mm2 = (a8, a7, a6, a5)           4 words
    PMADDWD MM0,    [EDX]     ;; mm0 = (a4*b4+a3*b3, a2*b2+a1*b1) 2 dwords

    MOVQ    MM4,    mm5       ;; mm4 = (a12, a11, a10, a9)        4 words
    PMADDWD MM2,    [EDX+8]   ;; mm2 = (a8*b8+a7*b7, a6*b6+a5*b5) 2 dwords

    MOVQ    MM6,    mm7       ;; mm6 = (..., a15, a14, a13)       3 words + fict
    PMADDWD MM4,    [EDX+16]  ;; mm4 = (a12*b12+a11*b11, a10*b10+a9*b9) 2 dwords
	PMADDWD MM6,    [EDX+24]  ;; mm6 = (a15*b15+a14*b14, a13*b13) 2 dwords
    nop
    PADDD   MM0,    MM2
	PADDD   MM0,    MM4
	PADDD   MM0,    MM6       ;; mm0 = ( s1, s2 )                 2 dwords

	MOVQ    MM2,    MM0       ;; mm2 = ( s1, s2 )
	PSRLQ   MM2,    32        ;; mm2 = ( 0,  s1 )
	PADDD   MM0,    MM2       ;; mm0 = ( s1, s1+s2) = (..., sum )
	MOVD    EAX,    MM0  	  ;; eax = sum

    //RET
    }
#else
    return 0;
#endif
}

MMX_FUNC(void) MMX_open_image(uint16_t *_EAX)
{
#ifdef _MSC_VER
_asm{
    mov eax, _EAX

	MOVQ    MM1,    [EAX]     ;; mm1 = (a4, a3, a2, a1)           4 words
	MOVQ    MM3,    [EAX+8]   ;; mm3 = (a8, a7, a6, a5)           4 words
	MOVQ    MM5,    [EAX+16]  ;; mm5 = (a12, a11, a10, a9)        4 words
	MOVQ    MM7,    [EAX+24]  ;; mm7 = (..., a15, a14, a13)       3 words + fict

    //RET
    }
#endif
}

MMX_FUNC(void) MMX_close_image(void)
{
#ifdef _MSC_VER
_asm{

    EMMS

    //RET
    }
#endif
}

MMX_FUNC(uint32_t) MMX_scalar_sq(uint16_t *_EAX,int32_t _ECX)
{
#ifdef _MSC_VER
_asm{
    mov		eax,	_EAX
	mov		ecx,	_ECX
    add     ecx,    7
	shr		ecx,	3		; align to 8 words
	PXOR	MM3,	MM3

cykl:
	MOVQ	MM1,	[EAX]
	MOVQ	MM7,	[EAX+8]
	PMADDWD	MM1,	MM1
	PMADDWD	MM7,	MM7
	PADDD	MM1,	MM7
	MOVQ	MM2,	MM1
	PSLLQ   MM1,    32
	add		eax,	16
	PSRLQ   MM2,    32

	PADDD	MM3,	MM2
	PSRLQ   MM1,    32
	dec		ecx
	PADDD	MM3,	MM1
	jne		cykl

	MOVQ	MM1,	MM3
	MOVD 	EAX,	MM3
	PSRLQ   MM1,    32
	MOVD 	EDX,	MM1
    add     eax,    edx

	EMMS

    }
#else
    return 0;
#endif
}

MMX_FUNC(void) MMX_scale( uint16_t *_EAX, int32_t _ECX, int32_t _EDX)
{
#ifdef _MSC_VER
_asm{
	mov		eax,	_EAX
	mov		ecx,	_ECX
	mov		edx,	_EDX
	shl		edx,	16
	or		edx,	_EDX
	shr		ecx,	2		; align to 4 words
	MOVD	MM0,	edx
	PSLLQ	MM0,	32
	MOVD 	MM1,	edx
	POR		MM0,	MM1

cykl:
	MOVQ	MM1,	[EAX]
	MOVQ	MM7,	[EAX+8]
	PMULLW	MM1,	MM0
	PMULLW	MM7,	MM0
	add		eax,	16
	sub		ecx,    2
	MOVQ	[EAX-16],	MM1
	MOVQ	[EAX-8],	MM7

	jne		cykl

	EMMS

	//RET
    }
#endif
}

MMX_FUNC(void) MMX_binarize_16x16(uint16_t *rin,uchar *rout)
{
#ifdef _MSC_VER
_asm{
    mov     eax,    rin

    mov     ecx,    001020408h
    movd    mm6,    ecx
    psllq   mm6,    32
    mov     ecx,    010204080h
    movd    mm0,    ecx
    por     mm6,    mm0         ; mm6 - mask of bites

    mov     ecx,    16*2
    mov     ebx,    rout
    pxor    mm7,    mm7


_loop:
    movq     mm0,    [eax]
    movq     mm1,    [eax+8]
    pcmpgtw  mm0,    mm7
    pcmpgtw  mm1,    mm7
    packsswb mm0,    mm1

    movq     mm1,    [eax+16]
    movq     mm2,    [eax+24]
    pcmpgtw  mm1,    mm7
    pcmpgtw  mm2,    mm7
    packsswb mm1,    mm2

    pand    mm0,    mm6
    pand    mm1,    mm6

    movq    mm2,    mm0
    movq    mm3,    mm1
    psllq   mm2,    16
    psllq   mm3,    16
    por     mm0,    mm2
    por     mm1,    mm3

    movq    mm2,    mm0
    movq    mm3,    mm1
    psllq   mm2,    32
    psllq   mm3,    32
    por     mm0,    mm2
    por     mm1,    mm3

    movq    mm2,    mm0
    movq    mm3,    mm1
    psllq   mm2,    8
    psllq   mm3,    8
    por     mm0,    mm2
    por     mm1,    mm3


    psrlq   mm0,    56
    psrlq   mm1,    56
    psllq   mm1,    8
    por     mm0,    mm1
    add     eax,    32
    dec     ecx
    movq    [ebx],  mm0
    add     ebx,    8

    dec     ecx
    jnz     _loop

    EMMS
    }
#endif
}
