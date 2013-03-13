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
#include "serialize.h"

namespace cf {

class CLA_EXPO FormatOptions
{
    public:
        FormatOptions();
        virtual ~FormatOptions();

        enum format_mode_t
        {
            // no formatting
            FORMAT_NONE = 0x0040,
            // format output into frames and columns
            FORMAT_ALL = 0x0001,
            // format output into frames only
            FORMAT_ONLY_FRAME = 0x0002
        };

        bool operator==(const FormatOptions& fopts) const;
        bool operator!=(const FormatOptions& fopts) const;

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
        bool writeBom() const;
        void writeBom(bool value);
        bool writeMetaGenerator() const;
        void writeMetaGenerator(bool value);
    private:
#ifdef CF_SERIALIZE
        friend class boost::serialization::access;
        template<class Archive>
        void serialize(Archive & ar, const unsigned int /*version*/) {
            using boost::serialization::make_nvp;
            ar & make_nvp("serif", serif_name_);
            ar & make_nvp("sans-serif", sans_serif_name_);
            ar & make_nvp("monospace", monospace_name_);
            ar & make_nvp("format-mode", format_mode_);
            ar & make_nvp("unrecognized-char", unrecognized_char_);
            ar & make_nvp("language", language_);
            ar & make_nvp("image-format", image_format_);
            ar & make_nvp("flags", flags_);
        }
#endif
    private:
        bool hasFlag(uint flag) const  { return flags_ & flag; }
        void setFlag(uint flag)        { flags_ |= flag; }
        void setFlag(uint flag, bool value) { value ? setFlag(flag) : unsetFlag(flag); }
        void unsetFlag(uint flag) { flags_ &= (~flag); }
    private:
        std::string serif_name_;
        std::string sans_serif_name_;
        std::string monospace_name_;
        format_mode_t format_mode_;
        wchar_t unrecognized_char_;
        language_t language_;
        image_format_t image_format_;
        size_t flags_;
};

FUN_EXPO__ std::ostream& operator<<(std::ostream& os, const FormatOptions& fmt);

}

#endif /* FORMATOPTIONS_H_ */
