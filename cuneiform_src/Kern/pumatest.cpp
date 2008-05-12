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
#include<stdint.h>
#include<stdlib.h>
#include"puma.h"

int main(int argc, char **argv) {
    char bmpheader[2];
    char *dib;
    const char *outfilename = "pumaout.txt";
    FILE *f;
    int32_t dibsize, offset;
    
    if(argc != 2) {
        printf("Usage: %s imagefile\n", argv[0]);
        return 0;
    }
    f = fopen(argv[1], "rb");
    if(!f) {
        printf("Could not open file %s.\n", argv[1]);
        return 1;
    }
    fread(bmpheader, 1, 2, f);
    if(bmpheader[0] != 'B' || bmpheader[1] != 'M') {
        printf("Invalid BMP header.\n");
        return 1;
    }
    fread(&dibsize, sizeof(int32_t), 1, f);
    fread(bmpheader, 1, 2, f);
    fread(bmpheader, 1, 2, f);
    fread(&offset, sizeof(int32_t), 1, f);

    dibsize -= ftell(f);
    dib = (char*)malloc(dibsize); // I don't free() this one, because I am just that tough.
    fread(dib, dibsize, 1, f);
    fclose(f);
    
    
    if(!PUMA_Init(0, 0)) {
        printf("PUMA_Init failed.\n");
        return 1;
    }
    printf("Puma initialized.\n");
    
    if(!PUMA_XOpen(dib, "none.txt")) {
        printf("PUMA_Xopen failed.\n");
        return 1;
    }
    printf("PUMA_XOpen succeeded.\n");
    
    /* From recogpuma.cpp
    LPUMA_SetSpeller(g_bSpeller);
    LPUMA_SetOneColumn(g_bOneColumn);
    LPUMA_SetFax100(g_bFax100);
    LPUMA_SetDotMatrix(g_bDotMatrix);
    LPUMA_SetPictures(g_nPicture);
    LPUMA_SetTables(g_nTable);
    LPUMA_SetFormatMode(g_nFormat);
    LPUMA_SetUnrecogChar(g_btUnrecogChar);

    LPUMA_SetBold(g_bBold);
    LPUMA_SetItalic(g_bItalic);
    LPUMA_SetSize(g_bSize);
    LPUMA_SetFormat(g_bFormat);
    LPUMA_SetSerifName(g_strSerifName);
    LPUMA_SetSansSerifName(g_strSansSerifName);
    LPUMA_SetCourierName(g_strCourierName);

    rc = LPUMA_XFinalRecognition();
    */
    if(!PUMA_XFinalRecognition()) {
        printf("PUMA_XFinalrecognition failed.\n");
        return 1;
    }
    printf("PUMA_XFinalRecognition succeeded.\n");
    
    if(!PUMA_XSave(outfilename, PUMA_TOTEXT, 0)) {
        printf("PUMA_XSave failed.\n");
        return 1;
    }
    printf("PUMA_XSave succeeded.\n");
    
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
