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

#ifdef __cplusplus
extern "C" {
#endif

#include "wind32.h"
#include "recdefs.h"

//typedef unsigned char BYTE;


#define SUB_ZN
#define SUB_FRM
	#ifdef WIN_MOD
	    #define FUL
	#endif
//#define MAX_ALT 4
#define DELTA_TITLE 4
//#define CMP
//------Значения поля TITLE_WORD.W_GEN.W_Spell-------
// SPELL_NOT - слово не пропущено через SPELL (либо вообще не было обращения
//   к SPELL_CHECK, либо слово недостойное);
// SPELL_FIND - слово либо его производное по правилам словообразования найдено;
// SPELL_NOFIND - слово либо его производное по правилам словообразования не найдено;
// SPELL_CORR - в пределах данного радиуса поиска нашлись подсказки к слову
// SPELL_PART - данное слово есть часть слова
//     (либо за счет переноса, либо за счет слияния слов)
#define SPELL_NOT  0
#define SPELL_FIND 1
#define SPELL_CORR 2
#define SPELL_PART 3
#define SPELL_NOFIND 4
//---Кодировка языков (Может быть сумма нескольких слагаемых)---
//#define CYR 1
//#define UKR 2
//#define ENG 4

#define FIRST 0
#define NEXT 1

#define MAX_SYM_WORD 100
#define PAR_FUL struct h_par_ful

#define PROC_SPELL 15
#define PROC_OPEN  14

#define MAX_VALUE 10

#pragma pack(1)

//#define ID_SYM struct h_id_sym /*Идентификатор символа*/
#define SRECT struct h_srect
#define ZN struct h_zn
#define TITLE_ZN struct h_title_zn
#define ALT_SPELL struct h_alt_spell
#define TITLE_WORD struct h_title_word
#define TITLE_STR struct h_title_str
#define TITLE_FUL struct h_title_ful
#define W_GEN struct h_w_gen
#define ALT_ZN struct h_alt_zn
#define S_GEN struct h_s_gen
#define RECOG_MODES struct h_recog_modes
#define COOR_COMP struct h_coor_comp
//ID_SYM { unsigned char col,str,comp,word; };
SRECT {int left,top,right,bottom;};
#define LEN_START 3
COOR_COMP
{
	BYTE  start_pos[LEN_START];
	BYTE  buf;
	short lenght;
};
TITLE_ZN { BYTE Z_Code; //Опознавательный код заголовка (=0)
           ID_SYM Z_Id; //Идентификатор символа
           BYTE Z_Gen;  //Родословная знакоместа
           SRECT Z_Rect;//Рамка
           SRECT Z_RealRect;//Реальная рамка
           BYTE Z_Num_Alt;//Число альтернатив
           BYTE Z_NumComp;
           #ifdef CMP
            COOR_COMP CoorComp;//Адрес компоненты (компонент)
           #endif
         };
ALT_ZN { BYTE a_Code; //Код символа
         BYTE a_Base; //Номер базы
         BYTE a_Prob;
         BYTE a_language;
         BYTE a_SpellNoCarrying;
         BYTE a_FlagCupDrop;
         BYTE a_Spell;
         float a_Dist;//Дефект распознавания
       };
ZN { TITLE_ZN Title;     //Заголовок знакоместа
     ALT_ZN Alt[REC_MAX_VERS];//Альтернативы
   };

W_GEN { WORD W_NumSym; //Число символов
        BYTE W_Spell;  //Spell-Check
        BYTE Reserv[2];
		WORD FontNumber;
		WORD FontSize;
      };
ALT_SPELL { BYTE Len; //Длина подсказки с завершающим \0 (счет - с 1)
            BYTE Reserv;
            int  Penalty;//Штраф в метрике SPELL до данной альтернативы
            char *Alt; //Сама альтернатива - строка в формате С
          };

TITLE_WORD { BYTE Z_Code; //Опознавательный код заголовка (=1)
             W_GEN W_Gen; //Родословная слова
             //SRECT W_Rect;//Рамка
             int NumAltSpell; //Число подсказок
             int PosFirstCorr;//позиция первой правки типа Ins,Del
             ALT_SPELL *AltSpell;//Подсказки
             WORD W_Attr; //Атрибуты слова
             #ifndef ID4
              BYTE Dummy[DELTA_TITLE];
             #endif
           };

S_GEN { 
			WORD S_NumWord;//Число слов в строке
		    BYTE HeadLine;
			BYTE Reserv[2];
      };

TITLE_STR { 
			BYTE     Z_Code;       //Опознавательный код заголовка (=2)
            S_GEN    S_Gen;        //Родословная строки
            SRECT    S_Rect;       //Base Line
			SRECT    S_Real_Rect;  //Координаты строки
            WORD     S_Attr;       //Атрибуты строки
			Word32   S_Flags;      //NEGA_STR vmk 10-06-2001
            #ifndef ID4
				BYTE Dummy[DELTA_TITLE];
            #endif
          };
RECOG_MODES 
{ 
	BYTE s[10]; 
};

//Стр-ры описания узлов дерева колонок в файле
typedef struct h_addrXX 
{
	short ind,lev;
} ADDR; //Древес.адрес=Nна уровне+Nуровня

typedef struct h_lev
{
	SRECT bnd; WORD SpecInt; 
	int kp; 
	ADDR *Addr;/*[kp+1],если kp>=0*/
} LEV;

TITLE_FUL
{ WORD wFHSize; //Размер заголовка файла
  WORD wZHSize; //Размер локал.заголовка знакоместа, слова или строки
  DWORD dNumZ;  //Количество знакомест в файле
  char StrLogo[16]; 
  char StrVer[2];//Номер версии без точки
  RECOG_MODES Recog_Modes;//Характеристики процесса распознавания
  DWORD FntNameOffset;//Смещение секции имен баз в файле
  DWORD ColOffset;//Смещение секции колонок в файле
  DWORD ZOffset;  //Смещение секции знакомест в файле
  BYTE ProcSpell;//Признак обработки Spell
  BYTE Reserv_1;
  WORD nWord;    // Число слов
  WORD nStr;     // Число строк
  BYTE Reserv[26];
  //BYTE *Reserv;//malloc(size=wFHSize-46)
};
#pragma pack(2)
#define PAR_FUL struct h_par_ful
PAR_FUL
{ char KodNoRecog;//Код нераспознан. символа для b_find
  char KodNoRecogOut;//Код нераспознан. символа для выходного текста
  int  NumAlt; //Макс.число искомых подсказок для SPELL
  int IndexDelta; //Индекс набора функций расчета метрики на словах
  float RelKrit;//Критерий отношения ближайших соседей
  float RelDistAltOCR;//Критерий отношения дистанций OCR-Alt
  // 0 - стандартный SPELL-способ
  // 1 - модульная метрика Spell-OCR
  // 2 - квадратичная метрика Spell-OCR
  int AllowSpell;
  //Параметры корректора по однородности
  int AllowCorr; //Разрешение коррекции слова по признакам однородности
  int AllowCorrShift; //Разрешение коррекции Shift
  int AllowCorrChifLet; //Разрешение коррекции Б/Ц
  int AllowCorrRusLat; //Разрешение коррекции по языку
  int AllowCorrFnt; //Разрешение коррекции по шрифту
  float DelStick; //Критерий выпрыгивания за рамку строки
  int AllowTestChifCl;//Разрешение использования внеш.Alt при тесте Б/Ц
  int AllowTestLangCl;//Разрешение использования внеш.Alt при тесте Рус/Лат
  int AllowCorrChifCl;//Разрешение использования внеш.Alt при Corr  Б/Ц
  int AllowCorrLangCl;//Разрешение использования внеш.Alt при Corr  Рус/Лат
  DWORD IntervalTimer;
  //Пока не используются
  int  ThresholdDist;//Порог в словной метрике для SPELL(радиус поиска в базе)
  float k1; //Weight NoCoincide Char
  float k2; //Weight NoRecog Char
  float gam_Ins;
  float gam_Del;
};

//---Прототипы функций
//int OpenFullOut(char *FileName);
//int FormatStr(int nc,SRECT *Bnd,char ***Str,int **Ksym,int ncOut);
BYTE KoderSym(BYTE Base,BYTE Kod);
int PASC GenFullTxt(char *FileNameFul,char *FileNameOut);
int AnalysLocationPairRect(SRECT b1,SRECT b2);
int BelongPointRect(SRECT b1,int x,int y);
int CalcMoveSep(SRECT b1,SRECT b2,int *dx1,int *dy1);
int OrderBndTxt(SRECT *BndColTxt);
void MoveOneCol(SRECT *Bnd,int dx,int dy);
int FreeStructFull(void);
int RtfFreeStructFull(void);
//int SaveFull(char *FileName);
void init1_par_ful(void);
//int ChooseBestWord(ZN *Zn,TITLE_WORD t,char *word,float k1,float k2,
//    float gam_Ins,float gam_Del);
//int ChooseRelationMin(TITLE_WORD t);
float FindPosDel(ZN *Zn,int kz,char *y,float k1,float k2,float D_Max,float gam,
      int *pos);
float FindPosIns(ZN *Zn,int kz,char *y,float k1,float k2,float D_Max,float gam,
      int *pos);
float DistDel(ZN *Zn,int kz,char *y,float k1,float k2,float D_Max,float gam,
      int pos);
float DistIns(ZN *Zn,int kz,char *y,float k1,float k2,float D_Max,float gam,
      int pos);
float DistEqNum(ZN *Zn,int kz,char *y,float k1,float k2,float D_Max);
float DistAltZn(ZN x,char y,float k1,float k2,float D_Max);
//float DistMaxWord(ZN *Zn,int kz);
//float Dist2MaxWord(ZN *Zn,int kz);
//--FullSpellFull--
//int GenFullSpellFull(char *FileNameFul,char *FileNameOut,int CodLang,
//    char *NameCorr);
int PASC GenFullSpellFull(char *FileNameFul,char *FileNameOut,INF_TREE *Inf);
int GetNextAltWord(ZN *WordAlt,int k,int Reg,int CodLang,uchar *word,float *dist);
int TstCarry(int nc,int ns,int nw);
//int TestPunctuationMarks(char sym);
//int init_ful();
int AllowWordLang(uchar *word,int CodLang);
int TstCarry(int nc,int ns,int nw);
int PutStructAltSpell(BYTE *buf,WORD  *Dist,TITLE_WORD *t);
int InsertWord(int nc,int ns,int nw,ZN *zi,int kz);
//int TstNameOwr(uchar LastCod,uchar FirstCod);
//uchar GetAlt(int nc,int ns,int nw,int nz,int na);
//int PASC InitSpell(char *PathName,int cod,int TypeDoc1);
#ifndef WIN_MOD
//void AnsiToOem(const char *word1,char *word2);
//void OemToAnsi(const char *word1,char *word2);
#endif
//WORD Penalty1LenWord(int n);
int NormParSpellOCR(ZN *z,int kz,int flo,WORD *reinst,WORD *reinstb,WORD *redel,
    WORD *reelse,WORD *reunknow,float *del,WORD *porog);
int Norm2ParSpellOCR(ZN *z,int kz,int flo,WORD *reinst,WORD *reinstb,WORD *redel,
    WORD *reelse,WORD *reunknow,float *del,WORD *porog);
int FindWord(int flo,ZN *z,BYTE *word,BYTE *buf,WORD *DistMin,WORD *porog1,
TITLE_WORD *t,WORD MaxAltSpell);
//void SetTimerOwn(DWORD NumMs);
//int PASCAL _export killOwn(void);
//--SpelOneWord--
int CloseSpell(void);
int PASC SpellOneWord(uchar *in,uchar *out,int MaxAltSpell);
int PASC CorrOneWord(uchar *in,uchar *out,
         BYTE LastCod,int fl_all,int HeadLine,SRECT *bnds);
//--SpelNewOneWord--
typedef int (CALLBACK *FUNC_GEN_FULWORD)(ID_SYM id,ZN *z,int MaxZn);
//int  PASC InitOneSpell(FUNC_GEN_FULWORD AddrFuncGenFulWord);
//void PASC CloseOneSpell(void);
int PASC FindOneWord(ID_SYM id,int Reg,char *OutWord);
int PASC FindOneWord1(ZN *zBuf,int NumZnWord, int Reg,char *OutWord);
//--Query FullFile--
int InsertWordFirstAlt(int nc,int ns,int nw,char *s,SRECT *frm);
//int DelWordFul(int nc,int ns,int nw);
//--util--
void bounds_rect(int ii,SRECT *bnd,int nx);
void image_r(SRECT *b);
//---КОРРЕКТОР ПО ОДНОРОДНОСТИ
//Возможные языки слова
#define RUS 0
#define LAT 1
//Признаки Ц/Б
#define CHIF 0
#define LET 1
//Верхний/нижний регистры
#define UPP 0
#define LOW 1
//Возможные позиции знакомест в слове
#define FIRST 0
#define FIRST_NEXT 1
#define MIDDLE 2
#define DELIMITER 3
#define NO_LET 4
//Значения параметра Action
#define TEST 0
#define CORR 1

#define PRECIS 2

#define MAX_CLUST 64
#define MAX_CLUST_COM 9
#define MAX_CLUST_ONE 9
#define MAX_ELEM_CLUST 19

typedef int (*REGLE)(BYTE kod);

#define FEAT_LET struct h_feat_let
FEAT_LET
{ uint Lang:1; // 0 - R, 1 - E
  uint Let:1;  // 1 - Letter, 0 - нет
  uint Chif:1; // 1 - цифра (0...9 * + - =)
  uint Shift:1;// 0 - Upper Case, 1 - Lower Case
  uint PosUpLine:1;//Позиция на верх.линейке:=UPP,если выступает и LOW,если на линейке
  uint PosDownLine:1;//Позиция на ниж.линейке:=UPP,если выступает и LOW,если на линейке
  uint ImUppLow:1;//Сходство начертания больших и малых букв
  uint DelimSubWord:1;// - ( ) [ ] { } " '
  uint IndCl:6;//Индекс кластера перепутыванмя,если > 0
  uint Reserv:2;
  //BYTE RusLat;//Коды сходных по написанию букв др.яз. или 0
};

//int Init_FeatLet(void);
uchar Get1Alt(ZN *z,int na);
int GetFeatLet(ZN *z,int fl_all,int fl_Ext,REGLE Regle);
int R_Chif(BYTE cod);
int R_Let (BYTE cod);
int R_Rus (BYTE cod);
int R_Lat (BYTE cod);
int R_Upp (BYTE cod);
int R_Low (BYTE cod);
int TestWordFeat(ZN *z,int kz, int fl_all,REGLE Regle1, REGLE Regle2);
int TestWordChifLet(ZN *z,int kz,int fl_all);
int TestWordLang(ZN *z,int kz,int fl_all);
int CorrFeat(ZN *z,int kz,REGLE Regle);
int CorrWordChifLet(ZN *z,int kz,int fl_all);
int CorrWordLang(ZN *z,int kz,int fl_all);
int CorrWord(ZN *z,int kz,BYTE LastCod,int fl_all,int HeadLine,SRECT *bnds);
int TestStickUpp(ZN *z,SRECT *bnds);
int GetFeatShift(ZN *z,int fl_all,SRECT *bnds);
int TestWordShift(ZN *z,int kz,int fl_all,SRECT *bnds);
int CorrWordShift(ZN *z,int kz,BYTE LastCod,int fl_all,int fl_bnd,int fl_ExtAlt,
    int Lang,int HeadLine,SRECT *bnds);
int CorrShift(ZN *z,int flUppLow,int fl_bnd,int fl_ExtAlt,int fl_all,int Lang,
    int PosWord,SRECT *bnds);
int CorrWordFnt(ZN *z,int kz,int fl_all);
int TestWordFnt(ZN *z,int kz,int fl_all);
int GetAllowExtAlt(int Action,REGLE Regle);
int CorrFul(void);
int EstRectWordFul(int nc,int ns);
int EstRectWord(SRECT *frm,int ksym,SRECT *bnd);
int AllowRelDistAltOCR(ZN *z,int alt1,int alt2);
//---------RTF-converter----------
//int FullRtf(char *FileNameFul,char *FileNameOut);
void MyUnionRect(SRECT *s1,SRECT *s2,SRECT *u);

#pragma pack(1)
//#ifdef FUL

#ifndef V_LOCK
  //#define PASCAL
  //#define FAR
  //#include "vocab.h"
#endif
	
//#endif
#pragma pack(2)
#ifdef MAIN2
/* Moved all these to util_spl.cpp.
  //---Из секции колонок---
  int NumCol,SizeSectionCol;
  int *NumStr;//[nc] 
  Word32 *UserNumber;//[nc]
  Word32 *FragFlag;//[nc]
  SRECT *BndCol;//[nc]
  STAT_COL *StatCol;//[nc]
  int len_col,k_frm;
  //int k_colt,slope,space;
  char name[80],name1[LEN_PATH];
  int k_col[MAX_LEV+1],k_lev; LEV **knot; //Дерево
  int fl_EditStruct; //Признак несоответствия дерева терминал. колонкам
  //---Из секции имен баз---
  int SizeSectionFntName; char *FntName;
  //---Из секции знакомест---
  ZN ****Zn;//[nc][ns][nw][nz]
  TITLE_STR **TitleStr;//[nc][ns]
  TITLE_WORD ***TitleWord;//[nc][ns][nw]
  TITLE_FUL TitleFul;
  COOR_COMP *CoorComp; long NumComp;
  //--FullSpellFull
  uchar word[MAX_SYM_WORD+1],word1[MAX_SYM_WORD+1];
  float dist[MAX_SYM_WORD+1],dist1[MAX_SYM_WORD+1];
  PAR_FUL par_ful;
  char PunctuationMarks[256];
  int ExtSettings;
  //char AlphabetAllow[256];
  char WordOne[256];
  uchar Upper[256],Lower[256],UppLow[256],UppLowOther[256],EndSentence[256];
  BYTE KoderRus[256],KoderLat[256];
  int FlCarryStr;
  #ifdef FUL
   // svocabg far *ParSpell;
  #endif
  char FileParSpel[MAXFILENAME],FileParSpelOut[MAXFILENAME];
  int *Pen;
  int MaxValue;
  //--КОРРЕКТОР ПО ОДНОРОДНОСТИ
  FEAT_LET FeatLet[256];
  BYTE **ClustOCR;int K_Clust;//Кластеры перепутывания
  //---WORK---
  BYTE buf[1024],buf1[1024];
*/
#else
  //---Из секции колонок---
  extern int NumCol,SizeSectionCol;
  extern int *NumStr;//[nc]
  extern Word32 *UserNumber;//[nc]
  extern Word32 *FragFlag;//[nc]
  extern SRECT *BndCol;//[nc]
  extern STAT_COL *StatCol;//[nc]
  extern int len_col,k_colt,slope,k_frm,space;
  extern char name[80],name1[LEN_PATH];
  extern int k_col[MAX_LEV+1],k_lev; extern LEV **knot; //Дерево
  extern int fl_EditStruct;
  //---Из секции имен баз---
  extern int SizeSectionFntName; extern char *FntName;
  //---Из секции знакомест---
  extern ZN ****Zn;//[nc][ns][nw][nz]
  extern TITLE_STR **TitleStr;//[nc][ns]
  extern TITLE_WORD ***TitleWord;//[nc][ns][nw]
  extern TITLE_FUL TitleFul;
  extern COOR_COMP *CoorComp; extern long NumComp;
  //--FullSpellFull
  extern uchar word[MAX_SYM_WORD+1],word1[MAX_SYM_WORD+1];
  extern float dist[MAX_SYM_WORD+1],dist1[MAX_SYM_WORD+1];
  extern PAR_FUL par_ful;
  extern char PunctuationMarks[256];
  extern int ExtSettings;
  //extern char AlphabetAllow[256];
  extern char WordOne[256];
  extern uchar Upper[256],Lower[256],UppLow[256],UppLowOther[256],EndSentence[256];
  extern BYTE KoderRus[256],KoderLat[256];
  extern int FlCarryStr;
  #ifdef FUL
 // extern svocabg far *ParSpell;
  #endif
  extern char FileParSpel[MAXFILENAME],FileParSpelOut[MAXFILENAME];
  extern int *Pen;
  extern int MaxValue;
  //--КОРРЕКТОР ПО ОДНОРОДНОСТИ
  extern FEAT_LET FeatLet[256];
  extern BYTE **ClustOCR;extern int K_Clust;
  //---WORK---
  extern BYTE buf[1024],buf1[1024];
#endif
void FreeClustOCR(void);
//#ifdef BLANK
//void ExtrKnot(KNOTT *Curr,KNOTT **back,KNOTT **next);
//void ReverseInsertKnot(KNOTT *Curr,KNOTT *back,KNOTT *next);
//int PASC GenFullTxtfromTree(char *FileNameFul,char *FileNameOut,INF_TREE *Inf);
void PASC OnBreakFrm(int ExtBreak_on);
//int FormatKnot(KNOTT *Knot,SRECT *Bnd,char ***Str,int **Ksym);
//int compKNOTT_RectBottom(KNOTT **a,KNOTT **b);
//int compKNOTT_RectRight(KNOTT **a,KNOTT **b);
int CorrFulCell(KNOTT **AllT,int nT,INF_TREE *Inf);
int CorrCol5_7(int nc);
//#endif
//Изменения Ful-формата
// 1.В заголовке файла после поля ZOffset (смещение секции знакомест)
// добавилось поле BYTE ProcSpell;//Признак обработки Spell
// 2. После чтения OpenFul вместо поля W_Rect (8 байт) в TitleWord - другие поля
//TITLE_WORD { BYTE Z_Code; //Опознавательный код заголовка (=1)
//             W_GEN W_Gen; //Родословная слова
//             //SRECT W_Rect;//Рамка
//             int NumAltSpell; //Число подсказок
//             int PosFirstCorr;//позиция первой правки типа Ins,Del
//             ALT_SPELL *AltSpell;//Подсказки
//             WORD W_Attr; //Атрибуты слова
//           };
//ALT_SPELL { BYTE Len; //Длина подсказки с завершающим \0 (счет - с 1)
//            BYTE Reserv;
//            int  Penalty;//Штраф в метрнике SPELL до данной альтернативы
//            char *Alt; //Сама альтернатива - строка в формате С
//          };

#include "undef32.h"

#ifdef __cplusplus
}
#endif

