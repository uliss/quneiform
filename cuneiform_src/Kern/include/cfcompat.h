/*
 Copyright (c) 2008, Jussi Pakkanen

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

/* Definitions needed for platform portability. */

#ifndef CFCOMPAT_H_
#define CFCOMPAT_H_

#include "globus.h"
#include <stdio.h>

#ifdef __CFCOMPAT__
#define CFCOMPAT_FUNC  FUN_EXPO
#else
#define CFCOMPAT_FUNC  FUN_IMPO
#endif

#if WIN32

#include<windows.h>

#else // WIN32
#include<wchar.h>
#include "compat_defs.h"

CFCOMPAT_FUNC(int) LoadString(HINSTANCE hInstance, uint uID, char* lpBuffer, int nBufferMax);

CFCOMPAT_FUNC(int) CreateDirectory(const char *dir, void *dummy);
CFCOMPAT_FUNC(uint32_t) GetTempPath(uint32_t nBufferLength, char* lpBuffer);
CFCOMPAT_FUNC(int) RemoveDirectory(const char *d);
CFCOMPAT_FUNC(void*) GlobalAlloc(uint uFlags, int dwBytes);
CFCOMPAT_FUNC(HGLOBAL) GlobalFree(void *f);
CFCOMPAT_FUNC(void*) GlobalReAlloc(void* hMem, int dwBytes, uint uFlags);
CFCOMPAT_FUNC(int) GetTempFileName(const char * lpPathName, const char * lpPrefixString,
		uint uUnique, char* lpTempFileName);
CFCOMPAT_FUNC(int) GetLastError();

CFCOMPAT_FUNC(uint32_t) GetModuleFileName(HMODULE hModule, char* lpFilename, uint32_t nSize);
CFCOMPAT_FUNC(Bool) CloseHandle(HANDLE hObject);
CFCOMPAT_FUNC(HANDLE) CreateFile(const char * lpFileName, uint32_t dwDesiredAccess,
		uint32_t dwShareMode, void* lpSecurityAttributes,
		uint32_t dwCreationDisposition, uint32_t dwFlagsAndAttributes, HANDLE hTemplateFile);

CFCOMPAT_FUNC(HWND) FindWindow(const char * lpClassName, const char * lpWindowName);
CFCOMPAT_FUNC(uint) RegisterWindowMessage(const char * lpString);

CFCOMPAT_FUNC(int) _findclose(long handle);
CFCOMPAT_FUNC(long) _findfirst(const char *filespec, struct _finddata_t *fileinfo);
CFCOMPAT_FUNC(int) _findnext(long handle, struct _finddata_t *fileinfo);
CFCOMPAT_FUNC(long) _tell(int handle);

CFCOMPAT_FUNC(Bool) GetComputerName(char* lpBuffer, long unsigned int *lpnSize);

CFCOMPAT_FUNC(int32_t) RegOpenKeyEx(HKEY hKey, const char * lpSubKey, uint32_t ulOptions,
		REGSAM samDesired, PHKEY phkResult);
CFCOMPAT_FUNC(int32_t) RegQueryValueEx(HKEY hKey, const char * lpValueName,
		LPDWORD lpReserved, LPDWORD lpType, LPBYTE lpData,
		LPDWORD lpcbData);

CFCOMPAT_FUNC(Bool) GetClientRect(HWND hWnd, LPRECT lpRect);
CFCOMPAT_FUNC(Bool) WritePrivateProfileString(const char * lpAppName,
		const char * lpKeyName, const char * lpString, const char * lpFileName);
CFCOMPAT_FUNC(uint32_t) GetPrivateProfileString(const char * lpAppName, const char * lpKeyName,
		const char * lpDefault, char* lpReturnedString, uint32_t nSize, const char * lpFileName);
CFCOMPAT_FUNC(uint) GetPrivateProfileInt(const char * lpAppName,
		const char * lpKeyName, int16_t nDefault, const char * lpFileName);

CFCOMPAT_FUNC(int) WideCharToMultiByte(uint CodePage, uint32_t dwFlags, const wchar_t *lpWideCharStr,
		int cchWideChar, char* lpMultiByteStr, int cbMultiByte,
		const char * lpDefaultChar, pBool lpUsedDefaultChar);

CFCOMPAT_FUNC(Bool) ShowWindow(HWND hWnd, int nCmdShow);

CFCOMPAT_FUNC(long) _filelength(int fd);
CFCOMPAT_FUNC(long) _msize(void *memblock);
CFCOMPAT_FUNC(int) _access(const char *filename, int mode);
CFCOMPAT_FUNC(Bool) SetWindowText(HWND hWnd,const char * lpString);
CFCOMPAT_FUNC(int) ReleaseDC(HWND hWnd, HDC hDC);
CFCOMPAT_FUNC(Bool) IsIconic(HWND hWnd);
CFCOMPAT_FUNC(HDC) GetDC(HWND hWnd);
CFCOMPAT_FUNC(Bool) EndPaint(HWND hWnd, ...);
CFCOMPAT_FUNC(HDC) BeginPaint(HWND hwnd,...);
CFCOMPAT_FUNC(LRESULT) SendMessage(HWND hWnd, uint Msg, WPARAM wParam, LPARAM lParam);
#ifndef __CYGWIN__
CFCOMPAT_FUNC(void) strlwr(char *foo);
#endif
CFCOMPAT_FUNC(HWND) CreateWindow(
		const char * lpClassName,
		const char * lpWindowName,
		uint32_t dwStyle,
		int x,
		int y,
		int nWidth,
		int nHeight,
		HWND hWndParent,
		HMENU hMenu,
		HINSTANCE hInstance,
		pvoid lpParam
);

CFCOMPAT_FUNC(HGDIOBJ) SelectObject(HDC hdc, HGDIOBJ hgdiobj);
CFCOMPAT_FUNC(char*) lstrcat(char* lpString1, char* lpString2);
CFCOMPAT_FUNC(int) lstrlen(const char * lpString);
CFCOMPAT_FUNC(int) lstrcmp(const char * lpString1, const char * lpString2);
CFCOMPAT_FUNC(char*) lstrcpy(char* lpString1, const char * lpString2);
CFCOMPAT_FUNC(int) wsprintf(char* lpOut, const char * lpFmt, ...);
CFCOMPAT_FUNC(int) lstrcmpi(const char * lpString1, const char * lpString2);

CFCOMPAT_FUNC(Bool) DeleteObject(HGDIOBJ hObject);

#define GetGValue(rgb) ((uchar) (((uint16_t) (rgb)) >> 8))
#define GetBValue(rgb) ((uchar) ((rgb) >> 16))
#define GetRValue(rgb) ((uchar) (rgb))

CFCOMPAT_FUNC(HWND) GetFocus();
CFCOMPAT_FUNC(int) MessageBox(HWND hWnd, const char * lpText, const char * lpCaption, uint uType);

CFCOMPAT_FUNC(int) WINAPI GlobalSize(HGLOBAL hMem);
CFCOMPAT_FUNC(pvoid) GlobalLock(HGLOBAL hMem);
CFCOMPAT_FUNC(Bool) GlobalUnlock(HGLOBAL hMem);
CFCOMPAT_FUNC(Bool) IsBadWritePtr(pvoid lp, int ucb);
CFCOMPAT_FUNC(void) OutputDebugString(const char * lpOutputString);
CFCOMPAT_FUNC(Bool) SetRect(LPRECT lprc, int xLeft, int yTop,
		int xRight, int yBottom);
CFCOMPAT_FUNC(Bool) PtInRect(const RECT *lprc, POINT pt);
CFCOMPAT_FUNC(Bool) IntersectRect(LPRECT lprcDst, const RECT *lprcSrc1, const RECT *lprcSrc2);
CFCOMPAT_FUNC(Bool) UnionRect(LPRECT lprcDst, const RECT *lprcSrc1,const RECT *lprcSrc2);

CFCOMPAT_FUNC(HWND) GetActiveWindow();
CFCOMPAT_FUNC(HFONT) CreateFont(int nHeight, int nWidth, int nEscapement,
		int nOrientation, int fnWeight, uint32_t fdwItalic, uint32_t fdwUnderline,
		uint32_t fdwStrikeOut, uint32_t fdwCharSet, uint32_t fdwOutputPrecision,
		uint32_t fdwClipPrecision, uint32_t fdwQuality, uint32_t fdwPitchAndFamily,
		const char * lpszFace);
CFCOMPAT_FUNC(Bool) GetTextExtentPoint32(HDC hdc, const char * lpString, int c, LPSIZE lpSize);
CFCOMPAT_FUNC(Bool) EnumWindows(WNDENUMPROC lpEnumFunc, LPARAM lParam);
CFCOMPAT_FUNC(int) GetWindowText(HWND hWnd, char* lpString, int nMaxCount);

CFCOMPAT_FUNC(HMODULE) LoadLibrary(const char * lpFileName);
CFCOMPAT_FUNC(Bool) FreeLibrary(HMODULE hModule);
CFCOMPAT_FUNC(void*) GetProcAddress(HMODULE hModule, const char * lpProcName);

CFCOMPAT_FUNC(HGDIOBJ) GetStockObject(int fnObject);
CFCOMPAT_FUNC(Bool) IsWindowVisible(HWND hWnd);
CFCOMPAT_FUNC(LRESULT) DefWindowProc(HWND hWnd, uint Msg, WPARAM wParam,
		LPARAM lParam);
CFCOMPAT_FUNC(int32_t) GetWindowLong(HWND hWnd, int nIndex);
CFCOMPAT_FUNC(Bool) RegisterClass(const WNDCLASS *lpWndClass);
CFCOMPAT_FUNC(HMODULE) GetModuleHandle(const char * lpModuleName);
CFCOMPAT_FUNC(HICON) LoadIcon(HINSTANCE hInstance, const char * lpIconName);

CFCOMPAT_FUNC(long) filelength(int fd);
CFCOMPAT_FUNC(int) LoadCursor(HINSTANCE hInstance, const char * lpCursorName);
CFCOMPAT_FUNC(Bool) Rectangle(HDC hdc,
		int nLeftRect, int nTopRect, int nRightRect, int nBottomRect);
CFCOMPAT_FUNC(char*) _strupr(char*s);

#endif /* not WIN32 */

/* These are not windows compat functions, but general helper functions.
 * I had no other place to put them.
 */

#ifdef _MSC_VER
#define strcasecmp(a, b) _stricmp(a, b)
#endif

#ifdef __cplusplus
extern "C" {
#endif

CFCOMPAT_FUNC(int)
open_data_file(const char *basename, int mode);

CFCOMPAT_FUNC(int)
data_file_exists(const char *basename);

CFCOMPAT_FUNC(void) split_path(const char *fname,
		char *file_path,
		char *basename,
		char *ext);

CFCOMPAT_FUNC(void) make_path(char *opath,
		const char *dir,
		const char *basename,
		const char *ext);

CFCOMPAT_FUNC(void) winpath_to_internal(char *p);

CFCOMPAT_FUNC(unsigned int) curr_dir(unsigned int bsize, char* buf);

CFCOMPAT_FUNC(FILE*) create_temp_file(void);

#ifdef __cplusplus
}
#endif

#endif
