/***************************************************************************
 *   Copyright (C) 2010 by Serge Poltavsky                                 *
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
#include "imageexporter.h"

using namespace std;

namespace CIF
{

ImageExporter::~ImageExporter() {

}

string ImageExporter::outputFilename() const {
    return output_filename_;
}

void ImageExporter::save(void * data, size_t dataSize, const string& path) {

    ofstream file;
    file.open(path.c_str(), ios::out | ios::binary | ios::trunc);
    if (!file)
        throw Exception("[ImageExporter::save] failed to: " + path);

    setOutputFilename(path);
    save(data, dataSize, file);
}

void ImageExporter::setOutputFilename(const std::string& filename) {
    output_filename_ = filename;
}

}
