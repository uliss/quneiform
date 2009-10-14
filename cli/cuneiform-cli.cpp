/*
 Copyright (c) 2008, 2009 Jussi Pakkanen

 Разрешается повторное распространение и использование как в виде исходного кода,
 так и в двоичной форме, с изменениями или без, при соблюдении следующих условий:

 * При повторном распространении исходного кода должны оставаться указанное
 выше уведомление об авторском праве, этот список условий и последующий
 отказ от гарантий.
 * При повторном распространении двоичного кода в документации и/или в
 других материалах, поставляемых при распространении, должны сохраняться
 указанная выше информация об авторском праве, этот список условий и
 последующий отказ от гарантий.
 * Ни название Cognitive Technologies, ни имена ее сотрудников не могут
 быть использованы в качестве средства поддержки и/или продвижения
 продуктов, основанных на этом ПО, без предварительного письменного
 разрешения.

 ЭТА ПРОГРАММА ПРЕДОСТАВЛЕНА ВЛАДЕЛЬЦАМИ АВТОРСКИХ ПРАВ И/ИЛИ ДРУГИМИ ЛИЦАМИ "КАК
 ОНА ЕСТЬ" БЕЗ КАКОГО-ЛИБО ВИДА ГАРАНТИЙ, ВЫРАЖЕННЫХ ЯВНО ИЛИ ПОДРАЗУМЕВАЕМЫХ,
 ВКЛЮЧАЯ ГАРАНТИИ КОММЕРЧЕСКОЙ ЦЕННОСТИ И ПРИГОДНОСТИ ДЛЯ КОНКРЕТНОЙ ЦЕЛИ, НО НЕ
 ОГРАНИЧИВАЯСЬ ИМИ. НИ ВЛАДЕЛЕЦ АВТОРСКИХ ПРАВ И НИ ОДНО ДРУГОЕ ЛИЦО, КОТОРОЕ
 МОЖЕТ ИЗМЕНЯТЬ И/ИЛИ ПОВТОРНО РАСПРОСТРАНЯТЬ ПРОГРАММУ, НИ В КОЕМ СЛУЧАЕ НЕ
 НЕСЁТ ОТВЕТСТВЕННОСТИ, ВКЛЮЧАЯ ЛЮБЫЕ ОБЩИЕ, СЛУЧАЙНЫЕ, СПЕЦИАЛЬНЫЕ ИЛИ
 ПОСЛЕДОВАВШИЕ УБЫТКИ, СВЯЗАННЫЕ С ИСПОЛЬЗОВАНИЕМ ИЛИ ПОНЕСЕННЫЕ ВСЛЕДСТВИЕ
 НЕВОЗМОЖНОСТИ ИСПОЛЬЗОВАНИЯ ПРОГРАММЫ (ВКЛЮЧАЯ ПОТЕРИ ДАННЫХ, ИЛИ ДАННЫЕ,
 СТАВШИЕ НЕГОДНЫМИ, ИЛИ УБЫТКИ И/ИЛИ ПОТЕРИ ДОХОДОВ, ПОНЕСЕННЫЕ ИЗ-ЗА ДЕЙСТВИЙ
 ТРЕТЬИХ ЛИЦ И/ИЛИ ОТКАЗА ПРОГРАММЫ РАБОТАТЬ СОВМЕСТНО С ДРУГИМИ ПРОГРАММАМИ,
 НО НЕ ОГРАНИЧИВАЯСЬ ЭТИМИ СЛУЧАЯМИ), НО НЕ ОГРАНИЧИВАЯСЬ ИМИ, ДАЖЕ ЕСЛИ ТАКОЙ
 ВЛАДЕЛЕЦ ИЛИ ДРУГОЕ ЛИЦО БЫЛИ ИЗВЕЩЕНЫ О ВОЗМОЖНОСТИ ТАКИХ УБЫТКОВ И ПОТЕРЬ.

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

#include "cimage/ctiimage.h" // Must be first, or else you get compile errors.
#include <iostream>
#include <sstream>
#include <iomanip>
#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <getopt.h>

#include "cttypes.h"
#include "puma/puma.h"
#include "puma/pumaimpl.h"
#include "lang_def.h"
#include "config.h"

using namespace std;

static const char * program_name = "";

struct langlist
{
    language_t code;
    const char *name;
    const char *fullname;
};

/* Language codes according to ISO 639-2.
 */
static const langlist langs[] = {
//
    { LANG_ENGLISH, "eng", "English" },
    { LANG_GERMAN, "ger", "German" },
    { LANG_FRENCH, "fra", "French" },
    { LANG_RUSSIAN, "rus", "Russian" },
    { LANG_SWEDISH, "swe", "Swedish" },
    { LANG_SPANISH, "spa", "Spanish" },
    { LANG_ITALIAN, "ita", "Italian" },
    { LANG_RUSENG, "ruseng", "Russian-English" },
    { LANG_UKRAINIAN, "ukr", "Ukrainian" },
    { LANG_SERBIAN, "srp", "Serbian" },
    { LANG_CROATIAN, "hrv", "Croatian" },
    { LANG_POLISH, "pol", "Polish" },
    { LANG_DANISH, "dan", "Danish" },
    { LANG_PORTUGUESE, "por", "Portuguese" },
    { LANG_DUTCH, "dut", "Dutch" },
    { LANG_DIG, "dig", "Digits" }, // This probably means "recognize digits only".
    //        {LANG_UZBEK,     "uzb"}, // These don't seem to have data files. Thus they are disabled.
    //        {LANG_KAZ,       "kaz"},
    //        {LANG_KAZ_ENG,   "kazeng"},
    { LANG_CZECH, "cze", "Czech" },
    { LANG_ROMAN, "rum", "Roman" },
    { LANG_HUNGAR, "hun", "Hungarian" },
    { LANG_BULGAR, "bul", "Bulgarian" },
    { LANG_SLOVENIAN, "slo", "Slovenian" },
    { LANG_LATVIAN, "lav", "Latvian" },
    { LANG_LITHUANIAN, "lit", "Lithuanian" },
    { LANG_ESTONIAN, "est", "Estonian" },
    { LANG_TURKISH, "tur", "Turkish" },
    { (language_t) - 1, NULL, NULL } };

struct formatlist
{
    puma_format_t code;
    const char * name;
    const char * descr;
};

static const formatlist formats[] = {
// Does not work.    {PUMA_TOTABLEDBF,   "dbf",       "DBF format"},
    { PUMA_TOHTML, "html", "HTML format" },
    { PUMA_TOHOCR, "hocr", "hOCR HTML format" },
    { PUMA_TOEDNATIVE, "native", "Cuneiform 2000 format" },
    { PUMA_TORTF, "rtf", "RTF format" },
    { PUMA_TOSMARTTEXT, "smarttext", "plain text with TeX paragraphs" },
    { PUMA_TOTEXT, "text", "plain text" },
    // Table code is missing. {PUMA_TOTABLETXT,   "tabletxt",  ""},
    { PUMA_DEBUG_TOTEXT, "textdebug", "for debugging purposes" },
    { (puma_format_t) -1, NULL, NULL } };

static string supported_languages() {
    ostringstream os;
    os << "Supported languages:\n";
    for (const langlist *l = langs; l->code >= 0; l++)
        os << "    " << left << setw(12) << l->name << " " << l->fullname << "\n";
    return os.str();
}

static string supported_formats() {
    ostringstream os;
    os << "Supported formats:\n";
    for (const formatlist * f = formats; f->code >= 0; f++)
        os << "    " << left << setw(12) << f->name << " " << f->descr << "\n";
    return os.str();
}

static string usage() {
    ostringstream os;
    os << "Usage: " << program_name << " [options] imagefile\n";
    os << ""
        "  -h   --help               Print this help message\n"
        "  -v   --verbose            Print verbose debugging messages\n"
        "  -V   --version            Print program version and exit\n"
        "       --autorotate         Automatically rotate input image\n"
        "  -f   --format   FORMAT    Sets output format\n"
        "                              type --format help to get full list\n"
        "  -l   --language LANGUAGE  Sets recognition language\n"
        "                              type --language help to gel full list\n"
        "  -o   --output   FILENAME  Sets output filename\n"
        "       --spell              Use spell correction\n"
        "       --onecolumn                              \n"
        "       --dotmatix                               \n"
        "       --fax                                    \n"
        "       --tables   MODE\n"
        "       --pictures MODE\n"
        "       --monospace-name     Use specified monospace font in RTF output\n"
        "       --serif-name         Use specified serif font in RTF output\n"
        "       --sansserif-name     Use seecified sans-serif font in RTF output\n";
    return os.str();
}

static puma_format_t output_format(const std::string& format) {
    for (int i = 0; formats[i].code >= 0; i++) {
        if (format == formats[i].name)
            return formats[i].code;
    }
    return (puma_format_t) -1;
}

static language_t recognize_language(const std::string& language) {
    for (const langlist *l = langs; l->code >= 0; l++) {
        if (language == l->name)
            return l->code;
    }
    return (language_t) - 1;
}

static string default_output_name(puma_format_t format) {
    string result = "cuneiform-out.";
    switch (format) {
    case PUMA_TOHOCR:
    case PUMA_TOHTML:
        result += "html";
        break;
    case PUMA_TORTF:
        result += "rtf";
        break;
    case PUMA_TOTEXT:
    case PUMA_TOSMARTTEXT:
    case PUMA_TOTABLETXT:
        result += "txt";
        break;
    case PUMA_TOEDNATIVE:
        result += "cf";
        break;
    case PUMA_TOTABLEDBF:
        result += "dbf";
        break;
    default:
        result += "buginprogram";
        break;
    }
    return result;
}

/**
 * Read file and return it as a BMP DIB entity. On failure write an error
 * and return NULL. Caller delete[]'s the returned result.
 */
static char* read_file(const char *fname);

#ifdef USE_MAGICK
#include <Magick++.h>

static char* read_file(const char *fname) {
    using namespace Magick;
    Blob blob;
    size_t data_size;
    char *dib;
    try {
        Image image(fname);
        // Write to BLOB in BMP format
        image.write(&blob, "DIB");
    }
    catch(Exception &error_) {
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

    if (*((int32_t*) dib) != 40) {
        cerr << "BMP is not of type \"Windows V3\", which is the only supported format.\n";
        cerr << "Please convert your BMP to uncompressed V3 format and try again.\n";
        delete[] dib;
        return NULL;
    }

    if (*((int32_t*) (dib + 16)) != 0) {
        cerr << fname << "is a compressed BMP. Only uncompressed BMP files are supported.\n";
        cerr << "Please convert your BMP to uncompressed V3 format and try again.";
        delete[] dib;
        return NULL;
    }
    return dib;
}
#endif // USE_MAGICK
int main(int argc, char **argv) {
    program_name = argv[0];

    int do_verbose = FALSE, do_fax = FALSE, do_dotmatrix = FALSE, do_speller = FALSE,
            do_singlecolumn = FALSE, do_pictures = FALSE, do_tables = FALSE, do_autorotate = FALSE;

    const char * const short_options = ":ho:vVl:f:";
    const struct option long_options[] = {
    //
        { "autorotate", no_argument, &do_autorotate, 1 },//
        { "dotmarix", no_argument, &do_dotmatrix, 1 },//
        { "fax", no_argument, &do_fax, 1 },//
        { "format", required_argument, NULL, 'f' },//
        { "help", no_argument, NULL, 'h' },//
        { "language", required_argument, NULL, 'l' },//
        { "monospace-name", required_argument, NULL, 'x' },
        { "output", required_argument, NULL, 'o' },//
        { "pictures", no_argument, &do_pictures, 1 },//
        { "sansserif-name", required_argument, NULL, 'y' },
        { "serif-name", required_argument, NULL, 'z' },
        { "onecolumn", no_argument, &do_singlecolumn, 1 },//
        { "spell", no_argument, &do_speller, 1 },//
        { "tables", required_argument, &do_tables, 1 },//
        { "verbose", no_argument, &do_verbose, 1 },//
        { "version", no_argument, NULL, 'V' },//
        { NULL, 0, NULL, 0 } };

    string outfilename, infilename, monospace, serif, sansserif;
    puma_format_t outputformat = PUMA_TOTEXT;
    language_t langcode = LANG_ENGLISH;
    int code;
    while ((code = getopt_long(argc, argv, short_options, long_options, NULL)) != -1) {
        switch (code) {
        case 'f': {
            if (strcmp(optarg, "help") == 0) {
                cout << supported_formats();
                return EXIT_SUCCESS;
            }

            puma_format_t format = output_format(optarg);
            if (format == -1) {
                cerr << "Unknown output format: " << optarg;
                cerr << supported_formats();
                return EXIT_FAILURE;
            }
            outputformat = format;

        }
            break;
        case 'h':
            cout << usage();
            return EXIT_SUCCESS;
            break;
        case 'l': {
            if (strcmp(optarg, "help") == 0) {
                cout << supported_languages();
                return EXIT_SUCCESS;
            }
            language_t lang = recognize_language(optarg);
            if (lang == -1) {
                cerr << "Unknown language: " << optarg;
                cerr << supported_languages();
                return EXIT_FAILURE;
            }
            langcode = lang;
        }
            break;
        case 'o':
            outfilename = optarg;
            break;
        case 'v':
            do_verbose = 1;
            break;
        case 'V':
            cout << "Cuneiform for Linux " << CF_VERSION << "\n";
            return EXIT_SUCCESS;
            break;
        case 'x':
            monospace = optarg;
            break;
        case 'y':
            sansserif = optarg;
            break;
        case 'z':
            serif = optarg;
            break;
        case 0:
            /*getopt_long() set option, just continue*/
            break;
        case ':':
            cerr << program_name << ": option '-" << (char) optopt << "' requires an argument\n";
            if (optopt == 'f')
                cerr << supported_formats();
            else if (optopt == 'l')
                cerr << supported_languages();

            return EXIT_FAILURE;
            break;
        case '?':
        default:
            cerr << program_name << ": option '-" << (char) optopt << "' is invalid: ignored\n";
            return EXIT_FAILURE;
            break;
        }
    }

    if (optind == argc) {
        cerr << "Input file not specified\n";
        cerr << usage();
        return EXIT_FAILURE;
    }
    else
        infilename = argv[optind];

    if (outfilename.empty())
        outfilename = default_output_name(outputformat);

    char * dib = read_file(infilename.c_str());
    if (!dib) // Error msg is already printed so just get out.
        return 1;

    using namespace CIF;

    Puma::instance().setOptionLanguage(langcode);
    Puma::instance().setOptionOneColumn(do_singlecolumn);
    Puma::instance().setOptionFax100(do_fax);
    Puma::instance().setOptionDotMatrix(do_dotmatrix);
    Puma::instance().setOptionUseSpeller(do_speller);
    Puma::instance().setOptionAutoRotate(do_autorotate);

    if (!serif.empty())
        Puma::instance().setOptionSerifName(serif.c_str());
    if (!sansserif.empty())
        Puma::instance().setOptionSansSerifName(sansserif.c_str());
    if (!monospace.empty())
        Puma::instance().setOptionMonospaceName(monospace.c_str());

    //	Puma::instance().setOptionUnrecognizedChar('?');
    //	Puma::instance().setOptionBold(true);
    //	Puma::instance().setOptionItalic(true);
    //  Puma::instance().setOptionSize(true);
    //  Puma::instance().setOptionFormatMode(puma_format_mode_t t);
    //	Puma::instance().setOptionPictures(puma_picture_t mode);
    //  Puma::instance().setOptionTables(puma_table_t mode);

    Puma::instance().open(dib);
    Puma::instance().recognize();
    Puma::instance().save(outfilename, outputformat);
    Puma::instance().close();

    delete[] dib;
    return 0;
}
