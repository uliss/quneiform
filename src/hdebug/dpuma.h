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

#ifndef __DPUMA_H__
#define __DPUMA_H__

#include "globus.h"
#include "charsets.h"
#include "point.h"
#include "rect.h"

#ifdef __DPUMA__
#define DPUMA_FUNC  //FUN_EXPO__
#else
#define DPUMA_FUNC  //FUN_IMPO__
#endif

#define  DPUMA_REC_MAX_RASTER_SIZE   4*1024  // 256*128
typedef struct DPUMA_tagRecRaster
{
    int32_t lnPixWidth;
    int32_t lnPixHeight;
    int32_t lnRasterBufSize; // maximum of raster length
    uchar Raster[DPUMA_REC_MAX_RASTER_SIZE]; // image
} DPUMA_RecRaster;

enum DPUMA_EXPORT
{
    DPUMA_FNDPUMA_CreateSnap,
    DPUMA_FNDPUMA_DestroySnap,
    DPUMA_FNDPUMA_Console,
    DPUMA_FNDPUMA_Stop,
    DPUMA_FNDPUMA_IsActive,
    DPUMA_FNDPUMA_StatusLine,
    DPUMA_FNDPUMA_FN,
    DPUMA_FNDPUMA_Registry,
    DPUMA_FNDPUMA_Skip,
    DPUMA_FNDPUMA_StartLoop,
    DPUMA_FNDPUMA_LoopNext,
    DPUMA_FNDPUMA_CreateWindow,
    DPUMA_FNDPUMA_MessageBoxOk,
    DPUMA_FNDPUMA_MessageBoxYesNo,
    DPUMA_FNDPUMA_WaitUserInput,
    DPUMA_FNDPUMA_DrawLine,
    DPUMA_FNDPUMA_DrawRect,
    DPUMA_FNDPUMA_DeleteRects,
    DPUMA_FNDPUMA_DeleteLines,
    DPUMA_FNDPUMA_GetUserPoint,
    DPUMA_FNDPUMA_GetUserRect,
    DPUMA_FNDPUMA_UpdateView,
    DPUMA_FNDPUMA_DrawRaster,
    DPUMA_FNDPUMA_RasterText,
    DPUMA_FNDPUMA_ZoomToRect,
    DPUMA_FNDPUMA_RasterHeader,
    DPUMA_FNDPUMA_DrawFocusRect,
    DPUMA_FNDPUMA_RegVariable,
    DPUMA_FNDPUMA_GetDIBptr,
    DPUMA_FNDPUMA_UnregVariable,
    DPUMA_FNDPUMA_DrawString,
    DPUMA_FNDPUMA_DeleteStrings,
    DPUMA_FNDPUMA_SetCallbackWindowProc,
    DPUMA_FNDPUMA_DeviceToImage,
    DPUMA_FNDPUMA_ImageToDevice,
    DPUMA_FNDPUMA_SetCallbackMainFrameWindowProc,
    DPUMA_FNDPUMA_DestroyWindow,
    DPUMA_FNDPUMA_SendWindow,
    DPUMA_FNDPUMA_SendMainWnd,
    DPUMA_FNDPUMA_CSTR_Monitor,
    DPUMA_FNDPUMA_CSTR_GetPosition,
    DPUMA_FNDPUMA_CSTR_SetPosition,
    DPUMA_FNDPUMA_DestroyRasterWnd,
    DPUMA_FNDPUMA_CSTR_GetLength,
    DPUMA_FNDPUMA_ShowCutPoint,
    DPUMA_FNDPUMA_GetRasterPixel,
    DPUMA_FNDPUMA_CSTR_Update,
    DPUMA_FNDPUMA_SkipEx,
    DPUMA_FNDPUMA_OpenFile,
    DPUMA_FNDPUMA_GetFileName,
    DPUMA_FNDPUMA_HandLayout,
    DPUMA_FNDPUMA_LockImage,
    DPUMA_FNDPUMA_RegistryHelp,
    DPUMA_FNDPUMA_SaveFile,
    DPUMA_FNDPUMA_ProgressStart,
    DPUMA_FNDPUMA_ProgressFinish,
    DPUMA_FNDPUMA_ProgressStep,
    DPUMA_FNDPUMA_SetConsoleProperty,
    DPUMA_FNDPUMA_CreateHistogramm,
    DPUMA_FNDPUMA_AddPointToHistogramm,
    DPUMA_FNDPUMA_DoHistogramm,
    DPUMA_FNDPUMA_DestroyHistogramm,
    DPUMA_FNDPUMA_GetWindowHandle,
    DPUMA_FNDPUMA_GetPrevSkipOwner,
    DPUMA_FNDPUMA_AllocHook,
    DPUMA_FNDPUMA_ConsoleClear,
    DPUMA_FNDPUMA_ConsoleGetCurLine,
    DPUMA_FNDPUMA_SetFileName,
    DPUMA_FNDPUMA_FOpen,
    DPUMA_FNDPUMA_FClose,
    DPUMA_FNDPUMA_FPrintf1024,
    DPUMA_FNDPUMA_FPuts,
    DPUMA_FNDPUMA_DrawLineTip,
    DPUMA_FNDPUMA_DrawRectTip
};
#define DEC_FUNC(a,b,c) typedef a (*FN##b) c; DPUMA_FUNC a b c; a L##b c;
#define DEC_VARG(a,b) typedef a (*FN##b)(const char * lpFormat,char * marker); \
						DPUMA_FUNC a b (const char * lpFormat,char * marker); \
						a L##b (const char * lpFormat,...);
#define DEC_VARGP(a,b,c) typedef a (*FN##b)(c,const char * lpFormat,char * marker); \
						DPUMA_FUNC a b (c,const char * lpFormat,char * marker); \
						a L##b (c,const char * lpFormat,...);
DEC_FUNC(uint32_t, DPUMA_GetErrorCode,(void))
DEC_FUNC(char *, DPUMA_GetErrorString,(uint32_t wError))
DEC_FUNC(uint32_t, DPUMA_CreateSnap,())
DEC_FUNC(void, DPUMA_DestroySnap,())
DEC_FUNC(void, DPUMA_Stop,())
DEC_FUNC(Bool32, DPUMA_IsActive,())
DEC_FUNC(uint32_t, DPUMA_FN,())
DEC_FUNC(void, DPUMA_Registry,(Handle owner, const char * lpName, Handle parent))
DEC_FUNC(Bool16, DPUMA_Skip,(Handle owner))
DEC_FUNC(void, DPUMA_StartLoop,(Handle owner,uint32_t inter_total))
DEC_FUNC(void, DPUMA_LoopNext,(Handle owner))
DEC_FUNC(Handle, DPUMA_CreateWindow,(const char * lpTitle, void * lpDib))
DEC_FUNC(void, DPUMA_DestroyWindow,(Handle wnd))
DEC_FUNC(uint32_t, DPUMA_WaitUserInput,(Handle owner,Handle wnd))
DEC_FUNC(void, DPUMA_DrawLine,(Handle wnd,CIF::Point16 * start, CIF::Point16 * end, int32_t skew, uint32_t rgb, int16_t pen, uint32_t key))
DEC_FUNC(void, DPUMA_DrawRect,(Handle wnd,CIF::Rect16 * rect, int32_t skew, uint32_t rgb, int16_t pen, uint32_t key))
DEC_FUNC(void, DPUMA_DeleteRects,(Handle wnd, uint32_t key))
DEC_FUNC(void, DPUMA_DeleteLines,(Handle wnd, uint32_t key))
DEC_FUNC(Bool16, DPUMA_GetUserPoint,(Handle wnd,CIF::Point16 * point))
DEC_FUNC(Bool16, DPUMA_GetUserRect,(Handle wnd,CIF::Rect16 * rect))
DEC_FUNC(void, DPUMA_UpdateView,(Handle wnd))
DEC_FUNC(void, DPUMA_DrawRaster,(DPUMA_RecRaster * lpRaster))
DEC_FUNC(void, DPUMA_RasterText,(const char * lpText))
DEC_FUNC(void, DPUMA_ZoomToRect,(Handle wnd,CIF::Rect16 * rect))
DEC_FUNC(void, DPUMA_RasterHeader,(char * lpText,uint32_t num))
DEC_FUNC(void, DPUMA_DrawFocusRect,(Handle wnd,CIF::Rect16 * lpRect))
DEC_FUNC(Bool32, DPUMA_RegVariable,(Handle owner, const char * lpName,void * lpData, const char * lpType))
DEC_FUNC(void, DPUMA_UnregVariable,(void * lpData))
DEC_FUNC(void *,DPUMA_GetDIBptr,(Handle wnd))
DEC_FUNC(void, DPUMA_DrawString,(Handle wnd, CIF::Point16 * start, const char * string, int32_t align, uint32_t rgb, int16_t size, uint32_t key))
DEC_FUNC(void, DPUMA_DeleteStrings,(Handle wnd, uint32_t key))
DEC_FUNC(void, DPUMA_DrawLineTip,(Handle wnd,CIF::Point16 * start, CIF::Point16 * end, int32_t skew, uint32_t rgb, int16_t pen, uint32_t key, const char *pTip))
DEC_FUNC(void, DPUMA_DrawRectTip,(Handle wnd,CIF::Rect16 * rect, int32_t skew, uint32_t rgb, int16_t pen, uint32_t key, const char *pTip))

DEC_VARG(void, DPUMA_MessageBoxOk)
DEC_VARG(void, DPUMA_Console)
DEC_VARG(void, DPUMA_ConsoleN)

/* callback functions */
typedef uint32_t (*DPUMA_Callback_WindowProc)(Handle wnd, Handle hWnd, uint32_t message,
        uint32_t wParam, uint32_t lParam);
DEC_FUNC(DPUMA_Callback_WindowProc, DPUMA_SetCallbackWindowProc,(Handle wnd, DPUMA_Callback_WindowProc func))
DEC_FUNC(void, DPUMA_DeviceToImage,(Handle wnd, CIF::Point * p, uint32_t number))
DEC_FUNC(void, DPUMA_ImageToDevice,(Handle wnd, CIF::Point * p, uint32_t number))
DEC_FUNC(DPUMA_Callback_WindowProc, DPUMA_SetCallbackMainFrameWindowProc,(DPUMA_Callback_WindowProc func))
#define WM_DPUMA 0x0500 // WM_USER + 0x100
DEC_FUNC(uint32_t, DPUMA_SendWindow,(Handle wnd,uint32_t message, uint32_t wParam, uint32_t lParam))
DEC_FUNC(uint32_t, DPUMA_SendMainWnd,(uint32_t message, uint32_t wParam, uint32_t lParam))
DEC_FUNC(uint32_t, DPUMA_CSTR_Monitor,(Handle owner, uint32_t cstr_line, uint32_t pos,
                DPUMA_Callback_WindowProc lpproc))
DEC_FUNC(uint32_t, DPUMA_CSTR_GetPosition,( uint32_t * cstr_rester ))
DEC_FUNC(void, DPUMA_CSTR_SetPosition,(uint32_t pos))
DEC_FUNC(void, DPUMA_DestroyRasterWnd,( void ))
DEC_FUNC(uint32_t, DPUMA_CSTR_GetLength,( void ))
DEC_FUNC(void, DPUMA_ShowCutPoint,(uint32_t lpcount, CIF::Point * lppoints ))
DEC_FUNC(CIF::Point,DPUMA_GetRasterPixel,(Handle wnd, CIF::Point point ))
DEC_FUNC(void, DPUMA_CSTR_Update,( void ))
DEC_FUNC(Bool32, DPUMA_SkipEx,(Handle owner,Bool32 bIter,Bool32 bParent,int32_t nSign))
DEC_FUNC(Bool32, DPUMA_OpenFile,(Handle wnd, char * lpFileName))
DEC_FUNC(const char *, DPUMA_GetFileName,(Handle wnd))
DEC_FUNC(void *, DPUMA_HandLayout,( void * pDIB, uint32_t flags ,CIF::Point * p))
DEC_FUNC(Bool32, DPUMA_LockImage,(Handle wnd,Bool32 bLock))

DEC_FUNC(void, DPUMA_RegistryHelp,(Handle owner, const char * lpName, Bool32 bAppend))
DEC_FUNC(Bool32, DPUMA_SaveFile,(void * lpDIB,char * lpFileName,uint32_t nFormat))

// Image Gear constants
enum puma_save_t
{
    PUMA_SAVE_BMP_UNCOMP = 2,
    PUMA_SAVE_TIF_UNCOMP = 40,
    PUMA_SAVE_TIF_G4 = 40 | (4L << 16)
};

DEC_FUNC(void, DPUMA_ProgressStart ,( void ))
DEC_FUNC(void, DPUMA_ProgressFinish ,( void ))
DEC_FUNC(Bool32, DPUMA_ProgressStep ,(uint32_t step, const char * name, uint32_t percent))
DEC_FUNC(Bool32, DPUMA_SetConsoleProperty,( Bool32 bold,Bool32 italic,
                Bool32 strikeout,Bool32 underline,int32_t height, int32_t offset, uint32_t textcolor,
                int32_t charset, const char * name ))
DEC_FUNC(Handle, DPUMA_CreateHistogramm,(const char *, uint32_t size, int32_t * lpVal))
DEC_FUNC(void, DPUMA_AddPointToHistogramm,(Handle hDlg, uint32_t point))
DEC_FUNC(Bool32, DPUMA_DoHistogramm,(Handle hDlg, uint32_t * lpCurpos))
DEC_FUNC(void, DPUMA_DestroyHistogramm,(Handle hDlg))
DEC_FUNC(Handle, DPUMA_TimeStamp,(const char * name,Handle hTimer))
DEC_FUNC(Handle, DPUMA_GetWindowHandle,(const char * name))
DEC_FUNC(Handle, DPUMA_GetPrevSkipOwner,())
DEC_FUNC(int32_t, DPUMA_AllocHook,( int32_t allocType, Handle userData, uint32_t size, uint32_t prevSize,
                int32_t blockType, int32_t requestNumber, puchar filename, int32_t lineNumber))
DEC_FUNC(void, DPUMA_ConsoleClear,(int32_t fromline))
DEC_FUNC(int32_t, DPUMA_ConsoleGetCurLine,())
DEC_FUNC(Bool32, DPUMA_SetFileName,(Handle wnd, const char * lpName))
DEC_FUNC(Handle, DPUMA_FOpen,(const char * lpName,const char * lpMode))
DEC_FUNC(void, DPUMA_FClose,(Handle hFile))
DEC_VARGP(int32_t, DPUMA_FPrintf1024,Handle hFile)
DEC_FUNC(int32_t, DPUMA_FPuts,(Handle hFile,const char * lpString))

#undef DEC_FUNC

enum puma_align_t
{
    TA_LEFT = 0,
    TA_RIGHT = 2,
    TA_CENTER = 6,
    TA_TOP = 0,
    TA_BOTTOM = 8,
    TA_BASELINE = 24,
    TA_NOUPDATECP = 0,
    TA_UPDATECP = 1
};

#endif
