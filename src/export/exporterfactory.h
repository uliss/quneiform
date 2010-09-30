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

#ifndef EXPORTERFACTORY_H_
#define EXPORTERFACTORY_H_

#include "cttypes.h"
#include "common/singleton.h"
#include "common/exception.h"
#include "common/outputformat.h"
#include "common/formatoptions.h"
#include "exporter.h"
#include "globus.h"

namespace cf
{

class CEDPage;

class CLA_EXPO ExporterFactoryImpl
{
    public:
        ExporterFactoryImpl();

        /**
         * Returns pointer to exporter for given format
         */
        ExporterPtr make(format_t format);

        /**
         * Sets export options
         */
        void setFormatOptions(const FormatOptions& opts);

        /**
         * Sets pointer to document
         */
        void setPage(CEDPage * page);

        typedef RuntimeExceptionImpl<ExporterFactoryImpl> Exception;
    private:
        CEDPage * page_;
        FormatOptions format_options_;
};

typedef Singleton<ExporterFactoryImpl> ExporterFactory;

}

#endif /* EXPORTERFACTORY_H_ */
