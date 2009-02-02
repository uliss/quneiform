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

#ifndef WINFUNCS_H_
#define WINFUNCS_H_

#if WIN32

#include<windows.h>

#else // WIN32

#include "globus.h"

#ifdef __WINDUMMY__
  #define WINDUMMY_FUNC  FUN_EXPO
#else
  #define WINDUMMY_FUNC  FUN_IMPO
#endif


#include<wchar.h>
#include "compat_defs.h"

WINDUMMY_FUNC(int) LoadString(HINSTANCE hInstance, UINT uID, LPTSTR lpBuffer, int nBufferMax);

WINDUMMY_FUNC(int) CreateDirectory(const char *dir, void *dummy);
WINDUMMY_FUNC(DWORD) GetTempPath(DWORD nBufferLength, LPTSTR lpBuffer);
WINDUMMY_FUNC(int) RemoveDirectory(const char *d);
WINDUMMY_FUNC(void*) GlobalAlloc(UINT uFlags, int dwBytes);
WINDUMMY_FUNC(HGLOBAL) GlobalFree(void *f);
WINDUMMY_FUNC(void*) GlobalReAlloc(void* hMem, int dwBytes, UINT uFlags);
WINDUMMY_FUNC(int) GetTempFileName(LPCTSTR lpPathName, LPCTSTR lpPrefixString,
                    UINT uUnique, LPTSTR lpTempFileName);
WINDUMMY_FUNC(int) GetLastError();

WINDUMMY_FUNC(DWORD) GetModuleFileName(HMODULE hModule, LPTSTR lpFilename, DWORD nSize);
WINDUMMY_FUNC(BOOL) CloseHandle(HANDLE hObject);
WINDUMMY_FUNC(HANDLE) CreateFile(LPCTSTR lpFileName, DWORD dwDesiredAccess,
DWORD dwShareMode, void* lpSecurityAttributes,
DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile);

WINDUMMY_FUNC(HWND) FindWindow(LPCTSTR lpClassName, LPCTSTR lpWindowName);
WINDUMMY_FUNC(UINT) RegisterWindowMessage(LPCTSTR lpString);

WINDUMMY_FUNC(int) _findclose(long handle);
WINDUMMY_FUNC(long) _findfirst(const char *filespec, struct _finddata_t *fileinfo);
WINDUMMY_FUNC(int) _findnext(long handle, struct _finddata_t *fileinfo);
WINDUMMY_FUNC(long) _tell(int handle);

WINDUMMY_FUNC(BOOL) GetComputerName(LPTSTR lpBuffer, long unsigned int *lpnSize);

WINDUMMY_FUNC(LONG) RegOpenKeyEx(HKEY hKey, LPCTSTR lpSubKey, DWORD ulOptions,
  REGSAM samDesired, PHKEY phkResult);
WINDUMMY_FUNC(LONG) RegQueryValueEx(HKEY hKey, LPCTSTR lpValueName,
LPDWORD lpReserved, LPDWORD lpType, LPBYTE lpData,
LPDWORD lpcbData);

WINDUMMY_FUNC(BOOL) GetClientRect(HWND hWnd, LPRECT lpRect);
WINDUMMY_FUNC(BOOL) WritePrivateProfileString(LPCTSTR lpAppName,
        LPCTSTR lpKeyName, LPCTSTR lpString, LPCTSTR lpFileName);
WINDUMMY_FUNC(DWORD) GetPrivateProfileString(LPCTSTR lpAppName, LPCTSTR lpKeyName,
LPCTSTR lpDefault, LPTSTR lpReturnedString, DWORD nSize, LPCTSTR lpFileName);
WINDUMMY_FUNC(UINT) GetPrivateProfileInt(LPCTSTR lpAppName,
  LPCTSTR lpKeyName, INT nDefault, LPCTSTR lpFileName);

WINDUMMY_FUNC(int) WideCharToMultiByte(UINT CodePage, DWORD dwFlags, const wchar_t *lpWideCharStr,
  int cchWideChar, LPSTR lpMultiByteStr, int cbMultiByte,
  LPCSTR lpDefaultChar, LPBOOL lpUsedDefaultChar);

WINDUMMY_FUNC(BOOL) ShowWindow(HWND hWnd, int nCmdShow);

WINDUMMY_FUNC(long) _filelength(int fd);
WINDUMMY_FUNC(long) _msize(void *memblock);
WINDUMMY_FUNC(int) _access(const char *filename, int mode);
WINDUMMY_FUNC(BOOL) SetWindowText(HWND hWnd,LPCTSTR lpString);
WINDUMMY_FUNC(int) ReleaseDC(HWND hWnd, HDC hDC);
WINDUMMY_FUNC(BOOL) IsIconic(HWND hWnd);
WINDUMMY_FUNC(HDC) GetDC(HWND hWnd);
WINDUMMY_FUNC(BOOL) EndPaint(HWND hWnd, ...);
WINDUMMY_FUNC(HDC) BeginPaint(HWND hwnd,...);
WINDUMMY_FUNC(LRESULT) SendMessage(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
WINDUMMY_FUNC(void) strlwr(char *foo);
WINDUMMY_FUNC(HWND) CreateWindow(
    LPCTSTR lpClassName,
    LPCTSTR lpWindowName,
    DWORD dwStyle,
    int x,
    int y,
    int nWidth,
    int nHeight,
    HWND hWndParent,
    HMENU hMenu,
    HINSTANCE hInstance,
    LPVOID lpParam
);

WINDUMMY_FUNC(HGDIOBJ) SelectObject(HDC hdc, HGDIOBJ hgdiobj);
WINDUMMY_FUNC(LPTSTR) lstrcat(LPTSTR lpString1, LPTSTR lpString2);
WINDUMMY_FUNC(int) lstrlen(LPCTSTR lpString);
WINDUMMY_FUNC(int) lstrcmp(LPCTSTR lpString1, LPCTSTR lpString2);
WINDUMMY_FUNC(LPTSTR) lstrcpy(LPTSTR lpString1, LPCTSTR lpString2);
WINDUMMY_FUNC(int) wsprintf(LPTSTR lpOut, LPCTSTR lpFmt, ...);
WINDUMMY_FUNC(int) lstrcmpi(LPCTSTR lpString1, LPCTSTR lpString2);

WINDUMMY_FUNC(BOOL) DeleteObject(HGDIOBJ hObject);

#define GetGValue(rgb) ((BYTE) (((WORD) (rgb)) >> 8))
#define GetBValue(rgb) ((BYTE) ((rgb) >> 16))
#define GetRValue(rgb) ((BYTE) (rgb))

WINDUMMY_FUNC(HWND) GetFocus();
WINDUMMY_FUNC(int) MessageBox(HWND hWnd, LPCTSTR lpText, LPCTSTR lpCaption, UINT uType);

WINDUMMY_FUNC(int) WINAPI GlobalSize(HGLOBAL hMem);
WINDUMMY_FUNC(LPVOID) GlobalLock(HGLOBAL hMem);
WINDUMMY_FUNC(BOOL) GlobalUnlock(HGLOBAL hMem);
WINDUMMY_FUNC(BOOL) IsBadWritePtr(LPVOID lp, int ucb);
WINDUMMY_FUNC(void) OutputDebugString(LPCTSTR lpOutputString);
WINDUMMY_FUNC(BOOL) SetRect(LPRECT lprc, int xLeft, int yTop,
int xRight, int yBottom);
WINDUMMY_FUNC(BOOL) PtInRect(const RECT *lprc, POINT pt);
WINDUMMY_FUNC(BOOL) IntersectRect(LPRECT lprcDst, const RECT *lprcSrc1, const RECT *lprcSrc2);
WINDUMMY_FUNC(BOOL) UnionRect(LPRECT lprcDst, const RECT *lprcSrc1,const RECT *lprcSrc2);

WINDUMMY_FUNC(HWND) GetActiveWindow();
WINDUMMY_FUNC(HFONT) CreateFont(int nHeight, int nWidth, int nEscapement,
int nOrientation, int fnWeight, DWORD fdwItalic, DWORD fdwUnderline,
DWORD fdwStrikeOut, DWORD fdwCharSet, DWORD fdwOutputPrecision,
DWORD fdwClipPrecision, DWORD fdwQuality, DWORD fdwPitchAndFamily,
LPCTSTR lpszFace);
WINDUMMY_FUNC(BOOL) GetTextExtentPoint32(HDC hdc, LPCTSTR lpString, int c, LPSIZE lpSize);
WINDUMMY_FUNC(BOOL) EnumWindows(WNDENUMPROC lpEnumFunc, LPARAM lParam);
WINDUMMY_FUNC(int) GetWindowText(HWND hWnd, LPTSTR lpString, int nMaxCount);

WINDUMMY_FUNC(HMODULE) LoadLibrary(LPCTSTR lpFileName);
WINDUMMY_FUNC(BOOL) FreeLibrary(HMODULE hModule);
WINDUMMY_FUNC(void*) GetProcAddress(HMODULE hModule, LPCSTR lpProcName);

WINDUMMY_FUNC(HGDIOBJ) GetStockObject(int fnObject);
WINDUMMY_FUNC(BOOL) IsWindowVisible(HWND hWnd);
WINDUMMY_FUNC(LRESULT) DefWindowProc(HWND hWnd, UINT Msg, WPARAM wParam,
    LPARAM lParam);
WINDUMMY_FUNC(LONG) GetWindowLong(HWND hWnd, int nIndex);
WINDUMMY_FUNC(BOOL) RegisterClass(const WNDCLASS *lpWndClass);
WINDUMMY_FUNC(HMODULE) GetModuleHandle(LPCTSTR lpModuleName);
WINDUMMY_FUNC(HICON) LoadIcon(HINSTANCE hInstance, LPCTSTR lpIconName);

WINDUMMY_FUNC(long) filelength(int fd);
WINDUMMY_FUNC(int) LoadCursor(HINSTANCE hInstance, LPCTSTR lpCursorName);
WINDUMMY_FUNC(BOOL) Rectangle(HDC hdc,
  int nLeftRect, int nTopRect, int nRightRect, int nBottomRect);
WINDUMMY_FUNC(char*) _strupr(char*s);

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

WINDUMMY_FUNC(int)
open_data_file(const char *basename, int mode);

WINDUMMY_FUNC(int)
data_file_exists(const char *basename);

WINDUMMY_FUNC(void) split_path(const char *fname,
        char *file_path,
        char *basename,
        char *ext);

WINDUMMY_FUNC(void) make_path(char *opath,
        const char *dir,
        const char *basename,
        const char *ext);

WINDUMMY_FUNC(void) winpath_to_internal(char *p);

unsigned int curr_dir(unsigned int bsize, char* buf);

#ifdef __cplusplus
}
#endif

#endif
