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

/**
 * \file
 * Typedefs for the various win32-only variable types.
 * Only defines those types that are currently undefined. Thus this
 * can be included after other headers without redefinition errors.
 * 
 * Most of these are also in c_types.h and/or cttypes.h. I added them
 * here so as to make changes minimal and isolated. Eventually this
 * header should be removed altogether. 
 */

#ifndef compat_typedefs_h_
#define compat_typedefs_h_

#include "minmax.h"

#ifndef BOOL
#define BOOL int
#endif

#ifndef HANDLE
#define HANDLE int
#endif

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE 1
#endif

#ifndef APIENTRY /* Not currently used in Linux. */
#define APIENTRY
#endif

#ifndef DWORD
#define DWORD unsigned long int 
#endif

typedef int (* _CRT_ALLOC_HOOK) (int, void *, size_t, int, long, const char *, int);
    
#ifndef LPVOID
#define LPVOID void*
#endif

#ifndef HINSTANCE
#define HINSTANCE int
#endif

#ifndef HWND
#define HWND unsigned int
#endif

#ifndef _ASSERT
#define _ASSERT assert
#endif

#ifndef LPSTR
#define LPSTR char*
#endif

#ifndef HDC
#define HDC int
#endif

#ifndef HUGE
#define HUGE
#endif

typedef struct {
    int left;
    int right;
    int top;
    int bottom;
} RECT;

typedef struct {
     char * lpszClassName;
     HINSTANCE hInstance;
     void * lpfnWndProc;
     unsigned int style;
     int hbrBackground;
     int hCursor;
     int hIcon;
     char * lpszMenuName;
     char *cbClsExtra;
     char *cbWndExtra;
} WNDCLASS;

#ifndef UINT
#define UINT unsigned int
#endif

#ifndef WPARAM
#define WPARAM int
#endif

#ifndef LPARAM
#define LPARAM int
#endif

#ifndef O_BINARY
#define O_BINARY 0
#endif

#ifndef LPTSTR
#define LPTSTR char *
#endif

int LoadString(HINSTANCE hInstance, UINT uID, LPTSTR lpBuffer, int nBufferMax);

/* A bunch of windows DLL initialisation values. I don't know the
 * real values of these, so I just put in some values. They are not
 * used anyway, so no harm.
 */

#ifndef DLL_PROCESS_ATTACH
#define DLL_PROCESS_ATTACH 1
#endif

#ifndef DLL_PROCESS_DETACH
#define DLL_PROCESS_DETACH 2
#endif

#ifndef DLL_THREAD_ATTACH
#define DLL_THREAD_ATTACH 3
#endif

#ifndef DLL_THREAD_DETACH
#define DLL_THREAD_DETACH 4
#endif

#ifndef _MAX_PATH
#define _MAX_PATH 1024
#endif

#ifndef GWL_HINSTANCE
#define GWL_HINSTANCE 42
#endif

#ifndef WS_OVERLAPPEDWINDOW
#define WS_OVERLAPPEDWINDOW 6
#endif

#ifndef GRAY_BRUSH
#define GRAY_BRUSH 1
#endif

#ifndef NULL_BRUSH
#define NULL_BRUSH 0
#endif

#ifndef WM_DESTROY
#define WM_DESTROY 96
#endif

#ifndef WM_SIZE
#define WM_SIZE 10
#endif

#ifndef WM_PAINT
#define WM_PAINT 73
#endif

#ifndef PAINTSTRUCT
#define PAINTSTRUCT int
#endif

#ifndef WHITE_BRUSH
#define WHITE_BRUSH 939
#endif

#ifndef IDC_ARROW
#define IDC_ARROW 38
#endif

#ifndef IDI_ASTERISK
#define IDI_ASTERISK 939
#endif

#endif
