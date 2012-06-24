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

#include <cstdio>
#include <sstream>

#include "c_api.h"
#include "common/recognizeoptions.h"
#include "common/formatoptions.h"
#include "common/language.h"
#include "common/helper.h"
#include "common/imageurl.h"
#include "ced/cedpage.h"
#include "puma/localrecognitionserver.h"
#include "puma/processrecognitionserver.h"
#include "export/exporterfactory.h"
#include "rdib/imageloaderfactory.h"

using namespace cf;

struct NullDeleter { template<typename T> void operator()(T*) {} };

#ifdef __cplusplus
extern "C" {
#endif

struct cf_format_options_ : public FormatOptions {};
struct cf_recognition_options_ : public RecognizeOptions {};

struct cf_page_ {
    CEDPagePtr ptr;
};

void cf_page_destroy(cf_page page) {
    delete page;
}

cf_format_options cf_format_options_create() {
    return new cf_format_options_;
}

void cf_format_options_destroy(cf_format_options opts) {
    delete opts;
}

int cf_format_language(cf_format_options opts) {
    if(!opts)
        return -1;

    return opts->language();
}

int cf_format_write_bom(cf_format_options opts) {
    if(!opts)
        return -1;

    return opts->writeBom() ? 1 : 0;
}

int cf_format_set_language(cf_format_options opts, int value) {
    if(!opts)
        return -1;

    Language lang(static_cast<language_t>(value));
    if(!lang.isValid()) {
        fputs("[Error] Invalid language", stderr);
        return -1;
    }

    opts->setLanguage(lang.get());

    return 0;
}

int cf_format_set_write_bom(cf_format_options opts, int value) {
    if(!opts)
        return -1;

    opts->writeBom(value == 0 ? false : true);

    return 0;
}

cf_recognition_options cf_recognition_options_create() {
    return new cf_recognition_options_;
}

void cf_recognition_options_destroy(cf_recognition_options opts) {
    delete opts;
}

int cf_recognition_dotmatrix(cf_recognition_options opts) {
    if(!opts)
        return -1;
    return opts->dotMatrix() ? 1 : 0;
}

int cf_recognition_fax(cf_recognition_options opts) {
    if(!opts)
        return -1;

    return opts->fax() ? 1 : 0;
}

int cf_recognition_language(cf_recognition_options opts) {
    if(!opts)
        return -1;

    return opts->language();
}

int cf_recognition_search_pictures(cf_recognition_options opts) {
    if(!opts)
        return -1;

    return opts->pictureSearch();
}

int cf_recognition_single_column(cf_recognition_options opts) {
    if(!opts)
        return -1;

    return opts->oneColumn() ? 1 : 0;
}

int cf_recognition_spell_correction(cf_recognition_options opts) {
    if(!opts)
        return -1;

    return opts->spellCorection() ? 1 : 0;
}

int cf_recognition_set_fax(cf_recognition_options opts, int value) {
    if(!opts)
        return -1;

    opts->setFax(value == 0 ? false : true);
    return 0;
}

int cf_recognition_set_language(cf_recognition_options opts, int language) {
    if(!opts)
        return -1;

    Language lang(static_cast<language_t>(language));
    if(!lang.isValid()) {
        fputs("[Error] Invalid language", stderr);
        return -1;
    }

    opts->setLanguage(lang.get());

    return 0;
}

int cf_recognition_set_search_pictures(cf_recognition_options opts, int value) {
    if(!opts)
        return -1;

    opts->setPictureSearch(value == 0 ? false : true);
    return 0;
}

int cf_recognition_set_single_column(cf_recognition_options opts, int value) {
    if(!opts)
        return -1;

    opts->setOneColumn(value == 0 ? false : true);
    return 0;
}

int cf_recognition_set_spell_correction(cf_recognition_options opts, int value) {
    if(!opts)
        return -1;

    opts->setSpellCorrection(value == 0 ? false : true);
    return 0;
}

int cf_recognition_set_dotmatrix(cf_recognition_options opts, int value) {
    if(!opts)
        return -1;

    opts->setDotMatrix(value == 0 ? false : true);
    return 0;
}

cf_page cf_recognize(const char * fname, cf_recognition_options ropts, cf_format_options fopts) {
    if(!fname)
        return NULL;

    RecognizeOptions recognition_opts;
    FormatOptions format_opts;

    if(ropts)
        recognition_opts = *ropts;

    if(fopts)
        format_opts = *fopts;

    try {
        ImagePtr img = ImageLoaderFactory::instance().load(ImageURL(fname));
        if (!img.get())
            return NULL;

        LocalRecognitionServer server;

        cf_page page = new cf_page_;
        page->ptr = server.recognize(img, recognition_opts, format_opts);
        return page;
    }
    catch(std::exception& e) {
        std::cerr << "[Error] " << e.what() << "\n";
        return NULL;
    }
}

cf_page cf_recognize_process(const char * fname, cf_recognition_options ropts, cf_format_options fopts) {
    if(!fname)
        return NULL;
		
#ifdef USE_SHMEM
    RecognizeOptions recognition_opts;
    FormatOptions format_opts;

    if(ropts)
        recognition_opts = *ropts;

    if(fopts)
        format_opts = *fopts;

    try {
        ImagePtr img = ImageLoaderFactory::instance().load(ImageURL(fname));
        if (!img.get())
            return NULL;

        ProcessRecognitionServer server;

        cf_page page = new cf_page_;
        page->ptr = server.recognize(img, recognition_opts, format_opts);
        return page;
    }
    catch(std::exception& e) {
        std::cerr << "[Error] " << e.what() << "\n";
        return NULL;
    }
#else
	return NULL;
#endif
}

static format_t toFormatType(int format) {
    OutputFormat f(static_cast<format_t>(format));
    return f.isValid() ? f.get() : FORMAT_NONE;
}

int cf_export_append(cf_page page, const char * fname, int format, cf_format_options fopts) {
    if(!page) {
        std::cerr << "[Error] null page pointer given\n";
        return -1;
    }

    format_t f = toFormatType(format);
    if(f == FORMAT_NONE) {
        std::cerr << "[Error] invalid output format\n";
        return -1;
    }

    try {
        ExporterPtr exporter = ExporterFactory::instance().make(f);
        ExporterFactory::instance().setPage(page->ptr);

        if(fopts)
            ExporterFactory::instance().setFormatOptions(*fopts);

        exporter->appendTo(fname);
    }
    catch(std::exception& e) {
        std::cerr << "[Error] " << e.what() << std::endl;
        return -1;
    }

    return 0;
}

int cf_export_save(cf_page page, const char * fname, int format, cf_format_options fopts) {
    if(!page) {
        std::cerr << "[Error] null page pointer given\n";
        return -1;
    }

    format_t f = toFormatType(format);
    if(f == FORMAT_NONE) {
        std::cerr << "[Error] invalid output format\n";
        return -1;
    }

    try {
        ExporterPtr exporter = ExporterFactory::instance().make(f);
        exporter->setPage(page->ptr);

        if(fopts)
            exporter->setFormatOptions(*fopts);

        exporter->exportTo(fname);
    }
    catch(std::exception& e) {
        std::cerr << "[Error] " << e.what() << std::endl;
        return -1;
    }

    return 0;
}

int cf_export_save_to_memory(cf_page page, void * dest, size_t * dest_size, int format, cf_format_options fopts) {
    if(!page) {
        std::cerr << "[Error] null page pointer given\n";
        return -1;
    }

    if(!dest) {
        std::cerr << "[Error] null destination given\n";
        return -1;
    }

    if(!dest_size) {
        std::cerr << "[Error] null pointer to destination size given\n";
        return -1;
    }

    format_t f = toFormatType(format);
    if(f == FORMAT_NONE) {
        std::cerr << "[Error] invalid output format\n";
        return -1;
    }

    try {
        ExporterPtr exporter = ExporterFactory::instance().make(f);
        exporter->setPage(page->ptr);

        if(fopts)
            exporter->setFormatOptions(*fopts);

        std::stringstream buf;
        exporter->exportTo(buf);

        size_t stream_size = cf::streamSize(buf);
        if(stream_size > *dest_size) {
            std::cerr << "[Error] destination buffer is too small.\n";
            return -1;
        }

        buf.read(static_cast<char*>(dest), stream_size);
        *dest_size = stream_size;
    }
    catch(std::exception& e) {
        std::cerr << "[Error] " << e.what() << std::endl;
        return -1;
    }

    return 0;
}

const char * cf_export_save_to_str(cf_page page, int format, cf_format_options fopts) {
    static std::string result;
    result.clear();

    if(!page) {
        std::cerr << "[Error] null page pointer given\n";
        return NULL;
    }

    format_t f = toFormatType(format);
    if(f == FORMAT_NONE) {
        std::cerr << "[Error] invalid output format\n";
        return NULL;
    }

    try {
        ExporterPtr exporter = ExporterFactory::instance().make(f);
        exporter->setPage(page->ptr);

        if(fopts)
            exporter->setFormatOptions(*fopts);

        std::stringstream buf;
        exporter->exportTo(buf);
        result = buf.str();
    }
    catch(std::exception& e) {
        std::cerr << "[Error] " << e.what() << std::endl;
        return NULL;
    }

    return result.c_str();
}

int cf_recognition_add_read_rect(cf_recognition_options opts, int left, int top, int width, int height)
{
    if(!opts)
        return -1;

    opts->addReadRect(Rect(left, top, width, height));
    return 0;
}

int cf_recognition_has_read_rects(cf_recognition_options opts)
{
    if(!opts)
        return -1;

    return opts->hasReadRects() ? 1 : 0;
}

int cf_recognition_clear_read_rects(cf_recognition_options opts)
{
    if(opts)
        return -1;

    opts->clearReadRects();
    return 0;
}

#ifdef __cplusplus
}
#endif
