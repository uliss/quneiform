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

/**********  Заголовок  **********/
/*  Автор      :  Александр Михайлов                                          */
/*  Редакция   :  06.09.00                                                    */
/*  Файл       :  'AM_COMM.H'                                                 */
/*  Содержание :  Прототипы всех используемых в содержательной части моих     */
/*                библиотек при отладке функций ввода/вывода.                 */
/*  Назначение :  Обмен с консолью и диском.                                  */
/*----------------------------------------------------------------------------*/
#include "cttypes.h"
#ifndef Handle
	#define Handle void *
#endif
typedef int (*FN_AM_Console) (const char * lpFormat,...);
typedef int (*FN_AM_ConsolN) (const char * lpFormat,...);
typedef void (*FN_AM_MessageBoxOk) (const char * lpFormat,...);
/*------------own functions---------------------------------------------------*/
int AM_OpenRes_rv_fte (int Code, const char *pFile);
int AM_SaveRes_rv_fte (int Code, const char *pFile);
int AM_WriteRes_rv_fte (int Code, const char *pStr);
void CloseAllRes ();
Bool   AM_InitComm (uint16_t wRltOwnCode);
void AM_DoneComm ();
uint32_t AM_GetOperCode (uint16_t wLocCode);
void   AM_RegiVert (Handle owner, const char *lpName, Handle parent);
void   AM_RegiVari (Handle owner, const char *lpName, void *lpData, const char *lpType);
void AM_RegiHelp (Handle owner, const char *lpName, Bool32 bAppend);
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
uint32_t AM_WaitUserInput (Handle owner, Handle wnd);
void   AM_DrawLine (Handle wnd, Point16 *start, Point16 *end, int32_t skew, uint32_t rgb, int16_t pen, uint32_t key);
void   AM_DrawRect (Handle wnd, Rect16 *pRc, int32_t skew, uint32_t rgb, int16_t pen, uint32_t key);
void   AM_DeleteLines (Handle wnd, uint32_t key);
void   AM_DeleteRects (Handle wnd, uint32_t key);
Bool16 AM_Skip (Handle owner);
#ifdef _AM_Comm_
	FN_AM_Console AM_Console;
	FN_AM_ConsolN AM_ConsolN;
	FN_AM_MessageBoxOk AM_MessageBoxOk;
#else
	extern FN_AM_Console AM_Console;
	extern FN_AM_ConsolN AM_ConsolN;
	extern FN_AM_MessageBoxOk AM_MessageBoxOk;
#endif
Handle AM_CreateWindow (const char *lpTitle, void *lpDib);
Handle AM_GetWindowHandle (const char *name);
char * AM_GetFileName (Handle wnd);
/*----------------------------------------------------------------------------*/
