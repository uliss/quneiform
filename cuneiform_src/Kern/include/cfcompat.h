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

CFCOMPAT_FUNC(int) LoadString(HINSTANCE hInstance, UINT uID, LPTSTR lpBuffer, int nBufferMax);

CFCOMPAT_FUNC(int) CreateDirectory(const char *dir, void *dummy);
CFCOMPAT_FUNC(uint32_t) GetTempPath(uint32_t nBufferLength, LPTSTR lpBuffer);
CFCOMPAT_FUNC(int) RemoveDirectory(const char *d);
CFCOMPAT_FUNC(void*) GlobalAlloc(UINT uFlags, int dwBytes);
CFCOMPAT_FUNC(HGLOBAL) GlobalFree(void *f);
CFCOMPAT_FUNC(void*) GlobalReAlloc(void* hMem, int dwBytes, UINT uFlags);
CFCOMPAT_FUNC(int) GetTempFileName(LPCTSTR lpPathName, LPCTSTR lpPrefixString,
		UINT uUnique, LPTSTR lpTempFileName);
CFCOMPAT_FUNC(int) GetLastError();

CFCOMPAT_FUNC(uint32_t) GetModuleFileName(HMODULE hModule, LPTSTR lpFilename, uint32_t nSize);
CFCOMPAT_FUNC(Bool) CloseHandle(HANDLE hObject);
CFCOMPAT_FUNC(HANDLE) CreateFile(LPCTSTR lpFileName, uint32_t dwDesiredAccess,
		uint32_t dwShareMode, void* lpSecurityAttributes,
		uint32_t dwCreationDisposition, uint32_t dwFlagsAndAttributes, HANDLE hTemplateFile);

CFCOMPAT_FUNC(HWND) FindWindow(LPCTSTR lpClassName, LPCTSTR lpWindowName);
CFCOMPAT_FUNC(UINT) RegisterWindowMessage(LPCTSTR lpString);

CFCOMPAT_FUNC(int) _findclose(long handle);
CFCOMPAT_FUNC(long) _findfirst(const char *filespec, struct _finddata_t *fileinfo);
CFCOMPAT_FUNC(int) _findnext(long handle, struct _finddata_t *fileinfo);
CFCOMPAT_FUNC(long) _tell(int handle);

CFCOMPAT_FUNC(Bool) GetComputerName(LPTSTR lpBuffer, long unsigned int *lpnSize);

CFCOMPAT_FUNC(LONG) RegOpenKeyEx(HKEY hKey, LPCTSTR lpSubKey, uint32_t ulOptions,
		REGSAM samDesired, PHKEY phkResult);
CFCOMPAT_FUNC(LONG) RegQueryValueEx(HKEY hKey, LPCTSTR lpValueName,
		LPDWORD lpReserved, LPDWORD lpType, LPBYTE lpData,
		LPDWORD lpcbData);

CFCOMPAT_FUNC(Bool) GetClientRect(HWND hWnd, LPRECT lpRect);
CFCOMPAT_FUNC(Bool) WritePrivateProfileString(LPCTSTR lpAppName,
		LPCTSTR lpKeyName, LPCTSTR lpString, LPCTSTR lpFileName);
CFCOMPAT_FUNC(uint32_t) GetPrivateProfileString(LPCTSTR lpAppName, LPCTSTR lpKeyName,
		LPCTSTR lpDefault, LPTSTR lpReturnedString, uint32_t nSize, LPCTSTR lpFileName);
CFCOMPAT_FUNC(UINT) GetPrivateProfileInt(LPCTSTR lpAppName,
		LPCTSTR lpKeyName, INT nDefault, LPCTSTR lpFileName);

CFCOMPAT_FUNC(int) WideCharToMultiByte(UINT CodePage, uint32_t dwFlags, const wchar_t *lpWideCharStr,
		int cchWideChar, LPSTR lpMultiByteStr, int cbMultiByte,
		LPCSTR lpDefaultChar, LPBOOL lpUsedDefaultChar);

CFCOMPAT_FUNC(Bool) ShowWindow(HWND hWnd, int nCmdShow);

CFCOMPAT_FUNC(long) _filelength(int fd);
CFCOMPAT_FUNC(long) _msize(void *memblock);
CFCOMPAT_FUNC(int) _access(const char *filename, int mode);
CFCOMPAT_FUNC(Bool) SetWindowText(HWND hWnd,LPCTSTR lpString);
CFCOMPAT_FUNC(int) ReleaseDC(HWND hWnd, HDC hDC);
CFCOMPAT_FUNC(Bool) IsIconic(HWND hWnd);
CFCOMPAT_FUNC(HDC) GetDC(HWND hWnd);
CFCOMPAT_FUNC(Bool) EndPaint(HWND hWnd, ...);
CFCOMPAT_FUNC(HDC) BeginPaint(HWND hwnd,...);
CFCOMPAT_FUNC(LRESULT) SendMessage(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
#ifndef __CYGWIN__
CFCOMPAT_FUNC(void) strlwr(char *foo);
#endif
CFCOMPAT_FUNC(HWND) CreateWindow(
		LPCTSTR lpClassName,
		LPCTSTR lpWindowName,
		uint32_t dwStyle,
		int x,
		int y,
		int nWidth,
		int nHeight,
		HWND hWndParent,
		HMENU hMenu,
		HINSTANCE hInstance,
		LPVOID lpParam
);

CFCOMPAT_FUNC(HGDIOBJ) SelectObject(HDC hdc, HGDIOBJ hgdiobj);
CFCOMPAT_FUNC(LPTSTR) lstrcat(LPTSTR lpString1, LPTSTR lpString2);
CFCOMPAT_FUNC(int) lstrlen(LPCTSTR lpString);
CFCOMPAT_FUNC(int) lstrcmp(LPCTSTR lpString1, LPCTSTR lpString2);
CFCOMPAT_FUNC(LPTSTR) lstrcpy(LPTSTR lpString1, LPCTSTR lpString2);
CFCOMPAT_FUNC(int) wsprintf(LPTSTR lpOut, LPCTSTR lpFmt, ...);
CFCOMPAT_FUNC(int) lstrcmpi(LPCTSTR lpString1, LPCTSTR lpString2);

CFCOMPAT_FUNC(Bool) DeleteObject(HGDIOBJ hObject);

#define GetGValue(rgb) ((BYTE) (((WORD) (rgb)) >> 8))
#define GetBValue(rgb) ((BYTE) ((rgb) >> 16))
#define GetRValue(rgb) ((BYTE) (rgb))

CFCOMPAT_FUNC(HWND) GetFocus();
CFCOMPAT_FUNC(int) MessageBox(HWND hWnd, LPCTSTR lpText, LPCTSTR lpCaption, UINT uType);

CFCOMPAT_FUNC(int) WINAPI GlobalSize(HGLOBAL hMem);
CFCOMPAT_FUNC(LPVOID) GlobalLock(HGLOBAL hMem);
CFCOMPAT_FUNC(Bool) GlobalUnlock(HGLOBAL hMem);
CFCOMPAT_FUNC(Bool) IsBadWritePtr(LPVOID lp, int ucb);
CFCOMPAT_FUNC(void) OutputDebugString(LPCTSTR lpOutputString);
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
		LPCTSTR lpszFace);
CFCOMPAT_FUNC(Bool) GetTextExtentPoint32(HDC hdc, LPCTSTR lpString, int c, LPSIZE lpSize);
CFCOMPAT_FUNC(Bool) EnumWindows(WNDENUMPROC lpEnumFunc, LPARAM lParam);
CFCOMPAT_FUNC(int) GetWindowText(HWND hWnd, LPTSTR lpString, int nMaxCount);

CFCOMPAT_FUNC(HMODULE) LoadLibrary(LPCTSTR lpFileName);
CFCOMPAT_FUNC(Bool) FreeLibrary(HMODULE hModule);
CFCOMPAT_FUNC(void*) GetProcAddress(HMODULE hModule, LPCSTR lpProcName);

CFCOMPAT_FUNC(HGDIOBJ) GetStockObject(int fnObject);
CFCOMPAT_FUNC(Bool) IsWindowVisible(HWND hWnd);
CFCOMPAT_FUNC(LRESULT) DefWindowProc(HWND hWnd, UINT Msg, WPARAM wParam,
		LPARAM lParam);
CFCOMPAT_FUNC(LONG) GetWindowLong(HWND hWnd, int nIndex);
CFCOMPAT_FUNC(Bool) RegisterClass(const WNDCLASS *lpWndClass);
CFCOMPAT_FUNC(HMODULE) GetModuleHandle(LPCTSTR lpModuleName);
CFCOMPAT_FUNC(HICON) LoadIcon(HINSTANCE hInstance, LPCTSTR lpIconName);

CFCOMPAT_FUNC(long) filelength(int fd);
CFCOMPAT_FUNC(int) LoadCursor(HINSTANCE hInstance, LPCTSTR lpCursorName);
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
