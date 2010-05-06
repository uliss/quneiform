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

#include "recdefs.h"

#define SUB_ZN
#define SUB_FRM
#ifdef WIN_MOD
#define FUL
#endif

#define DELTA_TITLE 4

/**
 * Значения поля  TITLE_WORD.W_GEN.W_Spell
 */
enum rfrmt_spell_t
{
    /**
     *  слово не пропущено через SPELL
     *  (либо вообще не было обращения к SPELL_CHECK, либо слово недостойное);
     */
    SPELL_NOT = 0,

    /**
     * слово либо его производное по правилам словообразования найдено;
     */
    SPELL_FIND = 1,

    /**
     * в пределах данного радиуса поиска нашлись подсказки к слову
     */
    SPELL_CORR = 2,

    /**
     * данное слово есть часть слова (либо за счет переноса, либо за счет слияния слов)
     */
    SPELL_PART = 3,

    /**
     * слово либо его производное по правилам словообразования не найдено;
     */
    SPELL_NOFIND = 4
};

#define FIRST 0
#define NEXT 1

#define MAX_SYM_WORD 100
#define PROC_SPELL 15
#define PROC_OPEN  14

#define MAX_VALUE 10

#pragma pack(1)

struct SRECT
{
        int left, top, right, bottom;
};

const int LEN_START = 3;

struct COOR_COMP
{
        uchar start_pos[LEN_START];
        uchar buf;
        short lenght;
};

struct TITLE_ZN
{
        uchar Z_Code; // identification code header (= 0)
        ID_SYM Z_Id; // identification code
        uchar Z_Gen; //Родословная знакоместа
        SRECT Z_Rect;// Frame
        SRECT Z_RealRect;// Real frame
        uchar Z_Num_Alt;// # of alternatives
        uchar Z_NumComp;
};

struct ALT_ZN
{
        uchar a_Code; //Код символа
        uchar a_Base; //Номер базы
        uchar a_Prob;
        uchar a_language;
        uchar a_SpellNoCarrying;
        uchar a_FlagCupDrop;
        uchar a_Spell;
        float a_Dist;//Дефект распознавания
};

struct ZN
{
        TITLE_ZN Title; //Заголовок знакоместа
        ALT_ZN Alt[REC_MAX_VERS];//Альтернативы
};

struct W_GEN
{
        uint16_t W_NumSym; //Число символов
        uchar W_Spell; //Spell-Check
        uchar Reserv[2];
        uint16_t FontNumber;
        uint16_t FontSize;
};

struct ALT_SPELL
{
        uchar Len; //Длина подсказки с завершающим \0 (счет - с 1)
        uchar Reserv;
        int Penalty;//Штраф в метрике SPELL до данной альтернативы
        char *Alt; //Сама альтернатива - строка в формате С
};

struct TITLE_WORD
{
        uchar Z_Code; //Опознавательный код заголовка (=1)
        W_GEN W_Gen; //Родословная слова
        //SRECT W_Rect;//Рамка
        int NumAltSpell; //Число подсказок
        int PosFirstCorr;//позиция первой правки типа Ins,Del
        ALT_SPELL *AltSpell;//Подсказки
        uint16_t W_Attr; //Атрибуты слова
#ifndef ID4
        uchar Dummy[DELTA_TITLE];
#endif
};

struct S_GEN
{
        uint16_t S_NumWord;//Число слов в строке
        uchar HeadLine;
        uchar Reserv[2];
};

struct TITLE_STR
{
        uchar Z_Code; //Опознавательный код заголовка (=2)
        S_GEN S_Gen; //Родословная строки
        SRECT S_Rect; //Base Line
        SRECT S_Real_Rect; //Координаты строки
        uint16_t S_Attr; //Атрибуты строки
        uint32_t S_Flags; //NEGA_STR vmk 10-06-2001
#ifndef ID4
        uchar Dummy[DELTA_TITLE];
#endif
};

struct RECOG_MODES
{
        uchar s[10];
};

//Стр-ры описания узлов дерева колонок в файле
typedef struct h_addrXX
{
        short ind, lev;
} ADDR; //Древес.адрес=Nна уровне+Nуровня

typedef struct h_lev
{
        SRECT bnd;
        uint16_t SpecInt;
        int kp;
        ADDR *Addr;/*[kp+1],если kp>=0*/
} LEV;

struct TITLE_FUL
{
        uint16_t wFHSize; //Размер заголовка файла
        uint16_t wZHSize; //Размер локал.заголовка знакоместа, слова или строки
        uint32_t dNumZ; //Количество знакомест в файле
        char StrLogo[16];
        char StrVer[2];//Номер версии без точки
        RECOG_MODES Recog_Modes;//Характеристики процесса распознавания
        uint32_t FntNameOffset;//Смещение секции имен баз в файле
        uint32_t ColOffset;//Смещение секции колонок в файле
        uint32_t ZOffset; //Смещение секции знакомест в файле
        uchar ProcSpell;//Признак обработки Spell
        uchar Reserv_1;
        uint16_t nWord; // Число слов
        uint16_t nStr; // Число строк
        uchar Reserv[26];
        //uchar *Reserv;//malloc(size=wFHSize-46)
};

#pragma pack(2)

struct PAR_FUL
{
        char KodNoRecog;//Код нераспознан. символа для b_find
        char KodNoRecogOut;//Код нераспознан. символа для выходного текста
        int NumAlt; //Макс.число искомых подсказок для SPELL
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
        uint32_t IntervalTimer;
        //Пока не используются
        int ThresholdDist;//Порог в словной метрике для SPELL(радиус поиска в базе)
        float k1; //Weight NoCoincide Char
        float k2; //Weight NoRecog Char
        float gam_Ins;
        float gam_Del;
};

//---Прототипы функций
int FreeStructFull(void);
//--SpelNewOneWord--
typedef int (CALLBACK *FUNC_GEN_FULWORD)(ID_SYM id, ZN *z, int MaxZn);
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

typedef int (*REGLE)(uchar kod);

#define FEAT_LET struct h_feat_let
FEAT_LET
{
        uint Lang :1; // 0 - R, 1 - E
        uint Let :1; // 1 - Letter, 0 - нет
        uint Chif :1; // 1 - цифра (0...9 * + - =)
        uint Shift :1;// 0 - Upper Case, 1 - Lower Case
        uint PosUpLine :1;//Позиция на верх.линейке:=UPP,если выступает и LOW,если на линейке
        uint PosDownLine :1;//Позиция на ниж.линейке:=UPP,если выступает и LOW,если на линейке
        uint ImUppLow :1;//Сходство начертания больших и малых букв
        uint DelimSubWord :1;// - ( ) [ ] { } " '
        uint IndCl :6;//Индекс кластера перепутыванмя,если > 0
        uint Reserv :2;
        //uchar RusLat;//Коды сходных по написанию букв др.яз. или 0
};

uchar Get1Alt(ZN *z, int na);
//---------RTF-converter----------
void MyUnionRect(SRECT *s1, SRECT *s2, SRECT *u);

#pragma pack(2)
//---Из секции колонок---
extern int16_t NumCol;
extern int16_t *NumStr;//[nc]
extern uint32_t *UserNumber;//[nc]
extern uint32_t *FragFlag;//[nc]
extern SRECT *BndCol;//[nc]
extern STAT_COL *StatCol;//[nc]
extern int k_col[MAX_LEV + 1], k_lev;
//---Из секции знакомест---
extern ZN ****Zn;//[nc][ns][nw][nz]
extern TITLE_STR **TitleStr;//[nc][ns]
extern TITLE_WORD ***TitleWord;//[nc][ns][nw]
//--FullSpellFull
extern PAR_FUL par_ful;
//--КОРРЕКТОР ПО ОДНОРОДНОСТИ
extern FEAT_LET FeatLet[256];
//---WORK---
extern uchar buf[1024];

