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

#include<iostream>
#include<sstream>
#include<iomanip>
#include<stdio.h>
#include<stdint.h>
#include<stdlib.h>
#include<cstring>
#include"puma.h"
#include"config.h"

using namespace std;

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
//        {PUMA_LANG_DIG,       "dig"}, // This probably means "recognize digits only".
//        {PUMA_LANG_UZBEK,     "uzb"}, // These don't seem to have data files. Thus they are disabled.
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

struct formatlist {
    int puma_number;
    const char * name;
    const char * descr;
};

static const formatlist formats[] = {
// Does not work.    {PUMA_TOTABLEDBF,   "dbf",       "DBF format"},
    {PUMA_TOHTML,       "html",      "HTML format"},
    {PUMA_TOHOCR,       "hocr",      "hOCR HTML format"},
    {PUMA_TOEDNATIVE,   "native",    "Cuneiform 2000 format"},
    {PUMA_TORTF,        "rtf",       "RTF format"},
    {PUMA_TOSMARTTEXT,  "smarttext", "plain text with TeX paragraphs"},
    {PUMA_TOTEXT,       "text",      "plain text"},
// Table code is missing. {PUMA_TOTABLETXT,   "tabletxt",  ""},
    {-1, NULL}
};


static string supported_languages() {
    ostringstream os;
    os << "Supported languages:";
    for(const langlist *l = langs; l->puma_number >= 0; l++)
        os << " " << l->name;
    os << ".\n";
    return os.str();
}

static string supported_formats() {
    ostringstream os;
    os << "Supported formats:\n";
    for(const formatlist * f = formats; f->puma_number >= 0; f++)
        os << "    " << setiosflags(ios::left) << setw(12) << f->name << " " << f->descr << "\n";
    return os.str();
}

/**
 * Read file and return it as a BMP DIB entity. On failure write an error
 * and return NULL. Caller delete[]'s the returned result.
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
        image.write(&blob, "DIB");
    } catch(Exception &error_) {
        cerr << error_.what() << "\n";
        return NULL;
    }
    data_size = blob.length();
    dib = new char[data_size];
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
        cerr << "Could not open file " << fname << ".\n";
        return NULL;
    }
    fread(bmpheader, 1, 2, f);
    if (bmpheader[0] != 'B' || bmpheader[1] != 'M') {
        cerr << fname << " is not a BMP file.\n";
        return NULL;
    }
    fread(&dibsize, sizeof(int32_t), 1, f);
    fread(bmpheader, 1, 2, f);
    fread(bmpheader, 1, 2, f);
    fread(&offset, sizeof(int32_t), 1, f);

    dibsize -= ftell(f);
    dib = new char[dibsize];
    fread(dib, dibsize, 1, f);
    fclose(f);

    if (*((int32_t*)dib) != 40) {
        cerr << "BMP is not of type \"Windows V3\", which is the only supported format.\n";
        cerr << "Please convert your BMP to uncompressed V3 format and try again.\n";
        return NULL;
    }

    if (*((int32_t*) (dib+16)) != 0) {
        cerr << fname << "is a compressed BMP. Only uncompressed BMP files are supported.\n";
        cerr << "Please convert your BMP to uncompressed V3 format and try again.";
        return NULL;
    }
    return dib;
}

#endif // USE_MAGICK

int main(int argc, char **argv) {
    char *dib;
    const char *infilename = NULL;
    int langcode = PUMA_LANG_ENGLISH; // By default recognize plain english text.
    Bool32 dotmatrix = FALSE;
    Bool32 fax = FALSE;
    const char *defaultnamestem = "cuneiform-out.";
    string outfilename;
    Int32 outputformat = PUMA_TOTEXT;

    cout << "Cuneiform for Linux " << CF_VERSION << "\n";

    /* Parsing command line parameters. */
    for(int i=1; i<argc; i++) {
        /* Changing language. */
        if(strcmp(argv[i], "-l") == 0) {
            langcode = -1;
            if(++i >= argc) {
                cout << supported_languages();
                return 1;
            }
            for(int j=0; langs[j].puma_number >= 0; j++) {
                if(strcmp(langs[j].name, argv[i]) == 0) {
                    langcode = langs[j].puma_number;
                    break;
                }
            }
            if(langcode == -1) {
                cerr << "Unknown language " << argv[i] << ".\n";
                cerr << supported_languages();
                return 1;
            }
        } else if(strcmp(argv[i], "-f") == 0) {
            outputformat = -1;
            if(++i >= argc) {
                cout << supported_formats();
                return 1;
            }
            for(int j=0; formats[j].puma_number >= 0; j++) {
                if(strcmp(formats[j].name, argv[i]) == 0) {
                    outputformat = formats[j].puma_number;
                    break;
                }
            }
            if(outputformat == -1) {
                cerr << "Unknown format " << argv[i] << ".\n";
                cerr << supported_formats();
                return 1;
            }
        } else if(strcmp(argv[i], "-o") == 0) {
            if(++i >= argc) {
                cerr << "Missing output file name.\n";
                return 1;
            }
            outfilename = argv[i];
        } else if(strcmp(argv[i], "--dotmatrix") == 0) {
            dotmatrix = TRUE;
        } else if(strcmp(argv[i], "--fax") == 0) {
            fax = TRUE;
        } else {
        /* No switches, so set input file. */
        infilename = argv[i];
        }
    }

    if (outfilename.empty()) {
        outfilename = defaultnamestem;
        switch (outputformat) {
            case PUMA_TOHOCR:
            case PUMA_TOHTML:
                outfilename += "html";
                break;

            case PUMA_TORTF:
                outfilename += "rtf";
                break;

            case PUMA_TOTEXT:
            case PUMA_TOSMARTTEXT:
            case PUMA_TOTABLETXT:
                outfilename += "txt";
                break;

            case PUMA_TOEDNATIVE:
                outfilename += "cf";
                break;

            case PUMA_TOTABLEDBF:
                outfilename += "dbf";
                break;

            default:
                outfilename += "buginprogram";
                break;
        }
    }

    if(infilename == NULL) {
        cout << "Usage: " << argv[0] << "[-l languagename -f format --dotmatrix --fax -o result_file] imagefile\n";
        return 0;
    }

    dib = read_file(infilename);
    if(!dib) // Error msg is already printed so just get out.
        return 1;

    if(!PUMA_Init(0, 0)) {
        cerr << "PUMA_Init failed.\n";
        return 1;
    }

    // Set the language.
    PUMA_SetImportData(PUMA_Word32_Language, &langcode);
    PUMA_SetImportData(PUMA_Bool32_DotMatrix, &dotmatrix);
    PUMA_SetImportData(PUMA_Bool32_Fax100, &fax);

    if(!PUMA_XOpen(dib, "none.txt")) {
        cerr << "PUMA_Xopen failed.\n";
        return 1;
    }

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
    */
    if(!PUMA_XFinalRecognition()) {
        cerr << "PUMA_XFinalrecognition failed.\n";
        return 1;
    }

    if(!PUMA_XSave(outfilename.c_str(), outputformat, PUMA_CODE_UTF8)) {
        cerr << "PUMA_XSave failed.\n";
        return 1;
    }

    if(!PUMA_XClose()) {
        cerr << "PUMA_XClose failed.\n";
        return 1;
    }

    if(!PUMA_Done()) {
        cerr << "PUMA_Done failed.\n";
        return 1;
    }

    delete []dib;
    return 0;
}
