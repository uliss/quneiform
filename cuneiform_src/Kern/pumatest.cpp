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

/* This is a simple test program for the Puma library. It does not
 * actually do anything.
 */

#include<stdio.h>
#include"puma.h"

int main(int argc, char **argv) {
    if(argc != 2) {
        printf("Usage: %s imagefile\n", argv[0]);
        return 0;
    }
    if(!PUMA_Init(0, 0)) {
        printf("PUMA_Init failed.\n");
        return 1;
    }
    printf("Puma initialized.\n");
    
    if(!PUMA_XOpen(argv[1], "none.txt")) {
        printf("PUMA_Xopen failed.\n");
        return 1;
    }
    printf("PUMA_XOpen succeeded.\n");
    
    if(!PUMA_XClose()) {
        printf("PUMA_XClose failed.\n");
        return 1;
    }
    printf("PUMA_XClose succeeded.\n");
    
    if(!PUMA_Done()) {
        printf("PUMA_Done failed.\n");
        return 1;
    }
    printf("PUMA_Done succeeded.\nAll done.\n");
    return 0;
}
