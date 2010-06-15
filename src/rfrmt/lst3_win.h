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

#include <sys/types.h>

#include "cfcompat.h"
#include "minmax.h"

#define OLD_COOR

const int PLAIN = 0;

struct h_frame
{
        unsigned long start_pos, end_pos;
        int left, up, right, down;
};

typedef h_frame FRAME;
typedef int TYPE;/*тип расстояний между FRAME*/

typedef int (*FUN_POMP)(void);
typedef void (*FUN_MESS)(uint16_t wPar, uint32_t lPar);
typedef int(*COMP_FUN)(void);
typedef TYPE (*DistFrame)(FRAME*, FRAME*);

/*Идентификатор символа*/

struct ID_SYM
{
        uint col, str, comp, word;
};

typedef void * PTR;

#define _GBORDER 1

#define do0(p1,p2,p3) for(p1=(int)(p2); p1<=(int)(p3); ++p1)
#define doi(p1,p2,p3) for(p1=(int)(p2); p1>=(int)(p3); --p1)

//Структура структуры в памяти

struct BOUND
{
        int left, up, right, down;
};

struct POS_STR
{
        uint HeadLine :1, buf :15;
};

struct STAT_STR
{
        int dx, dy, dsym, down_line;
        POS_STR PosStr;
};

struct STAT_COL
{
        STAT_STR *stat_str;
        int dx_col, dy_col, dsym_col, dy_Low, dy_Upp;
};

struct KNOT
{
        KNOT *next, *back;
};

struct KNOT3
{
        KNOT3 *next, *back;
        FRAME *f;
        int cl;
        KNOT3 *beg;
};

struct KNOT4
{
        KNOT4 *next, *back;
        FRAME *f;
};

struct AS
{
        KNOT3 ***beg_as; //Двумерный массив голов списков сегментов АС
        KNOT3 *beg_free; //Ук-ль на голову списка свободных эл-тов списк. прост-ва
        int kx, ky, dx, dy, xmin, ymin; //Кол-ва и размеры сегментов по осям,начало АС
        BOUND *bnd; //Габарит. прямоугольник АС (без учета фиктив. полос)
};

struct REFER
{
        uint ss;
};

#pragma pack(1)

struct PAR
{
        float ax, ay, percent_kgv;
        int xmin_abs, ymin_abs; /*параметры filtr_size*/
        float ax_as, ay_as; /*параметры развертки*/
        int kv;/*весовые коэф-ты "рамочной" метрики*/
        int nums, numw;
        float al, bet, delx; /*параметры union_str*/
        float csi, zet;
        int dp_abs;
        /*clust_as:относит.пороги буквенный csi и словный zet,
         dp_abs - абсолют.порог присоединения тонких изолир.фрагментов к строкам*/
        float k10, k11, size_win_word, num_por;
        int dh_word; //divide_word_str
        int g1, g2; /*миним.длина колонки и интервала в символах(коэф-ты при ave_x)*/
        int maxhg; /*Макс.допустимая высота гистограммы в интервале*/
        float minhg;/*мин.допустимая высота гистограммы в колонке(minh=minnhg*kstr)*/
        int midhg; /*Макс.допустимая средняя высота гист-мы в интервале*/
        int v1, v2; /*миним.длина колонки и интервала в символах(коэф-ты при ave_y)*/
        int maxhv; /*Макс.допустимая высота гистограммы в интервале*/
        float minhv;/*мин.допустимая высота гистограммы в колонке(minh=minnhg*kstr)*/
        int midhv; /*Макс.допустимая средняя высота гист-мы в интервале*/
        float ag, bg, gg, dg;/*Параметры процедуры union_comp*/
        float com1, com2, com3;/*Параметры процедуры find_comma*/
        float por_mono;/*порог на % общего числа рамок в окне для моноспейсности*/
        int allow_colH, allow_colV;/*1-разрешение H- или V-колонизации*/
        int allow_union_str;/*1-разрешение слияния строк*/
        int allow_cut_str;/*1-разрешение резки строк*/
        int allow_filtr_size;/*1-разрешение фильтра по размерам*/
        int allow_union_comp;/*1-разрешение наведения рамок на многосвяз.символ*/
        int allow_cut_comp;/*[1-разрешение вертикал. резки рамок*/
        int allow_cut_prs;/*[1-разрешение вертикал. резки PRS-компонент*/
        int allow_DeSkewCol;
        int allow_RevizRuleStr;
        int allow_SeparGraph;
        int allow_ProcSmall;
        int allow_RevizEstStrAfterDeSkewCol;
        int allow_DeSkewColFragm;
        int allow_DeSkewAll; //Запрещение всех видов вращений
        int allow_SplitCol; //Запрещение колонизации снизу-вверх
        float cut_str1;/*коэф-т для грубой оценки кратности строки*/
        float cut_str2;/*репрезантативность кратности в длинах символов*/
        float cut_str_a, cut_str_b;//Завис-ть от вертикал.кратности
        float EstBnd1;
        int EstBndSize;//Параметры EstBndStrHist
        float k_parag; //Относит. критерий красной строки DivideColParag
        float CutComp1; //Относит.порог вертикал."захода"в сосед.строку FindCutComp
        //Макс.уровень торчания и Max число торчащих наружу и внутрь от линеек для обнаружения заголовков
        int Stick, MaxStickOut, MaxStickIn;
        float StickCoeff;
        float UnionPairStr1; //ф-ция UnionPairStr
        int MinKsym;
        float PercKsym; //AnalysRuler
        float MinMax1, MinMax2;
        int NumIterL, NumIterW;//Параметры EstBndMinMax
        int MaxDelh, DelLine, DepthSearch;//Параметры DeSkewCol
        //Параметры алг-ма RevizRuleCol уточнения линеек строк
        int RevizRuleDelMax;//Коридор для согласования с линейками AnalysRule
        float RevizRuleC1;//Коэф-т при SizeLow порога высоты строки
        float RevizRuleC2;//C2*ksym - min-число рамок в коридоре согласования
        float RevizRuleC3;//C3*ksym - порог числа итераций EstBndMinMax
        int RevizRuleC4; //Мин.число рамок в коридоре сост-ти MM-оценки линеек
        float RevizRuleK1;//Относит.порог тонкости ММ-оценки строки EstBndMinMax
        float RevizRuleK2;//Относит.порог тонкости рамки для отбраковки
        int PenV; //ClassFrmSmall - Penv
        float WeightBetwLine;//ClassFrmEnv -критерий зацепленности по VERT для привед.окр.
        float ax_as_other, ay_as_other, RelThresholdOther;//ClassFrmOther
        float ThresholdXFrmCol, ThresholdYFrmCol;//ClustCOL Относит.пороги меры сходства
        int TypeDoc;
        int AllowLine;//Включать ли поиск линий
};

struct POS_BIT
{
        uint pos1;
        uint pos :8, word :1, sym :1, comma :1, parag :1, JoinComp :1, Index :1, Fract :1,
                MultiPoint :1;
};

struct POS_BIT8
{
        uint pos1, pos2, pos3;
        uint pos :8, word :1, sym :1, comma :1, parag :1, JoinComp :1, Index :1, Fract :1,
                MultiPoint :1;
};

struct POS1_BIT
{
        int DownL;
        uint pos :8, cut_comp :2, buf :6;
};

struct POS_INT
{
        uint pos :8, word :1, sym :1, comma :1, parag :1, JoinComp :1, Index :1, Fract :1,
                MultiPoint :1;
};

struct POS2_BIT
{
        uint pos1;
        uint pos :8;
        uint AveCrossInt :4; //Целая часть сред.сложности символа(числа пересечений)
        uint AveCrossFloat :4;//Дроб. часть сред.сложности символа(ЦМР - 1/16)
};

struct POS3_BIT
{
        uint pos1;
        uint pos :8;
        uint NumHole :4; //Число дыр
        uint MaxDepth :4;//Макс. глубина(ЦМР - 1/16 высоты рамки)
};

#pragma pack(2)
struct SPEC
{
        uint reg :1, typ :4, lev :4, end :1, buf :6;
};

const int MAXFILENAME = 256;
const int MAX_BLOC = 10;
const int MIN_KNOT = 10;
#define SIZE_SEG 65500

//Общие надстройки 4-8-байтные
#ifdef ID4
#define BIT_POSITION POS_BIT
#else
#define BIT_POSITION POS_BIT8
#endif

#define BEG_PARAG1(arg)   ((BIT_POSITION*) &arg)->parag

int shift(void);

void mess(char *s1, char *s2, char *s3, char *s4);

#pragma pack(2)
/*-------------Прототипы ф-ций-------------*/
int init_lst(KNOT ***knot, int *k_bloc, int max_knot, KNOT **beg_free, int size_item);
KNOT *inc_lst(KNOT **beg, KNOT **beg_free);
KNOT *inc_after_lst(KNOT *ptr, KNOT **beg, KNOT **beg_free);
void del_lst(KNOT *ptr, KNOT **beg, KNOT **beg_free);
void free_lst(KNOT **knot, int k_bloc);
int alloc_seg(KNOT **kn, int *kb, int max_knot, uint size_item, int *size_bloc);
uint determine_free_memory(uint size);

int comp1(TYPE *a, TYPE *b);

int search_int(int *x, int n, int a);
typedef int TYPE; /*тип сортируемых данных*/
int MaxArr(int *x, int n, int *PosExtr);
int MinArr(int *x, int n, int *PosExtr);
int Statist(int *arr, int n, int *ave, int *sig, int *med, int *mod, int regim);
int statis2(TYPE *arr, int n, TYPE *med, TYPE *mod, int SizeWin, int *NumMod);
int statis1(TYPE *arr, int n, TYPE *ave1, TYPE *sig1, TYPE *med, TYPE *mod, int regim);
int cut_str(FRAME ****str1, int **ksym1, int *k_str1, FRAME ***frms1, int size_x, int kmax_str);

//Резка рамок компонент
int CutComp(FRAME ***str, int *ksym, int k_str, FRAME **frms, FRAME **frm, int *k_frm);

//==Колонизация==
enum
{
    UNSORT = 0,
    HOR = 1,
    VER = 2,
    TERM = 3,
    MATR = 4,
    IND = 5,
    TREE = 6,
    PAIR = 7,
    TABL = 8,
    FORMUL = 9,
    SLOPE = 10
};

/*макс.число уровней*/
const int MAX_LEV = 6;
/*Макс.число терминал. колонок*/
const int MAX_COL = 45;
/*Рамки всех уровней, кроме компонент, связаны в списки по уровням*/
//---Колонизация "снизу-вверх"----
int GenAS(FRAME **frm, int k_frm, int dx, int dy, BOUND *bnd, KNOT3 *beg_free, int value, AS *As,
        RECT Rect);

extern int SizeYGlob;
extern uint16_t ScanResolution;
extern float MulScanRes;

/*Макросы*/
/*dv - вертикал. размер=минимуму из невязок низов,верхов и середин
 dg - горизонт. размер - расстояние между рамками по горизонтали,
 если они не перекрываются, либо размер перекрытия*/

#define DIST_V(u1,d1,u2,d2) (MIN(d1,d2)-MAX(u1,u2))

//---------ОПИСАНИЕ ЛИНИЙ------------
//--Ориентация линии--
enum
{
    HOR_LINE = 1, VER_LINE = 2
};

//--Тип линии--
enum
{
    SOLID_LINE = 0,
    DOUBLE_LINE = 1,
    THICK_LINE = 2,
    HIDDEN_LINE = 3,
    DOT_LINE = 4,
    MAX_TYPE_LINE = 6
};

//--Начал. или конеч. (.) отрезка--
enum
{
    POINT_BEG = 1, POINT_END = 2
};

#pragma pack(1)

typedef struct hSTRET
{
        short xn, xk, yn, yk;//Stretch-Coordinate
        float k; //Тангенс наклон линии y=kx+b
        short thickness; //средняя толщина отрезка
        float angle; //Угол наклона = atan2(..)
        uchar FeatOrient; //HOR_LINE || VER_LINE
        uchar TypeLine; //Тип линии DOT_LINE || SOLID_LINE || DOUBLE_LINE
        short LenShadow; //Длина перекрытия
        void *ptr; //вспомогат. ук-ль (для FindDot)
} STRET;

//пересчет к не300-dpi SCAN RESOLUTION
#define NORM_SCAN(value) ((int)(MulScanRes*value))
//Диагностика
#include "tabl.h"
const int NOT_ALLOC = -3;

#pragma pack()

void u4sort(void *base, int num, int width, int(*compare)());

