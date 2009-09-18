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
#include <time.h>

#include "dpuma.h"
#include "rselstr.h"
#include "cpage.h"
#include "ctiimage.h"
#include "negdefs.h"
#include "polyblock.h"
#include "criimage.h"
#include "ctdib.h"


/*----------------------------------------------------------------------------*/

//Настройки


/*----------------------------------------------------------------------------*/
//Параметры


extern Handle NegRoot;
extern Handle NegSearch;
extern Handle NegRec;
extern Handle NegSearchSize;
extern Handle NegSize;
extern Handle NegNorm;
extern Handle NegCut;
extern Handle NegCutStr;
extern Handle NegColor;
extern Handle NegSearchSizeD;
extern Handle NegSizeD;
extern Handle NegNormD;
extern Handle NegCutD;
extern Handle NegCutStrD;
extern Handle NegColorD;
extern Handle NegSearchSizeDC;
extern Handle NegSizeDC;
extern Handle NegNormDC;
extern Handle NegCutDC;
extern Handle NegCutStrDC;
extern Handle NegColorDC;
extern Handle MainWindowD;
extern Handle NegPrintFILE;
extern Handle TestCHSTR;
extern Handle PutToCHSTR;
extern Handle PutToCPage;
extern Handle hTime;
extern int TOP;
extern int BOTTOM;
extern int LEFT;
extern int RIGHT;
extern int VERTICALE;
extern int NEGATIVE;
extern Handle SetSkew;
extern int Skew;
extern double inf_prob;
extern Handle NegRecD;
extern Handle NegResD;
extern Handle RecOneLetter;
extern Handle PrintResConsole;


/*----------------------------------------------------------------------------*/




//My function's diclarations


void DelNegMas(Rect16**);
void NormNeg(Handle,NegList*,NegList**,int&);
void CutNeg(Handle,NegList**,int&,int);
void CutNegStr(Handle,NegList**,int&,int);
Bool SearchNegBySize(CCOM_handle,Handle,NegList**,int&);
void SearchNegByBlack(Handle,NegList**,int&);
void NegRecog(Handle,NegList**,int&,int);
void NegMoveMas(Rect16*,int&,int);
void NegDrawRect(Handle,uint32_t,uint32_t,Rect16);
void NegPrintInFile(FILE*,NegList*,int);
int NegGetCountOfStr(Rect16*,int);
Bool NegRotate(Handle,NegList*,int,int);
Bool GetMasP(Handle,Rect16,uchar**);
Bool GetMasP(Rect16*,uchar**);
void NegCopy(uchar*,uchar*,int,int);
Bool InitRotateMas(int,int,int16_t**,int16_t**,uchar**,int**);
void DeleteRotateMas(uchar**,int16_t**,int16_t**,uchar**,int**);
void DelIn(Handle,NegList**,int&);
Bool IsNegSize(int16_t h,int16_t w);
Bool IfNeedGlobalRotate(NegList* root,int& skew);
void UnRotateNeg(NegList* root,int& skew);
void PutTempToCPAGE(Handle hCPage,NegList* root);
Bool InitMyImage();
void ResetMyImage();
void CheckNega(NegList* root);void PostRotate(NegList* now,int skew);
/*----------------------------------------------------------------------------*/
