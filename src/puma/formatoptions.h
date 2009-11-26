/*
 * formatoptions.h
 *
 *  Created on: 18.10.2009
 *      Author: uliss
 */

#ifndef FORMATOPTIONS_H_
#define FORMATOPTIONS_H_

#include <string>
#include <iostream>
#include <cstddef>

#include "pumadef.h"
#include "lang_def.h"

namespace CIF
{

class FormatOptions
{
    public:
        FormatOptions();
        virtual ~FormatOptions();

        puma_format_mode_t formatMode() const;
        bool isBoldUsed() const;
        bool isFontSizeUsed() const;
        bool isItalicUsed() const;
        language_t language() const;
        std::string monospaceName() const;
        std::string sansSerifName() const;
        std::string serifName() const;
        void setFormatMode(puma_format_mode_t format);
        void setLanguage(language_t lang);
        void setMonospaceName(const std::string& name);
        void setSansSerifName(const std::string& name);
        void setSerifName(const std::string& name);
        void setUnrecognizedChar(wchar_t ch);
        void setUnrecognizedChar(char ch);
        wchar_t unrecognizedChar() const;
        void useBold(bool val = true);
        void useFontSize(bool val = true);
        void useItalic(bool val = true);
    private:
        std::string serif_name_;
        std::string sans_serif_name_;
        std::string monospace_name_;
        bool use_bold_;
        bool use_italic_;
        bool use_font_size_;
        puma_format_mode_t format_mode_;
        wchar_t unrecognized_char_;
        language_t language_;
};

std::ostream& operator<<(std::ostream& os, const FormatOptions& fmt);

}

#endif /* FORMATOPTIONS_H_ */
