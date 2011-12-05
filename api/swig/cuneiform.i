/* File : cuneiform.i */
%module cuneiform
%{
/* Put headers and other declarations here */
#include "c_api.h"
%}

%rename (format_options) cf_format_options_create;
%rename (recognition_options) cf_recognition_options_create;
%rename (save) cf_export_save;
%rename (destroy_page) cf_page_destroy;
%rename (destroy_format_options) cf_format_options_destroy;
%rename (destroy_recognition_options) cf_recognition_options_destroy;
%rename (recognize) cf_recognize;
%rename (set_language) cf_recognition_set_language;
%include "c_api.h"

