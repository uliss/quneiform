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

enum {
    USE_BOLD             = 0x0001,
    USE_ITALIC           = 0x0002,
    USE_STYLES           = 0x0004,
    USE_UNDERLINED       = 0x0008,
    USE_FONTSIZE         = 0x0010,
    PRESERVE_LINEBREAKS  = 0x0020,
    SHOW_ALTERNATIVES    = 0x0040,
    WRITE_BOM            = 0x0080,
    WRITE_META_GENERATOR = 0x0100,
    TEST_OUTPUT          = 0x0200
};

FormatOptions::FormatOptions() :
    serif_name_("Times New Roman"),
    sans_serif_name_("Arial"),
    monospace_name_("Courier New"),
    format_mode_(FormatOptions::FORMAT_ALL),
    unrecognized_char_('~'),
    language_(LANGUAGE_RUS_ENG),
    image_format_(FORMAT_UNKNOWN),
    flags_(0)
{
    useBold(true);
    useItalic(true);
    useStyles(true);
    useUnderlined(true);
    useFontSize(true);
    setPreserveLineBreaks(false);
    setShowAlternatives(false);
    writeMetaGenerator(true);

#ifdef __APPLE__
    writeBom(true);
#else
    writeBom(false);
#endif

    setTestOutput(false);
}

FormatOptions::~FormatOptions() {}

FormatOptions::format_mode_t FormatOptions::formatMode() const {
    return format_mode_;
}

image_format_t FormatOptions::imageExportFormat() const {
    return image_format_;
}

bool FormatOptions::isBoldUsed() const {
    return hasFlag(USE_BOLD);
}

bool FormatOptions::isFontSizeUsed() const {
    return hasFlag(USE_FONTSIZE);
}

bool FormatOptions::isItalicUsed() const {
    return hasFlag(USE_ITALIC);
}

bool FormatOptions::isStylesUsed() const {
    return hasFlag(USE_STYLES);
}

bool FormatOptions::isTestOutput() const {
    return hasFlag(TEST_OUTPUT);
}

bool FormatOptions::isUnderlinedUsed() const {
    return hasFlag(USE_UNDERLINED);
}

language_t FormatOptions::language() const {
    return language_;
}

std::string FormatOptions::monospaceName() const {
    return monospace_name_;
}

bool FormatOptions::preserveLineBreaks() const {
    return hasFlag(PRESERVE_LINEBREAKS);
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
    setFlag(PRESERVE_LINEBREAKS, val);
}

void FormatOptions::setSansSerifName(const std::string & name) {
    sans_serif_name_ = name;
}

void FormatOptions::setSerifName(const std::string & name) {
    serif_name_ = name;
}

void FormatOptions::setShowAlternatives(bool value) {
    setFlag(SHOW_ALTERNATIVES, value);
}

void FormatOptions::setTestOutput(bool value) {
    setFlag(TEST_OUTPUT, value);
}

void FormatOptions::setUnrecognizedChar(wchar_t ch) {
    unrecognized_char_ = ch;
}

void FormatOptions::setUnrecognizedChar(char ch) {
    unrecognized_char_ = ch;
}

bool FormatOptions::writeBom() const {
    return hasFlag(WRITE_BOM);
}

void FormatOptions::writeBom(bool value) {
    setFlag(WRITE_BOM, value);
}

bool FormatOptions::writeMetaGenerator() const {
    return hasFlag(WRITE_META_GENERATOR);
}

void FormatOptions::writeMetaGenerator(bool value) {
    setFlag(WRITE_META_GENERATOR, value);
}

bool FormatOptions::showAlternatives() const {
    return hasFlag(SHOW_ALTERNATIVES);
}

wchar_t FormatOptions::unrecognizedChar() const {
    return unrecognized_char_;
}

void FormatOptions::useBold(bool val) {
    setFlag(USE_BOLD, val);
}

void FormatOptions::useFontSize(bool val) {
    setFlag(USE_FONTSIZE, val);
}

void FormatOptions::useItalic(bool val) {
    setFlag(USE_ITALIC, val);
}

void FormatOptions::useStyles(bool val) {
    setFlag(USE_STYLES, val);
}

void FormatOptions::useUnderlined(bool val) {
    setFlag(USE_UNDERLINED, val);
}

template<class T>
static void OPT(std::ostream& os, const std::string& name, const T& value) {
    static const int FIELD_WIDTH = 25;
    static const std::string INDENT(4, ' ');
    os << INDENT << std::left << std::setw(FIELD_WIDTH) << name + ": " << value << "\n";
 }

std::ostream& operator <<(std::ostream& os, const FormatOptions& fmt) {
    using namespace std;

    os << "FormatOptions:\n";
    os << boolalpha;

    OPT(os, "Serif",          fmt.serifName());
    OPT(os, "Sans-Serif",     fmt.sansSerifName());
    OPT(os, "Monospace",      fmt.monospaceName());
    OPT(os, "Use bold",       fmt.isBoldUsed());
    OPT(os, "Use italic",     fmt.isItalicUsed());
    OPT(os, "Use underlined", fmt.isUnderlinedUsed());
    OPT(os, "Use font size",  fmt.isFontSizeUsed());
    OPT(os, "Unrecognized char", (char)fmt.unrecognizedChar());
    OPT(os, "Line breaks",    fmt.preserveLineBreaks());
    OPT(os, "Show alternatives", fmt.showAlternatives());
    OPT(os, "Language",       Language::isoName(fmt.language()));
    OPT(os, "Image export format", imageFormatToString(fmt.imageExportFormat()));
    OPT(os, "Write BOM",      fmt.writeBom());
    OPT(os, "Write meta generator", fmt.writeMetaGenerator());
    OPT(os, "Test output",    fmt.isTestOutput());

    os << noboolalpha;
    return os;
}

}
