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

/* DIFFR.C : diskrimintion functions for russian language */

#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <algorithm> // for std::min/max
#include "recdefs.h"
#include "leo/leodefs.h"
#include "dif.h"

namespace cf {
namespace dif {

uchar rec5_flag = 1, font_type = 0, omni = 1;

int16_t NumVertInterval(uchar *RASTER, int16_t D_X, int16_t dy, int16_t i);
int16_t VertSum(uchar *rastr, int16_t D_X, int16_t dy, int16_t i);
int16_t SumBits(uchar *rastr, int16_t D_X);
int16_t NumHorizInterval(uchar *, int16_t);
int16_t FOOT(uchar *raster, int16_t DX, uchar dx, uchar Ly, uchar sign_filter);
int16_t FOOT3(uchar *RASTER, int16_t Wx, uchar START, uchar NWIDTH,
              uchar NLENGTH, int16_t SHIFT);
int16_t EndBlackInterval(uchar *RASTER, int16_t NWIDTH);
int16_t FOOT3_2(uchar *RASTER, int16_t Wx, uchar NWIDTH, uchar NLENGTH);
int16_t MinMaxLeft(uchar *RASTER, int16_t Wx, uchar NWIDTH, uchar NHEIGHT,
                   int16_t *Pmin, int16_t *Pmax);
int16_t MinMaxRight(uchar *RASTER, int16_t Wx, uchar NWIDTH, uchar NHEIGHT,
                    int16_t *Pmin, int16_t *Pmax);
int16_t FOOT_A(uchar *raster, int16_t DX, uchar dx, uchar Ly);
int16_t CenterVertInterval(uchar *, int16_t, int16_t, int16_t, int16_t *,
                           int16_t *);
int16_t SumIntervalBits(uchar *RASTER, int16_t bx, int16_t ex);
int16_t LeftDistance(uchar *RASTER, int16_t dx);
int16_t RightDistance(uchar *RASTER, int16_t dx);
int16_t no_serific(uchar *RASTR, int16_t dy, int16_t dx, int16_t wb);
}
}
int16_t FOOT_HEI(uchar *RASTER, int16_t Wx, uchar NWIDTH, uchar NLENGTH);
int16_t DiskrRight(uchar *RASTR, int16_t D_X, int16_t dx, int16_t dy, int16_t L);
int16_t Num2Interval(uchar *r, int16_t D_X, int16_t dx, int16_t dy);
int16_t broken_M(uchar * r, int16_t D_X, int16_t dy, int16_t left_lim,
                 int16_t ll);
int16_t fill_center_zone(uchar *raster, int16_t D_X, int16_t dy, int16_t beg,
                         int16_t end, int16_t II);
int16_t up_down_hist_M(uchar *rastr, int16_t D_X, int16_t Dx, int16_t dy);
int16_t small_density(uchar *RAST, int16_t n, int16_t D_X, int16_t bw);
void init_diskrim(uchar* raster, int16_t height, int16_t width);
int16_t DiskrLeftBig(uchar *RASTR, int16_t D_X, int16_t dx, int16_t dy,
                     int16_t L);
int16_t DiskrRightBig(uchar *RASTR, int16_t D_X, int16_t dx, int16_t dy,
                      int16_t L);

static void DiskrIN(uchar *RASTR, int16_t D_X, int16_t dy, int16_t wb,
                    int16_t dx);
static int16_t DiskrHorizIN(uchar *RASTR, int16_t D_X, int16_t dy);

#define bytlen(bits)  (REC_GW_WORD8(bits))

namespace cf {
namespace dif {
extern uchar BUFFER[256]; /* вертикальная   прoекция              */
extern uchar LOCAL[50]; /* координаты     ног             */
extern uchar LOCAL_W[50]; /* ширины         ног             */
extern uchar end1, beg2; /* конец 1 и начало 2-ой ног инп  */
extern int broken_ii; /* флаг двух палок            */
extern uchar broken_flag;
extern int16_t dnri_hook; // bottom right hook in small russian italic II,III
}
}

using namespace cf;

extern int16_t uple_hook; // bottom left  hook in small russian italic II,III
extern int16_t up_jack; // upper jack

static int16_t upper_skip_lines, lower_skip_lines;
static int16_t diskr_f2, diskr_ii, diskr_sh, diskr_g, diskr_tsh, diskr_ju,
diskr_m, diskr_p, diskr_N, diskr_c, diskr_o, diskr_e, diskr_b, diskr_3,
IN_N_Bonus, IN_equ, IN_P_Bonus, IN_I_Bonus, IN_IN_Monus, IN_dis,
diskr_i, diskr_n, IN_N, IN_I, IN_M, IN_pics, mii;

namespace cf {
namespace dif {
int16_t left_dist[4], right_dist[4], num_foot, c_or_e, d_c, d_e, right_max,
left_max, left_line, right_line, left_dist_big[4], right_dist_big[4],
lower_long_line;
int16_t av_tl, av_bl, av_br, rotate;
/* g-Ж, sh-Ш, b-B, k-K, ju-Ю, f2-ИНП, tsh-Щ, ii-Ы, o-O, c-C, e-E,
 h-Х, ja-Я, z-З, ee-Э, d-Д, ce-Ц  */
int16_t fill_center, up_down_serif, up_down_serif_B, IN_horiz_dis, broken_M_pen;
}
}

using namespace cf;

static int16_t DiskrSymSh(uchar *RASTER, int16_t Wx, uchar NWIDTH,
                          uchar NLENGTH);
static int16_t DiskrSh(uchar *RASTR, int16_t D_X, int16_t dx, int16_t Ly);
static int16_t DiskrLeft(uchar *RASTR, int16_t D_X, int16_t dx, int16_t dy,
                         int16_t L);
static int16_t DiskrSh0(uchar *RASTR, int16_t D_X, int16_t dx, int16_t dy,
                        int16_t dx0);
static int16_t DiskrJ0(uchar *RASTR, int16_t D_X, int16_t dx, int16_t Ly,
                       int16_t lim);
static int16_t DiskrJ(uchar *RASTR, int16_t D_X, int16_t dx, int16_t dy);
static int16_t DiskrTsh(uchar *RASTR, int16_t D_X, int16_t dx, int16_t dy,
                        int16_t Dx);
static int16_t average_br_angle(uchar *RASTER, int16_t D_X, int16_t dx,
                                int16_t dy, int16_t t);
static int16_t average_angle(uchar *RASTER, int16_t D_X, int16_t dx,
                             int16_t dy, int16_t(*Distance)(uchar *, int16_t), int16_t t);
static int16_t DiskrJu(uchar *RASTR, int16_t D_X, int16_t dx, int16_t Ly);
static int16_t DiskrimM1(uchar *RAST, int16_t D_X, int16_t dx, int16_t dy);
static int16_t DiskrimM(uchar *RAST, int16_t D_X, int16_t dx, int16_t dy);
static int16_t whiteMeanBitRight(uchar *RAST, int16_t D_X, int16_t dx,
                                 int16_t meanBit);
static int16_t whiteMeanBitLeft(uchar *RAST, int16_t D_X, int16_t meanBit);
static int16_t up_down_zones(uchar *raster, int16_t D_X, int16_t dx,
                             int16_t dx0, int16_t start1, int16_t stop1, int16_t start2,
                             int16_t stop2);
static int16_t DiskrVertCE(uchar *RASTR, int16_t D_X, int16_t dx, int16_t dy,
                           int16_t X, uchar let, uchar inc);
static int16_t AngleBottomRight(uchar *raster, int16_t D_X, int16_t hei);
static int16_t AngleTopRight(uchar *raster, int16_t D_X, int16_t hei);
static int16_t BonusAnglesCurve(uchar *raster, int16_t D_X, int16_t dy);
static Bool32 DiskrJuCut(int16_t nfoot, int16_t dx);

namespace cf {
namespace dif {

/*  clear diskrimination flags */
void init_diskrim(uchar * raster, int16_t height, int16_t width)
{
    int16_t i, D_X = bytlen(width);
    uchar * r;
    dif::rotate = 0;
    diskr_f2 = diskr_ii = diskr_N = -1;
    diskr_sh = diskr_g = diskr_tsh = diskr_ju = diskr_m = diskr_ii = -1;
    diskr_p = diskr_c = diskr_o = diskr_e = diskr_b = diskr_3 = -1;
    dif::left_dist_big[1] = dif::right_dist_big[1] = dif::left_dist[1] = dif::right_dist[1]
                                                          = dif::left_dist_big[2] = -1;
    dif::right_dist_big[2] = dif::left_dist[2] = dif::right_dist[2] = dif::left_dist_big[3] = -1;
    dif::right_dist_big[3] = dif::left_dist[3] = dif::right_dist[3] = dif::num_foot = dif::c_or_e = -1;
    dif::d_c = dif::d_e = dif::fill_center = IN_dis = diskr_i = diskr_n = IN_equ = IN_N = -1;
    IN_I = IN_M = IN_pics = dif::up_down_serif = dif::up_down_serif_B = dif::IN_horiz_dis = -1;
    dif::broken_M_pen = dif::lower_long_line = dif::right_max = dif::left_max = -1;
    dif::av_tl = dif::av_bl = dif::av_br = -1;
    mii = -101;
    IN_N_Bonus = 0;
    IN_P_Bonus = 0;
    IN_I_Bonus = 0;
    IN_IN_Monus = 0;
    r = raster;

    for (i = 0; i < height && dif::SumBits(r, D_X) < 3; i++, r += D_X)
        ;

    upper_skip_lines = i;
    r = raster + D_X * (height - 1);

    for (i = 0; i < 2 && dif::SumBits(r, D_X) < 3; i++, r -= D_X)
        ;

    lower_skip_lines = i;
    return;
}
}
}

Bool32 LeftHole(uchar *rastr, int16_t D_X, int16_t Dx, int16_t Hy)
{
    int i, n, t, mi, ma;

    if (Hy > 10) {
        i = Hy >> 3;
        Hy -= i;
    }

    else {
        i = 1;
        Hy--;
    }

    n = 0;
    mi = 10000;
    ma = 0;

    for (; i < Hy; i++, rastr += D_X) {
        t = dif::LeftDistance(rastr, D_X);

        if (t >= 0) {
            if (mi > t)
                mi = t;

            if (ma < t)
                ma = t;

            n++;
        }
    }

    if (!n)
        return FALSE;

    if (Dx < 10)
        return FALSE;

    i = 5;
    return (mi > Dx / 2 && abs(mi - ma) < i);
}

/* Diskrim : diskrimination function for diapason 0-255    */
/* PARAMETRS :                                             */
/*  let    - name of letter                            */
/*  raster - bit map( work raster )                    */
/*  D_X    - whidth of bit map( length bit string )    */
/*  X,Y    - coordinats of first point in raster       */
/*  dx,dy  - whidth and height work raster             */
/*                             */
/*   RETURN :   0 - good letter                            */
/*         >0 - decreasing code                        */

int16_t Diskrim(uchar let, uchar* raster, int16_t D_X, int16_t dx, int16_t dy,
                uchar /*cg_flag*/, int16_t inc)
{
    int16_t P = 0, F = 0, Dx, Hy, bw, n, r;
    uchar *rastr, *rastr0;
    int16_t d_l, d_r;
    int16_t X = 0, Y = 0;
#define  step_diskr 20
    dif::rotate = (inc > 180);

    if (upper_skip_lines) {
        raster += bytlen(D_X) * upper_skip_lines;
        dy -= upper_skip_lines;
    }

    if (lower_skip_lines) {
        dy -= lower_skip_lines;
    }

    if (dy < 8 || dx < 3)
        return (step_diskr * 10);

    D_X = bytlen(D_X);
    Dx = dx + (X & 7);
    bw = bytlen(Dx);
    rastr0 = raster + Y * D_X + (X >> 3);
    rastr = rastr0 + D_X * (dy >> 2);
    Hy = dy - 2 * (dy >> 2);
    d_l = (X & 7) + (dx >> 2);
    d_r = ((bytlen(Dx)) << 3) - dx - (X & 7) + (dx >> 2);

    switch (let) {
            // russian small & capital E, Ze, 3
        case '3':
        case (uchar) '\x87':
        case (uchar) '\xa7' /* з */:
        case (uchar) '\x9d':
        case (uchar) '\xed' /* э */:

            if (diskr_3 < 0) {
                if (LeftHole(rastr, D_X, Dx, Hy)) { // need mount on the central zone
                    P += 60;
                }

                diskr_3 = P;
            }

            else
                P = diskr_3;

            break;
            // russian small & capital B
        case (uchar) '\x82':
        case (uchar) '\xa2' /* в */:

            if (diskr_b < 0) {
                if (!DiskrRight(rastr0, D_X, Dx, dy, 1)) { // need pimple
                    P += 9 * 1;
                }

                diskr_b = P;
            }

            else
                P = diskr_b;

            break;
            // russian e
        case (uchar) '\xa5' /* е */:

            if (diskr_e < 0) {
                if ((n = DiskrVertCE(rastr0, D_X, dx, dy, X, (uchar) '\xa5' /* е */,
                                     (uchar) inc)) > 2)
                    P += (n - 2) * step_diskr;

                diskr_e = P;
            }

            else
                P = diskr_e;

            break;
            // russian o,O
            //  case '0' :
        case (uchar) '\x8e':
        case (uchar) '\xae' /* о */:

            if (diskr_o < 0) {
                if (DiskrRight(rastr0, D_X, Dx, dy, 2))
                    P = step_diskr;

                diskr_o = P;
            }

            else
                P = diskr_o;

            break;
            // russian c,C
        case (uchar) '\x91':
        case (uchar) '\xe1' /* с */:

            if (diskr_c < 0) {
                if (!DiskrRight(rastr0, D_X, Dx, dy, 2))
                    P = step_diskr;

                if ((n = DiskrVertCE(rastr0, D_X, dx, dy, X, (uchar) '\xe1' /* с */,
                                     (uchar) inc)) != 0)
                    P += n * step_diskr;

                diskr_c = P;
            }

            else
                P = diskr_c;

            break;
            // capital N
        case (uchar) 'N':

            if (diskr_N < 0) {
                uchar rasterN[4096];
                int i, ii, iii;

                for (iii = D_X * (dy - 1), ii = i = 0; i < dy; i++, ii += D_X, iii
                        -= D_X)
                    memcpy(rasterN + iii, raster + ii, D_X);

                rastr0 = rasterN + Y * D_X + (X >> 3);
                rastr = rastr0 + D_X * (dy >> 2);
                F = dif::FOOT(rastr, D_X, (uchar) Dx, (uchar) Hy, 1);

                if (F != 2 || std::min(dif::LOCAL_W[0], dif::LOCAL_W[1]) > 3 && dif::beg2 - dif::end1
                        < 3) {
                    if (dy > 13)
                        F = dif::FOOT(rastr0 + 2 * D_X , D_X, (uchar) Dx,
                                 (uchar)(dy - 4), 0);

                    else
                        F = dif::FOOT(rastr0, D_X, (uchar) Dx, (uchar) dy, 0);
                }

                if (F != 2) {
                    if (small_density(rastr, Hy, D_X, bw))
                        P = 6 * step_diskr;

                    else
                        P += step_diskr;

                    if (F == 3)
                        P += 6 * step_diskr;
                }

                else {
                    IN_dis = -1;
                    DiskrIN(rastr0, D_X, dy, bw, dx);
                    IN_dis = -1;

                    if (2 * dif::LOCAL[0] > 5 * dif::LOCAL_W[0] && 2 * (dx - dif::LOCAL[1]) < 3
                            * dif::LOCAL_W[1])
                        P += 2 * step_diskr;

                    if (IN_I < 3)
                        P += std::min(2 * (7 - IN_I + IN_equ) * step_diskr, 160)
                             / 2;

                    else if (IN_I == 3 && IN_equ > 2)
                        P += IN_equ * step_diskr;
                }

                if (inc > 0) // OLEG : ERECTION conditions : 09-12-95 07:29pm
                    P >>= 1;

                diskr_N = P;
            }

            else
                P = diskr_N;

            IN_dis = -1;
            IN_IN_Monus = 0;
            IN_pics = 1;
            IN_M = 0;
            break;
            // capital & small I/I
        case (uchar) '\x88':
        case (uchar) '\xa8' /* и */:

            if (diskr_i < 0) {
                F = dif::FOOT(rastr, D_X, (uchar) Dx, (uchar) Hy, 1);

                if (F == 2 && dif::LOCAL[1] * 2 <= Dx) {
                    diskr_i = P = 120;
                    break;
                }

                if (F != 2 || std::min(dif::LOCAL_W[0], dif::LOCAL_W[1]) > 3 && dif::beg2 - dif::end1
                        < 3) {
                    if (dy > 13)
                        F = dif::FOOT(rastr0 + 2 * D_X, D_X, (uchar) Dx,
                                 (uchar)(dy - 4), 0);

                    else
                        F = dif::FOOT(rastr0, D_X, (uchar) Dx, (uchar) dy, 0);
                }

                if (F != 2) {
                    if (small_density(rastr, Hy, D_X, bw))
                        P = 6 * step_diskr;

                    else
                        P += step_diskr;

                    if (F == 3)
                        P += 6 * step_diskr;
                }

                else {
                    DiskrIN(rastr0, D_X, dy, bw, dx);

                    if (2 * dif::LOCAL[0] > 5 * dif::LOCAL_W[0])
                        P += 2 * step_diskr;

                    if (IN_I < 3)
                        P += std::min(2 * (7 - IN_I + IN_equ) * step_diskr, 160)
                             / 2;

                    else if (IN_I == 3 && IN_equ > 2)
                        P += IN_equ * step_diskr;
                }

                if (inc > 0) // OLEG : ERECTION conditions : 09-12-95 07:29pm
                    P >>= 1;

                if (IN_I_Bonus && dif::broken_flag && (dif::broken_ii || !IN_IN_Monus
                                                  && Num2Interval(rastr0 + 2* D_X , (int16_t) D_X,
                                                                  (int16_t) Dx, (int16_t) (dy - 4))))
                    P = (IN_I_Bonus == 2) ? -254 : -250;

                diskr_i = P;
            }

            else
                P = diskr_i;

            break;
            // capital & small H
        case (uchar) '\x8d':
        case (uchar) '\xad' /* н */:

            if (diskr_n < 0) {
                F = dif::FOOT(rastr, D_X, (uchar) Dx, (uchar) Hy, 1);

                if (F != 2 || std::min(dif::LOCAL_W[0], dif::LOCAL_W[1]) > 3 && dif::beg2 - dif::end1
                        < 3) {
                    if (dy > 13)
                        F = dif::FOOT(rastr0 + 2 * D_X, D_X, (uchar) Dx,
                                 (uchar)(dy - 4), 0);

                    else
                        F = dif::FOOT(rastr0, D_X, (uchar) Dx, (uchar) dy, 0);
                }

                if (F != 2)
                    P = 6 * step_diskr;

                else {
                    DiskrIN(rastr0, D_X, dy, bw, dx);

                    if (DiskrHorizIN(rastr0, D_X, dy)) { // жирная перекладина
                        IN_N = 3;
                        IN_I = 2;
                    }

                    if (IN_N > 3)
                        P += std::min(IN_N * step_diskr, 160) / 2;
                }

                if (IN_N_Bonus && dif::broken_flag && (dif::broken_ii || !IN_IN_Monus
                                                  && Num2Interval(rastr0 + 2 * D_X, (int16_t) D_X,
                                                                  (int16_t) Dx, (int16_t) (dy - 4))))
                    P = (IN_N_Bonus == 2) ? -254 : -250;

                diskr_n = P;
            }

            else
                P = diskr_n;

            break;
            // capital & small II
        case (uchar) '\x8f':
        case (uchar) '\xaf' /* п */:

            if (diskr_p) {
                if (dy > 13)
                    F = dif::FOOT(rastr0 + 2 * D_X, D_X, (uchar) Dx, (uchar)(dy - 4), 0);

                else
                    F = dif::FOOT(rastr0, D_X, (uchar) Dx, (uchar) dy, 0);

                if (F != 2)
                    P = 6 * step_diskr;

                else { // F==2
                    if (dif::rotate && (dif::dnri_hook/*||dy<22&&up_jack>1*/))
                        fill_center_zone(rastr + D_X * (Hy >> 2), D_X,
                                         (int16_t) (Hy - (Hy >> 2)), dif::end1, dif::beg2, 1);

                    else
                        //fill_center_zone(rastr,D_X,Hy,dif::end1,dif::beg2,0);
                        fill_center_zone(rastr + D_X * (Hy >> 2), D_X,
                                         (int16_t) (Hy - (Hy >> 2)), dif::end1, dif::beg2, 1);

                    {
                        int16_t f_c = dif::fill_center;
                        DiskrIN(rastr0, D_X, dy, bw, dx);

                        if (!(dif::rotate && (dif::dnri_hook/*||dy<22&&up_jack>1*/)))
                            f_c = dif::fill_center;

                        if (f_c == 1)
                            P += 8 * step_diskr;
                    }
                }

                if ((n = up_down_zones(rastr0, D_X, Dx, dx, 0, (int16_t) (dy >> 2),
                                       Hy, dy)) != 0) {
                    if (dif::omni) {
                        if (n != 1)
                            P += 4 * step_diskr * n;
                    }

                    else { /* no omni  */
                        if (!dif::broken_flag || n == 2)
                            P += 4 * step_diskr * n;

                        else
                            P += 1 * step_diskr;
                    }
                }

                if (IN_P_Bonus && dif::broken_flag && (dif::broken_ii || !IN_IN_Monus
                                                  && Num2Interval(rastr0 + 2 * D_X, D_X, Dx, (int16_t) (dy
                                                                                                        - 4))))
                    P = (IN_P_Bonus == 2) ? -254 : -250;

                diskr_p = P;
            }

            else
                P = diskr_p;

            break;
            // capital & small III
        case (uchar) '\x98':
        case (uchar) '\xe8' /* ш */:

            if (diskr_sh < 0) {
                dif::FOOT(rastr, D_X, (uchar) Dx, (uchar) Hy, 0);

                if (dif::rotate) { // OLEG : ERECTION conditions : 09-20-95
                    if (F != 3)
                        P = 10 * step_diskr;
                }

                else {
                    if ((F = dif::FOOT3(rastr, D_X, 0, (uchar) Dx, (uchar) Hy, 2)) != 3)
                        P = 10 * step_diskr;

                    else
                        P += (step_diskr >> 1) * DiskrSymSh(rastr, D_X, (uchar) Dx,
                                                            (uchar) Hy);
                }

                if (inc <= 0 && !dif::broken_flag && DiskrSh0(rastr0, D_X, Dx, dy, dx)
                        == 0)
                    P += step_diskr;

                F = dif::FOOT(rastr, D_X, (uchar) Dx, (uchar) Hy, 0);

                // OLEG : ERECTION conditions : 09-20-95 08:34pm
                if (inc > 0 && dif::dnri_hook) {
                    if (F > 1 && DiskrSh(rastr, D_X, (int16_t) (dif::LOCAL[2]
                                                                + dif::LOCAL_W[2] / 2), Hy))
                        P += step_diskr;
                }

                else {
                    if (F > 1 && (n = DiskrSh(rastr, D_X, Dx, Hy)) != 0)
                        P += n * step_diskr;
                }

                if (F != 3)
                    P += 12 * step_diskr;

                if (DiskrLeft(rastr0, D_X, Dx, dy, 2))
                    P += step_diskr / 2;

                if (DiskrRight(rastr0, D_X, Dx, dy, 2))
                    P += step_diskr / 2;

                if (dif::FOOT3_2(rastr0, D_X, (uchar) Dx, (uchar) dy))
                    P += step_diskr * 3;

                diskr_sh = P;
                P = std::max((int) P, 0);
            }

            else
                P = diskr_sh;

            break;
        case (uchar) '\x86':
        case (uchar) '\xa6' /* ж */:

            if (diskr_g < 0) {
                if (!DiskrLeftBig(rastr0, D_X, Dx, dy, 2))
                    P += 3 * step_diskr;

                if (!DiskrRightBig(rastr0, D_X, Dx, dy, 2))
                    P += 3 * step_diskr;

                F = dif::FOOT(rastr, D_X, (uchar) Dx, (uchar) Hy, 0);

                if (F != 3 && (n = DiskrJ0(rastr, D_X, Dx, Hy,
                                           (int16_t) (dy < 18 ? 4 : 5))) != 0) { /* middle tail have'nt 3-interval lines */
                    if (!DiskrJ(rastr0, D_X, Dx, dy))
                        P += 2 * n * step_diskr;
                }

                if (F != 3 && (n = DiskrJ0(rastr0, D_X, Dx, Hy,
                                           (int16_t) (dy < 18 ? 2 : 3))) != 0) { /* upper tail have'nt 3-interval lines  */
                    if (!DiskrJ(rastr0, D_X, Dx, dy))
                        P += 2 * n * step_diskr;
                }

                diskr_g = P;
            }

            else
                P = diskr_g;

            break;
            // capital & small III,
        case (uchar) '\x99':
        case (uchar) '\xe9' /* щ */:

            if (diskr_tsh < 0) {
                F = ((uchar) let == (uchar) '\x99') ? 4 : 2;

                if ((F = dif::FOOT(rastr0 + F * D_X, D_X, (uchar) Dx, (uchar) Hy, 0))
                        != 3)
                    P = 10 * step_diskr;

                else {
                    P += (step_diskr >> 1) * DiskrSymSh(rastr, D_X, (uchar) Dx,
                                                        (uchar) Hy);

                    if (DiskrTsh(rastr0 + D_X, D_X, dx, dy, Dx) == 1)
                        P += 3 * step_diskr;
                }

                if (average_br_angle(rastr0, D_X, Dx, dy, 0) > d_r)
                    P += 4 * step_diskr;

                P = std::max((int) P, 0);
                diskr_tsh = P;
            }

            else
                P = diskr_tsh;

            break;
            // capiatl & small IO
        case (uchar) '\x9e':
        case (uchar) '\xee' /* ю */:

            if (diskr_ju < 0) {
                r = FOOT_HEI(rastr0, D_X, (uchar) dx, (uchar) dy);

                if (DiskrJuCut(r, dx) && r > 1)
                    P += 3 * step_diskr;

                if (DiskrLeft(rastr0, D_X, Dx, dy, 2))
                    P += 3 * step_diskr;

                if (DiskrRight(rastr0, D_X, Dx, dy, 2))
                    P += 3 * step_diskr;

                if ((n = DiskrJu(rastr, D_X, Dx, Hy)))
                    P += n * step_diskr;

                diskr_ju = P;
            }

            else
                P = diskr_ju;

            break;
            // capital & small M
        case (uchar) '\x8c':
        case (uchar) '\xac' /* м */:

            if (diskr_m < 0) {
                if (mii < -100) {
                    if (dif::broken_flag)
                        mii = DiskrimM1(rastr0, D_X, dx, dy);

                    else
                        mii = DiskrimM(rastr0, D_X, dx, dy);
                }

                if (mii > 0)
                    P += mii * step_diskr;

                if (mii == 0)
                    P += 5 * step_diskr;

                if (dy > 13)
                    F = dif::FOOT(rastr0 + 2 * D_X , D_X, (uchar) Dx, (uchar)(dy - 4), 0);

                else
                    F = dif::FOOT(rastr0, D_X, (uchar) Dx, (uchar) dy, 0);

                if (F == 2) {
                    DiskrIN(rastr0, D_X, dy, bw, dx);

                    if (dif::LOCAL[0] > dx / 4)
                        P >>= 2;

                    P += IN_M;
                }

                diskr_m = P;
            }

            else
                P = diskr_m;

            break;
            // capital & small bl
        case (uchar) '\x9b':
        case (uchar) '\xeb' /* ы */:

            if (diskr_ii < 0) {
                if (mii < -100) {
                    if (dif::broken_flag)
                        mii = DiskrimM1(rastr0, D_X, dx, dy);

                    else
                        mii = DiskrimM(rastr0, D_X, dx, dy);
                }

                if (mii < 0)
                    P += (-mii) * step_diskr;

                if (mii == 0)
                    P += 5 * step_diskr;

                diskr_ii = P;
            }

            else
                P = diskr_ii;

            break;
        default:
            break;
    }

    return (P & 0xFFFF);
}

int16_t small_density(uchar *RAST, int16_t n, int16_t D_X, int16_t bw)
{
    int16_t i, l, w, d;
    int16_t b = bw << 3;

    if (!dif::omni && dif::font_type == 0)
        return (1);

    for (l = i = 0; i < n; i++, RAST += D_X) {
        w = b - dif::RightDistance(RAST, bw) - dif::LeftDistance(RAST, bw);
        d = dif::SumBits(RAST, bw);
        l += (d > w - 2);
    }

    return (l <= (n / 3));
}

namespace cf {
namespace dif {
int16_t no_serific(uchar *RASTR, int16_t dy, int16_t dx, int16_t wb)
{
    int16_t l0 = dif::VertSum(RASTR, wb, dy, 0);
    int16_t l1 = dif::VertSum(RASTR, wb, dy, 1);
    int16_t l2 = dif::VertSum(RASTR, wb, dy, 2);
    int16_t r0 = dif::VertSum(RASTR, wb, dy, (int16_t) (dx - 1));
    int16_t r1 = dif::VertSum(RASTR, wb, dy, (int16_t) (dx - 2));
    int16_t r2 = dif::VertSum(RASTR, wb, dy, (int16_t) (dx - 3));
    dy -= 2;

    if (l0 < dy && l1 <= dy && l1 > dy - 2 && l2 > dy)
        l1 = l2;

    if (r0 < dy && r1 <= dy && r1 > dy - 2 && r2 > dy)
        r1 = r2;

    //return ( l0>dy || l1>dy ) && ( r0>dy || r1>dy ) ;
    return 1;
}

int16_t vert_stairs(int16_t arr[], int16_t lim)
{
    int16_t i, old, jmp;

    for (jmp = 0, i = 1, old = arr[0]; i < lim; i++) {
        if (arr[i] == -1)
            continue;

        if (old < arr[i])
            jmp++;

        if (old > arr[i])
            return 0;

        old = arr[i];
    }

    return (jmp);
}
}
}

/***************************************************************************/
/*                                                                         */
/*  возвращает   оценку возрастания середин интервалов перкладины ИН       */
/*                                                                         */
/***************************************************************************/
static void DiskrIN(uchar *RASTR, int16_t D_X, int16_t dy, int16_t bw,
                    int16_t dx)
/***************************************************************************/
/****     *RASTR     указатель   на  массив                         ********/
/****      dy        kоличество  строк                              ********/
/****      D_X       количество  байтов  в  строке                  ********/
/***************************************************************************/
{
    uchar n[256], hist[256];
    int16_t ua[256], da[256];
    int16_t i, n2 = dy - 2 * (dy >> 2), n4, mean, fine;
    int16_t incr, decr, old, neue, equ;
    int16_t l = dif::beg2 - dif::end1 - 1, l_real, t, jump, rmin, rmax;
    int16_t ol = 1, or_ = 1; /* зазор слева и справа */
    uchar * RAST, *R;
    n4 = std::max(dy / 4, (dif::LOCAL_W[0] + dif::LOCAL_W[1]) / 4);

    if (n4 > dy / 3)
        n4 = dy / 4;

    n2 = dy - (n4 << 1);
    RAST = RASTR + D_X * n4;

    if (IN_dis > 0)
        return;

    IN_IN_Monus = 0;
    IN_pics = 1;
    IN_M = 0;

    if (l + ol + or_ < 2 && dy > 19) {
        if (small_density(RAST, n2, D_X, bw)) { /* нет заливки */
            IN_N = 4;
            IN_I = 2;
        }

        else { /* заливка */
            IN_N = 4;
            IN_I = 3;
        }

        IN_dis = 1;
        IN_equ = 0;
        return;
    }

    memset(hist, 0, dy);

    if (l < 3) { /* отменить зазоры */
        or_ = ol = 0;
        l = dif::beg2 - dif::end1 + 1 - ol - or_;
    }

    if (l > 3) {
        int16_t up_space = -1, down_space = -1;
        int16_t up_fill = 0, down_fill = 0, d;

        for (R = RAST, i = n4; i <= dy - 2; i++, R += D_X) {
            d = dif::SumIntervalBits(R, (int16_t) (dif::end1 + ol), (int16_t) (dif::beg2 - or_
                                                                     + 1)) / 3;
            hist[i] = (uchar) d;

            if (d == l && !up_fill)
                up_fill = i;

            if (d == l && i < n4 + n2)
                down_fill = i;
        }

        if (up_fill && down_fill && up_fill > n4 && (hist[up_fill - 1] == 0
                                                     || hist[up_fill - 2] == 0) && (hist[down_fill + 1] == 0
                                                                                    || hist[down_fill + 2] == 0)) {
            IN_N = 3;
            IN_I = 2;
            IN_dis = 1;
            IN_equ = 1;
            return;
        }

        for (old = hist[n4], i = n4 + 1; i <= n2; i++) {
            neue = hist[i];

            if (up_space == -1)
                if (old == 1 && neue >= l && hist[i + 1] >= l || old == 0
                        && neue >= l - 1 && hist[i + 1] >= l) {
                    if (dif::NumHorizInterval(RASTR + D_X * (i + 1), bw) == 1) {
                        int16_t j, d;
                        up_space = (neue == l ? 0 : 1);

                        for (j = i + 1; j < dy - 2 && hist[j] >= l - 1; j++)
                            ;

                        if (j < dy - 2) {
                            for (d = 0; j < dy - 2; j++)
                                d += (hist[j] < hist[j - 1]);

                            if (d > 2) {
                                up_space = -1;
                                break;
                            }

                            if (d < 3) {
                                down_space = 0;
                                break;
                            }
                        }
                    }
                }

            if (down_space == -1)
                if (neue == 1 && old >= l && hist[i - 2] >= l || neue == 0
                        && old >= l - 1 && hist[i - 2] >= l) {
                    if (dif::NumHorizInterval(RASTR + D_X * (i - 2), bw) == 1)
                        down_space = (old == l ? 0 : 1);
                }

            old = neue;
        }

        if (up_space != -1 && down_space != -1) {
            IN_N = 3;
            IN_I = std::min(up_space, down_space);
            IN_dis = 1;
            IN_equ = 2;
            return;
        }

        memset(hist, 0, dy << 1);
    } /* end of horiz study */

    if (dif::no_serific(RASTR, dy, dx, bw)) {
        n4 = dy > 17 ? 4 : 2;
        n2 = dy - (n4 << 1);
        RAST = RASTR + D_X * n4;
    }

    memset(ua, 0xFF, dy << 1);
    memset(da, 0xFF, dy << 1);
    i = dif::end1 + ol - 1;
    n[i] = (uchar) dif::CenterVertInterval(RAST, D_X, n2, i, &ua[i], &da[i]);

    /* запасной левый отсчет        */
    for (mean = l_real = 0, i = dif::end1 + ol; i <= dif::beg2 - or_; i++) { /* таблица отчетов середин вертикальных интервалов   */
        n[i] = (uchar) dif::CenterVertInterval(RAST, D_X, n2, i, &ua[i], &da[i]);
        mean += n[i];

        if (n[i]) /* mean     - сумма   отсчетов        */
            l_real++; /* l_real - число ненулевых отсчетов  */
    }

    n[i] = (uchar) dif::CenterVertInterval(RAST, D_X, n2, i, &ua[i], &da[i]);

    /* запасной отсчет          */
    if (l != l_real && (l_real == 2 || (n[dif::end1] | n[dif::end1 + 1]) && (n[dif::beg2]
                                                                   | n[dif::beg2 - 1]))) { /* отсутствие перекладины            */
        IN_N_Bonus = 2;
        IN_I_Bonus = 2;

        if (l_real == 2)
            IN_P_Bonus = 1;
    }

    if (l_real <= 1 && ((n[dif::end1] == 0 && n[dif::end1 + 1] == 0) || (n[dif::beg2] == 0
                                                               && n[dif::beg2 - 1] == 0))) { /* отсутствие перекладины          */
        IN_N_Bonus = 1;
        IN_P_Bonus = 1;
        IN_I_Bonus = 1;

        if (n[dif::end1] == 0 && n[dif::end1 + 1] == 0 && n[dif::beg2] == 0 && n[dif::beg2 - 1]
                == 0) {
            IN_P_Bonus = 2;
            IN_N_Bonus = 1;
            IN_I_Bonus = 1;
        }
    }

    if (dif::no_serific(RASTR, dy, dx, bw)) { /* обратный пересчет в интервал высот [dy/4,dy-dy/4] */
        int16_t nn4, nn2, h;
        nn4 = std::max(dy >> 2, (dif::LOCAL_W[0] + dif::LOCAL_W[1]) >> 1);

        if (nn4 > dy / 3)
            nn4 = dy / 4;

        nn2 = dy - (nn4 << 1);
        RAST = RASTR + D_X * n4;

        for (mean = l_real = 0, i = dif::end1; i <= dif::beg2; i++) {
            h = n2 + n4 - (n[i] >> 1);

            if (h < nn4 || h > nn4 + nn2)
                n[i] = 0;

            else {
                n[i] += ((nn2 + nn4 - n2 - n4) << 1);

                if (n[i] >= (nn2 << 1))
                    n[i] = 0;

                else if (n[i] && (i != dif::end1 || (i == dif::end1 && ol == 0)) && (i
                                                                           != dif::beg2 || i == dif::beg2 && or_ == 0)) {
                    mean += n[i];
                    l_real++;
                }
            }
        }

        n2 = nn2;
        n4 = nn4;
    }

    if (l_real < 4) {
        i = dif::beg2 - or_ + 1;

        if (or_ && n[i] && dif::VertSum(RAST, D_X, n2, i) < n2 - 1
                && dif::NumVertInterval(RAST, D_X, n2, i) == 1) { /* учитываем последний отсчет */
            mean += n[i];
            or_--;
            l_real++;
            l++;
        }

        i = dif::end1 + ol - 1;

        if (ol && n[i] && dif::VertSum(RAST, D_X, n2, i) < n2 - 1
                && dif::NumVertInterval(RAST, D_X, n2, i) == 1) { /* учитываем последний отсчет */
            mean += n[i];
            ol--;
            l_real++;
            l++;
        }
    }

    if (l_real <= 1) {
        int16_t dy1 = n2, nn, mm, mm1, minup, mindown, zaz;
        uchar *rrrr, *rrrr1;
        zaz = dif::beg2 - dif::end1;

        for (minup = mindown = zaz, rrrr = RASTR, rrrr1 = RASTR + (dy - 1)
                                                          * D_X, i = 0; i < 3; i++, rrrr += D_X, rrrr1 -= D_X) {
            mm = zaz - dif::SumIntervalBits(rrrr, dif::end1, dif::beg2) / 3;

            if (minup > mm)
                minup = mm;

            mm1 = zaz - dif::SumIntervalBits(rrrr1, dif::end1, dif::beg2) / 3;

            if (mindown > mm1)
                mindown = mm1;

            nn = dx - dif::SumBits(rrrr, bw);

            if (nn < 2)
                break;
        }

        if (i < 3 || (minup * 2 < mindown && minup * 3 < zaz && zaz > 3)) { /* почти залитая крышка */
            IN_N_Bonus = IN_I_Bonus = 1;
            IN_P_Bonus = 2;
        }

        else {
            if ((nn = n[dif::end1 + ol]) > 0) {
                IN_N_Bonus = IN_I_Bonus = IN_P_Bonus = 1;

                if (abs(nn - dy1) < 3)
                    IN_N_Bonus = 2;

                else if (nn < dy1 - 2)
                    IN_I_Bonus = 2;
            }

            else if ((nn = n[dif::beg2 - or_]) > 0) {
                IN_N_Bonus = IN_I_Bonus = IN_P_Bonus = 1;

                if (abs(nn - dy1) < 3)
                    IN_N_Bonus = 2;

                else if (nn > dy1 + 2)
                    IN_I_Bonus = 2;
            }
        }
    }

    {
        int16_t z = dif::beg2 - dif::end1 + 1;

        if (z > 4 && l_real * 2 > z)
            IN_IN_Monus = 1; /* А перекладины разрыв то маленький ! */

        if (l_real > 4 || l - l_real <= 3 && l > 5 || l_real == 4
                && dif::fill_center || l_real < 2 && z < 4 && n4 < 6 || z > 10
                && l_real < 3) {
            if (l_real > 1)
                IN_P_Bonus = 0;

            /*IN_P_Bonus=*/
            IN_I_Bonus = IN_N_Bonus = 0;
        }
    }

    if (l_real > 1) {
        for (i = dif::end1 + ol; i <= dif::beg2 - or_; i++)
            if ((neue = n[i]) != 0)
                hist[neue]++;

        mean = mean / l_real + ((mean % l_real) > (l_real >> 1));
        /* mean   - средний отсчет      */

        if (hist[mean - 1] > l_real - 2)
            mean--;

        if (hist[mean + 1] > l_real - 2)
            mean++;

        if (l_real != l && !(l_real == l - 1 && (n[dif::end1] == 0 || n[dif::beg2] == 0))) {
            if ((!dif::fill_center && l_real <= 4) || l_real <= 3) {
                int16_t an[2], en[2], ll, dy1 = n2, san[2], sen[2], z;
                /* поиск прыщей от '\x8d' '\x8d' (Н) на середине высоты */
                an[0] = n[dif::end1];
                an[1] = n[dif::end1 + 1];
                en[0] = n[dif::beg2 - 1];
                en[1] = n[dif::beg2];

                for (ll = i = 0; i < 2; i++) {
                    sen[i] = n[dif::beg2 - i];
                    san[i] = n[dif::end1 + i];

                    if (an[i] > 0) {
                        if (abs(an[i] - dy1) > 2)
                            an[i] = -1;

                        else
                            n[dif::end1 + i] = (uchar) dy1, ll++;
                    }

                    if (en[i] > 0) {
                        if (abs(en[i] - dy1) > 2)
                            en[i] = -1;

                        else
                            n[dif::beg2 - i] = (uchar) dy1, ll++;
                    }
                }

                if (IN_I_Bonus == 2 && ll == 1)
                    IN_I_Bonus = 1;

                if (ll > 2 || ll == 2 && dif::broken_flag) {
                    for (i = 0; i < 2; i++) {
                        if (an[i] > 0)
                            n[dif::end1 + i] = (uchar) dy1;

                        if (en[i] > 0)
                            n[dif::beg2 - i] = (uchar) dy1;
                    }

                    mean = dy1;
                    dif::fill_center = 1;
                    IN_N_Bonus = 2;
                    IN_P_Bonus = IN_I_Bonus = 0;
                }

                else {
                    for (i = 0; i < 2; i++) {
                        n[dif::beg2 - i] = (uchar) sen[i];
                        n[dif::end1 + i] = (uchar) san[i];
                    }

                    /* поиск прыщей от '\x88' (И) по разные стороны от середины высоты */
                    an[0] = n[dif::end1];
                    an[1] = n[dif::end1 + 1];
                    en[0] = n[dif::beg2 - 1];
                    en[1] = n[dif::beg2];

                    for (ll = i = 0; i < 2; i++) {
                        z = dif::VertSum(RAST, D_X, n2, (int16_t) (dif::end1 + i));

                        if (an[i] > 0 && an[i] < dy1 - 2 && z < n4)
                            ll++;

                        z = dif::VertSum(RAST, D_X, n2, (int16_t) (dif::beg2 - i));

                        if (en[i] > 0 && en[i] > dy1 + 2 && z < n4)
                            ll++;
                    }

#ifndef Int16ERSEPTOR

                    if (ll > 2 || ll == 2 && dif::broken_flag && !dif::rotate)
#else
                    if ( ll > 3 || ll == 2 && dif::broken_flag && !dif::rotate)
#endif
                    {
                        dif::fill_center = 1;
                        IN_I_Bonus = 2;
                        IN_P_Bonus = IN_N_Bonus = 0;
                    }
                }
            }

            else {
                if (dif::fill_center && l_real > l - 3 && l > 2) {
                    int16_t lim = (dif::beg2 - or_ - dif::end1 + ol) >> 1;

                    for (t = 0, i = dif::end1 + ol; i <= lim; i++)
                        if (n[i] > ((n2 - 2) << 1))
                            t++;

                    if (t >= std::max(2, (l_real >> 1))) { /* перекладина слишком высоко            */
                        dif::fill_center = 0; /* коррекция штрафа за отстутствие перекладины     */
                        IN_N = 4;
                        IN_I = 1;
                        IN_dis = 1;
                        return;
                    }
                }

                if (dif::fill_center && l_real < 2 && l > 4) { /* полное отсутствие перекладины      */
                    dif::fill_center = 0; /* коррекция штрафа за отстутствие перекладины     */
                    IN_N = 4;
                    IN_I = 1;
                    IN_dis = 1;
                    return;
                }
            }
        }

        for (old = incr = decr = fine = 0, i = dif::end1 + ol, rmin = dy, rmax = 0; i
                <= dif::beg2 - or_; i++)
            if ((neue = n[i]) != 0) { /* fine - сумма расстояний от среднего    */
                if (old == 0)
                    old = neue;/* первый ненулевой отсчет   */

                if (neue > rmax)
                    rmax = neue;

                if (neue < rmin)
                    rmin = neue;

                if (neue > mean)
                    fine += neue - mean;

                else
                    fine += mean - neue;

                if (neue > old)
                    incr++; /* число скачков возрастаний    */

                else if (neue < old)
                    decr++; /* число скачков убываний   */

                old = neue;
            }

        if (fine && rmax - rmin <= 2 && !(l_real == 3 && incr == 2))
            fine = 0; /* все отсчеты в 2-х линиях */

        if (fine && l_real == 3 && incr == 2)
            fine += 2; /* 2 скачка в 3-х столбцах */

        if (dif::omni) {
            int16_t fin = fine, inc = incr, dec = decr;
            neue = n[dif::end1];

            if (ol && neue && neue < n[dif::end1 + 1]) {
                if (neue > mean)
                    fin += neue - mean;

                else
                    fin += mean - neue;

                inc++; /* число скачков возрастаний     */
            }

            neue = n[dif::beg2];

            if (or_ && neue && neue > n[dif::beg2 - 1]) {
                if (neue > mean)
                    fin += neue - mean;

                else
                    fin += mean - neue;

                inc++; /* число скачков возрастаний     */
            }

            /* большое сходство с '\x88' (И) */
            if (fin > 10 && inc > 3 && dec < 1 && dif::LOCAL[0] <= dx / 4)
                IN_M = 80;

            else if (fin > 10 && inc > 2 && dec == 0 && dif::LOCAL[0] <= dx / 4)
                IN_M = 80;

            else
                IN_M = 0;
        }

        if (abs(incr - decr) < 2 && /* близкие значения скачков     */
                (incr > 1 && /* более 1 возрастания     */
                 decr > 1 || /* более 1 убывания     */
                 incr && decr && incr + decr < 4) && /* мало скачков */
                fine <= l /* малая дисперсия        */
           )
            fine = 0;

        if (incr < 2 && /* маловато возрастаний   */
                l_real > 4 && /* на широкой перекладине */
                decr && incr + decr < 4) /* скачков немного        */
            fine = 0;

        if (incr * 2 < decr && l_real > 4) /* too many incr */
            fine = 0;

        if (fine > 2 && incr < 2 && decr > 1 && l > 3)
            fine -= 2; /* всего 1 возрастание       */

        t = (incr <= 1) && (decr <= 1);

        for (i = dif::end1 + ol; n[i] == 0 && i <= dif::beg2 - or_; i++)
            ; /* skip empty columns */

        for (equ = incr = 0, old = n[i++]; i <= dif::beg2 - or_; i++) {
            if (n[i])
                neue = n[i];

            else
                continue;

            if (neue > old)
                incr += neue - old;

            old = neue;
        }

        if (t == 0) /* более 1-го возрастания или убывания  */
            fine += incr; /* добавили штраф за возрастание  */

        for (t = equ = i = 0; i < dy; i++)
            if (hist[i] > equ)
                equ = hist[(t = i)];

        if (fine > 1 && equ > 2 && t == mean && or_ == 0 && ol == 0 && n[dif::end1]
                < mean && mean < n[dif::beg2] && equ + 2 == l_real)
            fine = 0; /* 1-ый и последний скачки в  */

        /* разорванном растре, интервал */

        /* постоянства длины 3      */
        if (equ > 1 && fine < 6) {
            int16_t fineold = fine;
            t = n4 + n2 - (t >> 1);

            for (RAST = RASTR + D_X * (t - 2), i = t - 2; i <= t + 2; i++, RAST
                    += D_X)
                if (dif::SumIntervalBits(RAST, dif::end1, (int16_t) (dif::beg2 + 1)) == (dif::beg2
                                                                          - dif::end1 + 1) * 3) { /* есть перекладина */
                    fine = 0;
                    break;
                }

            if (equ == 2 && (ol && n[dif::end1 + ol] || or_ && n[dif::beg2 - or_]))
                fine = fineold;
        }

        if (l_real < 5 && equ > 2)
            fine = 0; /* 3 из 4(или <4) совпадают       */

        if (l_real > 4 && equ > l_real - 2)
            fine = 0; /* совпадают l-1 из l отсчетов        */

        jump = 0;

        if (equ == l - 1) { /* единственный всплеск         */
            i = dif::end1 + ol;

            if (n[i] != 0 && n[i] < n[i + 1] && n[i + 2] == n[i + 1]) {
                fine = 0; /* единственный левый всплеск     */
                jump = 1;
            }

            else {
                i = dif::beg2 - or_;

                if (n[i - 1] != 0 && n[i] > n[i - 1] && n[i - 2] == n[i - 1]) {
                    fine = 0;
                    /* единственный правый всплеск      */
                    jump = 1;
                }
            }
        }

        if (l_real > 2) { /* нормировка накопленного штрафа     */
            fine <<= 5; /* *32 ?????????????            */
            fine /= l_real; /* истинная ширина          */
            fine /= n2; /* высота               */
        }
    }

    else {
        fine = 0; /* l_real<2 : не можем оценить образ  */
        IN_dis = 1;
        IN_equ = IN_N = 0;
        IN_I = 3;
        IN_pics = 0;

        if (dif::fill_center && l > 2)
            dif::fill_center = 0; /* коррекция штрафа за отстутствие перекладины */

        if (!dif::fill_center && l_real == 1
                && (dif::NumVertInterval(RAST, D_X, n2, dif::end1) == 1 && dif::VertSum(RAST,
                                                                         D_X, n2, dif::end1) < n4 && n[dif::end1] && abs(n[dif::end1] - (dy
                                                                                                                          >> 1)) < 3 || dif::NumVertInterval(RAST, D_X, n2, dif::beg2) == 1
                    && dif::VertSum(RAST, D_X, n2, dif::beg2) < n4 && n[dif::beg2] && abs(
                        n[dif::beg2] - (dy >> 1)) < 3))
            dif::fill_center = 2;

        return;
    }

    IN_N = IN_I = fine; /*           штраф за возрастание   */

    if (incr < 2 && l_real > 5 && decr > 4) {
        if (fine > 6)
            IN_N = 5;

        IN_I = 1; /* similar to N */
    }

    if (incr < 1 && l_real > 3 && decr > l_real / 2) {
        IN_N = 6;
        IN_I = 1; /* similar to N */
    }

    if (!fine && IN_M == 0 && l_real > 3 && decr >= l_real / 3 && decr > 1) {
        IN_N = 6;
        IN_I = 1; /* similar to M */
    }

    if (fine == 0 && jump)
        IN_I = 3;

    if (!dif::fill_center && (l_real > 3 || l_real > l - 3) && l > 2 && mean * 4
            < dy * 3) {
        if (l_real > 2 || n[dif::end1] && n[dif::beg2])
#ifdef Int16ERSEPTOR
            if ( l_real > 5 )
#endif
                if (!dif::rotate || l_real > 3)
                    dif::fill_center = 1; /* коррекция штрафа за отстутствие перекладины */
    }

    if (fine > 5 && l_real == 2 && !dif::broken_flag && incr == 0 && decr == 1) {
        IN_N = 4;
        IN_I = 2;
        dif::fill_center = 0;
    }

    IN_dis = 1;
    equ -= l - 2;
    IN_equ = (equ > 0) ? 2 + equ : 0; /* IN_equ -штраф за постоянство */

    if (DiskrHorizIN(RASTR, D_X, dy)) { /* жирная перекладина */
        IN_N = 3;
        IN_I = 2;
    }

    if (dif::omni) { /* '\x88' (И) путается с '\x88' (И) */
        int16_t i, le, ri, nnn = (dif::beg2 + dif::end1 + ol - or_) / 2;

        if (fine > 15 && decr > 3 || fine > 20 && decr > 2 || fine > 10 && incr
                < 1 && decr > 3) //RUS_ENG_LANG
            IN_I = 1;

        if (l_real > 4 && fine > 9) {
            for (old = n[dif::end1 + ol], le = 0, i = dif::end1 + ol; i < nnn; i++) {
                neue = n[i];

                if (neue) {
                    if (neue < old)
                        le++;

                    old = neue;
                }
            }

            for (old = n[i], ri = 0; i < dif::beg2 - or_; i++) {
                neue = n[i];

                if (neue) {
                    if (neue > old)
                        ri++;

                    old = neue;
                }
            }

            if (le > 3 && ri > 3)
                IN_I = 1;
        }
    }

    {
        int16_t up_skip = dif::vert_stairs(&ua[dif::end1], (int16_t) (dif::beg2 - dif::end1 + 1));
        int16_t down_skip = dif::vert_stairs(&da[dif::end1], (int16_t) (dif::beg2 - dif::end1 + 1));

        if (IN_I <= 3)
            if (up_skip > 3 && down_skip > 3 || up_skip > 1 && down_skip > 1
                    && up_skip + down_skip > 4) {
                IN_N = 4;
                IN_I = 4;
                IN_dis = 1;
                IN_equ = 0;
                return;
            }
    }

    return;
} /*        DiskrIN         */

/***************************************************************************/
int16_t DiskrHorizIN(uchar *RASTR, int16_t D_X, int16_t dy)
/***************************************************************************/
/****     *RASTR     указатель   на  массив                         ********/
/****      dy        kоличество  строк                              ********/
/****      D_X       количество  байтов  в  строке                  ********/
/***************************************************************************/
{
    uchar n[256];
    int16_t i, j, n2 = dy - 2 * (dy >> 2), n4 = dy >> 2, imax, nmax, kmax;
    int16_t l = dif::beg2 - dif::end1, h;
    uchar * RAST = RASTR + D_X * n4;

    if (dif::IN_horiz_dis >= 0)
        return (dif::IN_horiz_dis);

    if (l < 4) { /* малое расстояние */
        dif::IN_horiz_dis = 0;
        return (dif::IN_horiz_dis);
    }

    for (imax = nmax = kmax = -1, i = n4, j = 0; j <= n2; j++, i++, RAST += D_X) { /* заливки линий     */
        n[i] = dif::SumIntervalBits(RAST, dif::end1, dif::beg2) / 3;

        if (n[i] > nmax) {
            nmax = n[i];
            imax = i;
            kmax = 1;
        }

        else if (n[i] == nmax)
            kmax++;
    }

    if (imax > 0 && nmax == l && kmax > 1) {
        for (i = imax; n[i] == nmax; i--)
            ;

        h = ((n[i - 1] == 0 && n[i] < 3) || n[i] < 2);

        for (i = imax; n[i] == nmax; i++)
            ;

        l = ((n[i + 1] == 0 && n[i] < 3) || n[i] < 2);
    }

    else
        l = h = 0;

    return (dif::IN_horiz_dis = (h && l));
} /*        DiskrHorizIN        */

int16_t fill_center_zone(uchar *raster, int16_t D_X, int16_t dy, int16_t beg,
                         int16_t end, int16_t II)
{
    int16_t i, num, l, ny, d = ((end - beg) >> 1), p, white, w;
    uchar *r = raster;
#ifdef INTERSEPTOR
    end--;
    beg++;
    d = ((end - beg) >> 1);
#endif

    if (dif::fill_center >= 0)
        return (dif::fill_center);

    p = (end - beg > 3);

    if (dif::rotate)
        p = 0; // OLEG : ERECTION CONDITION

    white = end + p - beg + 1;

    for (ny = num = i = 0; i < dy; i++, r += D_X) {
        l = dif::SumIntervalBits(r, beg, (int16_t) (end + p)) / 3;

        if (i == 0 && l >= d)
            continue;

        w = end + p - beg - l;

        if (white > w)
            white = w;

        if (l) {
            num += l; // SUM of black bits in critical lines
            ny++; // num of critical lines
        }
    }

    if (ny) {
        if (!(dif::rotate && (dif::dnri_hook || II))) {
            if (end - beg < 5)
                dif::fill_center = (num > ny);

            else
                dif::fill_center = (num > (ny << 1));
        }

        else {
            dif::fill_center = 0;

            if (dif::rotate) {
                if (white < 2 && dif::dnri_hook)
                    dif::fill_center = 1;

                else if (d > 2 && white < d)
                    dif::fill_center = 1;
            }
        }
    }

    else
        dif::fill_center = 0;

    return (dif::fill_center);
}

int16_t up_down_hist_M(uchar *rastr, int16_t D_X, int16_t Dx, int16_t dy)
{
    int16_t i, j, d, h = dy >> 1, s, t, n;
    uchar *r;
    Dx = bytlen(Dx);

    for (s = j = 0, i = 0, r = rastr + i * D_X; i < h; j++, i++, r += D_X) {
        n = dif::NumHorizInterval(r, Dx);
        s += (n == 2);

        if (s > 2)
            break;

        if (j > 3 && s == 0)
            break;
    }

    if (s < 2)
        return (0);

    for (t = d = j = 0, i = dy - 1, r = rastr + i * D_X; i >= h; i--, j++, r
            -= D_X) {
        n = dif::NumHorizInterval(r, Dx);
        t += (n == 3);
        d += (n == 2);

        if (d > 2)
            break;

        if (t > 2)
            break;

        if (j > 3 && t == 0)
            break;
    }

    if (t < 2)
        return (0);

    if (s > 3 && t > 3)
        return (8);

    return (4);
}

int16_t broken_M(uchar * r, int16_t D_X, int16_t dy, int16_t left_lim,
                 int16_t ll)
{
    int16_t i, old, neue, dest, sign, fc, maxd, incr;
    uchar *rr;

    if (dif::broken_M_pen >= 0)
        return dif::broken_M_pen;

    old = dif::RightDistance(r, D_X);

    for (rr = r + D_X, i = 1; i < dy; i++, rr += D_X) {
        neue = dif::RightDistance(rr, D_X);

        if (abs(neue - old) > 1)
            return (dif::broken_M_pen = 0);/* непрямой правый абрис */

        old = neue;
    }

    /* прямой правый абрис */
    old = dif::LeftDistance(r, D_X);
    dest = old;

    for (maxd = fc = sign = incr = 0, dest = old, rr = r + D_X, i = 1; i < dy; i++, rr
            += D_X) {
        neue = dif::LeftDistance(rr, D_X);

        if (neue < old) {
            if (neue == old - 1) {
                if (sign)
                    return (dif::broken_M_pen = 0);

                else
                    sign = 1;
            }

            else
                return (dif::broken_M_pen = 0);/* немонотонность левого абриса */
        }

        if (neue >= ll)
            fc++;

        if (maxd < neue)
            maxd = neue;

        if (neue >= old)
            incr++;

        old = neue;
    }

    dest = neue - dest; /* скачок монотонного левого абриса */

    if (left_lim == ll)
        left_lim = 3;

    return (dif::broken_M_pen = ((dest > left_lim || (dest > left_lim - 2
                                                 && left_lim > 5 || dest == left_lim && left_lim > 4) && incr > dy
                             / 2) && fc < 3 && maxd > 3));
}

int16_t Num2Interval(uchar *r, int16_t D_X, int16_t dx, int16_t dy)
{
    int16_t i, n2, p, d;
    d = bytlen(dx);

    for (i = 1; i < 3; i++)
        if ((p = dif::NumHorizInterval(r - D_X * i, d)) == 1 && (n2 = dif::SumBits(r
                                                                         - D_X * i, d)) > dx - 2)
            return (0);

    for (n2 = i = 0; i < dy; i++, r += D_X) {
        p = dif::NumHorizInterval(r, d);
        /*  dif::NumHorizInterval : number of intervals in line  */
        n2 += (p == 2);
    }

    return (n2 == dy);
}
/* DiskrRight : check left hole */
int16_t DiskrRight(uchar *RASTR, int16_t D_X, int16_t dx, int16_t dy, int16_t L)
{
    int16_t sum, p, x, dl, Ly, i, p_old, curr_sum, h;
    int16_t minr, maxr, ddy;
    uchar *RASTER = RASTR + D_X * (dy >> 2), *R;

    if (dif::right_dist[L] >= 0)
        return (dif::right_dist[L]);

    Ly = dy - 2 * (dy >> 2);
    h = Ly;
    R = RASTER;
    dl = (((dx + 7) >> 3) << 3);
    ddy = dy > 30 ? (dy >> 3) : (dy >> 2);
    dif::MinMaxRight(RASTER, D_X, (uchar) dx, (uchar) Ly, &minr, &maxr);
    x = maxr - minr;

    if (maxr - ((dx & 7) ? (8 - (dx & 7)) : 0) > (dx >> 1)) {
        dif::right_max = maxr - minr;
        dif::right_line = 0;
        return ((dif::right_dist[L] = 1));
    }

    dif::right_dist[L] = (x >= L);

    if (dif::right_dist[L]) { /* big hole */
        dif::right_line = 0;

        if (dy > 17)
            while (dif::VertSum(R, D_X, h, (int16_t) (dl - minr)) < ddy && minr
                    < maxr)
                minr++;

        for (p_old = -1, sum = curr_sum = 0, i = minr; i <= maxr; i++) {
            p = dif::NumVertInterval(R, D_X, h, (int16_t) (dl - i));

            if (p >= 2) { /* наbegin 2-intervalsаseries of columns */
                if (p_old < 2)
                    curr_sum = 1;

                else
                    curr_sum++; /* наaccount width of hole */

                if (curr_sum > L)
                    break;
            }

            else if (p_old >= 2) { /* end 2-intervalsаseries of columns */
                if (curr_sum > sum)
                    sum = curr_sum;

                if (sum >= L)
                    break;
            }

            p_old = p;
        }

        if (sum == 0 && curr_sum)
            sum = curr_sum;

        dif::right_dist[L] = (sum >= L); /* hole */
    }

    else
        dif::right_line = 1;

    dif::right_max = maxr - minr;
    return (dif::right_dist[L]);
}

static int16_t DiskrSymSh(uchar *RASTER, int16_t Wx, uchar NWIDTH,
                          uchar NLENGTH)
{
    int16_t i, old, l, k, d;
    uchar c, w, minw = 255, maxw = 0;
    dif::FOOT_A(RASTER, Wx, NWIDTH, NLENGTH); /* projection to horiz axes */
    d = (NLENGTH + 1) >> 1;

    for (i = 0; i < NWIDTH; i++)
        dif::BUFFER[i] = (dif::BUFFER[i] >= d); /* binarazing */

    for (old = l = k = i = 0; i <= NWIDTH; i++) {
        c = (i < NWIDTH) ? dif::BUFFER[i] : 0;

        if (old ^ c) {
            if (c)
                l = i; /* black : наbegin of foot */

            else { /* white   : end of foot  */
                w = i - l;

                if (w > maxw)
                    maxw = w;

                if (w < minw)
                    minw = w;

                dif::LOCAL_W[k] = w; /* width of foot   */
                dif::LOCAL[k] = (l + i); /* center+1/2. Accuracy 1/2 pixelа */
                k++;
            }
        }

        old = c;
    }

    if (k != 3 || maxw - minw > 1)
        return (0); // enigmatic image

    i = dif::LOCAL[2] + dif::LOCAL[0] - (dif::LOCAL[1] << 1); //аassimetry

    if (i == 0)
        return (-2);

    if (i < 0)
        i = -i;

    if (i < 3 && NWIDTH > 22)
        i = 1; //Oleg:02-13-96: too small for wide image

    i = (i << 6) / (32 + NWIDTH);

    if (i == 1)
        i = 0;

    return (i);
}

static int16_t DiskrSh(uchar *RASTR, int16_t D_X, int16_t dx, int16_t Ly)
{
    uchar *RASTER = RASTR;
    int16_t i, num, n2, p, ddx;
    ddx = bytlen(dx);

    for (n2 = num = i = 0; i < Ly; i++, RASTER += D_X) {
        p = dif::NumHorizInterval(RASTER, ddx);
        /*  dif::NumHorizInterval : number of intervals in line */
        num += (p != 3);
        n2 += (p == 2);
    }

    if (n2) { /* 2-interval lines are exist */
        int16_t b = (dif::LOCAL[0] + dif::LOCAL[1]) / 2, dd = (dif::LOCAL[1] - dif::LOCAL[0])
                                                    + (dif::LOCAL_W[1] + dif::LOCAL_W[0]) / 2;
        i = Ly << 1;
        i = Ly / 3 - (Ly >> 2);
        Ly -= i;

        for (RASTER = RASTR + D_X * i; i < Ly; i++, RASTER += D_X) {
            p = dif::SumIntervalBits(RASTER, b, dx) / 3;

            if (p >= dd && dif::NumHorizInterval(RASTER, ddx) == 2)
                return 4;
        }
    }

    return (num > 2 ? num - 2 : 0);
}

/* for letters III */
static int16_t DiskrSh0(uchar *RASTR, int16_t D_X, int16_t dx, int16_t dy,
                        int16_t dx0)
{
    uchar *RASTER = RASTR + D_X * (dy - (dy >> 2));
    int16_t i, num, l = dx0 - (dx0 >> 2);

    if (dif::lower_long_line < 0) {
        dx = bytlen(dx);

        for (num = 0, i = dy - (dy >> 2); i < dy; i++, RASTER += D_X)
            num += (dif::NumHorizInterval(RASTER, dx) == 1 && dif::SumBits(RASTER, dx)
                    > l);

        /*  num : number of lines haved one long interval */
        dif::lower_long_line = num; /* number of filled lines */
    }

    return (dif::lower_long_line);
}

/*  DiskrLeft : check left hole */
int16_t DiskrLeft(uchar *RASTR, int16_t D_X, int16_t dx, int16_t dy, int16_t L)
{
    int16_t sum, p, x, i, Ly, p_old, curr_sum, h;
    int16_t minr, maxr;
    uchar *RASTER = RASTR + D_X * (dy >> 2), *R;

    if (dif::left_dist[L] >= 0)
        return (dif::left_dist[L]);

    Ly = dy - 2 * (dy >> 2);
    h = Ly;
    R = RASTER;
    dif::MinMaxLeft(RASTER, D_X, (uchar) dx, (uchar) Ly, &minr, &maxr);

    if (minr && maxr) {
        minr--;
        maxr--;
    }

    x = maxr - minr;

    if (minr > (dx >> 1)) {
        dif::left_max = maxr - minr;
        dif::left_line = 0;
        return ((dif::left_dist[L] = 1));
    }

    dif::left_dist[L] = (x >= L);

    if (dif::left_dist[L]) { /* big hole */
        dif::left_line = 0;

        if (dy > 17)
            while (dif::VertSum(R, D_X, h, minr) < (dy >> 2) && minr < maxr)
                minr++;

        for (p_old = -1, curr_sum = sum = 0, i = minr; i <= maxr; i++) {
            p = dif::NumVertInterval(R, D_X, h, i);

            if (p >= 2) { /* begin 2-intervalsаseries of columns */
                if (p_old != 2)
                    curr_sum = 1;

                else
                    curr_sum++; /* наaccount width of hole */

                if (curr_sum > L)
                    break;
            }

            else if (p_old >= 2) { /* end 2-intervalsаseries of columns */
                if (curr_sum > sum)
                    sum = curr_sum;

                if (sum >= L)
                    break;
            }

            p_old = p;
        }

        if (sum == 0 && curr_sum)
            sum = curr_sum;

        dif::left_dist[L] = (sum >= L); /* hole */
    }

    else
        dif::left_line = 1;

    dif::left_max = maxr - minr;
    return (dif::left_dist[L]);
}

/*  DiskrLeftBig : check big left holeа */
int16_t DiskrLeftBig(uchar *RASTR, int16_t D_X, int16_t dx, int16_t dy,
                     int16_t L)
{
    int16_t sum, p, x, i, Ly, p_old, curr_sum, h;
    int16_t minr, maxr;
    uchar *RASTER = RASTR + D_X, *R;

    if (dif::left_dist_big[L] >= 0)
        return (dif::left_dist_big[L]);

    Ly = dy - 2;
    h = dy;
    R = RASTR;
    dif::MinMaxLeft(RASTER, D_X, (uchar) dx, (uchar) Ly, &minr, &maxr);

    if (minr && maxr) {
        minr--;
        maxr--;
    }

    x = maxr - minr;
    dif::left_dist_big[L] = (x >= L);

    if (dif::left_dist_big[L]) { /* big hole */
        if (dy > 17)
            while (dif::VertSum(R, D_X, h, minr) < (dy >> 2) && minr < maxr)
                minr++;

        for (p_old = -1, curr_sum = sum = 0, i = minr; i <= maxr; i++) {
            p = dif::NumVertInterval(R, D_X, h, i);

            if (p >= 2) { /* наbegin 2-intervalsаseries of columns */
                if (p_old != 2)
                    curr_sum = 1;

                else
                    curr_sum++; /* наaccount width of hole */

                if (curr_sum > L)
                    break;
            }

            else if (p_old >= 2) { /* end 2-intervalsаseries of columns */
                if (curr_sum > sum)
                    sum = curr_sum;

                if (sum >= L)
                    break;
            }

            p_old = p;
        }

        if (sum == 0 && curr_sum)
            sum = curr_sum;

        dif::left_dist_big[L] = (sum >= L); /* hole */
    }

    return (dif::left_dist_big[L]);
}

/*  DiskrRightBig : check big right hole */
int16_t DiskrRightBig(uchar *RASTR, int16_t D_X, int16_t dx, int16_t dy,
                      int16_t L)
{
    int16_t sum, p, x, dl, Ly, i, p_old, curr_sum, h;
    int16_t minr, maxr;
    uchar *RASTER = RASTR + D_X, *R;

    if (dif::right_dist_big[L] < 0) {
        Ly = dy - 2;
        h = dy;
        R = RASTR;
        dl = (((dx + 7) >> 3) << 3);
        dif::MinMaxRight(RASTER, D_X, (uchar) dx, (uchar) Ly, &minr, &maxr);
        x = maxr - minr;
        dif::right_dist_big[L] = (x >= L);

        if (dif::right_dist_big[L]) { /* big hole */
            if (dy < 13 || dif::VertSum(RASTR, D_X, dy, (int16_t) (dl - minr)) < dy
                    - 2) {
                if (dy > 17)
                    while (dif::VertSum(R, D_X, h, (int16_t) (dl - minr))
                            < (dy >> 2) && minr < maxr)
                        minr++;

                for (p_old = -1, sum = curr_sum = 0, i = minr; i <= maxr; i++) {
                    p = dif::NumVertInterval(R, D_X, h, (int16_t) (dl - i));

                    if (p >= 2) { /* begin 2-intervalsаseries of columns */
                        if (p_old < 2)
                            curr_sum = 1;

                        else
                            curr_sum++; /* account width of hole */

                        if (curr_sum > L) {
                            sum = curr_sum;
                            break;
                        }
                    }

                    else if (p_old >= 2) { /* end 2-intervalsаseries of columns */
                        if (curr_sum > sum)
                            sum = curr_sum;

                        if (sum >= L)
                            break;
                    }

                    p_old = p;
                }

                if (sum == 0 && curr_sum)
                    sum = curr_sum;

                dif::right_dist_big[L] = (sum >= L); /* hole */
            }

            else
                dif::right_dist_big[L] = 0; /* no hole - vert line */
        }
    } /* calc dif::right_dist */

    return (dif::right_dist_big[L]);
}

/* for letters >|< */
static int16_t DiskrJ0(uchar *RASTR, int16_t D_X, int16_t dx, int16_t Ly,
                       int16_t lim)
{
    uchar *RASTER = RASTR;
    int16_t i, three, n, all, ret, one;
    dx = bytlen(dx);

    for (one = all = three = i = 0; i < Ly; i++, RASTER += D_X) {
        n = dif::NumHorizInterval(RASTER, dx);
        /*  dif::NumHorizInterval : number of intervals in line */
        three += (n == 3);
        all += (n >= 3);
        one += (n == 1);
    }

    if (Ly > 9) {
        ret = (three < lim) ? lim - three : 0;

        if (all == Ly && lim > 3) /* on middle */
            ret = 2;
    }

    else { /* small >|< */
        if (three >= lim)
            ret = 0;

        else
            ret = (all < lim) ? lim - all : 0;
    }

    if (lim <= 3 && ret && one + all > Ly - 3)
        ret = 0;

    return (ret);
}

static int16_t DiskrJ(uchar *RASTR, int16_t D_X, int16_t dx, int16_t dy)
{
    int16_t i, t, tu, td, Ly = dy >> 2, ly = dy - 2 * Ly , p, l = dy >> 1;
    dx = bytlen(dx);

    for (tu = i = 0; i < Ly; i++, RASTR += D_X)
        tu += (dif::NumHorizInterval(RASTR, dx) == 3);

    for (td = t = 0; i < ly; i++, RASTR += D_X) {
        p = dif::NumHorizInterval(RASTR, dx);
        t += (p == 1);

        if (i < l)
            tu += (p == 3);

        if (i > l)
            td += (p == 3);
    }

    for (; i < dy; i++, RASTR += D_X)
        td += (dif::NumHorizInterval(RASTR, dx) == 3);

    /*  dif::NumHorizInterval : number of intervlas in line */
    return (tu > 3 && t >= 2 && td > 3 || tu > 1 && t > 2 && td > 1 && tu + td
            > 3);
}

//***************************************************************************
//*   return       1 if stick glued to rusian D, otherwise 0                *
//***************************************************************************
static int16_t DiskrTsh(uchar *RASTR, int16_t D_X, int16_t dx, int16_t dy,
                        int16_t Dx)
{
    uchar j, n4 = dy >> 2, bool_foot = 1;
    uchar bit0, bit1, bit2;
    int16_t i, fine = 0;
    uchar *RAST;
    //****************   check existance two hooks  ******************

    for (RAST = RASTR + (dy - 3) * D_X, i = dy - 3;; i--, RAST -= D_X) {
        j = dif::SumIntervalBits(RAST, (int16_t) 0, (int16_t) dx) / 3;

        if ((j > 5 * D_X) || (i < 2 * n4))
            break;

        j = (uchar) dif::NumHorizInterval(RAST, D_X);

        if (j > 1)
            fine += 20;
    }

    if (fine < 30) {
        return (0);
    }

    //****************    foots configuration       ******************
    bit0 = dif::LOCAL[0];
    bit1 = dif::LOCAL[1];
    bit2 = dif::LOCAL[2];
    //************   check gluing in upper left angle gluing  *************
    fine = 0;

    for (RAST = RASTR, i = 0; i < n4; i++, RAST += D_X) {
        j = dif::SumIntervalBits(RAST, (int16_t) bit0, (int16_t) bit1) / 3;

        if (j > 3 * (bit1 - bit0) / 5)
            fine += 20;

        if (j == (bit1 - bit0))
            goto end;
    }

    if (fine < 40) {
        goto bbb;
    }//* breaking

    //******************  make histogramm   ********************************
    dif::FOOT_A(RASTR, D_X, (uchar) Dx, (uchar) n4);
    bool_foot = 0;

    for (i = bit0 + 1; i < bit1 + 1; i++) {
        if (dif::BUFFER[i] == 0)
            goto bbb;

        //* breaking
    }

    goto end;//* gluing
    //************   check gluing on upper right angle ************
bbb:
    fine = 0;

    for (RAST = RASTR, i = 0; i < n4; i++, RAST += D_X) {
        j = dif::SumIntervalBits(RAST, (int16_t) bit1, (int16_t) bit2) / 3;

        if (j > 3 * (bit2 - bit1) / 5)
            fine += 20;

        if (j == (bit2 - bit1)) {
            return (1);
        }
    }

    if (fine < 40)
        goto end;// breaking

    //******************  make histogramm  ********************************

    if (bool_foot)
        dif::FOOT_A(RASTR, D_X, (uchar) Dx, (uchar) n4);

    for (i = bit1 + 1; i < bit2 + 1; i++) {
        if (dif::BUFFER[i] == 0)
            goto end;

        // breaking
    }

    return (1);// gluing
end:
    return (0);
} // DiskrTsh


static int16_t average_br_angle(uchar *RASTER, int16_t D_X, int16_t dx,
                                int16_t dy, int16_t t)
{
    if (dif::av_br < 0)
        dif::av_br = average_angle(RASTER + D_X * (dy - (dy >> 2)), D_X, dx,
                              (int16_t) (dy >> 2), dif::RightDistance, t);

    return (dif::av_br);
}

static int16_t average_angle(uchar *RASTER, int16_t D_X, int16_t dx,
                             int16_t dy, int16_t(*Distance)(uchar *, int16_t), int16_t t)
{
    int16_t i = 0, n, p, H;

    if (t == 0) { /* cut long lines for finding corners */
        while (dif::SumBits(RASTER, (int16_t) (bytlen(dx))) > (dx >> 1) && i < dy) {
            i++;
            RASTER += D_X; /* black strings */
        }
    }

    for (H = n = 0; i < dy; i++, RASTER += D_X)
        if ((p = Distance(RASTER, (int16_t) (bytlen(dx)))) >= 0) {
            H++;
            n += p; /* nonzero string */
        }

    return (H ? n / H : -1);
}

static int16_t DiskrJu(uchar *RASTR, int16_t D_X, int16_t dx, int16_t Ly)
{
    uchar *RASTER = RASTR;
    int16_t i, ret, r, s2, s3, n;
    dx = bytlen(dx);

    for (n = s2 = s3 = i = 0; i < Ly; i++, RASTER += D_X, n++) {
        r = dif::NumHorizInterval(RASTER, dx);
        s2 += (r == 2);
        s3 += (r == 3);
    }

    // dif::NumHorizInterval : число интервалов в строке
    n = Ly / 3;
    ret = 0;

    if (s3 < n)
        ret += n - s3;

    if (!dif::broken_flag && s2 == 0)
        ret += 2;

    return (ret);
}

static int16_t DiskrimM1(uchar *RAST, int16_t D_X, int16_t dx, int16_t dy)
{
    int16_t n2, n4 = dy >> 2, meanBit = dx >> 1; /* Oleg & Vova 09.03.94 */
    int16_t i, j, k, up = 0, down = 0, cUp = 0, cDown = 0;
    int16_t prev = 0, byte = 0;
    uchar *r;

    /*  calculate  mean  of  hole  */
    for (r = RAST + D_X, i = 0; i < n4; i++, r += D_X) {
        j = dif::NumHorizInterval(r, D_X);

        if (j == 2) {
            if ((j = dif::NumHorizInterval(r + D_X, D_X)) == 2) {
                r += D_X;
            }

            for (k = 0; k < dx; k++) {
                byte = k >> 3;
                byte = *(r + byte);
                byte >>= (7 - (k % 8));
                byte &= 0x1;

                if (!byte && prev && !down)
                    down = k;

                if (byte && !prev && down)
                    up = k;

                prev = byte;
            }

            meanBit = (up + down + (dx >> 1)) / 3;

            /* for  no  wide  letters */
            if ((up - down < 5) || (dx < dy))
                meanBit = (up + down) / 2;

            break;
        }
    }

    /*  calculate  heigth  of  hole  */
    r = RAST + D_X;
    byte = 0;
    prev = whiteMeanBitLeft(r, D_X, meanBit);

    for (i = 1; i < dy - 1; i++, r += D_X) {
        j = whiteMeanBitLeft(r, D_X, meanBit);

        if ((j == 0) && (i > n4 + 1)) {
            break;
        }

        byte += (prev - j);

        /* for  'bl */
        if ((byte < 1) && (i > n4 + 2)) {
            break;
        }

        prev = j;
    }

    if (i > dy - 2)
        i = 2 * dy / 3;

    n4 = i >> 1;
    n2 = 2 * n4 ;
    /*  calculate  symetry  of  hole  */
    down = up = byte = 0;
    r = RAST + D_X;
    prev = whiteMeanBitLeft(r, D_X, meanBit);

    for (i = 1; i <= n2; i++, r += D_X) {
        j = whiteMeanBitLeft(r, D_X, meanBit);

        if (i <= n4) {
            up += j;
            cUp++;
        }

        else {
            down += j;
            cDown++;
        }

        /* too  wide  hole */
        if ((i > n4) && (j > dx / 5)) {
            if ((cUp == 0) || (cDown == 0))
                return (0);

            if (up * cDown <= down * cUp)
                return (4); /* bl - too  wide  hole */

            if (byte < 1)
                return (3); /* bl - too  wide  hole */
        }

        byte += (prev - j);

        if ((prev - j > 3) && (i > n4))
            return (5); /* bl - too  big  jump */

        if ((byte > 1 + dx / 8) && (i > n4))
            return (-6); /* M - too  big  angle  for  cursive */

        prev = j;
    }

    if ((cUp != cDown) && (cDown)) {
        down *= cUp;
        down /= cDown;
    }

    if (down >= up)
        return (5); /* bl - wide  hole on  the  bottom */

    if (down < up)
        return (-5); /* M - wide  hole on  the  top */

    if (!cDown)
        return (6); /* bl - hole is absent */

    return (0);
} /* DiskrM1 */

static int16_t DiskrimM(uchar *RAST, int16_t D_X, int16_t dx, int16_t dy)
{
    int16_t n2, n4 = dy >> 2, meanBit = dx >> 1; /* Oleg & Vova 09.03.94 */
    int16_t i, j, k, up = 0, down = 0, cUp = 0, cDown = 0;
    int16_t prev = 0, byte = 0;
    uchar *r;

    /*  calculate  mean  of  hole  */
    for (r = RAST + D_X, i = 0; i < n4; i++, r += D_X) {
        j = dif::NumHorizInterval(r, D_X);

        if (j == 2) {
            for (k = 0; k < dx; k++) {
                byte = k >> 3;
                byte = *(r + byte);
                byte >>= (7 - (k % 8));
                byte &= 0x1;

                if (!byte && prev && !down)
                    down = k;

                if (byte && !prev && down)
                    up = k;

                prev = byte;
            }

            meanBit = (up + down + (dx >> 1)) / 3;

            /* for  no  wide  letters */
            if ((up - down < 5) || (dx < dy))
                meanBit = (up + down) / 2;

            break;
        }
    }

    if (down == 0 && up == 0)
        return (2); /* M - hole is absent */

    /*  calculate  heigth  of  hole  */
    r = RAST + D_X;
    byte = 0;
    prev = whiteMeanBitLeft(r, D_X, meanBit) + whiteMeanBitRight(r, D_X, dx,
                                                                 meanBit);

    for (i = 1; i < dy - 1; i++, r += D_X) {
        j = whiteMeanBitLeft(r, D_X, meanBit) + whiteMeanBitRight(r, D_X, dx,
                                                                  meanBit);

        if ((j == 0) && (i > n4 + 1)) {
            break;
        }

        byte += (prev - j);

        /* for  'bl' */
        if ((byte < 2) && (i > n4 + 2)) {
            break;
        }

        prev = j;
    }

    if (i > dy - 2)
        i = 2 * dy / 3;

    n4 = i >> 1;
    n2 = 2 * n4;
    /*  calculate  symetry  of  hole  */
    down = up = byte = 0;
    r = RAST + D_X;
    prev = whiteMeanBitLeft(r, D_X, meanBit) + whiteMeanBitRight(r, D_X, dx,
                                                                 meanBit);

    for (i = 1; i <= n2; i++, r += D_X) {
        j = whiteMeanBitLeft(r, D_X, meanBit) + whiteMeanBitRight(r, D_X, dx,
                                                                  meanBit);

        if (i <= n4) {
            up += j;
            cUp++;
        }

        else {
            down += j;
            cDown++;
        }

        /* too  wide  hole */
        if ((i > n4) && (j > dx / 3)) {
            if ((cUp == 0) || (cDown == 0))
                return (0);

            if (up * cDown <= down * cUp)
                return (4); /* bl - too  wide  hole */

            if (byte < 3)
                return (3); /* bl - too  wide  hole */
        }

        byte += (prev - j);

        if ((prev - j > 3) && (i > n4))
            return (5); /* bl - too  big  jump */

        if ((byte >= dx / 4) && (i > n4))
            return (-6); /* M - too  big  angle  for  cursive */

        prev = j;
    }

    if ((cUp != cDown) && (cDown)) {
        down *= cUp;
        down /= cDown;
    }

    if (down >= up)
        return (5); /* bl - wide  hole on  the  bottom */

    if (down < up)
        return (-5); /* M - wide  hole on  the  top */

    if (!cDown)
        return (6); /* bl - hole is absent */

    return (0);
} /* DiskrM */

static int16_t whiteMeanBitLeft(uchar *RAST, int16_t D_X, int16_t meanBit)
{
    int16_t meanByte, byte;
    int16_t i, counter = 0;

    for (i = meanBit; i >= 0; i--) {
        meanByte = i >> 3;
        byte = *(RAST + meanByte);
        byte >>= (7 - (i % 8));
        byte &= 0x1;

        if (byte) {
            meanByte = (i - 1) >> 3;
            byte = *(RAST + meanByte);
            byte >>= (7 - ((i - 1) % 8));
            byte &= 0x1;

            if (byte)
                break;

            else {
                meanByte = i >> 3;
                byte = *(RAST + D_X + meanByte);
                byte >>= (7 - (i % 8));
                byte &= 0x1;

                if (byte)
                    break;

                else {
                    meanByte = i >> 3;
                    byte = *(RAST - D_X + meanByte);
                    byte >>= (7 - (i % 8));
                    byte &= 0x1;

                    if (byte)
                        break;

                    else
                        counter++;
                }
            }
        }

        else
            counter++;
    }

    return (counter);
} /* whiteMeanBitLeft */

static int16_t whiteMeanBitRight(uchar *RAST, int16_t D_X, int16_t dx,
                                 int16_t meanBit)
{
    int16_t meanByte, byte;
    int16_t i, counter = 0;

    for (i = meanBit + 1; i < dx; i++) {
        meanByte = i >> 3;
        byte = *(RAST + meanByte);
        byte >>= (7 - (i % 8));
        byte &= 0x1;

        if (byte) {
            meanByte = (i + 1) >> 3;
            byte = *(RAST + meanByte);
            byte >>= (7 - ((i + 1) % 8));
            byte &= 0x1;

            if (byte)
                break;

            else {
                meanByte = i >> 3;
                byte = *(RAST + D_X + meanByte);
                byte >>= (7 - (i % 8));
                byte &= 0x1;

                if (byte)
                    break;

                else {
                    meanByte = i >> 3;
                    byte = *(RAST - D_X + meanByte);
                    byte >>= (7 - (i % 8));
                    byte &= 0x1;

                    if (byte)
                        break;

                    else
                        counter++;
                }
            }
        }

        else
            counter++;
    }

    return (counter);
} /* whiteMeanBitRight */

// check gluing roof and down zone for russian capital & small II
int16_t up_down_zones(uchar *raster, int16_t D_X, int16_t dx, int16_t dx0,
                      int16_t start1, int16_t stop1, int16_t start2, int16_t stop2)
{
    int16_t i, num1, num2, l = dx0 - (dx0 >> 3);
    uchar *r = raster;

    if (dif::up_down_serif >= 0)
        return (dif::up_down_serif);

    l = std::min((int) l, dx0 - 2);
    dx = bytlen(dx);

    for (r = raster + start1 * D_X, num1 = 0, i = start1; i < stop1; i++, r
            += D_X) {
        num1 += (dif::NumHorizInterval(r, dx) == 1 && dif::SumBits(r, dx) >= l);
        /*  num1 : число строк с одним длинным интервалом на крыше */
    }

    for (r = raster + start2 * D_X, num2 = 0, i = start2; i < stop2; i++, r
            += D_X) {
        num2 += (dif::NumHorizInterval(r, dx) == 1 && dif::SumBits(r, dx) >= l);
        /*  num2 : число строк с одним длинным интервалом внизу */
    }

    if (num1 == 0 && num2 >= 1) /* нижние серифы слиплись, а крыша разорвана */
        return ((dif::up_down_serif = 2));

    if (num1 == 0 && num2 == 0) /* две палки */
        return ((dif::up_down_serif = 1));

    return ((dif::up_down_serif = 0));
}

/* for letters 'C','c','e' */
static int16_t DiskrVertCE(uchar *RASTR, int16_t D_X, int16_t dx, int16_t dy,
                           int16_t X, uchar let, uchar inc)
{
    uchar *RASTER = RASTR;
    int16_t i, p, n, s3, d = dx >> 2, wid = bytlen(dx);
    int16_t ody = dy >> 2;
    uchar *r = RASTR + ody * D_X;
    int16_t t1, t2, t3, num1, num2, num3, minnum, n2;
    int16_t l = dy - (ody << 1), num, z = dx - (dx >> 3);
    int16_t nn[7];

    if (dif::c_or_e < 0) {
        dif::d_c = 0;

        /* горизонтальные исследования  */
        for (minnum = dx, n2 = num = num1 = num2 = num3 = 0, i = ody; i < l; i++, r
                += D_X) {
            t1 = dif::NumHorizInterval(r, wid);
            t2 = dif::SumBits(r, wid);

            if (t1 == 2) {
                t3 = (wid << 3) - dif::LeftDistance(r, wid) - dif::RightDistance(r, wid)
                     - t2;

                if (n2 > 0) {
                    if (dx > 10 && t3 < 3)
                        num3++;

                    if (minnum > t3)
                        minnum = t3;
                }

                n2++;
            }

            num += (t1 == 1 && t2 >= z);
            num1 += (t1 == 1 && t2 >= z - 1);
            num2 += (t2 >= z);
        }

        /*  num : число строк с одним длинным интервалом */
        if (num > 1 || num1 > 2 || num2 > 3 || num3 > 1) { /* есть непрерывная перекладина */
            dif::c_or_e = 1; /* или набор залитых строк       */
            dif::d_e = 0;
            dif::d_c = num;

            if (num3)
                dif::d_c += (minnum == 1 ? 4 : 3);

            return ((let == (uchar) '\xa5' /* е */) ? dif::d_e : dif::d_c);
        }

        if (num == 0 && num1 == 0 && num2 == 0 && dy < 24) { /* ищу прыщ напротив конца правого рога */
            r = RASTR + ody * D_X;
            t2 = dx >> 1;

            for (i = ody; i < l; i++, r += D_X) {
                num1 = dif::SumIntervalBits(r, t2, dx);

                if (i > ody && num != 0 && num1 == 0)
                    break;

                num = num1;
            }

            i -= 4;
            r = RASTR + i * D_X;

            for (t3 = t2 = 0; t2 < 7; t2++, i++, r += D_X) {
                nn[t2] = dif::EndBlackInterval(r, wid);
                t3 += (dif::NumHorizInterval(r, wid) == 2);
            }

            for (t1 = t2 = nn[0], num = 0, num1 = 1; num1 < 7; num1++) {
                if (t1 > nn[num1] && nn[num1] >= 0)
                    t1 = nn[num1];

                if (t2 < nn[num1])
                    t2 = nn[(num = num1)];

                if (t2 == nn[num1] && num == 0)
                    num = num1;
            }

            for (num1 = num - 1; num1 >= 0; num1--)
                if (nn[num1] < t2)
                    break;

            for (num2 = num + 1; num2 < 7; num2++)
                if (nn[num2] < t2)
                    break;

            if (inc && t2 - t1 < 3)
                t2 = t1; // Oleg : ERECTION conditions : 09-08-95 09:47pm

            if (t3 > 2 && t2 > t1 && num1 >= 0 && num2 < 7) { /* вот прыщик */
                dif::c_or_e = 1; /* или набор залитых строк       */
                dif::d_e = 0;
                dif::d_c = 1 + t2 - t1;

                if ((let == (uchar) '\xa5' /* е */))
                    return (dif::d_e);
            }
        }

        /* далее вертикальные исследования  */
        dx -= d;
        X &= 7;

        for (n = s3 = 0, i = d; i < dx; i++) {
            p = dif::NumVertInterval(RASTER, D_X, dy, (int16_t) (i + X));
            s3 += (p == 3);

            if (p == 3 || p == 2)
                n++;
        }

        /*  dif::NumVertInterval : число линий в столбце */
        p = n;
        n *= 4;
        n /= 10; /* 40 % */
        p -= n; /* 60 % */
        dif::c_or_e = 1;
        dif::d_e = (s3 < p) ? p - s3 : 0;

        if (s3 == 0 && dif::d_e < 3)
            dif::d_e = 4;

        dif::d_c = std::max((int) dif::d_c, (s3 > n) ? s3 - n + 1 : 0);
    }

    return ((let == (uchar) '\xa5' /* е */) ? dif::d_e : dif::d_c);
}

int16_t AngleBottomRight(uchar *raster, int16_t D_X, int16_t hei)
{
    int i, old, neue, inc;
    uchar *r;
    raster += D_X * (hei - 2);
    hei >>= 2;
    old = dif::RightDistance(raster, D_X);

    for (inc = 0, r = raster - D_X, i = 1; i < hei; i++, r -= D_X) {
        neue = dif::RightDistance(r, D_X);

        if (neue < old)
            inc++;

        if (neue > old)
            break;

        old = neue;
    }

    return inc;
}

int16_t AngleTopRight(uchar *raster, int16_t D_X, int16_t hei)
{
    int i, old, neue, inc;
    uchar *r;
    raster += D_X;
    hei >>= 2;
    old = dif::RightDistance(raster, D_X);

    for (inc = 0, r = raster + D_X, i = 1; i < hei; i++, r += D_X) {
        neue = dif::RightDistance(r, D_X);

        if (neue < old)
            inc++;

        if (neue > old)
            break;

        old = neue;
    }

    return inc;
}

int16_t BonusAnglesCurve(uchar *raster, int16_t D_X, int16_t hei)
{
    int pen = 0, inc;
    inc = AngleTopRight(raster, D_X, hei);

    if (inc > 3 || (hei < 23 && inc > 2))
        pen++;

    inc = AngleBottomRight(raster, D_X, hei);

    if (inc > 3 || (hei < 23 && inc > 2))
        pen++;

    return pen;
}

Bool32 DiskrJuCut(int16_t nfoot, int16_t dx)
{
    int i, l, d;
    l = dx * 3 / 4;
    d = dx / 5;

    for (i = 0; i < nfoot; i++)
        if (dif::LOCAL[i] > l && dif::LOCAL_W[i] > d)
            return TRUE;

    return FALSE;
}

int16_t DIF_GetNoCutPoint(uchar *RASTER, int16_t Wx, uchar NWIDTH,
                          uchar NLENGTH)
{
    int16_t f = FOOT_HEI(RASTER, Wx, NWIDTH, NLENGTH);

    if (f != 2 || f == 2 && (dif::LOCAL[0] - (dif::LOCAL_W[0] + 1) / 2)
            > (dif::LOCAL_W[0] + 1) / 2)
        return 0;

    return dif::LOCAL[1] + (dif::LOCAL_W[1] + 1) / 2;
}
