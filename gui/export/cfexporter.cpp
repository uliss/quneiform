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

#include <QDebug>

#include "export/exporterfactory.h"
#include "cfexporter.h"
#include "page.h"
#include "ced/cedpage.h"

CfExporter::CfExporter(const ExportSettings& s)
    : IQfExporter(s) {
}

void CfExporter::exportTo(Page * p, const QString& fname) {
    Q_CHECK_PTR(p);

    qDebug() << Q_FUNC_INFO << fname;

    cf::FormatOptions opts;
    p->formatSettings().exportTo(opts);

    cf::ExporterFactory::instance().setPage(cf::CEDPagePtr(p->cedPage()));
    cf::ExporterFactory::instance().setFormatOptions(opts);
    cf::ExporterPtr e = cf::ExporterFactory::instance().make(
                static_cast<cf::format_t>(settings().cfFormatType()));
//    e->setOutputPictureDir(output_image_dir_);

    e->exportTo(fname.toLocal8Bit().data());
}
