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

#ifndef IQFEXPORTER_H
#define IQFEXPORTER_H

#include <QSharedPointer>
#include <stdexcept>

#include "exportsettings.h"

class Page;

class IQfExporter {
public:
    IQfExporter(const ExportSettings& s) : settings_(s) {}
    virtual ~IQfExporter() {}
    virtual void exportTo(Page * p, const QString& fname) = 0;
    ExportSettings settings() const { return settings_; }
private:
    ExportSettings settings_;
};

struct ExporterException : public std::runtime_error {
    ExporterException(const QString& msg) : std::runtime_error(msg.toLocal8Bit().data()) {
    }
};

typedef QSharedPointer<IQfExporter> ExporterPtr;

#endif // IQFEXPORTER_H
