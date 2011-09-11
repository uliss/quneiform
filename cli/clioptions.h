/***************************************************************************
 *   Copyright (C) 2011 by Serge Poltavski                                 *
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

#ifndef CLIOPTIONS_H
#define CLIOPTIONS_H

#include <iosfwd>
#include <string>

#include "common/outputformat.h"

namespace cf {

class CliOptions
{
public:
    CliOptions();
    bool append() const { return append_; }
    std::string inputFilename() const { return input_; }
    std::string outputFilename() const { return output_; }
    format_t outputFormat() const { return output_format_; }
    std::string outputImageDir() const { return output_image_dir_; }
    void setAppend(bool value) { append_ = value; }
    void setInputFilename(const std::string& fname) { input_ = fname; }
    void setOutputFilename(const std::string& fname) { output_ = fname; }
    void setOutputFormat(format_t fmt) { output_format_ = fmt; }
    void setOutputImageDir(const std::string& dir) { output_image_dir_ = dir; }
    void setShowProgress(bool value) { progress_ = value; }
    void setStdOut(bool value) { stdout_ = value; }
    bool showProgress() const { return progress_; }
    bool stdOut() const { return stdout_; }
public:
    static void printOptions(std::ostream& os);
private:
    std::string input_;
    std::string output_;
    std::string output_image_dir_;
    format_t output_format_;
    bool stdout_;
    bool append_;
    bool progress_;
};

}

#endif // CLIOPTIONS_H
