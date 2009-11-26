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
 spellchk.c - object-parts-words check
 */

#include "spelmode.h"

#if defined(TURBO_C)
#include "tc_types.h"
#elif defined(WATCOM)
#include "spelwatc.h"
#else
#error   NO TOOOL SPECIFIED
#endif

#include <stdlib.h>
#include <ctype.h>

#include "speldefs.h"
#include "spelfunc.h"
#include "tigeremulate.h"

#define RASTER_SIZE 128*64/8

uchar raster[RASTER_SIZE];

#ifdef RECOGTEST
extern int16_t show;
extern int16_t begshow;
#endif

extern dQ SPQ; //Q;
extern int16_t back_flag; /*   ACT !!! see tg_spell */

struct tre_state tre_st;
struct tif_state tif_st;

/*=================================================================== */
/*                  Local  functions  definition                      */
/*=================================================================== */
static int16_t setwrd_changed(SOBJ * obj, LTIMG ** wrddef, SWORD * word);
static int16_t selectobj_best(SOBJ * obj, SPART part[], int16_t cur_part[],
                              int16_t * entire);
static int16_t compwrd(SOBJ * obj, SWORD * w1, SWORD * w2);

#ifdef V_RECOG
static int16_t form_raster (SOBJ *, SPOS *);
static int16_t back_recog (SOBJ *, SWORD *, int16_t, int16_t, int16_t * );
static void set_last_symb(struct segm *, char *);
static void set_tif_strip(struct segm *, char *, int16_t *, int16_t *);
static int16_t fill_raster();
#else
#define back_recog(a,s,c,v,b) ((a=a),0)
#endif

/***********************************************************************/
/* This procedure checks initial state of object just read from ED-file
 On the basis of the general info the decision is being made
 whether the object is worth or not for consideration
 */
/***********************************************************************/
int16_t checkobj(SOBJ * obj)

{
    int16_t val = 0, ppn = 0, lth = 0;
    ppn = obj->pos_part_nmb;
    lth = obj->pos_part[ppn];
    val = (((obj->blank_nmb * RMobj_blank_pen) + ((ppn - 1) * RMobj_spsp_pen))
           / lth);

    if ((obj->blank_nmb > RMobj_max_blank) || (lth < RMobj_min_lth) || (val
                                                                        > RMobj_no)) {
#ifdef OBJPR
#ifdef OBJPR_REJECT
        prtobj (obj, val, 0);
        PRINTF ("!*!*!*!*!*!*! OBJECT REJECTED !*!*!*!*!*!*!");
#endif
#endif
        return (No);
    }

    return (Ok);
}

/***********************************************************************/
/* This procedure checks initial state of object just read from ED-file
 On the basis of the general info the decision is being made
 whether the object is worth or not for consideration
 */
/***********************************************************************/
int16_t checkpart(SOBJ * obj)

{
    int16_t pi, si;
    int16_t BlankNo = 0, DigNo = 0, RelDigNo = 0;
    SPART *prt;
    prt = obj -> part;

    if (!(prt -> type & T_DIG))
        return Ok;

    for (pi = 0; pi < prt -> lth; pi++) {
        si = prt -> posn[pi];

        if (obj -> pos[si].type_sp & T_BLANK)
            BlankNo++;

        if (obj -> pos[si].alt[0].type & T_DIG) {
            DigNo++;

            if (obj-> pos[si].orig.code != '1')
                RelDigNo++;
        }
    }

    if (DigNo * 4 > prt -> lth)
        prt -> type |= T_CHEESE;

    if ((BlankNo > 1) && (RelDigNo > 1))
        return No;

    return (Ok);
}

/***********************************************************************/
/* This procedure constructs an object as a combination of parts.
 All possible part-chains are considered and all appropriate
 word-chains are compared to find the best word-chain.
 The best word-chain estimation is considered to be the estimation
 of the object itself.
 Thus the decision is being made to determine whether the object
 is worth or not.                                          */
/***********************************************************************/
int16_t selectopt(SOBJ * obj, SPART * part)

{
    int16_t pi;
    pi = obj->pos_part_nmb - 1; /* part not found => consider last part   */

    if (selectobj(obj, pi, part)) /* find best part-chain               */
        return (Ok); /* Yes, entire chain found                      */

    if (obj -> type_sp & T_HYPHEN)
        return (No);

    if (obj->opt_mark) /* good part-chain (not entire) constructed ?   */
        return (Ok); /* Yes */

    return (No); /* No  */
}

/****************************************************************************/
/* This procedure constructs object as parts-chain (from right to left).
 The last part is fixed (obj->part_beg, obj->part_end),
 then all chains to the left are considered to choose the chain
 with max{sum of chain-part-marks}.
 Current chain under consideration is stored in cur_part[],
 the best chain is saved in obj->opt_part[]
 The last active part is identified
 by {ibeg,obj->pos_part_nmb} (beg & end indicies of obj->pos_part[])

 Returns:
 Ok - best part-chain, covering entire obj, is found and obj->opt_mark>0;
 No - otherwise and if obj->opt_mark>0, then the best chain constructed,
 but the chain is not entire
 */
/****************************************************************************/
int16_t selectobj(SOBJ * obj, int16_t ibeg, SPART * part)

{
    int16_t i;
    int16_t ib, ie; /* curr ibeg & iend                          */
    int16_t cur_part[MAX_WORD_SIZE]; /* curr list of obj partitioning positions:
     it is the copy of pos_part[], but
     0,..x,0,x,0,x,..N - partitioning, so that
     0 => no end & beg of parts,
     x => end & beg of best parts chosen       */
    int16_t cur_pi[MAX_WORD_SIZE]; /* respective part indices: {0,x} - pi0,.. . */
    int16_t maxi; /* the most right index of cur_pi[curi]      */
    int16_t pi;
    int16_t entire = 0; /* nonzero, if entire chain is construced    */
    /* ------------------------------------------------------------------------ */
    /* The Data Structure Chart:
     ibeg
     |
     ib       ie
     |        |
     obj->pos_part: {x0,  x1, x2,  x3, x4,  x5, obj->pos_part_nmb}
     |        |        |        |
     cur_part: {x0,  0 , x2,  0 , x4,  0 , obj->pos_part_nmb}
     |    |   |    |   |    |   |
     cur_pi: {pi0, 0 , pi2, 0 , pi4, 0 , 0}

     part[pi] :     [pi0]  |--------|        |        |
     |        |        |
     [pi2]           |--------|        |
     |        |
     [pi4]                    |--------|
     |
     maxi
     */
    /* ------------------------------------------------------------------------ */
    /* An Expample of Parts In Order.
     (N.B.: any part may be omitted)

     |--------------------------|

     |--------|

     |-----------------|

     |--------|

     |--------|
     */
    /* ------------------------------------------------------------------------ */
    /* At 1-st, find a starting part to construct chains to the left:           */
    ib = ibeg; /* curr part ibeg */
    ie = obj->pos_part_nmb; /* curr part iend */
    memset(cur_part, 0, sizeof(cur_part));

    if (!(findpart(obj, part, obj->pos_part[ib], obj->pos_part[ie], &pi))) {
        pi = obj->part_max; /* part not found => consider last part         */
        ib = part[pi].begi; /* the last part's beg index in obj->pos_part[] */
        ie = part[pi].endi; /* the last part's end index in obj->pos_part[] */
    }

    if ((pi == 0) && (!(part[pi].word)))
        goto No_selectobj; /* not worth part                               */

    cur_part[ib] = obj->pos_part[ib]; /* copy last part beg */
    cur_part[ie] = obj->pos_part[ie]; /* copy last part end */

    for (i = 0; i <= obj->pos_part_nmb; i++)
        cur_pi[i] = pi; /* store last part index, other parts are before  */

    maxi = ib; /* max ib for which to find back                  */

    /* ----------------------------------------------------------------------- */

    /* Then, construct chains                                                  */

    /* and estimate every time, when the most left part of a chain is found :  */
    do {
        if (findparte(part, /* find part back with:                */
                      obj->pos_part[ib], /* end specified to find a part        */
                      cur_pi[ib], /* max part[] index to find back       */
                      &pi /* here resultant pi is expected       */
                     )) { /* --------------- left adjacent part found : --------------- */
            ie = ib;
            ib = part[pi].begi; /* new ib                             */
            cur_part[ib] = part[pi].beg; /* store beg delimiter                */
            cur_part[ie] = part[pi].end; /* store end delimiter                */

            for (i = ib; i <= ie; i++) {
                cur_pi[i] = pi; /* store index of part[pi] found      */

                if ((i > ib) && (i < ie))
                    cur_part[i] = 0; /* reset intermediate delimiters      */
            }
        }

        else { /* ----------------left adjacent part not found : ----------- */
            if (ib > 0) /* can we try to skip & find left more ? */
            { /* Yes :                                 */
                ib--; /* previous left beg                     */
                continue; /* try find left adjacent more           */
            }

            /* else (ib=0) : */
            if (selectobj_best(obj, part, cur_part, &entire)) /* find entire best */
                goto Ok_selectobj; /* other entire part-chains are not better*/

        Next_selectobj:
            ib++;

            if (ib >= maxi) {
                if (maxi > 0) {
                    ib = maxi - 1;
                    cur_pi[ib] = cur_pi[maxi];
                    cur_pi[maxi] = 0; /* no alternative parts more with this left end */
                    cur_part[maxi] = 0; /* reset delimiter                      */
                    maxi = ib; /* decrease max left end                */
                }

                else
                    /* cannot do any more                   */
                    goto Ok_selectobj;
            }

            if (cur_pi[ib])
                cur_pi[ib]--;

            else
                goto Next_selectobj;

            pi = cur_pi[ib];

            for (i = 0; i < ib; i++) {
                cur_part[i] = 0; /* reset left delimiters                          */
                cur_pi[i] = pi; /* store last part index, other parts are before  */
            }
        } /* --------------------- EO_else ---------------------------- */
    }
    while (ib >= 0);

    /* ------------------------------- EO_do --------------------------------- */
Ok_selectobj:

    if ((obj->opt_mark) && (obj->opt_entire))/* Ok: entire opt part-chain found*/
        return (Ok);

No_selectobj:
    return (No); /* No: no worth entire part-chains*/
}

/***************************************************************************/
/* This procedure estimates a part-set given by part-delimiters cur_part[].
 The two values are being estimated:
 - penalty, counted as the sum of positions not available in the part-set;
 - bonus, counted as the sum of part->word->mark of the parts available
 The values are being compared with those of currently best partitioning,
 given by obj->opt_part, and the new best one is being chosen.

 Reply:
 Ok - best entire part-chain estimated;
 No - otherwise
 */
/***************************************************************************/
int16_t selectobj_best(SOBJ * obj, SPART part[], int16_t cur_part[],
                       int16_t * entire)

{
    int16_t ib, ie, pi, sumpen = 0, summark = 0;
    *entire = 1; /* default: entire part-chain       */

    for (ib = 0, ie = 1; ie <= obj->pos_part_nmb; ie++) {
        if (cur_part[ie]) /* is the delimiter in our chain here ? */
        { /* Yes => try find the part             */
            if (findpart(obj, part, cur_part[ib], cur_part[ie], &pi))
                summark += part[pi].word->mark; /* add part mark                   */

            else {
                sumpen += (obj->pos_part[ie] - obj->pos_part[ib]); /* add penalty */
                *entire = 0; /* the part-chain is not entire         */
            }

            ib = ie; /* end --> beg                          */
        }
    }

    if (!summark) /* nothing found                           */
        return (No); /* get out                                 */

    if (!obj->opt_mark) /* nothing previously                      */
        goto Ok_selobjbest; /* new is better anyway                    */

    if (sumpen > obj->opt_pen) /* 1-st, compare penalty                   */
        return (No); /* new is worst                            */

    if (summark > obj->opt_mark) /* is curr partitioning better then best ? */
    Ok_selobjbest: { /* Yes => curr will be best                */
        memcpy(obj->opt_part, cur_part, sizeof(obj->opt_part)); /* partitioning */
        obj->opt_mark = summark; /* mark         */
        obj->opt_pen = sumpen; /* penalty      */
        obj->opt_entire = *entire; /* flag         */

        if (*entire) /* only if entire chain */
            return (Ok); /* Ok reply             */
    }

    return (No); /* Otherwise No         */
}

/***********************************************************************/
/* This procedure determines whether any worthy words have been
 generated with a part considered or not. If yes, the part will
 be presented in parts-buf.                                */
/***********************************************************************/
int16_t selectpart(SOBJ * obj)
{
    int16_t nw = 0; /* a nmb of words in the part           */
    SWORD * wrdptr; /* curr wrd ptr                         */
    SWORD * wrdfst; /* it will be 1-st wrd ptr for the part */
    int16_t bestmark = 0; /* best word's mark                 */
    SWORD * bestword = NULL; /* best wrd ptr                     */
    wrdfst = obj->word;

    for (wrdptr = obj->part->word; wrdptr; wrdptr = wrdptr->next) {
        nw++; /* nmb of words in the part */
        wrdfst = wrdptr; /* at the end of for-loop will be 1-st wrd ptr */

        if (wrdptr->mark >= bestmark) { /* find best word                        */
            if ((!bestmark) && (obj->part->type_sp & T_BLANK)) /* the 1-st word    */
                goto Selectp_comp; /* is to be absolutely checked, if blanks */

            if (wrdptr->mark == bestmark) /* trying to find absolutely best word : */
            Selectp_comp:
                if (!(compwrd(obj, bestword, wrdptr))) /*      detailed comparison       */
                    continue; /* still bestword is the best               */

            bestmark = wrdptr->mark; /* store best mark  */
            bestword = wrdptr; /* and best wrd ptr */
        }
    }

    if (nw) /* if we have smth in the part-word-list : */
    { /* --------------------------------------- */
        if (nw == 1)
            goto Selectp_ok; /* if it is the only word => select (V.L.) */

        if (!(obj->part->type_sp & T_BLANK)) /* no blanks => no doubt            */
            goto Selectp_ok;

        if (bestword->mark_doubt > 0) /* no doubt with the best word ?           */
            goto Selectp_ok; /* Ok, no doubt                            */

        if (bestword->mark_doubt == 0)/* could not estimate ?                    */
            goto Selectp_no; /* Yes, not sure => nothing to do          */

        if ((bestword->mark + bestword->mark_doubt) <= VMwrd_sure) /* not sure ?*/
            goto Selectp_no; /* Yes, not sure => nothing to do          */

    Selectp_ok: /* we're sure in the best word of the part:*/
        obj->part->word = wrdfst; /* the part will have the only word(best)  */

        if (bestword != wrdfst) /* is the best word first               */
            memcpy(wrdfst, bestword, sizeof(SWORD)); /* best is 1-st !      */

        wrdfst++; /* save only the best word of part      */
        obj->word = wrdfst; /* will proceed with the next           */
        return (Ok); /* Yes, Ok part                         */
    }

    else
        /* nothing in the part                  */
    Selectp_no: { /* ------------------------------------ */
        obj->part->word = NULL; /* no words for the part           */
        obj->word = wrdfst; /* remove all words of a part      */
        return (No); /* no words, no part               */
    }
}

/***********************************************************************/
/*
 This procedure determines whether current word and
 in general object-part are worthy of further consideration or not.

 A part may be not worthy (case "Stop") if:
 - there are no remaind words to be generated via the part,
 which could be good enough;
 - there is enough very good words generated via the part,
 we should just to choose the best of them;

 In the "Stop"-case the part once more may be considered
 from the begining once more
 (i.e. all words generation from the part will be repeated);
 It may be used if glue-cut or special replacement available.
 */
/***********************************************************************/
int16_t checkwrd(SOBJ * obj)

{
    int16_t val = 0;
    int16_t lthmin = 0;
    SWORD * wrd;
    wrd = obj->word;
    /* set lthmin, if it is the original word : */
    /*                          --------        */

    if (obj->type_art & T_ALTS) { /* Lepik */
        if (wrd->type & T_NAME)
            lthmin = RMwrd_minlth_nam;

        else {
            if ((wrd->type & T_CAP) && (!(wrd->type & T_LOW)))
                lthmin = RMwrd_minlth_cap;

            if ((wrd->type & T_LOW) && (!(wrd->type & T_CAP)))
                lthmin = RMwrd_minlth_low;
        }
    }

    val = RMwrd_bad_alt_nmb * wrd->bad_alt_nmb + (((wrd->type_art & T_REPL)
                                                   && (!(wrd->type_art & T_GC))) ? RMwrd_repl_alt : 0)
          + ((wrd->type_art & T_GC) ? RMwrd_gc_alt : 0);

    if (
        /* (val > RMwrd_stop)
         ||
         */(wrd->lth < lthmin))
        return Stop;

    val = val + RMwrd_low_alt_nmb * wrd->low_alt_nmb
          /* + RMwrd_dif_alt_wt  * wrd->dif_wt */; /* Lepik comment */
    /*   Lepik's comment
     if (val > RMwrd_no)
     return No;
     */
    return Ok;
}

/* ------------------------------------------------------------------ */

int16_t CheckContext(SOBJ *obj, LTIMG *wrddef[], int16_t lth)
{
    int16_t type;
    type = obj -> word -> type;

    if ((type & T_CAP) && (type & T_LOW)) { /* low & capital  ?         */
        char a;
        int16_t fl = 0;
        int16_t i;
        int16_t ApfFlag = FALSE;

        for (i = 0; i < lth; i++) {
            if (*(wrddef[i] -> source) == BLANK)
                continue;

            a
            = ((wrddef[i]->lt == (LT *) & wrddef[i]->std)
               && (wrddef[i] ->lt ->attr & EOLIST)) ? (*wrddef[i] -> source)
              : (wrddef[i] -> lt -> code);

            switch (symcode((char *) &a)) {
                case _APF:
                    fl = 0;
                    ApfFlag = TRUE;
                    break;
                case E_CP:
                case R_CP:

                    if (ApfFlag == TRUE) {
                        ApfFlag = FALSE;
                        break;
                    }

                    if (!i)
                        break;

                    if (fl & T_LOW) {
                        obj->lthok = i;
                        return User_dicts;
                    }

                    fl |= T_CAP;
                    break;
                case E_OD:
                case R_OD:
                    ApfFlag = FALSE;

                    if (fl & T_CAP) {
                        obj->lthok = i;
                        return User_dicts;
                    }

                    fl |= T_LOW;
                    break;
                case _DIG:
                    fl |= T_DIG;
                    ApfFlag = FALSE;
                    break;
            }
        }
    }

    return All_dicts;
}
//////////////////////////////////////////////////////////////////////////////
// Allex 27.04.99
// подбираем максимальную разность для замены альтернатив
static int16_t GetMaxDiff(SOBJ * pObj, int16_t WordLenght)
{
    int16_t MaxDiff;

    if (pObj -> part -> type & T_NAME)
        if (pObj -> word -> type & T_NAME)
            MaxDiff = MD_for_name;

        else
            MaxDiff = MD_for_first_in_name;

    else if ((pObj -> part -> type & T_STMT) && ((pObj -> word -> type & T_LOW)
                                                 || (pObj -> word ->lth <= 3)))
        MaxDiff = MD_for_first;

    else
        MaxDiff = MD_for_not_name;

    //MD_for_one_char * WordLenght;
    return MaxDiff;
}
/***********************************************************************/
/*
 This procedure determines whether current word and
 in general object-part are worthy of further consideration or not.

 A part may be not worthy (case "Stop") if:
 - there is enough very good words generated via the part,
 we should just to choose the best of them;
 - there are no remaind words to be generated via the part,
 which could be good enough;

 In the "Stop"-case the part may be considered
 from the begining once more
 (i.e. all words generation from the part will be repeated);
 It may be used if glue-cut or special replacement available.        */
/***********************************************************************/
int16_t selectwrd(SOBJ * obj, LTIMG ** wrddef)
{
    int16_t val = 0;
    SWORD * wrd;
    int16_t max_dif;
    wrd = obj->word;
    max_dif = GetMaxDiff(obj, obj->word->lth);

    if (wrd->dif_wt > max_dif) {
        int16_t i;

        for (i = 0; i <= obj -> lthok; i++)
            if ((wrd->pos[i]->alt[wrd->altn[i]].dif_wt > max_dif)
                    && (wrd -> pos[i]->orig.attr >= PROB_TO_BRK)) {
                obj -> lthok = i - 1;
                return (No);
            }
    }

    val = VMwrd_bad_alt_nmb * wrd->bad_alt_nmb + VMwrd_low_alt_nmb
          * wrd->low_alt_nmb
          /* +  VMwrd_dif_alt_wt  *  wrd->dif_wt */;
#ifdef S_COMMENTS

    if (val > VMwrd_no) {
#ifdef WORDPR
#ifdef WORDPR_REJECT
        prtwrd(obj, wrd, 0, val);
        PRINTF ("-----SELECTWRD: uint16_t REJECTED -----");
#endif
#endif
        return (No);
    }

#endif
    {
        int16_t i;
        int16_t not_first = 0;
        int16_t del_fl = 0;
        int16_t tp = 0;

        for (i = 0; i < wrd->lth; i++) {
            if (wrd->altn[i])
                if ((wrd -> pos[i] ->orig.attr < PROB_ALLOWED)
                        || (wrd -> pos[i] ->alt[0].orig.attr
                            != wrd -> pos[i] ->alt[wrd -> altn[i]].orig.attr))
                    not_first++;

            if (wrd->pos[i]->alt[wrd -> altn[i]].type & T_DELIM)
                if (tp & (T_LOW | T_CAP | T_DIG))
                    if (i + 1 != wrd -> lth)
                        del_fl = 1;

            tp |= wrd -> pos[i] -> alt[wrd -> altn[i]].type;
        }

        if (not_first > 1)
            if ((not_first * 2) >= wrd->lth)
                return (No);

        if (!del_fl)
            wrd -> type &= (~T_DELIM);
    }
    wrd->mark
    = VMwrd_voc_prob * ((wrd->voc_prob) ? 1 : 0) /*Lepik */
      + ((wrd->type == obj->type_orig) ? VMwrd_contxt_ok : 0)
      - ((wrd->type_art & T_CHANGE) ? VMwrd_change : 0)
      - (((wrd->type_art & T_REPL) && (!(wrd->type_art & T_GC))) ? VMwrd_repl_alt
         : 0) - ((wrd->type_art & T_GC) ? VMwrd_gc_alt : 0)
      /*LEPIK*/ - (((wrd->type & T_DELIM) && (wrd->type & (T_CAP
                                                           | T_LOW))) ? VMwrd_delim : 0) - ((obj->type_art
                                                                                             & T_ALTS) ? VMwrd_alts : 0) - val;

    if (wrd->mark > VMwrd_min) {
        if ((obj->type_art & (T_REPL | T_GC | T_BRK))/* are there art changes in the word? */
                || (obj->part->type_sp & T_BLANK) /* or blank positions ?               */
           )
            setwrd_changed(obj, wrddef, wrd); /* change alts of the word to be correct*/

#ifdef WORDPR
#ifdef WORDPR_ACCEPT
        prtwrd(obj, wrd, wrd->mark, val);
        PRINTF ("*****SELECTWRD: uint16_t ACCEPTED *****");
#endif
#endif
        return (Ok);
    }

    else {
#ifdef WORDPR
#ifdef WORDPR_REJECT
        prtwrd(obj, wrd, wrd->mark, val);
        PRINTF ("-----SELECTWRD: uint16_t REJECTED -----");
#endif
#endif
        return (No);
    }
}
/////////////////////////////////////////////////////////////////////////////
/****************************************************************************/
/* This procedure stores changed alts of the word to be them correct,
 if they are artificially changed
 */
/****************************************************************************/
int16_t setwrd_changed(SOBJ * obj, LTIMG ** wrddef, SWORD * word)
{
    int16_t pi;
    uchar c;
    uchar * pc;
    pc = &c;

    for (pi = 0; pi < word->lth; pi++) {
        if ((word->pos[pi]->type_art & (T_REPL | T_GC | T_BRK))/* is the pos artificial? */
                /* Lepik comments */
                || (word->pos[pi]->type_sp & T_BLANK) /* or blank ?*/
           ) { /* Yes, it must be stored as a SPEC_POS: */
            c = (uchar) wrddef[pi]->lt->code;
            word->altn[pi] = (uchar) getstdn((char*) pc); /* decode letter to the std-alt nmb     */
            word->pos[pi] = &(obj->pos[SPEC_POS]); /*  and store spec pos        */
        }
    }

    return (OK);
}

/***********************************************************************/
/* This procedure carries out the detailed comparison of two words.
 bestword(w1) was considered before to be the best word,
 wrdptr(w2) is a new word to be compared.
 If bestword==NULL => the new word is just to be estimated.

 The length of the words is considered to be the same.
 Reply:
 Ok - wrdptr is the new best word,
 No - bestword is still the best
 */
/***********************************************************************/
int16_t compwrd(SOBJ * obj, SWORD * w1, SWORD * w2)

{
    int16_t pi; /* pos nmb                */
    int16_t ai1, ai2; /* alt nmbrs              */
    int16_t m1 = 0, m2 = 0; /* total estimations      */
    int16_t v1 = 0, v2 = 0; /* one-letter estimations */

    for (pi = 0; pi < w2->lth; pi++) {
        if (w1 != NULL)
            ai1 = w1->altn[pi];

        ai2 = w2->altn[pi];

        if (w2->pos[pi]->type_sp & T_BLANK) { /* if blank => compare */
            /* in all cases we increase mark_doubt, if OK, or decrease otherwise */
            if (!(back_recog (obj, w2, pi, ai2, &v2))) /* couldn't estimate ? */
            { /* Yes, cannot         */
                w2->mark_doubt += v2; /* v2<0 ! Your penalty, please, nothing more */
                continue; /* v2<0 can be set by prop.index or alt_sure check */
            }

            else /* the word estimated :*/
            { /* v2>0: alt estimated, use it like a prob : */
                if (v2 < PROB_MIN)
                    w2->mark_doubt -= VMwrd_bad_alt_nmb;/* like one bad alt more :    */

                else {
                    if (v2 < PROB_ALLOWED) /* like one low-prob alt more: */
                        w2->mark_doubt -= VMwrd_low_alt_nmb;

                    else
                        /* OK - estimation :       */
                        w2->mark_doubt += VMwrd_low_alt_nmb; /* like one low alt less   */
                }
            }

            if (w1 == NULL) /* no old word, just to estimate new ? */
                goto Compwrd_next; /* yes                                 */

            if (w1->mark_doubt != 0) { /* the old word already estimated      */
                v1 = w1->alt_doubt[pi]; /* restore old word mark by back_recog */
                goto Compwrd_ex;
            }

            if (!(back_recog (obj, w1, pi, ai1, &v1))) /* couldn't estimate ?  */
                goto Compwrd_next; /* Yes, unable          */

        Compwrd_ex:

            if (v1 == v2)
                continue; /* we account only the best alts' marks   */

            if (v1 > v2)
                m1 += v1;

            else
                m2 += v2;

        Compwrd_next:
            w2->alt_doubt[pi] = (uchar) v2; /* store new word mark by back_recog */
        }

        else {
            if (w1 == NULL) /* no old word, just to estimate new ? */
                continue; /* yes                                 */

            if (w1->pos[pi]->alt[ai1].lt->attr
                    == w2->pos[pi]->alt[ai2].lt->attr)
                continue; /* we account only the best alts' marks  */

            if (w1->pos[pi]->alt[ai1].lt->attr > w2->pos[pi]->alt[ai2].lt->attr)
                m1 += w1->pos[pi]->alt[ai1].lt->attr;

            else
                m2 += w2->pos[pi]->alt[ai2].lt->attr;
        }
    }

    /* the new word is better, if : */
    if ((m2 >= m1) && (m2 > 0)) /* it is estimated & not rejected */
        return (Ok); /* either 1-st or compared with the old */

    if ((m2 >= m1) && (w1 == 0)) /* LEPIK */
        return (Ok);

    if ((m2 >= m1) && (w1->mark_doubt == 0)) /* it is estimated & the 1-st */
        return (Ok);

    return (No); /* the old is better        */
}

#ifdef V_RECOG
/****************************************************************************/
/* This procedure gives estimation of the word->pos[pi]->alt[ai]->lt->code
 with respect to the position's original image in tif-file.
 The last one is specified by its envelope:
 word->pos->tifref.(row, col, width, height).

 Reply:
 Yes - the alternative is estimated, *val = estimation,
 No  - the alternative cannot be estimated (should be rejected)
 */
/****************************************************************************/
extern uchar * letters_pidx_table;
static back_recog(SOBJ * obj,
                  SWORD * word,
                  int16_t pi, int16_t ai,
                  int16_t * val
                 )
{
    int16_t i, j;
    char fl = 0;
    int16_t code;
    int32_t isq = 0; /* the eBOX structure item */
    uint16_t icosinus;
    struct tifref * wt;
    char hyp[10];
    t_answer ans[10];
    hyp[0] = word->pos[pi]->alt[ai].lt->code;
    hyp[1] = _2low(hyp[0]); /* LEPIK */
    hyp[2] = 0;
    code = hyp[0]; /* int code is used as an arg of makeBOX() */
    wt = &(word->pos[pi]->tif_ref);
    *val = -VMwrd_doubt_pen; /* default doubt penalty */
#ifdef S_COMMENTS

    /* 1-st, check the proportionality index:                                  */

    /* may be the hyp with the given width&height is not allowed by the index  */
    if (wt->width < wt->height)
        i = wt->width * 16 / wt->height;

    if (wt->width == wt->height)
        i = 16;

    if (wt->width > wt->height)
        i = 32 - wt->height * 16 / wt->width;

    /* proport.index index i calculated; it has to be in the range,            */
    /*  specified in the letter_pidx_table el(min,max), pointed by the hyp:    */
    j = hyp[0] * 2; /* pointer to the letter_pidx_table el                  */

    if ( (i < letters_pidx_table[j]) || (i > letters_pidx_table[j+1]) ) {
        *val = -VMwrd_propind_pen; /* the hyp is not allowed: penalty & go away ! */
        return No;
    }

    /* the hyp is allowed, let's check it via BOX:                             */
#endif

    if (!(form_raster (obj, word->pos[pi])))
        return (No);

#ifdef S_COMMENTS

    if (word->pos[pi]->e_BOX.isq) /*is it 1-st time we're checking the pos ? */
        fl = 1;

    embBOX(raster, wt->height, wt->width, hyp, ans, fl, word->pos[pi]->e_BOX.isq, 0);
#endif
    embBOX(raster, wt->height, wt->width, hyp, ans, fl, isq, 0);
    icosinus = ans[0].iprob;

    /***     PRINTF ("%s %u ",ans[0].ltr,icosinus);  ***/
    if (icosinus)
        *val = make_probBOXf (code, &icosinus);

    if ( (*val) > PROB_SURE )
        return (Ok);

    if ( (*val) > 0 ) /* not sure ?                         */
        *val = -VMwrd_doubt_pen; /* Your default doubt penalty, please */

    return (No);
}

/****************************************************************************/
/* Andrew Leman's responsibility :
 */
/****************************************************************************/
static char *Plst[] = { /* The matter of what to discriminate : */   "   ",
                                                                     "MwW",
                                                                     "1liItfr",
                                                                     "nhu",
                                                                     "Y",
                                                                     "/"
                      };
static int16_t Pl = 6; /* N_of items in the Discr.Table */
static int16_t Proi[][6] = { /* the Discriminating Table:     */
    {   255, 230, 200, 120, 70, 0}, /* all DEFAULT           */
    {   255, 180, 100, 70, 40, 0}, /* mw  VERY BAD      */
    {   255, 220, 160, 100, 60, 0}, /* 1il NOT VERY BAD      */
    {   255, 220, 160, 100, 70, 0}, /* nu  BAD           */
    {   255, 230, 200, 120, 70, 0}, /* all MEDIUM            */
    {   255, 240, 220, 150, 70, 0} /* /   RELIABLE       */
};
static uint16_t prob[7] = {32768, 32113, 31455, 30475, 29200, 0, 0};

/* ??? old static Proi independent of symbs (no discrimination) :
 static uint16_t mark[7]={32767,32685,30474, 29491, 27525, 0, 0};
 static int16_t Proi[7]={255, 240,200,150,50,0,0};
 ??? */

/****************************************************************************/
/* This procedure converts cosinus between two BOX matrices
 to discrete probability. The convertion depends on a symbol
 being recognized, using a reliability (a so-called
 "discriminating") table Proi
 */
/****************************************************************************/
static int16_t make_probBOXf(int16_t ltr, uint16_t *fi)
{
    int16_t result = 0;
    int16_t px, *pi1, *pi2;
    uint16_t wcos, *pb1, *pb2;
    char pl;
    int16_t w1;

    for (pl = 1; pl < Pl; pl++)
        if (STRCHR(Plst[pl], (int16_t)ltr))
            goto Discret;

    pl = 0;
Discret:
    wcos = *fi;

    for (px = 0, pb1 = prob, pi1 = Proi[pl], pb2 = pb1 + 1, pi2 = pi1 + 1;
            px < 5; px++, pi1++, pb1++, pi2++, pb2++) {
        if (wcos >= *pb2) {
            w1 = proport(wcos, *pb1, *pb2, *pi1, *pi2);

            if (w1 > 254)
                w1 = 254;

            result = w1 & 0x00fe;
            break;
        }
    }

    return(result);
}

#ifdef S_COMMENTS
/****************************************************************************/
/* This procedure converts cosinus between two BOX matrices
 to discrete probability. The convertion is independent of
 a symbol being recognized. Isn't used more
 */
/****************************************************************************/
static int16_t make_probBOXf_old (uint16_t *fi)
{
    int16_t prob, px, *pi1, *pi2;
    uint16_t wcos, *pb1, *pb2;
    wcos = *fi;

    for (px = 0, pb1 = mark, pi1 = Proi, pb2 = pb1 + 1, pi2 = pi1 + 1;
            px < 5; px++, pi1++, pb1++, pi2++, pb2++) {
        if (wcos >= *pb2) {
            prob = proport(wcos, *pb1, *pb2, *pi1, *pi2);

            if (prob > 254) prob = 254;

            break;
        }
    }

    return(prob);
}
#endif

/****************************************************************************/
/*
 */
/****************************************************************************/
form_raster(SOBJ * obj, SPOS * pos)

{
    if ((pos->tif_ref.width == 0) || (pos->tif_ref.height == 0))
        return No;

#ifdef S_COMMENTS

    if ( CHECKTIF || /* ????   */
            (pos->type_sp & T_LINE) /* new-line pos ?    */
       )
#endif
    {
        int32_t sh;

        if (!adjust_tif(obj, pos))
            return (No);

        sh = (int32_t)tif_st.shift * Q.tif_line_lth;
        trseek(sh);

        if (seek_and_read(Q.tif_file, tif_st.lth, tif_st.buf, sh) == -1)
            return (No);
    }
    fill_raster(raster, tif_st.buf, pos->tif_ref.col,
                (pos->tif_ref.row - 1) - tif_st.shift,
                pos->tif_ref.width, pos->tif_ref.height,
                Q.tif_line_lth, T->black_white);
    return (Ok);
}

/****************************************************************************/
/*
 */
/****************************************************************************/
adjust_tif(SOBJ * obj, SPOS * pos)
{
    uint16_t min, max;
    set_tif_strip(pos->tif_ref.segm, pos->tif_ref.symb, &min, &max);
    tif_st.shift = min;
    tif_st.lth = (max - min + 1) * Q.tif_line_lth;

    if (tif_st.lth > tif_st.buf_size) {
#ifdef SYSPR_ERROR
        PRINTF("TIF buffer is too small (%d) need %x\n", obj->nmb,
               tif_st.lth);
#endif
        return 0;
    }

    return 1;
}

/****************************************************************************/
/*
 */
/****************************************************************************/
void set_tif_strip(struct segm *segm, char *symb, int16_t*min, int16_t*max)

{
    char *savesymb;
    char *savesegm;
    int16_t i;
    savesymb = Q.ns_symb;
    savesegm = Q.ns_segm;
    Q.ns_segm = segm;
    skip_letter_in_line(segm, 0);

    while (Q.ns_symb != symb)
        next_symb(YES, YES, YES, Q.ns_segm, Q.ns_symb);

    *min = Q.stack->map_ref.row;
    *max = *min + Q.stack->map_ref.height;

    while (next_symb(YES, YES, YES, Q.ns_segm, Q.ns_symb)) {
        if (*min > (i = Q.stack->map_ref.row))
            * min = i;

        if (*max < (i = i + Q.stack->map_ref.height))
            * max = i;
    }

    set_last_symb(savesegm, savesymb);
}

/****************************************************************************/
/*
 */
/****************************************************************************/
void set_last_symb(struct segm *savesegm, char *savesymb)
{
    Q.ns_segm = savesegm;
    skip_letter_in_line(savesegm, 0);

    while ((Q.ns_symb != savesymb) &&
            (next_symb(YES, YES, YES, Q.ns_segm, Q.ns_symb)));
}

/* ????    static char stick[]={'?',0}; ???? */

/****************************************************************************/
/*
 */
/****************************************************************************/
check_voc()
{
    if (Q.v_state.vocfree)
        return(NO);

    else {
        tre_st.lth = 0;
        tif_st.lth = 0;
        return(YES);
    }
}

/****************************************************************************/
/*
 */
/****************************************************************************/
check_tre()
{
    if (tre_st.lth)
        return(NO);

    else {
        if (Q.v_state.vocfree)
            write_all_file(Q.temp_voc_file, V_POINT(Q.v_state.vocseg, 0),
                           (uint32_t)Q.v_state.vocfree);

        Q.v_state.vocfree = 0;
        return(YES);
    }
}

/****************************************************************************/
/*
 */
/****************************************************************************/
check_tif()
{
    if (tif_st.lth)
        return(NO);

    else {
        if (Q.v_state.vocfree)
            write_all_file(Q.temp_voc_file, MK_FP(Q.v_state.vocseg, 0),
                           (uint32_t)Q.v_state.vocfree);

        Q.v_state.vocfree = 0;
        return(YES);
    }
}

/****************************************************************************/
/*
 */
/****************************************************************************/
static int16_t fill_raster(char raster[], char *tif_buf,
                           int16_t x_map, int16_t y_map, int16_t t_wth, int16_t t_hght,
                           int16_t tif_line_lth, int16_t status)
/* check y_map ???? */
/*
 This procedure writes part of tif lane into raster
 corresponding to tif address of current symbol in ed_file.
 */
{
    int16_t tx_map; /* curr byte in tiff-line                   */
    int16_t ty_map; /* curr         tiff-line                   */
    int16_t tx_wth; /* tiff width (bytes)                       */
    int16_t xx; /* curr byte in curr result-line                 */
    int16_t dest; /* destination: x_map%8                          */
    /* it will be used for tiff shift (left) while   */
    /* move to raster                                */
    uint16_t two; /* the two bytes to use for shift & move     */
    uchar *c, *cc;
    uchar *tt;
    tx_map = x_map / 8; /* |tx_map                         */
    dest = x_map % 8; /* |dest   |    8    |    8    |.. */
    tx_wth = (t_wth + 7) / 8; /* total lth (bytes) */
    /* set init state:                               */
    memset (raster, 0, RASTER_SIZE); /* of raster                   */

    if ( tx_wth*t_hght > RASTER_SIZE )
        return -1;

    /* move & shift two bytes */
    c = raster;

    for ( ty_map = y_map; ty_map < y_map + t_hght; ty_map++) { /* tiff lines*/
        tt = tif_buf + (int32_t)ty_map * (int32_t)tif_line_lth + tx_map;

        for ( xx = 0; xx < tx_wth; xx++, c++, tt++) { /* tiff bytes*/
            cc = &two;
            *(cc + 1) = *tt;
            *cc = *(tt + 1);
            two = two << dest; /* shift to the left bound */
            *c = *(cc + 1); /* only senior byte has been formed */

            if (status)
                *c = ~(*c);
        }

        *(c - 1) &= ( 0xFF << (tx_wth * 8 - t_wth));
        /* set the rest of the last byte in the line to be zero */
    }

    return 0;
}

#endif

