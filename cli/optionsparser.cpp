/***************************************************************************
 *   Copyright (C) 2011 by Serge Poltavski                                 *
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

#include <iomanip>
#include <stdexcept>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <getopt.h>

#include "optionsparser.h"
#include "options.h"
#include "common/cifconfig.h"
#include "common/language.h"
#include "common/outputformat.h"

namespace cf
{

// recognition options
static int do_dotmatrix = FALSE;
static int do_fax = FALSE;
static int do_pictures = TRUE;
static int do_singlecolumn = FALSE;
static int do_speller = FALSE;
static int do_tables = TRUE;
static language_t langcode = LANGUAGE_ENGLISH;
static std::string page_template;
static Rect page_template_r;

// format options
static std::string monospace;
static int preserve_line_breaks = FALSE;
static std::string serif;
static std::string sansserif;
static int show_alternatives = FALSE;
static int test_output = FALSE;
static int use_bold = TRUE;
static int use_italic = TRUE;
static int use_font_size = TRUE;
static int use_underlined = TRUE;
static char unrecognized_char = 0;
static int write_bom = FALSE;
static int write_meta_generator = TRUE;

// cli options
static int do_append = FALSE;
static int do_dump = FALSE;
static int do_verbose = FALSE;
static int print_image_formats = FALSE;
static int do_progress = FALSE;

// in/out options
static int stdout_output = FALSE;
static format_t outputformat = FORMAT_TEXT;
static std::string output_image_dir;
static std::string outfilename;
static std::string infilename;

const static int MONOSPACE_FLAG = 1024;
const static int SERIF_FLAG = 1025;
const static int SANS_SERIF_FLAG = 1026;
const static int IMAGE_OUTPUT_DIR_FLAG = 1027;
const static int PAGE_TEMPLATE_FLAG = 1028;

static const char * const short_options = ":abho:pvVl:f:u:";
static const struct option long_options[] = {
    //
    { "append", no_argument, &do_append, 'a' },//
    { "bom", no_argument, &write_bom, 1 }, //
    { "debug-dump", no_argument, &do_dump, 1 },//
    { "dotmatrix", no_argument, &do_dotmatrix, 1 },//
    { "fax", no_argument, &do_fax, 1 },//
    { "format", required_argument, NULL, 'f' },//
    { "help", no_argument, NULL, 'h' },//
    { "language", required_argument, NULL, 'l' },//
    { "monospace", required_argument, NULL, MONOSPACE_FLAG }, //
    { "no-bold", no_argument, &use_bold, 0 }, //
    { "no-bom", no_argument, &write_bom, 0 }, //
    { "no-italic", no_argument, &use_italic, 0 },//
    { "no-meta-generator", no_argument, &write_meta_generator, 0},//
    { "no-font-size", no_argument, &use_font_size, 0 }, //
    { "nopictures", no_argument, &do_pictures, 0 },//
    { "output", required_argument, NULL, 'o' },//
    { "output-image-dir", required_argument, NULL, IMAGE_OUTPUT_DIR_FLAG },//
    { "page-template", required_argument, NULL, PAGE_TEMPLATE_FLAG }, //
    { "pictures", no_argument, &do_pictures, 1 },//
    { "preserve-line-breaks", no_argument, &preserve_line_breaks, 1 },//
    { "progress", no_argument, &do_progress, 'p' }, //
    { "sans-serif", required_argument, NULL, SANS_SERIF_FLAG }, //
    { "serif", required_argument, NULL, SERIF_FLAG },//
    { "show-alternatives", no_argument, &show_alternatives, 1 }, //
    { "stdout", no_argument, &stdout_output, 1 }, //
    { "supported-image-formats", no_argument, &print_image_formats, 1 }, //
    { "onecolumn", no_argument, &do_singlecolumn, 1 },//
    { "spell", no_argument, &do_speller, 1 },//
    { "tables", required_argument, &do_tables, 1 },//
    { "test-output", no_argument, &test_output, 1},//
    { "unrecognized", required_argument, NULL, 'u' },//
    { "verbose", no_argument, &do_verbose, 1 },//
    { "version", no_argument, NULL, 'V' },//
    { NULL, 0, NULL, 0 } };

static std::string defaultOutputName(format_t format) {
    std::string extension = OutputFormat::extension(format);
    if (extension.empty())
        throw std::runtime_error("Invalid format");
    return "cuneiform-out." + extension;
}

static language_t processLangOptions(const char *optarg) {
    if (strcmp(optarg, "help") == 0) {
        printLanguages(std::cout);
        throw OptionsParser::ExitException(EXIT_SUCCESS);
    }

    // first try 2-character iso language code
    Language lang = Language::byCode2(optarg);

    // if fail try 3-character iso language code
    if (!lang.isValid())
        lang = Language::byCode3(optarg);

    // if still not found try get language by name
    if (!lang.isValid())
        lang = Language::byName(optarg);

    // nothing found - error message
    if (!lang.isValid()) {
        std::cerr << "[Error] Unknown language: " << optarg << "\n";
        printLanguages(std::cerr);
        throw OptionsParser::ExitException(EXIT_FAILURE);
    }

    return lang.get();
}

static format_t processFormatOptions(const char * optarg) {
    if (strcmp(optarg, "help") == 0) {
        printSupportedFormats(std::cout);
        throw OptionsParser::ExitException(EXIT_SUCCESS);
    }

    OutputFormat format = OutputFormat::byName(optarg);

    if (format.isValid()) {
        return format.get();
    } else {
        std::cerr << "[Error] Unknown output format: " << optarg << "\n";
        printSupportedFormats(std::cerr);
        throw OptionsParser::ExitException(EXIT_FAILURE);
    }
}

static bool isLangOption(const char * opt) {
    return (strcmp(opt, "-l") == 0) || (strcmp(opt, "--language") == 0);
}

static bool isFormatOption(const char * opt) {
    return (strcmp(opt, "-f") == 0) || (strcmp(opt, "--format") == 0);
}

static Rect parsePageTemplate(const std::string& str)
{
    int x = 0;
    int y = 0;
    int w = 0;
    int h = 0;
    int n = sscanf(str.c_str(), "%d,%d,%d,%d", &x, &y, &w, &h);

    if(n == 4)
        return Rect(Point(x, y), w, h);
    else if(n == 2)
        return Rect(Point(), x, y);
    else
        return Rect();
}

OptionsParser::OptionsParser() {}

void OptionsParser::getoptParse(int argc, char **argv)
{
    int code = 0;
    int longopt_index = -1;

    // disable error reporting
    opterr = 0;
    optind = 0;
    while ((code = getopt_long(argc, argv, short_options, long_options, &longopt_index)) != -1) {
        switch (code) {
        case 'a':
            do_append = TRUE;
            break;
        case 'b':
            write_bom = TRUE;
            break;
        case 'f':
            outputformat = processFormatOptions(optarg);
            break;
        case 'h':
            printUsage(argv[0]);
            throw OptionsParser::ExitException(EXIT_SUCCESS);
            break;
        case IMAGE_OUTPUT_DIR_FLAG:
            output_image_dir = optarg;
            break;
        case PAGE_TEMPLATE_FLAG:
            page_template = optarg;
            break;
        case 'l':
            langcode = processLangOptions(optarg);
            break;
        case 'o':
            outfilename = optarg;
            break;
        case 'p':
            do_progress = TRUE;
            break;
        case 'u':
            unrecognized_char = optarg[0];
            break;
        case 'v':
            do_verbose = TRUE;
            break;
        case 'V':
            printVersion(std::cout);
            throw OptionsParser::ExitException(EXIT_SUCCESS);
            break;
        case MONOSPACE_FLAG:
            monospace = optarg;
            break;
        case SANS_SERIF_FLAG:
            sansserif = optarg;
            break;
        case SERIF_FLAG:
            serif = optarg;
            break;
        case 0:
            /*getopt_long() set option, just continue*/
            break;
        case ':':
            std::cerr << "[Error] option '" << argv[optind - 1]
                      << "' requires an argument\n";
            if(isLangOption(argv[optind - 1])) {
                printLanguages(std::cerr);
                throw OptionsParser::ExitException(EXIT_FAILURE);
            }
            else if(isFormatOption(argv[optind - 1])) {
                printSupportedFormats(std::cerr);
                throw OptionsParser::ExitException(EXIT_FAILURE);
            }
            break;
        case '?':
        default:
            std::cerr << "[Error] option '" << argv[optind - 1]
                      << "' is invalid: ignored\n";
            break;
        }

        // reset index
        longopt_index = -1;
    }

    if(print_image_formats) {
        printSupportedInputImageFormats(std::cout);
        throw OptionsParser::ExitException(EXIT_SUCCESS);
    }

    if (optind == argc) {
        std::cerr << "[Error] Input file not specified\n";
        printUsage(argv[0]);
        throw OptionsParser::ExitException(EXIT_FAILURE);
    } else
        infilename = argv[optind];

    if (outfilename.empty())
        outfilename = defaultOutputName(outputformat);

    if(!page_template.empty()) {
        Rect r = parsePageTemplate(page_template);
        if(r.perimeter() > 0)
            page_template_r = r;
    }
}

void OptionsParser::parse(int argc, char **argv) {
    getoptParse(argc, argv);
    updateFormatOptions();
    updateRecognizeOptions();
    updateCliOptions();
    updateDebugOptions();
}

void OptionsParser::print(std::ostream& os) {
    os << std::string(60, '#') << "\n";
    os << format_opts_;
    os << recognize_opts_;
    os << std::string(60, '#');
    os << std::endl;
}

void OptionsParser::printUsage(const char * program) {
    std::cout << "Usage: " << program << " [OPTIONS] FILE\n\n";
    cli_opts_.printOptions(std::cout);
    printRecognizeOptions(std::cout);
    printFormatOptions(std::cout);
}

void OptionsParser::printFormatOptions(std::ostream& os) {
    os << "Format options:\n";
    printOption(os, "--bom", "-b", "Writes BOM (byte order mark) in the beginning of document.");
    printOption(os, "--no-bom", "", "Do not write BOM.");
    printOption(os, "--no-bold", "", "Use normal font for bold text.");
    printOption(os, "--no-font-size", "", "Do not use font size.");
    printOption(os, "--no-italic", "", "Use normal font for italic text.");
    printOption(os, "--no-meta-generator", "",
                "Do not write meta generator info.");
    printOption(os, "--output-image-dir PATH", "", "Sets  image  output  directory.");
    printOption(os, "--preserve-line-breaks", "", "Preserves line-breaking.");
    printOption(os, "--show-alternatives", "", "Show alternatives of recognition (now HTML only).");
    printOption(os,
                "--unrecognized CHAR", "-u",
                "Set symbol, that shown instead of unrecognized characters.\n"
                "    Default is '~'.");

    printOption(os, "--monospace  FONT", "", "Use specified monospace font.");
    printOption(os, "--serif      FONT", "", "Use specified serif font.");
    printOption(os, "--sans-serif FONT", "", "Use specified sans-serif font.");
}

void OptionsParser::printRecognizeOptions(std::ostream& os) {
    os << "Recognition options:\n";
    printOption(os, "--onecolumn", "", "Use one column layout.");
    printOption(os, "--dotmatrix", "",
                "Use recognition mode optimized for text printed with a dot matrix printer.");
    printOption(os, "--fax", "",
                "Use recognition mode optimized for text that has been faxed.");
    printOption(os, "--page-template RECT", "", "Sets recognition area [X,Y,WIDTH,HEIGHT].");
    printOption(os, "--pictures", "", "Search pictures (default).");
    printOption(os, "--nopictures", "", "Do not search pictures.");

    printOption(os, "--language LANGUAGE", "-l",
                "Set recognition language. LANGUAGE is ISO code or language name.\n"
                "    Type --language help to gel full list.");
    printOption(os, "--spell", "", "Use spell correction.");
}

CliOptions OptionsParser::cliOptions() const {
    return cli_opts_;
}

FormatOptions OptionsParser::formatOptions() const {
    return format_opts_;
}

RecognizeOptions OptionsParser::recognizeOptions() const {
    return recognize_opts_;
}

void OptionsParser::updateCliOptions() {
    cli_opts_.setInputFilename(infilename);
    if(stdout_output) {
        cli_opts_.setStdOut(stdout_output);
    }
    else {
        if(outfilename.empty())
            outfilename = defaultOutputName(outputformat);
        cli_opts_.setOutputFilename(outfilename);
    }

    cli_opts_.setAppend(do_append);
    cli_opts_.setOutputFormat(outputformat);
    cli_opts_.setOutputImageDir(output_image_dir);
    cli_opts_.setShowProgress(do_progress != FALSE ? true : false);
}

void OptionsParser::updateDebugOptions() {
    if (do_verbose == 1) {
        print(std::cerr);
        Config::instance().setDebug(true);
        Config::instance().setDebugLevel(100);
    } else {
        Config::instance().setDebug(false);
    }

    if(do_dump) {
        Config::instance().setDebug(true);
        Config::instance().setDebugDump(true);
    }
}

void OptionsParser::updateFormatOptions() {
    format_opts_.writeBom(write_bom);
    format_opts_.writeMetaGenerator(write_meta_generator);
    format_opts_.useBold(use_bold);
    format_opts_.useItalic(use_italic);
    format_opts_.useUnderlined(use_underlined);
    format_opts_.useFontSize(use_font_size);
    format_opts_.setShowAlternatives(show_alternatives);

    if(unrecognized_char)
        format_opts_.setUnrecognizedChar(unrecognized_char);

    if(!serif.empty())
        format_opts_.setSerifName(serif);
    if(!monospace.empty())
        format_opts_.setMonospaceName(monospace);
    if(!sansserif.empty())
        format_opts_.setSansSerifName(sansserif);

    if(langcode != LANGUAGE_UNKNOWN)
        format_opts_.setLanguage(langcode);

    format_opts_.setPreserveLineBreaks(preserve_line_breaks);
    format_opts_.setTestOutput(test_output);
}

void OptionsParser::updateRecognizeOptions() {
    if(langcode != LANGUAGE_UNKNOWN)
        recognize_opts_.setLanguage(langcode);
    recognize_opts_.setFax(do_fax);
    recognize_opts_.setDotMatrix(do_dotmatrix);
    recognize_opts_.setSpellCorrection(do_speller);
    recognize_opts_.setOneColumn(do_singlecolumn);
    recognize_opts_.setPictureSearch(do_pictures);

    if(page_template_r.perimeter() > 0)
        recognize_opts_.setPageTemplate(page_template_r);
}

}
