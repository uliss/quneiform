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

//ifdef EXACT_REG => read_frm use cutted rect
//#define EXACT_REG

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

#define ORDER(arg)  ((SPEC*) &arg) -> typ

const int MAXFILENAME = 256;
const int MAX_BLOC = 10;
const int MIN_KNOT = 10;
#define SIZE_SEG 65500

#ifndef BITS32
#define MAX_FRAME 16300
#else
#define MAX_FRAME 32000
#endif

#define P 0
#define R 1
#define S 2
#define PROP 0
#define LEFT 0
#define RIGHT 1
#define CENTER 3
#define PI 3.1415926
#define UNDEF 2

//Для 4-байт. записи
#define END_WORD(arg)    ((POS_BIT*) &arg)->word
#define END_SYM(arg)     ((POS_BIT*) &arg)->sym
#define COMMA(arg)       ((POS_BIT*) &arg)->comma
#define BEG_PARAG(arg)   ((POS_BIT*) &arg)->parag
#define JOIN_COMP(arg)   ((POS_BIT*) &arg)->JoinComp
#define INDICE(arg)      ((POS_BIT*) &arg)->Index
#define FRACT(arg)       ((POS_BIT*) &arg)->Fract
#define MULTI_POINT(arg) ((POS_BIT*) &arg)->MultiPoint

//Общие надстройки 4-8-байтные
#ifdef ID4
#define BIT_POSITION POS_BIT
#else
#define BIT_POSITION POS_BIT8
#endif

#define END_WORD1(arg)    ((BIT_POSITION*) &arg)->word
#define END_SYM1(arg)     ((BIT_POSITION*) &arg)->sym
#define COMMA1(arg)       ((BIT_POSITION*) &arg)->comma
#define BEG_PARAG1(arg)   ((BIT_POSITION*) &arg)->parag
#define JOIN_COMP1(arg)   ((BIT_POSITION*) &arg)->JoinComp
#define INDICE1(arg)      ((BIT_POSITION*) &arg)->Index
#define FRACT1(arg)       ((BIT_POSITION*) &arg)->Fract
#define MULTI_POINT1(arg) ((BIT_POSITION*) &arg)->MultiPoint

#define CUT_COMP(arg)  ((POS1_BIT*) &arg)->cut_comp
#define DOWNL(arg)     ((POS1_BIT*) &arg)->DownL
#define HEAD_LINE(arg) ((POS_STR*) &arg)->HeadLine

#define SPACE_MAX 200

/*Режим TREE1 - генерация дерева структуры общего вида (объемная по памяти операция)
 иначе дерево в ОЗУ не генерится*/
/*#define TREE1*/
/*-----Графика-----*/
#define EGA 16
#define CGA 4
#ifndef ZAGL
extern int nxn, nyn, nxk, nyk, KX, KY, gr_regim;
extern float xnn, ynn, xkk, ykk, xmm, ymm;
#else
int nxn, nyn, nxk, nyk, KX, KY, gr_regim;
float xnn, ynn, xkk, ykk, xmm, ymm;
#endif

int shift(void);

void mess(char *s1, char *s2, char *s3, char *s4);

#ifdef ZAGL
int Esc = 27, Enter = 13, Del = 1083, Delete = 1008, Tab = 1009, Home = 1071, End1 = 1079,
Ins = 1082, Up = 1072, Down = 1080, Left = 1075, Right = 1077, PgUp = 1073, PgDown = 1081,
CtrlEsc = 2027, CtrlEnter = 2010, CtrlDel = 2127, CtrlDelete = 2147, CtrlTab = 2148,
CtrlHome = 2119, CtrlEnd = 2117, CtrlIns = 2146, CtrlUp = 2141,
CtrlDown = 2145, CtrlLeft = 2115, CtrlRight = 2116, CtrlPgUp = 2132, CtrlPgDown = 2118,
AltEsc = 3001, AltEnter = 3028, AltDel = 3014, AltDelete = 3163, AltTab = 3165,
AltHome = 3151, AltEnd = 3159, AltIns = 3162, AltUp = 3152,
AltDown = 3160, AltLeft = 3155, AltRight = 3157, AltPgUp = 3153, AltPgDown = 3161,
ShiftEsc = 4027, ShiftEnter = 4013, ShiftDel = 4083, ShiftDelete = 4008, ShiftTab = 4009,
ShiftHome = 4071, ShiftEnd = 4079, ShiftIns = 4082, ShiftUp = 4072, ShiftDown = 4080,
ShiftLeft = 4075, ShiftRight = 4077, ShiftPgUp = 4073, ShiftPgDown = 4081;
#else
extern int Esc, Enter, Del, Delete, Tab, Home, End1, Ins, Up, Down, Left, Right, PgUp, PgDown,
        CtrlEsc, CtrlEnter, CtrlDel, CtrlDelete, CtrlTab, CtrlHome, CtrlEnd, CtrlIns, CtrlUp,
        CtrlDown, CtrlLeft, CtrlRight, CtrlPgUp, CtrlPgDown, AltEsc, AltEnter, AltDel, AltDelete,
        AltTab, AltHome, AltEnd, AltIns, AltUp, AltDown, AltLeft, AltRight, AltPgUp, AltPgDown,
        ShiftEsc, ShiftEnter, ShiftDel, ShiftDelete, ShiftTab, ShiftHome, ShiftEnd, ShiftIns,
        ShiftUp, ShiftDown, ShiftLeft, ShiftRight, ShiftPgUp, ShiftPgDown;
#endif
#pragma pack(1)
#define PRS_ONE1 struct h_prs_one1
//Вариации описания pole_bit
#ifdef PRS_T
typedef unsigned short PRS_ONE;
#define WIDTH_PRS 2
#else
#define PRS_ONE struct h_prs_one
PRS_ONE1
{
        uchar code;
        uint16_t metri;
};
PRS_ONE
{
        uint typ :2, het :1, scob :1, dummi :12;
        uchar dummi1;
};
#define WIDTH_PRS 3
#endif

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
/*Кластер-анализ с использованием АС*/
#define MAX_ENV 100
#define DIST_LETTER 0
#define DIST_WORD 1
#define DIST_COL 2
#define DIST_LINE 3
#define DIST_LINE1 4
#define DIST_DOT 5
#define DIST_DIAG 6
int Statist(int *arr, int n, int *ave, int *sig, int *med, int *mod, int regim);
int statis2(TYPE *arr, int n, TYPE *med, TYPE *mod, int SizeWin, int *NumMod);
int statis1(TYPE *arr, int n, TYPE *ave1, TYPE *sig1, TYPE *med, TYPE *mod, int regim);
int cut_str(FRAME ****str1, int **ksym1, int *k_str1, FRAME ***frms1, int size_x, int kmax_str);

//Резка рамок компонент
int CutComp(FRAME ***str, int *ksym, int k_str, FRAME **frms, FRAME **frm, int *k_frm);

//==Колонизация==
#define UNSORT 0
#define HOR    1
#define VER    2
#define TERM   3
#define MATR   4
#define IND    5
#define TREE   6
#define PAIR   7
#define TABL   8
#define FORMUL 9
#define SLOPE  10
#define MAX_LEV  6 /*макс.число уровней*/
//==Иерархич.индекс==
#define LET 1
#define WRD 2
#define STR 3
#define COL 4
#define COLVT 5
#define MAX_COL 45 /*Макс.число терминал. колонок*/
/*Рамки всех уровней, кроме компонент, связаны в списки по уровням*/
//---Колонизация "снизу-вверх"----
int GenAS(FRAME **frm, int k_frm, int dx, int dy, BOUND *bnd, KNOT3 *beg_free, int value, AS *As,
        RECT Rect);

#ifndef MAIN
extern PAR par;
extern int viz, extr;
extern long na, ko, pos1;
extern FILE *fip, *out, *out_rb;
extern long PRSMAX;
extern void *low; /*нижняя граница загрузки программы - для контроля*/
extern char *s1, *s2, *s3; /*Сообщения для интерактива*/
extern char *abcd;
extern int count, rou;
extern int h_term;
extern STAT_COL stat_col;
extern int dx_prs, dy_prs;
extern int k_prop, k_mono;
extern int upi;
extern char dir[_MAX_DIR], fname[_MAX_FNAME], ext[_MAX_EXT];
extern char Fullpath[255];
extern int SizeXGlob, SizeYGlob;
extern REFER *fiv;
extern int NumCut;
extern FRAME *Cut;
extern int SizeX_W, SizeY_W;
extern float AveNumCrossCol, AveNumCross1Col, AveThickCol, AveThick1Col;
extern int KbAll;
extern int CountCol, Percent1, Percent2, Percent3, NumColt;
extern char FileParStr[MAXFILENAME];
extern uint16_t ScanResolution;
extern float MulScanRes;
extern int Break_on;
extern FUN_POMP pump;
extern FUN_MESS FunMessage;
extern int WidthPRS, MaxShtrih;
#else
PAR par;
int viz = 0, extr = 0;
long na = 0, ko = -1, pos1;
FILE *fip, *out, *out_rb;
long PRSMAX;
void *low; /*нижняя граница загрузки программы - для контроля*/
const char *s1 = "Esc - continue;", *s2 = "Up,Down,Left,Right - Move image;",
*s3 = "Ctrl/<Up,Down,Left,Right> - Scale image";
const char *abcd = "|\\-/";
int count = 0, rou = 0;
int h_term;/*Счетчик текущей высоты PRS-линии(для записи в терминатор)*/
STAT_COL stat_col;/*Статистика по строкам текущей колонки*/
int dx_prs, dy_prs;/*Габариты PRS-файла*/
int k_prop, k_mono;//Кол-ва пропорц. и моноспейс. колонок и общая оценка моноспейса
int upi;/*Счетчик строк для рисовалки*/
char drive[_MAX_DRIVE], dir[_MAX_DIR], fname[_MAX_FNAME], ext[_MAX_EXT];
char Fullpath[255];
//char NameFuncErr[100],Buff[60]; int NumErr;
int SizeXGlob, SizeYGlob;
REFER *fiv;
int NumCut;
FRAME *Cut; //Счетчик и буфер разрезанных компонент
int SizeX_W, SizeY_W;
float AveNumCrossCol, AveNumCross1Col, AveThickCol, AveThick1Col;
int KbAll;
int CountCol, Percent1 = 5, Percent2 = 55, Percent3 = 40, NumColt;
char FileParStr[MAXFILENAME];
uint16_t ScanResolution;
float MulScanRes;//Разрешения сканера и нормир.множитель
int Break_on = 0; // if == 1 => break process
FUN_POMP pump; // вставляем в цикле
FUN_MESS FunMessage; //в режиме DLL вместо SendMessage
int WidthPRS, MaxShtrih;
#endif

/*Макросы*/
/*dv - вертикал. размер=минимуму из невязок низов,верхов и середин
 dg - горизонт. размер - расстояние между рамками по горизонтали,
 если они не перекрываются, либо размер перекрытия*/

#define dist_frm(f1,f2,dist) { int dv,d_up;\
  d_up=  abs(f1->up - f2->up); \
  dv=abs(f1->down - f2->down); if(d_up < dv) dv=d_up; \
  dist=abs(MAX(f1->left,f2->left) - MIN(f1->right,f2->right));\
  dist+=par.kv*dv; \
}

#define DIST_V(u1,d1,u2,d2) (MIN(d1,d2)-MAX(u1,u2))

#define DV_FRM(a,b) DIST_V(a->up,a->down,b->up,b->down)

#define DH_FRM(a,b) DIST_V(a->left,a->right,b->left,b->right)

//Мин.расст. м/точками рамок *f1,*f2, long Zv,Zh
#define DIST_MIN_FRM(f1,f2) \
  ((Zv=DIST_V(f1->up,f1->down,f2->up,f2->down)), \
  (Zh=DIST_V(f1->left,f1->right,f2->left,f2->right)), \
  (Zv >= 0 ? ((Zh < 0) ? Zh*Zh : 0) : ((Zh >= 0) ? Zv*Zv : Zh*Zh+Zv*Zv)))
//Мера сх-ва <0,1> с раздел.порогами ph,pv по обеим коор-там, int ZZv,ZZh

#define DIST_FRM_COL(f1,f2,ph,pv) \
  ((ZZv=DIST_V(f1->up,f1->down,f2->up,f2->down)), \
  (ZZh=DIST_V(f1->left,f1->right,f2->left,f2->right)), \
  (ZZv >= pv && ZZh >= ph ? 1:0))

//---------ОПИСАНИЕ ЛИНИЙ------------

#include "lindefs.h"
//--Диагностика тестировщика структуры линий--
#define NULL_LINE 1
#define NULL_THICK 2
#define COINCIDE_STRETCH 4
//--Ориентация линии--
#define HOR_LINE 1
#define VER_LINE 2
#define SLOPE_LINE 4
//--Тип линии--
#define SOLID_LINE  0
#define DOUBLE_LINE 1
#define THICK_LINE  2
#define HIDDEN_LINE 3
#define DOT_LINE    4
#define MAX_TYPE_LINE 6
//--Начал. или конеч. (.) отрезка--
#define POINT_BEG 1
#define POINT_END 2
//Мин.тангенс, при котором еще делаем поворот всей системы линий
//fi|10'  |20'  |30'  |60'  |90'  |120' |150' |180' |210' |
//tg|.0029|.0058|.0087|.0175|.0262|.0349|.0437|.0524|.0612|
#define MIN_TANG .001

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

typedef struct hSTRET_P
{
        STRET *Stret;
        short Feat; //POINT_BEG || POINT_END
} STRET_P;

//пересчет к не300-dpi SCAN RESOLUTION
#define NORM_SCAN(value) ((int)(MulScanRes*value))
//Диагностика
#include "tabl.h"
#define NOT_ALLOC -3

//TypeCall: CALL_BOX - without open output PRN-file, CALL_STR - with ...
#define CALL_STR 0
#define CALL_BOX 1

#pragma pack()

void u4sort(void *base, int num, int width, int(*compare)());

