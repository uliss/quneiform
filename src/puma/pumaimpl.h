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

#ifndef PUMAIMPL_H_
#define PUMAIMPL_H_

#include <string>
#include <iosfwd>
#include <stdexcept>
#include <memory>
#include <set>

#include "pumadef.h"
#include "cfcompat.h"
#include "formatoptions.h"
#include "layoutoptions.h"
#include "rect.h"
#include "cimage/imageinfo.h"
#include "lang_def.h"

class CTIControl;

namespace CIF {

class RStuff;
class Rsl;
class RMarker;

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

    void addUserDictionary(const std::string& name);
    void close();
    FormatOptions formatOptions() const;
    LayoutOptions layoutOptions() const;
    void open(char * dib);
    void recognize();
    void removeUserDictionary(const std::string& name);
    void save(const std::string& outputFilename, int format) const;
    void save(void * dest, size_t size, int format) const;
    void setFormatOptions(const FormatOptions& opt);
    void setLayoutOptions(const LayoutOptions& opt);
    void setOptionFax100(bool val);
    void setOptionLanguage(language_t language);
    void setOptionUserDictionaryName(const char * name);
    void setOptionUseSpeller(bool value = true);
    void setPageTemplate(const Rect& r);
private:
    void addUserDictionary_(const std::string& name);
    void binarizeImage();
    void clearAll();
    void extractStrings();
    void formatResult();
    void layout();
    void layoutRsl();
    void layoutRStuff();
    void layoutRMarker();
    void loadLayoutFromFile(const std::string& fname);
    void modulesDone();
    void modulesInit();
    const char * modulePath() const;
    const char * moduleTmpPath() const;
    void normalize();
    void pass1();
    void pass2();
    void preOpenInitialize();
    void printRecognizeOptions() const;
    void printResult(std::ostream& os);
    void printResultLine(std::ostream& os, size_t lineNumber);
    void postOpenInitialize();
    void recognizeCorrection();
    void recognizePass1();
    void recognizePass2();
    void recognizeSetup();
    void rotate(void * dib, Point * p);
    void rout(const std::string& fname, int Format) const;
    void rout(void * dest, size_t size, int format) const;
    void saveCSTR(int pass);
    void saveLayoutToFile(const std::string& fname);
    void setData(RSPreProcessImage& data);
    void spellCorrection();
private:
    std::auto_ptr<CTIControl> cimage_;
    std::auto_ptr<RStuff> rstuff_;
    std::auto_ptr<Rsl> rsl_;
    std::auto_ptr<RMarker> rmarker_;
    BitmapInfoHeader info_;
    bool do_spell_correction_;
    bool fax100_;
    language_t language_;
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
    std::string user_dict_name_;
    typedef std::set<std::string> DictContainer;
    DictContainer user_dict_;
};

}

#endif /* PUMAIMPL_H_ */
