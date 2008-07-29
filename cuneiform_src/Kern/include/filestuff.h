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

#ifndef FILESTUFF_H_
#define FILESTUFF_H_


#ifndef _MAX_PATH
#define _MAX_PATH 1024
#endif

#ifndef MAX_PATH
#define MAX_PATH _MAX_PATH
#endif

#ifndef _MAX_FNAME
#define _MAX_FNAME 32
#endif

#ifndef _MAX_DRIVE
#define _MAX_DRIVE 32
#endif

#ifndef _MAX_DIR
#define _MAX_DIR 128
#endif

#ifndef _MAX_EXT
#define _MAX_EXT 32
#endif

#endif
