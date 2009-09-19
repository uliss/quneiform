/***************************************************************************
 *   Copyright (C) 2008 by Сергей Полтавский   *
 *   poltava@gentoo   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#ifndef CIFTEXTEXPORTER_H
#define CIFTEXTEXPORTER_H

#include <iostream>
#include "exporter.h"

namespace CIF
{

/**
 @author Serj Poltavskiy <serge.uliss at gmail.com>
*/
class TextExporter : public Exporter
{
    public:
        TextExporter(std::ostream& os);
        ~TextExporter();

        void exportCell(const Cell& cell) const;
        void exportDocument(const Document& doc) const;
        void exportImage(const Image& img) const;
        void exportPage(const Page& page) const;
        void exportParagraph(const Paragraph& par) const;
        void exportRaw(const Raw& raw) const;
        void exportString(const String& string) const;
        void exportTable(const Table& table) const;
    private:
        std::ostream& m_os;
};

}

#endif
