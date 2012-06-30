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

#include <iostream>

#include "clioptions.h"
#include "options.h"

namespace cf
{

CliOptions::CliOptions() :
    output_format_(FORMAT_TEXT),
    stdout_(false),
    append_(false)
{
}

void CliOptions::printOptions(std::ostream& os) {
    os << "Common options:\n";
    printOption(os, "--append", "-a",
                               "Appends output to existing document.\n"
                               "    Note: not all format supported.");
    printOption(os, "--debug-dump", "",
                               "Dumps various temporary recognition data files to current directory.");
    printOption(os, "--format FORMAT", "-f", "Select output format.\n"
                "    Type --format help to get list of supported formats.");
    printOption(os, "--help", "-h", "Prints this help message.");
    printOption(os, "--output FILENAME", "-o", "Sets output filename.");
    printOption(os, "--page-number NUMBER", "", "Specifies page number in multipage documents.");
    printOption(os, "--progress", "-p", "Prints recognition progress.");
    printOption(os, "--stdout", "", "Puts recognition result to standard output.");
    printOption(os, "--supported-image-formats", "", "Prints supported image formats.");
    printOption(os, "--verbose", "-v", "Print verbose debugging messages.");
    printOption(os, "--version", "-V", "Print program version and exit.");
}

std::ostream& operator<<(std::ostream& os, const CliOptions& opts)
{
    os << "Command line options:\n"
       << "    append:      " << opts.append() << "\n"
       << "    format:      " << OutputFormat(opts.outputFormat()).name() << "\n"
       << "    output:      '" << opts.outputFilename() << "'\n"
       << "    page-number: " << opts.inputURL().imageNumber() << "\n"
       << "    stdout:      " << opts.stdOut() << "\n"
       << "    verbose:     " << opts.stdOut() << "\n";

    return os;
}

}
