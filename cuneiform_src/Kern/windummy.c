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

#include <errno.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <dlfcn.h>

#if defined(__APPLE__) && defined(__MACH__)
#include <malloc/malloc.h>
#define malloc_usable_size(a) malloc_size(a)
#elif defined(__FreeBSD__)
/* freebsd system malloc don`t work correctly with malloc_usable_size in c++ */
#include <dlmalloc/malloc.h>
#else
#include <malloc.h>
#endif


#include "compat_defs.h"
#include "config.h"

int HFILE_ERROR;

/* Stub functions for Windows functions. These do nothing and will get
 * removed eventually.
 */

int LoadString(HINSTANCE hInstance, UINT uID, LPTSTR lpBuffer, int nBufferMax) {
    return 0;
}

int CreateDirectory(const char *dir, void *dummy) {
       if(!mkdir(dir,0755)) return TRUE;
       else return FALSE;
}

void _makepath(char *f,const char *drive,const char *dir,const char *fname,const char *ext) {

}

/* These should use split_path instead. It is portable. */

void _splitpath(/*const*/ char *f,char *drive,char *dir,char *fname,char *ext) {
    drive[0] = '\0';
    dir[0] = '\0';
    ext[0] = '\0';
    strcpy(fname, f);
}

char *_fullpath(char *absPath, const char *relPath, int maxLength) {
    return NULL;
}

DWORD GetTempPath(DWORD nBufferLength, LPTSTR lpBuffer) {
    return 0;
}

int RemoveDirectory(const char *d) {
  return rmdir(d);
}

void* GlobalAlloc(UINT uFlags, int dwBytes) {
  if(uFlags & GMEM_ZEROINIT)
    return calloc(dwBytes, 1);
  return malloc(dwBytes);
}

HGLOBAL GlobalFree(void *f) {
  free(f);
}

void* GlobalReAlloc(void* hMem, int dwBytes, UINT uFlags) {
  return realloc(hMem, dwBytes); // Should init to zero on uFlags & GMEM_ZEROINIT.
}

int GetTempFileName(LPCTSTR lpPathName, LPCTSTR lpPrefixString,
                    UINT uUnique, LPTSTR lpTempFileName) {
    return -1;
}

int GetLastError() {
    return errno;
}


DWORD GetModuleFileName(HMODULE hModule, LPTSTR lpFilename, DWORD nSize) {
    lpFilename[0] = '.'; /* Currently all modules must be in the directory pumatest was run in. */
    lpFilename[1] = '\0';
    return 1;
}

BOOL CloseHandle(HANDLE hObject) {
    return FALSE;
}

DWORD GetCurrentDirectory(DWORD nBufferLength, LPTSTR lpBuffer) {
    return 0;
}

HANDLE CreateFile(LPCTSTR lpFileName, DWORD dwDesiredAccess,
DWORD dwShareMode, void* lpSecurityAttributes,
DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile) {
    return 0;
}

HWND FindWindow(LPCTSTR lpClassName, LPCTSTR lpWindowName) {
    return 0;
}

UINT RegisterWindowMessage(LPCTSTR lpString) {
    return 0;
}

int _findclose(long handle);
long _findfirst(const char *filespec, struct _finddata_t *fileinfo);
int _findnext(long handle, struct _finddata_t *fileinfo);
long _tell(int handle) {
    return lseek(handle, 0, SEEK_CUR);
}

BOOL GetComputerName(LPTSTR lpBuffer, long unsigned int *lpnSize) {
    return 0;
}

UINT GetDriveType(LPCTSTR lpRootPathName) {
    return 0;
}

LONG RegOpenKeyEx(HKEY hKey, LPCTSTR lpSubKey, DWORD ulOptions,
  REGSAM samDesired, PHKEY phkResult) {
    return 0;
}

LONG RegQueryValueEx(HKEY hKey, LPCTSTR lpValueName,
LPDWORD lpReserved, LPDWORD lpType, LPBYTE lpData,
LPDWORD lpcbData) {
    return 0;
}

BOOL GetClientRect(HWND hWnd, LPRECT lpRect) {
    return 0;
}

BOOL WritePrivateProfileString(LPCTSTR lpAppName,
        LPCTSTR lpKeyName, LPCTSTR lpString, LPCTSTR lpFileName) {
    return 0;
}

DWORD GetPrivateProfileString(LPCTSTR lpAppName, LPCTSTR lpKeyName,
LPCTSTR lpDefault, LPTSTR lpReturnedString, DWORD nSize, LPCTSTR lpFileName) {
    return 0;
}

UINT GetPrivateProfileInt(LPCTSTR lpAppName,
  LPCTSTR lpKeyName, INT nDefault, LPCTSTR lpFileName) {
    return 0;
}

int WideCharToMultiByte(UINT CodePage, DWORD dwFlags,LPCWSTR lpWideCharStr,
  int cchWideChar, LPSTR lpMultiByteStr, int cbMultiByte,
  LPCSTR lpDefaultChar, LPBOOL lpUsedDefaultChar) {
    return 0;
}
BOOL ShowWindow(HWND hWnd, int nCmdShow) {
    return 0;
}

long _filelength(int fd) {
    struct stat foo;
    if(fstat(fd, &foo) != 0) {
        return 0;
    }
    return foo.st_size;
}

long filelength(int fd) {
        return _filelength(fd);
}


long _msize(void *memblock) {
    return malloc_usable_size(memblock);
}

/* All uses in Cuneiform just check if the file exists. Ignoring all other
 * cases.
 */
int _access(const char *filename, int mode) {
    struct stat foo;
    assert(mode == 0);
    return stat(filename, &foo);
}

BOOL SetWindowText(HWND hWnd,LPCTSTR lpString) {
    return 0;
}

int ReleaseDC(HWND hWnd, HDC hDC) {
    return 0;
}

BOOL IsIconic(HWND hWnd) {
    return 0;
}

HDC GetDC(HWND hWnd) {
    return 0;
}

BOOL EndPaint(HWND hWnd, ...) {
    return 1;
}

HDC BeginPaint(HWND hwnd,...) {
    return 0;
}

LRESULT SendMessage(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam) {
    return 0;
}

void strlwr(char *foo) {
    // FIXME: this is probably actually used somewhere.
}

HWND CreateWindow(
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
    LPVOID lpParam) {
    return (HWND) 55;
}

HGDIOBJ SelectObject(HDC hdc, HGDIOBJ hgdiobj) {
    return 0;
}

LPTSTR lstrcat(LPTSTR lpString1, LPTSTR lpString2) {
    return strcat(lpString1, lpString2);
}

int lstrlen(LPCTSTR lpString) {
    return strlen(lpString);
}

int lstrcmp(LPCTSTR lpString1, LPCTSTR lpString2) {
    return strcmp(lpString1, lpString2);
}

LPTSTR lstrcpy(LPTSTR lpString1, LPTSTR lpString2) {
    return strcpy(lpString1, lpString2);
}

int wsprintf(LPTSTR lpOut, LPCTSTR lpFmt, ...) {
    return sprintf(lpOut, "WSPRINTF-FUNCTION");
}

int lstrcmpi(LPCTSTR lpString1, LPCTSTR lpString2) {
    return strcasecmp(lpString1, lpString2);
}

HDC CreateMetaFile(LPCTSTR lpszFile) {
    return NULL;
}

HMETAFILE CloseMetaFile(HDC hdc) {
    return 0;
}

BOOL SetWindowOrgEx(HDC hdc, int X, int Y, LPPOINT lpPoint) {
    return 0;
}

BOOL SetWindowExtEx(HDC hdc, int nXExtent, int nYExtent,
LPSIZE lpSize) {
    return 0;
}

COLORREF SetTextColor(HDC hdc, COLORREF crColor) {
    return NULL;
}

COLORREF SetBkColor(HDC hdc, COLORREF crColor) {
    return NULL;
}

int SetStretchBltMode(HDC hdc, int iStretchMode) {
    return 0;
}

HPALETTE CreatePalette(const LOGPALETTE *lplgpl) {
    return NULL;
}

HPALETTE SelectPalette(HDC hdc, HPALETTE hpal, BOOL bForceBackground) {
    return hpal;
}

UINT RealizePalette(HDC hdc) {
    return 0;
}

int StretchDIBits(HDC hdc, int XDest, int YDest, int nDestWidth,
int nDestHeight, int XSrc, int YSrc, int nSrcWidth, int nSrcHeight,
const void *lpBits, const BITMAPINFO *lpBitsInfo, UINT iUsage,
DWORD dwRop) {
    return -1;
}

BOOL UnrealizeObject(HGDIOBJ hgdiobj) {
    return 0;
}

BOOL DeleteObject(HGDIOBJ hObject) {
    return 0;
}

BOOL DeleteMetaFile(HMETAFILE hmf) {
    return 0;
}

UINT GetMetaFileBitsEx(HMETAFILE hmf, UINT nSize, LPVOID lpvData) {
    return 0;
}

BYTE GetRValue(DWORD rgb) {
    return 0;
}

BYTE GetGValue(DWORD rgb) {
    return 0;
}

BYTE GetBValue(DWORD rgb) {
    return 0;
}

HWND GetFocus() {
    return NULL;
}

int MessageBox(HWND hWnd, LPCTSTR lpText, LPCTSTR lpCaption, UINT uType) {
    printf("lpText");
    return 0;
}

int WINAPI GlobalSize(HGLOBAL hMem) {
    return 0;
}

LPVOID GlobalLock(HGLOBAL hMem) {
    return NULL;
}

BOOL GlobalUnlock(HGLOBAL hMem) {
    return 0;
}

BOOL IsBadWritePtr(LPVOID lp, int ucb) {
    return 0;
}

void OutputDebugString(LPCTSTR lpOutputString) {

}

BOOL SetRect(LPRECT lprc, int xLeft, int yTop,
int xRight, int yBottom) {
    lprc->left = xLeft;
    lprc->right = xRight;
    lprc->top = yTop;
    lprc->bottom = yBottom;
    return TRUE;
}

BOOL PtInRect(const RECT *lprc, POINT pt) {
    if(pt.x >= lprc->left && pt.x < lprc->right
            && pt.y >= lprc->top && pt.y < lprc->bottom)
        return TRUE;
    return FALSE;
}

/* This is only called from creatertf.cpp. It does not use lprcDst at all so
 * we do not need to calculate it.
 */

BOOL IntersectRect(LPRECT lprcDst, const RECT *lprcSrc1, const RECT *lprcSrc2) {
    if(lprcSrc1->left > lprcSrc2->right
       || lprcSrc1->right < lprcSrc2->left
       || lprcSrc1->top > lprcSrc2->bottom
       || lprcSrc1->bottom < lprcSrc2->top)
        return FALSE;
    return TRUE;
}

BOOL UnionRect(LPRECT lprcDst, const RECT *lprcSrc1, const RECT *lprcSrc2) {
    if(lprcSrc1->left - lprcSrc1->right == 0 || lprcSrc1->top - lprcSrc1->bottom == 0) {
        lprcDst->left = lprcSrc2->left;
        lprcDst->right = lprcSrc2->right;
        lprcDst->top = lprcSrc2->top;
        lprcDst->bottom = lprcSrc2->bottom;
        return TRUE;
    }
    if(lprcSrc2->left - lprcSrc2->right == 0 || lprcSrc2->top - lprcSrc2->bottom == 0) {
        lprcDst->left = lprcSrc1->left;
        lprcDst->right = lprcSrc1->right;
        lprcDst->top = lprcSrc1->top;
        lprcDst->bottom = lprcSrc1->bottom;
        return TRUE;
    }

    lprcDst->left = lprcSrc1->left < lprcSrc2->left ? lprcSrc1->left : lprcSrc2->left;
    lprcDst->right = lprcSrc1->right > lprcSrc2->right ? lprcSrc1->right : lprcSrc2->right;
    lprcDst->top = lprcSrc1->top < lprcSrc2->top ? lprcSrc1->top : lprcSrc2->top;
    lprcDst->bottom = lprcSrc1->bottom > lprcSrc2->bottom ? lprcSrc1->bottom : lprcSrc2->bottom;

    return 0;
}

HWND GetActiveWindow() {
    return NULL;
}

HFONT CreateFont(int nHeight, int nWidth, int nEscapement,
int nOrientation, int fnWeight, DWORD fdwItalic, DWORD fdwUnderline,
DWORD fdwStrikeOut, DWORD fdwCharSet, DWORD fdwOutputPrecision,
DWORD fdwClipPrecision, DWORD fdwQuality, DWORD fdwPitchAndFamily,
LPCTSTR lpszFace) {
    return 0;
}

BOOL GetTextExtentPoint32(HDC hdc, LPCTSTR lpString, int c, LPSIZE lpSize) {
    return 0;
}

BOOL EnumWindows(WNDENUMPROC lpEnumFunc, LPARAM lParam) {
    return 0;
}

int GetWindowText(HWND hWnd, LPTSTR lpString, int nMaxCount) {
    return 0;
}

HMODULE LoadLibrary(LPCTSTR lpFileName) {
	return dlopen(lpFileName, RTLD_LAZY);
}

BOOL FreeLibrary(HMODULE hModule) {
	return dlclose(hModule);
}

void* GetProcAddress(HMODULE hModule, LPCSTR lpProcName) {
	return dlsym(hModule, lpProcName);
}

HGDIOBJ GetStockObject(int fnObject) {
	return -1;
}

BOOL IsWindowVisible(HWND hWnd) {
	return 0;
}

LRESULT DefWindowProc(HWND hWnd, UINT Msg, WPARAM wParam,
    LPARAM lParam) {
	return 0;
}

LONG GetWindowLong(HWND hWnd, int nIndex) {
	return 0;
}

BOOL RegisterClass(const WNDCLASS *lpWndClass) {
	return 0;
}

HMODULE GetModuleHandle(LPCTSTR lpModuleName) {
	return NULL;
}

HICON LoadIcon(HINSTANCE hInstance, LPCTSTR lpIconName) {
	return NULL;
}

int LoadCursor(HINSTANCE hInstance, LPCTSTR lpCursorName) {
	return 0;
}

BOOL Rectangle(HDC hdc,
  int nLeftRect, int nTopRect, int nRightRect, int nBottomRect) {
	return 0;
}

/* String to uppercase. */

char* _strupr(char*s) {
  return s;
}

/* General helper functions. */

/* We try to locate data files in two locations:
 *
 * 1. At the directory pointed to by environment variable CF_DATADIR
 * 2. At install prefix.
 *
 * Caller's responsibility is to ensure *e1 and *e2 are large enough.
 *
 * Porting note: on Windows it probably makes sense to use the function
 * that tells the current code module's path and go from there.
 *
 * The same probably goes for OS X, if you want to go the GUI route.
 *
 */

static void build_name_estimates(const char *base_name, char *env_name, char *prefix_name) {
    const char *separator = "/"; /* Change to backslash on Windows. */
    const char *env_prefix;
    const char *varname = "CF_DATADIR";
    int len = 0;

    env_name[0] = '\0';
    prefix_name[0] = '\0';

    env_prefix = getenv(varname);
    if(env_prefix)
        len = strlen(env_prefix);
    if(len > 0) {
        strcat(env_name, env_prefix);
        if(strcmp(env_prefix + len-1, separator) != 0) {
            strcat(env_name, separator);
        }
        strcat(env_name, base_name);
    }

    len = strlen(INSTALL_DATADIR);
    if(len > 0) {
        strcat(prefix_name, INSTALL_DATADIR);
        strcat(prefix_name, separator);
        strcat(prefix_name, base_name);
    }
}

int open_data_file(const char *basename, int mode) {
    char ename[1024];
    char pname[1024];
    int i;

    build_name_estimates(basename, ename, pname);
    i = open(ename, mode);
    if(i != -1)
        return i;
    return open(pname, mode);
}

int data_file_exists(const char *basename) {
    char ename[1024];
    char pname[1024];

    build_name_estimates(basename, ename, pname);
    if(_access(ename, 0) == 0)
        return 0;
    return _access(pname, 0);
}

/* Split a file name in three: path, base file name, and extension.
 * All internal file names use / as path separator, even on Windows.
 */

void split_path(const char *fname, char *file_path, char *basename, char *ext) {
    int last_path = -1;
    int suff = -1;
    size_t l = strlen(fname);
    int path_end, base_start, base_end, ext_start;
    int i;

    file_path[0] = '\0';
    basename[0] = '\0';

    for (i = 0; i < l; i++) {
        if (fname[i] == '.')
            suff = i;
        if (fname[i] == '/')
            last_path = i;
    }

    path_end = 0;
    base_start = 0;
    if (last_path == 0) { // File in root.
        path_end = 1;
        base_start = 1;
    }
    if (last_path > 0) {
        path_end = last_path;
        base_start = last_path + 1;
    }

    if (suff > last_path) {
        ext_start = suff + 1;
        base_end = suff;
    } else {
        base_end = ext_start = l;
    }

    memcpy(file_path, fname, path_end);
    file_path[path_end] = '\0';
    memcpy(basename, fname + base_start, base_end - base_start);
    basename[base_end - base_start] = '\0';
    memcpy(ext, fname + ext_start, l - ext_start);
    ext[l - ext_start] = '\0';
}
