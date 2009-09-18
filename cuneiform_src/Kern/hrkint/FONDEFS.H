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

#ifndef __FONDEFS_H
#define __FONDEFS_H


#include "recdefs.h"
#include "leodefs.h"

	typedef struct tagFontInfo {
		int count[256];   // how many symbols in biggest cluster
		int width[256];   // symbol width in biggest
		int height[256];  // symbol height in biggest
		int number[256];  // welet number - for fast search
		int numFont[256]; // how many clusters with name
	} FontInfo;

	typedef struct tagSizesInfo {
		int name;
		int count;
		int width;
		int height;
	} SizesInfo;

	typedef struct tagClustInfo {          // information about one cluster
      uchar let;     // ASCII имя
	  uchar weight;  // из скольких собран
	  uchar prob;    // оценка
	  uchar valid;   // валидность (LEO_VALID_...)
	  uchar attr;    // атрибуты (CTB_PRINT_...)
	  uchar mw;      // средняя ширина символов в кластере
	  uchar mh;      // средняя высота символов в кластере
	  uchar kegl;    // типичный кегль
      char reserved[24];
	} ClustInfo;

	// информация о распознаваемом символе
	typedef struct tagFonSpecInfo {
      int16_t  nInCTB;      // вход > 0 - номер в CTB
	  int16_t  nFieldRow;   // вход > 0 - номер строки (поля?)
      int16_t  col;         // вход - координаты
	  int16_t  row;         //
	  uint32_t wFonFlags;   // возврат - флаги frhdefs.h

//Andrey - 20030620 - flags from frhdefs.h
//----------------------------------------
         #define  FFC_EMPTY         0x0001
            // cell is empty, in this case rcFill should contain:
            // rcFill.left =<leftmost> ?<field.left> :<prev_char>.rcFill.right+1;
            // rcFill.right=<rightmost>?<field.right>:<next_char>.rcFill.left-1;
            // rcFill.top   = ave( <prev_char>.rcFill.top,<next_char>.rcFill.top );
            // rcFill.bottom= ave( <prev_char>.rcFill.bottom,<next_char>.rcFill.bottom );
            // so, rcFill width should strictly equals to width of space!

         // cell in real coordinates is determined by rcCell:
         #define  FFC_CELL                   0x0002 // rcCell initialized
         #define  FFC_CELL_STRONG_TOPBOTTOM  0x0004 // if rcCell initialized
         #define  FFC_CELL_STRONG_LEFTRIGHT  0x0008 // if rcCell initialized

      ////////////////////////////////////////////
      // разметка знакомест от шрифтового дораспознавания:
         #define  FFC_FON_CONFIRMED    0x8000                     // пред: FFC_FON_VERIFICATION
            // первая альтернатива однозначно подтверждена

         #define  FFC_FON_TWIN         0x4000
            // несколько первых альтернатив укладываются в близкие кластеры и плохо различимы

	      #define  FFC_FON_ALERT        0x2000
            // знакоместо сомнительно (плохой размер, наличие других кластеров с такой буквой...)
            // но не настолько, чтобы однозначно забраковать

	      #define  FFC_FON_REJECT       0x1000
            // результат сегментации и распознавания знакоместа забракован шрифтом

	      #define  FFC_FON_CHANGE       0x0800
            // произвел замену первой альтернативы

      ////////////////////////////////////////////
      // разметка знакомест от контекста
         #define  FFC_CON_CONFIRMED    0x800000
            // первая альтернатива однозначно подтверждена

         #define  FFC_CON_TWIN         0x400000
            // несколько первых альтернатив укладываются в близкие кластеры и плохо различимы

	      #define  FFC_CON_ALERT        0x200000
            // знакоместо сомнительно
            // но не настолько, чтобы однозначно забраковать

	      #define  FFC_CON_REJECT       0x100000
            // результат сегментации и распознавания знакоместа забракован шрифтом

	      #define  FFC_CON_CHANGE       0x080000
            // произвел замену первой альтернативы

      ////////////////////////////////////////////
      // разметка знакомест от FRH
         #define  FFC_STR_BEG          0x80000000
            // начало строки

         #define  FFC_STR_END          0x40000000
            // конец строки

	      #define  FFC_DBT_BEG          0x20000000
            // начало сомнительного фрагмента

	      #define  FFC_DBT_END          0x10000000
            // конец сомнительного фрагмента

	  int16_t  nClust;      // возврат - номер лучшего кластера
	  int16_t  countRazmaz;
	  uchar  nLet;        // вход > 0 - имя ASCII
	  uchar  onlySelf;
      int16_t  h;          // вход - размеры
	  int16_t  w;          //
	  uchar  palkiLeo;   // вход != 0 - распознавать палки по ЛЕО
	} FonSpecInfo;

#define MAXCHECKALT 4
	// информация о результатах проверки символа
	typedef struct tagFonTestInfo {
		int16_t nClust; // какой кластер
		uchar prob;
		uchar flag;
		uchar kegl;
		uchar reserv[3];
	} FonTestInfo;

	// данные о кластерной базе в CTB-файле
	typedef struct tagFonClusterBase{
		uchar  oem;
		uchar  fullX;
		uchar  fullY;
		uchar  let;
		uchar  w;
		uchar  h;
		uchar  weight;
		uchar  porog;
		uchar  mw;
		uchar  mh;
		uchar  prob;
		uchar  attr;
		uint16_t fill;
		uint16_t num;
		uint16_t invalid;
		uchar  valid;
		uchar  kegl;
        int16_t  sr_col;
		int16_t  sr_row;
		uint32_t summa;
		uint32_t fields[2]; // [NFIELDDWORD]
		int16_t  nInCTB;
		uint32_t tablColumn;
	} FonClusterBase;

	/////////
	// ПАРАМЕТРЫ КЛАСТЕРИЗАЦИИ ///
	// кластеризовать и рукописные символы
#define FONCLU_ClusterHand  0x4000
	// в одной строке может быть много шрифтов
#define FONCLU_MultiFontRow 0x2000
	// сохранять как файл .clu
#define FONCLU_SaveAsClu    0x1000
	// сохранять все кластеры - не искать шрифты
#define FONCLU_NotFonts     0x800
	// сохранять даже слабые кластеры
#define FONCLU_SaveWeak     0x400
	// добавлять к уже имеющимся кластерам
#define FONCLU_AddClu       0x200
    // входной файл - файл растров (tiger format)
#define FONCLU_InputR       0x100
	// маска для порога
#define FONCLU_Threshold    0xFF
#endif
