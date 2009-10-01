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
CPUID0 MACRO
        DB      0FH             ;; Opcode for CPUID instruction
        DB      0A2H
        ENDM
        */
#define CPUID0 _asm _emit 0fh _asm _emit 0a2h

#define FAMILY_MASK   0f00h
#define FAMILY_SHIFT  8




//;;
//;;  return typ of CPU : 386, 486, 586, 686, 5860 (586+IA MMX),
//;;                       6860 (686+IA MMX)
//;;
int get_CPU(void)
{
#ifdef _MSC_VER
short int retu;
_asm{
;
; cmp 386 & 486 &586 : study bit 18 in flag register EFLAGS. If we can
; chandged this bit, then used i486. If can't - i386.
;
; save stack pointer
      mov             edx     ,       esp
; align stack pointer for disable during sexception etting flag AC
      and             esp     ,       not 3
; copy registr EFLAGS to EAX
      pushfd
      pop             eax
; saving start state  EFLAGS
      mov             ecx     ,       eax
;	switch flag AC
      xor             eax     ,       40000H
; write changed value to EFLAGS
      push    eax
      popfd
; copy registr EFLAGS to EAX
      pushfd
      pop             eax
;	compare old and new values of bit AC
      xor             eax     ,       ecx
      shr             eax     ,       18
      and             eax     ,       1
      push    ecx
;	restore registr EFLAGS
      popfd
;	restore stack pointer
      mov             esp     ,       edx
; AX=0 for i386; AX=1 for i486
      mov             dx      ,       386
      test    ax      ,       ax
      mov         cx      , 1
      jz              cpu_end
	  mov		dx	,	486
      }

_asm{
;
; cmp 486 & CPUID : study bit 21 in flag register EFLAGS. If we can
; chandged this bit, then used Intel_with_CPUID. If can't - i486.
;
; copy registr EFLAGS to EAX
	pushfd
	pop		eax
; saving start state  EFLAGS
	mov		ecx	,	eax
;	switch flag AC
	xor		eax	,	200000H
; write changed value to EFLAGS
	push	eax
	popfd
; copy registr EFLAGS to EAX
	pushfd
	pop		eax
;	compare old and new values of bit AC
	xor		eax	,	ecx
	shr		eax	,	21
	and		eax	,	1
	push	ecx
;	restore registr EFLAGS
	popfd
	test	ax	,	ax
	mov		cx	, 1
	jz		cpu_end
    }

_asm{
;
; CPUID studies
;
	mov		ax	,	0
        CPUID0
	cmp		ebx,	0756E6547h ;"Genu"
	jne		NO_GENUININTEL
	cmp		edx,	049656E69h ;"ineI"
	jne		NO_GENUININTEL
    cmp		ecx,	06C65746Eh ;"ntel"
	jne		NO_GENUININTEL
	jmp		GENUININTEL
    }
NO_GENUININTEL:
_asm{
	mov		cx	,	1
	mov		dx	,	486
	jmp		cpu_end
    }
GENUININTEL:
_asm{
	mov		eax	,	1
    CPUID0
	test	edx	,	00800000h
	mov		cx	,	1
	jz		MMX_SKIP
	mov		cx	,	10  ; scaling
    }
MMX_SKIP :
_asm{
	test	edx	,	02000000h
	mov		bx	,	1
	jz		XMM_SKIP
	mov		bx	,	10  ; scaling
    }
XMM_SKIP :
_asm{
	mov		dx	,	486
	and		ax	,   FAMILY_MASK
	shr		ax	,	FAMILY_SHIFT
	cmp		ax	,	5
	jl		cpu_end
	je		cpu_586
	mov		dx  , 686
    mov     cx  , 1   ; Klamath, Detushes - not opt, Katmai - future
	jmp		cpu_end
    }
cpu_586:
_asm{
	mov		dx	,	586
    }
cpu_end:
_asm{
	mov		ax	,	dx
	mov		dx	,	0
	mul		cx
    mov     retu,    ax
    }
return retu;
#else
return 386;
#endif
}

int get_CPUID_EDX_BIT(int mask)
{
#ifdef _MSC_VER
int retu=0;
_asm{
;
; CPUID studies
;
	mov		eax	,	0
    CPUID0
	cmp		ebx,	0756E6547h ;"Genu"
    mov     ebx,    0
	jne		END
	cmp		edx,	049656E69h ;"ineI"
	jne		END
    cmp		ecx,	06C65746Eh ;"ntel"

	jne		END
	mov		eax	,	1
    CPUID0
    mov     ecx,    mask
	test	edx	,	ecx
	jz		END
	mov		ebx	,	1  ; mask & EDX  != 0
    }
END :
_asm{
	mov     retu, ebx
    }
    return retu;
#else
    return 0;
#endif
}

int get_MMX(void)
{
	return get_CPUID_EDX_BIT(0x800000);

}

int get_XMM(void)
{
	return get_CPUID_EDX_BIT(0x2000000);

}
