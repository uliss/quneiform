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

#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>

#include "compat_defs.h"

int HFILE_ERROR;

/* Stub functions for Windows functions. These do nothing and will get
 * removed eventually.
 */

int LoadString(HINSTANCE hInstance, UINT uID, LPTSTR lpBuffer, int nBufferMax) {
    return 0;
}

int CreateDirectory(const char *dir, void *dummy) {
    return 0;
}

void _makepath(char *f,const char *drive,const char *dir,const char *fname,const char *ext) {
    
}

void _splitpath(/*const*/ char *f,char *drive,char *dir,char *fname,char *ext) {
    
}

char *_fullpath(char *absPath, const char *relPath, int maxLength) {
    return NULL;
}

// This is in effect memcpy.
void CopyMemory(void *Destination, const void* Source, int length) {
    
}

DWORD GetTempPath(DWORD nBufferLength, LPTSTR lpBuffer) {
    return 0;
}

int RemoveDirectory(const char *d) {
    return -1;
}

void* GlobalAlloc(UINT uFlags, int dwBytes) {
    return NULL;
}

HGLOBAL GlobalFree(void *f) {
    return NULL;
}

void* GlobalReAlloc(void* hMem, int dwBytes, UINT uFlags) {
    return NULL;
}

int GetTempFileName(LPCTSTR lpPathName, LPCTSTR lpPrefixString,
                    UINT uUnique, LPTSTR lpTempFileName) {
    return -1;
}

int GetLastError() {
    return 0;
}


DWORD GetModuleFileName(HMODULE hModule, LPTSTR lpFilename, DWORD nSize) {
    return 0;
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

BOOL DeleteFile(LPCTSTR lpFileName) {
    return FALSE;
}

UINT RegisterWindowMessage(LPCTSTR lpString) {
    return 0;
}

void Sleep(int s) {
    sleep(s);
}

int _findclose(long handle);
long _findfirst(const char *filespec, struct _finddata_t *fileinfo);
int _findnext(long handle, struct _finddata_t *fileinfo);
long _tell(int handle);

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

int _chdir(const char *dirname) {
    return chdir(dirname);
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
    return strcasecomp(lpString1, lpString2);
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

void *_fmemcpy(void *dst, const void *src, size_t length) {
    return memcpy(dst, src, length);
}

void *_fmemset(void *dst, int c, size_t length) {
    return memset(dst, c, length);
}

HWND GetFocus() {
    return NULL;
}

int MessageBox(HWND hWnd, LPCTSTR lpText, LPCTSTR lpCaption, UINT uType) {
    printf("lpText");
    return 0;
}

LPVOID GlobalLock(HGLOBAL hMem) {
    return NULL;
}

BOOL GlobalUnlock(HGLOBAL hMem) {
    return 0;
}

