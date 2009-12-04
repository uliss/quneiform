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

#include "pumadef.h"
#include "cfcompat.h"
#include "formatoptions.h"
#include "common/rect.h"
#include "common/memorybuffer.h"
#include "cimage/imageinfo.h"

class CTIControl;

namespace CIF
{

struct PumaException: std::runtime_error {
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
        void open(char * dib);
        Rect pageTemplate() const;
        void recognize();
        void save(const std::string& outputFilename, int format) const;
        void save(void * dest, size_t size, int format) const;
        void setFormatOptions(const FormatOptions& opt);
        void setOptionAutoRotate(bool val);
        void setOptionDotMatrix(bool val);
        void setOptionFax100(bool val);
        void setOptionFormatMode(puma_format_mode_t format);
        void setOptionLanguage(language_t language);
        void setOptionOneColumn(bool val);
        void setOptionPictures(puma_picture_t type);
        void setOptionPreserveLineBreaks(bool value);
        void setOptionTable(puma_table_t mode);
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
        void preprocessImage();
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
        Rect rect_template_;
        bool do_spell_corretion_;
        bool fax100_;
        bool one_column_;
        bool dot_matrix_;
        bool auto_rotate_;
        bool preserve_line_breaks_;
        language_t language_;
        std::string user_dict_name_;
        std::string input_filename_;
        std::string layout_filename_;
        FormatOptions format_options_;
        puma_picture_t pictures_;
        puma_table_t tables_;
        void * input_dib_;
        void * recog_dib_;
        int tables_num_;
        Handle ccom_;
        Handle cpage_;
        Handle lines_ccom_;
        Handle cline_;
        Handle ed_page_;
        Bool32 rc_line_;
        Bool32 kill_vsl_components_;
        Bool32 need_clean_line_;
        const char * recog_name_;
};

}

#endif /* PUMAIMPL_H_ */
