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

#include "sys_prog.h"

/*#include <io.h>*/
#include <sys/types.h>

#include "compat_defs.h"
#include "minmax.h"

//#define TIGER_CORR
//#define __MRK__
//#define __DOT__
//В этом случае формат хранения рамок: FRAME + POINT OldCoor
//#if defined (__DOT__) || defined (__MRK__)
#define OLD_COOR
//#endif
//#define BLANK
#ifdef __STR_DLL__
#define KREST
#define PRAFAX
#endif

#define DEB_PRA
#ifndef BLANK
#define SPLIT_COL
//#define PROF_COL
//--Отделение графики
#define GRAPH
#endif
#define PLAIN 0
#define NORV  1

#if defined (WIN_MOD) && !defined (__MRK__) && !defined (__DOT__)
//#include "resource.h"
#endif
//вариант, когда считается, что в метриках смертей стоят глубины
//#define _DEPTH_

#define DEBUG
#define VIZ

#if defined (VIZ) && defined (DOS_MOD)
#define DRAW
#endif

#define WORK 0
#define EDIT 1

//#define INTEL
//#define AU
//ext - .prn or .prc
//#define OLD_NAME   В файле column.c

//для динамич. переключения длины PRS-кода
//#define PRS_2_3

#include "wind32.h"

#define FRAME struct h_frame
FRAME {
	unsigned long start_pos, end_pos;
	int left, up, right, down;
};
#define TYPE int /*тип расстояний между FRAME*/

typedef int (*FUN_POMP)(void);
typedef void (*FUN_MESS)(uint16_t wPar, uint32_t lPar);
typedef int(*COMP_FUN)(void);
typedef TYPE (*DistFrame)(FRAME*, FRAME*);

#ifndef CPP
//#define CPP
#endif

//Если установлено ID4, ID_SYM длиной 4 байта, иначе - 8
#ifndef BLANK
//#define ID4
#endif

#define ID_SYM struct h_id_sym /*Идентификатор символа*/

#ifdef ID4
ID_SYM {unsigned char col,str,comp,word;};
#define CONV uchar
#define FL_ID 1
#else
ID_SYM {
	uint col, str, comp, word;
};
#define CONV uint
#define FL_ID 0
#endif

typedef void * PTR;
typedef void ** PTR2;
typedef void *** PTR3;

#if !defined (DOS_MOD)
#define _GFILLINTERIOR 0
#define _GBORDER 1
#endif

#define do0(p1,p2,p3) for(p1=(int)(p2); p1<=(int)(p3); ++p1)
#define dou(p1,p2,p3) for(p1=(p2); p1<=(p3); ++p1)
#define doi(p1,p2,p3) for(p1=(int)(p2); p1>=(int)(p3); --p1)

//Структура структуры в памяти
#define PIECE  struct h_piece
PIECE {
	int up, down;
};
#define POINT_H POINT *
typedef struct hFRM_ARR {
	FRAME **FrmArr, **frm;
	int NumArr, NumFrm, AllFrm;
} FRM_ARR;
#define BOUND struct h_bound
BOUND {
	int left, up, right, down;
};
#define POS_STR struct h_pos_str

POS_STR {
	uint HeadLine :1, buf :15;
};

#define STAT_STR struct h_stat_str
STAT_STR {
	int dx, dy, dsym, down_line;
	POS_STR PosStr;
};
#define STAT_COL struct h_stat_col
STAT_COL {
	STAT_STR *stat_str;
	int dx_col, dy_col, dsym_col, dy_Low, dy_Upp;
};
#define KNOT struct h_knot
#define KNOT2 struct h_knot2
#define KNOT3 struct h_knot3
#define KNOT4 struct h_knot4
#define FRML  struct h_frml
#define AS struct h_as
KNOT {
	KNOT *next, *back;
};
KNOT2 {
	KNOT2 *next, *back;
	FRAME *f;
	TYPE dist;
};
KNOT3 {
	KNOT3 *next, *back;
	FRAME *f;
	int cl;
	KNOT3 *beg;
};
KNOT4 {
	KNOT4 *next, *back;
	FRAME *f;
};
FRML {
	FRML *next, *back;
	int left, up, right, down;
};
AS {
	KNOT3 ***beg_as; //Двумерный массив голов списков сегментов АС
	KNOT3 *beg_free; //Ук-ль на голову списка свободных эл-тов списк. прост-ва
	int kx, ky, dx, dy, xmin, ymin; //Кол-ва и размеры сегментов по осям,начало АС
	BOUND *bnd; //Габарит. прямоугольник АС (без учета фиктив. полос)
};
#define REFER struct h_refer
REFER {
	uint ss;
};
typedef struct hTITLE_PRS {
	char Name[4], ScanResX, ScanResY, Thick, Reserv;
	int SizeX, SizeY;
} TITLE_PRS;

#pragma pack(1)

#define PAR struct h_par
PAR {
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

#define POS_BIT struct h_pos_bit
POS_BIT {
	uint pos1;
	uint pos :8, word :1, sym :1, comma :1, parag :1, JoinComp :1, Index :1,
			Fract :1, MultiPoint :1;
};
#define POS_BIT8 struct h_pos_bit8
POS_BIT8 {
	uint pos1, pos2, pos3;
	uint pos :8, word :1, sym :1, comma :1, parag :1, JoinComp :1, Index :1,
			Fract :1, MultiPoint :1;
};
#define POS1_BIT struct h_pos1_bit
POS1_BIT {
	int DownL;
	uint pos :8, cut_comp :2, buf :6;
};
//POS1_BIT { uint pos:8,cut_comp:2,buf:6; uint pos1; };
#define POS_INT struct h_pos_int
POS_INT {
	uint pos :8, word :1, sym :1, comma :1, parag :1, JoinComp :1, Index :1,
			Fract :1, MultiPoint :1;
};
#define POS2_BIT struct h_pos_bit2
POS2_BIT {
	uint pos1;
	uint pos :8;
	uint AveCrossInt :4; //Целая часть сред.сложности символа(числа пересечений)
	uint AveCrossFloat :4;//Дроб. часть сред.сложности символа(ЦМР - 1/16)
};

#define AVE_CROSS(arg) ((POS2_BIT*)&arg)->AveCrossInt+(((POS2_BIT*)&arg)->AveCrossFloat)/16.
#define POS3_BIT struct h_pos_bit3
POS3_BIT {
	uint pos1;
	uint pos :8;
	uint NumHole :4; //Число дыр
	uint MaxDepth :4;//Макс. глубина(ЦМР - 1/16 высоты рамки)
};

#define NUM_HOLE(arg) ((POS3_BIT*)&arg)->NumHole
#define MAX_DEPTH(arg) ((POS3_BIT*)&arg)->MaxDepth

/*Описание узла дерева:
 *next,*back - ссылки по элементам с общим предком;
 *up - ссылка на предка;
 *down - ссылка на голову своих дочерей;
 *f - ссылка в список рамок на объемлющую рамку;
 reg - признак регулярности данного узла;
 typ - характер упорядоченности списка его дочерей;
 lev - уровень узла (это не чисто древесное понятие, а структурное, т.е.
 узлы одного уровня могут находиться на разном расстоянии от корня);
 end - признак блокировки (1 - конец блока);
 buf - резерв */
#define KNOTG struct h_knotg
KNOTG {
	KNOTG *next, *back, *up, *down;
	FRML *f;
	uint reg :1, typ :4, lev :4, end :1, buf :6;
};

#pragma pack(2)
typedef struct h_spec {
	uint reg :1, typ :4, lev :4, end :1, buf :6;
} SPEC;

#define ORDER(arg)  ((SPEC*) &arg) -> typ

#ifndef CPP
#ifdef __cplusplus
extern "C" {
#endif
#endif

//typedef int(*COMP_FUN)(void);
//typedef TYPE (*DistFrame)(FRAME*,FRAME*);

#ifdef WIN32
//#define _splitpath __splitpath
//#define _makepath  __makepath
//void _splitpath(const char *f,char *drive,char *dir,char *fname,char *ext);
//void _makepath(char *f,
//  const char *drive,const char *dir,const char *fname,const char *ext);
#endif

#define MAXFILENAME 256
#define MAX_KNOT 15000
#define MAX_CLU_BS 20
#define MAX_BLOC 10
#define MIN_KNOT 10
#define SIZE_FRAME_FILE 16
#define SIZE_SEG 65500

#ifndef BITS32
#define MAX_FRAME 16300
#else
#define MAX_FRAME 32000
#endif

#define ENDFILE 0
#define ERRFILE -1
#define P 0
#define R 1
#define S 2
#define T 3
#define PROP 0
#define MONO 1
#define SIZE_PRS_TITLE 12
#define T_ORDIN 0
#define T_DOWN_LINE 1
#define T_END_STR 2
#define LEN_PATH 78
#define T_FALSE 3
#define CUT_NOT 0
#define CUT_UP 1
#define CUT_DOWN 2
#define LEFT 0
#define RIGHT 1
#define JUST 2
#define CENTER 3
#define PI 3.1415926
#define PI2 1.5708
#ifndef FALSE
#define FALSE 0
#define TRUE 1
#endif
#define COL_TXT FALSE
#define COL_GRAPH TRUE
#define UNDEF 2
long load_pos(long cod);
void copy_attr(POS_BIT *in, POS_BIT *out);
void copy_attr48(POS_BIT *in, POS_BIT8 *out);
long PutStrBit(long pos, POS_INT bit);
POS_INT GetStrBit(long pos);

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
int nxn,nyn,nxk,nyk,KX,KY,gr_regim;
float xnn,ynn,xkk,ykk,xmm,ymm;
#endif
void pointt_w(int x1, int y1, int krat, int ifil);
void point_w(int x1, int y1), point(int x, int y), vect_w(int x1, int y1),
		vector_w(int x1, int y1, int x2, int y2), vect(int x, int y), vector(
				int x1, int y1, int x2, int y2), clear(void), pause_internal(
				void), opn1(void), win_real(void), bounds_frm(int ii,
				FRAME **frm, int nx), BoundsRect(int ii, RECT *frm, int nx),
		polygon_w(int *x, int *y, int n, int col, int zap, int krat, int ifil),
		polygon(int *x, int *y, int n, int col, int zap, int krat, int ifil),
		zpage(int ist);
int setcol(int i);
void point_f(float x1, float y1);
int getch1(void), shift(void), ctrl(void), alt(void);
void image_frm(FRAME *f, int col, int line_style, int fill);
void image_frame(FRAME **frm, int k, int col, int line_style, int fill);
void image_bnd(BOUND *f, int col, int line_style, int fill);
void image_rect(RECT *f, int col, int line_style, int fill);
void scale_image(float ax, float ay);
void move_image(int nx, int ny);
int analys_key(int *fl);
int hist_int(char *name, int *x, int n, int Xmin, int Xmax, int reg, int *y,
		int k);
void mess(char *s1, char *s2, char *s3, char *s4);
uint32_t SendMessage_m(uint16_t wParam, uint32_t lParam);
int chk_string(FRAME ***str, int *ksym, int k_str, FRAME **frms);
int chk_ptr(void *ptr);
#ifdef ZAGL
int Esc= 27,Enter= 13,Del=1083,Delete=1008,Tab=1009,Home=1071,End1=1079,
Ins=1082,Up=1072,Down=1080,Left=1075,Right=1077,PgUp=1073,PgDown=1081,
CtrlEsc=2027,CtrlEnter=2010,CtrlDel=2127,CtrlDelete=2147,CtrlTab=2148,
CtrlHome=2119,CtrlEnd=2117,CtrlIns=2146,CtrlUp=2141,
CtrlDown=2145,CtrlLeft=2115,CtrlRight=2116,CtrlPgUp=2132,CtrlPgDown=2118,
AltEsc=3001,AltEnter=3028,AltDel=3014,AltDelete=3163,AltTab=3165,
AltHome=3151,AltEnd=3159,AltIns=3162,AltUp=3152,
AltDown=3160,AltLeft=3155,AltRight=3157,AltPgUp=3153,AltPgDown=3161,
ShiftEsc=4027,ShiftEnter=4013,ShiftDel=4083,ShiftDelete=4008,ShiftTab=4009,
ShiftHome=4071,ShiftEnd=4079,ShiftIns=4082,ShiftUp=4072,ShiftDown=4080,
ShiftLeft=4075,ShiftRight=4077,ShiftPgUp=4073,ShiftPgDown=4081;
#else
extern int Esc, Enter, Del, Delete, Tab, Home, End1, Ins, Up, Down, Left,
		Right, PgUp, PgDown, CtrlEsc, CtrlEnter, CtrlDel, CtrlDelete, CtrlTab,
		CtrlHome, CtrlEnd, CtrlIns, CtrlUp, CtrlDown, CtrlLeft, CtrlRight,
		CtrlPgUp, CtrlPgDown, AltEsc, AltEnter, AltDel, AltDelete, AltTab,
		AltHome, AltEnd, AltIns, AltUp, AltDown, AltLeft, AltRight, AltPgUp,
		AltPgDown, ShiftEsc, ShiftEnter, ShiftDel, ShiftDelete, ShiftTab,
		ShiftHome, ShiftEnd, ShiftIns, ShiftUp, ShiftDown, ShiftLeft,
		ShiftRight, ShiftPgUp, ShiftPgDown;
#endif
#pragma pack(1)
#define PRS_ONE1 struct h_prs_one1
//#ifdef INTEL
//Вариации описания pole_bit
#ifdef PRS_T
typedef unsigned short PRS_ONE;
#define WIDTH_PRS 2
#else
#define PRS_ONE struct h_prs_one
PRS_ONE1 {
	uchar code;
	uint16_t metri;
};
PRS_ONE {
	uint typ :2, het :1, scob :1, dummi :12;
	uchar dummi1;
};
#define WIDTH_PRS 3
#endif
//#else

//#endif
#pragma pack(2)
/*-------------Прототипы ф-ций-------------*/
int init_lst(KNOT ***knot, int *k_bloc, int max_knot, KNOT **beg_free,
		int size_item);
KNOT *inc_lst(KNOT **beg, KNOT **beg_free);
KNOT *inc_after_lst(KNOT *ptr, KNOT **beg, KNOT **beg_free);
void del_lst(KNOT *ptr, KNOT **beg, KNOT **beg_free);
KNOT* DelLstNext(KNOT *ptr, KNOT **beg, KNOT **beg_free);
void free_lst(KNOT **knot, int k_bloc);
int memmove_m(void *out, void *in, long size);
int alloc_seg(KNOT **kn, int *kb, int max_knot, uint size_item, int *size_bloc);
uint determine_free_memory(uint size);
void print_ptr(KNOT *beg);
void print_ptr_invert(KNOT *beg);
void pr_ptr(KNOT *ptr, char *str);
int read_frm(char *file_frm, FRAME ***frm_arr, int *k_arr_frm, FRAME ***frm,
		int *k_frm);
void GetOldCoor(FRAME *f, POINT *old);
void RestoreOldCoorFrm(FRAME *f, FRAME *fo, float tg_ang);
void RestoreOldCoorRect(FRAME *f, RECT *fo);
int read_frmW(char *file_frm, POINT_H *CentrW, int *SizeX_W, int *SizeY_W,
		long *k_frm);
void PASC free_frm(FRAME **frm_arr,int k_arr_frm,FRAME **frm);
int unfold(FRAME **frm, int k_frm, TYPE *dist1, int ave_y, BOUND *bnd);
int clust_unfold(FRAME **frm, int k_frm, TYPE *dist, TYPE dp, int max_cl,
		int *k_cl1, KNOT3 **beg_free1, KNOT3 **beg_cl);
int viz_clust_word(FRAME **frm, int k_frm, int k_cl, KNOT3 **beg_cl);
int comp_left(FRAME **a, FRAME **b);
int sort_lett(int k_cl, KNOT3 **beg_free1, KNOT3 **beg_cl, COMP_FUN CompFun);
int cut_word_unfold(int *k_cl1, KNOT3 **beg_cl);
int project_frm(FRAME **frm, int k_frm, float tg_ang, int AllowW,
		POINT_H CentrW, long k_frmW);
/*TYPE dist_frm(FRAME *f1,FRAME *f2);*/
//long time(void);
void sort_int(TYPE *a, int num, int *nodr);
int comp1(TYPE *a, TYPE *b);
int compF(float *a, float *b);
int compare(TYPE *a, TYPE *b);
int comp_long(uint32_t *a, uint32_t *b);

//	void  u4sort(void *base, int  num, int  width, int  (*compare)());

int search_int(int *x, int n, int a);
#define TYPE int /*тип сортируемых данных*/
float tg(float ang);
//int est_struct(char *file_frm,char *file_pcx,FRAME ***frm_arr,int *k_arr_frm,
//         FRAME ***frm1,int *k_frm1);
int filtr_size(FRAME **frm, int *k_frm1, int *kg1, int *kv1, int *kgv1,
		int ave_x, int ave_y);
int ClassFrmSize(FRAME **frm, int *k_frm, int *k_small, int xmin, int ymin,
		int ymin1);
int ClassFrmSmall(FRAME **frm, int k_frm, int k_small, int *IndSmall,
		BOUND *bnd, KNOT3 **beg_str, FRAME **frms, int k_str, int size_x,
		int size_y);
int d_small(FRAME *l, FRAME *s, int k);
int init_par(char *PathName, int reg);
int init_arr_frm(FRAME ***frm_arr, int *k_arr_frm, FRAME ***frm, int k_frm);
int est_frame_word(FRAME **frmw, int k_cl, KNOT3 **beg_cl);
int EstBndMinMax(short *up, short *down, int n, int NumIter, short *t,
		short *b, int AveY, float k1, float k2);
int imax(short *x, int n, char *pr, int *imax);
int imin(short *x, int n, char *pr, int *imin);
int MaxArr(int *x, int n, int *PosExtr);
int MinArr(int *x, int n, int *PosExtr);
int ReAnalysStr(FRAME **f, int k, int Reg);
int RevizRule(BOUND *bnds, int dU, int dL, FRAME **str, int ksym, int DelMax,
		int flUpp);
int RevizRuleCol(BOUND *bnds, FRAME ***str, int *ksym, int k_str, int DelMax);
int ImFrmRule(FRAME ***str, int *ksym, int k_str, BOUND *bnd_str, int in);
#define COEFF 300
int est_angle_word(int k_cl, KNOT3 **beg_cl, float *tg_ang);
//int DeSkewCol(BOUND *bnd_str,FRAME ***str,int *ksym,int k_str,int del,
//  int MaxDelh,int k);
int DeSkewCol(BOUND *bnd_str, FRAME ***str, int *ksym, int k_str, int del,
		int MaxDelh, int k, FRAME **frm, int k_frm);
/*Кластер-анализ с использованием АС*/
#define MAX_ENV 100
#define DIST_LETTER 0
#define DIST_WORD 1
#define DIST_COL 2
#define DIST_LINE 3
#define DIST_LINE1 4
#define DIST_DOT 5
#define DIST_DIAG 6
void bound_frm(FRAME **frm, int k_frm, BOUND* bnd);
static int comp(TYPE *a, TYPE *b);
int est_size_frm(FRAME **frm, int k_frm, int *size_x, int *size_y, int step);
void PutUpDown(KNOT3 *beg, int reg, short up, short down);
void GetUpDown(KNOT3 *beg, int reg, short *up, short *down);
TYPE dist_word(FRAME *f1, FRAME *f2);
int Statist(int *arr, int n, int *ave, int *sig, int *med, int *mod, int regim);
int statis2(TYPE *arr, int n, TYPE *med, TYPE *mod, int SizeWin, int *NumMod);
int statis1(TYPE *arr, int n, TYPE *ave1, TYPE *sig1, TYPE *med, TYPE *mod,
		int regim);
int statis(TYPE *arr, int n, TYPE *ave1, TYPE *sig1, TYPE *med, TYPE *mod,
		int regim);
int statisF(float *arr, int n, float *ave, float *sig, float *med, float *mod);
int sort_str(int k_str, KNOT3 **beg_str, FRAME **frms);
int sort1_str(int k_str, FRAME ***str, int *ksym, FRAME **frms);
int comp_vert(FRAME **a, FRAME **b);
//int gen_str(KNOT3 **beg_str,int k_str,FRAME ***str,int *ksym);
int gen_str(KNOT3 **beg_str, int k_str, FRAME ***str, int *ksym, FRAME **frm,
		int *IndSmall, int k_frm, int k_small);
int union_str(int *k_str1, FRAME **frms, KNOT3 **beg_str, int *ksym,
		KNOT3 **beg_freew, int size_x, int size_y);
int dist_x(FRAME *f1, FRAME *f2);
int cut_str(FRAME ****str1, int **ksym1, int *k_str1, FRAME ***frms1,
		int size_x, int kmax_str);
float f_inc(int v_krat);
int divide_str_substr(int k, int ns, FRAME ****str1, int **ksym1, int *k_str1,
		FRAME*** frms1, int kmax_str, BOUND *bnd_str);
void out_vih(FRAME ***str, int *ksym, int k_str, char *file);
int divide_str_word(FRAME ***str, int *ksym, int k_str, int size_x);
int DivideColParag(FRAME ***str, int *ksym, int k_str, int size_x);
int union_comp(FRAME **f, int ksym, int ave_x, int ave_y);
int TestPoint(FRAME f1, FRAME f2, int fl, int ave_y);
int find_comma(FRAME **f, int ksym, FRAME *frms, int ave_x, int ave_y);
int DetectHeadLine(FRAME ***str, int *ksym, int k_str, FRAME **frms, int ave_y);
int UnionPairStr(FRAME ***str, int *ksym, int *k_str1, FRAME **frms, int SizeY);
int CalcHist(int *x, int *krat, int n, int *MinX, int *MaxX, int **hist);
int EstBndStrHist(FRAME **f, int n, BOUND *bnd, int ksym_ave, int size_x);
int AnalysRuler(FRAME **str, int ksym, BOUND *frms, int DelMax, int DelMin,
		int *AllU, int *AllD);
int est_monospace(FRAME ***str, int *ksym, int k_str, int *space1);
int DetectColGraph(POINT_H CentrW, long k_frmw, FRAME ***str, int *ksym,
		int k_str, BOUND bndt, BOUND *BndStr);
int calc_frm_str(FRAME **str, int ksym, BOUND *bnd);
void corr_title_prs(char *file_prs_out);
uint mark_term(uint typ_term, uint param);
void image_prs_str(uint *sym, int len, int col);
char *getpath(char *buf);
//Резка рамок компонент
int CutComp(FRAME ***str, int *ksym, int k_str, FRAME **frms, FRAME **frm,
		int *k_frm);
int FindCutComp(FRAME ***str, int *ksym, int k_str, FRAME **frms, int SizeY);
int breath(int **pprs, int j, int *jj);
//int CutVertComp(uint16_t **pprs,int *num,int up,int down,
//    int *LineCut,int reg,int dir,int *nn_alt,int *nCurrA,int *found_max);
int CutVertComp(int **pprs, int *num, int up, int down, int *LineCut, int reg,
		int dir, int *num_alt, int *nCurrA, int *found_max);
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
int search_col(FRAME **frm, int k_frm, int *intr, int *k_int1, BOUND *bnd,
		int ave_dir, int ave_ort, int reg);
int inc_g(KNOTG *after, KNOTG **begg, KNOTG **freeg, KNOTG **ptrg, FRML **begf,
		FRML **freef, FRML **ptrf, int lev);
int
		est_string(FRAME **frm, int k_frm, FRAME ****str1, int **ksym1,
				int *k_str1);
int EstSizeUppLow_Col(FRAME ***str, int *ksym, int k_str, BOUND *bnd_str,
		int *SizeUpp, int *SizeLow);
int inc_g(KNOTG *after, KNOTG **begg, KNOTG **freeg, KNOTG **ptrg, FRML **begf,
		FRML **freef, FRML **ptrf, int lev);
int class_frm_col(FRAME **frm, int k_frm, BOUND *bnd, int ave_x, int ave_y,
		int reg, int *intr, int *k_int);
int calc_stat_col(FRAME ***str, int *ksym, int k_str, BOUND *bnd_str);
//---Колонизация "снизу-вверх"----
long DistMinFrm(FRAME *f1, FRAME *f2);
int DistFrmCol(FRAME *f1, FRAME *f2, int ph, int pv);
int GenAS(FRAME **frm, int k_frm, int dx, int dy, BOUND *bnd, KNOT3 *beg_free,
		int value, AS *As, RECT Rect);
int EstBetwLine(AS *As, int *BetwLine, int k_frm);
int ClassFrmEnv(FRAME **frm, int *k_frm1, int *k_izo1, AS *As, int BetwLine,
		int AveX, int AveY);
int ClassFrmOther(FRAME **frm, int *k_frm1, int k_other, KNOT3 **beg_cl,
		int k_cl, KNOT3 *beg_free, int AveX, int AveY, int BetwLine,
		BOUND *bnd, int *intr, int *k_col);
int SplitCol(FRAME **frm, int *k_frm1, int SizeXGlob, int SizeYGlob,
		BOUND *bndc, KNOT3 **beg_free1, int **intr1, int *kcol, int *NumMax);
//--Отделение текста от графики--
int CalcHistVert(FRAME ***f1, int k_str, int *ksym1, BOUND *bnds1);
int OrtFoure(float *y, float *Re, float *Im, float *ys, float *Err, int k,
		int p, int na, int beg, int end);

#ifndef V_LOCK
int32_t filelength_m(FILE *stream);

int hread_m(void *buf, int size, int count, FILE *stream);
int CopyFile_my(char *NameIn, char *NameOut);
int fgets_m(char *str, int max_len, FILE *f);
char get_kod(FILE *f);
int fgets1_m(char *str, int max_len, FILE *f);
char get1_kod(FILE *f);
char *get_param(char *str, char *param, int max_len);
char *get1_param(char *str, char *param, int max_len);
#endif

#ifndef MAIN
extern PAR par;
extern int viz,extr;
extern long na,ko,pos1; extern FILE *fip,*out,*out_rb;
extern long PRSMAX;
extern void *low; /*нижняя граница загрузки программы - для контроля*/
extern char *s1,*s2,*s3; /*Сообщения для интерактива*/
extern char *abcd;
extern int count,rou;
extern int h_term;
extern STAT_COL stat_col;
extern int dx_prs,dy_prs;
extern int k_prop,k_mono;
extern int upi;
extern char dir[_MAX_DIR], fname[_MAX_FNAME], ext[_MAX_EXT];
extern char Fullpath[255];
extern int SizeXGlob,SizeYGlob;
extern REFER *fiv;
extern int NumCut; extern FRAME *Cut;
extern int SizeX_W,SizeY_W;
extern float AveNumCrossCol,AveNumCross1Col,AveThickCol,AveThick1Col;
extern int KbAll;
extern int CountCol,Percent1,Percent2,Percent3,NumColt;
extern char FileParStr[MAXFILENAME];
extern uint16_t ScanResolution;extern float MulScanRes;
extern int Break_on;
extern FUN_POMP pump;
extern FUN_MESS FunMessage;
extern int WidthPRS,MaxShtrih;
#else
PAR par;
int viz=0,extr=0;
long na=0,ko=-1,pos1; FILE *fip,*out,*out_rb;
long PRSMAX;
void *low; /*нижняя граница загрузки программы - для контроля*/
const char *s1="Esc - continue;",*s2="Up,Down,Left,Right - Move image;",
*s3="Ctrl/<Up,Down,Left,Right> - Scale image";
const char *abcd="|\\-/";
int count=0,rou=0;
int h_term;/*Счетчик текущей высоты PRS-линии(для записи в терминатор)*/
STAT_COL stat_col;/*Статистика по строкам текущей колонки*/
int dx_prs,dy_prs;/*Габариты PRS-файла*/
int k_prop,k_mono;//Кол-ва пропорц. и моноспейс. колонок и общая оценка моноспейса
int upi;/*Счетчик строк для рисовалки*/
char drive[_MAX_DRIVE],dir[_MAX_DIR],fname[_MAX_FNAME],ext[_MAX_EXT];
char Fullpath[255];
//char NameFuncErr[100],Buff[60]; int NumErr;
int SizeXGlob,SizeYGlob;
REFER *fiv;
int NumCut; FRAME *Cut; //Счетчик и буфер разрезанных компонент
int SizeX_W,SizeY_W;
float AveNumCrossCol,AveNumCross1Col,AveThickCol,AveThick1Col;
int KbAll;
int CountCol,Percent1=5,Percent2=55,Percent3=40,NumColt;
char FileParStr[MAXFILENAME];
uint16_t ScanResolution;float MulScanRes;//Разрешения сканера и нормир.множитель
int Break_on=0; // if == 1 => break process
FUN_POMP pump; // вставляем в цикле
FUN_MESS FunMessage; //в режиме DLL вместо SendMessage
int WidthPRS,MaxShtrih;
#endif

/*Макросы*/
/*dv - вертикал. размер=минимуму из невязок низов,верхов и середин
 dg - горизонт. размер - расстояние между рамками по горизонтали,
 если они не перекрываются, либо размер перекрытия*/
/*#define dist_frm(f1,f2,dist) { int dv,d_up,d_down;\
  d_up=  abs(f1->up - f2->up); \
  d_down=abs(f1->down - f2->down);  \
  dv=abs( ( (f1->up+f1->down) - (f2->up+f2->down) )>>1 );\
  if(d_up < dv) dv=d_up; if(d_down < dv) dv=d_down;         \
  dist=abs(max(f1->left,f2->left) - min(f1->right,f2->right));\
  dist+=par.kv*dv; \
} */
#define dist_frm(f1,f2,dist) { int dv,d_up;\
  d_up=  abs(f1->up - f2->up); \
  dv=abs(f1->down - f2->down); if(d_up < dv) dv=d_up; \
  dist=abs(max(f1->left,f2->left) - min(f1->right,f2->right));\
  dist+=par.kv*dv; \
}

#define DIST_V(u1,d1,u2,d2) (min(d1,d2)-max(u1,u2))

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

//Утилиты
void tik(int k_tik);
void init_font(void);
void mess_f(char *s,int x,int y,int col);
int find1_end(int left,FRAME **str,int ksym,int lev);
int find1_beg(int right,FRAME **str,int ksym,int lev);
void union1_frm(FRAME **frm,int k_frm,BOUND *bnd);

//---------ОПИСАНИЕ ЛИНИЙ------------

#ifndef MAIN5
#error  ...... NOT MAIN5 ......
#endif

#ifdef MAIN5
#include "undef32.h"
//  #include "detlin.h"
#include "lindefs.h"
#include "wind32.h"
int ReadLines(char *FileLine,LINES *Lines1,int *NumStretchAll1);
int TestLines(LINES *Lines1);
int EstAngleLine(LINES *Lines1,float *tg_ang1,int NumStretchAll);
//int PASC CorrectImage(BMPL *obmp,RECT *lpRect,int nRect);
//int PASC PutImage(BMPL *obmp,char *outname,int teep);
int Free_Line(LINES *Lines);
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
{	short xn,xk,yn,yk;//Stretch-Coordinate
	float k; //Тангенс наклон линии y=kx+b
	short thickness; //средняя толщина отрезка
	float angle; //Угол наклона = atan2(..)
	uchar FeatOrient; //HOR_LINE || VER_LINE
	uchar TypeLine; //Тип линии DOT_LINE || SOLID_LINE || DOUBLE_LINE
	short LenShadow; //Длина перекрытия
	void *ptr; //вспомогат. ук-ль (для FindDot)
}STRET;

typedef struct hSTRET_P
{	STRET *Stret;
	short Feat; //POINT_BEG || POINT_END
}STRET_P;

int GetConnectLine(STRET_P *Obj1,STRET_P *Obj2,long ThresEvkl);
int GetConnAxis(STRET *Obj1,STRET *Obj2);
int ClustLine(LINES *Lines, short NumStretchAll,float *tg_ang1,long ThresEvkl,
		int size_x,int size_y,STRET **LineV,short *NumLV,STRET **LineH,short *NumLH,
		FRAME **frm,int *NumFrm);
int MetrRectSoAxis(FRAME *ff,KNOT3 *ptr,KNOT3 **beg_cl);
int UnionEtalonClust(KNOT3 **beg_cl,KNOT3 **bs, int *k_bs,FRAME *ff,RECT **ReUn);
void UnionStret(STRET *Stret,int *IndStretLine,int Num,uchar Orient,
		STRET *Stret1,int RegDouble);
int compKNOT3_STRET_xn(KNOT3 **a,KNOT3 **b);
int compKNOT3_STRET_yn(KNOT3 **a,KNOT3 **b);
int comp_STRET_xn(STRET**a,STRET**b);
int comp_STRET_yn(STRET**a,STRET**b);
int ProcSoAxis(uchar Orient,STRET *Stret1,short NumS,BOUND bnd,
		float ThresDelOrient,short ThresShadow,short ThresDouble,
		int size_x,int size_y,
		KNOT3 **beg_cl,KNOT3 **beg_free1,short *k_cl1,STRET_P **PtrObj,
		STRET **Line1);
short FilterPiecesLines(FRAME **frm,int *k_frm1,
		float kX,float kY,int SizeX,int SizeY,short Thickness);
short FilterPiecesLines1(FRAME **frm,int *k_frm1,STRET *Stret,short NumStret,
		int Reg,short Thickness,short ThrShadow);
short LikeFrmLineV(STRET *s, FRAME *f,short ThrShadow);
short LikeFrmLineH(STRET *s, FRAME *f,short ThrShadow);
short AccumulDelRect(int *nRect,short Num,RECT *Rect,int Reg,STRET *s,
		short MaxDelPix,BMPL *Bmp,float si,float co,float tg_ang);
void CreateRect(int xn,int xk,int yn,int yk,STRET *s,RECT *r,int Reg,
		int *nRect);
int CopyBMPLMem(BMPL *BmpIn,BMPL *BmpOut);
#endif
/**/
//пересчет к не300-dpi SCAN RESOLUTION
#define NORM_SCAN(value) ((int)(MulScanRes*value))
//Диагностика
#include "undef32.h"
#include "tabl.h"
#include "wind32.h"
#define NOT_ALLOC -3
//Основная ф-ция структурайзера
#ifndef GLOB
int PASC est_column(char *file_frm,char *file_prs,INF_TREE **InfStr);
#endif
int PASC FreeInfTree(INF_TREE *Inf);
void PASC SetMessage(FUN_MESS ExtFunMessage);
void PASC SetBreakStr(FUN_POMP Ext_pump);
void PASC OnBreakStr(int ExtBreak_on);
void PASC InitEstStruct(FUN_MESS ExtFunMessage,FUN_POMP Ext_pump,
		char **ParStruct,char **ParTabl,char **ParLine);
void PASC CloseEstStruct(char *ParStruct,char *ParTabl,char *ParLine);
int TestBreak(void);
void FreeAllStruct(FRAME **frm_arr,int k_arr_frm,FRAME **frm,
		STRET *LineV,STRET *LineH,INF_TREE *Inf);
//--Индексы--
int EstBottomStr(FRAME **f,int NumSym,int *Bottom,int dyLow,int dyUpp);
int DetectIndice(FRAME ***sym,int *ksym,int *kstr,BOUND *bnds,int dyLow,int dyUpp);
//--Дроби--
int FindFractCell(FRAME ***sym,int *ksym,STAT_CELL *StatCell);
//--Многоточия--
int FindMultiPointCell(FRAME ***sym,int *ksym,int kstr,STAT_CELL *StatCell);
//--Внутриячеистый фильтр--
int FilterNoiseCell(KNOTT *Knott,STAT_CELL *StatCell,INF_CELL *InfCell,FRAME **frm);
//--
int GetCriptDir(char *path,int len);
int GetTypeDoc(void);
#ifdef DLL_MOD
// int WINAPI adv4prs_conv(char *prs_name,BMPL *BmpIn,int teep);
// int /*FAR PASCAL _export */ WINAPI _export ConvertImage(char far *infile,char far *outfile,int teepBMPHorFILE,
//            int teep,float tg_ugol);
#endif

//=== K R E S T ===
int EstAnglePlain(FRAME **frm,int NumFrm,float *tg_ang);
int PASC DetectStringRECT(RECT *Rect,int NumFrm,float tg_ang,
		RECT ***StrRect1,int **NumSym1,int *NumStr1);
//=== P R A F A X ===
int PASC InitExtrSym
(char *FileRb,int WidthPRS,FRAME ***frm,int *NumFrm,int Reg,FRAME *kramki);
void PASC CloseExtrSym(FRAME **frm);
int PASC DetectLineRaster(char *file_pcx,char *FileOut,STRET **Stret1,
		int *NumStretchAll1);
int PASC SaveLine(int flPRA,STRET *Stret,int NumStret);
int PASC ExtractComp(PRS_ONE *sym1,long *LenSym,FRAME *ff,char **symH1);
int PASC ExtractComp3(PRS_ONE *sym1,long *LenSym,FRAME *ff,char **symH1);
//=== Library ===
int PASC EstAnglePlainImage(char *File,float *tg_ang);
int PASC DeSkewlePlainImage(char *FileIn,char *FileOut,float tg_ang);

//TypeCall: CALL_BOX - without open output PRN-file, CALL_STR - with ...
#define CALL_STR 0
#define CALL_BOX 1

int init_file_prs (char *file_prs,FRAME **frm,int NumFrm,int TypeCall);
int init_file_prs3(char *file_prs,FRAME **frm,int NumFrm,int TypeCall);
void FreePrsLine(void);

int ProjectFrm(FRAME **frm,int NumFrm,float tg_ang);
int ProjectFrm1024(FRAME **frm,int NumFrm,int32_t Skew1024);
int EstIntrvlHor(FRAME **frm,int num,BOUND *bnd,int dxAS,int dyAS,
		RECT *Limit,int MinVol,float MinPerc,int limDX,int limDY,
		int *dsym,int *AveX,int *AveY);

#pragma pack()

#ifndef CPP
#ifdef __cplusplus
}
#endif
#endif

#define STRUCT_INI "struct.ini"

#ifdef __cplusplus
// extern "C" {
#endif

void u4sort(void *base, int num, int width, int(*compare)());
int clust_as(FRAME **frm, int k_frm, KNOT3 **beg_pr, TYPE dp, int size_x,
		int size_y, BOUND *bnd, int max_cl, int *k_cl1, KNOT3 **beg_free1,
		KNOT3 **beg_cl, TYPE(*dist_frame)(FRAME*, FRAME*), int reg);

#ifdef __cplusplus
// }
#endif

#include "undef32.h"
