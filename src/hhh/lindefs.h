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

#ifndef __LINDEFS_H
#define __LINDEFS_H

#include "wind32.h"

#define FILE_PCX         900
#define ONLY_STRETCH     910
#define ONLY_LINE        911
#define ONLY_BOX_LINE    912
#define NO_RASTER        920
#define YES_RASTER       921
#define YES_OUT_LIN      930
#define NO_OUT_LIN       931
#define YES_OUT_BIN      940
#define NO_OUT_BIN       941
#define ID_USER          999

typedef struct lines_par{
  int vmaxwid;
	int vminlen;
	int vlsarea;
	int vlconnill;
	int vlconnils;
  int vlconniss;
	int vstartlenf;
	int vstartlens;
	int gmaxwid;
  int gminlen;
	int glsarea;
	int glconnill;
	int glconnils;
  int glconniss;
	int gstartlenf;
	int gstartlens;
	int lcont;
  int minboxw;
	int bsarf;
	int psarf;
	int psarb;
  int gangl;
	int langl;
	int linnm;
}	PAR_LINE;

#define STRETCH struct h_STRETCH //Ћ¤Ё­ ®ваҐ§®Є
#define LINE struct h_LINE //ЋЇЁб ­ЁҐ ®¤­®© «Ё­ЁЁ
#define BOX struct h_BOX
#define KNOTL struct h_KNOTL //“§«л, ў®§­ЁЄ ойЁҐ ЇаЁ ЇҐаҐбҐзҐ­ЁЁ «Ё­Ё©
#define LINES struct h_LINES //‚бп бвагЄвга  «Ё­Ё©, Ў®Єб®ў Ё г§«®ў.
STRETCH
{
	POINT Beg,End;       //­ з «® Ё Є®­Ґж ®ваҐ§Є  Ї®б«Ґ Ї®ў®а®в 
  POINT BegOld,EndOld; //­ з «® Ё Є®­Ґж ®ваҐ§Є  ¤® Ї®ў®а®в 
  float k;             //­ Є«®­ «Ё­ЁЁ y=kx+b
  float b;             //б¬ҐйҐ­ЁҐ
  short thickness; //баҐ¤­пп в®«йЁ­  ®ваҐ§Є 
};
LINE
{
	int NumStretch;
  STRETCH *Stretch; //[NumStretch] - зЁб«® ®ваҐ§Є®ў ®¤­®© «Ё­ЁЁ
};
BOX
{
	short Top;   //€­¤ҐЄб ўҐае­Ґ© «Ё­ЁЁ Ў®Єб 
  short Left;  //€­¤ҐЄб «Ґў®© «Ё­ЁЁ Ў®Єб 
  short Bottom;//€­¤ҐЄб ­Ё¦­Ґ© «Ё­ЁЁ Ў®Єб 
  short Right; //€­¤ҐЄб Їа ў®© «Ё­ЁЁ Ў®Єб 
  short Diag13, Diag24;
};
KNOTL
{
  short InVertLine;   //­®¬Ґа ўҐавЁЄ «м­®© «Ё­ЁЁ
  short InVertStretch;//Ё ­®¬Ґа ®ваҐ§Є  ў Ї®ап¤ЄҐ ®Ўе®¤  ҐҐ ®ваҐ§Є®ў
  short InHorLine;    //­®¬Ґа Ј®аЁ§®­в «м­®© «Ё­ЁЁ
  short InHorStretch; //Ё ­®¬Ґа ®ваҐ§Є  ў Ї®ап¤ЄҐ ®Ўе®¤  ҐҐ ®ваҐ§Є®ў
  POINT PointIntersection;//в®зЄ  ЇҐаҐбҐзҐ­Ёп «Ё­Ё©
  short TypeIntersection;//вЁЇ ЇҐаҐбҐзҐ­Ёп «Ё­Ё©
};
LINES
{
	short NumAll;  //®ЎйҐҐ зЁб«® «Ё­Ё©
  short NumVert; //зЁб«® ўҐавЁЄ «м­ле «Ё­Ё©
  short NumHor;  //зЁб«® Ј®аЁ§®­в «м­ле «Ё­Ё©
  short NumOther;//зЁб«® ®бв «м­ле «Ё­Ё©
  LINE *Line;        //[NumAll] - ¬ ббЁў «Ё­Ё©
  short NumBox;  //зЁб«® Їап¬®гЈ®«м­ЁЄ®ў
  BOX *Box;          //[NumBox] - ¬ ббЁў Їап¬®гЈ®«м­ЁЄ®ў
  short NumKnot;//зЁб«® г§«®ў
  KNOTL *Knotl;      //[NumKnotl] - ¬ ббЁў г§«®ў
};

typedef struct h_BMPL
{
	int widthBit;
  int height;
  int widthByte;
  int NumBuf;
  uchar **bmp; //bmp[NumBuf][64000]
} BMPL;
//typedef int (*FUN_POMP)(void);
//typedef void (*FUN_MESS)(uint16_t wPar,DWORD lPar);

#include "undef32.h"

#endif
