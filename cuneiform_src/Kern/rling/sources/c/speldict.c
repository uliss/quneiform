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

/**********************************************************************/
/* DICTIONARY access: search in dictionary.                           */
/**********************************************************************/
/**********************************************************************
 -- Updation history. --
 03-19-93 00:57am, Mike
 New ( 6-bit ) dictionary format.
 04-09-93 05:33pm, Mike
 Module taken from Lepik's new version of spelling checker.
 ***********************************************************************/

#include "spelmode.h"

#if defined(TURBO_C)
#include "tc_types.h"
#elif defined(WATCOM) || defined(THINK_C_5)
#include "spelwatc.h"
#elif defined(THINK_C)
#include "spelthin.h"
#else
#error   NO TOOOL SPECIFIED
#endif

#include "speldefs.h"
#include "spelfunc.h"
#include "tigeremulate.h"

/* ------ Export section. ------------------------------------------- */
/* -- Code -- */

INT search(KEYTYPE *word, INT *wordsize, LTIMG * wrddef[],
		struct dict_state * dict);

/* -- Data -- */
extern BYTE alphabet[2][ABCSIZE];
extern KEYTYPE codetable[256];

#ifdef S_COMMENTS

/* 03-22-93 10:51pm, Mike                 */
/* 03-22-93 10:52pm, Mike -- See above... */
BYTE alphabet[][ABCSIZE]= {
	{	'\'','A','.','Ћ','Џ','.','.','’','B','C','Ђ','D','E','.','.',
		'.','ђ','F','G','H','I','.','.','.','.','J','K','L','M','N',
		'Ґ','O','.','™','.','.','P','Q','R','S','T','U','љ','.','.',
		'.','V','W','X','Y','.','Z'},
	{	'\'','a','ѓ','„','†','…',' ','‘','b','c','‡','d','e','€','Љ',
		'‰','‚','f','g','h','i','‹','Њ','Ќ','Ў','j','k','l','m','n',
		'¤','o','“','”','•','ў','p','q','r','s','t','u','Ѓ','–','—',
		'Ј','v','w','x','y','�','z'}
};

KEYTYPE codetable[256];

#endif
/*************************************************************************/
/*                Locals section.                                        */
/*************************************************************************/
/* -- Code -- */

static INT
		analyse(INT * account, INT * wordsize, LTIMG * wrddef[], INT * found);
static INT next_level(KEYTYPE ch);
static INT test_tail(BYTE * ptr, INT * accounter, INT * tailmaxl,
		LTIMG * wrddef[], INT * tailfound);

static TShiftType brother2(BYTE * ptr);

static INT comp_tail(INT varnum, INT rest, INT cnt, KEYTYPE * wptr,
		INT * tailmaxl, LTIMG * wrddef[], INT * tailfound);
static INT fillgap(KEYTYPE * word, INT cnt, LTIMG * wrddef[], INT gapcont);
static INT tailout(INT cnt, INT rest, KEYTYPE * wptr, LTIMG * wrddef[],
		INT * tailfound, BYTE *tailptr);
static INT chkfill(KEYTYPE * word, INT cnt, LTIMG * wrddef[], INT gapcont,
		INT * fillfind, BYTE symb, LT ** gaplt);

/* -- Macro -- */

#define HAVE_BLANK 2

/* -- Data -- */

static PTDictState dictInfo; /* dictionary control structure  */

static KEYTYPE *wptr; /* pointer to working word       */
static INT rest; /* lth of word rest              */
static INT cnt; /* character counter             */
static TShiftType lthorshift; /* lth of curr VERTV or shift to */

static INT poslevel; /* current level in pos. tree if indpos==1  */
static INT indpos; /* 1-ndptr points to pos. tree,0-otherwise  */
static INT maxlevel; /* max num of level in current pos. tree    */

static BYTE *ndptr; /* pointer to current vertex in  */

/*************************************************************************/
/*              Macros & constants insted of Bit Fields.                 */
/*************************************************************************/
/* -- Pointer tree node head fields. -------------------------------- */

#define VERTV_SIZE      1
#define VERTV_CONT(p)   ((WORD)((*(p))&0x01))       /* Boolean */
#define VERTV_NOTERM(p) ((WORD)((*(p))&0x02))       /* Boolean */
#define VERTV_KEY(p)    ((CHAR)((*(p)) >> 2))       /* CHAR */

/* -- Positional tree node fields. ---------------------------------- */

#define VERTP_SIZE      3
#define VERTP_CONT      VERTV_CONT                  /* Boolean */
#define VERTP_NOTERM    VERTV_NOTERM                /* Boolean */
#define VERTP_EXIST(p)  ((WORD)((*(p))&0x04))       /* Boolean */
#define VERTP_SHIFT0(p) ((WORD)((*(p)) >> 3))       /* WORD */
#define VERTP_SHIFT1(p) ((WORD)(*(p+1)))            /* WORD */
#define VERTP_SHIFT2(p) ((WORD)(*(p+2)))            /* WORD */

/* -- Pointer tree node postfics fields. ---------------------------- */

#define POSTFICS_SIZE       2
#define POSTFICS_CONT       VERTV_CONT              /* Boolean */
#define POSTFICS_TAIL(p)    ((WORD)((*(p))&0x02))   /* Boolean */
#define POSTFICS_ACCNT(p)   ((WORD)((*(p))&0x04))   /* Boolean */
#define POSTFICS_ENTER0(p)  ((WORD)((*(p)) >> 3))   /* WORD    */
#define POSTFICS_ENTER1(p)  ((WORD)(*(p+1)))        /* WORD    */

/* -- Pointer tree node account fields. ----------------------------- */

#define ACCOUNT_SIZE        1
#define ACCOUNT_CONT        VERTV_CONT              /* Boolean */
#define ACCOUNT_TAIL        POSTFICS_TAIL           /* Boolean */
#define ACCOUNT_ACCNT       POSTFICS_ACCNT          /* Boolean */
#define ACCOUNT_WRDTERM(p)  ((WORD)((*(p))&0x08))   /* Boolean */
#define ACCOUNT_FREQ(p)     ((WORD)((*(p)) >> 5))   /* WORD */

/* -- Pointer tree relative shift to the next node in level. -------- */

/* ADDR_SIZE not a valid value!!!                              */
/* ADDR may be interpreted differently in the current context. */

#define ADDR_CONT      VERTV_CONT                   /* Boolean */
#define ADDR_TAIL      POSTFICS_TAIL                /* Boolean */
#define ADDR_LTH(p)    ((WORD)((*(p))&0x04))        /* Boolean */
#define ADDR_SHIFT0(p) ((WORD)((*(p)) >> 3))        /* WORD */
#define ADDR_LTH2(p)   ((WORD)((*((p)+1))&0x01))    /* WORD */
#define ADDR_SHIFT2(p) ((WORD)((*((p)+1)) >> 1))    /* WORD */
#define ADDR_SHIFT3(p) ((WORD)(*((p)+2)))           /* WORD */

/* -- Tailset element fields. --------------------------------------- */

#define TAILSET_SIZE 1
#define TAILSET_CH(p)       ((CHAR)((*(p))&0x3F))   /* CHAR */
#define TAILSET_TAILEND(p)  ((WORD)((*(p))&0x80))   /* Boolean */

/*************************************************************************/
/*                                                                       */
/*    Function search() sifts input word through the dictionary.         */
/*                                                                       */
/*************************************************************************/

INT search(KEYTYPE *word, INT *wordsize, LTIMG * wrddef[],
		struct dict_state * dict) {
	INT found = 0;
	INT state; /* state of search in dictionary */
	INT account = 0; /* word statistic                */
	INT full_size = *wordsize; /* ATAL, save word size          */

	dictInfo = dict; /* initial values of prossess    */
	ndptr = dict->root;
	indpos = 1;
	wptr = word;
	poslevel = 0;
	maxlevel = VERTV_KEY(ndptr);

	for (cnt = 0, rest = *wordsize; rest >= 0; ++cnt, --rest) {
		if (wrddef[cnt]->blank == 1) {
			if (!fillgap(word, cnt, wrddef, found))
				if (!found) {
					*wordsize = cnt - 1;
					goto Fail;
				} else
					goto Success;
		}
		if (!next_level(word[cnt])) {
			if (!found) {
				*wordsize = cnt;
				goto Fail;
			} else
				goto Success;
		}
		state = analyse(&account, wordsize, wrddef, &found);
		if (state != 0) { /* -- word found or break is in need -- */
			if (((state == 1) && (!found)) || state == 2)
				break;
		}
	}

	if (state == 1 || found)
		goto Success;
	else
		goto Fail;

	Fail: return FAIL; /* <==> return (0)        */

	Success: while (full_size >= 0) {
		wrddef[full_size--]->blank = 0;
	}
	return account; /* return word statistic  */

}

/*************************************************************************/
/*  The function next_level() cnanges the global pointer *ndptr so that  */
/*  *ndptr pointing to vertex of next level of dictionary tree with the  */
/*  key equals functiuon's parameter ch. If the vertex exists, function  */
/*  returns 1, otherwise 0 is returned.                                  */
/*************************************************************************/

INT next_level(KEYTYPE ch) {
	TShiftType shift;

	Begin: if (indpos)
		if (poslevel != maxlevel) {
			if (maxlevel > 2) {
				register WORD i;
				for (i = maxlevel - poslevel - 1, shift = 1L; i > 0; --i) {
					shift *= dictInfo->abcSize;
					shift++;
				}
			} else if (poslevel == 0) {
				shift = dictInfo->abcSize + 1L;
				ndptr -= 2;
			} else {
				shift = 1L;
			}

			ndptr += (ch * shift + 1) * VERTP_SIZE;
			poslevel++;

			if (VERTP_EXIST(ndptr) != 0)
				goto Success;
			else
				goto Fail;
		} else {
			ndptr += lthorshift;
			indpos = 0;
			lthorshift = 0L;
			goto Begin;
		}
	else { /* -- if ( ! indpos ) -> pointer tree */
		ndptr += lthorshift;

		while (VERTV_KEY(ndptr) < ch) {
			shift = brother2(ndptr);
			if (shift == 0L)
				goto Fail;
			/* it was the last brother ? */
			ndptr += shift;
		}

		if (VERTV_KEY(ndptr) == ch)
			goto Success;
		else
			goto Fail;
	}

	Success: return OK;

	Fail: return FAIL;

}

/**************************************************************************/
/*       Function brother2() returns the value of shift to next (right)   */
/* brother.If VERTV.lth==0 && VERTV.cont==0 , then function returns zero. */
/* If VERTV.lth==1 && shift==0L (vertex is terminal in a word) , then     */
/* function cntinues searching to next(right) brother.                    */
/**************************************************************************/

TShiftType brother2(BYTE * ptr) {
	TShiftType shift = 0;

	if (VERTV_CONT(ptr) == 0) {
		return 0; /* continuation absent  */
	}

	ptr += VERTV_SIZE; /* skip vertex itself */

	while (POSTFICS_TAIL(ptr) != 0 /* skip account and postfics */
	&& POSTFICS_CONT(ptr) != 0) {
		if (POSTFICS_ACCNT(ptr) != 0)
			ptr += ACCOUNT_SIZE;
		else
			ptr += POSTFICS_SIZE;
	}

	if (ADDR_TAIL(ptr) == 0) {
		shift = ADDR_SHIFT0(ptr);
		if (ADDR_LTH(ptr) != 0) {
			shift <<= 7;
			shift += ADDR_SHIFT2 ( ptr );
			if (ADDR_LTH2(ptr) != 0) {
				shift <<= 8;
				shift += ADDR_SHIFT3 ( ptr );
			}
		}
	}

	return shift;
}

/*************************************************************************/
/*    Function analyse() calculates pointer *ptr : *ptr pointing to      */
/*  vertex continuation,test tails and test the status of vertex -       */
/*  terminal in tree or not.                                             */
/*************************************************************************/

INT analyse(INT * account, INT * wordsize, LTIMG ** wrddef, INT * found) {
	INT tailmaxl = 0;
	BYTE * ptr; /* serv pointer: points to vertex cont. */

	if (indpos)
		lthorshift = ((TShiftType) VERTP_SHIFT0(ndptr) << 16)
				+ ((TShiftType) VERTP_SHIFT1(ndptr) << 8) + VERTP_SHIFT2(ndptr);
	else
		lthorshift = 1; /*lth of VERTV vertex */

	ptr = ndptr + lthorshift;

	if (VERTV_CONT(ndptr) != 0 && rest >= 0) {
		INT ret = test_tail(ptr, account, &tailmaxl, wrddef, found);
		if (ret && !*found) {
			goto Success;
		}
	}

	if (rest == 0) { /* test the end of word, if current letter  */
		goto Fail;
		/* is the last letter in the word.          */
	} else { /* the current letter isn't the last.       */
		if (VERTV_NOTERM(ndptr) != 0) {
			goto GoNext;
		} else {
			if (*found)
				goto Fail;
			else
				goto Failcnt;
		}
	}

	GoNext: return 0; /* go to the next vertex in tree         */
	Success: return 1; /* the word is in dictionary             */
	Failcnt: if (!(VERTV_CONT(ndptr) != 0 && rest >= 0))
		*wordsize = cnt; /* continue searching impossible         */
	else
		*wordsize = cnt + tailmaxl + 1;
	/* Test variant : cnt points for the current letter.
	 We test tails hence we are working with the next letter:
	 1 is in need.
	 */
	Fail: return 2;
}

/**********************************************************************/
/*       Function test_tail(void *ptr) analize element of vertex      */
/*    continuation, calculates length of vertex in global variable    */
/* lthorshift and calls function comp_tail(),if element is tail       */
/*                            variant.                                */
/*       The order of potfics takes a great importance:               */
/*               ->  LONGADDR,ACCOUNT,POSTFICS !                      */
/**********************************************************************/

INT test_tail(BYTE * ptr, INT * accounter, INT * tailmaxl, LTIMG * wrddef[],
		INT * tailfound) {
	INT contflag = 0;
	INT tailscnter = 0;
	INT accntcnter = 0;
	uint32_t wCount = 0;

	do {
		contflag = POSTFICS_CONT(ptr);
		if (POSTFICS_TAIL(ptr)) {
			if (!POSTFICS_ACCNT(ptr)) {
				if (rest != 0) {
					/* -- don't check tails if no rest. */
					WORD enterNum;
					enterNum = (POSTFICS_ENTER0(ptr) << 8)
							+ POSTFICS_ENTER1(ptr);

					if (comp_tail(enterNum, rest, cnt, wptr, tailmaxl, wrddef,
							tailfound)) {
						goto Success;
					}
				}
				tailscnter++;
				ptr += POSTFICS_SIZE;
			} else {
				if (!*tailfound) {
					*accounter = ACCOUNT_FREQ(ptr);
					if (rest == 0 && ACCOUNT_WRDTERM(ptr) != 0) {
						goto Success;
					}
				}
				accntcnter++;
				ptr += ACCOUNT_SIZE;
			}
		} else {
			if (ADDR_LTH(ptr) == 0) {
				lthorshift += 1;
				ptr += 1;
			} else {
				if (ADDR_LTH2(ptr) == 0) {
					lthorshift += 2;
					ptr += 2;
				} else {
					lthorshift += 3;
					ptr += 3;
				}
			}
		}
		/////////
		wCount++;
		/////////
	} while (contflag);

	lthorshift += tailscnter * POSTFICS_SIZE + accntcnter * ACCOUNT_SIZE;

	if (*tailfound == 0) {
		return FAIL;
	}

	Success: return OK;
}

/*************************************************************************/
/*    Function comp_tail() compares the end of word pointed *wptr with   */
/* the tail variant with the number varnum.                              */
/*************************************************************************/

#define MASK 0x1L

INT comp_tail(INT varnum, INT rest, INT cnt, KEYTYPE * wptr, INT * tailmaxl,
		LTIMG * wrddef[], INT * tailfound) {
	register INT j, k;
	BYTE * ptr;
	BYTE * tailptr;
	uint32_t mask;

	if (dictInfo->vartable[varnum].maxtaillth < rest) {
		return FAIL;
	}

	ptr = dictInfo->tailset_root
			+ dictInfo->table[dictInfo->vartable[varnum].tablenum];
	mask = dictInfo->vartable[varnum].tailmask;
	for (; mask != 0; mask >>= 1) {
		if ((MASK & mask) != 0) {
			tailptr = ptr, j = cnt + 1, k = rest - 1;
			do {
				if (wrddef[j]->blank != 1) {
					if (wptr[j] != TAILSET_CH(ptr)) {
						if (*tailmaxl <= j - cnt - 2) {
							*tailmaxl = j - cnt - 1;
						}
						break;
					}
				}
				/*-------------------------------------------------------------------------*/
				if (TAILSET_TAILEND(ptr) && k == 0) { /* end of word & tail */
					if (tailout(cnt, rest, wptr, wrddef, tailfound, tailptr)
							!= HAVE_BLANK) {
						return OK;
					}
				}
				if (TAILSET_TAILEND(ptr)) {
					break;
				}
				/*------------------------------------------------------------------------*/
				j++, k--, ptr++;
			} while (k >= 0);
		}

		while (TAILSET_TAILEND(ptr) == 0)
			ptr++;
		ptr++;
	}

	return FAIL;
}

/*************************************************************************/
/*                                                                       */
/*************************************************************************/

INT fillgap(KEYTYPE * word, INT cnt, LTIMG * wrddef[], INT gapcont) {

	TShiftType shift;

	LT * gaplt;
	INT xindpos;
	INT xposlevel;
	INT xmaxlevel;
	TShiftType xlthorshift;
	BYTE * xndptr;

	INT fillfind = 0;

	xindpos = indpos;
	xposlevel = poslevel;
	xmaxlevel = maxlevel;
	xndptr = ndptr;
	xlthorshift = lthorshift;

	if (gapcont == 0) { /* fill gap from the beginning  */
		wrddef[cnt]->lt = (LT *) &wrddef[cnt]->std;
		gaplt = wrddef[cnt]->lt;
	} else { /* continue filling             */
		gaplt = wrddef[cnt]->lt;
		while (!(gaplt->attr & EOLIST)) {
			++gaplt;
		}
		++gaplt;
	}

	Begin:

	if (xindpos) {
		if (xposlevel != xmaxlevel) {

			register INT i;
			WORD abcSize = dictInfo->abcSize;
			;

			if (xmaxlevel > 2) {
				for (i = xmaxlevel - xposlevel - 1, shift = 1L; i > 0; --i) {
					shift *= abcSize;
					++shift;
				}
			} else if (xposlevel == 0) {
				shift = abcSize + 1;
				xndptr -= 2;
			} else {
				shift = 1L;
			}

			xndptr += VERTP_SIZE;
			for (i = 0; i < abcSize; ++i) {
				if (VERTP_EXIST(xndptr) != 0) {
					chkfill(word, cnt, wrddef, gapcont, &fillfind,
							alphabet[0][i], &gaplt);
				}
				xndptr += shift * VERTP_SIZE;
			}

			if (fillfind != 0)
				goto Success;
			else
				goto Fail;
		} else {
			xndptr += xlthorshift;
			xindpos = 0;
			xlthorshift = 0L;
			goto Begin;
		}
	} else { /* -- if ( !xindpos ) -> pointer tree */
		xndptr += xlthorshift;

		chkfill(word, cnt, wrddef, gapcont, &fillfind,
				alphabet[0][VERTV_KEY(xndptr)], &gaplt);
		while ((shift = brother2(xndptr)) != 0) {
			xndptr += shift;
			chkfill(word, cnt, wrddef, gapcont, &fillfind,
					alphabet[0][VERTV_KEY(xndptr)], &gaplt);
		}

		if (fillfind > 0)
			goto Success;
		else
			goto Fail;

	} /* end if (xindpos) */

	Success: wrddef[cnt]->blank = 0;
	return OK;

	Fail: if (gapcont != 0) {
		wrddef[cnt]->blank = 0;
		return OK;
	} else {
		return FAIL;
	}
}

/*************************************************************************/
/*                                                                       */
/*************************************************************************/

INT chkfill(KEYTYPE * word, INT cnt, LTIMG * wrddef[], INT gapcont,
		INT * fillfind, BYTE symb, LT ** gaplt) {
	LT * xxgaplt;

	if (gapcont == 0) {
		goto Gapadd;
		/* fill gap from the beginning */
	}

	xxgaplt = (LT *) &wrddef[cnt]->std;
	while (xxgaplt != *gaplt) {
		if (xxgaplt->code == symb) {
			break;
		}
		xxgaplt++;
	}

	if (xxgaplt == *gaplt) { /* symb not found in the available list */
		goto Gapadd;
	} else {
		goto CheckFirstFind;
	}

	Gapadd: if ((*gaplt) != (LT *) &wrddef[cnt]->std) {
		((*gaplt) - 1)->attr = 0;
	}
	(*gaplt)->code = symb;
	(*gaplt)->attr = 1;
	++(*gaplt);

	CheckFirstFind: if (*fillfind == 0) { /* 1-st find */
		if (gapcont == 0) {
			word[cnt] = codetable[symb];
		}
		*fillfind = 1;
	}

	return OK;
}

/*************************************************************************/
/*             Function fills blank positions from the tail.             */
/*************************************************************************/

INT tailout(INT cnt, INT rest, KEYTYPE * wptr, LTIMG * wrddef[],
		INT * tailfound, BYTE *tailptr) {

	BYTE * xxptr;
	INT xj;
	LT * xxgaplt = NULL;
	INT bl_flag = 0;
	INT last_bl = 0;

	xj = cnt + 1;
	xxptr = tailptr;
	do { /* loop to fill all wrddef, if blank  */
		if (wrddef[xj]->blank == 1) {
			bl_flag = 1;
			if (*tailfound == 0) {
				xxgaplt = (LT *) &wrddef[xj]->std;
				/* ---------------------------------------------------------- */
				wrddef[xj]->lt = xxgaplt;
				xxgaplt->attr = 1;
				xxgaplt->code = alphabet[0][TAILSET_CH(xxptr)];
				wptr[xj] = TAILSET_CH(xxptr); /* output ch instead of BLANK */
			} else if (xxgaplt == NULL || (xxgaplt && last_bl != -1 && (xxgaplt
					== (LT *) &wrddef[last_bl]->std))) {
				xxgaplt = (LT *) &wrddef[xj]->std;
				xxgaplt--;
				do {
					xxgaplt++;
					if (xxgaplt->code == alphabet[0][TAILSET_CH(xxptr)]) {
						break;
					}
				} while (!(xxgaplt->attr & EOLIST));
				if (xxgaplt->code != alphabet[0][TAILSET_CH(xxptr)]) {
					(xxgaplt)->attr = 0;
					(++xxgaplt)->attr = 1;
					(xxgaplt)->code = alphabet[0][TAILSET_CH(xxptr)];
				}
			}
			/* ------------------------------------------------------------ */
			else {
				last_bl = -1;
			}
			if (last_bl != -1) {
				last_bl = xj;
			}
		}

		++xj, ++xxptr;
	} while ((--rest) != 0);

	if (bl_flag) {
		*tailfound = 1;
	}

	if (!bl_flag)
		return OK;
	else
		return HAVE_BLANK;

}
/*************************************************************************/
/*                                                                       */
/*************************************************************************/
