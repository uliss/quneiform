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
#include <math.h>
#include<time.h>

#include "lhstr.h"
#include "ccom.h"
#include "ctiimage.h"
#include "recdefs.h"
#include "rstr.h"
#include "dpuma.h"
#include "cpage.h"
#include "cstr.h"
#include "exc.h"
#include "criimage.h"
#include "evn.h"

/*----------------------------------------------------------------------------*/

//Настройки
extern int inf_str_h;
extern int inf_str_w;
extern double cut_white;
extern double cut_str;

/*----------------------------------------------------------------------------*/
//Параметры


extern CLhstr* list;
extern CLPrepHstr* prelist;
extern Handle MainWindowD;
extern Handle StrD;
extern Handle Root;

#define TYPE_RSELSTR_TEMP_PHSTR CPAGE_GetInternalType("TYPE_RSELSTR_TEMP_PHSTR")
#define TYPE_RNEG_TEMP_PHSTR CPAGE_GetInternalType("TYPE_RNEG_TEMP_PHSTR")

/*-------------------------------------------------------------------------------------*/

void CleanCont(void);
Bool StrGetRaster(Handle, uchar*, Rect16, CCOM_comp*, RecRaster*, Bool, Bool,
		int);
void StrAr2(uchar*, int, int);
Bool GetMasP(Handle, uchar*, Rect16, uchar**);
void Invert(RecRaster*);
int MyGetMaxFragment(int);
void StrDrawRect(Handle, uint32_t, uint32_t, Rect16);
void Invert(Rect16*, uchar*);
void CopyP(Rect16*, uchar*, uchar*);
void ToHorizont(Rect16*, uchar*, uchar*);
Bool IfNewStr(uchar*, int, int, int, int, int&, int);
void StrDrawRect(Handle, uint32_t, uint32_t, Rect16*, int, Bool, Rect16);
void SortLetter(CCOM_comp** ppComp, int, Bool);
void StrMoveMas(Rect16*, int&, int);
void StrMoveMas(int*, int&, int);
void StrMoveMas(CCOM_comp**, int&, int);
void StrMoveMasR(Rect16*, int&, int);
int IfStr(Rect16*, int, Bool);
Bool GetMasP(Handle, uchar*, Rect16, uchar**);
Handle GetStrCCOM(Handle, uchar*, Rect16, Bool, Bool);
Bool InitStrMas(Rect16**, int);
Bool InitStrMas(CCOM_comp***, int);
void InitRc(Rect16*, int, CCOM_comp *);
//void DelStrMas(Rect16*);
void DelStrMas(Rect16**);
void DelStrMas(CCOM_comp**);
Bool AddLenStrMas(Rect16**, int&, int);
Bool AddLenStrMas(CCOM_comp***, int&, int);
Bool MyRotateImage(uchar*, uchar*, int, Rect16*, Bool, int, MATRIX*, MATRIX*);
void InitRotateImageName(uchar*, uchar*);
void InitRotateImageName(uchar*, int);
void GetRasterRect(uchar*, Rect16, Rect16*, CSTR_attr*, Bool);
int GetStrScale(Rect16*, int, Bool);
void CleanStr(Rect16*, int&, int, int, int, int, int, Bool);
Bool InitRotateMas(Rect16, int16_t**, int16_t**, uchar**);
Bool InitRotateMas(int**, int);
void DeleteRotateMas(int16_t*, int16_t*, uchar*, int*);
Bool32 Filter(int32_t, int32_t, int32_t, int32_t);
int GetStrW(uchar*, int, int, int, int, int&, int&);
int GetMediumSize(CCOM_handle, Rect16, int&, Bool);
int GetMaxSize(CCOM_handle hCCOM, Rect16 Rc, int& num, Bool vert);
//Bool IfBadCut(Handle,Rect16*,Rect16,int,uchar*,CHstr*);
CCOM_handle CreateContainer(Handle hCPage, CCOM_handle hCCOM_old, Rect16 Rc);
int GetMinCol(CCOM_comp** pC, int nN, Bool vert);
Bool AddToPrepList(CPrepHstr* temp);
Bool AddToCpage(Handle hCPAGE, CPrepHstr* temp);
Bool InitPrepList(Handle hCPAGE);
CPrepHstr* WasAlreadyPrep(CHstr* my_str);
void GetIdealRect(CCOM_handle HCCOM, Rect16 Rc, Rect16* Rect);
void GetStrBounds(CCOM_handle hCCOM, Rect16* pRc, int nRc, int num, int* ptop,
		int* pbottom, Bool vert, Rect16 Rc);
Bool MainVertDiagnostic(Handle hCPage, CCOM_handle hCCOM, CPrepHstr* temp,
		Rect16* pRc, int nRc, int param);
Bool NotIn(CCOM_handle, Rect16 Rc, Rect16* pRc, int nRc, Bool vert, int);
Bool NotInAll(CCOM_handle, Rect16 Rc, Rect16* pRc, int& nRc, Bool vert);
Bool IfOneRowComp(CCOM_handle hCCOM, Rect16 Rc, Bool vert);
