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
#include <iomanip>

#include "formatoptions.h"
#include "common/language.h"

namespace cf
{

FormatOptions::FormatOptions() :
    serif_name_("Times New Roman"),
    sans_serif_name_("Arial"),
    monospace_name_("Courier New"),
    use_bold_(true),
    use_italic_(true),
    use_styles_(true),
    use_underlined_(true),
    use_font_size_(true),
    preserve_line_breaks_(false),
    format_mode_(FormatOptions::FORMAT_ALL),
    unrecognized_char_('~'),
    language_(LANGUAGE_RUS_ENG),
    image_format_(FORMAT_UNKNOWN),
    preserve_line_hyphens_(false),
    show_alternatives_(false),
#ifdef __APPLE__
    write_bom_(true)
#else
    write_bom_(false)
#endif
{
}

bool FormatOptions::bomWritten() const {
    return write_bom_;
}

FormatOptions::format_mode_t FormatOptions::formatMode() const {
    return format_mode_;
}

image_format_t FormatOptions::imageExportFormat() const {
    return image_format_;
}

bool FormatOptions::isBoldUsed() const {
    return use_bold_;
}

bool FormatOptions::isFontSizeUsed() const {
    return use_font_size_;
}

bool FormatOptions::isItalicUsed() const {
    return use_italic_;
}

bool FormatOptions::isStylesUsed() const {
    return use_styles_;
}

bool FormatOptions::isUnderlinedUsed() const {
    return use_underlined_;
}

language_t FormatOptions::language() const {
    return language_;
}

std::string FormatOptions::monospaceName() const {
    return monospace_name_;
}

bool FormatOptions::preserveLineBreaks() const {
    return preserve_line_breaks_;
}

std::string FormatOptions::sansSerifName() const {
    return sans_serif_name_;
}

std::string FormatOptions::serifName() const {
    return serif_name_;
}

void FormatOptions::setFormatMode(format_mode_t format) {
    format_mode_ = format;
}

void FormatOptions::setImageExportFormat(image_format_t format) {
    image_format_ = format;
}

void FormatOptions::setLanguage(language_t lang) {
    language_ = lang;
}

void FormatOptions::setMonospaceName(const std::string & name) {
    monospace_name_ = name;
}

void FormatOptions::setPreserveLineBreaks(bool val) {
    preserve_line_breaks_ = val;
}

void FormatOptions::setSansSerifName(const std::string & name) {
    sans_serif_name_ = name;
}

void FormatOptions::setSerifName(const std::string & name) {
    serif_name_ = name;
}

void FormatOptions::setShowAlternatives(bool value) {
    show_alternatives_ = value;
}

void FormatOptions::setUnrecognizedChar(wchar_t ch) {
    unrecognized_char_ = ch;
}

void FormatOptions::setUnrecognizedChar(char ch) {
    unrecognized_char_ = ch;
}

void FormatOptions::writeBom(bool value) {
    write_bom_ = value;
}

bool FormatOptions::showAlternatives() const {
    return show_alternatives_;
}

wchar_t FormatOptions::unrecognizedChar() const {
    return unrecognized_char_;
}

void FormatOptions::useBold(bool val) {
    use_bold_ = val;
}

void FormatOptions::useFontSize(bool val) {
    use_font_size_ = val;
}

void FormatOptions::useItalic(bool val) {
    use_italic_ = val;
}

void FormatOptions::useStyles(bool val) {
    use_styles_ = val;
}

void FormatOptions::useUnderlined(bool val) {
    use_underlined_ = val;
}

std::ostream & operator <<(std::ostream & os, const FormatOptions & fmt) {
    using namespace std;
    os << "FormatOptions:\n";
    os << setw(25) << "   SerifName:" << fmt.serifName() << "\n";
    os << setw(25) << "   SansSerifName:" << fmt.sansSerifName() << "\n";
    os << setw(25) << "   Monospace Name:" << fmt.monospaceName() << "\n";
    os << setw(25) << "   Use bold:" << fmt.isBoldUsed() << "\n";
    os << setw(25) << "   Use Italic: " << fmt.isItalicUsed() << "\n";
    os << setw(25) << "   Use Underlined: " << fmt.isUnderlinedUsed() << "\n";
    os << setw(25) << "   Use font size: " << fmt.isFontSizeUsed() << "\n";
    os << setw(25) << "   Unrecognized char: " << (char) (fmt.unrecognizedChar()) << "\n";
    os << setw(25) << "   Line breaks: " << fmt.preserveLineBreaks() << "\n";
    os << setw(25) << "   Language: " << Language::isoName(fmt.language()) << "\n";
    os << setw(25) << "   Image export format: " << fmt.imageExportFormat() << "\n";
    return os;
}

}
