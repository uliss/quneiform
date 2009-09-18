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
/*  Файл       :  'RSTimer.cpp'                                               */
/*  Содержание :  Калбэки puma.dll для привязки времени и отладчика           */
/*  Назначение :                                                              */
/*----------------------------------------------------------------------------*/
#include "rstuff.h"
#include "rsglobaldata.h"
#include "rsdefines.h"
#include "rsfunc.h"
//////////////////////////////////////////////////////////////////////////////////////
//
Bool32 SetCBProgressPoints(void * pData)
{
	PRSCBProgressPoints pPoints = (PRSCBProgressPoints)pData;

#define SET_CB_POINTS(a,b)  ProgressPoints.p##b = a->p##b
	SET_CB_POINTS(pPoints, ProgressStart);
	SET_CB_POINTS(pPoints, ProgressFinish);
	SET_CB_POINTS(pPoints, RestorePRGTIME);
	SET_CB_POINTS(pPoints, SetUpdate);
	SET_CB_POINTS(pPoints, ProgressStep);
	SET_CB_POINTS(pPoints, ProgressStepLines);
	SET_CB_POINTS(pPoints, ProgressStepTables);
	SET_CB_POINTS(pPoints, InitPRGTIME);
	SET_CB_POINTS(pPoints, StorePRGTIME);
	SET_CB_POINTS(pPoints, DonePRGTIME);
	SET_CB_POINTS(pPoints, DPumaSkipComponent);
	SET_CB_POINTS(pPoints, DPumaSkipTurn);
	SET_CB_POINTS(pPoints, rexcProgressStep);
	SET_CB_POINTS(pPoints, GetModulePath);
	//SET_CB_POINTS(pPoints,                   );
#undef SET_CB_POINTS
	return true;
}
#define DEF_CB_FUNC(a,b,c,d,e)       a b c \
{\
	DEC_CB_TYPE(b) pfFunc; \
	a ret = e ; \
	if ( ProgressPoints.p##b ) \
	{ \
		pfFunc = ( DEC_CB_TYPE(b) )ProgressPoints.p##b; \
		return pfFunc d ; \
	} \
	return ret; \
}
#define DEF_CB_VOID_FUNC(b,c,d)       void b c \
{ \
	DEC_CB_TYPE(b) pfFunc; \
	if ( ProgressPoints.p##b ) \
	{ \
		pfFunc = ( DEC_CB_TYPE(b) )ProgressPoints.p##b; \
		pfFunc d; \
	}\
}

DEF_CB_VOID_FUNC(ProgressStart,   (void),() )
DEF_CB_VOID_FUNC(ProgressFinish,  (void),() )
DEF_CB_VOID_FUNC(RestorePRGTIME,  (PRGTIME	prev),(prev) )
DEF_CB_VOID_FUNC(SetUpdate,       (uint32_t flgAdd,uint32_t flgRemove),(flgAdd, flgRemove) )
DEF_CB_FUNC(Bool32,  ProgressStep,       (uint32_t step, uint32_t percent), (step, percent), FALSE  )
DEF_CB_FUNC(Bool32,  ProgressStepLines,  (uint32_t step, uint32_t percent), (step, percent), FALSE  )
DEF_CB_FUNC(Bool32,  ProgressStepTables, (uint32_t step, uint32_t percent), (step, percent), FALSE  )
DEF_CB_FUNC(Bool32,  InitPRGTIME,        (void), (), FALSE  )
DEF_CB_FUNC(PRGTIME, StorePRGTIME,       (uint32_t beg, uint32_t end), (beg, end), {0}  )
DEF_CB_FUNC(Bool32,  DonePRGTIME,        (void), (), FALSE  )
DEF_CB_FUNC(Bool32,  DPumaSkipComponent, (void), (), FALSE  )
DEF_CB_FUNC(Bool32,  DPumaSkipTurn,      (void), (), FALSE  )
DEF_CB_FUNC(Bool32,  rexcProgressStep,   (uint32_t step), (step), FALSE  )
DEF_CB_FUNC(char *,  GetModulePath,      (void), (), NULL  )

#undef DEF_CB_FUNC
#undef DEF_CB_VOID_FUNC
//////////////////////////////////////////////////////////////////////////////////////
//end of file
