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

#ifndef __GLOBUS_H
#include "globus.h"
#endif
/*
#ifndef __RECDEFS_H
   #include "recdefs.h"
#endif
   #include "memfunc.h"
*/
#ifdef __DPUMA__
  #define DPUMA_FUNC  FUN_EXPO
#else
  #define DPUMA_FUNC  FUN_IMPO
#endif

#ifdef __cplusplus
	extern "C" {
#endif


#pragma pack (push,8)
////////////////////////////////////

#define  DPUMA_REC_MAX_RASTER_SIZE   4*1024  // 256*128
typedef struct DPUMA_tagRecRaster
   {
      int32_t    lnPixWidth;
      int32_t    lnPixHeight;
      int32_t    lnRasterBufSize;				// maximum of raster length
      Word8    Raster[DPUMA_REC_MAX_RASTER_SIZE];	// image
   }  DPUMA_RecRaster;

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


#define DEC_FUNC(a,b,c) typedef a (*FN##b) c; DPUMA_FUNC(a) b c; a L##b c
#define DEC_VARG(a,b) typedef a (*FN##b)(const char * lpFormat,char * marker); \
						DPUMA_FUNC(a) b (const char * lpFormat,char * marker); \
						a L##b (const char * lpFormat,...)
#define DEC_VARGP(a,b,c) typedef a (*FN##b)(c,const char * lpFormat,char * marker); \
						DPUMA_FUNC(a) b (c,const char * lpFormat,char * marker); \
						a L##b (c,const char * lpFormat,...)

DEC_FUNC(Bool32,  DPUMA_Init,(Word16 wHightCode, Handle hStorage));
DEC_FUNC(Bool32,  DPUMA_Done,());
DEC_FUNC(Word32,  DPUMA_GetErrorCode,(void));
DEC_FUNC(Int8 *,  DPUMA_GetErrorString,(Word32 wError));
DEC_FUNC(Bool32,  DPUMA_SetImportData,(Word32 wType, void * pData));
DEC_FUNC(Bool32,  DPUMA_GetExportData,(Word32 wType, void * pData));

DEC_FUNC(Word32, DPUMA_CreateSnap,());
DEC_FUNC(void, DPUMA_DestroySnap,());
DEC_FUNC(void, DPUMA_Stop,());
DEC_FUNC(Bool32, DPUMA_IsActive,());
DEC_FUNC(Word32, DPUMA_FN,());
DEC_FUNC(void, DPUMA_Registry,(Handle owner, const char * lpName, Handle parent));
DEC_FUNC(Bool16, DPUMA_Skip,(Handle owner));
DEC_FUNC(void, DPUMA_StartLoop,(Handle owner,Word32 inter_total));
DEC_FUNC(void, DPUMA_LoopNext,(Handle owner));
DEC_FUNC(Handle, DPUMA_CreateWindow,(const char * lpTitle, void * lpDib));
DEC_FUNC(void,  DPUMA_DestroyWindow,(Handle wnd));
DEC_FUNC(Word32,  DPUMA_WaitUserInput,(Handle owner,Handle wnd));
DEC_FUNC(void,  DPUMA_DrawLine,(Handle wnd,Point16 * start, Point16 * end, int32_t skew, Word32 rgb, Int16 pen, Word32 key));
DEC_FUNC(void,  DPUMA_DrawRect,(Handle wnd,Rect16 * rect, int32_t skew, Word32 rgb, Int16 pen, Word32 key));
DEC_FUNC(void,  DPUMA_DeleteRects,(Handle wnd, Word32 key));
DEC_FUNC(void,  DPUMA_DeleteLines,(Handle wnd, Word32 key));
DEC_FUNC(Bool16,  DPUMA_GetUserPoint,(Handle wnd,Point16 * point));
DEC_FUNC(Bool16,  DPUMA_GetUserRect,(Handle wnd,Rect16 * rect));
DEC_FUNC(void,  DPUMA_UpdateView,(Handle wnd));
DEC_FUNC(void,  DPUMA_DrawRaster,(DPUMA_RecRaster * lpRaster));
DEC_FUNC(void,  DPUMA_RasterText,(char * lpText));
DEC_FUNC(void,  DPUMA_ZoomToRect,(Handle wnd,Rect16 * rect));
DEC_FUNC(void,  DPUMA_RasterHeader,(char * lpText,Word32 num));
DEC_FUNC(void,  DPUMA_DrawFocusRect,(Handle wnd,Rect16 * lpRect));
DEC_FUNC(Bool32,  DPUMA_RegVariable,(Handle owner, const char * lpName,void * lpData, const char * lpType));
DEC_FUNC(void,  DPUMA_UnregVariable,(void * lpData));
DEC_FUNC(void *,DPUMA_GetDIBptr,(Handle wnd));
DEC_FUNC(void,  DPUMA_DrawString,(Handle wnd, Point16 * start, const char * string, int32_t align, Word32 rgb, Int16 size, Word32 key));
DEC_FUNC(void,  DPUMA_DeleteStrings,(Handle wnd, Word32 key));
DEC_FUNC(void,  DPUMA_DrawLineTip,(Handle wnd,Point16 * start, Point16 * end, int32_t skew, Word32 rgb, Int16 pen, Word32 key, const char *pTip));
DEC_FUNC(void,  DPUMA_DrawRectTip,(Handle wnd,Rect16 * rect, int32_t skew, Word32 rgb, Int16 pen, Word32 key, const char *pTip));

DEC_VARG(void,		DPUMA_MessageBoxOk);
DEC_VARG(void,		DPUMA_StatusLine);
DEC_VARG(Bool16,	DPUMA_MessageBoxYesNo);
DEC_VARG(int32_t,		DPUMA_Console);
DEC_VARG(int32_t,		DPUMA_ConsoleN);

/* callback functions */
typedef Word32 (*DPUMA_Callback_WindowProc)(Handle wnd,Handle hWnd,Word32 message,Word32 wParam,Word32 lParam);

DEC_FUNC(DPUMA_Callback_WindowProc,  DPUMA_SetCallbackWindowProc,(Handle wnd, DPUMA_Callback_WindowProc func));
DEC_FUNC(void,  DPUMA_DeviceToImage,(Handle wnd, Point32 * p, Word32 number));
DEC_FUNC(void,  DPUMA_ImageToDevice,(Handle wnd, Point32 * p, Word32 number));
DEC_FUNC(DPUMA_Callback_WindowProc,  DPUMA_SetCallbackMainFrameWindowProc,(DPUMA_Callback_WindowProc func));

#define WM_DPUMA 0x0500 // WM_USER + 0x100

DEC_FUNC(Word32,  DPUMA_SendWindow,(Handle wnd,Word32 message, Word32 wParam, Word32 lParam));
DEC_FUNC(Word32,  DPUMA_SendMainWnd,(Word32 message, Word32 wParam, Word32 lParam));

DEC_FUNC(Word32,  DPUMA_CSTR_Monitor,(Handle owner, Word32 cstr_line, Word32 pos,
									  DPUMA_Callback_WindowProc lpproc));

DEC_FUNC(Word32, DPUMA_CSTR_GetPosition,( Word32 * cstr_rester ));
DEC_FUNC(void,   DPUMA_CSTR_SetPosition,(Word32 pos));
DEC_FUNC(void,   DPUMA_DestroyRasterWnd,( void ));
DEC_FUNC(Word32, DPUMA_CSTR_GetLength,( void ));
DEC_FUNC(void,   DPUMA_ShowCutPoint,(Word32 lpcount, Point32 * lppoints ));
DEC_FUNC(Point32,DPUMA_GetRasterPixel,(Handle wnd, Point32 point ));
DEC_FUNC(void,   DPUMA_CSTR_Update,( void ));
DEC_FUNC(Bool32, DPUMA_SkipEx,(Handle owner,Bool32 bIter,Bool32 bParent,int32_t nSign));

DEC_FUNC(Bool32, DPUMA_OpenFile,(Handle wnd, char * lpFileName));
DEC_FUNC(char *, DPUMA_GetFileName,(Handle wnd));
DEC_FUNC(void *, DPUMA_HandLayout,( void * pDIB, Word32 flags ,Point32 * p));
DEC_FUNC(Bool32, DPUMA_LockImage,(Handle wnd,Bool32 bLock));

DEC_FUNC(void,   DPUMA_RegistryHelp,(Handle owner, const char * lpName, Bool32 bAppend));
DEC_FUNC(Bool32, DPUMA_SaveFile,(void * lpDIB,char * lpFileName,Word32 nFormat));
// Image Gear constants
#define PUMA_SAVE_BMP_UNCOMP       (2)
#define PUMA_SAVE_TIF_UNCOMP       (40)
#define PUMA_SAVE_TIF_G4           (40|(4L << 16))
DEC_FUNC(void,	 DPUMA_ProgressStart	,( void ));
DEC_FUNC(void,	 DPUMA_ProgressFinish	,( void ));
DEC_FUNC(Bool32, DPUMA_ProgressStep		,(Word32 step, const char * name, Word32 percent));
DEC_FUNC(Bool32, DPUMA_SetConsoleProperty,(	Bool32 bold,Bool32 italic,
	Bool32 strikeout,Bool32 underline,int32_t height,	int32_t offset,	Word32 textcolor,
	int32_t charset,	const char * name ));
DEC_FUNC(Handle, DPUMA_CreateHistogramm,(const char *, Word32 size, int32_t * lpVal));
DEC_FUNC(void,	 DPUMA_AddPointToHistogramm,(Handle hDlg, Word32 point));
DEC_FUNC(Bool32, DPUMA_DoHistogramm,(Handle hDlg, PWord32 lpCurpos));
DEC_FUNC(void,	 DPUMA_DestroyHistogramm,(Handle hDlg));
DEC_FUNC(Handle, DPUMA_TimeStamp,(const char * name,Handle hTimer));
DEC_FUNC(Handle, DPUMA_GetWindowHandle,(const char * name));
DEC_FUNC(Handle, DPUMA_GetPrevSkipOwner,());
DEC_FUNC(int32_t,  DPUMA_AllocHook,( int32_t allocType, Handle userData, Word32 size, Word32 prevSize,
		 int32_t blockType, int32_t requestNumber, PWord8 filename, int32_t lineNumber));
DEC_FUNC(void,	 DPUMA_ConsoleClear,(int32_t fromline));
DEC_FUNC(int32_t,  DPUMA_ConsoleGetCurLine,());

DEC_FUNC(Bool32, DPUMA_SetFileName,(Handle wnd, const char * lpName));
DEC_FUNC(Handle,  DPUMA_FOpen,(const char * lpName,const char * lpMode));
DEC_FUNC(void,  DPUMA_FClose,(Handle hFile));

DEC_VARGP(int32_t,  DPUMA_FPrintf1024,Handle hFile);

DEC_FUNC(int32_t,  DPUMA_FPuts,(Handle hFile,const char * lpString));







#undef DEC_FUNC

#if !defined(TA_LEFT)
	/* Text Alignment Options form WINGDI.h */
	#define TA_LEFT                      0
	#define TA_RIGHT                     2
	#define TA_CENTER                    6
	#define TA_TOP                       0
	#define TA_BOTTOM                    8
	#define TA_BASELINE                  24
	#define TA_NOUPDATECP                0
	#define TA_UPDATECP                  1
#endif
#if !defined(ANSI_CHARSET)
/* charset form WINGDI.h */
	#define ANSI_CHARSET            0
	#define DEFAULT_CHARSET         1
	#define SYMBOL_CHARSET          2
	#define SHIFTJIS_CHARSET        128
	#define HANGEUL_CHARSET         129
	#define HANGUL_CHARSET          129
	#define GB2312_CHARSET          134
	#define CHINESEBIG5_CHARSET     136
	#define OEM_CHARSET             255

	#define JOHAB_CHARSET           130
	#define HEBREW_CHARSET          177
	#define ARABIC_CHARSET          178
	#define GREEK_CHARSET           161
	#define TURKISH_CHARSET         162
	#define VIETNAMESE_CHARSET      163
	#define THAI_CHARSET            222
	#define EASTEUROPE_CHARSET      238
	#define RUSSIAN_CHARSET         204

	#define MAC_CHARSET             77
	#define BALTIC_CHARSET          186
#endif

#if !defined(VK_RETURN) // Если не определен WINUSER.H

	#define WM_KEYFIRST                     0x0100
	#define WM_KEYDOWN                      0x0100
	#define WM_KEYUP                        0x0101
	#define WM_CHAR                         0x0102
	#define WM_DEADCHAR                     0x0103
	#define WM_SYSKEYDOWN                   0x0104
	#define WM_SYSKEYUP                     0x0105
	#define WM_SYSCHAR                      0x0106
	#define WM_SYSDEADCHAR                  0x0107
	#define WM_KEYLAST                      0x0108


	#define VK_LBUTTON        0x01
	#define VK_RBUTTON        0x02
	#define VK_CANCEL         0x03
	#define VK_MBUTTON        0x04    /* NOT contiguous with L & RBUTTON */

	#define VK_BACK           0x08
	#define VK_TAB            0x09

	#define VK_CLEAR          0x0C
	#define VK_RETURN         0x0D

	#define VK_SHIFT          0x10
	#define VK_CONTROL        0x11
	#define VK_MENU           0x12
	#define VK_PAUSE          0x13
	#define VK_CAPITAL        0x14


	#define VK_ESCAPE         0x1B

	#define VK_SPACE          0x20
	#define VK_PRIOR          0x21
	#define VK_NEXT           0x22
	#define VK_END            0x23
	#define VK_HOME           0x24
	#define VK_LEFT           0x25
	#define VK_UP             0x26
	#define VK_RIGHT          0x27
	#define VK_DOWN           0x28
	#define VK_SELECT         0x29
	#define VK_PRINT          0x2A
	#define VK_EXECUTE        0x2B
	#define VK_SNAPSHOT       0x2C
	#define VK_INSERT         0x2D
	#define VK_DELETE         0x2E
	#define VK_HELP           0x2F

	/* VK_0 thru VK_9 are the same as ASCII '0' thru '9' (0x30 - 0x39) */
	/* VK_A thru VK_Z are the same as ASCII 'A' thru 'Z' (0x41 - 0x5A) */

	#define VK_LWIN           0x5B
	#define VK_RWIN           0x5C
	#define VK_APPS           0x5D

	#define VK_NUMPAD0        0x60
	#define VK_NUMPAD1        0x61
	#define VK_NUMPAD2        0x62
	#define VK_NUMPAD3        0x63
	#define VK_NUMPAD4        0x64
	#define VK_NUMPAD5        0x65
	#define VK_NUMPAD6        0x66
	#define VK_NUMPAD7        0x67
	#define VK_NUMPAD8        0x68
	#define VK_NUMPAD9        0x69
	#define VK_MULTIPLY       0x6A
	#define VK_ADD            0x6B
	#define VK_SEPARATOR      0x6C
	#define VK_SUBTRACT       0x6D
	#define VK_DECIMAL        0x6E
	#define VK_DIVIDE         0x6F
	#define VK_F1             0x70
	#define VK_F2             0x71
	#define VK_F3             0x72
	#define VK_F4             0x73
	#define VK_F5             0x74
	#define VK_F6             0x75
	#define VK_F7             0x76
    #define VK_F8             0x77
	#define VK_F9             0x78
	#define VK_F10            0x79
	#define VK_F11            0x7A
	#define VK_F12            0x7B
	#define VK_F13            0x7C
	#define VK_F14            0x7D
	#define VK_F15            0x7E
	#define VK_F16            0x7F
	#define VK_F17            0x80
	#define VK_F18            0x81
	#define VK_F19            0x82
	#define VK_F20            0x83
	#define VK_F21            0x84
	#define VK_F22            0x85
	#define VK_F23            0x86
	#define VK_F24            0x87

	#define VK_NUMLOCK        0x90
	#define VK_SCROLL         0x91

	/*
	 * VK_L* & VK_R* - left and right Alt, Ctrl and Shift virtual keys.
	 * Used only as parameters to GetAsyncKeyState() and GetKeyState().
	 * No other API or message will distinguish left and right keys in this way.
	 */
	#define VK_LSHIFT         0xA0
	#define VK_RSHIFT         0xA1
	#define VK_LCONTROL       0xA2
	#define VK_RCONTROL       0xA3
	#define VK_LMENU          0xA4
	#define VK_RMENU          0xA5

	#if(WINVER >= 0x0400)
	#define VK_PROCESSKEY     0xE5
	#endif /* WINVER >= 0x0400 */

	#define VK_ATTN           0xF6
	#define VK_CRSEL          0xF7
	#define VK_EXSEL          0xF8
	#define VK_EREOF          0xF9
	#define VK_PLAY           0xFA
	#define VK_ZOOM           0xFB
	#define VK_NONAME         0xFC
	#define VK_PA1            0xFD
	#define VK_OEM_CLEAR      0xFE

	#define WM_MOUSEFIRST                   0x0200
	#define WM_MOUSEMOVE                    0x0200
	#define WM_LBUTTONDOWN                  0x0201
	#define WM_LBUTTONUP                    0x0202
	#define WM_LBUTTONDBLCLK                0x0203
	#define WM_RBUTTONDOWN                  0x0204
	#define WM_RBUTTONUP                    0x0205
	#define WM_RBUTTONDBLCLK                0x0206
	#define WM_MBUTTONDOWN                  0x0207
	#define WM_MBUTTONUP                    0x0208
	#define WM_MBUTTONDBLCLK                0x0209

    #define WM_DDE_FIRST	    0x03E0
    #define WM_DDE_INITIATE     (WM_DDE_FIRST)
    #define WM_DDE_TERMINATE    (WM_DDE_FIRST+1)

	#if (_WIN32_WINNT >= 0x0400) || (_WIN32_WINDOWS > 0x0400)
	#define WM_MOUSEWHEEL                   0x020A
	#define WM_MOUSELAST                    0x020A
	#else
	#define WM_MOUSELAST                    0x0209
	#endif /* if (_WIN32_WINNT < 0x0400) */

#endif
//




////////////////////////////////////

#pragma pack (pop)

#ifdef __cplusplus
      }
#endif


#endif
