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

#ifndef FORMATOPTIONS_H_
#define FORMATOPTIONS_H_

#include <string>
#include <iostream>
#include <cstddef>

#include "lang_def.h"
#include "globus.h"
#include "imageformats.h"

namespace cf {

class CLA_EXPO FormatOptions
{
    public:
        FormatOptions();

        enum format_mode_t
        {
            // no formatting
            FORMAT_NONE = 0x0040,
            // format output into frames and columns
            FORMAT_ALL = 0x0001,
            // format output into frames only
            FORMAT_ONLY_FRAME = 0x0002
        };

        bool bomWritten() const;
        format_mode_t formatMode() const;
        image_format_t imageExportFormat() const;
        bool isBoldUsed() const;
        bool isFontSizeUsed() const;
        bool isItalicUsed() const;
        bool isStylesUsed() const;
        bool isTestOutput() const;
        bool isUnderlinedUsed() const;
        language_t language() const;
        std::string monospaceName() const;
        bool preserveLineBreaks() const;
        std::string sansSerifName() const;
        std::string serifName() const;
        void setFormatMode(format_mode_t format);
        void setImageExportFormat(image_format_t format);
        void setLanguage(language_t lang);
        void setMonospaceName(const std::string& name);
        void setPreserveLineBreaks(bool val = true);
        void setSansSerifName(const std::string& name);
        void setSerifName(const std::string& name);
        void setShowAlternatives(bool value);
        void setTestOutput(bool value);
        void setUnrecognizedChar(wchar_t ch);
        void setUnrecognizedChar(char ch);
        bool showAlternatives() const;
        wchar_t unrecognizedChar() const;
        void useBold(bool val = true);
        void useFontSize(bool val = true);
        void useItalic(bool val = true);
        void useStyles(bool val = true);
        void useUnderlined(bool val = true);
        void writeBom(bool value);
        bool writeMetaGenerator() const;
        void writeMetaGenerator(bool value);
    private:
        std::string serif_name_;
        std::string sans_serif_name_;
        std::string monospace_name_;
        bool use_bold_;
        bool use_italic_;
        bool use_styles_;
        bool use_underlined_;
        bool use_font_size_;
        bool preserve_line_breaks_;
        format_mode_t format_mode_;
        wchar_t unrecognized_char_;
        language_t language_;
        image_format_t image_format_;
        bool preserve_line_hyphens_;
        bool show_alternatives_;
        bool write_bom_;
        bool write_meta_generator_;
        bool test_output_;
};
FUN_EXPO__ std::ostream& operator<<(std::ostream& os, const FormatOptions& fmt);

}

#endif /* FORMATOPTIONS_H_ */
