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

#include <stdio.h>
/*#include <windows.h>*/


#include "ccom.h"
#include "ctiimage.h"
#include "exc.h"
#include "recdefs.h"
#include "negdefs.h"
#include "rstr.h"
#include "dpuma.h"
#include "cpage.h"
#include "cstr.h"
#include "rselstr.h"


/*----------------------------------------------------------------------------*/

//Настройки


extern int min_let_h;
extern int min_let_w;
//extern int max_let_h;
//extern int max_let_w;
extern int NegRecTYPEE;
extern double inf_prob;
/*----------------------------------------------------------------------------*/

//Параметры

extern int	DPIX;
extern int	DPIY;
extern int Height;
extern int Width;
extern double inf_prob;
extern Word8 ImageName[CPAGE_MAXNAME];
extern Handle NegRecD;
extern Handle NegResD;
extern Handle NegRecDC;
extern Handle MainWindowD;
extern Handle RecOneLetter;
extern Handle PrintResConsole;
extern Handle hShowNegComps;

/*----------------------------------------------------------------------------*/




//My function's diclarations

BOOL InitNegMas(Rect16**,int);
BOOL InitNegMas(RecVersions**,int);
BOOL InitNegMas(UniVersions**,int);
BOOL InitNegMas(CCOM_comp***,int);
void DelNegMas(Rect16**);
void DelNegMas(RecVersions*);
void DelNegMas(UniVersions*);
void DelNegMas(CCOM_comp**);
BOOL AddLenNegMas(Rect16**,int&,int);
BOOL AddLenNegMas(CCOM_comp***,int&,int);
BOOL GetMasP(Handle,Rect16,Word8**);
BOOL GetMasP(Rect16*,Word8**);
BOOL NegGetRaster(Handle,Rect16,RecRaster*,BOOL);
void InitRc(Rect16*,int,CCOM_comp *);
void PrintFrameComp(Handle,Rect16*,int);
Handle GetNegCCOM(Handle,Rect16*,int);
void NegRecog(Handle,NegList**,int&,int);
void NegInvert(RecRaster*,Rect16);
void SortLetter(Rect16*,int,BOOL);
void ToHoriz(Rect16*,int);
void NegPrintConsol(Word8*,int);
void NegPrintConsol(double);
void NegPrintConsol(int,int,int,int);
void NegMoveResult(Word8*,int&,int);
void NegMoveMas(Rect16*,int&,int);
void PMoveMas(double*,int,int);
void NegMoveMas(Rotating*,int,int);
void NegMoveMasR(Rect16*,int&,int);
void NegPutLetter(Word8*,int&,RecAlt,BOOL);
int GetMediumW(Rect16*,int);
int GetMediumH(Rect16*,int);
BOOL IfExistA(RecVersions);
BOOL IfGl(Word8);
BOOL IfExistDef(RecVersions);
BOOL IfExistI(RecVersions);
BOOL IfBadResult(Word8*,int);
BOOL IfNegIn(NegList*,int,int,int,int,int);
void NegDrawRect(Handle,uint32_t,uint32_t,Rect16);
void NegAr2(Word8*,int,int);
double NegStrControl(Rect16*,int,BOOL,Rect16*);
double NegRecControl(int);
int CountLetter(Rect16*,int,BOOL);
int NegGetCountOfStr(Rect16*,int);
void NegPrintInFile(FILE*,NegList*,int);
int NegCountNumbers(int);
void NegSetAlf(char*);
void DeleteRotateMas(Word8**,int16_t**,int16_t**,Word8**,int**);
void NegMoveMas(Handle* hCCOM,int n,int num);
BOOL UnifCont(CCOM_handle to,CCOM_handle from);
void TurnRaster(RecRaster*);
void TurnOverNeg(RecRaster, Word8*);


/*----------------------------------------------------------------------------*/
