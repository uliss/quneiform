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

#ifndef ODTEXPORTER_H_
#define ODTEXPORTER_H_

#include <vector>
#include <string>
#include <map>
#include <boost/shared_ptr.hpp>

#include "xmlexporter.h"
#include "zipcpp.h"

namespace cf
{

class OdfStyleExporter;

class OdfExporter: public XmlExporter
{
    public:
        OdfExporter(CEDPage * page, const FormatOptions& opts);

        void exportTo(const std::string& fname);
        void exportTo(std::ostream& os);
    protected:
        void writeCharacterBegin(CEDChar& chr);
        void writeLineBreak();
        void writeMeta(std::ostream& os);
        void writeMetaDate(std::ostream& os);
        void writeMetaGenerator(std::ostream& os);
        void writeMetaStatistics(std::ostream& os);
        void writePageBegin(CEDPage& page);
        void writePageEnd(CEDPage& page);
        void writeParagraphBegin(CEDParagraph& par);
        void writeParagraphEnd(CEDParagraph& par);
        void writePicture(CEDPicture& picture);
        void writeSectionBegin(CEDSection& sect);
        void writeSectionEnd(CEDSection& sect);
        void writeTableBegin(CEDTable& table);
        void writeTableEnd(CEDTable& table);
    private:
        void addOdfContent();
        void addOdfManifest();
        void addOdfManifestFile(const std::string& path, const std::string& type);
        void addOdfMeta();
        void addOdfMime();
        void addOdfSettings();
        void addOdfStyles();
        void makePicturesDir();
        void odfOpen(const std::string& fname);
        void odfSave();
        void odfWrite(const std::string& fname, const std::string& data);
        void setCommonOdfNamespaces(Attributes& attrs) const;
        void writeOdfAutomaticStyles();
    private:
        typedef std::map<std::string, std::string> ManifestList;
    private:
        ZipCpp zip_;
        std::string fname_;
        ManifestList files_;
        boost::shared_ptr<OdfStyleExporter> style_exporter_;
        size_t prev_char_style_hash_;
        bool style_span_opened_;
};

}

#endif /* ODTEXPORTER_H_ */
