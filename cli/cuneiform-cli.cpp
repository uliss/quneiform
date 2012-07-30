/***************************************************************************
 *   Copyright (C) 2009-2011 by Serge Poltavski                            *
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

#include "optionsparser.h"
#include "puma/localrecognitionserver.h"
#include "common/percentcounter.h"
#include "common/consoleprogresscounter.h"
#include "export/exporterfactory.h"
#include "common/log.h"

using namespace cf;

static bool setupLogger()
{
    Logger::config().enableRuntimeConfig(MODULES_ALL);
    return true;
}

static bool lsetup = setupLogger();

ConsoleProgressCounter * makeCounter() {
    static ConsoleProgressCounter counter;
    return &counter;
}

int main(int argc, char **argv)
{
    try {
        OptionsParser parser;
        parser.parse(argc, argv);

        CliOptions cli_opts = parser.cliOptions();
        FormatOptions fopts = parser.formatOptions();
        fopts.setImageExportFormat(FORMAT_PNG);

        LocalRecognitionServer server;

        if(cli_opts.showProgress())
            server.setCounter(makeCounter());

        server.setTextDebug(cli_opts.outputFormat() == FORMAT_DEBUG);
        CEDPagePtr page = server.recognize(cli_opts.inputURL(), parser.recognizeOptions(), fopts);

        ExporterFactory::instance().setPage(page);
        ExporterFactory::instance().setFormatOptions(fopts);
        ExporterPtr exporter = ExporterFactory::instance().make(cli_opts.outputFormat());

        exporter->setOutputPictureDir(cli_opts.outputImageDir());

        if(cli_opts.append()) {
            exporter->appendTo(cli_opts.outputFilename());
        }
        else {
            if(cli_opts.stdOut())
                exporter->exportTo(std::cout);
            else
                exporter->exportTo(cli_opts.outputFilename());
        }
    }
    catch(OptionsParser::ExitException& e) {
        return e.exitCode();
    }
    catch (std::runtime_error& e) {
        cfError() << e.what();
        return EXIT_FAILURE;
    }
}
