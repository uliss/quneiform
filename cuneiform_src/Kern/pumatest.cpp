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

/* This is a simple test program for the Puma library. */

#include "ctiimage.h"

//#include"dpuma.h"

#include<stdio.h>
#include<stdint.h>
#include<stdlib.h>
#include<cstring>
#include"puma.h"

struct langlist {
    int puma_number;
    const char *name;
};

/* Language codes according to ISO 639-2. Most of these don't seem to have
 * corresponding data files. A bug?
 */
static const langlist langs[] = { 
        {PUMA_LANG_ENGLISH,   "eng"},
        {PUMA_LANG_GERMAN,    "ger"},
        {PUMA_LANG_FRENCH,    "fra"},
        {PUMA_LANG_RUSSIAN,   "rus"},
        {PUMA_LANG_SWEDISH,   "swe"},
        {PUMA_LANG_SPANISH,   "spa"},
        {PUMA_LANG_ITALIAN,   "ita"},
        {PUMA_LANG_RUSENG,    "ruseng"},
        {PUMA_LANG_UKRAINIAN, "ukr"},
        {PUMA_LANG_SERBIAN,   "srp"},
        {PUMA_LANG_CROATIAN,  "hrv"},
        {PUMA_LANG_POLISH,    "pol"},
        {PUMA_LANG_DANISH,    "dan"},
        {PUMA_LANG_PORTUGUESE,"por"},
        {PUMA_LANG_DUTCH,     "dut"},
        {PUMA_LANG_DIG,       "dig"}, // What is this language?
        {PUMA_LANG_UZBEK,     "uzb"},
        {PUMA_LANG_KAZ,       "kaz"}, 
        {PUMA_LANG_KAZ_ENG,   "kazeng"},
        {PUMA_LANG_CZECH,     "cze"},
        {PUMA_LANG_ROMAN,     "rum"},
        {PUMA_LANG_HUNGAR,    "hun"},
        {PUMA_LANG_BULGAR,    "bul"},
        {PUMA_LANG_SLOVENIAN, "slo"},
        {PUMA_LANG_LATVIAN,   "lav"},
        {PUMA_LANG_LITHUANIAN,"lit"},
        {PUMA_LANG_ESTONIAN,  "est"},
        {PUMA_LANG_TURKISH,   "tur"},
        {-1, NULL}
};

static void print_supported_languages() {
    printf("Supported languages:");
    for(const langlist *l = langs; l->puma_number >= 0; l++)
        printf(" %s", l->name);
    printf(".\n");
}

int main(int argc, char **argv) {

    char bmpheader[2];
    char *dib;
    const char *infilename = NULL;
    Word32 langcode = PUMA_LANG_ENGLISH; // By default recognize plain english text.
    const char *outfilename = "pumaout.txt";
    FILE *f;
    int32_t dibsize, offset;
    
    printf("Cuneiform for Linux 0.1\n");
    
    for(int i=1; i<argc; i++) {
        /* Changing language. */
        if(strcmp(argv[i], "-l") == 0) {
            if(++i >= argc) {
                printf("Missing language after -l.\n");
                print_supported_languages();
                return 1;
            }
            for(int j=0; langs[j].puma_number >= 0; j++) {
                if(strcmp(langs[j].name, argv[i]) == 0) {
                    langcode = langs[j].puma_number;
                    break;
                }
            }
            if(langcode == -1) {
                printf("Unknown language %s.\n", argv[i]);
                print_supported_languages();
                return 1;
            }
        } else {        
        /* No switches, so set input file. */
        infilename = argv[i];
        }
    }
    
    if(infilename == NULL) {
        printf("Usage: %s [-l languagename] imagefile\n", argv[0]);
        return 0;
    }
    f = fopen(infilename, "rb");
    if(!f) {
        printf("Could not open file %s.\n", infilename);
        return 1;
    }
    fread(bmpheader, 1, 2, f);
    if(bmpheader[0] != 'B' || bmpheader[1] != 'M') {
        printf("%s is not a BMP file.\n", infilename);
        return 1;
    }
    fread(&dibsize, sizeof(int32_t), 1, f);
    fread(bmpheader, 1, 2, f);
    fread(bmpheader, 1, 2, f);
    fread(&offset, sizeof(int32_t), 1, f);

    dibsize -= ftell(f);
    dib = (char*)malloc(dibsize);
    fread(dib, dibsize, 1, f);
    fclose(f);
    
    if(*((int32_t*)dib) != 40) {
        printf("BMP is not of type \"Windows V3\", which is the only supported format.\n");
        printf("Please convert your BMP to uncompressed V3 format and try again.");
        return 1;
    }
    
    if(*((int32_t*) (dib+16)) != 0) {
        printf("%s is a compressed BMP. Only uncompressed BMP files are supported.\n", infilename);
        printf("Please convert your BMP to uncompressed V3 format and try again.");
        return 1;
    }
    
    if(!PUMA_Init(0, 0)) {
        printf("PUMA_Init failed.\n");
        return 1;
    }
    //printf("Puma initialized.\n");
    
    // Set the language.
    PUMA_SetImportData(PUMA_Word32_Language, &langcode);
    
    if(!PUMA_XOpen(dib, "none.txt")) {
        printf("PUMA_Xopen failed.\n");
        return 1;
    }
    //printf("PUMA_XOpen succeeded.\n");
    
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
    //printf("PUMA_XFinalRecognition succeeded.\n");
    
    if(!PUMA_XSave(outfilename, PUMA_TOTEXT, 0)) {
        printf("PUMA_XSave failed.\n");
        return 1;
    }
    //printf("PUMA_XSave succeeded.\n");
    
    if(!PUMA_XClose()) {
        printf("PUMA_XClose failed.\n");
        return 1;
    }
    //printf("PUMA_XClose succeeded.\n");
    
    if(!PUMA_Done()) {
        printf("PUMA_Done failed.\n");
        return 1;
    }
    //printf("PUMA_Done succeeded.\nAll done.\n");
    free(dib);
    return 0;
}
