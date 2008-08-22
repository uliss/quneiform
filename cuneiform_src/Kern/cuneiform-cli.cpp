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

/* This is a simple command line program for the Puma library. */

#include"ctiimage.h" // Must be first, or else you get compile errors.

#include<stdio.h>
#include<stdint.h>
#include<stdlib.h>
#include<cstring>
#include"puma.h"
#include"config.h"

struct langlist {
    int puma_number;
    const char *name;
};

/* Language codes according to ISO 639-2.
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
//        {PUMA_LANG_DIG,       "dig"}, // These four languages don't seem to have data files.
//        {PUMA_LANG_UZBEK,     "uzb"}, // Thus they are disabled.
//        {PUMA_LANG_KAZ,       "kaz"},
//        {PUMA_LANG_KAZ_ENG,   "kazeng"},
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

/**
 * Read file and return it as a BMP DIB entity. On failure write an error
 * and return NULL. Caller free()'s the returned result.
 */
static char* read_file(const char *fname);

#ifdef USE_MAGICK
#include <Magick++.h>
using namespace Magick;

static char* read_file(const char *fname) {
    Blob blob;
    size_t data_size;
    char *dib;
    try {
        Image image(fname);
        // Write to BLOB in BMP format
        image.write( &blob, "DIB" );
    } catch(Exception &error_) {
        printf("ImageMagick error: %s\n", error_.what());
        return NULL;
    }
    data_size = blob.length();
    dib = static_cast<char*> (malloc(data_size));
    memcpy(dib, blob.data(), data_size);
    return dib;
}

#else // No ImageMagick++

static char* read_file(const char *fname) {
    char bmpheader[2];
    char *dib;
    FILE *f;
    int32_t dibsize, offset;

    f = fopen(fname, "rb");
    if (!f) {
        printf("Could not open file %s.\n", fname);
        return NULL;
    }
    fread(bmpheader, 1, 2, f);
    if (bmpheader[0] != 'B' || bmpheader[1] != 'M') {
        printf("%s is not a BMP file.\n", fname);
        return NULL;
    }
    fread(&dibsize, sizeof(int32_t), 1, f);
    fread(bmpheader, 1, 2, f);
    fread(bmpheader, 1, 2, f);
    fread(&offset, sizeof(int32_t), 1, f);

    dibsize -= ftell(f);
    dib = static_cast<char*> (malloc(dibsize));
    fread(dib, dibsize, 1, f);
    fclose(f);

    if (*((int32_t*)dib) != 40) {
        printf("BMP is not of type \"Windows V3\", which is the only supported format.\n");
        printf("Please convert your BMP to uncompressed V3 format and try again.");
        return NULL;
    }

    if (*((int32_t*) (dib+16)) != 0) {
        printf("%s is a compressed BMP. Only uncompressed BMP files are supported.\n",
               fname);
        printf("Please convert your BMP to uncompressed V3 format and try again.");
        return NULL;
    }
    return dib;
}

#endif // USE_MAGICK

int main(int argc, char **argv) {
    char *dib;
    const char *infilename = NULL;
    Word32 langcode = PUMA_LANG_ENGLISH; // By default recognize plain english text.
    Bool32 dotmatrix = FALSE;
    Bool32 fax = FALSE;
    const char *defaulttextname = "cuneiform-out.txt";
    const char *defaulthtmlname = "cuneiform-out.html";
    const char *outfilename = NULL;
    Int32 outputformat = PUMA_TOTEXT;

    printf("Cuneiform for Linux %s\n", CF_VERSION);

    for(int i=1; i<argc; i++) {
        /* Changing language. */
        if(strcmp(argv[i], "-l") == 0) {
            langcode = -1;
            if(++i >= argc) {
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
        } else if(strcmp(argv[i], "-o") == 0) {
            if(++i >= argc) {
                printf("Missing output file name.\n");
                return 1;
            }
            outfilename = argv[i];
        } else if(strcmp(argv[i], "--html") == 0) {
            outputformat = PUMA_TOHTML;
        } else if(strcmp(argv[i], "--dotmatrix") == 0) {
            dotmatrix = TRUE;
        } else if(strcmp(argv[i], "--fax") == 0) {
            fax = TRUE;
        } else {
        /* No switches, so set input file. */
        infilename = argv[i];
        }
    }

    if(outfilename == NULL) {
        // Use default output name.
        switch(outputformat) {
            case PUMA_TOHTML : outfilename = defaulthtmlname; break;
            default : outfilename = defaulttextname; break;
        }
    }

    if(infilename == NULL) {
        printf("Usage: %s [-l languagename --html --dotmatrix --fax -o result_file] imagefile\n", argv[0]);
        return 0;
    }

    dib = read_file(infilename);
    if(!dib) // Error msg is already printed so just get out.
        return 1;

    if(!PUMA_Init(0, 0)) {
        printf("PUMA_Init failed.\n");
        return 1;
    }
    //printf("Puma initialized.\n");

    // Set the language.
    PUMA_SetImportData(PUMA_Word32_Language, &langcode);
    PUMA_SetImportData(PUMA_Bool32_DotMatrix, &dotmatrix);
    PUMA_SetImportData(PUMA_Bool32_Fax100, &fax);

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

    /* This seems to work (at least not crash). But since it is untested and
     * I am not sure what is the proper function calling convention, it
     * is disabled.
     */
    /*
    Bool32 singlecolumn = FALSE;
    PUMA_SetImportData(PUMA_Bool32_OneColumn, &foo);

    if(!PUMA_XPageAnalysis()) {
        printf("PUMA_XPageAnalysis failed.\n");
        return 1;
    }
    //printf("PUMA_XPageAnalysis succeeded.\n");
    */
    if(!PUMA_XFinalRecognition()) {
        printf("PUMA_XFinalrecognition failed.\n");
        return 1;
    }
    //printf("PUMA_XFinalRecognition succeeded.\n");

    if(!PUMA_XSave(outfilename, outputformat, 0)) {
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
 //   free(dib);
    return 0;
}
