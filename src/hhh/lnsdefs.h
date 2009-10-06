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

#ifndef __LNSDEFS_H
#define __LNSDEFS_H

   #ifndef __GLOBUS_H
      #include "globus.h"
   #endif

   #include "linedefs.h"

typedef struct tagLnsFrag
{
   int32_t  flags;  // misc info
#define LF_LINE   0x0001   // fragment decided in lns.dll to be line

#define LF_STICK_LEFT   0x0010 // there is stick at left side - may be letter
#define LF_STICK_RIGHT  0x0020 // there is stick at right side - may be letter
#define LF_STICK_TOP    0x0040 // there is stick at top side - may be letter
#define LF_STICK_BOTTOM 0x0080 // there is stick at top side - may be letter

#define LF_TEXT     0x0002 // fragment seems to be part of text (can be combined with LF_LINE)


   int32_t  mass;   // count of filtered black pixels
   Rect16 rc;     // real image rect
   Point16 A,B;   // start and end - if ~LF_LINE - may be not linear, so...
   double Xc     ;// center of mass in real coords
   double Yc     ;
   double Phi    ;// angle of own coords
   double aveW   ;// average width
   double mainMxx;// squared sum in own axes
   double mainMyy;// squared sum in own axes

//   double relMxx ;// squared sum by X axes
//   double relMxy ;// squared sum by Y axes
//   double relMyy ;// mixed sum

   int16_t frag_handle;   // internal usage - link to own data; 0xffff - undefined
   char  reserved[22];
} LnsFrag;

typedef struct tagAdjacentLst
{
	int16_t          ltiNext;       // list of adjacent lines (left to right,
	int16_t          ltiPrev;       // top to bottom), -1 == None
	int16_t          ltiFirst;
	int16_t          ltiLast;
}  AdjacentLst;

typedef struct tagLnsCorner
{
   int16_t          ltiStart;      // LTI index of perpendicular lines
   int16_t          ltiEnd;        // joined with proper end of this line
}  LnsCorner;


typedef struct tagLineInfo
{
   //========= Геометрическое положение =============================
   Point16       A;             // start
   Point16       B;             // end
   Point16       Ar;            // start at rotated coords
   Point16       Br;            // end at rotated coords
   Point16       Anew;          // start for sweep: if set LI_NOTWHOLE (real cords)
   Point16       Bnew;          // end for sweep: if set LI_NOTWHOLE (real cords)

   //========= Специальные характеристики ==========================
	uint32_t         Flags;         // common use info
//все флаги вынесены в LineDefs.h
/*
      #define LI_NOISE      0x00000001   // is a noise line
      #define LI_ATTACHED   0x00000002   // Line  confidently
                                         // attached to form element
      #define LI_SMARTSWEEP 0x00000004   // must smartly dissolve it from image
      #define LI_CRUDESWEEP 0x00000008   //
      #define LI_SWEEP (LI_SMARTSWEEP | LI_CRUDESWEEP)
      #define LI_DOT        0x00000010   //
      #define LI_COVERED    0x00000020   // always with LI_NOISE
*/
	/*****  Выставляются верификатором линий.  *****/
/*		// Либо только один из следующих, либо ни одного. Если ни одного, то
		// либо линия сомнительна, либо не иссдедовалась.
	#define LI_IsTrue       0x00000040
			// правильно выделенная линия
	#define LI_IsFalse      0x00000080
			// не линия
	#define LI_NOTWHOLE     0x00000100
			// Краевые захваты. Реальная линия короче - от Anew до Bnew.
	#define LI_COMPLEX      0x00000200
			// Составная линия : несколько реальных линий, иногда еще и одна
			// или несколько компонент связности. Информация о входящих в ее
			// состав реальных линиях и компонентах отписывается в совершенно
			// независимую структуру.
	#define LI_Pointed      0x00001000
	#define LI_Doubt        0x00002000
	#define LI_Used         0x00004000  // Использована как подчеркивание
 #define LI_FRMT_Used    0x00008000  // Formatter used
*/
	/*****  Выставляются выделителем таблиц.  *****/
/*
	#define LI_IsAtTable    0x00000400
	#define LI_IsNotAtTable 0x00000800
*/
	/**********************************************/
   int16_t          SegCnt;        // число сегментов в составной линии, 0-неинициализировано
   uchar          Quality;       // 0-255; 255 == good line
   uchar          Thickness;     // средняя толщина линии/штриха

   //========= Групповые характеристики ============================
   AdjacentLst    Adj;           // потенциальное расширение на уровне
   LnsCorner      Corner;        // стыковка углов с перпендикулярными
   Rect32        XXContext;     // согласование с параллельными линиями
   Rect32        XYContext;     // согласование с перпендикулярными линиями

   //========= Привязка к выделителю (для последующего снятия,...) =====
   int32_t          ExtrDllHnd;    // внутренний номер линии от выделителя
   uint16_t         Extractor;     // библиотека, выделившая линию
      #define LI_UNKNOWN   0
      #define LI_LNSDLL    1
      #define LI_DOTDLL    2
      #define LI_UNION     3      // union of some set of dot & some set of solid lines

   //========= Определения, специфичные для пунктиров ===============
   int16_t          DotAveLen;     // Средняя длина штриха
   int16_t          DotAveGap;     // Средний интервал м/штрихами

   //================================================================
   int16_t          IndCover;      // valid only for LI_COVERED-lines
   uint32_t         pFrmLineAtom;  // link to form description
   uchar          __buf[32];      // Зарезервировано, заполнено 0
   int32_t          TmpUsage;      // поле для временного использования

}  LineInfo;


#define LINE_HAS_EXT( li )                         \
   ( (li).Adj.ltiFirst != (li).Adj.ltiLast) &&     \
   ( (li).Adj.ltiFirst != -1              ) &&     \
   ( (li).Adj.ltiLast  != -1              )

#define LINE_HOR_LEN( li )  ( (li).B.x - (li).A.x + 1 )
#define LINE_VER_LEN( li )  ( (li).B.y - (li).A.y + 1 )
#define LINE_LEN( li )     ( maxi( LINE_HOR_LEN(li), LINE_VER_LEN(li) ) )

//#define LI_START_JOINED    0x0001   // left or top
//#define LI_END_JOINED      0x0002   // right bottom


typedef struct tagLnsInfoArray
{
	LineInfo*      Lns;            // first line pointer
   int32_t          Cnt;             // total lines count
//   int32_t          SumLen;         // sum lines len (excluding noise)
//removed   int32_t          NoiseCnt;       // count of noise lines
//   int32_t          ExtCnt;         // count of extended lines
   char reserved[12];
}  LnsInfoArray;

typedef struct tagLCSetup
{                                    // for 300 dpi:
   int32_t       NeglectedDelta;       // 5
   int32_t       MaxCornerGap;         // 20
   int32_t       MinExtLineFilling;    // 50 (in PERCENTS)
   int32_t       MinHLen;              //
   int32_t       MinVLen;              //
   // for all fields 0 - use default;
}  LCSetup;

#define LTI_DEF_VLEN       60         // default min length of line to be noise
#define LTI_DEF_HLEN       100

typedef struct tagLinesTotalInfo
{
   LnsInfoArray      Hor;
   LnsInfoArray      Ver;
   int32_t             Skew1024;
   Point16          ImgSize;
   Point16          ImgResolution;
   LCSetup           LCS;
   uchar             __buf[32];
}  LinesTotalInfo;

#ifdef __cplusplus
inline void ltiGetNotNoise( LinesTotalInfo* plti,  // get
                            int& hcnt, int& vcnt   // put
                          )
{
   int i;
   hcnt = plti->Hor.Cnt;
   for (i=0; i < plti->Hor.Cnt; i++)
   {  if (plti->Hor.Lns[i].Flags & LI_NOISE)
         hcnt--;
   }

   vcnt = plti->Ver.Cnt;
   for ( i=0; i < plti->Ver.Cnt; i++)
   {  if (plti->Ver.Lns[i].Flags & LI_NOISE)
         vcnt--;
   }
}
#endif
#endif
