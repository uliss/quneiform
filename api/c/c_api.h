/***************************************************************************
 *   Copyright (C) 2011 by Serge Poltavsky                                 *
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

#ifndef C_API_H
#define C_API_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#else
#include "c_api_def.h"
#endif

struct cf_format_options_;
struct cf_recognition_options_;
struct cf_page_;

typedef struct cf_format_options_ * cf_format_options;
typedef struct cf_recognition_options_ * cf_recognition_options;
typedef struct cf_page_ * cf_page;

// page
void cf_page_destroy(cf_page page);

// recognition options
cf_recognition_options cf_recognition_options_create();
void cf_recognition_options_destroy(cf_recognition_options opts);

int cf_recognition_dotmatrix(cf_recognition_options opts);
int cf_recognition_fax(cf_recognition_options opts);
int cf_recognition_language(cf_recognition_options opts);
int cf_recognition_search_pictures(cf_recognition_options opts);
int cf_recognition_single_column(cf_recognition_options opts);
int cf_recognition_spell_correction(cf_recognition_options opts);

int cf_recognition_set_dotmatrix(cf_recognition_options opts, int value);
int cf_recognition_set_fax(cf_recognition_options opts, int value);
int cf_recognition_set_language(cf_recognition_options opts, int language);
int cf_recognition_set_search_pictures(cf_recognition_options opts, int value);
int cf_recognition_set_single_column(cf_recognition_options opts, int value);
int cf_recognition_set_spell_correction(cf_recognition_options opts, int value);

// format options
cf_format_options cf_format_options_create();
void cf_format_options_destroy(cf_format_options opts);

int cf_format_language(cf_format_options opts);
int cf_format_write_bom(cf_format_options opts);

int cf_format_set_language(cf_format_options opts, int value);
int cf_format_set_write_bom(cf_format_options opts, int value);

// recognize
cf_page cf_recognize(const char * fname, cf_recognition_options ropts, cf_format_options fopts);

// export
int cf_export_append(cf_page page, const char * fname, int format, cf_format_options fopts);
int cf_export_save(cf_page page, const char * fname, int format, cf_format_options fopts);
int cf_export_save_to_memory(cf_page page, void * dest, size_t * dest_size, int format, cf_format_options fopts);

#ifdef __cplusplus
}
#endif

#endif // C_API_H
