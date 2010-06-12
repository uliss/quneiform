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

#define TIGER_CORR       //alik
#include "lst3_win.h"
#include <stdlib.h>

#ifndef WIN_MOD
#define LST_WWIN
#endif
#include "ful_txt.h"
#include "globus.h"
#include "aldebug.h"
#include "crtffunc.h"
#include "consmess.h"

/* These were in ful_txt.h. That caused missing symbols
 * so I moved them here. JussiP.
 */

//---Из секции колонок---
int16_t NumCol;
int16_t *NumStr;//[nc]
uint32_t *UserNumber;//[nc]
uint32_t *FragFlag;//[nc]
SRECT *BndCol;//[nc]
STAT_COL *StatCol;//[nc]
int k_col[MAX_LEV + 1], k_lev;
//---Из секции знакомест---
ZN ****Zn;//[nc][ns][nw][nz]
TITLE_STR **TitleStr;//[nc][ns]
TITLE_WORD ***TitleWord;//[nc][ns][nw]
//--FullSpellFull
PAR_FUL par_ful;
//--КОРРЕКТОР ПО ОДНОРОДНОСТИ
FEAT_LET FeatLet[256];
//---WORK---
uchar buf[1024];
/* End of things brought from ful_txt.h. */

/* Rectangles are written in frmtfile.cpp and they contain 16-bit integers.
 * They are processed as 32-bit integers here, so we need to convert.
 */

extern int dets;

static void readSRECT(SRECT *r, FILE *f) {
    Rect16 r16;
    fread(&r16, sizeof(Rect16), 1, f);
    r->left = r16.left;
    r->top = r16.top;
    r->right = r16.right;
    r->bottom = r16.bottom;
}

#ifdef SUB_ZN
SUB_ALLOC SubZn;
#endif

#ifdef TIGER_CORR
int CalcStatTiger(void);

//Параметры:
//FileNameFul - имя файла Full Out Tiger
//FileNameOut - имя выходного файла
//Return: 0 - OK
//
uint16_t NumZ, NumW, NumS;
int16_t SizeYGlobUpp;
int IsB1(uchar a) {
    if ((FeatLet[a].Chif && a != ',' && a != '.' && a != '-') || (FeatLet[a].Let && a != '-' && a
            != '\'' && (FeatLet[a].Shift == UPP || FeatLet[a].PosUpLine == UPP)))
        return 1;

    return 0;
}
int IsB2(uchar a) {
    if ((FeatLet[a].Let && a != '-' && a != '\'' && (FeatLet[a].Shift == LOW
            && FeatLet[a].PosUpLine == LOW)))
        return 1;

    return 0;
}

//==Расчет статистики
int CalcStatTiger() {
    int numL, numU, maxL = 1000, maxU = 300, *Upp, *Low, med, mod, sig, ave, nc, ns, nw, nz;
    int numB1, *arrB1, maxB1 = 30;
    int ThrDif_b1b2;
    uchar a;
    Upp = (int*) malloc(maxU * sizeof(int));
    Low = (int*) malloc(maxL * sizeof(int));
    arrB1 = (int*) malloc(maxB1 * sizeof(int));

    if (Upp == NULL || Low == NULL || arrB1 == NULL)
        return NOT_ALLOC;

    // -- Расчет глобал.размеров с учетом OCR --
    numL = numU = 0;

    for (nc = 0; nc <= NumCol; ++nc)
        for (ns = 0; ns <= NumStr[nc]; ++ns)
            for (nw = 0; nw < TitleStr[nc][ns].S_Gen.S_NumWord; ++nw)
                for (nz = 0; nz < TitleWord[nc][ns][nw].W_Gen.W_NumSym; ++nz) {
                    ZN *z = &Zn[nc][ns][nw][nz];
                    SRECT *r = &z->Title.Z_Rect;

                    if (a = Get1Alt(z, 0)) { //распознанно
                        if (IsB1(a)) {
                            if (numU >= maxU) {
                                maxU *= 2;

                                if ((Upp = (int*) realloc(Upp, maxU * sizeof(int))) == NULL)
                                    return NOT_ALLOC;
                            }

                            Upp[numU++] = r->bottom - r->top;
                        }

                        if (IsB2(a)) {
                            if (numL >= maxL) {
                                maxL *= 2;

                                if ((Low = (int*) realloc(Low, maxL * sizeof(int))) == NULL)
                                    return NOT_ALLOC;
                            }

                            Low[numL++] = r->bottom - r->top;
                        }
                    }
                }

    sig = 0;
    SizeYGlob = SizeYGlobUpp = INDEF;

    if (numU > 2)
        Statist(Upp, numU, &ave, &sig, (int*) &SizeYGlobUpp, &mod, 0);

    if (numL > 10)
        Statist(Low, numL, &ave, &sig, &SizeYGlob, &mod, 0);
    else { //Расчет глобал.размеров без учета OCR
        numL = numU = 0;

        for (nc = 0; nc <= NumCol; ++nc)
            for (ns = 0; ns <= NumStr[nc]; ++ns)
                for (nw = 0; nw < TitleStr[nc][ns].S_Gen.S_NumWord; ++nw)
                    for (nz = 0; nz < TitleWord[nc][ns][nw].W_Gen.W_NumSym; ++nz) {
                        ZN *z = &Zn[nc][ns][nw][nz];
                        SRECT *r = &z->Title.Z_Rect;

                        if (numL >= maxL) {
                            maxL *= 2;

                            if ((Low = (int*) realloc(Low, maxL * sizeof(int))) == NULL)
                                return NOT_ALLOC;
                        }

                        Low[numL++] = r->bottom - r->top;
                    }

        if (numL > 0) //ALIK 01.05.99
            Statist(Low, numL, &ave, &sig, &SizeYGlob, &mod, 0);
        else
            return -1;
    }

    // -- расчет колонных размеров --
    if (NumCol) {
        for (nc = 0; nc <= NumCol; ++nc) {
            numL = numU = 0;

            for (ns = 0; ns <= NumStr[nc]; ++ns)
                for (nw = 0; nw < TitleStr[nc][ns].S_Gen.S_NumWord; ++nw)
                    for (nz = 0; nz < TitleWord[nc][ns][nw].W_Gen.W_NumSym; ++nz) {
                        ZN *z = &Zn[nc][ns][nw][nz];
                        SRECT *r = &z->Title.Z_Rect;

                        if (a = Get1Alt(z, 0)) {
                            if (IsB1(a)) {
                                if (numU >= maxU) {
                                    maxU *= 2;

                                    if ((Upp = (int*) realloc(Upp, maxU * sizeof(int))) == NULL)
                                        return NOT_ALLOC;
                                }

                                Upp[numU++] = r->bottom - r->top;
                            }

                            if (IsB2(a)) {
                                if (numL >= maxL) {
                                    int MaxOld = maxL * sizeof(int);
                                    maxL *= 2;

                                    if ((Low = (int*) realloc(Low, maxL * sizeof(int))) == NULL)
                                        return NOT_ALLOC;
                                }

                                Low[numL++] = r->bottom - r->top;
                            }
                        }
                    }

            sig = 0;
            StatCol[nc].dy_Upp = SizeYGlobUpp;

            if (numU > 2)
                Statist(Upp, numU, &ave, &sig, &StatCol[nc].dy_Upp, &mod, 0);
            else
                StatCol[nc].dy_Upp = SizeYGlobUpp;

            if (numL > 10) {
                med = 1;
                Statist(Low, numL, &ave, &sig, &med, &mod, 0);
                StatCol[nc].dy_col = StatCol[nc].dy_Low = med;
            } else
                StatCol[nc].dy_col = StatCol[nc].dy_Low = SizeYGlob;
        }
    } else {
        StatCol[0].dy_col = StatCol[0].dy_Low = SizeYGlob;
        StatCol[0].dy_Upp = SizeYGlobUpp;
    }

    // -- расчет строчных размеров малых букв и ревизия линейки b2 --
    for (nc = 0; nc <= NumCol; ++nc) {
        int Dif_b1b2;//признак различия линеек b1 и b2
        ThrDif_b1b2 = NORM_SCAN(3);
        ThrDif_b1b2 = MAX(ThrDif_b1b2,
                (StatCol[nc].dy_Upp - StatCol[nc].dy_Low) / 2);

        if ((StatCol[nc].stat_str = (STAT_STR*) malloc((NumStr[nc] + 1) * sizeof(STAT_STR)))
                == NULL)
            return NOT_ALLOC;

        for (ns = 0; ns <= NumStr[nc]; ++ns) {
            SRECT *rS = &TitleStr[nc][ns].S_Rect;
            numL = numU = numB1 = 0;
            Dif_b1b2 = (rS->top - rS->left > ThrDif_b1b2);

            for (nw = 0; nw < TitleStr[nc][ns].S_Gen.S_NumWord; ++nw)
                for (nz = 0; nz < TitleWord[nc][ns][nw].W_Gen.W_NumSym; ++nz) {
                    ZN *z = &Zn[nc][ns][nw][nz];
                    SRECT *r = &z->Title.Z_Rect;

                    if (a = Get1Alt(z, 0)) {
                        if (IsB2(a)) {
                            if (numL >= maxL) {
                                maxL *= 2;

                                if ((Low = (int*) realloc(Low, maxL * sizeof(int))) == NULL)
                                    return NOT_ALLOC;
                            }

                            Low[numL++] = r->bottom - r->top;

                            //ревизуем ли те b2, кот. отличны от b1
                            if (Dif_b1b2 && !FeatLet[a].ImUppLow) {
                                TITLE_ZN *t = &z->Title;
                                int na = t->Z_Num_Alt, flp, i;
                                flp = TRUE;

                                for (i = 1; i < na; ++i)
                                    if (!IsB2(a = Get1Alt(z, i)) || FeatLet[a].ImUppLow) {
                                        flp = FALSE;
                                        break;
                                    }

                                if (flp && abs(r->top - rS->left) < abs(r->top - rS->top)) {
                                    if (numU >= maxU) {
                                        maxU *= 2;

                                        if ((Upp = (int*) realloc(Upp, maxU * sizeof(int))) == NULL)
                                            return NOT_ALLOC;
                                    }

                                    Upp[numU++] = r->top;
                                }
                            }
                        }

                        //==ревизия b1 делаем лишь по первой альтер.,т.к. заглав.букв мало==
                        if (IsB1(a) && (!FeatLet[a].ImUppLow || FeatLet[a].Chif) && abs(rS->left
                                - r->top) > 1) {
                            if (numB1 >= maxB1) {
                                maxB1 *= 2;

                                if ((arrB1 = (int*) realloc(arrB1, maxB1 * sizeof(int))) == NULL)
                                    return NOT_ALLOC;
                            }

                            arrB1[numB1++] = r->top;
                        }
                    }
                }

            sig = 0;

            if (numL > 5) {
                med = 1;
                Statist(Low, numL, &ave, &sig, &med, &mod, 0);
                StatCol[nc].stat_str[ns].dy = med;
            } else
                StatCol[nc].stat_str[ns].dy = StatCol[nc].dy_Low;

            if (numU) { //ревизия линейки b2
                med = 1;
                Statist(Upp, numU, &ave, &sig, &med, &mod, 0);
                rS->top = med;
            }

            if (numB1) { //ревизия линейки b2
                med = 1;
                Statist(arrB1, numB1, &ave, &sig, &med, &mod, 0);
                rS->left = med;//?или только если med < old b1?
            }
        }
    }

    free(Upp);
    free(Low);
    free(arrB1);
    return 0;
}

//Формат файла:
//int16_t ScanResolution
//int16_t NumCol,NumZ,NumW,NumS - числа колонок, знакомест, слов и строк in all page
//int16_t MonoSpaceAllPage - distance between left margin letters-neighdoors from one word
//int16_t HeiStrAllPage - distance between top margin
//Fragm[0], ..., Fragm[NumCol-1] фрагменты
//  Fragm[nf]: один фрагмент
//    Rect16 RectFragm
//    int16_t  NumStr число строк
//    Str[0], ..., Str[NumStr-1] строки
//      Str[nf][ns]:
//        Rect16 S_Rect // 4 base lines
//        int16_t  NumWordStr; число слов текущей строки
//        Word[0], ..., Word[NumWordStr-1] слова
//          Word[nf][ns][nw]: одно слово
//            int16_t NumSym число знакомест слова (пробелов нет)
//            Sym[0], ..., Sym[NumSym-1] знакоместа
//              Sym[nf][ns][nw][nz]:
//                Rect16 Z_Rect     рамка знакоместа
//                int16_t  NumAlt     число альтернатив
//                Alt[0], ..., Alt[NumAlt-1] альтернативы
//                  Alt[nf][ns][nw][nz][na] одна альтернатива
//                    uchar Code код
//                    uchar Prob  вероятность
//                    uchar Spell check end string for -
//                    uchar Base  base number чтобы задавать шрифты для таблицы шрифтов RTF (семейство + имя)

// -------  paragraphs mark --------
//if(BEG_PARAG1(Zn[nc][ns][0][0].Title.Z_Id) == 1 ||
//  (Zn[nc][ns][0][0].Title.Z_Num_Alt==1 &&
//   Zn[nc][ns][0][0].Alt[0].a_Code=='#') )
//     ++k_parag[nc];


extern Rect16 *RectFragm;

short OpenFullOutTiger(FILE *in) {
    static int HeiStrAllPage;
    static int MonoSpaceAllPage;
    int nc, ns, nw, nz, k_word, k_z, i;
#ifdef alDebug

    if (dets) {
        ConsMess("OpenFullOutTiger Begin ");
    }

#endif
    rewind(in);
    fread(&ScanResolution, sizeof(uint16_t), 1, in);
    fread(&NumCol, sizeof(uint16_t), 1, in);
    fread(&NumZ, sizeof(uint16_t), 1, in);
    fread(&NumW, sizeof(uint16_t), 1, in);
    fread(&NumS, sizeof(uint16_t), 1, in);
#ifdef alDebug

    if (dets) {
        ConsMess("OpenFullOutTiger ScanResolution=%d ", ScanResolution);
    }

    if (dets) {
        ConsMess("OpenFullOutTiger NumCol=%d ", NumCol);
    }

    if (dets) {
        ConsMess("OpenFullOutTiger NumZ=%d ", NumZ);
    }

    if (dets) {
        ConsMess("OpenFullOutTiger NumW=%d ", NumW);
    }

    if (dets) {
        ConsMess("OpenFullOutTiger NumS=%d ", NumS);
    }

#endif
    fread(&MonoSpaceAllPage, 2, 1, in);
    fread(&HeiStrAllPage, 2, 1, in);

    // Twips = (float)((int)(Twips+0.5));
    if (NumCol) {
        NumStr = (int16_t*) malloc(NumCol * sizeof(int16_t));
        StatCol = (STAT_COL*) malloc(NumCol * sizeof(STAT_COL));

        if (NumStr == NULL || StatCol == NULL) {
#ifdef alDebug

            if (dets) {
                ConsMess("OpenFullOutTiger Memory str2367 Numstr,StatCol");
            }

#endif
            goto BadReturn;
        }
    } else {
        NumStr = NULL;
        StatCol = NULL;
        goto BadReturn;
    }

    BndCol = (SRECT*) malloc(NumCol * sizeof(SRECT));
    UserNumber = (uint32_t*) malloc(NumCol * sizeof(uint32_t));
    FragFlag = (uint32_t*) malloc(NumCol * sizeof(uint32_t));
    RectFragm = (Rect16*) malloc(NumCol * sizeof(Rect16));

    if (BndCol == NULL || UserNumber == NULL || RectFragm == NULL) {
#ifdef alDebug

        if (dets) {
            ConsMess("OpenFullOutTiger Memory str2385 BndCol,UserNumber,RectFragm");
        }

#endif
        goto BadReturn;
    }

    // *********** РАССЧИТАТЬ slope,SizeXGlob,SizeYGlob + OneColStat *********
    {
        int fl; //---Calculate Common Size Section Zn---
        long lenZn, lenWord, lenStr, NumT = NumCol;

        if (NumT + NumS + NumW) {
            lenZn = (NumT + NumS + NumW) * sizeof(PTR) + NumZ * sizeof(ZN);
            lenWord = (NumT + NumS) * sizeof(PTR) + NumW * sizeof(TITLE_WORD);
            lenStr = NumT * sizeof(PTR) + NumS * sizeof(TITLE_STR);

            if ((fl = InitSubAlloc(2 * (lenZn + lenWord + lenStr) + 10000, &SubZn))) {
#ifdef alDebug

                if (dets) {
                    ConsMess("OpenFullOutTiger str2400 InitSubAlloc");
                }

#endif
                goto BadReturn;
            }
        }
    }

    if (NumCol) {
        Zn = (ZN****) Submalloc(NumCol * sizeof(ZN***), &SubZn);
        TitleStr = (TITLE_STR**) Submalloc(NumCol * sizeof(TITLE_STR*), &SubZn);
        TitleWord = (TITLE_WORD***) Submalloc(NumCol * sizeof(TITLE_WORD**), &SubZn);

        if (Zn == NULL || TitleStr == NULL || TitleWord == NULL) {
#ifdef alDebug

            if (dets) {
                ConsMess("OpenFullOutTiger Memory str2413 Zn,TitleStr,TitleWord");
            }

#endif
            goto BadReturn;
        }
    }

    for (nc = 0; nc < NumCol; ++nc) {
        fread(&RectFragm[nc], 1, sizeof(Rect16), in);
        // *********** РАСЧЕТ КОЛОННОЙ СТАТИСТИКИ *************
        fread(&NumStr[nc], sizeof(int16_t), 1, in);
        fread(&UserNumber[nc], sizeof(uint32_t), 1, in);
        fread(&FragFlag[nc], sizeof(uint32_t), 1, in);
        Zn[nc] = (ZN***) Submalloc((NumStr[nc]) * sizeof(ZN**), &SubZn);
        TitleStr[nc] = (TITLE_STR*) Submalloc((NumStr[nc]) * sizeof(TITLE_STR), &SubZn);
        TitleWord[nc] = (TITLE_WORD**) Submalloc((NumStr[nc]) * sizeof(TITLE_WORD*), &SubZn);

        if (Zn[nc] == NULL || TitleStr[nc] == NULL || TitleWord[nc] == NULL)
            goto BadReturn;

        --NumStr[nc];

        for (ns = 0; ns <= NumStr[nc]; ++ns) {
            TITLE_STR *t = &TitleStr[nc][ns];
            int16_t tmp;
            t->Z_Code = 2;
            t->S_Attr = 0;
            // 4 base lines,?
            readSRECT(&t->S_Rect, in);
            //exchange : top,bottom - standard, left,right - extern
            tmp = t->S_Rect.right;
            t->S_Rect.right = t->S_Rect.bottom;
            t->S_Rect.bottom = tmp;
            //Реальные коор. строки!
            readSRECT(&t->S_Real_Rect, in);
            fread(&tmp, 2, 1, in);
            t->S_Gen.S_NumWord = tmp; // NumWrd
            fread(&t->S_Flags, sizeof(t->S_Flags), 1, in); //NEGA_STR
            t->S_Gen.HeadLine = 0; // zero HeadLine
            k_word = TitleStr[nc][ns].S_Gen.S_NumWord - 1;
            Zn[nc][ns] = (ZN**) Submalloc((k_word + 1) * sizeof(ZN*), &SubZn);
            TitleWord[nc][ns] = (TITLE_WORD*) Submalloc((k_word + 1) * sizeof(TITLE_WORD), &SubZn);

            if (Zn[nc][ns] == NULL || TitleWord[nc][ns] == NULL)
                goto BadReturn;

            for (nw = 0; nw <= k_word; ++nw) {
                TITLE_WORD *tw = &TitleWord[nc][ns][nw];
                tw->Z_Code = 1;
                fread(&tmp, sizeof(int16_t), 1, in);
                tw->W_Gen.W_NumSym = tmp;// NumZn
                k_z = tw->W_Gen.W_NumSym - 1;
                fread(&tmp, sizeof(int16_t), 1, in);
                tw->W_Gen.FontNumber = (uint16_t) tmp;
                fread(&tmp, sizeof(int16_t), 1, in);
                tw->W_Gen.FontSize = (uint16_t) tmp;

                if ((Zn[nc][ns][nw] = (ZN*) Submalloc((k_z + 1) * sizeof(ZN), &SubZn)) == NULL)
                    goto BadReturn;

                for (nz = 0; nz <= k_z; ++nz) {
                    ZN *z = &Zn[nc][ns][nw][nz];
                    TITLE_ZN *tz = &z->Title;
                    int16_t num;
#pragma pack(1)
                    struct ALT_TIGER1
                    {
                            unsigned char let, prob;
                    } alt1;
                    struct ALT_TIGER2
                    {
                            unsigned char language, spellnocarrying, FlagCapDrop, spell, base;
                    } alt2;
#pragma pack()
                    // BOX
                    readSRECT(&tz->Z_Rect, in);
                    // Real BOX
                    readSRECT(&tz->Z_RealRect, in);
                    fread(&num, sizeof(int16_t), 1, in);
                    tz->Z_Num_Alt = (uchar) MIN(num, REC_MAX_VERS); //NumAlt
                    //                  if(num > 1)
                    //                      num = 1;
                    tz->Z_Code = 0;
                    tz->Z_Id.col = nc + 1;
                    tz->Z_Id.str = ns + 1;
                    tz->Z_Id.comp = nz + 1;
                    tz->Z_Id.word = 0;

                    for (i = 0; i < num; i++) {
                        fread(&alt1, sizeof(struct ALT_TIGER1), 1, in);

                        if (i < REC_MAX_VERS) {
                            ALT_ZN *Alt = &z->Alt[i];
                            Alt->a_Code = alt1.let;
                            Alt->a_Prob = alt1.prob;
                            Alt->a_Dist = (float) (alt1.prob / 256.);
                        }
                    }

                    fread(&alt2, sizeof(struct ALT_TIGER2), 1, in);

                    for (i = 0; i < tz->Z_Num_Alt; i++) {
                        ALT_ZN *Alt = &z->Alt[i];
                        Alt->a_Base = alt2.base;
                        Alt->a_language = alt2.language;
                        Alt->a_SpellNoCarrying = alt2.spellnocarrying;
                        Alt->a_FlagCupDrop = alt2.FlagCapDrop;
                        Alt->a_Spell = alt2.spell;
                    }
                }
            }
        }
    }

    --NumCol;
#ifdef alDebug

    if (dets) {
        ConsMess("OpenFullOutTiger  End");
    }

#endif
    return TRUE;
    BadReturn:
#ifdef alDebug

    if (dets) {
        ConsMess("OpenFullOutTiger  End");
    }

#endif
    return FALSE;
}

#endif

//Освобождение памяти, захваченной под секции знакомест и колонок .ful-файла
int FreeStructFull() {
    int kw = 0, nc, ns = 0, nw = 0, sp = 0, num = 0, i = 0;
    const char *err = "FreeStructFull";
#ifdef SUB_ZN
    do0(nc, 0, NumCol) {
        if (StatCol[nc].stat_str)
            free(StatCol[nc].stat_str);
    }
    free(StatCol);
    DeleteSubAlloc(&SubZn);
#else
    //---Секция знакомест---
    do0(nc, 0, NumCol) {
        if (StatCol[nc].stat_str) free(StatCol[nc].stat_str);

        do0(ns, 0, NumStr[nc]) {
            kw = TitleStr[nc][ns].S_Gen.S_NumWord - 1;
            do0(nw, 0, kw) {
                free(Zn[nc][ns][nw]);

                if ((sp = TitleWord[nc][ns][nw].W_Gen.W_Spell) == SPELL_CORR) {
#ifndef FUL
                    Error(1, err);
#endif

                    if ((num = TitleWord[nc][ns][nw].NumAltSpell) <= 0) Error(1, err);

                    do0(i, 0, num - 1) free(TitleWord[nc][ns][nw].AltSpell[i].Alt);
                    free(TitleWord[nc][ns][nw].AltSpell);
                }
            }
            free(Zn[nc][ns]);
            free(TitleWord[nc][ns]);
        }
        free(Zn[nc]);
        free(TitleWord[nc]);
        free(TitleStr[nc]);
    }
    free(Zn);
    free(TitleWord);
    free(TitleStr);
#endif
#ifdef CMP
    free(CoorComp);
#endif
#ifndef PROF_COL
    free(BndCol);
#else
    //---Секция колонок---
    free(BndCol);
    free(NumStr);
    free(StatCol);
#ifndef TIGER_CORR
    {   int nlev; //Освобождение памяти под иерархию колонок
        do0(nlev, 0, k_lev) {
            do0(nc, 0, k_col[nlev])

            if (knot[nlev][nc].kp >= 0)
            free(knot[nlev][nc].Addr);

            free(knot[nlev]);
        }
        free(knot);
    }
#endif
#endif //PROF_COL
    //---Секция имен баз---
#ifndef TIGER_CORR
    free(FntName);
#endif
    return 0;
}

#ifndef BLANK
#define SPACE_RED 4
#else
#define SPACE_RED 0
#endif

