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
// CONTAINER OF THE STRING                                //
////////////////////////////////////////////////////////////
// O.Slavin                                               //
////////////////////////////////////////////////////////////
// Creation     : 20 Jul 1998                             //
////////////////////////////////////////////////////////////
// Modification : 21 Jul 1998                             //
////////////////////////////////////////////////////////////
#ifndef __CSTR_H
#define __CSTR_H

#include "globus.h"
#include "ccomdefs.h"
#include "cstrdefs.h"

#ifndef __RECDEFS_H
#include "recdefs.h"
#endif
#include "memfunc.h"

#ifdef __CSTR__
#define CSTR_FUNC  FUN_EXPO
#else
#define CSTR_FUNC  FUN_IMPO
#endif

#ifdef __cplusplus
extern "C" {
#endif

///////////////////////////////AK

CSTR_FUNC(Bool32) CSTR_Init( uint16_t wHeightCode , Handle hStorage);
CSTR_FUNC(void) CSTR_Done(void);
CSTR_FUNC(uint32_t) CSTR_GetReturnCode(void);
CSTR_FUNC(char*) CSTR_GetReturnString(uint32_t dwError);
CSTR_FUNC(Bool32) CSTR_SetImportData(uint32_t dwType, void * pData);
CSTR_FUNC(Bool32) CSTR_GetExportData(uint32_t dwType, void * pData);

enum CSTRFunctionsExt {
	CSTR_FNNEWLINE,
	CSTR_FNDELLINE,
	CSTR_FNSETSTRATTR,
	CSTR_FNGETSTRATTR,
	CSTR_FNGETFIRST,
	CSTR_FNGETLAST,
	CSTR_FNGETNEXT,
	CSTR_FNGETPREV,
	CSTR_FNGETLINE,
	CSTR_FNSTORELINE,
	CSTR_FNNEWRASTER,
	CSTR_FNSTORERASTER,
	CSTR_FNSTORECOMP,
	CSTR_FNSTORECOLLECTION,
	CSTR_FNDELRASTER,
	CSTR_FNVERIFY,
	CSTR_FNGETRASTER,
	CSTR_FNGETATTR,
	CSTR_FNGETCOLLECT,
	CSTR_FNSETUSER,
	CSTR_FNGETUSER,
	CSTR_FNSETATTR,
	CSTR_FNCOPYLINE,
	CSTR_FNGETLNHND,
	CSTR_FNGETCOMP,
	CSTR_FNNEWUSERCODE,
	CSTR_FNVERSION,
	CSTR_FNTOTXT,
	CSTR_FNINSRST,
	CSTR_FNMAXNUM,
	CSTR_FNFIRSTLN,
	CSTR_FNNEXTTLN,
	CSTR_FNDELALL,
	CSTR_FNNEXT,
	CSTR_FNPREV,
	CSTR_FNSWAP,
	CSTR_FNDELSAVE,
	CSTR_FNRESTRAST,
	CSTR_FNCOMPOSE,
	CSTR_KILLIM,
	CSTR_FNSAVE,
	CSTR_FNGETLIST,
	CSTR_FNSETLIST,
	CSTR_FNGETCOLLECTIONUNI,
	CSTR_FNSTORECOLLECTIONUNI,
	CSTR_FNGETCONTAINER,
	CSTR_FNSTORESCALE,
	CSTR_FNGETLINEFIRST,
	CSTR_FNGETLINENEXT,
	CSTR_FNCLEARLINE,
	CSTR_FNGETRASTERLINE,
	CSTR_FNSETDUP,
	CSTR_FNSETDUPEND,
	CSTR_FNGETDUP,
	CSTR_FNGETDUPEND,
	CSTR_FNPACK,
	CSTR_FNUNPACK,
	CSTR_FNSORTFRAGM,
	CSTR_FNFIRSTLINEFRAGM,
	CSTR_FNNEXTLINEFRAGM,
	CSTR_FNSTORECOMPORIGINAL,
	CSTR_FNGETMAXFRAGMENT,
	CSTR_SAVECONT,
	CSTR_RESTORECONT,
	CSTR_INSERTRASTERDOWN,
	CSTR_COPYRASTER,
	CSTR_DELETELOOPS,
	CSTR_GETNEXTDOWN,
	CSTR_GETPREVDOWN,
	CSTR_GETNEXTUP,
	CSTR_GETPREVUP,
	CSTR_GETLENGTH,
	CSTR_FNTOTXT_COORD
};
enum CSTRFunctionsImp {
	CSTR_FNIMP_ALLOC,
	CSTR_FNIMP_REALLOC,
	CSTR_FNIMP_FREE,
	CSTR_FNIMP_OPEN,
	CSTR_FNIMP_CLOSE,
	CSTR_FNIMP_READ,
	CSTR_FNIMP_WRITE
};

// 1    CSTR_FNNEWLINE      Образовать новую строку
typedef CSTR_line (*FNCSTR_NewLine)(int32_t lineno, int32_t version,
		int32_t container);
CSTR_FUNC(CSTR_line) CSTR_NewLine (int32_t lineno, int32_t version, int32_t container);
// 2    CSTR_FNDELLINE      Удалить строку
typedef Bool32 (*FNCSTR_DeleteLine)(CSTR_line line);
CSTR_FUNC(Bool32) CSTR_DeleteLine (CSTR_line line);
// 3    CSTR_FNSETSTRATTR   установить атрибуты текущей строки
typedef Bool32 (*FNCSTR_SetLineAttr)(CSTR_line line, CSTR_attr * attr);
CSTR_FUNC(Bool32) CSTR_SetLineAttr (CSTR_line line,CSTR_attr * attr);
// 4    CSTR_FNGETSTRATTR   получить атрибуты текущей строки
typedef Bool32 (*FNCSTR_GetLineAttr)(CSTR_line line, CSTR_attr * attr);
CSTR_FUNC(Bool32) CSTR_GetLineAttr (CSTR_line line,CSTR_attr * attr);
// 5    CSTR_FNGETFIRST     дать хэндл первого (фиктивного) растра в строке
typedef CSTR_rast (*FNCSTR_GetFirstRaster)(CSTR_line line);
CSTR_FUNC(CSTR_rast) CSTR_GetFirstRaster (CSTR_line line);
// 6    CSTR_FNGETLAST      дать хэндл последнего (фиктивного) растра в строке
typedef CSTR_rast (*FNCSTR_GetLastRaster)(CSTR_line line);
CSTR_FUNC(CSTR_rast) CSTR_GetLastRaster (CSTR_line line);
// 7    CSTR_FNGETNEXT      дать хэндл следующего растра
typedef CSTR_rast (*FNCSTR_GetNextRaster)(CSTR_rast curr_raster,
		uint32_t type_raster);
CSTR_FUNC(CSTR_rast) CSTR_GetNextRaster (CSTR_rast curr_raster, uint32_t type_raster);
// 8    CSTR_FNGETPREV      дать хэндл предыдущего растра
typedef CSTR_rast (*FNCSTR_GetPrevRaster)(CSTR_rast curr_raster,
		uint32_t type_raster);
CSTR_FUNC(CSTR_rast) CSTR_GetPrevRaster (CSTR_rast curr_raster, uint32_t type_raster);
// 9    CSTR_FNGETLINE      дать хэндл строки, сопряжённой с растром
typedef CSTR_line (*FNCSTR_GetLine)(CSTR_rast curr_raster);
CSTR_FUNC(CSTR_line) CSTR_GetLine (CSTR_rast curr_raster);
// 10   CSTR_FNSTORELINE    придать растру строку
typedef Bool32 (*FNCSTR_StoreLine)(CSTR_rast curr_raster, CSTR_line line);
CSTR_FUNC(Bool32) CSTR_StoreLine (CSTR_rast curr_raster, CSTR_line line);
// 11   CSTR_FNNEWRASTER    создать новый растр в текущей строке
typedef CSTR_rast (*FNCSTR_NewRaster)(CSTR_line line, int32_t col, int32_t row,
		int32_t w);
CSTR_FUNC(CSTR_rast) CSTR_NewRaster (CSTR_line line ,int32_t col, int32_t row, int32_t w);
// 12   CSTR_FNSTORERASTER  запомнить растр как изображение
typedef Bool32 (*FNCSTR_StoreRaster)(CSTR_rast curr_raster, RecRaster *image);
CSTR_FUNC(Bool32) CSTR_StoreRaster (CSTR_rast curr_raster, RecRaster *image);
// 13   CSTR_FNSTORECOMP запомнить растр как компоненту с интервальным представлением
typedef Bool32 (*FNCSTR_StoreComp)(CSTR_rast curr_raster, uchar *lp,
		Bool32 raster_init, uchar scale);
CSTR_FUNC(Bool32) CSTR_StoreComp (CSTR_rast curr_raster, uchar *lp, Bool32 raster_init, uchar scale);
// 14   CSTR_FNSTORECOLLECTION запомнить коллекцию
typedef Bool32 (*FNCSTR_StoreCollection)(CSTR_rast curr_raster,
		RecVersions *collect);
CSTR_FUNC(Bool32) CSTR_StoreCollection (CSTR_rast curr_raster, RecVersions *collect);
// 15   CSTR_FNDELRASTER    удалить растр
typedef CSTR_rast (*FNCSTR_DelRaster)(CSTR_rast curr_raster);
CSTR_FUNC(CSTR_rast) CSTR_DelRaster (CSTR_rast curr_raster);
// 16   CSTR_FNVERIFY       отсортировать список строки
typedef Bool32 (*FNCSTR_VerifyLine)(CSTR_line line);
CSTR_FUNC(Bool32) CSTR_VerifyLine (CSTR_line line);
// 17   CSTR_FNGETRASTER    получить изображение (растр или интервалы)
typedef Bool32 (*FNCSTR_GetImage)(CSTR_rast curr_raster, uchar *out_res,
		uint32_t type_image);
CSTR_FUNC(Bool32) CSTR_GetImage (CSTR_rast curr_raster, uchar *out_res, uint32_t type_image);
// 18   CSTR_FNGETATTR      получить атрибуты растра
typedef Bool32 (*FNCSTR_GetAttr)(CSTR_rast curr_raster, CSTR_rast_attr * attr);
CSTR_FUNC(Bool32) CSTR_GetAttr (CSTR_rast curr_raster, CSTR_rast_attr * attr);
// 19   CSTR_FNGETCOLLECT   получить коллекцию
typedef Bool32 (*FNCSTR_GetCollection)(CSTR_rast curr_raster,
		RecVersions *result_collect);
CSTR_FUNC(Bool32) CSTR_GetCollection (CSTR_rast curr_raster, RecVersions *result_collect);
// 20   CSTR_FNSETUSER      записать в растр пользовательские данные
typedef Bool32 (*FNCSTR_SetUserAttr)(CSTR_rast raster, CCOM_USER_BLOCK *ub);
CSTR_FUNC(Bool32) CSTR_SetUserAttr (CSTR_rast raster, CCOM_USER_BLOCK *ub);
// 21   CSTR_FNGETUSER      прочитать из растра пользовательские данные
typedef Bool32 (*FNCSTR_GetUserAttr)(CSTR_rast raster, CCOM_USER_BLOCK *ub);
CSTR_FUNC(Bool32) CSTR_GetUserAttr (CSTR_rast raster, CCOM_USER_BLOCK *ub);
// 22   CSTR_FNSETATTR      записать атрибуты растра
typedef Bool32 (*FNCSTR_SetAttr)(CSTR_rast curr_raster, CSTR_rast_attr * attr);
CSTR_FUNC(Bool32) CSTR_SetAttr (CSTR_rast curr_raster, CSTR_rast_attr * attr);
// 23   CSTR_FNCOPYLINE     скопировать содержимое строки
typedef Bool32 (*FNCSTR_CopyLine)(CSTR_line trg, CSTR_line src);
CSTR_FUNC(Bool32) CSTR_CopyLine (CSTR_line trg,CSTR_line src);
// 24   CSTR_FNGETLNHND     дать хендл строки по номеру и версии
typedef CSTR_line (*FNCSTR_GetLineHandle)(int32_t line_no, int32_t version);
CSTR_FUNC(CSTR_line ) CSTR_GetLineHandle (int32_t line_no, int32_t version);
// 25   CSTR_FNGETCOMP      получить компонентy растра
typedef CCOM_comp * (*FNCSTR_GetComp)(CSTR_rast curr_raster);
CSTR_FUNC(CCOM_comp *) CSTR_GetComp (CSTR_rast curr_raster);
// 26   CSTR_FNNEWUSERCODE  получить доступный код пользовательского блока
typedef int32_t (*FNCSTR_NewUserCode)(void);
CSTR_FUNC(int32_t) CSTR_NewUserCode (void);
// 27   CSTR_FNVERSION      версия библиотеки
// 28   CSTR_FNTOTXT        получить текстовый обрах строки
typedef Bool32 (*FNCSTR_LineToTxt)(CSTR_line lin, char *txt);
CSTR_FUNC(Bool32) CSTR_LineToTxt (CSTR_line lin, char *txt);
// 29   CSTR_FNINSRST       вставить растр после текущего
typedef CSTR_rast (*FNCSTR_InsertRaster)(CSTR_rast curr_raster);
CSTR_FUNC(CSTR_rast) CSTR_InsertRaster (CSTR_rast curr_raster);
// 30   CSTR_FNMAXNUM       наибольший номер строки в контейнере
typedef int32_t (*FNCSTR_GetMaxNumber)(void);
CSTR_FUNC(int32_t) CSTR_GetMaxNumber (void);
// 31   CSTR_FNFIRSTLN      первая строка указанной версии
typedef CSTR_line (*FNCSTR_FirstLine)(int32_t version);
CSTR_FUNC(CSTR_line) CSTR_FirstLine ( int32_t version);
// 32   CSTR_FNNEXTTLN      следующая строка указанной версии
typedef CSTR_line (*FNCSTR_NextLine)(CSTR_line start, int32_t version);
CSTR_FUNC(CSTR_line) CSTR_NextLine ( CSTR_line start,int32_t version);
// 33   CSTR_FNDELALL       удалить все строки
typedef void (*FNCSTR_DeleteAll)(void);
CSTR_FUNC(void) CSTR_DeleteAll(void);
// 34   CSTR_FNNEXT         следующий нефиктивный растр
typedef CSTR_rast (*FNCSTR_GetNext)(CSTR_rast rst);
CSTR_FUNC(CSTR_rast) CSTR_GetNext(CSTR_rast rst);
// 35   CSTR_FNPREV         предыдущий нефиктивный растр
typedef CSTR_rast (*FNCSTR_GetPrev)(CSTR_rast rst);
CSTR_FUNC(CSTR_rast) CSTR_GetPrev(CSTR_rast rst);
// 36   CSTR_FNSWAP         обменять местами 2 растра
typedef Bool32 (*FNCSTR_SwapRasters)(CSTR_rast r1, CSTR_rast r2);
CSTR_FUNC(Bool32) CSTR_SwapRasters(CSTR_rast r1, CSTR_rast r2);
// 37   CSTR_FNDELSAVE      удалить с сохранением
typedef CSTR_rast (*FNCSTR_DelSaveRaster)(CSTR_rast c);
CSTR_FUNC(CSTR_rast) CSTR_DelSaveRaster(CSTR_rast c);
// 38   CSTR_FNRESTRAST     восстановить
typedef CSTR_rast (*FNCSTR_RestoreRaster)(CSTR_line ln, CSTR_rast rst);
CSTR_FUNC(CSTR_rast) CSTR_RestoreRaster(CSTR_line ln,CSTR_rast rst);
// 39   CSTR_FNCOMPOSE      склеить массив растров
typedef CSTR_rast (*FNCSTR_compose_Cell)(int32_t n, CSTR_rast *clist,
		int32_t nIncline, Bool32 NeedDel);
CSTR_FUNC(CSTR_rast) CSTR_compose_Cell(int32_t n,CSTR_rast *clist, int32_t nIncline, Bool32 NeedDel);
// 40   CSTR_KILLIM         убить образ в растре
typedef Bool32 (*FNCSTR_KillImage)(CSTR_rast rst);
CSTR_FUNC(Bool32) CSTR_KillImage(CSTR_rast rst);
// 41   CSTR_FNSAVE
typedef CSTR_rast (*FNCSTR_SaveRaster)(CSTR_rast c);
CSTR_FUNC(CSTR_rast) CSTR_SaveRaster(CSTR_rast c);
// 42   CSTR_FNGETLIST
typedef CSTR_rast (*FNCSTR_GetComplist)(CSTR_rast rst);
CSTR_FUNC(CSTR_rast) CSTR_GetComplist(CSTR_rast rst);
// 43   CSTR_FNSETLIST
typedef Bool32 (*FNCSTR_SetComplist)(CSTR_rast rst, CSTR_rast complist);
CSTR_FUNC(Bool32) CSTR_SetComplist(CSTR_rast rst, CSTR_rast complist);
// 44   CSTR_FNGETCOLLECTIONUNI дать расширенную коллекцию
typedef Bool32(*FNCSTR_GetCollectionUni)(CSTR_rast curr_raster,
		UniVersions *result_collect);
CSTR_FUNC(Bool32) CSTR_GetCollectionUni (CSTR_rast curr_raster, UniVersions *result_collect);
// 45   CSTR_FNSTORECOLLECTIONUNI запомнить расширенную коллекцию
typedef Bool32(*FNCSTR_StoreCollectionUni)(CSTR_rast curr_raster,
		UniVersions *result_collect);
CSTR_FUNC(Bool32) CSTR_StoreCollectionUni (CSTR_rast curr_raster, UniVersions *result_collect);
// 46   CSTR_FNGETCONTAINER дать хэндл контейнера строки
typedef CCOM_handle (*FNCSTR_GetContainer)(CSTR_line ln);
CSTR_FUNC(CCOM_handle) CSTR_GetContainer(CSTR_line ln);
// 47   CSTR_FNSTORESCALE   запомнить код масштабирования
typedef Bool32 (*FNCSTR_StoreScale)(CSTR_rast curr_raster, uchar scale);
CSTR_FUNC(Bool32) CSTR_StoreScale(CSTR_rast curr_raster,uchar scale);
// 48   CSTR_GETLINEFIRST   дать хэндл первой строки данных фрагмента и версии
typedef CSTR_line (*FNCSTR_GetLineFirst)(int32_t fragment_no, int32_t version);
CSTR_FUNC(CSTR_line ) CSTR_GetLineFirst ( int32_t fragment_no, int32_t version);
// 49   CSTR_GETLINENEXT    дать хэндл следующей строки данных фрагмента и версии
typedef CSTR_line (*FNCSTR_GetLineNext)(CSTR_line lin, int32_t fragment_no,
		int32_t version);
CSTR_FUNC(CSTR_line ) CSTR_GetLineNext (CSTR_line lin, int32_t fragment_no, int32_t version);
// 50   CSTR_FNCLEARLINE    Удалить растры из строки
typedef Bool32 (*FNCSTR_ClearLine)(CSTR_line lin, int16_t left, int16_t right);
CSTR_FUNC(Bool32) CSTR_ClearLine(CSTR_line lin,int16_t left, int16_t right);
// 51   CSTR_GETRASTERLINE      дать хэндл строки растра
typedef CSTR_line (*FNCSTR_GetRasterLine)(CSTR_rast curr_raster);
CSTR_FUNC(CSTR_line) CSTR_GetRasterLine (CSTR_rast curr_raster);
// 52   CSTR_SETDUP             запомнить хэндл сопряжённого растра
typedef Bool32 (*FNCSTR_SetDup)(CSTR_rast rus, CSTR_rast eng);
CSTR_FUNC(Bool32) CSTR_SetDup(CSTR_rast rus, CSTR_rast eng);
// 53   CSTR_SETDUPEND          запомнить хэндл дополнительного сопряжённого растра
typedef Bool32 (*FNCSTR_SetDupEnd)(CSTR_rast rus, CSTR_rast eng);
CSTR_FUNC(Bool32) CSTR_SetDupEnd(CSTR_rast rus,CSTR_rast eng);
// 54   CSTR_GETDUP             дать хэндл сопряжённого растра
typedef CSTR_rast (*FNCSTR_GetDup)(CSTR_rast rus);
CSTR_FUNC(CSTR_rast) CSTR_GetDup(CSTR_rast rus);
// 55   CSTR_GETDUPEND          дать хэндл дополнительного сопряжённого растра
typedef CSTR_rast (*FNCSTR_GetDupEnd)(CSTR_rast rus);
CSTR_FUNC(CSTR_rast) CSTR_GetDupEnd(CSTR_rast rus);
// 56   CSTR_FNPACK:            удалить дублирующую информацию
typedef Bool32 (*FNCSTR_PackLine)(CSTR_line line);
CSTR_FUNC(Bool32) CSTR_PackLine(CSTR_line line);
// 57   CSTR_FNUNPACK:          восстановить дублирующую информацию
typedef Bool32 (*FNCSTR_UnpackLine)(CSTR_line line);
CSTR_FUNC(Bool32) CSTR_UnpackLine(CSTR_line line);
// 58   CSTR_FNSORTFRAGM        отсортировать по фрагментам
typedef Bool32 (*FNCSTR_SortFragm)(int32_t version);
CSTR_FUNC(Bool32) CSTR_SortFragm ( int32_t version);
// 59   CSTR_FNFIRSTLINEFRAGM   первая во фрагменте
typedef CSTR_line (*FNCSTR_FirstLineFragm)(int32_t fragm, int32_t version);
CSTR_FUNC(CSTR_line) CSTR_FirstLineFragm ( int32_t fragm, int32_t version );
// 60   CSTR_FNNEXTLINEFRAGM    следующая по фрагменту
typedef CSTR_line (*FNCSTR_NextLineFragm)(CSTR_line start);
CSTR_FUNC(CSTR_line) CSTR_NextLineFragm ( CSTR_line start);
// 61   CSTR_FNSTORECOMPORIGINAL    запомнить компоненту (для оригинального контейнера)
typedef Bool32 (*FNCSTR_StoreCompOriginal)(CSTR_rast curr_raster,
		CCOM_comp * comp, Bool32 raster_init);
CSTR_FUNC(Bool32) CSTR_StoreCompOriginal (CSTR_rast curr_raster, CCOM_comp * comp, Bool32 raster_init);
// 62   CSTR_FNGETMAXFRAGMENT   максимальный номер фрагмента
typedef int32_t (*FNCSTR_GetMaxFragment)(int32_t version);
CSTR_FUNC(int32_t) CSTR_GetMaxFragment(int32_t version);
// 63   CSTR_SAVECONT           сохранить контейнер на диске
typedef Bool32 (*FNCSTR_SaveCont)(char *filename);
CSTR_FUNC(Bool32) CSTR_SaveCont(char *filename);
// 64   CSTR_RESTORECONT        воостановить контейнер с диска
typedef Bool32 (*FNCSTR_RestoreCont)(char *filename);
CSTR_FUNC(Bool32) CSTR_RestoreCont(char *filename);
// 65   CSTR_INSERTRASTERDOWN   вставить ссылку вниз
typedef Bool32 (*FNCSTR_InsertRasterDown)(CSTR_rast start_raster,
		CSTR_rast stop_raster);
CSTR_FUNC(CSTR_rast) CSTR_InsertRasterDown (CSTR_rast start_raster, CSTR_rast stop_raster);
// 66   CSTR_COPYRASTER                 скопировать содержимое растра
typedef Bool32 (*FNCSTR_CopyRaster)(CSTR_rast trg, CSTR_rast src);
CSTR_FUNC(Bool32) CSTR_CopyRaster(CSTR_rast trg,CSTR_rast src);
// 67   CSTR_DELETELOOPS                удалить обходные пути
typedef Bool32 (*FNCSTR_DeleteLoops)(CSTR_line lin);
CSTR_FUNC(Bool32) CSTR_DeleteLoops (CSTR_line lin);
// 68   CSTR_GETNEXTDOWN                ссылка на петлю вперёд
typedef CSTR_rast (*FNCSTR_GetNextDown)(CSTR_rast rst);
CSTR_FUNC(CSTR_rast) CSTR_GetNextDown(CSTR_rast rst);
// 69   CSTR_GETPREVDOWN                ссылка на петлю назад
typedef CSTR_rast (*FNCSTR_GetPrevDown)(CSTR_rast rst);
CSTR_FUNC(CSTR_rast) CSTR_GetPrevDown(CSTR_rast rst);
// 70   CSTR_GETNEXTUP                  ссылка из петли вперёд
typedef CSTR_rast (*FNCSTR_GetNextUp)(CSTR_rast rst);
CSTR_FUNC(CSTR_rast) CSTR_GetNextUp(CSTR_rast rst);
// 71   CSTR_GETPREVUP                  ссылка из петли назад
typedef CSTR_rast (*FNCSTR_GetPrevUp)(CSTR_rast rst);
CSTR_FUNC(CSTR_rast) CSTR_GetPrevUp(CSTR_rast rst);
// 72   CSTR_GETLENGTH                  длина нужного текстового буфера
typedef int32_t (*FNCSTR_GetLength)(CSTR_line lin);
CSTR_FUNC(int32_t) CSTR_GetLength (CSTR_line lin);
// 73   CSTR_FNTOTXT_COORD              получить текстовый обрах строки с координатами
typedef Bool32 (*FNCSTR_LineToTxt_Coord)(CSTR_line lin, char *txt, int32_t len);
CSTR_FUNC(Bool32) CSTR_LineToTxt_Coord (CSTR_line lin, char *txt, int32_t len);

#ifdef __cplusplus
}
#endif

#endif
