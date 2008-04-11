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

#ifndef DWORD /* 32 bit unsigned value */
#define DWORD unsigned int 
#endif
/*
#ifndef Word32
#define Word32 unsigned int
#endif

#ifndef Int32
#define Int32 int
#endif

#ifndef Int16
#define Int16 short
#endif

#ifndef Word16
#define Word16 unsigned short
#endif
*/
#ifndef LPVOID
#define LPVOID void*
#endif

#ifndef HINSTANCE
#define HINSTANCE int
#endif

#ifndef _ASSERT
#define _ASSERT assert
#endif

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

/* The code base has a gazillion MIN and MAX macros. Some parts of the
 * code use them without proper includes which lead to link errors.
 */
#ifndef MIN
#define MIN(a,b) (((a)>(b))?(b):(a))
#endif
#ifndef MAX
#define MAX(a,b) (((a)>(b))?(a):(b))
#endif

#endif
