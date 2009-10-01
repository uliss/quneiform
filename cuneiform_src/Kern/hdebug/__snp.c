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

/*#include <windows.h>*/
/*#include <crtdbg.h>*/
#include <stdio.h>
#include <stdarg.h>
#include <time.h>

#define __SNAP_C

#include "dpuma.h"

#include "compat_defs.h"

#ifdef __cplusplus
extern "C" {
#endif
	HINSTANCE hDPuma = NULL;

	static FNDPUMA_Init Init = NULL;
	static FNDPUMA_Done Done = NULL;
	static FNDPUMA_SetImportData SetImportData = NULL;
	static FNDPUMA_GetExportData GetExportData = NULL;

	//static FNDPUMA_CreateSnap,
	//static FNDPUMA_DestroySnap,
	static FNDPUMA_CreateSnap CreateSnap = NULL;
	static FNDPUMA_DestroySnap DestroySnap= NULL;
	static FNDPUMA_Console Console = NULL;
	static FNDPUMA_StatusLine StatusLine = NULL;
	static FNDPUMA_Stop Stop = NULL;
	static FNDPUMA_IsActive IsActive = NULL;
	static FNDPUMA_Registry Registry = NULL;
	static FNDPUMA_Skip Skip = NULL;
	static FNDPUMA_StartLoop StartLoop = NULL;
	static FNDPUMA_LoopNext LoopNext = NULL;
	static FNDPUMA_CreateWindow fCreateWindow = NULL;
	static FNDPUMA_MessageBoxOk MessageBoxOk = NULL;
	static FNDPUMA_MessageBoxYesNo MessageBoxYesNo = NULL;
	static FNDPUMA_WaitUserInput WaitUserInput = NULL;
	static FNDPUMA_DrawLine DrawLine = NULL;
	static FNDPUMA_DrawRect DrawRect = NULL;
	static FNDPUMA_DrawLineTip DrawLineTip = NULL;
	static FNDPUMA_DrawRectTip DrawRectTip = NULL;
	static FNDPUMA_DeleteRects DeleteRects = NULL;
	static FNDPUMA_DeleteLines DeleteLines = NULL;
	static FNDPUMA_GetUserPoint GetUserPoint = NULL;
	static FNDPUMA_GetUserRect GetUserRect = NULL;
	static FNDPUMA_UpdateView UpdateView = NULL;
	static FNDPUMA_DrawRaster DrawRaster = NULL;
	static FNDPUMA_RasterText RasterText = NULL;
	static FNDPUMA_ZoomToRect ZoomToRect = NULL;
	static FNDPUMA_RasterHeader RasterHeader = NULL;
	static FNDPUMA_DrawFocusRect fnDrawFocusRect = NULL;
	static FNDPUMA_RegVariable RegVariable = NULL;
	static FNDPUMA_UnregVariable UnregVariable = NULL;
	static FNDPUMA_GetDIBptr GetDIBptr = NULL;
	static FNDPUMA_DrawString DrawString = NULL;
	static FNDPUMA_DeleteStrings DeleteStrings = NULL;
	static FNDPUMA_SetCallbackWindowProc SetCallbackWindowProc = NULL;
	static FNDPUMA_DeviceToImage DeviceToImage = NULL;
	static FNDPUMA_ImageToDevice ImageToDevice = NULL;
	static FNDPUMA_SetCallbackMainFrameWindowProc SetCallbackMainFrameWindowProc = NULL;
	static FNDPUMA_DestroyWindow fDestroyWindow = NULL;
	static FNDPUMA_SendWindow SendWindow = NULL;
	static FNDPUMA_SendMainWnd SendMainWnd = NULL;
	static FNDPUMA_CSTR_Monitor cstr_Monitor = NULL;
	static FNDPUMA_CSTR_GetPosition cstr_GetPosition = NULL;
	static FNDPUMA_CSTR_SetPosition cstr_SetPosition = NULL;
	static FNDPUMA_DestroyRasterWnd DestroyRasterWnd = NULL;
	static FNDPUMA_CSTR_GetLength cstr_GetLength = NULL;
	static FNDPUMA_ShowCutPoint ShowCutPoint = NULL;
	static FNDPUMA_GetRasterPixel GetRasterPixel = NULL;
	static FNDPUMA_CSTR_Update cstr_Update = NULL;
	static FNDPUMA_SkipEx SkipEx = NULL;
	static FNDPUMA_OpenFile fOpenFile = NULL;
	static FNDPUMA_GetFileName fGetFileName = NULL;
	static FNDPUMA_HandLayout fHandLayout = NULL;
	static FNDPUMA_LockImage fLockImage = NULL;
	static FNDPUMA_RegistryHelp RegistryHelp = NULL;
	static FNDPUMA_SaveFile fSaveFile = NULL;
	static FNDPUMA_ProgressStart fProgressStart = NULL;
	static FNDPUMA_ProgressFinish fProgressFinish = NULL;
	static FNDPUMA_ProgressStep fProgressStep = NULL;
	static FNDPUMA_SetConsoleProperty fSetConsoleProperty = NULL;
	static FNDPUMA_CreateHistogramm fCreateHistogramm = NULL;
	static FNDPUMA_AddPointToHistogramm fAddPointToHistogramm = NULL;
	static FNDPUMA_DoHistogramm fDoHistogramm = NULL;
	static FNDPUMA_DestroyHistogramm fDestroyHistogramm = NULL;
	static FNDPUMA_GetWindowHandle fGetWindowHandle = NULL;
	static FNDPUMA_GetPrevSkipOwner fGetPrevSkipOwner = NULL;
	static FNDPUMA_AllocHook fAllocHook = NULL;
	static _CRT_ALLOC_HOOK PrevAllocHook = NULL;
	static FNDPUMA_ConsoleClear fConsoleClear=NULL;
	static FNDPUMA_ConsoleGetCurLine fConsoleGetCurLine=NULL;
	static FNDPUMA_SetFileName fSetFileName = NULL;
	static FNDPUMA_FOpen fFOpen = NULL;
	static FNDPUMA_FClose fFClose=NULL;
	static FNDPUMA_FPrintf1024 fFPrintf1024=NULL;
	static FNDPUMA_FPuts fFPuts = NULL;

	static Handle hWriteFile = NULL;
	//////////////////////////////////////////////
	static int __DPUMA__AllocHook__( int allocType, void *userData, size_t size, int blockType,
			long requestNumber, const unsigned char *filename, int lineNumber)
	{
		uint32_t prevSize = 0;
		int rc = PrevAllocHook(allocType,userData,size,blockType,
				requestNumber, filename, lineNumber);
		if(fAllocHook && rc > 0) // Если rc меньше нуля, тогда мы уже работали
		{
#ifdef _DEBUG
			if(userData)
			prevSize = _msize_dbg(userData,blockType);
#endif
			if(LDPUMA_IsActive())
			rc = fAllocHook(allocType,userData,size,prevSize,blockType,
					requestNumber,(puchar)filename, lineNumber);
		}
		return rc;
	}

	//////////////////////////////////////////////
	Bool32 LDPUMA_Init(uint16_t wHightCode, Handle hStorage)
	{
		Bool32 rc = FALSE;
#ifdef _DEBUG
		_CrtDbgReport( _CRT_WARN,NULL,__LINE__,__FILE__,
				"LDPUMA_Init(%i,%x)\n",wHightCode, hStorage);
#endif
		hDPuma = LoadLibrary("DPUMA.DLL");
		if(hDPuma)
		{
			Init = (FNDPUMA_Init)GetProcAddress(hDPuma,"DPUMA_Init");
			Done = (FNDPUMA_Done)GetProcAddress(hDPuma,"DPUMA_Done");
			GetExportData = (FNDPUMA_GetExportData)GetProcAddress(hDPuma,"DPUMA_GetExportData");
			SetImportData = (FNDPUMA_SetImportData)GetProcAddress(hDPuma,"DPUMA_SetImportData"); //tanya
		}
		if(Init && GetExportData)
		{
			if(GetExportData(DPUMA_FNDPUMA_Console,&Console)&&
					GetExportData(DPUMA_FNDPUMA_CreateSnap,&CreateSnap)&&
					GetExportData(DPUMA_FNDPUMA_DestroySnap,&DestroySnap)&&
					GetExportData(DPUMA_FNDPUMA_Stop,&Stop)&&
					GetExportData(DPUMA_FNDPUMA_IsActive,&IsActive)&&
					GetExportData(DPUMA_FNDPUMA_StatusLine,&StatusLine)&&
					GetExportData(DPUMA_FNDPUMA_Registry,&Registry)&&
					GetExportData(DPUMA_FNDPUMA_Skip,&Skip)&&
					GetExportData(DPUMA_FNDPUMA_StartLoop,&StartLoop)&&
					GetExportData(DPUMA_FNDPUMA_LoopNext,&LoopNext)&&
					GetExportData(DPUMA_FNDPUMA_CreateWindow,&fCreateWindow)&&
					GetExportData(DPUMA_FNDPUMA_MessageBoxOk,&MessageBoxOk)&&
					GetExportData(DPUMA_FNDPUMA_MessageBoxYesNo,&MessageBoxYesNo)&&
					GetExportData(DPUMA_FNDPUMA_WaitUserInput,&WaitUserInput)&&
					GetExportData(DPUMA_FNDPUMA_DrawLine,&DrawLine)&&
					GetExportData(DPUMA_FNDPUMA_DrawRect,&DrawRect)&&
					GetExportData(DPUMA_FNDPUMA_DrawLineTip,&DrawLineTip)&&
					GetExportData(DPUMA_FNDPUMA_DrawRectTip,&DrawRectTip)&&
					GetExportData(DPUMA_FNDPUMA_DeleteRects,&DeleteRects)&&
					GetExportData(DPUMA_FNDPUMA_DeleteLines,&DeleteLines)&&
					GetExportData(DPUMA_FNDPUMA_GetUserPoint,&GetUserPoint)&&
					GetExportData(DPUMA_FNDPUMA_GetUserRect,&GetUserRect)&&
					GetExportData(DPUMA_FNDPUMA_UpdateView,&UpdateView)&&
					GetExportData(DPUMA_FNDPUMA_DrawRaster,&DrawRaster)&&
					GetExportData(DPUMA_FNDPUMA_ZoomToRect,&ZoomToRect)&&
					GetExportData(DPUMA_FNDPUMA_RasterHeader,&RasterHeader)&&
					GetExportData(DPUMA_FNDPUMA_DrawFocusRect,&fnDrawFocusRect)&&
					GetExportData(DPUMA_FNDPUMA_RegVariable,&RegVariable)&&
					GetExportData(DPUMA_FNDPUMA_UnregVariable,&UnregVariable)&&
					GetExportData(DPUMA_FNDPUMA_GetDIBptr,&GetDIBptr)&&
					GetExportData(DPUMA_FNDPUMA_DrawString,&DrawString)&&
					GetExportData(DPUMA_FNDPUMA_DeleteStrings,&DeleteStrings)&&
					GetExportData(DPUMA_FNDPUMA_SetCallbackWindowProc,&SetCallbackWindowProc)&&
					GetExportData(DPUMA_FNDPUMA_DeviceToImage,&DeviceToImage)&&
					GetExportData(DPUMA_FNDPUMA_ImageToDevice,&ImageToDevice)&&
					GetExportData(DPUMA_FNDPUMA_SetCallbackMainFrameWindowProc,&SetCallbackMainFrameWindowProc)&&
					GetExportData(DPUMA_FNDPUMA_DestroyWindow,&fDestroyWindow)&&
					GetExportData(DPUMA_FNDPUMA_SendWindow,&SendWindow)&&
					GetExportData(DPUMA_FNDPUMA_SendMainWnd,&SendMainWnd)&&
					GetExportData(DPUMA_FNDPUMA_CSTR_Monitor,&cstr_Monitor)&&
					GetExportData(DPUMA_FNDPUMA_CSTR_GetPosition,&cstr_GetPosition)&&
					GetExportData(DPUMA_FNDPUMA_CSTR_SetPosition,&cstr_SetPosition)&&
					GetExportData(DPUMA_FNDPUMA_DestroyRasterWnd,&DestroyRasterWnd)&&
					GetExportData(DPUMA_FNDPUMA_CSTR_GetLength,&cstr_GetLength)&&
					GetExportData(DPUMA_FNDPUMA_ShowCutPoint,&ShowCutPoint)&&
					GetExportData(DPUMA_FNDPUMA_GetRasterPixel,&GetRasterPixel)&&
					GetExportData(DPUMA_FNDPUMA_CSTR_Update,&cstr_Update)&&
					GetExportData(DPUMA_FNDPUMA_SkipEx,&SkipEx)&&
					GetExportData(DPUMA_FNDPUMA_OpenFile,&fOpenFile)&&
					GetExportData(DPUMA_FNDPUMA_GetFileName,&fGetFileName)&&
					GetExportData(DPUMA_FNDPUMA_HandLayout,&fHandLayout)&&
					GetExportData(DPUMA_FNDPUMA_LockImage,&fLockImage)&&
					GetExportData(DPUMA_FNDPUMA_RegistryHelp,&RegistryHelp)&&
					GetExportData(DPUMA_FNDPUMA_SaveFile,&fSaveFile)&&
					GetExportData(DPUMA_FNDPUMA_ProgressStart, &fProgressStart )&&
					GetExportData(DPUMA_FNDPUMA_ProgressFinish,&fProgressFinish)&&
					GetExportData(DPUMA_FNDPUMA_ProgressStep, &fProgressStep )&&
					GetExportData(DPUMA_FNDPUMA_SetConsoleProperty, &fSetConsoleProperty )&&
					GetExportData(DPUMA_FNDPUMA_CreateHistogramm, &fCreateHistogramm )&&
					GetExportData(DPUMA_FNDPUMA_AddPointToHistogramm, &fAddPointToHistogramm )&&
					GetExportData(DPUMA_FNDPUMA_DoHistogramm, &fDoHistogramm )&&
					GetExportData(DPUMA_FNDPUMA_DestroyHistogramm, &fDestroyHistogramm )&&
					GetExportData(DPUMA_FNDPUMA_GetWindowHandle,&fGetWindowHandle)&&
					GetExportData(DPUMA_FNDPUMA_GetPrevSkipOwner,&fGetPrevSkipOwner)&&
					GetExportData(DPUMA_FNDPUMA_AllocHook,&fAllocHook)&&
					GetExportData(DPUMA_FNDPUMA_ConsoleClear,&fConsoleClear)&&
					GetExportData(DPUMA_FNDPUMA_ConsoleGetCurLine,&fConsoleGetCurLine)&&
					GetExportData(DPUMA_FNDPUMA_SetFileName,&fSetFileName )&&
					GetExportData(DPUMA_FNDPUMA_FOpen,&fFOpen )&&
					GetExportData(DPUMA_FNDPUMA_FClose,&fFClose)&&
					GetExportData(DPUMA_FNDPUMA_FPrintf1024,&fFPrintf1024)&&
					GetExportData(DPUMA_FNDPUMA_FPuts,&fFPuts)&&
					GetExportData(DPUMA_FNDPUMA_RasterText,&RasterText)&&
					SetImportData != NULL/*tanya*/
			)
			{
				rc = Init(wHightCode,hStorage);
				if(rc)
				{
#ifdef _DEBUG
					// На случай, если вызывается не один раз.
					_CRT_ALLOC_HOOK pHook = _CrtSetAllocHook(__DPUMA__AllocHook__);
					if(__DPUMA__AllocHook__ != pHook)
					PrevAllocHook = pHook;
#endif
					fCreateWindow("Main",NULL);
					LDPUMA_Registry(&hWriteFile,"Выполнять отладочную запись в файл.",NULL);
					LDPUMA_RegistryHelp(hWriteFile,"Разрешает отладочную запись функциями DPUMA.",FALSE);
				}
			}
		}
		return rc;
	}
	//////////////////////////////////////////////
	Bool32 LDPUMA_Done()
	{
		Bool32 rc = FALSE;
#ifdef _DEBUG
		_CrtDbgReport( _CRT_WARN,NULL,__LINE__,__FILE__,
				"LDPUMA_Done()\n");
		if(PrevAllocHook)
		_CrtSetAllocHook(PrevAllocHook);
#endif

		if(Done)
		rc = Done();
		if(hDPuma)
		{
			FreeLibrary(hDPuma);
			hDPuma = NULL;
		}
		return rc;
	}
	//////////////////////////////////////////////
	Bool32 LDPUMA_SetImportData(uint32_t wType, void * pData)
	{
		if(SetImportData)
		return SetImportData(wType,pData);
		return FALSE;
	}
	//////////////////////////////////////////////
	Bool32 LDPUMA_GetExportData(uint32_t wType, void * pData)
	{
		if(GetExportData)
		return GetExportData(wType,pData);
		return FALSE;
	}
	//////////////////////////////////////////////
	void LDPUMA_DestroySnap()
	{
#ifdef _DEBUG
		_CrtDbgReport( _CRT_WARN,NULL,__LINE__,__FILE__,
				"LDPUMA_DestroySnap()\n");
#endif
		if(DestroySnap)
		DestroySnap();
	}
	//////////////////////////////////////////////
	uint32_t LDPUMA_CreateSnap()
	{
		uint32_t rc = 0;
#ifdef _DEBUG
		_CrtDbgReport( _CRT_WARN,NULL,__LINE__,__FILE__,
				"LDPUMA_CreateSnap()\n");
#endif
		if(CreateSnap)
		rc = CreateSnap();
		return rc;
	}
	//////////////////////////////////////////////
	void LDPUMA_Stop()
	{
		if(Stop)
		Stop();
	}
	//////////////////////////////////////////////
	Handle LDPUMA_CreateWindow(const char * lpName, void * lpDIB)
	{
		if(fCreateWindow)
		return fCreateWindow(lpName,lpDIB);
		return NULL;
	}
	//////////////////////////////////////////////
	Bool32 LDPUMA_IsActive(void)
	{
		return IsActive ? IsActive():FALSE;
	};
	//////////////////////////////////////////////
	void LDPUMA_DrawRect(Handle wnd,Rect16* rc, int32_t skew, uint32_t rgb_color,
			int16_t pen_width,uint32_t key)
	{
		if(DrawRect)
		DrawRect(wnd,rc,skew,rgb_color,(int16_t)pen_width,key);
	}
	//////////////////////////////////////////////
	void LDPUMA_DrawRectTip(Handle wnd,Rect16* rc, int32_t skew, uint32_t rgb_color,
			int16_t pen_width,uint32_t key,const char* pTip)
	{
		if(DrawRectTip)
		DrawRectTip(wnd,rc,skew,rgb_color,(int16_t)pen_width,key,pTip);
	}
	//////////////////////////////////////////////
	void LDPUMA_DeleteRects(Handle wnd, uint32_t key)
	{
		if(DeleteRects)
		DeleteRects(wnd,key);
	};
	//////////////////////////////////////////////
	void LDPUMA_DrawLine(Handle wnd,Point16* start, Point16* end, int32_t skew,
			uint32_t rgb_color,int16_t pen_width,uint32_t key )
	{
		if(DrawLine)
		DrawLine(wnd,start,end,skew,rgb_color,pen_width,key);
	};
	//////////////////////////////////////////////
	void LDPUMA_DrawLineTip(Handle wnd,Point16* start, Point16* end, int32_t skew,
			uint32_t rgb_color,int16_t pen_width,uint32_t key,const char* pTip)
	{
		if(DrawLineTip)
		DrawLineTip(wnd,start,end,skew,rgb_color,pen_width,key,pTip);
	};
	//////////////////////////////////////////////
	void LDPUMA_DeleteLines(Handle wnd,uint32_t key)
	{
		if(DeleteLines)
		DeleteLines(wnd,key);
	};
	//////////////////////////////////////////////
	void LDPUMA_UpdateView(Handle wnd)
	{
		if(UpdateView)
		UpdateView(wnd);
	}
	//////////////////////////////////////////////
	int32_t LDPUMA_Console(const char * message,...)
	{
#ifdef WIN32
		int rc = 0;
		if(Console)
		{
			va_list marker;
			va_start( marker, message);
			rc = Console(message,marker);
			va_end(marker);
		}
		return rc;
#else
#ifdef _DEBUG // console output only in debug mode
		va_list marker;
		va_start(marker, message);
		vprintf(message, marker);
		va_end(marker);
#endif // _DEBUG
		return 1;
#endif
	}
	int32_t LDPUMA_ConsoleN(const char * message,...)
	{
		int32_t rc = 0;
		if(Console)
		{
			va_list marker;
			va_start( marker, message);
			rc = Console(message,marker);
			va_end(marker);
			LDPUMA_Console("\n");
		}
		return rc;
	}
	//////////////////////////////////////////////
	void LDPUMA_StatusLine(const char * message,...)
	{
		if(StatusLine)
		{
			va_list marker;
			va_start( marker, message);
			StatusLine(message,marker);
			va_end(marker);
		}
	}
	//////////////////////////////////////////////
	void LDPUMA_MessageBoxOk( const char * message,...)
	{
		if(MessageBoxOk)
		{
			va_list marker;
			va_start( marker, message);
			MessageBoxOk(message,marker);
			va_end(marker);
		}
	};
	//////////////////////////////////////////////
	Bool16 LDPUMA_MessageBoxYesNo(const char * message,...)
	{
		Bool16 rc = FALSE;
		if(MessageBoxYesNo)
		{
			va_list marker;
			va_start( marker, message);
			rc = MessageBoxYesNo(message,marker);
			va_end(marker);
		}
		return rc;
	};
	//////////////////////////////////////////////
	Bool16 LDPUMA_GetUserRect(Handle wnd, Rect16* rect )
	{
		Bool16 rc = FALSE;
		if(GetUserRect)
		rc = GetUserRect(wnd,rect);
		return rc;
	};
	//////////////////////////////////////////////
	Bool16 LDPUMA_GetUserPoint(Handle wnd,Point16* pnt)
	{
		Bool16 rc = FALSE;
		if(GetUserPoint)
		rc = GetUserPoint(wnd,pnt);
		return rc;
	};
	//////////////////////////////////////////////
	uint32_t LDPUMA_WaitUserInput(Handle cur_node,Handle wnd)
	{
		uint32_t rc = 0;
		if(WaitUserInput)
		rc = WaitUserInput(cur_node,wnd);
		return rc;
	};
	//////////////////////////////////////////////
	void LDPUMA_Registry( Handle node, const char * name,Handle parent )
	{
		if(Registry)
		Registry(node,name,parent);
	};
	//////////////////////////////////////////////
	void LDPUMA_StartLoop( Handle node, uint32_t iter_total )
	{
		if(StartLoop)
		StartLoop(node,iter_total);
	};
	//////////////////////////////////////////////
	void LDPUMA_LoopNext( Handle node )
	{
		if(LoopNext)
		LoopNext(node);
	};
	//////////////////////////////////////////////
	Bool16 LDPUMA_Skip( Handle node )
	{
		return Skip ? Skip(node):TRUE;
	};
	//////////////////////////////////////////////
	void LDPUMA_DrawRaster(DPUMA_RecRaster * raster)
	{
		if(DrawRaster)
		DrawRaster((DPUMA_RecRaster *)raster);
	};
	//////////////////////////////////////////////
	void LDPUMA_RasterText(char * lpText)
	{
		if(RasterText)
		RasterText(lpText);
	};
	//////////////////////////////////////////////
	void LDPUMA_ZoomToRect(Handle wnd,Rect16 * lpRect)
	{
		if(ZoomToRect)
		ZoomToRect(wnd,lpRect);
	};
	//////////////////////////////////////////////
	void LDPUMA_RasterHeader(char * lpText,uint32_t num)
	{
		if(RasterHeader)
		RasterHeader(lpText,num);
	};
	//////////////////////////////////////////////
	void LDPUMA_DrawFocusRect(Handle wnd,Rect16* rc)
	{
		if(fnDrawFocusRect)
		fnDrawFocusRect(wnd,rc);
	};
	//////////////////////////////////////////////
	Bool32 LDPUMA_RegVariable(Handle owner, const char * lpText,void * lpData, const char * lpType)
	{
		Bool rc = FALSE;
		if(RegVariable)
		rc = RegVariable(owner,lpText,lpData,lpType);
		return rc;
	};

	void LDPUMA_UnregVariable(void * lpData)
	{
		if(UnregVariable)
		UnregVariable(lpData);
	};

	void * LDPUMA_GetDIBptr(Handle wnd)
	{
		if(GetDIBptr)
		return GetDIBptr(wnd);
		return NULL;
	};
	//////////////////////////////////////////////
	void LDPUMA_DrawString(Handle wnd,Point16* start, const char * string, int32_t align,
			uint32_t rgb_color,int16_t size,uint32_t key )
	{
		if(DrawString)
		DrawString(wnd,start,string,align,rgb_color,size,key);
	};
	//////////////////////////////////////////////
	void LDPUMA_DeleteStrings(Handle wnd,uint32_t key)
	{
		if(DeleteStrings)
		DeleteStrings(wnd,key);
	};
	//////////////////////////////////////////////
	DPUMA_Callback_WindowProc LDPUMA_SetCallbackWindowProc(Handle wnd, DPUMA_Callback_WindowProc func)
	{
		if(SetCallbackWindowProc)
		return SetCallbackWindowProc(wnd,func);
		return NULL;
	};
	//////////////////////////////////////////////
	void LDPUMA_DeviceToImage(Handle wnd, Point32 * p, uint32_t number)
	{
		if(DeviceToImage)
		DeviceToImage( wnd, p, number);
	};
	//////////////////////////////////////////////
	void LDPUMA_ImageToDevice(Handle wnd, Point32 * p, uint32_t number)
	{
		if(ImageToDevice)
		ImageToDevice( wnd, p, number);
	};
	//////////////////////////////////////////////
	DPUMA_Callback_WindowProc LDPUMA_SetCallbackMainFrameWindowProc(DPUMA_Callback_WindowProc func)
	{
		if(SetCallbackMainFrameWindowProc)
		return SetCallbackMainFrameWindowProc(func);
		return NULL;
	};
	//////////////////////////////////////////////
	void LDPUMA_DestroyWindow(Handle wnd)
	{
		if(fDestroyWindow)
		fDestroyWindow( wnd );
	};
	//////////////////////////////////////////////
	uint32_t LDPUMA_SendWindow(Handle wnd,uint32_t message, uint32_t wParam, uint32_t lParam)
	{
		uint32_t rc = 0;
		if(SendWindow)
		rc = SendWindow(wnd,message,wParam,lParam);
		return rc;
	};
	//////////////////////////////////////////////
	uint32_t LDPUMA_SendMainWnd(uint32_t message, uint32_t wParam, uint32_t lParam)
	{
		uint32_t rc = 0;
		if(SendMainWnd)
		rc = SendMainWnd(message,wParam,lParam);
		return rc;
	};
	//////////////////////////////////////////////
	uint32_t LDPUMA_CSTR_Monitor(Handle owner, uint32_t cstr_line, uint32_t pos,
			DPUMA_Callback_WindowProc lpproc)
	{
		uint32_t rc = 0;
		if(cstr_Monitor)
		rc = cstr_Monitor(owner,cstr_line,pos,lpproc);
		return rc;
	}
	//////////////////////////////////////////////
	uint32_t LDPUMA_CSTR_GetPosition(uint32_t * cstr_raster)
	{
		uint32_t rc = 0;
		if(cstr_GetPosition)
		cstr_GetPosition(cstr_raster);

		return rc;
	}
	//////////////////////////////////////////////
	void LDPUMA_CSTR_SetPosition(uint32_t pos)
	{
		if(cstr_SetPosition)
		cstr_SetPosition(pos);
	}
	//////////////////////////////////////////////
	void LDPUMA_DestroyRasterWnd()
	{
		if(DestroyRasterWnd)
		DestroyRasterWnd();
	}
	//////////////////////////////////////////////
	uint32_t LDPUMA_CSTR_GetLength()
	{
		uint32_t rc = 0;
		if(cstr_GetLength)
		rc = cstr_GetLength();
		return rc;
	}
	//////////////////////////////////////////////
	void LDPUMA_ShowCutPoint(uint32_t number, Point32 * lppoint)
	{
		if(ShowCutPoint)
		ShowCutPoint(number,lppoint);
	}
	//////////////////////////////////////////////
	Point32 LDPUMA_GetRasterPixel(Handle wnd, Point32 point )
	{
		Point32 rc = {-1,-1};
		if(GetRasterPixel)
		rc = GetRasterPixel(wnd,point);
		return rc;
	}
	//////////////////////////////////////////////
	void LDPUMA_CSTR_Update( )
	{
		if(cstr_Update)
		cstr_Update();
	}

	//////////////////////////////////////////////
	Bool32 LDPUMA_SkipEx(Handle owner, Bool32 bIter, Bool32 bParent, int32_t nSign )
	{
		Bool32 rc = TRUE;
		if(SkipEx)
		rc = SkipEx(owner, bIter, bParent, nSign);
		return rc;
	}
	//////////////////////////////////////////////
	Bool32 LDPUMA_OpenFile(Handle wnd, char * name )
	{
		Bool32 rc = FALSE;
		if(fOpenFile)
		rc = fOpenFile(wnd,name);
		return rc;
	}
	//////////////////////////////////////////////
	char * LDPUMA_GetFileName(Handle wnd)
	{
		char * rc = "";
		if(fGetFileName)
		rc = fGetFileName(wnd);
		return rc;
	}
	//////////////////////////////////////////////
	void * LDPUMA_HandLayout( void * lpDIB, uint32_t flags ,Point32 * p)
	{
		void * rc = NULL;
		if(fHandLayout)
		rc = fHandLayout(lpDIB,flags,p);
		return rc;
	}
	//////////////////////////////////////////////
	Bool32 LDPUMA_LockImage( Handle wnd, Bool32 bLock )
	{
		Bool32 rc = FALSE;

		if(fLockImage)
		rc = fLockImage(wnd,bLock);

		return rc;
	}
	//////////////////////////////////////////////
	void LDPUMA_RegistryHelp( Handle owner, const char * lpstrHelp,Bool32 bAppend )
	{
		if(RegistryHelp)
		RegistryHelp(owner,lpstrHelp,bAppend);
	}
	//////////////////////////////////////////////
	Bool32 LDPUMA_SaveFile( void * lpDIB,char * lpFileName,uint32_t nFormat )
	{
		Bool32 rc = FALSE;
		if(fSaveFile)
		rc = fSaveFile(lpDIB,lpFileName,nFormat);
		return rc;
	}
	//////////////////////////////////////////////
	void LDPUMA_ProgressStart( void )
	{
		if(fProgressStart)
		fProgressStart();
	}
	//////////////////////////////////////////////
	void LDPUMA_ProgressFinish( void )
	{
		if(fProgressFinish)
		fProgressFinish();
	}
	//////////////////////////////////////////////
	Bool32 LDPUMA_ProgressStep(uint32_t step, const char * name, uint32_t percent)
	{
		Bool32 rc = TRUE;
		if(fProgressStep)
		rc = fProgressStep(step,name,percent);
		return rc;
	}
	//////////////////////////////////////////////
	Bool32 LDPUMA_SetConsoleProperty(Bool32 bold,Bool32 italic,
			Bool32 strikeout,Bool32 underline,int32_t height, int32_t offset, uint32_t textcolor,
			int32_t charset, const char * name )
	{
		Bool32 rc = TRUE;

		if(fSetConsoleProperty)
		rc = fSetConsoleProperty(bold,italic,
				strikeout,underline,height, offset, textcolor,
				charset, name );

		return rc;
	}
	///////////////////////////////////////
	Handle LDPUMA_CreateHistogramm(const char * name, uint32_t size, int32_t * lpVal)
	{
		Handle rc = NULL;
		if(fCreateHistogramm)
		rc = fCreateHistogramm(name,size,lpVal);
		return rc;
	}
	//////////////////////////////////////////////
	void LDPUMA_AddPointToHistogramm(Handle hDlg, uint32_t point)
	{
		if(fAddPointToHistogramm)
		fAddPointToHistogramm(hDlg,point);
	}
	//////////////////////////////////////////////
	Bool32 LDPUMA_DoHistogramm(Handle hDlg, uint32_t * lpCurpos)
	{
		Bool32 rc = FALSE;
		if(fDoHistogramm)
		rc = fDoHistogramm(hDlg,lpCurpos);
		return rc;
	}
	//////////////////////////////////////////////
	void LDPUMA_DestroyHistogramm(Handle hDlg)
	{
		if(fDestroyHistogramm)
		fDestroyHistogramm(hDlg);
	}
	//////////////////////////////////////////////
	Handle LDPUMA_TimeStamp(const char * name,Handle hTimer)
	{
		time_t ltime = 0;
		int clock1 = 0;
		if(LDPUMA_IsActive())
		{
			time( &ltime );
			clock1=clock();
			if(name==NULL)
			name = "Time stamp";
			if(hTimer==NULL)
			LDPUMA_Console("%s : %s\n",name,asctime(localtime(&ltime)));
			else
			{
				int clockprev = (int)hTimer;
				LDPUMA_Console("%s : %i msec.\n",name,clock1-clockprev);
			}
		}
		return (Handle) clock1;
	}
	//////////////////////////////////////////////
	Handle LDPUMA_GetWindowHandle(const char * name)
	{
		Handle rc = NULL;
		if(fGetWindowHandle)
		rc = fGetWindowHandle(name);
		return rc;
	}
	//////////////////////////////////////////////
	Handle LDPUMA_GetPrevSkipOwner()
	{
		Handle rc = NULL;
		if(fGetPrevSkipOwner)
		rc = fGetPrevSkipOwner();
		return rc;
	}
	//////////////////////////////////////////////
	void LDPUMA_ConsoleClear(int32_t beforeline)
	{
		if(fConsoleClear)
		fConsoleClear(beforeline);
	}
	//////////////////////////////////////////////
	int32_t LDPUMA_ConsoleGetCurLine()
	{
		int32_t rc = -1;
		if(fConsoleGetCurLine)
		rc = fConsoleGetCurLine();
		return rc;
	}
	//////////////////////////////////////////////
	Bool32 LDPUMA_SetFileName(Handle wnd, const char * lpName)
	{
		Bool32 rc = FALSE;
		if(fSetFileName)
		rc = fSetFileName(wnd,lpName);
		return rc;
	}
	//////////////////////////////////////////////
	Handle LDPUMA_FOpen(const char * lpName,const char * lpMode)
	{
		Handle hFile = NULL;

		if(fFOpen && !LDPUMA_Skip(hWriteFile))
		hFile = fFOpen(lpName,lpMode);
		return hFile;
	}
	//////////////////////////////////////////////
	void LDPUMA_FClose(Handle hFile)
	{
		if(fFClose && hFile)
		fFClose(hFile);
	}
	//////////////////////////////////////////////
	int32_t LDPUMA_FPrintf1024(Handle hFile,const char * lpFormat,...)
	{
		int32_t rc = 0;
		if(fFPrintf1024 && hFile)
		{
			va_list marker;
			va_start( marker, lpFormat);
			rc = fFPrintf1024(hFile,lpFormat,marker);
			va_end(marker);
		}
		return rc;
	}
	//////////////////////////////////////////////
	int32_t LDPUMA_FPuts(Handle hFile,const char * lpString)
	{
		int32_t rc = 0;
		if(fFPuts && hFile)
		rc = fFPuts(hFile,lpString);
		return rc;
	}
	//////////////////////////////////////////////
#ifdef DPUMA_SNAP

#include "snptools.h"

	void SnpSetTools(__SnpToolBox* tools)
	{
		LDPUMA_Init(0,NULL);
	};
	void SnpDrawFocusRect(Rect16* rc)
	{
		LDPUMA_DrawFocusRect(NULL,rc);
	}
	Bool16 SnpGetUserLong(char * static_text,int32_t * result_long)
	{
		return LDPUMA_MessageBoxYesNo(static_text);
	};
	Bool16 SnpGetUserString (char * static_text,
			char * result_string, uint32_t result_string_length )
	{
		return LDPUMA_MessageBoxYesNo(static_text);
	};
	Bool16 SnpGetUserRect( Rect16* rect )
	{
		return LDPUMA_GetUserRect(NULL, rect );
	}
	Bool16 SnpGetUserPoint(Point16* pnt)
	{
		return LDPUMA_GetUserPoint(NULL,pnt);
	};
	uint32_t SnpWaitUserInput(SnpTreeNode* cur_node)
	{
		return LDPUMA_WaitUserInput(cur_node ? *(Handle *)cur_node:NULL,NULL);
	};
	void SnpAddNode( SnpTreeNode * node, const char * name,SnpTreeNode * parent )
	{
		LDPUMA_Registry( node, name,parent ? *(Handle *)parent:NULL );
	};
	void SnpStartLoop( SnpTreeNode* node, uint32_t iter_total )
	{
		LDPUMA_StartLoop( node ? *(Handle *)node:NULL, iter_total );
	};
	void SnpLoopNext( SnpTreeNode* node )
	{
		LDPUMA_LoopNext( node ? *(Handle *)node:NULL );
	};
	Bool16 SnpSkip( SnpTreeNode* node )
	{
		return LDPUMA_Skip( node ? *(Handle *)node:NULL);
	};
	void SnpDrawRaster(RecRaster * raster)
	{
		LDPUMA_DrawRaster((DPUMA_RecRaster *)raster);
	};
	void SnpRasterText(char * lpText)
	{
		LDPUMA_RasterText(lpText);
	};
	void SnpZoomToRect(Rect16 * lpRect)
	{
		LDPUMA_ZoomToRect(NULL,lpRect);
	};
	void SnpRasterHeader(char * lpText,uint32_t num)
	{
		LDPUMA_RasterHeader(lpText,num);
	};
	void SnpDrawLine(Point16* start, Point16* end, int32_t skew,
			uint32_t rgb_color, int16_t pen_width, Handle key )
	{
		LDPUMA_DrawLine(NULL,start,end,skew,rgb_color,pen_width,key );
	};
	void SnpHideLines(Handle key)
	{
		LDPUMA_DeleteLines(NULL,key);
	};
	void SnpUpdateViews(void)
	{
		LDPUMA_UpdateView(NULL);
	};
	int SnpLog(const char * message,...)
	{
		int rc = 0;
		if(Console)
		{
			va_list marker;
			va_start( marker, message);
			rc = Console(message,marker);
			va_end(marker);
		}
		return rc;
	}
	void SnpStatusLine(const char * message,...)
	{
		if(StatusLine)
		{
			va_list marker;
			va_start( marker, message);
			StatusLine(message,marker);
			va_end(marker);
		}
	}
	void SnpMessBoxOk( char * message )
	{
		LDPUMA_MessageBoxOk( message );
	}
	Bool16 SnpMessBoxYesNo( char * message)
	{
		return LDPUMA_MessageBoxYesNo( message);
	};
	Bool SnpIsActive(void)
	{
		return LDPUMA_IsActive();
	};
	void SnpDrawRect(Rect16* rc, int32_t skew, uint32_t rgb_color,
			int32_t pen_width,uint32_t key)
	{
		LDPUMA_DrawRect(NULL,rc,skew,rgb_color,(int16_t)pen_width,key);
	}
	void SnpHideRects(uint32_t key)
	{
		LDPUMA_DeleteRects(NULL,key);
	}
	uint32_t SnpSetZoneOn(Rect16* zone_rect,
			uint32_t rgb_color,char* status_line_comment,
			uint32_t users_zone_handle, FTOnMouseDown on_mouse_down )
	{	return 0;};
	void SnpSetZoneOff( uint32_t zone_handle ) {;};

	void SnpNotifyAppl(SnpTreeNode* cur_node) {;};
	// tree manipulation
	void __SnpIterParent( SnpTreeNode* node, Bool activate ) {;};
#else
	int SnpLog(const char * message,...)
	{
		return 0;
	}
#endif
	//////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////
#ifdef __cplusplus
}
#endif
