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

/***************************************************************************
 *   Copyright (C) 2009 by Serge Poltavsky                                 *
 *   serge.poltavski@gmail.com                                             *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program. If not, see <http://www.gnu.org/licenses/>   *
 ***************************************************************************/

/* This is a simple command line program for the Puma library. */
#include <iostream>
#include <sstream>
#include <iomanip>
#include <cstdlib>
#include <cstring>
#include <getopt.h>
#include <Magick++.h>
#include <boost/scoped_array.hpp>
#include "config.h"

#include "puma/puma.h"
#include "common/cifconfig.h"

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
    { LANGUAGE_ENGLISH, "eng", "English" },
    { LANGUAGE_GERMAN, "ger", "German" },
    { LANGUAGE_FRENCH, "fra", "French" },
    { LANGUAGE_RUSSIAN, "rus", "Russian" },
    { LANGUAGE_SWEDISH, "swe", "Swedish" },
    { LANGUAGE_SPANISH, "spa", "Spanish" },
    { LANGUAGE_ITALIAN, "ita", "Italian" },
    { LANGUAGE_RUS_ENG, "ruseng", "Russian-English" },
    { LANGUAGE_UKRAINIAN, "ukr", "Ukrainian" },
    { LANGUAGE_SERBIAN, "srp", "Serbian" },
    { LANGUAGE_CROATIAN, "hrv", "Croatian" },
    { LANGUAGE_POLISH, "pol", "Polish" },
    { LANGUAGE_DANISH, "dan", "Danish" },
    { LANGUAGE_PORTUGUESE, "por", "Portuguese" },
    { LANGUAGE_DUTCH, "dut", "Dutch" },
    { LANGUAGE_DIGITS, "dig", "Digits" }, // This probably means "recognize digits only".
    //        {LANGUAGE_UZBEK,     "uzb"}, // These don't seem to have data files. Thus they are disabled.
    //        {LANGUAGE_KAZAKH,       "kaz"},
    //        {LANGUAGE_KAZ_ENG,   "kazeng"},
    { LANGUAGE_CZECH, "cze", "Czech" },
    { LANGUAGE_ROMANIAN, "rum", "Roman" },
    { LANGUAGE_HUNGARIAN, "hun", "Hungarian" },
    { LANGUAGE_BULGARIAN, "bul", "Bulgarian" },
    { LANGUAGE_SLOVENIAN, "slo", "Slovenian" },
    { LANGUAGE_LATVIAN, "lav", "Latvian" },
    { LANGUAGE_LITHUANIAN, "lit", "Lithuanian" },
    { LANGUAGE_ESTONIAN, "est", "Estonian" },
    { LANGUAGE_TURKISH, "tur", "Turkish" },
    { (language_t) -1, NULL, NULL } };

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
        "  -h   --help                   Print this help message\n"
        "  -v   --verbose                Print verbose debugging messages\n"
        "  -V   --version                Print program version and exit\n"
        "       --debug-dump             Dumps various temporary recognition data\n"
        "                                   to current directory\n"
        "       --autorotate             Automatically rotate input image\n"
        "  -f   --format   FORMAT        Sets output format\n"
        "                                   type --format help to get full list\n"
        "  -l   --language LANGUAGE      Sets recognition language\n"
        "                                   type --language help to gel full list\n"
        "  -o   --output   FILENAME      Sets output filename\n"
        "       --spell                  Use spell correction\n"
        "       --onecolumn              Use one column layout\n"
        "       --dotmatrix                               \n"
        "       --fax                                     \n"
        "       --tables   MODE\n"
        "       --pictures               Search pictures (default)\n"
        "       --nopictures             Do not search pictures\n"
        "       --preserve-line-breaks                  \n"
        "       --unrecognized CHAR      Set symbol, that shown instead of unrecognized characters.\n"
        "                                    Default is '~'.\n"
        "       --monospace-name         Use specified monospace font in RTF output\n"
        "       --serif-name             Use specified serif font in RTF output\n"
        "       --sansserif-name         Use seecified sans-serif font in RTF output\n";
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
    return (language_t) -1;
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
static char* read_file(const char *fname) {
    using namespace Magick;
    Blob blob;
    size_t data_size;
    char *dib;
    try {
        Image image(fname);
        switch (image.type()) {
        case BilevelType:
        case TrueColorType:
            break;
        default:
            image.type(TrueColorType);
        }
        if (CIF::Config::instance().debugHigh())
            image.verbose(true);
        // Write to BLOB in BMP format
        image.magick("DIB");
        image.write(&blob);
    }
    catch (Exception &error_) {
        cerr << error_.what() << "\n";
        return NULL;
    }
    data_size = blob.length();
    dib = new char[data_size];
    memcpy(dib, blob.data(), data_size);
    return dib;
}

int main(int argc, char **argv) {
    program_name = argv[0];
    std::string dictionaries;
    char unrecognized_char = 0;

    int do_verbose = FALSE, do_fax = FALSE, do_dotmatrix = FALSE, do_speller = FALSE,
            do_singlecolumn = FALSE, do_pictures = TRUE, do_tables = FALSE, do_autorotate = FALSE,
            preserve_line_breaks = FALSE, do_dump = FALSE;

    const char * const short_options = ":ho:vVl:f:d:u:";
    const struct option long_options[] = {
    //
        { "autorotate", no_argument, &do_autorotate, 1 },//
        { "debug-dump", no_argument, &do_dump, 1 },//
        { "dictionary", required_argument, NULL, 'd' }, //
        { "dotmatrix", no_argument, &do_dotmatrix, 1 },//
        { "fax", no_argument, &do_fax, 1 },//
        { "format", required_argument, NULL, 'f' },//
        { "help", no_argument, NULL, 'h' },//
        { "language", required_argument, NULL, 'l' },//
        { "monospace-name", required_argument, NULL, 'x' },
        { "nopictures", no_argument, &do_pictures, 0 },//
        { "output", required_argument, NULL, 'o' },//
        { "pictures", no_argument, &do_pictures, 1 },//
        { "preserve-line-breaks", no_argument, &preserve_line_breaks, 1 },//
        { "sansserif-name", required_argument, NULL, 'y' },
        { "serif-name", required_argument, NULL, 'z' },
        { "onecolumn", no_argument, &do_singlecolumn, 1 },//
        { "spell", no_argument, &do_speller, 1 },//
        { "tables", required_argument, &do_tables, 1 },//
        { "unrecognized", required_argument, NULL, 'u' },//
        { "verbose", no_argument, &do_verbose, 1 },//
        { "version", no_argument, NULL, 'V' },//
        { NULL, 0, NULL, 0 } };

    string outfilename, infilename, monospace, serif, sansserif;
    puma_format_t outputformat = PUMA_TOTEXT;
    language_t langcode = LANGUAGE_ENGLISH;
    int code;
    while ((code = getopt_long(argc, argv, short_options, long_options, NULL)) != -1) {
        switch (code) {
        case 'd':
            dictionaries = optarg;
            break;
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
        case 'u':
            unrecognized_char = optarg[0];
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

    using namespace CIF;
    try {
        if (do_verbose == 1) {
            Config::instance().setDebug(true);
            Config::instance().setDebugLevel(100);
        }
        else {
            Config::instance().setDebug(false);
        }

        if (do_dump) {
            Config::instance().setDebug(true);
            Config::instance().setDebugDump(true);
        }

        boost::scoped_array<char> dib(read_file(infilename.c_str()));
        if (!dib)
            return EXIT_FAILURE;

        Puma::instance().setOptionLanguage(langcode);
        Puma::instance().setOptionOneColumn(do_singlecolumn);
        Puma::instance().setOptionFax100(do_fax);
        Puma::instance().setOptionDotMatrix(do_dotmatrix);
        Puma::instance().setOptionUseSpeller(do_speller);
        Puma::instance().setOptionAutoRotate(do_autorotate);

        FormatOptions opt = Puma::instance().formatOptions();
        if (!serif.empty())
            opt.setSerifName(serif);
        if (!sansserif.empty())
            opt.setSansSerifName(sansserif);
        if (!monospace.empty())
            opt.setMonospaceName(monospace);

        if (unrecognized_char)
            opt.setUnrecognizedChar(unrecognized_char);
        if(preserve_line_breaks)
            opt.setPreserveLineBreaks(true);

        //  opt.useBold(true);
        //  opt.useItalic(true);
        //  opt.useFontSize(true);
        //  opt.setFormatMode(puma_format_mode_t t);

        Puma::instance().setFormatOptions(opt);

        if (do_pictures)
            Puma::instance().setOptionPictures(PUMA_PICTURE_ALL);
        else
            Puma::instance().setOptionPictures(PUMA_PICTURE_NONE);

        //  Puma::instance().setOptionTables(puma_table_t mode);

        Puma::instance().open(dib.get());
        Puma::instance().recognize();
        Puma::instance().save(outfilename, outputformat);
        Puma::instance().close();
    }
    catch (std::runtime_error& e) {
        cerr << e.what() << endl;
        return EXIT_FAILURE;
    }
}
