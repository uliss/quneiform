/*
 * pumaimpl.h
 *
 *  Created on: 22.09.2009
 *      Author: uliss
 */

#ifndef PUMAIMPL_H_
#define PUMAIMPL_H_

#include <string>
#include <iosfwd>
#include <stdexcept>
#include <memory>

#include "pumadef.h"
#include "cfcompat.h"
#include "formatoptions.h"
#include "layoutoptions.h"
#include "rect.h"
#include "memorybuffer.h"
#include "cimage/imageinfo.h"

class CTIControl;

namespace CIF {

struct PumaException: std::runtime_error
{
    PumaException(const std::string& msg) :
        std::runtime_error(msg) {
    }
};

class PumaImpl
{
public:
    PumaImpl();
    ~PumaImpl();

    void close();
    FormatOptions formatOptions() const;
    LayoutOptions layoutOptions() const;
    void open(char * dib);
    void recognize();
    void save(const std::string& outputFilename, int format) const;
    void save(void * dest, size_t size, int format) const;
    void setFormatOptions(const FormatOptions& opt);
    void setLayoutOptions(const LayoutOptions& opt);
    void setOptionFax100(bool val);
    void setOptionLanguage(language_t language);
    void setOptionUserDictionaryName(const char * name);
    void setOptionUseSpeller(bool value = true);
    void setPageTemplate(const Rect& r);
public:
    static unsigned char * mainBuffer();
    static unsigned char * workBuffer();
    static const size_t MainBufferSize = 500000;
    static const size_t WorkBufferSize = 180000;
private:
    void binarizeImage();
    void clearAll();
    void extractComponents();
    void extractStrings();
    void formatResult();
    void layout();
    void loadLayoutFromFile(const std::string& fname);
    void modulesDone();
    void modulesInit();
    const char * modulePath() const;
    const char * moduleTmpPath() const;
    void normalize();
    void pass1();
    void pass2();
    void preOpenInitialize();
    void printResult(std::ostream& os);
    void printResultLine(std::ostream& os, size_t lineNumber);
    void postOpenInitialize();
    void recognizeCorrection();
    void recognizePass1();
    void recognizePass2();
    void recognizeSetup(int lang);
    void rotate(void * dib, Point * p);
    void rout(const std::string& fname, int Format) const;
    void rout(void * dest, size_t size, int format) const;
    void saveCSTR(int pass);
    void saveLayoutToFile(const std::string& fname);
    void saveToText(std::ostream& os) const;
    void saveToText(const std::string& filename) const;
    void spellCorrection();
private:
    static FixedBuffer<unsigned char, MainBufferSize> main_buffer_;
    static FixedBuffer<unsigned char, WorkBufferSize> work_buffer_;
private:
    std::auto_ptr<CTIControl> cimage_;
    BitmapInfoHeader info_;
    bool do_spell_corretion_;
    bool preserve_line_breaks_;
    bool fax100_;
    language_t language_;
    std::string user_dict_name_;
    std::string input_filename_;
    std::string recog_name_;
    FormatOptions format_options_;
    LayoutOptions layout_options_;
    void * input_dib_;
    void * recog_dib_;
    Handle ccom_;
    Handle cpage_;
    Handle lines_ccom_;
    Handle cline_;
    Handle ed_page_;
    uchar special_project_;
    char special_global_buf_[64000];
    size_t special_global_buf_len_;
    bool kill_vsl_components_;
};

}

#endif /* PUMAIMPL_H_ */
