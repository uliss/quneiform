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
BOOL StrGetRaster(Handle,Word8*,Rect16,CCOM_comp*,RecRaster*,BOOL,BOOL,int);
void StrAr2(Word8*,int,int);
BOOL GetMasP(Handle,Word8*,Rect16,Word8**);
void Invert(RecRaster*);
int MyGetMaxFragment(int);
void StrDrawRect(Handle,Word32,Word32,Rect16);
void Invert(Rect16*,Word8*);
void CopyP(Rect16*,Word8*,Word8*);
void ToHorizont(Rect16*,Word8*,Word8*);
BOOL IfNewStr(Word8*,int,int,int,int,int&,int);
void StrDrawRect(Handle,Word32,Word32,Rect16*,int,BOOL,Rect16);
void SortLetter(CCOM_comp** ppComp,int,BOOL);
void StrMoveMas(Rect16*,int&,int);
void StrMoveMas(int*,int&,int);
void StrMoveMas(CCOM_comp**,int&,int);
void StrMoveMasR(Rect16*,int&,int);
int IfStr(Rect16*,int,BOOL);
BOOL GetMasP(Handle,Word8*,Rect16,Word8**);
Int32 GetStrCCOM(Handle,Word8*,Rect16,BOOL,BOOL);
BOOL InitStrMas(Rect16**,int);
BOOL InitStrMas(CCOM_comp***,int);
void InitRc(Rect16*,int,CCOM_comp *);
//void DelStrMas(Rect16*);
void DelStrMas(Rect16**);
void DelStrMas(CCOM_comp**);
BOOL AddLenStrMas(Rect16**,int&,int);
BOOL AddLenStrMas(CCOM_comp***,int&,int);
BOOL MyRotateImage(Word8*,Word8*,int,Rect16*,BOOL,int,MATRIX*,MATRIX*);
void InitRotateImageName(Word8*,Word8*);
void InitRotateImageName(Word8*,int);
void GetRasterRect(Word8*,Rect16,Rect16*,CSTR_attr*,BOOL);
int GetStrScale(Rect16*,int,BOOL);
void CleanStr(Rect16*,int&,int,int,int,int,int,BOOL);
BOOL InitRotateMas(Rect16,Int16**,Int16**,Word8**);
BOOL InitRotateMas(int**,int);
void DeleteRotateMas(Int16*,Int16*,Word8*,int*);
Bool32 Filter(Int32,Int32,Int32,Int32);
int GetStrW(Word8*,int,int,int,int,int&,int&);
int GetMediumSize(CCOM_handle,Rect16,int&,BOOL);
int GetMaxSize(CCOM_handle hCCOM,Rect16 Rc,int& num,BOOL vert);
//BOOL IfBadCut(Handle,Rect16*,Rect16,int,Word8*,CHstr*);
CCOM_handle CreateContainer(Handle hCPage,CCOM_handle hCCOM_old,Rect16 Rc);
int GetMinCol(CCOM_comp** pC,int nN,BOOL vert);
BOOL AddToPrepList(CPrepHstr* temp);
BOOL AddToCpage(Handle hCPAGE,CPrepHstr* temp);
BOOL InitPrepList(Handle hCPAGE);
CPrepHstr* WasAlreadyPrep(CHstr* my_str);
void  GetIdealRect(CCOM_handle HCCOM,Rect16 Rc,Rect16* Rect);
void GetStrBounds(CCOM_handle hCCOM,Rect16* pRc,int nRc,int num,int* ptop,int* pbottom,BOOL vert,Rect16 Rc);
BOOL MainVertDiagnostic(Handle hCPage,CCOM_handle hCCOM,CPrepHstr* temp,Rect16* pRc,int nRc,int param);
BOOL NotIn(CCOM_handle,Rect16 Rc,Rect16* pRc,int nRc,BOOL vert,int);
BOOL NotInAll(CCOM_handle,Rect16 Rc,Rect16* pRc,int& nRc,BOOL vert);
BOOL IfOneRowComp(CCOM_handle hCCOM,Rect16 Rc,BOOL vert);
