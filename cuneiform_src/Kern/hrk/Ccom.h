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

////////////////////////////////////////////////////////////
// CONTAINER OF THE COMPONENTS                            //
////////////////////////////////////////////////////////////
// O.Slavin                                               //
////////////////////////////////////////////////////////////
// Creation     : 08 Jul 1998                             //
////////////////////////////////////////////////////////////
// Modification : 21 Jul 1998                             //
////////////////////////////////////////////////////////////
#ifndef __CCOM_H
#define __CCOM_H

#include "globus.h"
#include "ccomdefs.h"
#include "recdefs.h"
#include "memfunc.h"

#ifdef __CCOM__
#define CCOM_FUNC  FUN_EXPO
#else
#define CCOM_FUNC  FUN_IMPO
#endif

#ifdef __cplusplus
extern "C" {
#endif

CCOM_FUNC(Bool32) CCOM_Init( uint16_t wHeightCode, Handle hStorage );
CCOM_FUNC(void) CCOM_Done(void);
CCOM_FUNC(uint32_t) CCOM_GetReturnCode(void);
CCOM_FUNC(char*) CCOM_GetReturnString(uint32_t dwError);
CCOM_FUNC(Bool32) CCOM_SetImportData(uint32_t dwType, void * pData);
CCOM_FUNC(Bool32) CCOM_GetExportData(uint32_t dwType, void * pData);

enum CCOMFunctionsExt {
	CCOM_FNNEW,
	CCOM_FNSTORE,
	CCOM_FNCOPY,
	CCOM_FNDELETE,
	CCOM_FNGETFIRST,
	CCOM_FNGETNEXT,
	CCOM_FNGETLINE,
	CCOM_FNGETRASTER,
	CCOM_FNGETCOLLECTION,
	CCOM_FNGETUSERBLOCK,
	CCOM_FNCREATECONT,
	CCOM_FNDELETECONT,
	CCOM_FNADDLPTORS,
	CCOM_FNMAKELP,
	CCOM_FNSETUSERBLOCK,
	CCOM_FNNEWUSERCODE,
	CCOM_FNVERSION,
	CCOM_FNBACKUP,
	CCOM_FNRESTORE,
	CCOM_FNDELALL,
	CCOM_FNLARGENEW,
	CCOM_FNLARGENEWLN,
	CCOM_FNLARGENEWINTERVAL,
	CCOM_FNLARGECLOSE,
	CCOM_FNSETKILLEDACCESS,
	CCOM_FNKILL,
	CCOM_FNGETEXTRASTER,
	CCOM_FNCLEARCONTAINER,
	CCOM_FNGETSCALERASTER,
	CCOM_COMPRESSCONTAINER,
	CCOM_SETLANGUAGE,
	CCOM_GETLANGUAGE,
	CCOM_GECONTAINERVOLUME,
	CCOM_YDECR,
	CCOM_YINCR,
	CCOM_FNREANIMATE,
	CCOM_FNADDCOMPTORS
};
enum CCOMFunctionsImp {
	CCOM_FNIMP_ALLOC,
	CCOM_FNIMP_REALLOC,
	CCOM_FNIMP_FREE,
	CCOM_FNIMP_OPEN,
	CCOM_FNIMP_CLOSE,
	CCOM_FNIMP_READ,
	CCOM_FNIMP_WRITE,
	CCOM_FNIMP_MAKELP
};
// 1    CCOM_FNNEW          создать новую компоненту
typedef CCOM_comp * (*FNCCOM_New)(CCOM_handle cont, int32_t upper,
		int32_t left, int32_t w, int32_t h);
CCOM_FUNC(CCOM_comp*) CCOM_New(CCOM_handle cont,int32_t upper, int32_t left, int32_t w, int32_t h);
// 2    CCOM_FNSTORE        записать в компоненту образ
typedef Bool32 (*FNCCOM_Store)(CCOM_comp * comp, int16_t numcomp,
		int32_t size_nlines, uchar *lines, int16_t nl, uchar free_beg,
		uchar free_e, RecVersions *vers, CCOM_USER_BLOCK *ub);
CCOM_FUNC(Bool32) CCOM_Store(CCOM_comp * comp,int16_t numcomp,
		int32_t size_nlines, uchar *lines, int16_t nl,
		uchar free_beg, uchar free_e,
		RecVersions *vers,
		CCOM_USER_BLOCK *ub);
// 3    CCOM_FNSTORE        записать в компоненту образ
typedef Bool32 (*FNCCOM_Copy)(CCOM_comp * to, CCOM_comp *from);
CCOM_FUNC(Bool32) CCOM_Copy(CCOM_comp * to,CCOM_comp *from);
// 4    CCOM_FNDELETE       удалить компоненту
typedef Bool32 (*FNCCOM_Delete)(CCOM_handle cont, CCOM_comp * comp);
CCOM_FUNC(Bool32) CCOM_Delete(CCOM_handle cont, CCOM_comp * comp);
// 5    CCOM_FNGETFIRST     дать первую компоненту из контейнера
typedef CCOM_comp * (*FNCCOM_GetFirst)(CCOM_handle ccom, Bool32(*filtrate)(
		int32_t upper, int32_t left, int32_t w, int32_t h));
CCOM_FUNC(CCOM_comp*) CCOM_GetFirst(CCOM_handle ccom,Bool32 (*filtrate)(int32_t upper, int32_t left, int32_t w, int32_t h) );
// 6    CCOM_FNGETNEXT      дать следующую компоненту
typedef CCOM_comp * (*FNCCOM_GetNext)(CCOM_comp * comp, Bool32(*filtrate)(
		int32_t upper, int32_t left, int32_t w, int32_t h));
CCOM_FUNC(CCOM_comp*) CCOM_GetNext(CCOM_comp * comp, Bool32 (*filtrate)(int32_t upper, int32_t left, int32_t w, int32_t h));
// 7    CCOM_FNGETLINE      получить интервальное представление
typedef Bool32 (*FNCCOM_GetLine)(CCOM_comp * comp, CCOM_linerep *lrep);
CCOM_FUNC(int16_t) CCOM_GetLine(CCOM_comp * comp,CCOM_linerep *lrep);
// 8    CCOM_FNGETRASTER    получить рабочий растр компоненты
typedef Bool32 (*FNCCOM_GetRaster)(CCOM_comp * comp, RecRaster *rec);
CCOM_FUNC(Bool32) CCOM_GetRaster(CCOM_comp * comp, RecRaster *rec);
// 9    CCOM_FNGETCOLLECTION  Получить коллекцию распознавания
typedef Bool32 (*FNCCOM_GetCollection)(CCOM_comp * comp, RecVersions *vers);
CCOM_FUNC(Bool32) CCOM_GetCollection(CCOM_comp * comp, RecVersions *vers);
// 10    CCOM_FNGETUSERBLOCK Получить пользовательский блок
typedef Bool32 (*FNCCOM_GetUserBlock)(CCOM_comp * comp, CCOM_USER_BLOCK *ub);
CCOM_FUNC(Bool32) CCOM_GetUserBlock(CCOM_comp * comp,CCOM_USER_BLOCK *ub);
// 11   CCOM_FNCREATECONT   Создать новый контейнер
typedef CCOM_handle (*FNCCOM_CreateContainer)(void);
CCOM_FUNC(CCOM_handle) CCOM_CreateContainer(void);
// 12   CCOM_FNDELETECONT   Уничтожить контейнер
typedef Bool32 (*FNCCOM_DeleteContainer)(CCOM_handle con);
CCOM_FUNC(Bool32) CCOM_DeleteContainer (CCOM_handle con);
// 13   CCOM_FNADDLPTORS    Добавить ЛП к растру
typedef Bool32 (*FNCCOM_AddLPToRaster)(CCOM_comp * comp, RecRaster *rec);
CCOM_FUNC(Bool32) CCOM_AddLPToRaster (CCOM_comp * comp, RecRaster *rec);
// 14   CCOM_FNMAKELP       Получить интервальное представление
typedef Bool32 (*FNCCOM_MakeLP)(RecRaster *rRaster, uchar *lp,
		int16_t *lp_size, int16_t *numcomp);
CCOM_FUNC(Bool32) CCOM_MakeLP ( RecRaster *rRaster , uchar *lp, int16_t *lp_size, int16_t *numcomp);
// 15   CCOM_FNSETUSERBLOCK Записать пользовательский блок
typedef Bool32 (*FNCCOM_SetUserBlock)(CCOM_comp * comp, CCOM_USER_BLOCK *ub);
CCOM_FUNC(Bool32) CCOM_SetUserBlock(CCOM_comp * comp, CCOM_USER_BLOCK *ub);
// 16   CCOM_FNNEWUSERCODE  Дать код свободного пользовательского кода
typedef int32_t (*FNCCOM_NewUserCode)(void);
CCOM_FUNC(int32_t) CCOM_NewUserCode (void);
// 17   CCOM_FNVERSION      Дать номер версии
// 18   CCOM_FNBACKUP       сохранить контейнер на диск
typedef Bool32 (*FNCCOM_Backup)(CCOM_handle hcont);
CCOM_FUNC(Bool32) CCOM_Backup (CCOM_handle hcont);
// 19   CCOM_FNRESTORE      восстановить контейнер с диска
typedef CCOM_handle (*FNCCOM_Restore)(void);
CCOM_FUNC(CCOM_handle) CCOM_Restore (void);
// 20   CCOM_FNDELALL       удалить все контейнеры
typedef void (*FNCCOM_DeleteAll)(void);
CCOM_FUNC(void) CCOM_DeleteAll(void);
// 21   CCOM_FNLARGENEW     создать новую компоненту с длинными интервалами
typedef CCOM_comp *(*FNCCOM_LargeNew)(CCOM_handle cont, int32_t upper,
		int32_t left, int32_t w, int32_t h, int32_t size);
CCOM_FUNC(CCOM_comp) *CCOM_LargeNew(CCOM_handle cont,int32_t upper, int32_t left, int32_t w, int32_t h, int32_t size);
// 22   CCOM_FNLARGENEWLN   создать новую линию с длинными интервалами
typedef Bool32 (*FNCCOM_LargeNewLn)(CCOM_comp *cmp, CCOM_lnhead **lnh);
CCOM_FUNC(Bool32) CCOM_LargeNewLn(CCOM_comp *cmp,CCOM_lnhead **lnh);
// 23   CCOM_FNLARGENEWINTERVAL записать новый длинный интервал
typedef Bool32 (*FNCCOM_LargeNewInterval)(CCOM_comp *cmp, int16_t e, int16_t l);
CCOM_FUNC(Bool32) CCOM_LargeNewInterval(CCOM_comp *cmp,int16_t e,int16_t l);
// 24   CCOM_FNLARGECLOSE   закончить обработку компоненты с длинными интервалами
typedef void (*FNCCOM_LargeClose)(CCOM_comp *cmp);
CCOM_FUNC(void) CCOM_LargeClose(CCOM_comp *cmp);
// 25   CCOM_FNSETKILLEDACCESS    режим доступа к убитым
typedef Bool32 (*FNCCOM_SetKilledAccess)(Bool32 ShowEnable);
CCOM_FUNC(Bool32) CCOM_SetKilledAccess(Bool32 ShowEnable);
// 26   CCOM_FNKILL               удалить без уничтожения
typedef Bool32 (*FNCCOM_Kill)(CCOM_comp *c);
CCOM_FUNC(Bool32) CCOM_Kill(CCOM_comp *c);
// 27   CCOM_FNGETEXTRASTER    получить рабочий растр компоненты
typedef Bool32 (*FNCCOM_GetExtRaster)(CCOM_comp * comp, RecRaster *rec);
CCOM_FUNC(Bool32) CCOM_GetExtRaster(CCOM_comp * comp, RecRaster *rec);
// 28   CCOM_FNCLEARCONTAINER    удалить все компоненты  в контейнере
typedef Bool32 (*FNCCOM_ClearContatiner)(CCOM_handle con);
CCOM_FUNC(Bool32) CCOM_ClearContatiner(CCOM_handle con);
// 29   CCOM_FNGETSCALERASTER    ЛП в растр со сжатием
typedef Bool32 (*FNCCOM_GetScaleRaster)(CCOM_comp * comp, RecRaster *rec,
		int32_t scale);
CCOM_FUNC(Bool32) CCOM_GetScaleRaster(CCOM_comp * comp, RecRaster *rec,int32_t scale);
// 30   CCOM_COMPRESSCONTAINER  удалить мелочь
typedef Bool32 (*FNCCOM_CompressContatiner)(CCOM_handle hcont);
CCOM_FUNC(Bool32) CCOM_CompressContatiner(CCOM_handle hcont);
// 31   CCOM_SETLANGUAGE        установить язык контейнера
typedef Bool32 (*FNCCOM_SetLanguage)(CCOM_handle hcont, int16_t language);
CCOM_FUNC(Bool32) CCOM_SetLanguage(CCOM_handle hcont,int16_t language);
// 32   CCOM_GETLANGUAGE        получить язык контейнера
typedef Bool32 (*FNCCOM_GetLanguage)(CCOM_handle hcont, int16_t *language);
CCOM_FUNC(Bool32) CCOM_GetLanguage(CCOM_handle hcont,int16_t *language);
// 33   CCOM_GECONTAINERVOLUME  число компонент в контейнере
typedef int32_t (*FNCCOM_GetContainerVolume)(CCOM_handle hcont);
CCOM_FUNC(int32_t) CCOM_GetContainerVolume(CCOM_handle hcont);
// 34   CCOM_YDECR:             уменьшить ординаты всех компонент контейнера на 1
typedef Bool32 (*FNCCOM_YDecr)(CCOM_handle hcont);
CCOM_FUNC(Bool32) CCOM_YDecr (CCOM_handle hcont);
// 35   CCOM_YINCR:             увеличить ординаты всех компонент контейнера на 1
typedef Bool32 (*FNCCOM_YIncr)(CCOM_handle hcont);
CCOM_FUNC(Bool32) CCOM_YIncr (CCOM_handle hcont);
// 36   CCOM_FNREANIMATE               вернуть к жизни условно удалённые
typedef Bool32 (*FNCCOM_Reanimate)(CCOM_comp *c);
CCOM_FUNC(Bool32) CCOM_Reanimate(CCOM_comp *c);
// 37   CCOM_FNADDCOMPTORS
// Добавить ЛП к растру -- исправленный вариант
typedef Bool32 (*FNCCOM_AddCompToRaster)(CCOM_comp * comp, int16_t relleft,
		int16_t relupper, RecRaster *rec);
CCOM_FUNC(Bool32) CCOM_AddCompToRaster(CCOM_comp * comp, int16_t relleft, int16_t relupper, RecRaster *rec);
// error code


#ifdef __cplusplus
}
#endif

#endif
