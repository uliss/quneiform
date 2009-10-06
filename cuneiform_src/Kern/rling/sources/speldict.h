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

/* ------------------------------------------------------------------ *\
|* 03-18-93 02:42pm, Mike                                             *|
 |*                                                                    *|
 |*             Spelling dictionary project. Release 2.0               *|
 |*                         64-unit alphabet.                          *|
 |*                                                                    *|
 \* ------------------------------------------------------------------ */

#ifndef __SPELLDICT_H
#define __SPELLDICT_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
/* ----------------------------------------------------------------- *
 *  File contents:                                                    *
 *                                                                    *
 *    The first generation of the internal structures of the static   *
 *  dictionary.                                                       *
 *                                                                    *
 * ------------------------------------------------------------------ *
 *                                                                    *
 *  List of types:                                                    *
 *                                                                    *
 *    TVertP                                                          *
 *    TVertV                                                          *
 *    TPostfics                                                       *
 *    TAccount                                                        *
 *    TAddr                                                           *
 *    TTailVar                                                        *
 *    TShiftType                                                      *
 *    TTailSet                                                        *
 *    TDictState;                                                     *
 *    TBuildInfo;                                                     *
 *                                                                    *
 * ------------------------------------------------------------------ */

/* --- Dictionary constants. ---------------------------------------- */

#define STAT_DICT_SIGN  "CTCDict"

#ifdef S_COMMENTS
#if sizeof( STAT_DICT_SIGN ) > 8
#error Check header size for static dictionary.
#endif
#endif

#define ABCSIZE  66             /* size of alfabet <= 64          ??? */
#define POS_ROOT 0x07           /* mask on 3 first buits in VERTV ??? */

#define MAX_WORD_SIZE 32        /* Maximal lth of word <-> tree depth */
#define MAXLEVEL      2         /* Depth of positional subtree        */

/* --- Key type for search in the dictionary. ----------------------- */

#define KEYTYPE uchar
#define NATURAL uint16_t

/* --- Positional tree. --------------------------------------------- */

typedef struct {
	uint16_t cont :1; /*  20 bits - addr of continuation       */
	uint16_t noterm :1; /*  vert isn't terminal in tree          */
	uint16_t exist :1; /*  vertex exists                        */
	uint16_t shift0 :5; /*  first 5 bits & next 8 + 8 bits       */
	uint16_t shift1 :8; /*    in 21 bits relative address        */
	uint16_t shift2 :8; /*                                       */
} TVertP;

typedef TVertP * PTVertP;

/* --- Pointer tree. ------------------------------------------------ */

typedef struct {
	uint16_t cont :1; /*  if == 1 then this vertex is          */
	/*  followed by the continuation         */
	uint16_t noterm :1; /*  vert isn't terminal in tree          */
	uint16_t key :6; /*  letter no.: 0--63                    */
} TVertV;

typedef TVertV * PTVertV;

/* --- Continuations of vertex. ------------------------------------- */

typedef struct {
	uint16_t cont :1; /*  continuation exists                  */
	uint16_t tail :1; /*  =1 (the cont is for tail/accnt)      */
	uint16_t accnt :1; /*  =0 - tail, =1 - accnt                */
	uint16_t enter0 :5; /*  tail kind number - first 5 bits      */
	uint16_t enter1 :8; /*  tail kind number - next  8 bits      */
} TPostfics;

typedef TPostfics * PTPostfics;

typedef struct {
	uint16_t cont :1; /*  continuation exists                  */
	uint16_t tail :1; /*  =1 (the cont is for tail/accnt)      */
	uint16_t accnt :1; /*  =0 - tail, =1 - accnt                */
	uint16_t wrdterm :1; /*  if == 1 - vertex may be end of word  */
	uint16_t spare :1; /*  spare                                */
	uint16_t freq :3; /*  frequency of word occurence          */
} TAccount;

typedef TAccount * PTAccount;

typedef struct {
	uint16_t cont :1; /*  continuation exists                  */
	uint16_t tail :1; /*  = 0 (the cont is for reference)      */
	uint16_t lth :1; /*  lth of shift (bytes): 0->1, 1->2/3   */
	uint16_t shift0 :5; /*  first 5 bits or 1-byte shift         */
	uint16_t lth2 :1; /*  lth of long shift: = 0 - 2 bytes     */
	uint16_t shift2 :7; /*  next 7 bits of shift (lth=1,lth2=0)  */
	uint16_t shift3 :8; /*  next 8 bits of shift (lth=1,lth2=1)  */
} TAddr;

typedef TAddr * PTAddr;

/* --- Tail string unit structure. ---------------------------------- */

typedef struct {
	uint16_t ch :6; /* character of the tail                  */
	uint16_t :1; /* not used                               */
	uint16_t tailend :1; /* flag of end of the tail                */
} TTailSet;

typedef TTailSet * PTTailSet;

/* --- Tail rules. -------------------------------------------------- */

typedef struct {
	uint16_t tablenum; /* tail table number                      */
	uint16_t maxtaillth; /* max lth of tail in the table           */
	uint32_t tailmask; /* mask of tail set variant               */
} TTailVar;

typedef TTailVar * PTTailVar;

typedef uint32_t TShiftType;
typedef TShiftType * PTShiftType;

/* --- Static dictionary control structure type. -------------------- */

struct dict_state { /* declaration for compatibility... */
	uchar * root; /* pointer to dictionary tree       */
	uchar * tailset_root; /* pointer to tail set allocation   */
	PTTailVar vartable; /* table of "tail rules"            */
	PTShiftType table; /* table of shifts to "tail strings"*/
	uint32_t size; /* size of used memory              */
	uint16_t abcSize; /* size of alphabet                 */
};

typedef struct dict_state TDictState;
typedef TDictState * PTDictState;

/* ------ Static dictionary header mask stucture. ------------------- */

typedef struct {

	char sign[8]; /* "CTCDict"                        */
	char cpuType[8]; /* "Mac" or "Intel"                 */
	char language[8]; /* "FRA"/"RUS"/"ENG"/etc.           */
	char version[8]; /* "01.00"                          */
	char treeLength[8]; /* RADIX 10 - in bytes              */
	char tailsLength[8]; /* RADIX 10 - in bytes              */
	char rulesLength[8]; /* RADIX 10 - in bytes              */
	char hushLength[8]; /* RADIX 10 - in bytes              */
	char abcSize[8]; /* size of the current alphabet     */
	char abcUpper[64]; /* string of uppercase letters      */
	char abcLower[64]; /* string of lowercase letters      */

} TDictHeaderMask;

typedef TDictHeaderMask * PTDictHeaderMask;

/* ------ Dictionary builder information stucture. ------------------ */

typedef struct { /* -- information about vertex --- */
	uint16_t key; /* key of vertex                   */
	uint16_t levNum; /* absolute level number           */
	uint16_t wordTerm; /* vertex is terminal in a word    */
	uint16_t treeTerm; /* vertex is terminal in the tree  */
	uint16_t levTerm; /* vertex is most right in level   */
	uint16_t tailsNum; /* number of vertex's tails        */
	uint16_t refLth; /* length of reference to brother  */
	uint16_t sonsNum; /* number of vertex's sons         */
} TBuildInfo;

typedef TBuildInfo * PTBuildInfo;

/* ------------------------------------------------------------------ */
#endif
