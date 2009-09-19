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

/**********  Заголовок  *******************************************************/
/*  Автор,                                                                    */
/*  комментарии                                                               */
/*  и далнейшая                                                               */
/*  правка     :  Алексей Коноплев                                            */
/*  Редакция   :  08.06.00                                                    */
/*  Файл       :  'dll.cpp'                                                   */
/*  Содержание :  Интерфейс библиотеки                                        */
/*  Назначение :                                                              */
/*----------------------------------------------------------------------------*/
#ifndef _RS_FUNC_H_
#define _RS_FUNC_H_
/////////////////////////////////////////////////////////////////////////////////////////////////////
//
#include "rsdefines.h"
#include "ctiimage.h"
#include "ccomdefs.h"
#include "lnsdefs.h"
#include "linedefs.h"
//#include "RStuff.H"
//////////////////////////
//общего назначения
void SetReturnCode_rstuff(uint16_t);
void SetReturnCode_rstuff(uint32_t);
void DebugInit (void );
void DebugDPumaDrawRect(Handle, Point16 *, uint32_t, int32_t, uint32_t);
void DebugDPumaShowComponents(PRSPreProcessImage, Handle, uint32_t, int32_t, uint32_t);

Bool32    Binarise( void );
Bool32    Normalise( PRSPreProcessImage );
Bool32    VerifyN( PRSPreProcessImage );
Bool32    DeleteLines(Handle hCPage, void* phCLINE, const char* ImageDelLines);
Bool32    DeleteDotLines(void* phCLINE, const char* ImageDelLines);

Bool32    KillLinesN( PRSPreProcessImage );
Bool32    Layout( PRSPreProcessImage );
/////////////////////////////////////////////////////////////////////////////////////////////////
// обработка сырья
Bool32    PreProcessImage      ( PRSPreProcessImage );
Bool32    CreateContainerBigComp( PRSPreProcessImage );
Bool32    SearchNewLines       ( PRSPreProcessImage Image );
Bool32    SearchLines          ( PRSPreProcessImage );
Bool32    CalcIncline          ( PRSPreProcessImage );
Bool32    VerifyLines          ( PRSPreProcessImage );
Bool32    OrtoMove             ( PRSPreProcessImage );
Bool32    KillLines            ( PRSPreProcessImage );
Bool32    SearchTables         ( PRSPreProcessImage );
/////////////////////////////////////////////////////////////////////////////////////////////////////
// разметка и линии
Bool32    LineKiller           ( PRSPreProcessImage );
Bool32    SearchAndKill        ( PRSPreProcessImage, LinesTotalInfo* );
Bool32    ComponentFilter      ( PRSPreProcessImage, LineInfo* );
Bool32    IsRectIntersect      ( Rect16*, Rect16* );
Bool32    TuneFilter           ( LineInfo*, Rect16*, uint32_t, uint32_t);
Bool32    ChekComponentAndLine ( LineInfo*, Rect16*, uint32_t);
Bool32    CheckSquare          ( LineInfo*, Rect16*, uint32_t, uint32_t);
/////////////////////////////////////////////////////////////////////////////////////////////////////
//
Bool32    ExtractComponents    ( Bool32, Handle *, puchar, PRSPreProcessImage );
Bool32    RemoveLines          ( PRSPreProcessImage, puchar *);
Bool32    MyGetZher            ( void **, int32_t *, int32_t, Handle );
Bool32    remove_overlayed     ( CCOM_comp *,CCOM_handle );
Bool32    comp_over            ( CCOM_comp *,CCOM_comp * );
//Bool32    AboutLines           ( PRSPreProcessImage, Bool32 *, int32_t * );
Bool32    AboutLines           ( PRSPreProcessImage, Bool32 *, int32_t * );
void GiveMainBuff (void **vvBuff, int *Size);
void GiveWorkBuff (char **ccBuff, int *Size);
void SetMainBuff(void *vBuff, int Size);
void SetWorkBuff(void *vBuff, int Size);
void ReSetMem(void);
/////////////////////////////////////////////////////////////////////////////////////////////////////
// обработка коротких вертикальных линий
//Bool32    ShortVerticalLinesProcess    ( PRSPreProcessImage, uint32_t );
//Bool32    ReadSVLFromPageContainer     ( PRSPreProcessImage, void * );
/////////////////////////////////////////////////////////////////////////////////////////////////////
// калбэки
Bool32    SetCBProgressPoints(void *);
//
#define DEC_CB_TYPE(a)  PF##a
#define DEC_CB_FUN(a,b,c) typedef a (*DEC_CB_TYPE(b))c; a b c;
DEC_CB_FUN(void,    ProgressStart,      (void))
DEC_CB_FUN(Bool32,  ProgressStep,       (uint32_t,  uint32_t))
DEC_CB_FUN(Bool32,  ProgressStepLines,  (uint32_t,  uint32_t))
DEC_CB_FUN(Bool32,  ProgressStepTables, (uint32_t,  uint32_t))
DEC_CB_FUN(void,    ProgressFinish,     (void))
DEC_CB_FUN(Bool32,  InitPRGTIME,        (void))
DEC_CB_FUN(PRGTIME, StorePRGTIME,       (uint32_t, uint32_t))
DEC_CB_FUN(void,    RestorePRGTIME,     (PRGTIME))
DEC_CB_FUN(Bool32,  DonePRGTIME,        (void))
DEC_CB_FUN(Bool32,  DPumaSkipComponent, (void))
DEC_CB_FUN(Bool32,  DPumaSkipTurn,      (void))
DEC_CB_FUN(Bool32,  rexcProgressStep,   (uint32_t))
DEC_CB_FUN(void,    SetUpdate,          (uint32_t, uint32_t))
DEC_CB_FUN(char *,  GetModulePath,      (void))
#undef DEC_CB_FUN
#endif //_RS_FUNC_H_
////////////////////////////////////////////////////////////////////////////////
//end of file
