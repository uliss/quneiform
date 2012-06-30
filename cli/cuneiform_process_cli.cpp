/***************************************************************************
 *   Copyright (C) 2011 by Serge Poltavsky                                 *
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
#include <cstdlib>
#include <stdexcept>

#include "puma/processrecognitionserver.h"
#include "common/recognizeoptions.h"
#include "common/formatoptions.h"
#include "common/consoleprogresscounter.h"
#include "ced/cedpage.h"
#include "export/exporterfactory.h"
#include "optionsparser.h"

using namespace cf;

ConsoleProgressCounter * makeCounter() {
    static ConsoleProgressCounter counter;
    return &counter;
}

int main(int argc, char ** argv) {
    try {
        OptionsParser parser;
        parser.parse(argc, argv);

        CliOptions copts = parser.cliOptions();

        ProcessRecognitionServer s;

        if(copts.showProgress())
            s.setCounter(makeCounter());

        FormatOptions fopts = parser.formatOptions();
        CEDPagePtr page = s.recognize(copts.inputURL(), parser.recognizeOptions(), fopts);

        ExporterFactory::instance().setPage(page);
        ExporterFactory::instance().setFormatOptions(fopts);
        ExporterPtr exporter = ExporterFactory::instance().make(copts.outputFormat());
        exporter->setOutputPictureDir(copts.outputImageDir());

        if(copts.append()) {
            exporter->appendTo(copts.outputFilename());
        }
        else {
            if(copts.stdOut())
                exporter->exportTo(std::cout);
            else
                exporter->exportTo(copts.outputFilename());
        }
    }
    catch(OptionsParser::ExitException& e) {
        return e.exitCode();
    }
    catch(std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

