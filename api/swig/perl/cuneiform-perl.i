/* File : cuneiform.i */
%module Cuneiform
%{
/* Put headers and other declarations here */
#include "c_api.h"
typedef unsigned int size_t;
%}

%include "c_api_def.h"

%rename (VERSION) CF_VERSION;
%rename (BUILD_NUMBER) CF_BUILD_NUMBER;
%include "config-version.h"
%include "build_number.h"

/* Format Options */
%inline %{
class FormatOptions {
private:
    cf_format_options opts_;
public:
    FormatOptions() { opts_ = cf_format_options_create();}
    ~FormatOptions() { cf_format_options_destroy(opts_);}
    cf_format_options handle() const { return opts_;}
    int language() const { return cf_format_language(opts_);}
    void setLanguage(int lang) { cf_format_set_language(opts_, lang);} 
    int writeBom() const { return cf_format_write_bom(opts_);}
    void setWriteBom(int value) { cf_format_set_write_bom(opts_, value);}
};

%}

/* Recognition Options */
%inline %{
class RecognitionOptions {
private:
    cf_recognition_options opts;
public:
    RecognitionOptions() { opts = cf_recognition_options_create();}
    ~RecognitionOptions() { cf_recognition_options_destroy(opts);}
    cf_recognition_options handle() const { return opts;}
    int dotMatrix() const { return cf_recognition_dotmatrix(opts);}
    void setDotMatrix(int value) { cf_recognition_set_dotmatrix(opts, value);}
    int fax() const { return cf_recognition_fax(opts);}
    void setFax(int value) { cf_recognition_set_fax(opts, value);}
    int language() const { return cf_recognition_language(opts);}
    void setLanguage(int lang) { cf_recognition_set_language(opts, lang); }
    int searchPictures() const { return cf_recognition_search_pictures(opts);}
    void setSearchPictures(int value) { cf_recognition_set_search_pictures(opts, value);}
    int singleColumn() const { return cf_recognition_single_column(opts);}
    void setSingleColumn(int value) { cf_recognition_set_single_column(opts, value);}
    int spellCorrection() const { return cf_recognition_spell_correction(opts);}
    void setSpellCorrection(int value) { cf_recognition_set_spell_correction(opts, value);}
};

%}

/* Page */
%inline %{
class Page {
private:
    cf_page p_;
    unsigned int * ref_;
    void hold() { (*ref_)++; }
    unsigned int release() { return --(*ref_);}
public:
    Page(cf_page p = 0) : p_(p), ref_(0) { 
        ref_ = new unsigned int(0); 
        hold();
    }

    Page(const Page& page) : p_(page.p_), ref_(page.ref_) { 
       hold(); 
    }

    void operator=(const Page& page) {
        p_ = page.p_;
        ref_ = page.ref_;
        hold();
    }

    ~Page() { 
        if(release() == 0) {
            cf_page_destroy(p_);
            delete ref_;
        }
    }

    int appendTo(const char * fileName, int format, const FormatOptions& fopts = FormatOptions()) 
    {
        return cf_export_append(p_, fileName, format, fopts.handle());
    }

    int save(const char * fileName, int format, const FormatOptions& fopts = FormatOptions()) 
    {
        return cf_export_save(p_, fileName, format, fopts.handle());
    }
};
%}

/* recognition */
%inline %{
Page recognize(const char * fname, const RecognitionOptions& ropts, const FormatOptions& fopts) {
    cf_page p = cf_recognize(fname, ropts.handle(), fopts.handle());
    return Page(p);
} 
%}

