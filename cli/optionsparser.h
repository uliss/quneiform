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

#ifndef OPTIONSPARSER_H
#define OPTIONSPARSER_H

#include <iosfwd>
#include <stdexcept>

#include "clioptions.h"
#include "common/formatoptions.h"
#include "common/recognizeoptions.h"
#include "common/binarizeoptions.h"

namespace cf
{

class OptionsParser
{
public:
    class ExitException : public std::runtime_error {
    public:
        ExitException(int exitCode, const std::string& msg = "") :
            std::runtime_error(msg), exit_code_(exitCode) {}
        int exitCode() const { return exit_code_; }
    private:
        int exit_code_;
    };
public:
    OptionsParser();

    /**
      * Parses given argument list
      * @throw ExitException on parse error
      */
    void parse(int argc, char **argv);

    /**
     * Returns binarization options
     */
    BinarizeOptions binarizeOptions() const;

    /**
      * Returns command line options
      */
    CliOptions cliOptions() const;

    /**
      * Returns format options
      */
    FormatOptions formatOptions() const;

    /**
      * Returns recognition options
      */
    RecognizeOptions recognizeOptions() const;
private:
    static void printFormatOptions(std::ostream& os);
    static void printRecognizeOptions(std::ostream& os);
private:
    void getoptParse(int argc, char **argv);
    void print(std::ostream& os);
    void printConfig(std::ostream& os);
    void printUsage(const char * program);
    void updateFormatOptions();
    void updateRecognizeOptions();
    void updateCliOptions();
    void updateDebugOptions();
    void updateBinarizeOptions();
private:
    FormatOptions format_opts_;
    RecognizeOptions recognize_opts_;
    CliOptions cli_opts_;
    BinarizeOptions bin_opts_;
};

}

#endif // OPTIONSPARSER_H
