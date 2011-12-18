/* File : cuneiform.i */
%module cuneiform
%{
/* Put headers and other declarations here */
#include "c_api.h"
typedef unsigned int size_t;
%}

#define CF_LANGUAGE_ENGLISH     0
#define CF_LANGUAGE_GERMAN      1
#define CF_LANGUAGE_FRENCH      2
#define CF_LANGUAGE_RUSSIAN     3
#define CF_LANGUAGE_SWEDISH     4
#define CF_LANGUAGE_SPANISH     5
#define CF_LANGUAGE_ITALIAN     6
#define CF_LANGUAGE_RUS_ENG     7
#define CF_LANGUAGE_UKRAINIAN   8
#define CF_LANGUAGE_SERBIAN     9
#define CF_LANGUAGE_CROATIAN    10
#define CF_LANGUAGE_POLISH      11
#define CF_LANGUAGE_DANISH      12
#define CF_LANGUAGE_PORTUGUESE  13
#define CF_LANGUAGE_DUTCH       14
#define CF_LANGUAGE_CZECH       19
#define CF_LANGUAGE_ROMANIAN    20
#define CF_LANGUAGE_HUNGARIAN   21
#define CF_LANGUAGE_BULGARIAN   22
#define CF_LANGUAGE_SLOVENIAN   23
#define CF_LANGUAGE_LATVIAN     24
#define CF_LANGUAGE_LITHUANIAN  25
#define CF_LANGUAGE_ESTONIAN    26
#define CF_LANGUAGE_TURKISH     27
#define CF_LANGUAGE_BELARUSIAN  28

#define CF_FORMAT_TEXT       0
#define CF_FORMAT_SMARTTEXT  1
#define CF_FORMAT_HTML       2
#define CF_FORMAT_HOCR       3
#define CF_FORMAT_DEBUG      4
#define CF_FORMAT_ODF        5
#define CF_FORMAT_SUMMARY    6
#define CF_FORMAT_DJVUXML    7
#define CF_FORMAT_DJVUTXT    8
#define CF_FORMAT_NATIVE_TXT 9
#define CF_FORMAT_NATIVE_XML 10
#define CF_FORMAT_FB2        11

%include "config-version.h"
%include "build_number.h"

/* Format Options */
%inline %{
class cuneiform_format_options {
private:
    cf_format_options opts_;
public:
    cuneiform_format_options() { opts_ = cf_format_options_create();}
    ~cuneiform_format_options() { cf_format_options_destroy(opts_);}
    cf_format_options handle() const { return opts_;}
    int language() const { return cf_format_language(opts_);}
    void set_language(int lang) { cf_format_set_language(opts_, lang);}
    int write_bom() const { return cf_format_write_bom(opts_);}
    void set_write_bom(int value) { cf_format_set_write_bom(opts_, value);}
};
%}

/* Recognition Options */
%inline %{
class cuneiform_recognition_options {
private:
    cf_recognition_options opts;
public:
    cuneiform_recognition_options() { opts = cf_recognition_options_create();}
    ~cuneiform_recognition_options() { cf_recognition_options_destroy(opts);}
    cf_recognition_options handle() const { return opts;}
    int dot_matrix() const { return cf_recognition_dotmatrix(opts);}
    void set_dot_matrix(int value) { cf_recognition_set_dotmatrix(opts, value);}
    int fax() const { return cf_recognition_fax(opts);}
    void set_fax(int value) { cf_recognition_set_fax(opts, value);}
    int language() const { return cf_recognition_language(opts);}
    void set_language(int lang) { cf_recognition_set_language(opts, lang); }
    int search_pictures() const { return cf_recognition_search_pictures(opts);}
    void set_search_pictures(int value) { cf_recognition_set_search_pictures(opts, value);}
    int single_column() const { return cf_recognition_single_column(opts);}
    void set_single_column(int value) { cf_recognition_set_single_column(opts, value);}
    int spell_correction() const { return cf_recognition_spell_correction(opts);}
    void set_spell_correction(int value) { cf_recognition_set_spell_correction(opts, value);}
};
%}

/* Page */
%inline %{
class cuneiform_page {
private:
    cf_page p_;
    unsigned int * ref_;
    void hold() { (*ref_)++; }
    unsigned int release() { return --(*ref_);}
public:
    cuneiform_page(cf_page p = 0) : p_(p), ref_(0) {
        ref_ = new unsigned int(0); 
        hold();
    }

    cuneiform_page(const cuneiform_page& page) : p_(page.p_), ref_(page.ref_) {
       hold(); 
    }

    void operator=(const cuneiform_page& page) {
        p_ = page.p_;
        ref_ = page.ref_;
        hold();
    }

    ~cuneiform_page() {
        if(release() == 0) {
            cf_page_destroy(p_);
            delete ref_;
        }
    }

    int append_to(const char * fileName,
                 int format,
                 const cuneiform_format_options& fopts = cuneiform_format_options())
    {
        return cf_export_append(p_, fileName, format, fopts.handle());
    }

    int save(const char * fileName,
             int format,
             const cuneiform_format_options& fopts = cuneiform_format_options())
    {
        return cf_export_save(p_, fileName, format, fopts.handle());
    }

    const char * str(int format, const cuneiform_format_options& fopts = cuneiform_format_options())
    {
        return cf_export_save_to_str(p_, format, fopts.handle());
    }
};
%}

/* recognition */
%inline %{
cuneiform_page recognize(const char * fname,
                            const cuneiform_recognition_options& ropts,
                            const cuneiform_format_options& fopts) {
    cf_page p = cf_recognize(fname, ropts.handle(), fopts.handle());
    return cuneiform_page(p);
} 
%}

