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

#include <fstream>
#include <cstdlib>

#include "exporter.h"
#include "nullimageexporter.h"
#include "common/filesystem.h" // for removeFileExt
#include "common/language.h"

using namespace std;

namespace cf
{

const char * DEFAULT_PICTURE_DIR = "cuneiform-out_files";

Exporter::Exporter(CEDPagePtr page, const FormatOptions& opts) :
    CEDExporter(page),
    format_options_(opts) {
    //autoDetectOutputEncoding();
    image_exporter_.reset(new NullImageExporter);
    setEncodings();
}

void Exporter::autoDetectOutputEncoding() {
    char * lc = ::getenv("LC_ALL");
    std::string locale;
    if (!lc)
        lc = ::getenv("LANG");
    if (lc)
        locale = lc;
    if (locale.empty()) {
        output_encoding_ = "UTF-8";
        return;
    }
    size_t dot_pos = locale.find('.', 0);
    // TODO check this. uliss
    if (dot_pos == std::string::npos) {
        output_encoding_ = "UTF-8";
        return;
    }
    output_encoding_ = locale.substr(dot_pos + 1);
}

void Exporter::appendTo(const std::string& /*filename*/) {
    throw Exception("[Exporter::appendTo] method not implemented for this format");
}

bool Exporter::isCharsetConversion() const {
    if (output_encoding_.empty() || input_encoding_.empty())
        return false;
    return output_encoding_ != input_encoding_;
}

void Exporter::exportTo(const std::string& filename) {
    ofstream f;
    f.open(filename.c_str(), std::ios::out | std::ios::binary | std::ios::trunc);
    if (!f)
        throw Exception("Can't write to file: " + filename);
    setOutputFilename(filename);
    doExport(f);
}

void Exporter::exportTo(std::ostream& os) {
    if (os.fail())
        throw Exception("[GenericExporter] invalid output stream given");

    doExport(os);
}

const FormatOptions& Exporter::formatOptions() const {
    return format_options_;
}

FormatOptions& Exporter::formatOptions() {
    return format_options_;
}

ImageExporterPtr Exporter::imageExporter() {
    return image_exporter_;
}

std::string Exporter::inputEncoding() const {
    return input_encoding_;
}

std::string Exporter::makeOutputPictureDir() const {
    if (output_picture_dir_.empty()) {
        if (output_filename_.empty())
            return DEFAULT_PICTURE_DIR;

        return fs::removeFileExtension(outputFilename()) + "_files";
    }

    return output_picture_dir_;
}

std::string Exporter::outputEncoding() const {
    return output_encoding_;
}

std::string Exporter::outputFilename() const {
    return output_filename_;
}

void Exporter::setEncodings() {
    setInputEncoding(Language(formatOptions().language()).encoding());
    setOutputEncoding("utf-8");
}

void Exporter::setFormatOptions(const FormatOptions& opts) {
    format_options_ = opts;
    image_exporter_->setFormat(format_options_.imageExportFormat());
}

void Exporter::setInputEncoding(const std::string& enc) {
    input_encoding_ = enc;
}

void Exporter::setImageExporter(ImageExporterPtr exporter) {
    image_exporter_ = exporter;
    image_exporter_->setFormat(format_options_.imageExportFormat());
}

void Exporter::setOutputEncoding(const std::string& enc) {
    output_encoding_ = enc;
}

void Exporter::setOutputFilename(const std::string& filename) {
    output_filename_ = filename;
}

void Exporter::setOutputPictureDir(const std::string& path) {
    output_picture_dir_ = path;
}

}
