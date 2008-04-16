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
#include "compat_defs.h"

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

void GlobalFree(void *f) {
    
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
