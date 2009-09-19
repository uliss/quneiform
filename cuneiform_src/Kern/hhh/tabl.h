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

#ifndef __TABL_H
#define __TABL_H

#include "wind32.h"

//#define DEMO
#ifdef DEMO
  //#define uint  unsigned int
  #define KNOT struct h_knot
  KNOT { KNOT *next,*back; };
  typedef struct {int i;} PAR;
  typedef struct {int i;} STAT_STR;
  typedef struct {STAT_STR *stat_str; int dx_col,dy_col,dsym_col,dy_Low,dy_Upp;} STAT_COL;
  typedef int (*FUN_POMP)(void);
  typedef void (*FUN_MESS)(uint16_t wPar,DWORD lPar);
  typedef struct hSTRET
  { short xn,xk,yn,yk;
    float k;
    short thickness;
    float angle;
    uchar FeatOrient;
    uchar TypeLine;
    short LenShadow;
  } STRET;
  typedef struct { KNOT ***beg_as; KNOT *beg_free;
         int kx,ky,dx,dy,xmin,ymin; BOUND *bnd; } AS;
  //typedef struct {int i;};
#endif
#define SUB_TREE
typedef struct hSUB_ALLOC
{
	char **Ptr;//[NumPtr][SizePtr[i]]Массив указателей (сегментир.адрес.простр-во)
  int NumPtr,CurrPtr;//Число указателей, индекс текущ. сегмента
  long *SizePtr,Size;//[NumPtr]Массив длин сегментов и общей длины простр-ва
  long CurrPos;      //позиция в текущ.сегменте

} SUB_ALLOC;

#define SIZE_SEGL 65500L

//Задание априорной информации о структуре таблиц
//---Иерархия линий(=> и бланка) по типам линий
typedef struct hHIER_LINE
{
	int NumLev;    //Число V- или H-уровней
  uchar *TypeLine;//[NumLev] Типы линий на V- или H-уровнях
} HIER_LINE;

//---Иерархия шапки/боковика таблицы (фиксирована, полностью задается
//номерами терминал. граф/строк таблицы)
typedef struct hKNOT_TABL
{
	int beg,end;//Гранич. номера терминал. граф узла
  char *Name;   //Имя узла | NULL | "NO_OCR" => нутро этого узла не надо OCR-ть
} KNOT_TABL;
typedef struct hCAPTION_TABLE
{
	int NumLev;  //Число уровней шапки/боковика
  int *NumCol; //[NumLev] Числа граф уровней
  KNOT_TABL **TreeCapt; //[NumLev][NumCol[NumLev]] Узлы дерева по уровням
} CAPTION_TABLE;
//--- V- или H-иерархия, определяемая по геометрии текста в ячейках
typedef struct hHIER_CELL_TXT
{
	int NumCol; //Число граф(H|V), ячейки которых задают ортогонал. иерархию
  int *IndCol;//[NumCol] индексы классообразующих граф
} HIER_CELL_TXT;
//---Априор.инф-ция о расположении линий
typedef struct hLINE_INTRVL
{
	int Interval,sko;//интервал до след.соседа-линии и его с.к.о
  uchar TypeLine;
  uchar Align;
} LINE_INTRVL;
typedef struct hLINE_APRIOR
{
	int Num;          //Числа V- или H-линий
  LINE_INTRVL *Line;//[Num] - массивы интервалов V- или H-линий
} LINE_APRIOR;
//---Набор символов графы!!!ADD XOR и т.п.!!!
#ifndef DEMO
typedef struct hCONTENT
{
	uint Digit:1,      //цифра
  Letter:1,          //буква
  LowerCase:1,       //допустимы ли малые буквы
  UpperCase:1,       //--//-- большие
  PunctuationMarks:1,//знаки препинания
  MultiPoints:1,     //многоточие
  Indice:1,          //индексы
  Fraction:1,        //дроби
  ProperName:1,      //первое слово ячейки - имя собственное
  All:1,             //любой символ
  buf:6;             //резерв
  uchar *SpecSymb;    //здесь перечислены в форме С-строки возможные отдельные
                     //коды или NULL
} CONTENT;
#endif
#define IN_NO -30000
//Содержимое графы
#define ALG_BEG 0
#define ALG_END 1
#define ALG_CENTER 2
#define TRUEinBLOCK 2
//Типы узлов
#define ROOT 0
#define TXT  1
#define CELL 2
#define CELL_CAPT 3
#define HIER_V 4
#define HIER_H 5

#define INDEF -32000

#ifndef DEMO
typedef struct hINF_CELL
{
	CONTENT Content;
  uint JustH:2,   //гориз.выравнивание(LEFT/RIGHT/CENTER)
  JustV:2,        //верт. выравнивание(TOP/BOTTOM/CENTER)
  FontSizeConst:1,//постоянный размер шрифта в колонке
  FontStyle:3,    //комбинация бит.флагов Normal|Bold|Italic
  MultiStr:1,     //многострочная ячейка
  MultiWord:1,    //многословная ячейка
  TreeCell:1,     //возможна ли древовидная структура ячейки
  SortedNumber:2, //есть ли порядок на ячейках всей графы(TRUE),
                  //либо в пределах строчного блока(TRUEinBLOCK)
                  //либо нет порядка (FALSE) (имеет смысл только для
                  //граф, CONTENT которых=DIGIT
  buf:3;          //резерв
} INF_CELL;
#endif

typedef struct hPAR_TABL
{
	char *TitleTable;
  int NumColV;
  HIER_LINE HierLineH,HierLineV;
  CAPTION_TABLE CaptionH,CaptionV;
  HIER_CELL_TXT HierCellTxtH,HierCellTxtV;
  LINE_APRIOR LineH,LineV;
  INF_CELL *InfCell;//[NumColV]
} PAR_TABL;
typedef struct hSMP_LINE
{
	int Coor,Interval,sko;//Коор-та оси линии,интервал до соседа ,с.к.о. интервала
  uchar TypeLine; //Тип линии
  uchar Align;
  int Num;       //Число измерений, соотнесенных с этим эталоном
  int LenShad;   //Длина взаим.затенения
  int Ave;       //Усредненная коор-та измерений
  int AveThick;  //Усредненная толщина линии
} SMP_LINE;
typedef struct hCOOR_IDENT
{
	short Coor,Thick;
  uchar Id; //TRUE или FALSE, если линия не соответствует реальной линии,
  uchar TypeLine;
} COOR_IDENT;
typedef struct hKNOTT
{
	struct hKNOTT *next,*back,*up,*down;
  RECT Rect;//Пиксел./текст. рамка узла либо индексы гранич.линий
  int InBegFrm,NumFrm;//Индекс начал.рамки и число рамок узла
  int InColA;         //Индекс априор.графы(ParTabl.InfCell[InColA])
  //int InTerm;       //Индекс терминал.ячейки(sym[InTerm][nstr][ncomp])
  //поле InTerm м.б. совмещено с полем InBegFrm
  uint OrderChild:4,  //UNSORT,VER,HOR,MATR,TABL,FORMUL,TERM
       Type:4,        //TXT,TITLE,CELL,CELL_TITLE,CELL_CAPT,HIER_V,HIER_H
       AllowOCR:1,    //TRUE/FALSE - должен ли узел идти на распоз-ние символов
       JustH:2,       //Гориз.выравнивание: ALG_BEG/ALG_END/ALG_CENTER
       JustV:2,       //Верт. выравнивание: --/--
       dummi:3;
  //В случае AllowOCR=FALSE м.б. ссылка на строку текста данного узла
  //Эта ссылка м.б. задана след. образом:
  //--Эконом.способы--
  //1. InBegFrm - индекс строки в одномер. массиве строк
  //2. union {int InBegFrm,NumFrm; char *str;}
  //3. InBegFrm,NumFrm - ParTabl.CaptionV.TreeCapt[InBegFrm][NumFrm]
  //4. Просто новое поле char *str;
  //Способ 3 ограничивает поля, кот. не надо OCR-ть, лишь верт. шапкой => не ага
  //Способ 1 пригоден, если заводить спец. одномер. массив для строк => не ага
  //Выбор м/способами 2 и 4:
  //(2) не удобно тем, что появл. дополнит. уровень и вдруг все-таки надо будет
  //иметь доступ к компонентам узла, кот. не надо узнавать
  char *Name;
  struct hKNOTT *RefOrt; //Ссылка на узел куста ортогонал. иерархии
  struct hKNOTT *RefH;   //Эквигоризонт.ссылка на V-узлах
  //Способы: если лишь для терминал.узлов, то ее можно совместить с down,
  //иначе отдел. ссылка (? а нельзя ли совместить поля str и ReferOrt?)
  //??А нужны ли вообще ссылки м/ортогонал.иерархиями, т.к. InTerm - уже
  //задает идентификатор терм.ячейки и вообще м.б. не понадобится
  //при построении дерева и форматировании по нему перескакивать на ортог.TREE
} KNOTT;
typedef struct hTREE2
{
	KNOTT *Root,*free; //Корень и голова списка свобод. элементов дерева
  int NumKnot;       //Общее число узлов в списк. пространстве
  int NumSeg;KNOT **ArrSeg; //Сегментир. массив
} TREE2;
typedef struct hSEG_ARR
{
	KNOT3 *free; //голова списка свобод. элементов дерева
  int NumKnot;//Общее число узлов в списк. пространстве
  int NumSeg;KNOT **ArrSeg; //Сегментир. массив
} SEG_ARR;
typedef struct hLINE_KNOT //Описание линии для узлов дерева
{
	int beg;       //Для V-Line это левая граница толстой линии
  int BegTxt;    //Текстовая коор-та
  uchar Thick;    //Толщина
  uchar TypeLine; //Тип
} LINE_KNOT;
typedef struct hKNOT_TMP {KNOTT *Knot; int in;} KNOT_TMP;

typedef struct hSTAT_CELL //Статистика по ячейке
{
	int dx,   //стандарт.ширина
      dyLow,//-/- высота строч.
      dyUpp,//-/- высота пропис.
      dsym, //-/- межсимвол.интервал
      dstr, //-/- межстроч. интервал
      HeiStr;//высота строки с межстроч.интервалом
} STAT_CELL;
typedef struct hINDEX_SORT {int ind,value;} INDEX_SORT;

typedef struct hINF_TREE
{
	 int       TypeDoc; //Признак: PLAIN - PlainText, ...
  TREE2     Tree;//Дерево
  KNOTT     *Table,*HierV,*HierH;//Некоторые узлы дерева
  KNOTT     **TermV;//[NumTV] - терминал. V-графы
  int       NumTV;
  STAT_CELL *StatCell;////[NumTV] - Стат-ка по терм. V-графам
  PAR_TABL  ParTabl;
  PAR       *ParStr;
  LINE_KNOT *LineVK,*LineHK;//[nH],[nV]-разграфка дерева горизонталями и вер-ми
  int       nH,nV;
  //++Индексы ключевых линий
  int       InLineBotTabl;
  //++Стр-ра терм. ячеек
  KNOTT     **ColT;//[NumT] - терминал. ячейки
  int       NumT;
  //++Описание листа
  float     slope;
  int       DislocationX,DislocationY;//параметры сдвига
  int       MonoSpace,MonoSpaceTrue; //толщина штриха и истин.моноспейс
  int       SizeXGlob,SizeYGlob;
  int       NumFrm;
  //++Описания терминал.ячеек
  BOUND     bnd_page,*bnd_col,**bnd_str;
  FRAME     ***sym;
  int       k_colt,**k_sym;
  int16_t  	*k_str;
  STAT_COL  *StatCol;
  POS_STR   **PosStr;
  char      FilePrc[255];//имя файла извлечен.компонент
  SUB_ALLOC SubTree;
  FUN_POMP  pump;
  FUN_MESS  FunMessage;
} INF_TREE;

int FilterLineNorveg(STRET *LineV,short *NumLV,STRET *LineH,short *NumLH,
    RECT bnd,short Thick,RECT *BndTabl,short *TopCapt,short *BottomCapt);
int IdentApriorLineV(STRET *LineV,short *NumLV,STRET *LineH,short *NumLH,
 short Thick,RECT *BndTabl,PAR_TABL *ParTabl,short *DelCoor,COOR_IDENT *CoorId);
int DivFrmBox(FRAME **frm,int k_frm,RECT *box,int NumBox,int reg,int DelFrm,
    int *intr);
KNOTT* IncKnot(KNOTT *up,KNOTT *after,KNOTT **free);
void FillFieldKNOTT(KNOTT *ptr,int Left,int Right,int Top,int Bottom,
int InBegFrm,int NumFrm,int InColA,
uint OrderChild,uint Type,uint AllowOCR,uint JustH,uint JustV,char *Name);
int FindLineNumPage(STRET *LineH,short NumLH,FRAME **frm,int NumFrm,
    short TopCapt,RECT *RectTabl,int *Coor);
int CreateTree(BOUND BndTxt,RECT *RectAll,RECT *RectTabl,STRET *LineV,short NumLV,
    STRET *LineH,short NumLH,FRAME **frm,int NumFrm,short Thick,INF_TREE *Inf);
int compKNOTT_InColA(KNOTT **a,KNOTT **b);
int EstSizeKnott(FRAME **f,int NumKnot,KNOTT **Knot,int DYmin,int DYmax,
    int MinNum,int *DYest,int Reg);
int EstSizesCompCell(KNOTT **TermV,int NumCol,STAT_CELL *StatCell,FRAME **f);
int EstInterval(AS *As,int Upp,int Low,int *dstr,int *dsym,int MinNum);
int EstIntervalKnott(FRAME **f,int NumKnot,KNOTT **Knot,
    int MinNum,int Upp,int Low,int *dstr,int *dsym);
int CalcHistCell(int NumKnot,KNOTT **Knot,FRAME **frm,int Orient,int Pixel,
    int *MinCoor,int *MaxCoor,int **Hist,int *NumHist);
int DivColH(FRAME **f,KNOTT *Col,STAT_CELL *StatCell,int *NumIntr,int *intr,
    float k1,float k2,LINE_KNOT *LineHK,int Reg);
int compINDEX_SORT(INDEX_SORT *a,INDEX_SORT *b);
int compLINE_KNOT(LINE_KNOT *a,LINE_KNOT *b);
int SortHorLine(LINE_KNOT *LineHK,int NumH,LINE_KNOT *LineVK,int NumV,
    KNOTT *Root,KNOTT ***colt1,int *k_colt1,FRAME **frm,STAT_CELL *StatCell);
void DelTree(TREE2 *Tree);
void ClearStatCell(STAT_CELL *s);
int EstOneString(FRAME **frm,int k_frm,FRAME ****str1,int **ksym1,int *k_str1);
int PASC InitParTabl(char *File,PAR_TABL *ParTabl);
int PASC FreeParTabl(PAR_TABL *p);

//==Stack

typedef struct hSTACK
{
	KNOTT **arr;//[size] массив указателей
  int pos;//индекс текущ.свобод.позиция
  int size;//размер
} STACK;
int NewStack(int size,STACK *St);
void DelStack(STACK *St);
int PushStack(KNOTT *ptr,STACK *St);
KNOTT *PopStack(STACK *St);
void ClearStack(STACK *St);
int OverflowStack(STACK *St);

/*int FilterPiecesLinesKnot(KNOTT *Knot,int flL,int flR,int flU,int flD,
    int ThickH,int ThickV,int ThickAbsH,int ThickAbsV,int ThickRel,
    FRAME **frm);*/
int FilterPiecesLinesKnot(KNOTT *Knot,int flL,int flR,int flU,int flD,
      int ThickH,int ThickV,int ThickAbsH,int ThickAbsV,int ThickRel,
      FRAME **fRm1,LINE_KNOT *LineH,LINE_KNOT *LineV,STAT_CELL *StatCell);
//LINE_KNOT *LineH,LINE_KNOT *LineV,STAT_CELL *StatCell);

//== TREE2-Utilites

KNOTT *NextKnot(KNOTT *Curr,STACK *St);

//*********Эмулятор SubAllocator***********
int InitSubAlloc(long Size,SUB_ALLOC *Sub);
char *Submalloc(uint size,SUB_ALLOC *s);
void DeleteSubAlloc(SUB_ALLOC *s);
void ClearSubAlloc(SUB_ALLOC *s);

//*********Plain Text колонизация с новым форматом дерева стр-ры*********
int SearchInterval(FRAME **frm,int k_frm,int **beg1,int **end1,
    int *k_int1,BOUND *bnd,int ave_dir,int ave_ort,int reg,int *NumMax);
int SearchColHist(FRAME **frm,int k_frm,BOUND *bnd,int ave_x,int ave_y,int reg,
    int *k_int,int **intr1,int **begI,int **endI,int *NumMax);
int AddLine(LINE_KNOT **Line1,int *nCurr,int *nMax,int Coor,int Thres);
int CreateTreePlainTxt(BOUND BndTxt,STRET *LineV,short NumLV,STRET *LineH,
    short NumLH,FRAME **frm,int NumFrm,INF_TREE *Inf,int size_x,int size_y);

//== Serv. func

void ImageKnot(KNOTT *ptr,LINE_KNOT *LineVK,LINE_KNOT *LineHK,int col,
int line_style,int fill,int ColFrm,FRAME **f,int NumFrm,int NumVK,int NumHK);
void TestKNOTT(KNOTT *ptr,LINE_KNOT *LineVK,LINE_KNOT *LineHK,int NumFrm,int NumVK,int NumHK);
int ImageTree(KNOTT *Root,LINE_KNOT *LineVK,LINE_KNOT *LineHK,
  FRAME **f,int NumFrm,int NumVK,int NumHK);

#include "undef32.h"

#endif
