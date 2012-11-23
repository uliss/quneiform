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

#ifndef CFEXPORTER_H
#define CFEXPORTER_H

#include "iqfexporter.h"
#include "formatsettings.h"
#include "ced/cedpageptr.h"

class CfExporter : public IQfExporter
{
public:
    CfExporter(const ExportSettings& s);
    void exportTo(Page * p, const QString& fname);
    void exportCEDPage(cf::CEDPagePtr ptr, const FormatSettings& settings, const QString& fullPath);
};

#endif // CFEXPORTER_H
