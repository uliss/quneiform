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

#include <stdio.h>
#include <wchar.h>

#ifdef WIN32
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#include <io.h>
#endif

#include "globus.h"
#include "compat_defs.h"
#include "common/point.h"

#ifdef __CFCOMPAT__
#define CFCOMPAT_FUNC  FUN_EXPO__
#else
#define CFCOMPAT_FUNC  FUN_IMPO__
#endif

#ifndef WIN32
CFCOMPAT_FUNC void* GlobalAlloc(uint uFlags, int dwBytes);
CFCOMPAT_FUNC void GlobalFree(void *f);
CFCOMPAT_FUNC void * GlobalLock(HGLOBAL hMem);
CFCOMPAT_FUNC Bool GlobalUnlock(HGLOBAL hMem);

CFCOMPAT_FUNC int GetLastError();
CFCOMPAT_FUNC Handle CreateFile(const char * lpFileName, uint32_t dwDesiredAccess,
        uint32_t dwShareMode, void* lpSecurityAttributes, uint32_t dwCreationDisposition,
        uint32_t dwFlagsAndAttributes, Handle hTemplateFile);
CFCOMPAT_FUNC uint32_t GetModuleFileName(HMODULE hModule, char * lpFilename, size_t size);
CFCOMPAT_FUNC int GetTempFileName(const char * lpPathName, const char * lpPrefixString,
        uint uUnique, char * lpTempFileName);
CFCOMPAT_FUNC size_t GetTempPath(size_t BufferLength, char * buffer);
CFCOMPAT_FUNC long _msize(void *memblock);
int RemoveDirectory(const char *d);

CFCOMPAT_FUNC long _tell(int handle);
CFCOMPAT_FUNC int _access(const char *filename, int mode);
CFCOMPAT_FUNC long filelength(int fd);
CFCOMPAT_FUNC long filelength(FILE * f);

CFCOMPAT_FUNC uint GetPrivateProfileString(const char * AppName, const char * KeyName,
        const char * Default, char* ReturnedString, size_t nSize, const char * lpFileName);
CFCOMPAT_FUNC Bool WritePrivateProfileString(const char * AppName, const char * KeyName,
        const char * String, const char * FileName);
CFCOMPAT_FUNC uint GetPrivateProfileInt(const char * AppName, const char * KeyName,
        uint defaultValue, const char * FileName);
CFCOMPAT_FUNC Bool GetComputerName(char* buffer, size_t * size);
CFCOMPAT_FUNC Bool CreateDirectory(const char * dir, void* = 0);
CFCOMPAT_FUNC void OutputDebugString(const char * lpOutputString);

#define GetGValue(rgb) ((uchar) (((uint16_t) (rgb)) >> 8))
#define GetBValue(rgb) ((uchar) ((rgb) >> 16))
#define GetRValue(rgb) ((uchar) (rgb))
#define RGB(r,g,b) ((COLORREF)(((uchar)(r)|((uint16_t)((uchar)(g))<<8))|(((uint32_t)(uchar)(b))<<16)))

CFCOMPAT_FUNC int MessageBox(HWND hWnd, const char * lpText, const char * lpCaption, uint uType);
CFCOMPAT_FUNC int wsprintf(char* lpOut, const char * lpFmt, ...);
CFCOMPAT_FUNC Bool SetRect(LPRECT lprc, int xLeft, int yTop, int xRight, int yBottom);
CFCOMPAT_FUNC Bool PtInRect(const RECT *lprc, const cf::Point16& pt);
CFCOMPAT_FUNC Bool IntersectRect(LPRECT lprcDst, const RECT *lprcSrc1, const RECT *lprcSrc2);
CFCOMPAT_FUNC Bool UnionRect(LPRECT lprcDst, const RECT *lprcSrc1, const RECT *lprcSrc2);
CFCOMPAT_FUNC Bool Rectangle(HDC hdc, int nLeftRect, int nTopRect, int nRightRect, int nBottomRect);
#endif

#ifdef WIN32 
CFCOMPAT_FUNC char* mkdtemp(char *tmpl);

#ifdef _MSC_VER
#define snprintf _snprintf
#define strncasecmp _strnicmp
#define strcasecmp _stricmp
#endif
#endif

CFCOMPAT_FUNC int open_data_file(const char *basename, int mode);
CFCOMPAT_FUNC int data_file_exists(const char *basename);
CFCOMPAT_FUNC void split_path(const char *fname, char *file_path, char *basename, char *ext);
CFCOMPAT_FUNC void make_path(char *opath, const char *dir, const char *basename, const char *ext);
CFCOMPAT_FUNC void winpath_to_internal(char *p);
CFCOMPAT_FUNC unsigned int curr_dir(unsigned int bsize, char* buf);
CFCOMPAT_FUNC FILE* create_temp_file(void);

#endif
