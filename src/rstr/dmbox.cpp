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

//#define NO_LIN_KLASTER // for full 3x5 without linear klasterization

/*********************************************************************/
/*                                                                   */
/*                           module DMBOX                            */
/*                                                                   */
/*********************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "common/interval.h"
#include "struct.h"
#include "status.h"
#include "cuthdr.h"
#include "dmconst.h"
#include "func.h"
#include "ligas.h"
#include "linear.h"
#include "lang.h"

#include "compat_defs.h"
#include "minmax.h"

#define RASTER_SIZE 128*64/8
#define RASTER_WIDTH_B 128/8
int16_t my_Ps;

servBOX SBOX;
#define WIDE5x3  170
servBOX SBOX5x3;
uchar bool5x3;

extern uchar db_status; // snap presence byte
extern uchar db_pass; // current pass letter
extern int16_t best_answer_BOX;

static int16_t Proi[][6] = { { 255, 230, 190, 110, 50, 0 }, // all   DEFAULT
        { 255, 180, 100, 70, 40, 0 }, // wM@  VERY BAD
        { 255, 220, 160, 100, 60, 0 }, // 1il   NOT VERY BAD
        };

static uint16_t prob[7] = { 32768, 32113, 31455, 30475, 29200, 0, 0 };
//                    100     98     96     93

static int16_t many_legs(s_glue *);

#include "linutil.h"

static uchar wide_let[] = { 166, 172, 228, 232, 233, 235, 238 };

//  Add component to box raster
static void make_box_raster5x3(uint16_t* matr, c_comp * cp, int16_t row, int16_t col, int16_t h,
        int16_t w, lnhead * lp);

static void comp_to_box5x3(uint16_t* matr, c_comp * cp, uint16_t row, uint16_t col, uint16_t h,
        uint16_t w) {
    make_box_raster5x3(matr, cp, row, (int16_t) ((cp->left - col) * 5), h, w,
            (lnhead *) ((puchar) cp + cp->lines + sizeof(uint16_t)));
}

static void make_box_raster5x3(uint16_t* matr, c_comp * cp, int16_t row, int16_t add_col,
        int16_t h, int16_t w, lnhead * lp) {
    int16_t r3, row_rest, w2, w3, w4;
    uint16_t* pm;
    uint16_t *p;
    int16_t xs, xe, s1, s2, s3, s4, s5;
    interval * ip;

    w2 = w + w;
    w3 = w + w2;
    w4 = w + w3;
    next_line: r3 = (lp->row + cp->upper - row) * 3;
    row_rest = h - r3 % h;
    pm = matr + (r3 / h) * 5;
    ip = (interval *) (lp + 1);

    while (ip->l) {
        //              Count segment dispersion
        xe = ip->e * 5 + add_col;
        xs = xe - (ip++)->l * 5;
        s1 = s2 = s3 = s4 = s5 = 0;
        if (xs < w) {
            if (xe <= w) {
                s1 = xe - xs;
                goto fint;
            }
            s1 = w - xs;
            xs = w;
            goto sec_cell;
        }
        if (xs < w2) {
            sec_cell: if (xe <= w2) {
                s2 = xe - xs;
                goto fint;
            }
            s2 = w2 - xs;
            xs = w2;
            goto thr_cell;
        }
        if (xs < w3) {
            thr_cell: if (xe <= w3) {
                s3 = xe - xs;
                goto fint;
            }
            s3 = w3 - xs;
            xs = w3;
            goto fou_cell;
        }
        if (xs < w4) {
            fou_cell: if (xe <= w4) {
                s4 = xe - xs;
                goto fint;
            }
            s4 = w4 - xs;
            s5 = xe - w4;
        } else
            s5 = xe - xs;
        fint: ;

        //      Add segment to box raster
        if ((row_rest -= 3) >= 0) {
            *pm += s1 * 3;
            *(pm + 1) += s2 * 3;
            *(pm + 2) += s3 * 3;
            *(pm + 3) += s4 * 3;
            *(pm + 4) += s5 * 3;
            continue;
        }
        p = pm;
        pm += 5;
        row_rest += h;
        switch (h - row_rest) {
        case 1:
            *pm += s1;
            *(pm + 1) += s2;
            *(pm + 2) += s3;
            *(pm + 3) += s4;
            *(pm + 4) += s5;
            *p += s1 * 2;
            *(p + 1) += s2 * 2;
            *(p + 2) += s3 * 2;
            *(p + 3) += s4 * 2;
            *(p + 4) += s5 * 2;
            break;
        case 2:
            *pm += s1 * 2;
            *(pm + 1) += s2 * 2;
            *(pm + 2) += s3 * 2;
            *(pm + 3) += s4 * 2;
            *(pm + 4) += s5 * 2;
            *p += s1;
            *(p + 1) += s2;
            *(p + 2) += s3;
            *(p + 3) += s4;
            *(p + 4) += s5;
            break;
        case 3:
            *pm += s1 * 3;
            *(pm + 1) += s2 * 3;
            *(pm + 2) += s3 * 3;
            *(pm + 3) += s4 * 3;
            *(pm + 4) += s5 * 3;
            break;
        }
    }
    lp = (lnhead *) ((puchar) lp + lp->lth);
    if (lp->lth != 0)
        goto next_line;
}/*make_box_raster5x3*/

int16_t crecell5x3(cell *B1, s_glue *GL) {
    servBOX save;

    if (bool5x3) {
        bool5x3 = 0;
        memcpy(&save, &SBOX, sizeof(servBOX));
        crecell(B1, GL, 5);
        memcpy(&SBOX5x3, &SBOX, sizeof(servBOX));
        memcpy(&SBOX, &save, sizeof(servBOX));
    }
    return GL->ncell;
}/*crecell5x3*/

int16_t isWideLetter(uchar let) {
    if ((language == LANGUAGE_RUSSIAN) && memchr(wide_let, to_lower(let), sizeof(wide_let)))
        return 1;

    return 0;
}/*isWideLetter*/

int16_t isKlasterFull(int16_t typl);

static int16_t recBOX(int16_t rq, int16_t tpl, int16_t ftv, cell *BC) {
    int16_t typl, svr, svh, svw, svc, svmr, svmc, pa, pA, py;
    uchar c1, c2, c3;
    uchar p1, p2, p3;
    //extern uchar no_linpen;
    //if( no_linpen )
    //    return 8;

    typl = ftv; // **********************  not used
    typl = tpl;
    svr = BC->r_row;
    svh = BC->h;
    svw = BC->w;
    svc = BC->r_col;
    svmr = BC->row;
    svmc = BC->col;
    BC->r_row = SBOX.up_row;
    BC->r_col = SBOX.left_col;
    BC->w = SBOX.rast_w;
    BC->h = SBOX.rast_h;
    BC->row = SBOX.math_row;
    BC->col = SBOX.math_col;
    if (rq) {
        if (language == LANGUAGE_RUSSIAN) { // Valdemar
            BC->vers[0].let = (uchar) '\xa0' /* а */;
            BC->vers[1].let = (uchar) '\x80' /* А */;
            BC->vers[2].let = (uchar) '\xe3' /* у */;
        } else {
            BC->vers[0].let = 'a';
            BC->vers[1].let = 'A';
            BC->vers[2].let = 'y';
        }

        BC->vers[0].prob = 254;
        BC->vers[1].prob = 254;
        BC->vers[2].prob = 254;
        BC->nvers = 3;
        BC->vers[3].let = BC->vers[3].prob = 0; // 93.07.12
        levcut(BC, 1);
        p1 = BC->vers[0].prob;
        c1 = BC->vers[0].let;
        p2 = BC->vers[1].prob;
        c2 = BC->vers[1].let;
        p3 = BC->vers[2].prob;
        c3 = BC->vers[2].let;
        pa = pA = py = 0;
        if (language == LANGUAGE_RUSSIAN) {
            if (c1 == (uchar) '\xa0' /* а */)
                pa = p1;
            if (c1 == (uchar) '\x80' /* А */)
                pA = p1;
            if (c1 == (uchar) '\xe3' /* у */)
                py = p1;
            if (c2 == (uchar) '\xa0' /* а */)
                pa = p2;
            if (c2 == (uchar) '\x80' /* А */)
                pA = p2;
            if (c2 == (uchar) '\xe3' /* у */)
                py = p2;
            if (c3 == (uchar) '\xa0' /* а */)
                pa = p3;
            if (c3 == (uchar) '\x80' /* А */)
                pA = p3;
            if (c3 == (uchar) '\xe3' /* у */)
                py = p3;
        } else {
            if (c1 == 'a')
                pa = p1;
            if (c1 == 'A')
                pA = p1;
            if (c1 == 'y')
                py = p1;
            if (c2 == 'a')
                pa = p2;
            if (c2 == 'A')
                pA = p2;
            if (c2 == 'y')
                py = p2;
            if (c3 == 'a')
                pa = p3;
            if (c3 == 'A')
                pA = p3;
            if (c3 == 'y')
                py = p3;
        }
        if (pa > 120)
            typl |= 1;
        if (pA > 120)
            typl |= 2;
        if (py > 120)
            typl |= 4;
        if (!typl)
            typl = 8; // all other types
    } else
        reco2_cell(BC);
    BC->r_row = svr;
    BC->h = svh;
    BC->r_col = svc;
    BC->w = svw;
    BC->row = svmr;
    BC->col = svmc;
    return typl/*(rq)?(typl|8):(typl)*/;
}
extern uchar db_trace_flag;

int16_t dmiBOX(cell *A, s_glue *GL, int16_t fl2) {
    cell *BC;
    t_answer * v;
    int16_t i, ps, typl, fvers, ffull, fBOXvers, ftv, w1, w1max, upper, lower;
    int16_t *pi1, *pi2;
    SVERS svers, fsvers;
    cf::version *dv1, *dv2;
    uint16_t wcos, beste, best3, *pb1, *pb2;
    uchar c, c_acc, pb, wc;
    int16_t flit;
    char scg, vx, px, pl, flnu, flag_m, flag_rtf, stick;
    char wbuf[256]; // 06-09-94 09:59pm
    char flag_stick = 0; // Oleg : 31.08.92.

    Z = &string;
    my_Ps = def_locbas(A);
    BC = A;
    fvers = BC->nvers;
    fsvers.nvers = -1;
    scg = BC->cg_flag;
    if (GL->ncell > 1)
        BC->cg_flag |= c_cg_noenv;
    if (many_legs(GL)) {
        set_bad_cell(BC);
        if (db_status && (db_trace_flag & 4))
            est_snap(db_pass, BC, "many legs");
        return 0;
    }
    save_vers(BC, &svers);
    abris_reset();
    stick = 1;
    flit = 0;
    dv1 = BC->vers;
    ftv = 0;
    if (dv1->prob > (language != LANGUAGE_RUSSIAN ? 170 : 200))
        ftv = 1; // solid versions present
    for (; (wc = dv1->let) != 0; dv1++) {
        if (!memchr("lI1\xbb|!", wc, 6) && wc != liga_i && !(language == LANGUAGE_TURKISH && // 30.05.2002 E.P.
                (wc == i_sans_accent || wc == II_dot_accent)) && wc != liga_exm // 10.09.2000 E.P.
        )
            stick = 0;
    }
    ffull = 0;
    flag_rtf = flag_m = 0;
    SBOX.best_BOX[0].ltr = 0;

    if (ftv // solid tiger versions present
            //&& !(language == LANGUAGE_RUSSIAN && strchr("оЗз3", BC->vers[0].let))) //Pual 23-12-96
            && !(language == LANGUAGE_RUSSIAN && strchr("\xAE\x87\xA7\x33", BC->vers[0].let))) //Pual 23-12-96
    {
        if (stick && (BC->w <= 5)) {
            if (db_status && (db_trace_flag & 4))
                est_snap(db_pass, BC, "solid stick w<=5 no BOX");
            stick_center_study(BC, GL, 1); // Oleg : 04.09.92.
            return -1; // strong sticks not to be BOXtted
        }
        embBOX(&SBOX, &svers, (Bool) ((A->pos_inc & erect_rot) != 0)); // BOX estimate
        if (GL->arg & GABOXs)
            goto makeans; // single "stick" version
        if ((best_answer_BOX > 29500 && language != LANGUAGE_RUSSIAN) || (best_answer_BOX > 30300
                && language == LANGUAGE_RUSSIAN)) {
            if (db_status && (db_trace_flag & 4)) {
                char wrk[32];
                sprintf(wrk, language == LANGUAGE_RUSSIAN ? "best %d > 30300" : "best %d > 29500",
                        best_answer_BOX);
                est_snap(db_pass, BC, wrk);
            }
            rest_vers(BC, &svers);
            if (stick) {
                if (db_status && (db_trace_flag & 4))
                    est_snap(db_pass, BC, "good stick - restore EVENT's vers");
                stick_center_study(BC, GL, 1); // Oleg : 04.09.92.
                return -1; // good sticks - no need in BOX
            }
            goto makeans;
            // tiger_to_BOX_mixture
        } // BEST > 29500
        // express estimate advices  "full" BOX
        typl = 0;
        ps = (int16_t) get_size(); // try to cut some letters by linear criterion
        if (ps) {
            typl = recBOX(1, typl, ftv, BC);
            set_bad_cell(BC);
        }
#ifdef NO_LIN_KLASTER
        typl = 15;
#endif
        embBOXF(&SBOX, typl, (Bool) ((A->pos_inc & erect_rot) != 0));
        ffull = 1;
        goto makeans;
    } // IF (ftv) -- solid TIGERs versions in cell
    else // no solid tiger versions at all --> use full BOX
    {
        typl = 0;
        if (db_status && (db_trace_flag & 4))
            est_snap(db_pass, BC, "use FULL BOX");
        if (GL->arg & GABOXl) {
            typl = 15;
            embBOXF(&SBOX, typl, (Bool) ((A->pos_inc & erect_rot) != 0));
            goto makeans;
        }
        ps = (int16_t) get_size(); // try to cut some letters by linear criterion
        if (ps) {
            if (!fvers) // no tiger's - use letters compatile with linear criterion
                typl = recBOX(1, typl, ftv, BC);
            else
                typl = 15; // all tiger's were bad - don't beleave linear criterion
            set_bad_cell(BC);
        }
#ifdef NO_LIN_KLASTER
        typl = 15;
#endif
        if (GL->ncell > 1)
            typl |= 16; // forbid sticks
        embBOXF(&SBOX, typl, (Bool) ((A->pos_inc & erect_rot) != 0));
        ffull = 1;
    }
    makeans: BC->nvers = 0;
    dv1 = BC->vers;
    BC->recsource |= c_rs_BOX;
    BC->history |= c_rs_BOX;

    v = (t_answer*) &(SBOX.best_BOX); //AK 04.03.97 ? for address
    vx = VERS_IN_CELL - 1;
    fBOXvers = v->ltr;
    wbuf[0] = 0; // clear print buf
    flnu = 0;
    if ((SBOX.dens >= 16) && (fvers == 0)) // bold non-tiger letter
    // bold 'u' and 'n' tend to substitute each other
    {
        flnu = 1;
        upper = mBOX[1] + mBOX[4];
        lower = mBOX[10] + mBOX[13];
        if (upper < lower)
            flnu = 1;
        else
            flnu = 2;
    }
    while ((c = v->ltr) != 0) {
        char wrkb[20];
        wcos = v->iprob;

        if (db_status && (db_trace_flag & 4)) {
            sprintf(wrkb, "%c(%d,%d) ", c, wcos, v->n_rsn);
            strcat(wbuf, wrkb);
        }
        BC->nvers++;
        switch (flnu) {
        case 1:
            if (c == 'n') {
                c = 'u';
                if (db_status && (db_trace_flag & 4))
                    est_snap(db_pass, BC, "n-->u");
            }
            break;
        case 2:
            if (c == 'u') {
                c = 'n';
                if (db_status && (db_trace_flag & 4))
                    est_snap(db_pass, BC, "u-->n");
            }
            if (language == LANGUAGE_RUSSIAN && c == (uchar) '\xf7' /* ч */) {
                c = (uchar) '\xaf' /* п */;
                if (db_status && (db_trace_flag & 4))
                    //est_snap(db_pass, BC, "ў-->п");
                    est_snap(db_pass, BC, "\xF7\x2D\x2D\x3E\xAF");
            }
            break;
        }
        dv1->let = c;
        pl = 0; // assume non-discrimination
        if (!ftv) // BOX full list
        {
            switch (c) {
            case '@':
                pl = 1;
                break;

            case '1':
            case 'l':
            case 'i':
            case 'I':
                pl = 2;
                break;

                // Конфликтные коды 31.08.2000 E.P.
            case liga_exm_usual:
                if (liga_exm_usual == liga_exm)
                    pl = 2;
                break;

            case liga_exm_latin:
                if (liga_exm_latin == liga_exm)
                    pl = 2;
                break;

            case liga_i_usual:
                if (liga_i_usual == liga_i)
                    pl = 2;
                break;

            case liga_i_latin:
                if (liga_i_latin == liga_i)
                    pl = 2;
                break;

                // Турецкие палки 30.05.2002 E.P.
            case i_sans_accent:
            case II_dot_accent:
                if (language == LANGUAGE_TURKISH)
                    pl = 2;

                break;

            default:
                pl = 0;
            }
        }
        if (wcos != 32767) //  found in BOX table
        {
            for (px = 0, pb1 = prob, pi1 = Proi[pl], pb2 = pb1 + 1, pi2 = pi1 + 1; px < 5; px++, pi1++, pb1++, pi2++, pb2++) {
                if (wcos >= *pb2) {
                    w1 = proport(wcos, *pb1, *pb2, *pi1, *pi2);
                    if (w1 == 0)
                        w1 = 2;
                    w1 &= 0x00fe;
                    dv1->prob = (uchar) w1;
                    if ((my_Ps < 18) && (v->fnt == 4) && ((c == 'i') || (c == 'l')))
                        flit = w1 - 8;
                    break;
                }
            }
        }
        dv1++;
        v++;
        if (!(--vx))
            break;
    } // while  versions
    if (db_status && (db_trace_flag & 4))
        est_snap(db_pass, BC, wbuf);
    dv1->prob = dv1->let = 0;

    SBOX.best_BOX[BC->nvers].ltr = 0;
    if (db_status) {
        if ((GL->arg & GABOXt) || (fvers))
            snap_keep(7, (puchar) &SBOX.best_BOX, (int16_t) (BC->nvers * 5 + 1));
        else {
            if (GL->arg & GABOXl)
                snap_keep(8, (puchar) &SBOX.best_BOX, (int16_t) (BC->nvers * 5 + 1));
            else
                snap_keep(10, (puchar) &SBOX.best_BOX, (int16_t) (BC->nvers * 5 + 1));
        }
    }

    if (!ftv) {
        dv1 = BC->vers;
        for (i = 0; i < BC->nvers; i++) {
            c = dv1[i].let;
            if (isWideLetter(c) && isKlasterFull((int16_t) (0x100 + c)))
                if (dv1[i].prob >= WIDE5x3) {
                    crecell5x3(BC, GL);
                    embBOXF(&SBOX5x3, (int16_t) (0x100 + c), (Bool) ((A->pos_inc & erect_rot) != 0));
                    wcos = SBOX5x3.best_BOX->iprob;
                    for (px = 0, pb1 = prob, pi1 = Proi[pl], pb2 = pb1 + 1, pi2 = pi1 + 1; px < 5; px++, pi1++, pb1++, pi2++, pb2++) {
                        if (wcos >= *pb2) {
                            w1 = proport(wcos, *pb1, *pb2, *pi1, *pi2);

                            if (db_status && snap_activity('a')) {
                                char snap[80], *s = snap;
                                snap_newcell(BC);
                                s += sprintf(s, "5x3 : vers = %c(%3d)  3x5 : (%3d)", c, w1,
                                        dv1[i].prob);
                                snap_show_text(snap);
                                snap_monitor();
                            }

                            if (w1 == 0)
                                w1 = 2;
                            else if (w1 > dv1[i].prob)
                                w1 = dv1[i].prob;
                            w1 &= 0x00fe;
                            break;
                        }
                    }
                    dv1[i].prob = (uchar) w1;
                }
        }// for  versions
    }//if( !ftv )

    if (fl2) // recog2 requested
        recBOX(0, 0, ftv, BC);
    if (db_status && (db_trace_flag & 4))
        est_snap(db_pass, BC, "recog-2 done");
    BC->cg_flag = scg; // restore flag
    // -------------- Russian discrim -----------------
    if (language == LANGUAGE_RUSSIAN)
        r_criteria(BC, GL);

    // APPEND
    if (fvers) {
        if (ffull && stick) // weak sticks only in TIGER
        {
            if (best_answer_BOX <= 29500) // nothing valuable obtained
            { // retain tiger's versions
                if (db_status && (db_trace_flag & 4))
                    est_snap(db_pass, BC, "full BOX for stick < 29500- restore EVENTS");
                rest_vers(BC, &svers);
                if ((fvers == 1) && (BC->vers[0].let == SBOX.best_BOX[0].ltr)) {
                    if (db_status && (db_trace_flag & 4))
                        est_snap(db_pass, BC, "1 vers same as event's - to discrim");
                    goto personal;
                    // BOX same as TIGER - not to penalize
                }
                if (db_status && (db_trace_flag & 4))
                    est_snap(db_pass, BC, "sticks/2");
                for (dv2 = BC->vers; (((wc = dv2->let) != 0) && ((w1 = dv2->prob) != 0)); dv2++)
                    dv2->prob = (w1 / 2) & 0xfe; // suppress sticks, forget other
                goto personal;
            }
            // else : sticks were a dream of EVENTS; retain BOX best list
            best3 = beste = 0;
            for (dv2 = svers.vers; //AK 04.03.97 ? for address
            (((wc = dv2->let) != 0) && ((w1 = dv2->prob) != 0)); dv2++)
                if (beste < w1)
                    beste = w1; // beste - best tiger stick
            for (dv2 = BC->vers; (((wc = dv2->let) != 0) && ((w1 = dv2->prob) != 0)); dv2++)
                if ((wc == '!') || (wc == 'i') || (wc == 'l') || (wc == 'I') || (wc == '1') || (wc
                        == liga_exm) || (wc == liga_i) || language == LANGUAGE_TURKISH && // 30.05.2002 E.P.
                        (wc == i_sans_accent || wc == II_dot_accent)

                ) // stick
                    if (best3 < w1)
                        best3 = w1; // best3 - best BOX stick estimate
            beste = (beste + best3) / 2;
            for (dv2 = BC->vers; (((wc = dv2->let) != 0) && ((w1 = dv2->prob) != 0)); dv2++)
                if ((wc == '!') || (wc == 'i') || (wc == 'l') || (wc == 'I') || (wc == '1') || (wc
                        == liga_exm) || (wc == liga_i) || language == LANGUAGE_TURKISH && // 30.05.2002 E.P.
                        (wc == i_sans_accent || wc == II_dot_accent)) // stick
                    dv2->prob = (uchar) beste; // make all sticks equivalent by BOX
            if (db_status && (db_trace_flag & 4))
                est_snap(db_pass, BC, "all sticks to (best EVENT's+best BOX)/2");
            goto personal;
        }
        // else: BOX run at TIGER's letters; append BOX_best list by tiger's letters
        for (dv1 = svers.vers; (c = dv1->let) != 0; dv1++) {
            for (ftv = 0, dv2 = BC->vers; (dv2->let && dv2->prob); dv2++) {
                if (dv1->let == dv2->let) {
                    ftv = 1;
                    pb = dv2->prob = (dv2->prob + dv1->prob) / 2 & 0xfe;
                    if ((fvers == 1) && (BC->vers[0].let == SBOX.best_BOX[0].ltr))
                        if ((pb < 220) && (pb >= 170))
                            //  07/14/92
                            if (BC->cg_flag & c_cg_cut == 0)
                                dv2->prob = 218;
                    // promote strong single version to maximal possible value
                    // (try not to stop cut attempts)
                    //  07/11/92  FOR ORIGINAL LETTERS  O N L Y
                    break;
                }
            }
            if (ftv)
                continue; // Tiger letter found in BOX
            if (BC->nvers < (VERS_IN_CELL - 1)) // TIGER's version killed
            {
                dv2->let = c;
                if (fBOXvers) // version killed
                    w1 = (dv1->prob / 2) & 0xfe;
                else
                    w1 = dv1->prob & 0xfe; // all were killed
                dv2->prob = w1 & 0x00fe;
                dv2++;
                dv2->let = dv2->prob = 0;
                BC->nvers++;
            } else // list of versions is full; try to replace weakest
            {
                sort_vers(BC);
                if (fBOXvers) // version killed
                    w1 = (dv1->prob / 2) & 0xfe;
                else
                    w1 = dv1->prob & 0xfe; // all were killed
                w1 &= 0x00fe;
                if (w1 > dv2->prob) // killed is better
                {
                    (dv2 - 1)->let = c;
                    (dv2 - 1)->prob = (uchar) w1;
                } // don't increment dv2: list is full
            }
        } // for all old tiger's
    } // if fvers   (append tiger versions to BOX)

    if (db_status && (db_trace_flag & 4))
        est_snap(db_pass, BC, "BOX before discrim");
    // apply personal trial criteria

    personal: if (flit && (BC->h > (my_Ps + 1)))
        promote(0, BC, 't', (int16_t) (flit + 512));
    for (w1max = (dv2 = BC->vers)->prob; (((c_acc = dv2->let) != 0) && ((w1 = dv2->prob) != 0)); dv2++) {
        c = let_sans_acc[c_acc];

        // CROAT, POLISH, CZECH, ROMAN, HUNGAR 05.09.2000 E.P.
        if (!BC->accent_leader && is_cen_language(language) && (c_acc == AA_right_accent || c_acc
                == a_right_accent || c_acc == AA_semicircle || c_acc == a_semicircle || c_acc
                == AA_roof_accent || c_acc == a_roof_accent || c_acc == CC_right_accent || c_acc
                == c_right_accent || c_acc == CC_inv_roof || c_acc == c_inv_roof || c_acc
                == DD_inv_roof || c_acc == d_inv_roof || c_acc == EE_right_accent || c_acc
                == e_right_accent || c_acc == EE_inv_roof || c_acc == e_inv_roof || c_acc
                == II_right_accent || c_acc == i_right_accent || c_acc == II_roof_accent || c_acc
                == i_roof_accent || c_acc == NN_right_accent || c_acc == n_right_accent || c_acc
                == NN_inv_roof || c_acc == n_inv_roof || c_acc == OO_right_accent || c_acc
                == o_right_accent || c_acc == OO_2dot_accent || c_acc == o_2dot_accent || c_acc
                == OO_double_right || c_acc == o_double_right || c_acc == RR_inv_roof || c_acc
                == r_inv_roof || c_acc == SS_right_accent || c_acc == s_right_accent || c_acc
                == SS_inv_roof_latin || c_acc == s_inv_roof_latin || c_acc
                == SS_bottom_accent_latin || c_acc == s_bottom_accent_latin || c_acc
                == TT_bottom_accent || c_acc == t_bottom_accent || c_acc == TT_inv_roof || c_acc
                == t_inv_roof || c_acc == UU_right_accent || c_acc == u_right_accent || c_acc
                == UU_circle_accent || c_acc == u_circle_accent || c_acc == UU_2dot_accent || c_acc
                == u_2dot_accent || c_acc == UU_double_right || c_acc == u_double_right || c_acc
                == YY_right_accent || c_acc == y_right_accent || c_acc == ZZ_right_accent || c_acc
                == z_right_accent || c_acc == ZZ_dot_accent || c_acc == z_dot_accent || c_acc
                == ZZ_inv_roof_latin || c_acc == z_inv_roof_latin

        ))
            c = 0;

        // LANGUAGE_LATVIAN и т.д. 09.07.2001 E.P.
        if (!BC->accent_leader && is_baltic_language(language) && (is_latvian_special(c_acc)
                || is_lithuanian_special(c_acc) || is_estonian_special(c_acc)))
            c = 0;

        // Турецкий язык 21.05.2002 E.P.
        if (!BC->accent_leader && is_turkish_language(language) && is_turkish_special(c_acc)
                && !is_turkish_palka(c_acc) // 18.06.2002 E.P.
        )
            c = 0;

        if (c == 'm')
            flag_m |= 1;
        if (c == 'n')
            flag_m |= 2;
        if (c == 'u')
            flag_m |= 4;
        if (c == '\xb6')
            flag_m |= 2; // "ri"

        //if ((w1max >= 60) && (memchr("sSaoO0QGDwWMNHURdxq6<>cCkBEAюЮ", c, 30)
        if ((w1max >= 60) && (memchr("sSaoO0QGDwWMNHURdxq6<>cCkBEA\xEE\x9E", c, 30)
                && !is_russian_baltic_conflict(c) && // 17.07.2001 E.P.
                !is_russian_turkish_conflict(c) // 21.05.2002 E.P.
                || (c == ss_deaf_sound) || (c == (uchar) liga_CC) || (c == (uchar) liga_CR)))
            w1 = abris(GL, BC, c, w1);

        if ((c == 'r') || (c == 'f') || (c == 't'))
            flag_rtf = 1;

        if ((c == 'h') || (c == 'b')) {
            w1 = h_filt(BC, w1, GL, c);
            if (w1 <= 2)
                w1 = 2;
        }

        //if (memchr("YTIJL1il![]<>()/F7тТгГ°|", c, 24) && !is_russian_baltic_conflict(c) && // 17.07.2001 E.P.
        if (memchr("YTIJL1il![]<>()/F7\xE2\x92\xA3\x83\xF8\x7C", c, 24) && !is_russian_baltic_conflict(c) && // 17.07.2001 E.P.
                !is_russian_turkish_conflict(c) // 21.05.2002 E.P.
                || // "03.03.1993 RESTORE"
                language != LANGUAGE_RUSSIAN && (c == liga_i || c == liga_j ||

                // Конфликтные коды 17.07.2001 E.P.
                        !is_baltic_language(language) && // 17.07.2001 E.P.
                                !is_turkish_language(language) && ( // 30.05.2002 E.P.
                                c == i_right_accent || // 08.07.1993 FRENCH:
                                        c == i_roof_accent || c == II_right_accent || c
                                        == II_roof_accent ||

                                // Конфликтные коды // 07.09.2000 E.P.
                                        !is_cen_language(language) && (

                                        c == i_left_accent || c == i_2dot_accent || c
                                                == II_left_accent || c == II_2dot_accent))
                        || is_baltic_palka(c) || // Балтийские палки. 10.07.2001 E.P.
                        is_turkish_palka(c) // Турецкие палки. 30.05.2002 E.P.
                        ))
            flag_stick = 1; // Oleg : 16.09.92.

        if (language == LANGUAGE_CROATIAN && (c == CROAT_d || c == 'd') || language
                == LANGUAGE_POLISH && c == POLISH_l ||

        // 08.09.2000 E.P.
                language == LANGUAGE_CZECH && (c == d_inv_roof || c == t_inv_roof) || language
                == LANGUAGE_ROMANIAN && c == c == t_bottom_accent)

            flag_stick = 1;

        dv2->prob = w1 & 0x00fe;
    }

    if (flag_rtf | flag_stick)
        stick_center_study(BC, GL, 1); // Oleg : 04.09.92.
    if (flag_m)
        atlant(BC, GL, ftv, flag_m); // study legs
    apply_monus(BC);
    sort_vers(BC);
    if (db_status && (db_trace_flag & 4))
        est_snap(db_pass, BC, "BOX after discrim");
    //
    pb = 0;
    if (BC->nvers == 0) // no versions got
    {
        if (fsvers.nvers > 0) //  1st atempt done ?
        {
            rest_vers(BC, &fsvers);
            BC->flg = c_f_let;
            adjust_links(BC);
            pb = BC->vers[0].prob;
            return pb;
        }
        if (fsvers.nvers == 0)
            return 0;
        goto make_full;
    }

    BC->flg = c_f_let;
    adjust_links(BC);
    pb = BC->vers[0].prob;
    if (pb >= 36)
        return pb;

    // weak answer
    if (fsvers.nvers == 0) // 1st attempt done an d fully wrong
        return pb;
    if (fsvers.nvers > 0) // 1st made some versions - compare
    {
        if (fsvers.vers[0].prob > pb) // 1st was better
        {
            rest_vers(BC, &fsvers);
            pb = BC->vers[0].prob;
        }
        return pb;
    }
    // 1st attempt just done, was weak
    make_full: if (ffull) // already full BOX ?
        return pb;
    save_vers(BC, &fsvers);
    set_bad_cell(BC);
    save_vers(BC, &svers);
    fvers = 0;
    ftv = 0;
    ffull = 1;
    if (db_status && (db_trace_flag & 4))
        est_snap(db_pass, BC, "now use FULL BOX");
    ps = (int16_t) get_size(); // try to cut some letters by linear criterion
    if (ps)
        typl = recBOX(1, 0, ftv, BC);
    else
        typl = 15; // all tiger's were bad - don't believe linear criterion
#ifdef NO_LIN_KLASTER
    typl = 15;
#endif
    if (GL->ncell > 1)
        typl |= 16; // forbid sticks
    embBOXF(&SBOX, typl, (Bool) ((A->pos_inc & erect_rot) != 0));
    goto makeans;

}

#include "p2libr.h"
extern int32_t all_probs, good_probs, zero_probs;
int16_t dmBOX(cell *BC, s_glue *GL) {
    int16_t ret = 0;
    if (pass4_in) {// OLEG : SERBIAN PASS4
        extern uchar langSer;
        ret = estletter(BC, GL);
        if (language == LANGUAGE_RUSSIAN && langSer && BC->nvers && BC->vers[0].let == SERB_j
                && ij_dot(BC) <= 0)
            ret -= 100;
        return ret;
    } else
        ret = dmiBOX(BC, GL, 1);
    /*
     if( 0&&spec_camera )
     {
     all_probs++;
     if(!BC->nvers)
     zero_probs++;
     else
     {
     if( BC->vers[0].prob>50 )
     good_probs++;
     else
     zero_probs++;
     }
     if( all_probs>100 )
     {

     if( good_probs*4*2<zero_probs )
     {
     //     good_probs = zero_probs/ret;
     ErrorExit(3);
     }
     }
     }
     */
    return ret;
}

char dust_in_pattern;
int16_t crepat(cell *A, s_glue *GL, int16_t var, int16_t flag) {
    cell *BC, *DC;
    int16_t d1, e3, e4;
    uchar pc;

    BC = A;
    dust_in_pattern = 0;
    GL->ncell = 1;
    GL->celist[0] = A;
    GL->ngluc = 1;
    GL->complist[0] = A->env;
    if (!(var & 3))
        goto retpt;

    DC = BC;
    e3 = MAX(BC->w * LEFT_DISTANCE_MIN, 200);
    e4 = MAX(BC->w * RIGHT_DISTANCE_MIN, 200);
    while ((DC->prev)->prev) // serach to the left
    {
        DC = DC->prev;
        d1 = BC->r_col - DC->r_col - DC->w;
        if (d1 > BC->w)
            break;
        if (DC->cg_flag & c_cg_noglue)
            continue;
        // if ((DC->flg & c_f_let) && ((DC->cg_flag & c_cg_rqdot) == 0)) break;
        ////// 93.08.19  No more RQDOT
        if (DC->flg & c_f_let)
            break;
        if (d1 > 4)
            break; // too far
        if (var & 2) // surrounding cells
        {
            if (d1 * 100 > e3)
                continue;
        } else // close cells
        {
            if (d1 > 0)
                continue;
        }
        // dust admitted; admit bad if not wide and bad enough
        ////// 93.08.19  No more RQDOT
        // if ((DC->flg & c_f_bad) || (DC->cg_flag & c_cg_rqdot))
        if (DC->flg & c_f_bad) {
            if ((flag & GFbadl) == 0)
                break;
            if (2 * DC ->w > 3 * A ->w)
                break;
            ////// 93.08.19  No more RQDOT
            /*******
             if (DC->cg_flag & c_cg_rqdot)
             goto take_bad1;
             *******/
            pc = DC->vers[0].prob;
            if (pc > 200)
                break; // 10/23/92:  was  > 170
        }
        if (DC->flg & c_f_dust)
            dust_in_pattern++;
        //////  no more RQDOT take_bad1:
        GL->celist[GL->ncell] = DC;
        GL->complist[GL->ncell++] = DC->env; // close enough
        if (GL->ncell + 2 >= MAX_GLUE_COMP)
            break;
    }

    DC = BC;
    while ((DC->next)->next) { // search to the right
        DC = DC->next;
        d1 = DC->r_col - BC->r_col - BC->w;
        if (d1 > BC->w)
            break; // too far - stop search
        if (DC->cg_flag & c_cg_noglue)
            continue;
        ////// 93.08.19  No more RQDOT
        // if ((DC->flg & c_f_let) && ((DC->cg_flag & c_cg_rqdot) == 0)) break;
        if (DC->flg & c_f_let)
            break;
        // Valdemar 08.11.93 to paste ы
        if (language == LANGUAGE_RUSSIAN && (BC->vers[0].let == (uchar) '\xec' /* ь */ || BC->vers[0].let
                == (uchar) '\x9c' /* Ь */))
            if (d1 > 5)
                break;
            else if (d1 > 4)
                break; // too far
        if (var & 2) // surrounding cells
        {
            if (d1 * 100 > e4)
                continue;
        } else // close cells
        {
            if (d1 > 0)
                continue;
        }
        // dust admitted; admit bad if not wide and bad enough
        ////// 93.08.19  No more RQDOT
        // if ((DC->flg & c_f_bad) || (DC->cg_flag & c_cg_rqdot))
        if (DC->flg & c_f_bad) {
            if ((flag & GFbadr) == 0)
                break;
            if (2 * DC ->w > 3 * A ->w)
                break;
            ////// 93.08.19  No more RQDOT
            /********
             if (DC->cg_flag & c_cg_rqdot)
             goto take_bad2;
             *********/
            pc = DC->vers[0].prob;
            if (pc > 200)
                break; // 10/23/92  was 170
        }
        if (DC->flg & c_f_dust)
            dust_in_pattern++;
        ////// no more RQDOT take_bad2:
        GL->celist[GL->ncell] = DC;
        GL->complist[GL->ncell++] = DC->env; // close enough
        if (GL->ncell + 2 >= MAX_GLUE_COMP)
            break;
    }
    retpt: GL->complist[GL->ncell] = NULL;
    GL->celist[GL->ncell] = NULL;
    return GL->ncell;
}

static int16_t sv_w, sv_h, sv_r, sv_c, sv_mr, sv_mc, sv_dens;
static int16_t sc_w, sc_h, sc_r, sc_c, sc_mr, sc_mc, sc_dens;
void SBOX_to_static() {
    sv_r = SBOX.up_row;
    sv_c = SBOX.left_col;
    sv_w = SBOX.rast_w;
    sv_h = SBOX.rast_h;
    sv_mr = SBOX.math_row;
    sv_mc = SBOX.math_col;
    sv_dens = SBOX.dens;
}
void SBOX_to_GL(s_glue *g) {
    g->c_r = SBOX.up_row;
    g->c_c = SBOX.left_col;
    g->c_w = SBOX.rast_w;
    g->c_h = SBOX.rast_h;
    g->c_mr = SBOX.math_row;
    g->c_mc = SBOX.math_col;
    g->c_dens = SBOX.dens;
}

void static_to_SBOX() {
    SBOX.up_row = sv_r;
    SBOX.left_col = sv_c;
    SBOX.rast_w = sv_w;
    SBOX.rast_h = sv_h;
    SBOX.math_row = sv_mr;
    SBOX.math_col = sv_mc;
    SBOX.dens = (char) sv_dens;
}
void GL_to_SBOX(s_glue *g) {
    SBOX.up_row = g->c_r;
    SBOX.left_col = g->c_c;
    SBOX.rast_w = g->c_w;
    SBOX.rast_h = g->c_h;
    SBOX.math_row = g->c_mr;
    SBOX.math_col = g->c_mc;
    SBOX.dens = (char) g->c_dens;
}

void dens_to_cell(cell *c) {
    c->dens = (uchar) sv_dens;
}

void static_to_cell(cell *c) {
    c->r_row = sv_r;
    c->r_col = sv_c;
    c->w = sv_w;
    c->h = sv_h;
    c->row = sv_mr;
    c->col = sv_mc;
    c->dens = (uchar) sv_dens;
}

void svcell_to_cell(cell *c) {
    c->r_row = sc_r;
    c->r_col = sc_c;
    c->w = sc_w;
    c->h = sc_h;
    c->row = sc_mr;
    c->col = sc_mc;
    c->dens = (uchar) sc_dens;
}

void cell_to_svcell(cell *c) {
    sc_r = c->r_row;
    sc_c = c->r_col;
    sc_w = c->w;
    sc_h = c->h;
    sc_mr = c->row;
    sc_mc = c->col;
    sc_dens = c->dens;
}

int16_t crecell(cell *B1, s_glue *GL, int16_t var) {
    int16_t wr, wc, ww, wh, wmr, wmc;
    // make raster and (if var & 2) calculate new width,height
    if (!make_broken_raster(GL, var))
        return -1;
    if (var & 2) // save row, col;  calc new;  restore original
    {
        wr = B1->r_row;
        wc = B1->r_col;
        ww = B1->w;
        wh = B1->h;
        wmr = B1->row;
        wmc = B1->col;
        B1->w = SBOX.rast_w;
        B1->h = SBOX.rast_h;
        B1->r_row = SBOX.up_row;
        B1->r_col = SBOX.left_col;
        ideal_rc(B1);
        SBOX.math_row = B1->row;
        SBOX.math_col = B1->col;
        if (!(var & 4)) // actual correction not forbidden
        {
            B1->r_row = wr;
            B1->r_col = wc;
            B1->w = ww;
            B1->h = wh;
            B1->row = wmr;
            B1->col = wmc;
        }
    }
    return GL->ncell;
}

static int16_t many_legs(s_glue *GL) {
    cell *A;
    lnhead *Lp1;
    char n1, h1, h2;
    uchar *wb;
    int16_t nc, Lc1, lc1, nleg;
    struct comp_struc *cp1;
    int16_t bm;

    nc = 0;
    nleg = 0;
    while (1) {
        A = GL->celist[nc++];
        if (!A)
            return 0;
        bm = get_bsm() + A->bdiff - A->row;
        cp1 = A->env;
        wb = (uchar *) cp1;
        wb += cp1->lines;
        Lc1 = cp1->nl; // number of lines in component
        if (Lc1 < 5)
            return 0;
        wb += 2;
        Lp1 = (lnhead *) wb;
        for (lc1 = 0; lc1 < Lc1; lc1++) {
            n1 = Lp1->h - 1;
            h1 = (char) Lp1->row;
            h2 = h1 + n1;
            if ((h1 < (bm - 2)) && (h2 > (bm + 2))) {
                nleg++;
                if (nleg > 4)
                    return 1;
            }
            Lp1 = (lnhead *) ((char *) Lp1 + Lp1->lth); // next line
        }
    }
}

servBOX *make_broken_raster(s_glue *GL, int16_t rq)
//
//      This procedure makes joint raster for several components.
//
{
    int16_t i, bw;
    int32_t m, t, s;
    uint16_t *p;
    c_comp **cp;
    c_comp *cp1;
    cell **celist;

    if (!(rq & 4))
        bool5x3 = 1;
    i = 0;
    cp = GL->complist;
    celist = GL->celist;
    SBOX.up_row = SBOX.left_col = 10000;
    SBOX.rast_h = SBOX.rast_w = 0;
    memset(&mBOX, 0, sizeof(mBOX));
    while ((cp1 = cp[i]) != NULL) // calculation of (absolute)
    { //    upper and left bounds
        if (cp1->left < SBOX.left_col)
            SBOX.left_col = cp1->left;
        if (cp1->upper < SBOX.up_row)
            SBOX.up_row = cp1->upper;
        if (SBOX.rast_h < celist[i]->h + celist[i]->r_row)
            SBOX.rast_h = celist[i]->h + celist[i]->r_row;
        if (SBOX.rast_w < celist[i]->w + celist[i]->r_col)
            SBOX.rast_w = celist[i]->w + celist[i]->r_col;
        i++;
    }
    SBOX.rast_w -= SBOX.left_col;
    SBOX.rast_h -= SBOX.up_row;
    if ((SBOX.rast_w > RASTER_MAX_WIDTH) || (SBOX.rast_h > RASTER_MAX_HEIGHT))
        return NULL;
    i = 0;
    if (rq & 1) {
        while ((cp1 = cp[i++]) != NULL)
            if (rq & 4)
                comp_to_box5x3(mBOX, cp1, SBOX.up_row, SBOX.left_col, SBOX.rast_h, SBOX.rast_w);
            else
                comp_to_box(mBOX, cp1, SBOX.up_row, SBOX.left_col, SBOX.rast_h, SBOX.rast_w);
    }
    m = SBOX.rast_h;
    m *= SBOX.rast_w;
    t = 15 * m;

    if (t < 0x10000) {
        i = *mBOX + *(mBOX + 1) + *(mBOX + 2) + *(mBOX + 3) + *(mBOX + 4) + *(mBOX + 5) + *(mBOX
                + 6) + *(mBOX + 7) + *(mBOX + 8) + *(mBOX + 9) + *(mBOX + 10) + *(mBOX + 11)
                + *(mBOX + 12) + *(mBOX + 13) + *(mBOX + 14);
        s = (uint16_t) i;
    } else
        for (s = 0, p = mBOX; p != mBOX + 15; p++)
            s += *p;

    bw = SBOX.dens = (char) (32 * s / t); // in 1/32 - units
    bw = 0; // USE black table
    if (bw) {
        for (i = 0, p = mBOX; i < 15; i++, p++)
            *p = m - (*p);
    }
    if ((SBOX.matrBOX.isq_rt = long_sqrt((*scalar)(mBOX, mBOX))) == 0)
        SBOX.matrBOX.isq_rt = 1;
    return &SBOX;
}

void snBOX(cell *b) {
    s_glue GL;
    GL.arg = GABOXt;
    crepat(b, &GL, 0, 0);
    crecell(b, &GL, 3);
    if (b->nvers)
        dmiBOX(b, &GL, 1);
    GL.arg = 0;
    set_bad_cell(b);
    dmiBOX(b, &GL, 1);
    GL.arg = GABOXl; // no LINEAR !!
    set_bad_cell(b);
    dmiBOX(b, &GL, 1);
}

int16_t simpleBOX(cell *C, int16_t typl) //(cell,128) for full exept 1l
{
    s_glue GL;

    GL.celist[0] = C;
    GL.celist[1] = NULL;
    GL.complist[0] = C->env;
    GL.complist[1] = NULL;
    GL.ncell = 1;

    crecell(C, &GL, 3);
    embBOXF(&SBOX, typl, (Bool) ((C->pos_inc & erect_rot) != 0));

    return 1;
}
